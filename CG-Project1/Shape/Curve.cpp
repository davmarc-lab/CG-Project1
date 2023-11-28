#include "Curve.hpp"
#include <glm/gtc/type_ptr.hpp>

Curve Curva = Curve();
Curve Derivata = Curve();
Curve Poligonale = Curve();

// number of points for final result
int pval = 140;
float* t;

float Curve::dx(int i, float* t, float Tens, float Bias, float Cont, Curve* Fig)
{
	if (i == 0)
		return 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP[i + 1].x - Fig->CP[i].x) / (t[i + 1] - t[i]);
	if (i == Fig->CP.size() - 1)
		return 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP[i].x - Fig->CP[i - 1].x) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return 0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (Fig->CP.at(i).x - Fig->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP.at(i + 1).x - Fig->CP.at(i).x) / (t[i + 1] - t[i]);
	else
		return 0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (Fig->CP.at(i).x - Fig->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (Fig->CP.at(i + 1).x - Fig->CP.at(i).x) / (t[i + 1] - t[i]);
}
float Curve::dy(int i, float* t, float Tens, float Bias, float Cont, Curve* Fig)
{
	if (i == 0)
		return 0.5 * (1.0 - Tens) * (1.0 - Bias) * (1 - Cont) * (Fig->CP.at(i + 1).y - Fig->CP.at(i).y) / (t[i + 1] - t[i]);
	if (i == Fig->CP.size() - 1)
		return 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP.at(i).y - Fig->CP.at(i - 1).y) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return 0.5 * (1 - Tens) * (1 + Bias) * (1 + Cont) * (Fig->CP.at(i).y - Fig->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (Fig->CP.at(i + 1).y - Fig->CP.at(i).y) / (t[i + 1] - t[i]);
	else
		return 0.5 * (1 - Tens) * (1 + Bias) * (1 - Cont) * (Fig->CP.at(i).y - Fig->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (Fig->CP.at(i + 1).y - Fig->CP.at(i).y) / (t[i + 1] - t[i]);
}

float Curve::DX(int i, float* t)
{
    return Derivata.CP.at(i).x == 0 ? dx(i, t, 0.0, 0.0, 0.0, &Poligonale) : Derivata.CP.at(i).x;
}

float Curve::DY(int i, float* t)
{
    return Derivata.CP.at(i).y == 0 ? dy(i, t, 0.0, 0.0, 0.0, &Poligonale) : Derivata.CP.at(i).y; 
}

void Curve::hermiteInterpolation(float* t, vec4 color_top, vec4 color_bot, Curve* Fig)
{
	float p_t = 0, p_b = 0, p_c = 0, x, y;
	float passotg = 1.0 / (float)(pval - 1);

	float tg = 0, tgmapp, ampiezza;
	int i = 0;
	int is = 0;

	Fig->vertex.clear();
	Fig->colors.clear();
    /*this->addElementVertex(vec3(0, 0, 0));
    this->addElementColors(color_bot);*/

	for (tg = 0; tg <= 1; tg += passotg)
	{
		if (tg > t[is + 1])
			is++;

		ampiezza = (t[is + 1] - t[is]);
		tgmapp = (tg - t[is]) / ampiezza;

		x = Fig->CP[is].x * PHI0(tgmapp) + DX(is, t) * PHI1(tgmapp) * ampiezza + Fig->CP[is + 1].x * PSI0(tgmapp) + DX(is + 1, t) * PSI1(tgmapp) * ampiezza;
		y = Fig->CP[is].y * PHI0(tgmapp) + DY(is, t) * PHI1(tgmapp) * ampiezza + Fig->CP[is + 1].y * PSI0(tgmapp) + DY(is + 1, t) * PSI1(tgmapp) * ampiezza;
		this->vertex.push_back(vec3(x, y, 0.0));
		this->colors.push_back(color_top);
	}
}

void Curve::buildHermite(vec4 color_top, vec4 color_bot)
{
	Poligonale.CP = Curva.CP;
	Poligonale.colCP = Curva.colCP;
    Poligonale.createVertexArray();

	if (Poligonale.CP.size() > 1)
	{
		t = new float[Poligonale.CP.size()];
		float step = 1.0 / (float)(Poligonale.CP.size() - 1);
		for (int i = 0; i < Poligonale.CP.size(); i++)
			t[i] = (float)i * step;

		this->hermiteInterpolation(t, color_top, color_bot, &Curva);
        this->setVertexNum(this->vertex.size());
	}
}

void Curve::readDataFromFile(const char* path)
{
	int i;
	struct Dati
	{
		float x;
		float y;
		float z;
	};

	FILE* file = fopen(path, "r");
	if (file == NULL)
	{
		perror("Impossibile aprire il file");
	}

	// Vettore per memorizzare i dati
	struct Dati dati[1000]; // Supponiamo che ci siano al massimo 100 righe nel file

	int riga = 0;
	while (fscanf(file, "%f %f %f", &dati[riga].x, &dati[riga].y, &dati[riga].z) == 3)
	{
		// Incrementa l'indice della riga
		riga++;

		// Puoi aggiungere un controllo qui per evitare il superamento dell'array dati
		if (riga >= 1000)
		{
			printf("The file is too long, cannot write all data.\n");
			break;
		}
	}

	// Chiudi il file
	fclose(file);

	for (int i = 0; i < riga; i++)
	{
		Curva.CP.push_back(vec3(dati[i].x, dati[i].y, dati[i].z));
		Derivata.CP.push_back(vec3(0.0, 0.0, 0.0));
	}
}

void Curve::createVertexArray()
{
    glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->vbo_g);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_g);
	glBufferData(GL_ARRAY_BUFFER, this->vertex.size() * sizeof(vec3), this->vertex.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &this->vbo_c);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_c);
	glBufferData(GL_ARRAY_BUFFER, this->colors.size() * sizeof(vec4), this->colors.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

void Curve::draw(Shader shader)
{
    GLuint modelLoc = glGetUniformLocation(shader.getId(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(this->getModelMatrix()));

    glBindVertexArray(this->getVertexArrayObject());
	glDrawArrays(GL_TRIANGLE_FAN, 0, this->getVertexNum());
	glBindVertexArray(0);
}

void Curve::clearShape()
{
    this->clearVertexArray();
    this->clearColorArray();
    this->CP.clear();
    this->colCP.clear();

    // Clearing memory for VAO, VBO buffers
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo_g);
    glDeleteBuffers(1, &this->vbo_c);
}
