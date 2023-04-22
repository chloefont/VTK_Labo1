#include <vector>
#include <cmath>
#include <string>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

#include "Camera.h"
#include "Viewport.h"
#include "Triangle.h"

#include "PhongShader.h"
#include "ShadowShader.h"

using namespace std;

int main() {

    string modelName = "../../obj/10_Capricorn";
    string modelFileName = modelName + ".obj";

    Model model(modelFileName.c_str());
    model.normalize();

    int const imageW = 800, imageH = 800;
    int const viewW = imageW, viewH = imageH;

    // Calculate zbuffer from light point of view
    Camera cameraLight{.eye = {3, 10, 10}, .center = {0, 0, 0}, .up = {0, 1, 0}};

    ShadowShader shadowShader;

    shadowShader.model = &model;
    shadowShader.view = cameraLight.view();
    shadowShader.projection = cameraLight.projection();
    shadowShader.light = {0, 0, 1};
    shadowShader.camera = cameraLight.direction();
    shadowShader.backfaceCulling = false;
    shadowShader.frontfaceCulling = false;
    shadowShader.light = {0, 1, 1};
    shadowShader.light.normalize();

    shadowShader.viewport = make_viewport(0, 0, viewW, viewH, 0.7);

    TGAImage imageLight(imageW, imageH, TGAImage::RGB);

    vector<float> zbufferShadow(imageW * imageH, std::numeric_limits<float>::lowest());
    fill(zbufferShadow.begin(), zbufferShadow.end(), std::numeric_limits<float>::lowest());

    for (size_t f = 0; f < size_t(model.nfaces()); ++f) {
        Vec3f screen[3];
        for (size_t v = 0; v < 3; ++v) {
            screen[v] = shadowShader.vertex(f, v);
        }
        triangle(screen, shadowShader, zbufferShadow.data(), imageLight);
    }


    // Render from camera point of view
    Camera camera{.eye = {5, 2, 5}, .center = {0, 0, 0}, .up = {0, 1, 0}};

    PhongShader phongShader;
    phongShader.model = &model;
    phongShader.ambientColor = TGAColor(255, 255, 255, 255);
    phongShader.diffuseColor = TGAColor(255, 128, 128, 255);
    phongShader.specularColor = TGAColor(255, 255, 255, 255);
    phongShader.ambientWeight = 0.2f;
    phongShader.diffuseWeight = 0.8f;
    phongShader.specularWeight = 0.5f;
    phongShader.specularPower = 10.f;
    phongShader.view = camera.view();
    phongShader.projection = camera.projection();
    phongShader.light = {0, 0, 1};
    phongShader.camera = camera.direction();
    phongShader.backfaceCulling = false;
    phongShader.frontfaceCulling = false;
    phongShader.light = {0, 1, 1};
    phongShader.light.normalize();
    phongShader.width = imageW;
    phongShader.height = imageH;
    phongShader.toLightScreenTransform = shadowShader.viewport * shadowShader.projection * shadowShader.view;
    phongShader.screen = vector<Vec3f>(3, Vec3f(0, 0, 0));

    phongShader.zbufferShadow = zbufferShadow;

    TGAImage image(imageW, imageH, TGAImage::RGB);

    phongShader.viewport = make_viewport(0, 0, viewW, viewH, 0.7);

    vector<float> zbuffer(imageW * imageH, std::numeric_limits<float>::lowest());
    fill(zbuffer.begin(), zbuffer.end(), std::numeric_limits<float>::lowest());

    for (size_t f = 0; f < size_t(model.nfaces()); ++f) {
        Vec3f screen[3];
        for (size_t v = 0; v < 3; ++v) {
            screen[v] = phongShader.vertex(f, v);
        }
        triangle(screen, phongShader, zbuffer.data(), image);
    }

    string filename = "OneShader.tga";
    image.write_tga_file(filename.data());
}

