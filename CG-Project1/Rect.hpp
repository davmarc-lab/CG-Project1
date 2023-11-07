#pragma once
#include "IntScene.hpp"

class Rect : public IntScene
{
public:
	Rect() {}

	void printState() override { cout << "Rect" << endl; }
};

