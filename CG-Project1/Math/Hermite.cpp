#include "Hermite.hpp"

#include <cstdio>
#include <iostream>

#define MAX_DATA 1000

Curve* derivate = new Curve();
Curve* poly = new Curve();
Curve* curve = new Curve();

float pval = 140;
float *t;

float dx (int i, float *t, float Tens, float Bias, float Cont, Curve *shape)
{
    if (i == 0)
        return 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (shape->CP[i + 1].x - shape->CP[i].x) / (t[i + 1] - t[i]);
    if (i == shape->CP.size() - 1)
        return 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (shape->CP[i].x - shape->CP[i - 1].x) / (t[i] - t[i - 1]);

    if (i % 2 == 0)
        return 0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (shape->CP.at(i).x - shape->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (shape->CP.at(i + 1).x - shape->CP.at(i).x) / (t[i + 1] - t[i]);
    else
        return 0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (shape->CP.at(i).x - shape->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (shape->CP.at(i + 1).x - shape->CP.at(i).x) / (t[i + 1] - t[i]);

}

float dy(int i, float *t, float Tens, float Bias, float Cont, Curve *shape)
{
    if (i == 0)
        return 0.5 * (1.0 - Tens) * (1.0 - Bias) * (1 - Cont) * (shape->CP.at(i + 1).y - shape->CP.at(i).y) / (t[i + 1] - t[i]);
    if (i == shape->CP.size() - 1)
        return 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (shape->CP.at(i).y - shape->CP.at(i - 1).y) / (t[i] - t[i - 1]);

    if (i % 2 == 0)
        return 0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (shape->CP.at(i).y - shape->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (shape->CP.at(i + 1).y - shape->CP.at(i).y) / (t[i + 1] - t[i]);
    else
        return 0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (shape->CP.at(i).y - shape->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (shape->CP.at(i + 1).y - shape->CP.at(i).y) / (t[i + 1] - t[i]);
}

float DX (int i, float *t)
{
    if (derivate->CP.at(i).x == 0)
        return dx(i, t, 0.0, 0.0, 0.0, poly);
    else
        return derivate->CP.at(i).x;
}

float DY (int i, float *t)
{
    if (derivate->CP.at(i).y == 0)
        return dy(i, t, 0.0, 0.0, 0.0, poly);
    else
        return derivate->CP.at(i).y;
}

void hermiteInterpolation(float *t, Curve *shape, vec4 color_top, vec4 color_bot)
{
    float p_t = 0, p_b = 0, p_c = 0, x, y;
    float step_tg = 1.0 / (float)(pval - 1);

    float tg = 0, tgmap, phase;
    int is = 0;

    shape->clearVertexArray();
    shape->clearColorArray();

    shape->addElementVertex(vec3(-0.1, -0.7, 0.0));
    shape->addElementColors(color_bot);

    for (tg = 0; tg <= 1; tg += step_tg)
    {
        if (tg > t[is +1])
            is++;
        phase = t[is + 1] - t[is];
        tgmap = (tg - t[is]) / phase;

        x = shape->CP[is].x + PHI0(tgmap) + DX (is, t) * PHI1(tgmap) * phase + shape->CP[is + 1].x + PSI0(tgmap) + DX (is + 1, t) * PSI1(tgmap) * phase;
        y = shape->CP[is].y + PHI0(tgmap) + DY (is, t) * PHI1(tgmap) * phase + shape->CP[is + 1].y + PSI0(tgmap) + DY (is + 1, t) * PSI1(tgmap) * phase;
    }

}

Curve* buildHermite(vec4 color_top, vec4 color_bot, Curve *shape)
{
    poly->CP = curve->CP;
    poly->colCP = curve->colCP;

    if (poly->CP.size() > 1)
    {
        t = new float[curve->CP.size()];
        float step = 1.0 / (float)(curve->CP.size() - 1);

        for (int i = 0; i < curve->CP.size(); i++)
            t[i] = (float)i * step;
        hermiteInterpolation(t, curve, color_top, color_bot);

        shape->setVertexNum(curve->getVertexArray().size());
    }
    return curve;
}

void readDataFromFile(const char* pathFile)
{
    typedef struct Coords
    {
        float x, y, z;
    } Coords;

    FILE *file = fopen(pathFile, "r");
    if (file == NULL)
        perror("Cannot open the file");

    Coords data[MAX_DATA];

    int row = 0;
    while (fscanf(file, "%f %f %f", &data[row].x, &data[row].y, &data[row].z) == 3)
    {
        row++;

        if (row <= 1000)
        {
            std::cout << "Too much row in the file, cannot read all the data" << std::endl;
            break;
        }
    }

    fclose(file);
    
    for (int i = 0; i < row; i++)
    {
        curve->CP.push_back(vec3(data[i].x, data[i].y, data[i].z));
        derivate->CP.push_back(vec3(0.0, 0.0, 0.0));
    }
}
