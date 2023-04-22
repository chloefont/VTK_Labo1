#include "PhongShader.h"
#include "TGAColor.h"

Vec3f PhongShader::vertex(size_t face, size_t vert) {
   PhongNormals::vertex(model, face, vert);
   return Shader::vertex(face,vert);
}

bool PhongShader::fragment(Vec3f bary, TGAColor &color) const {

   TGAColor dColor = diffuseColor;
   auto n = PhongNormals::fragment(light, bary, dColor);

   VecH projectionInv = Matrix4x4(projection).inverse() * VecH(bary);
   VecH viewInv = Matrix4x4(view).inverse() * projectionInv;
   VecH world = Matrix4x4(viewport).inverse() * viewInv;

   Vec3f screen = lightViewport * projection * view * VecH(world);

   TGAColor sColor = specularColor;
   PhongNormals::specular(light,camera,n,specularPower,sColor);

   color = ambientColor * ambientWeight + dColor * diffuseWeight + sColor * specularWeight;

    if (zbufferShadow[screen.x + screen.y * width] < screen.z - 10) {
        color = color * 0.5f;
        //color = TGAColor(255,0,0,255);
    }
   return true;
}