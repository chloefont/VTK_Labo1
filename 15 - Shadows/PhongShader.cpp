#include "PhongShader.h"
#include "TGAColor.h"

Vec3f PhongShader::vertex(size_t face, size_t vert) {
   PhongNormals::vertex(model, face, vert);
    Vec3f world = model->vert(size_t(model->face(face)[vert]));
    Vec3f result = toLightScreenTransform * VecH(world);
    screen[vert] = result;
   return Shader::vertex(face,vert);
}

bool PhongShader::fragment(Vec3f bary, TGAColor &color) const {

   TGAColor dColor = diffuseColor;
   auto n = PhongNormals::fragment(light, bary, dColor);

   // Find the point in the light screen space
   Vec3f lightVertex = screen[0] * bary.x + screen[1] * bary.y + screen[2] * bary.z;
   lightVertex.z = bary.x * screen[0].z + bary.y * screen[1].z + bary.z * screen[2].z;


   TGAColor sColor = specularColor;
   PhongNormals::specular(light,camera,n,specularPower,sColor);

   int x = int(round(lightVertex.x));
   int y = int(round(lightVertex.y));

   x = x > width ? width : x;
   y = y > height ? height : y;

   const float delta = 15;

   if (zbufferShadow[x + y * width] > (lightVertex.z + delta)) {
       color = ambientColor * ambientWeight + dColor * diffuseWeight * 0.5f;
   } else {
       color = ambientColor * ambientWeight + dColor * diffuseWeight + sColor * specularWeight;
   }
   return true;
}