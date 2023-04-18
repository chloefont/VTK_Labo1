#ifndef CPPRENDERER_TRIANGLE_H
#define CPPRENDERER_TRIANGLE_H

#include "geometry.h"
#include "tgaimage.h"
#include "Shader.h"
#include<tuple>

void triangle(Vec3f screen[3], Shader& shader, float* zbuffer, std::tuple<int, float>* zbufferShades, TGAImage &image, size_t id);

#endif //CPPRENDERER_TRIANGLE_H
