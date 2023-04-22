#ifndef CPPRENDERER_PHONGSHADER_H
#define CPPRENDERER_PHONGSHADER_H

#include "Shader.h"

struct PhongShader
        : public Shader,
          public Culler,
          public PhongNormals {
   TGAColor ambientColor, diffuseColor, specularColor;
   float ambientWeight = 0.3f, diffuseWeight = 0.7f, specularWeight = 0.f;
   float specularPower = 10.f;
   int width;
   int height;
   std::vector<Vec3f> screen;
   Matrix4x4 transformation;
   Vec3f currentPoint;

   std::vector<float> zbufferShadow;
   virtual Vec3f vertex(size_t face, size_t vert);
   virtual bool fragment(Vec3f bary, TGAColor &color) const;
};

#endif //CPPRENDERER_PHONGSHADER_H
