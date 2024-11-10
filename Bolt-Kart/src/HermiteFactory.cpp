#include "../include/HermiteFactory.hpp"

#include "../../Bolt-Core/include/Engine.hpp"
#include "../../Bolt-Core/include/Graphics.hpp"

#include <cmath>
#include <cstdio>
#include <iostream>

Curve Curva = Curve();
Curve Derivata = Curve();
Curve Poligonale = Curve();

int pval = 140;
float *t;

float dx(int i, float *t, float Tens, float Bias, float Cont, Curve *Fig) {
	if (i == 0)
		return 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP[i + 1].x - Fig->CP[i].x) / (t[i + 1] - t[i]);
	if (i == Fig->CP.size() - 1)
		return 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP[i].x - Fig->CP[i - 1].x) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return 0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (Fig->CP.at(i).x - Fig->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP.at(i + 1).x - Fig->CP.at(i).x) / (t[i + 1] - t[i]);
	else
		return 0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (Fig->CP.at(i).x - Fig->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (Fig->CP.at(i + 1).x - Fig->CP.at(i).x) / (t[i + 1] - t[i]);
}
float dy(int i, float *t, float Tens, float Bias, float Cont, Curve *Fig) {
	if (i == 0)
		return 0.5 * (1.0 - Tens) * (1.0 - Bias) * (1 - Cont) * (Fig->CP.at(i + 1).y - Fig->CP.at(i).y) / (t[i + 1] - t[i]);
	if (i == Fig->CP.size() - 1)
		return 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP.at(i).y - Fig->CP.at(i - 1).y) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return 0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (Fig->CP.at(i).y - Fig->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP.at(i + 1).y - Fig->CP.at(i).y) / (t[i + 1] - t[i]);
	else
		return 0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (Fig->CP.at(i).y - Fig->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (Fig->CP.at(i + 1).y - Fig->CP.at(i).y) / (t[i + 1] - t[i]);
}

float DX(int i, float *t) {
	return Derivata.CP.at(i).x == 0 ? dx(i, t, 0.0, 0.0, 0.0, &Poligonale) : Derivata.CP.at(i).x;
}

float DY(int i, float *t) {
	return Derivata.CP.at(i).y == 0 ? dy(i, t, 0.0, 0.0, 0.0, &Poligonale) : Derivata.CP.at(i).y;
}

void hermiteInterpolation(float *t, vec4 color_top, vec4 color_bot, Curve *curve) {
	float p_t = 0, p_b = 0, p_c = 0, x, y;
	float passotg = 1.0 / (float)(pval - 1);

	float tg = 0, tgmapp, ampiezza;
	int i = 0;
	int is = 0;

	for (tg = 0; tg <= 1; tg += passotg) {
		if (tg > t[is + 1])
			is++;

		ampiezza = (t[is + 1] - t[is]);
		tgmapp = (tg - t[is]) / ampiezza;

		x = curve->CP[is].x * PHI0(tgmapp) + DX(is, t) * PHI1(tgmapp) * ampiezza + curve->CP[is + 1].x * PSI0(tgmapp) + DX(is + 1, t) * PSI1(tgmapp) * ampiezza;
		y = curve->CP[is].y * PHI0(tgmapp) + DY(is, t) * PHI1(tgmapp) * ampiezza + curve->CP[is + 1].y * PSI0(tgmapp) + DY(is + 1, t) * PSI1(tgmapp) * ampiezza;

		curve->vertex.push_back({x, y, 0.0});
		curve->colors.push_back(color_top);
	}
	curve->nv = curve->vertex.size();
}

void buildHermite(vec4 color_top, vec4 color_bot, Curve *curve) {
	// init Curve in GPU
	Poligonale.CP = curve->CP;
	Poligonale.colCP = curve->colCP;

	if (curve->CP.size() > 1) {
		t = new f32[curve->CP.size()];
		float step = 1. / (f32)(curve->CP.size() - 1);

		for (int i = 0; i < curve->CP.size(); i++) {
			t[i] = (f32)i * step;
		}

		hermiteInterpolation(t, color_top, color_bot, curve);
		curve->nv = curve->vertex.size();
	}

	Curva.vertex.clear();
	Curva.colors.clear();
	Curva.CP.clear();
	Curva.colCP.clear();
	Derivata.vertex.clear();
	Derivata.colors.clear();
	Derivata.CP.clear();
	Derivata.colCP.clear();
	Poligonale.vertex.clear();
	Poligonale.colors.clear();
	Poligonale.CP.clear();
	Poligonale.colCP.clear();
}

Shared<Curve> readDataFromFile(const char *path) {
	auto result = CreateShared<Curve>();

	struct Data {
		f32 x, y, z;
	};

	FILE *file = fopen(path, "r");
	if (file == NULL) {
		std::cerr << "Cannot open the file: " << path << "\n";
		return nullptr;
	}

	struct Data data[1000]{};

	int row = 0;
	while (fscanf(file, "%f %f %f", &data[row].x, &data[row].y, &data[row].z) == 3) {
		row++;
		if (row >= 1000) {
			std::cerr << "File is too long: " << path << "\n";
			return nullptr;
		}
	}

	fclose(file);

	for (int i = 0; i < row; i++) {
		result->CP.push_back({data[i].x, data[i].y, data[i].z});
		Derivata.CP.push_back({0, 0, 0});
	}

	return result;
}

void initHermiteMesh(const u32 &id, Curve *mesh) {
	const auto em = EntityManager::instance();
	Shared<Mesh> comp;

	if (!em->entityHasComponent<Transform>(id))
		em->addComponent<Transform>(id);

	if (!em->entityHasComponent<Mesh>(id)) {
		comp = em->addComponent<Mesh>(id);

		comp->vertices = mesh->vertex;
		comp->colorComponent.colors = mesh->colors;
	} else {
		comp = em->getEntityComponent<Mesh>(id);
	}

	comp->vao.onAttach();
	comp->vbo_g.onAttach();

	comp->vao.bind();
	comp->vbo_g.setup(comp->vertices, 0);
	comp->vao.linkAttribFast(0, 3, 0, false, 0, 0);

	comp->colorComponent.vbo_c.onAttach();
	comp->colorComponent.vbo_c.setup(comp->colorComponent.colors, 0);
	comp->vao.linkAttribFast(1, 4, 0, false, 0, 0);

	auto shaderComp = em->addComponent<ShaderComponent>(id);
	shaderComp->shader = CreateUnique<ShaderProgram>("shader/defaultOrthoVertShader.glsl", "shader/defaultFragShader.glsl");
	shaderComp->shader->createShaderProgram();

    auto vao = comp->vao;
    auto size = comp->vertices.size();
	
    comp->render.setCall([vao, size]() {
        RenderApi::instance()->getRenderer()->drawArraysTriangleFan(vao, size);
    });
}
