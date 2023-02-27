#pragma once

class point
{
public:
	point(float x = 0, float y = 0);
	void abcisse();
	void ordonnee();
	void deplace(float x, float y);

private:
	float Px;
	float Py;
};

