#ifndef COLOR_H
#define COLOR_H

struct Color
{
	Color() {
        r = g = b = 0;
		a = 1.0;
    }

    Color(float r, float g, float b)
        : r(r), g(g), b(b), a(1.0) {}

	float r;
	float g;
	float b;
	float a;
};
#endif