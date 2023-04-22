#include "PhongShader.h"
#include "TGAColor.h"

Vec3f PhongShader::vertex(size_t face, size_t vert) {
   PhongNormals::vertex(model, face, vert);
    Vec3f world = model->vert(size_t(model->face(face)[vert]));
    Vec3f result = transformation * VecH(world);
    screen[vert] = result;
   return Shader::vertex(face,vert);
}

bool PhongShader::fragment(Vec3f bary, TGAColor &color) const {

   TGAColor dColor = diffuseColor;
   auto n = PhongNormals::fragment(light, bary, dColor);

   // find point with bary and currentScreen
   Vec3f p = screen[0] * bary.x + screen[1] * bary.y + screen[2] * bary.z;
   p.z = bary.x*screen[0].z + bary.y*screen[1].z + bary.z*screen[2].z;


   TGAColor sColor = specularColor;
   PhongNormals::specular(light,camera,n,specularPower,sColor);



   int x = round(p.x);
   int y = round(p.y);

   x = x > width ? width : x;
   y = y > height ? height : y;

   float delta = 15;

    if (zbufferShadow[x + y * width] > (p.z + delta)) {
        color = ambientColor * ambientWeight + dColor * diffuseWeight * 0.5f;
        //color = TGAColor(255,0,0,255);
    } else {
        color = ambientColor * ambientWeight + dColor * diffuseWeight + sColor * specularWeight;
    }
   return true;
}