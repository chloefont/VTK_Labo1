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

#include "PhongNormalTextureShader.h"

using namespace std;

int main() {

   string modelFileName = "../../obj/african_head.obj";
   Model model(modelFileName.c_str());

   string textureFileName = "../../obj/african_head_diffuse.tga";
   TGAImage texture;
   texture.read_tga_file(textureFileName.c_str());
   texture.flip_vertically();

   string normalsFileName = "../../obj/african_head_nm.tga";
   TGAImage normals;
   normals.read_tga_file(normalsFileName.c_str());
   normals.flip_vertically();

   Camera camera{.eye = {1, 1, 2}, .center = {0, 0, 0}, .up = {0, 1, 0}};

   PhongNormalTextureShader phongShader;
   phongShader.model = &model;
   phongShader.textureImage = &texture;
   phongShader.normalsImage = &normals;
   phongShader.ambientWeight = 0.2f;
   phongShader.diffuseWeight = 1.f;
   phongShader.specularWeight = 1.f;
   phongShader.specularPower = 10.f;
   phongShader.view = camera.view();
   phongShader.projection = camera.projection();
   phongShader.light = {0,0,1};
   phongShader.camera = camera.direction();
   phongShader.backfaceCulling = true;
   phongShader.frontfaceCulling = false;

   int const nFrames = 100;
   for(int i = 0; i < nFrames; ++i) {
      float angle = i * 2 * M_PI / nFrames;

      int const imageW = 800, imageH = 800;
      TGAImage image(imageW, imageH, TGAImage::RGB);

      int const viewW = imageW, viewH = imageH;
      phongShader.viewport = make_viewport(0, 0, viewW, viewH);

      vector<float> zbuffer(imageW * imageH, std::numeric_limits<float>::lowest());
      fill(zbuffer.begin(), zbuffer.end(), std::numeric_limits<float>::lowest());

      phongShader.light = {sin(angle), 0, cos(angle)};

      for (size_t f = 0; f < size_t(model.nfaces()); ++f) {
         Vec3f screen[3];
         for (size_t v = 0; v < 3; ++v) {
            screen[v] = phongShader.vertex(f, v);
         }
         triangle(screen, phongShader, zbuffer.data(), image);
      }

      image.flip_vertically();
      string filename = "rotatingLight"+to_string(1000+i)+".tga";
      image.write_tga_file(filename.data());
   }
}

