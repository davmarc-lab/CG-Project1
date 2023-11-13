#include "Curve.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <iostream>

#define MAX_DATA 1000

Curve* derivate = new Curve();
Curve* poly = new Curve();
Curve* curve = new Curve();

float pval = 140;
float *t;

float Curve::dx (int i, float *t, float Tens, float Bias, float Cont, Curve *poly)
{
    if (i == 0)
        return 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (poly->CP[i + 1].x - poly->CP[i].x) / (t[i + 1] - t[i]);
    if (i == poly->CP.size() - 1)
        return 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (poly->CP[i].x - poly->CP[i - 1].x) / (t[i] - t[i - 1]);

    if (i % 2 == 0)
        return 0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (poly->CP.at(i).x - poly->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (poly->CP.at(i + 1).x - poly->CP.at(i).x) / (t[i + 1] - t[i]);
    else
        return 0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (poly->CP.at(i).x - poly->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (poly->CP.at(i + 1).x - poly->CP.at(i).x) / (t[i + 1] - t[i]);

}

float Curve::dy(int i, float *t, float Tens, float Bias, float Cont, Curve *poly)
{
    if (i == 0)
        return 0.5 * (1.0 - Tens) * (1.0 - Bias) * (1 - Cont) * (poly->CP.at(i + 1).y - poly->CP.at(i).y) / (t[i + 1] - t[i]);
    if (i == poly->CP.size() - 1)
        return 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (poly->CP.at(i).y - poly->CP.at(i - 1).y) / (t[i] - t[i - 1]);

    if (i % 2 == 0)
        return 0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (poly->CP.at(i).y - poly->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (poly->CP.at(i + 1).y - poly->CP.at(i).y) / (t[i + 1] - t[i]);
    else
        return 0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (poly->CP.at(i).y - poly->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (poly->CP.at(i + 1).y - poly->CP.at(i).y) / (t[i + 1] - t[i]);
}

float Curve::DX (int i, float *t)
{
    if (derivate->CP.at(i).x == 0)
        return dx(i, t, 0.0, 0.0, 0.0, poly);
    else
        return derivate->CP.at(i).x;
}

float Curve::DY (int i, float *t)
{
    if (derivate->CP.at(i).y == 0)
        return dy(i, t, 0.0, 0.0, 0.0, poly);
    else
        return derivate->CP.at(i).y;
}

void Curve::hermiteInterpolation(float *t, vec4 color_top, vec4 color_bot)
{
    float p_t = 0, p_b = 0, p_c = 0, x, y;
    float step_tg = 1.0 / (float)(pval - 1);

    float tg = 0, tgmap, phase;
    int is = 0;

    this->clearVertexArray();
    this->clearColorArray();

    this->addElementVertex(vec3(-0.446875, 0.125, 0));
    this->addElementColors(color_bot);

    for (tg = 0; tg <= 1; tg += step_tg)
    {
        if (tg > t[is +1])
            is++;
        phase = t[is + 1] - t[is];
        tgmap = (tg - t[is]) / phase;

        x = this->CP[is].x + PHI0(tgmap) + DX (is, t) * PHI1(tgmap) * phase + this->CP[is + 1].x + PSI0(tgmap) + DX (is + 1, t) * PSI1(tgmap) * phase;
        y = this->CP[is].y + PHI0(tgmap) + DY (is, t) * PHI1(tgmap) * phase + this->CP[is + 1].y + PSI0(tgmap) + DY (is + 1, t) * PSI1(tgmap) * phase;

        this->addElementVertex(vec3(x, y, 0));
        this->addElementColors(color_top);
    }
}

void Curve::buildHermite(vec4 color_top, vec4 color_bot)
{
    poly->CP = curve->CP;
    poly->colCP = curve->colCP;

    if (poly->CP.size() > 1)
    {
        t = new float[curve->CP.size()];
        float step = 1.0 / (float)(curve->CP.size() - 1);

        for (int i = 0; i < curve->CP.size(); i++)
            t[i] = (float)i * step;
        this->hermiteInterpolation(t, color_top, color_bot);

        this->setVertexNum(curve->getVertexArray().size());
    }
    cout << this->getVertexArray().size() << endl;
}

void Curve::readDataFromFile(const char* pathFile)
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

        if (row >= 1000)
        {
            cout << "Too much row in the file, cannot read all the data" << endl;
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

void Curve::createVertexArray()
{
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo_g);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_g);
    glBufferData(GL_ARRAY_BUFFER, this->vertex.size() * sizeof(vec3), this->vertex.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &this->vbo_c);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_c);
    glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(vec4), this->colors.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);
}

void Curve::draw(Shader shader) 
{
    GLuint modelLoc = glGetUniformLocation(shader.getId(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(this->getModelMatrix()));

    glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, this->getVertexNum());
}
