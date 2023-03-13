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

#include "FlatShader.h"
#include "FlatTextureShader.h"
#include "GouraudShader.h"
#include "GouraudTextureShader.h"

using namespace std;

int main() {

   string modelFileName = "../../obj/african_head.obj";
   Model model(modelFileName.c_str());

   string textureFileName = "../../obj/african_head_diffuse.tga";
   TGAImage texture;
   texture.read_tga_file(textureFileName.c_str());
   texture.flip_vertically();

   Camera camera{ .eye = {1,1,2}, .center = {0,0,0}, .up = {0,1,0} };

   FlatShader flatShader;
   flatShader.color = TGAColor(160,255,255,255);

   FlatTextureShader flatTextureShader;
   flatTextureShader.texture = &texture;

   GouraudShader gouraudShader;
   gouraudShader.color = TGAColor(255,255,160,255);

   GouraudTextureShader gouraudTextureShader;
   gouraudTextureShader.texture = &texture;

   vector<Shader*> shaders = {
           &gouraudShader,
           &gouraudTextureShader,
           &flatShader,
           &flatTextureShader };

   int const imageW = 1000, imageH = 1000;
   int const gridX = 2, gridY = 2;
   int const viewW = imageW / gridX, viewH = imageH / gridY;
   TGAImage image(imageW, imageH, TGAImage::RGB);
   vector<float> zbuffer(imageW * imageH, std::numeric_limits<float>::lowest());

   for(size_t i = 0; i < shaders.size(); ++i) {
      fill(zbuffer.begin(),zbuffer.end(),std::numeric_limits<float>::lowest());

      int x = int(i) % gridX;
      int y = int(i) / gridX;

      auto& shader = shaders[i];
      shader->model = &model;
      shader->view = camera.view();
      shader->projection = camera.projection();
      shader->viewport = make_viewport(x * viewW, y * viewH, viewW, viewH);
      shader->light = {0, 0, 1};

      for (size_t f = 0; f < size_t(model.nfaces()); ++f) {
         Vec3f screen[3];
         for (size_t v = 0; v < 3; ++v) {
            screen[v] = shader->vertex(f, v);
         }
         triangle(screen, *shader, zbuffer.data(), image);
      }
   }

   image.flip_vertically();
   string filename = "shaders.tga";
   image.write_tga_file(filename.data());
}

