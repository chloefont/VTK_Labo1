//
// Created by cfont on 22.04.2023.
//

#include "ShadowShader.h"

Vec3f ShadowShader::vertex(size_t face, size_t vert) {
    PhongNormals::vertex(model, face, vert);
    return Shader::vertex(face, vert);
}

bool ShadowShader::fragment(Vec3f bary, TGAColor &color) const {
    return true;
}
