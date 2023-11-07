#pragma once
#include "IntScene.hpp"

class Quad: public IntScene
{
public:
	void printState() override { cout << "Quad" << endl; }
};

