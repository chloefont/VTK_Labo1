#include "Triangle.h"

std::array<Vec2i,2> boite_englobante(const TGAImage& image, Vec3f t[3])
{
   const Vec2i imageMin { 0, 0 };
   const Vec2i imageMax { image.get_width()-1, image.get_height() - 1 };

   std::array<Vec2i,2> bbox { imageMax, imageMin } ;
   for(int i = 0; i < 3; ++i) {
      Vec3f& v = t[i];
      bbox[0].x = std::min(bbox[0].x, int(v.x));
      bbox[0].y = std::min(bbox[0].y, int(v.y));
      bbox[1].x = std::max(bbox[1].x, int(v.x));
      bbox[1].y = std::max(bbox[1].y, int(v.y));
   }

   bbox[0].x = std::max(bbox[0].x, imageMin.x);
   bbox[0].y = std::max(bbox[0].y, imageMin.y);

   bbox[1].x = std::min(bbox[1].x, imageMax.x);
   bbox[1].y = std::min(bbox[1].y, imageMax.y);

   return bbox;
}

Vec3f barycentriques(Vec3f* t, Vec3f p) {
   Vec3f x { (t[1].x - t[0].x),
             (t[2].x - t[0].x),
             (t[0].x - p.x) };
   Vec3f y { (t[1].y - t[0].y),
             (t[2].y - t[0].y),
             (t[0].y - p.y)};
   Vec3f u = x^y;
   if(abs(u.z) < 0.01)
      return {-1,1,1};
   else
      return { 1.f - (u.x+u.y)/u.z,u.x/u.z, u.y/u.z };
}

bool est_dans_le_triangle(Vec3f* t, Vec3f pt) {
   Vec3f b = barycentriques(t,pt);
   return ( b.x >= 0 and b.y >= 0 and b.z >= 0 );
}

void triangle(Vec3f screen[3], Shader& shader, float* zbuffer, std::tuple<int, float>* zbufferShades, TGAImage &image, size_t id) {
   auto bbox = boite_englobante(image, screen);

//#pragma omp parallel for
   for(int y = bbox[0].y; y <= bbox[1].y; ++y) {
      for(int x = bbox[0].x; x <= bbox[1].x; ++x) {
         Vec3f p { float(x), float(y), 0};
         Vec3f b = barycentriques(screen,p);
         if ( b.x >= 0 and b.y >= 0 and b.z >= 0 ) {
            p.z = b.x*screen[0].z + b.y*screen[1].z + b.z*screen[2].z;
            if(zbuffer[x + image.get_width() * y] < p.z) {
               TGAColor color;
               if( shader.fragment(b, color) ) {
                  zbuffer[x + image.get_width() * y] = p.z;

                  if (get<0>(zbufferShades[x + image.get_width() * y]) == id) {
                     //zbufferShades[x + image.get_width() * y] = p.z;
                     color = TGAColor(255,255,255, 255);
                  }

//                  if (zbufferShades[x + image.get_width() * y] < p.z) {
//                     //zbufferShades[x + image.get_width() * y] = p.z;
//                     color = TGAColor(255,255,255, 255);
//                  }
                  image.set(x,y,color);
               }
            }
         }
      }
   }
}
