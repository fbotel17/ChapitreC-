#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include "point.h"

using namespace std;

point::point(float x, float y)
{
	Px = x;
	Py = y;
}


void point::affiche()
{
	cout << "Position de x : " << Px << endl;
	cout << "Position de y : " << Py << endl;
}

void point::deplace(float x, float y)
{
	Px += x;
	Py += y;
}