#include<graphics.h>
#include<iostream>
#include<vector>

namespace demo {

	struct rgb {
		int r, g, b;
		rgb(int x, int y, int z) : r(x), g(y), b(z) {};
	};

	// Bresenham line algorithm
	void line(int x1, int y1, int x2, int y2, rgb lcolor, rgb rcolor) {
		int dx = x2 - x1, dy = y2 - y1;
		int ux = ((dx > 0) << 1) - 1,			// Step of increment
			uy = ((dy > 0) << 1) - 1,			// Step of increment
			eps = 0;							// Error sum
		dx = std::abs(dx), dy = std::abs(dy);
		rgb color(0, 0, 0);

		// tan¦Á = | k | < 1
		if (dx > dy) {
			for (int x = x1, y = y1; x <= x2; x += ux) {
				// Single linear interpolation
				float rate = (float)abs(x - x1) / (float)abs(x2 - x1);
				color.r = lcolor.r * rate + rcolor.r * (1 - rate);
				color.g = lcolor.g * rate + rcolor.g * (1 - rate);
				color.b = lcolor.b * rate + rcolor.b * (1 - rate);
				putpixel(x, y, RGB(color.r, color.g, color.b));

				eps += dy;
				if ((eps << 1) >= dx) {
					y += uy;
					eps -= dx;
				}
			}
		}

		// tan¦Á = | k | >= 1
		else {
			for (int x = x1, y = y1; y <= y2; y += uy) {
				float rate = (float)abs(x - x1) / (float)abs(x2 - x1);
				color.r = lcolor.r * rate + rcolor.r * (1 - rate);
				color.g = lcolor.g * rate + rcolor.g * (1 - rate);
				color.b = lcolor.b * rate + rcolor.b * (1 - rate);
				putpixel(x, y, RGB(color.r, color.g, color.b));

				eps += dx;
				if ((eps << 1) >= dx) {
					x += ux;
					eps -= dy;
				}
			}
		}

	}
}

int main() {
	// Init device and window
	initgraph(1000, 1000);
	
	int num = 16;
	double r = 200, t = 2 * 3.13159 / num;
	bool even = num % 2 == 0;

	std::vector<int> x(num), y(num);
	// Param function of circle
	for (int i = 0; i < num; ++i) {
		x[i] = r * cos(i * t) + 500;
		y[i] = r * sin(i * t) + 400;
	}

	// Draw line to connect vertex
	for (int i = 0; i < num; ++i) {
		for (int j = 0; j < num; ++j) {
			if (i == j) continue;
			if (even && i == num / 2 + j || j == num / 2 + i) continue;
			demo::line(x[i], y[i], x[j], y[j],
			demo::rgb(255, 255, 0), demo::rgb(0, 255, 255));
		}
	}

	while (1);

}