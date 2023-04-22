//
// Created by cfont on 22.04.2023.
//

#ifndef CPPRENDERER_SHADOWSHADER_H
#define CPPRENDERER_SHADOWSHADER_H

#include "Shader.h"

struct ShadowShader
        : public Shader,
          public Culler,
          public PhongNormals {
    virtual Vec3f vertex(size_t face, size_t vert);
    virtual bool fragment(Vec3f bary, TGAColor &color) const;
};

#endif //CPPRENDERER_SHADOWSHADER_H
