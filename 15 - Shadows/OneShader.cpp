#include <vector>
#include <cmath>
#include <algorithm>
#include <utility>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

#include "Camera.h"
#include "Viewport.h"
#include "Triangle.h"

#include "PhongShader.h"

#include <bits/stdc++.h>
#include<tuple>

using namespace std;

std::array<Vec2i,2> boite_englobante2(const TGAImage& image, Vec3f t[3])
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

Vec3f barycentriques2(Vec3f* t, Vec3f p) {
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

int main() {

   string modelName = "../../obj/10_Capricorn";
   string modelFileName = modelName + ".obj";

   Model model(modelFileName.c_str());
   model.normalize();

   Camera camera{.eye = {5, 2, 5}, .center = {0, 0, 0}, .up = {0, 1, 0}};

   PhongShader phongShader;
   phongShader.model = &model;
   phongShader.ambientColor = TGAColor(255, 255, 255, 255);
   phongShader.diffuseColor = TGAColor(255, 128, 128, 255);
   phongShader.specularColor = TGAColor(255, 255, 255, 255);
   phongShader.ambientWeight = 0.2f;
   phongShader.diffuseWeight = 0.8f;
   phongShader.specularWeight = 0.5f;
   phongShader.specularPower = 10.f;
   phongShader.view = camera.view();
   phongShader.projection = camera.projection();
   phongShader.light = {0, 0, 1};
   phongShader.camera = camera.direction();
   phongShader.backfaceCulling = false;
   phongShader.frontfaceCulling = false;
   phongShader.light = {0, 1, 1};
   phongShader.light.normalize();

   int const imageW = 800, imageH = 800;
   TGAImage image(imageW, imageH, TGAImage::RGB);

   int const viewW = imageW, viewH = imageH;
   phongShader.viewport = make_viewport(0, 0, viewW, viewH, 0.7);

   vector<float> zbuffer(imageW * imageH, std::numeric_limits<float>::lowest());
   fill(zbuffer.begin(), zbuffer.end(), std::numeric_limits<float>::lowest());

   //-- mon code

    vector<tuple <int, float>> zbufferShades(imageW * imageH);
    fill(zbufferShades.begin(), zbufferShades.end(), make_tuple(std::numeric_limits<int>::lowest(), std::numeric_limits<float>::lowest()));

    Camera camera2{.eye = {3, 10, 10}, .center = {0, 0, 0}, .up = {0, 1, 0}};

    PhongShader phongShader2;
    phongShader2.model = &model;
    phongShader2.ambientColor = TGAColor(255, 255, 255, 255);
    phongShader2.diffuseColor = TGAColor(255, 128, 128, 255);
    phongShader2.specularColor = TGAColor(255, 255, 255, 255);
    phongShader2.ambientWeight = 0.2f;
    phongShader2.diffuseWeight = 0.8f;
    phongShader2.specularWeight = 0.5f;
    phongShader2.specularPower = 10.f;
    phongShader2.view = camera2.view();
    phongShader2.projection = camera2.projection();
    phongShader2.light = {0, 0, 1};
    phongShader2.camera = camera2.direction();
    phongShader2.backfaceCulling = false;
    phongShader2.frontfaceCulling = false;
    phongShader2.light = {0, 1, 1};
    phongShader2.light.normalize();

    for (size_t f = 0; f < size_t(model.nfaces()); ++f) {
        Vec3f screen[3];
        for (size_t v = 0; v < 3; ++v) {
            screen[v] = phongShader.vertex(f, v);
        }



        auto bbox = boite_englobante2(image, screen);
        for(int y = bbox[0].y; y <= bbox[1].y; ++y) {
            for(int x = bbox[0].x; x <= bbox[1].x; ++x) {
                Vec3f p { float(x), float(y), 0};
                Vec3f b = barycentriques2(screen,p);
                if ( b.x >= 0 and b.y >= 0 and b.z >= 0 ) {
                    p.z = b.x*screen[0].z + b.y*screen[1].z + b.z*screen[2].z;
                    if(get<1>(zbufferShades[x + image.get_width() * y]) < p.z) {
                        TGAColor color;
                        if( phongShader2.fragment(b, color) ) {
                            zbufferShades[x + image.get_width() * y] = make_tuple(f, p.z);
                            image.set(x,y,color);
                        }
                    }
                }
            }
        }


    }



    // -- mon code

   for (size_t f = 0; f < size_t(model.nfaces()); ++f) {
      Vec3f screen[3];
      for (size_t v = 0; v < 3; ++v) {
         screen[v] = phongShader.vertex(f, v);
      }
      triangle(screen, phongShader, zbuffer.data(), zbufferShades.data(), image, f);
   }

   string filename = "OneShader.tga";
   image.write_tga_file(filename.data());
}
