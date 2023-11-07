#pragma once
#include <string>
#include <iostream>

using namespace std;

class IntScene
{
protected:
	string state;
public:
	virtual void printState() = 0;
};

