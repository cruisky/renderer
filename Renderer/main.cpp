#include "stdafx.h"

#include "txbase/math/color.h"
#include "txbase/math/transform.h"
#include "txbase/math/sample.h"
#include "txbase/image/image.h"
#include "txbase/image/film.h"
#include "txbase/scene/camera.h"

#include "Core/BSDF.h"
#include "Core/RayTracer.h"
#include "Core/Sampler.h"
#include "Core/Scene.h"
#include "Core/Primitive.h"
#include "Core/Light.h"
#include "Core/SceneMesh.h"
#include "Core/RendererConfig.h"
#include "Core/Renderer.h"
#include "Accelerators/BVH.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"
#include "Samplers/RandomSampler.h"

#include "Application/GUIViewer.h"

#include <iostream>
#include <string>

using namespace std;
using namespace TX;
using namespace TX::UI;

#pragma warning(disable: 4005)
#pragma warning(disable: 4244)
#pragma warning(disable: 4305)
#pragma warning(disable: 4018)

void GUIMainMesh() {
#ifndef _DEBUG
	int width = 800;
	int height = 600;
#else
	int width = 320;
	int height = 240;
#endif
	/////////////////////////////////////
	// Camera
	shared_ptr<Camera> camera(new Camera(width, height));
	camera->transform.Rotate(Quaternion::Euler(Math::PI / 2, 0.f, 0.f));
	camera->transform.Translate(0, 2, 4);

	/////////////////////////////////////////////
	// Materials
	shared_ptr<BSDF> diffuse_blue(new Diffuse(Color(0.29, 0.29, 0.53)));
	shared_ptr<BSDF> diffuse_red(new Diffuse(Color(0.725, 0.26, 0.24)));
	shared_ptr<BSDF> diffuse_green(new Diffuse(Color(0.2, 0.7, 0.2)));
	shared_ptr<BSDF> diffuse_yellow(new Diffuse(Color(0.7, 0.6, 0.3)));
	shared_ptr<BSDF> diffuse_orange(new Diffuse(Color(0.7, 0.4, 0.1)));
	shared_ptr<BSDF> diffuse_gray(new Diffuse(Color(0.8)));
	shared_ptr<BSDF> diffuse_black(new Diffuse(Color::BLACK));
	shared_ptr<BSDF> diffuse_white(new Diffuse(Color::WHITE));
	shared_ptr<BSDF> mirror(new Mirror);
	shared_ptr<BSDF> glass(new Dielectric);
	shared_ptr<BSDF> glass_blue(new Dielectric(Color(0.2, 0.5, 1.0)));
	shared_ptr<BSDF> glass_green(new Dielectric(Color(0.7, 1.0, 0.8)));
	shared_ptr<BSDF> glass_red(new Dielectric(Color(1.0, 0.3, 0.3)));
	shared_ptr<BSDF> glass_purple(new Dielectric(Color(0.8, 0.6, 1.0)));

	///////////////////////////////////////////
	// Shapes & Primitives
	SceneMesh sphere;
#ifdef _DEBUG
	sphere.LoadSphere(1.f, 4, 1);
#else
	sphere.LoadSphere();
#endif
	SceneMesh plane; plane.LoadPlane();

	// load teapot
	std::vector<ObjShape> teapot_shapes;
	std::vector<ObjMaterial> teapot_mat;
	ObjLoader::Load(teapot_shapes, teapot_mat, "../ObjViewer/teapot.obj", "./");
	SceneMesh teapot(teapot_shapes.front().mesh);

	int wall_size = 9;
	shared_ptr<Primitive> w_bottom(new Primitive(plane, diffuse_white));
	w_bottom->transform.Scale(wall_size, wall_size, 1);

	shared_ptr<Primitive> w_forward(new Primitive(plane, diffuse_white));
	w_forward->transform.SetRotation(Quaternion::AngleAxis(Math::PI / 2, Vec3::X));
	w_forward->transform.Translate(0, 0, -wall_size / 2);
	w_forward->transform.Scale(wall_size, wall_size, 1);

	shared_ptr<Primitive> w_back(new Primitive(plane, diffuse_white));
	w_back->transform.SetRotation(Quaternion::AngleAxis(-Math::PI / 2, Vec3::X));
	w_back->transform.Translate(0, 0, -wall_size / 2);
	w_back->transform.Scale(wall_size, wall_size, 1);

	shared_ptr<Primitive> w_left(new Primitive(plane, diffuse_blue));
	w_left->transform.SetRotation(Quaternion::AngleAxis(Math::PI / 2, Vec3::Y));
	w_left->transform.Translate(0, 0, -wall_size / 2);
	w_left->transform.Scale(wall_size, wall_size, 1);

	shared_ptr<Primitive> w_right(new Primitive(plane, diffuse_yellow));
	w_right->transform.SetRotation(Quaternion::AngleAxis(-Math::PI / 2, Vec3::Y));
	w_right->transform.Translate(0, 0, -wall_size / 2);
	w_right->transform.Scale(wall_size, wall_size, 1);

	shared_ptr<Primitive> w_top(new Primitive(plane, diffuse_white));
	w_top->transform.SetRotation(Quaternion::AngleAxis(Math::PI, Vec3::Y));
	w_top->transform.Translate(0, 0, -wall_size / 2);
	w_top->transform.Scale(wall_size, wall_size, 1);

	shared_ptr<Primitive> ball1(new Primitive(teapot, diffuse_red));
	ball1->transform.Translate(-2, 0, 2.5);
	ball1->transform.SetRotation(Quaternion::AngleAxis(Math::PI / 2, Vec3::X));
	shared_ptr<Primitive> ball2(new Primitive(teapot, glass));
	ball2->transform.Translate(2.5, 0, 0);
	ball2->transform.SetRotation(Quaternion::AngleAxis(Math::PI / 2, Vec3::X));
	ball2->transform.Rotate(Quaternion::AngleAxis(-Math::PI * 0.9, Vec3::Y));
	ball2->transform.Scale(1.5, 1.5, 1.5);
	shared_ptr<Primitive> ball3(new Primitive(teapot, mirror));
	ball3->transform.Translate(-0.5, 0.5, 0);
	ball3->transform.SetRotation(Quaternion::AngleAxis(Math::PI / 2, Vec3::X));
	ball3->transform.Scale(2, 2, 2);

	/////////////////////////////////////
	// Lights

	//shared_ptr<Light> light_main(new PointLight(Color(3), 200, Vec3(0, 0, wall_size / 2 - 0.6)));
	shared_ptr<Primitive> lamp(new Primitive(plane, diffuse_black));
	lamp->transform.SetRotation(Quaternion::AngleAxis(Math::PI, Vec3::Y));
	lamp->transform.Translate(0, 0, -wall_size / 2 + 0.05);
	lamp->transform.Scale(2, 2, 1);
	shared_ptr<Light> light_lamp(new AreaLight(Color(9), lamp));

	/////////////////////////////////////
	// Scene
	shared_ptr<Film> film(new Film(FilterType::GaussianFilter));
	shared_ptr<Scene> scene(new Scene(std::make_unique<BVH>()));

	scene->AddPrimitive(w_bottom);
	scene->AddPrimitive(w_top);
	scene->AddPrimitive(w_forward);
	scene->AddPrimitive(w_back);
	scene->AddPrimitive(w_left);
	scene->AddPrimitive(w_right);

	scene->AddPrimitive(ball1);
	scene->AddPrimitive(ball2);
	scene->AddPrimitive(ball3);

	//scene->AddLight(light_main);
	scene->AddPrimitive(lamp); scene->AddLight(light_lamp);

	scene->Construct();
	RendererConfig config;
	//config.tracer_t = TracerType::DirectLighting;
	config.width = width;
	config.height = height;
#ifndef _DEBUG
	config.samples_per_pixel = 100;
#else
	config.samples_per_pixel = 4;
#endif
	config.tracer_maxdepth = 12;
	GUIViewer gui(config, *scene, *camera, *film);
	gui.Run();
}

int main() {
	bool succeeded = false;
	try {
		GUIMainMesh();
		succeeded = true;
	}
	catch (int ex) {
		std::fprintf(stderr, "Uncaught Exception: \n\t%d\n", ex);
	}
	catch (char const *ex) {
		std::fprintf(stderr, "Uncaught Exception: \n\t%s\n", ex);
	}
	catch (const std::exception& ex) {
		std::fprintf(stderr, "Uncaught Exception: \n\t%s\n", ex.what());
	}
	catch (const std::string& ex) {
		std::fprintf(stderr, "Uncaught Exception: \n\t%s\n", ex.c_str());
	}
	catch (...) {
		std::fprintf(stderr, "Uncaught Exception\n");
	}
	if (!succeeded) {
		std::fprintf(stdout, "Press enter to exit.\n");
		getchar();
	}
	return 0;
}
