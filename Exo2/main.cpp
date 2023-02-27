#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include "point.h"

using namespace std;

int main(int argc, char** argv)
{
	point P1(4, 5);
	P1.abcisse();
	P1.ordonnee();
	P1.deplace(-1, -4);
	P1.abcisse();
	P1.ordonnee();
}