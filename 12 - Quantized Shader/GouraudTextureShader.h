#ifndef CPPRENDERER_GOURAUDTEXTURESHADER_H
#define CPPRENDERER_GOURAUDTEXTURESHADER_H

#include "Shader.h"

struct GouraudTextureShader
        : public Shader,
          public TextureShaderBase,
          public GouraudShaderBase {
   virtual Vec3f vertex(size_t face, size_t vert);

   virtual bool fragment(Vec3f bary, TGAColor& color);
};


#endif //CPPRENDERER_GOURAUDTEXTURESHADER_H