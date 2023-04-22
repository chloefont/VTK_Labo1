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
   std::vector<float> zbufferShadow;
   Matrix<4, 4> lightViewport;
   virtual Vec3f vertex(size_t face, size_t vert);
   virtual bool fragment(Vec3f bary, TGAColor &color) const;
};

#endif //CPPRENDERER_PHONGSHADER_H
