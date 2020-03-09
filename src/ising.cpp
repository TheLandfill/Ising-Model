#include "util.h"
#include "ising.h"
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdio>

namespace ising {
typedef uint32_t uint;

Ising_2D::Ising_2D(uint sz, double temp, uint sd) :
	size(sz),
	table(sz, sz, 1),
	pixels(new uint32_t[sz * sz]),
	temperature(temp),
	seed(sd) {
		gen_random();
		glGenTextures(1, &texture);
		draw();
	}

double Ising_2D::delta_U(uint x, uint y) {
	char top = at(x, (y + 1) % size);
	char bottom = at(x, (y + size - 1) % size);
	char left = at((x - 1 + size) % size, y);
	char right = at((x + 1) % size, y);
	return 2 * at(x, y) * (top + bottom + left + right);
}

void Ising_2D::flip(uint x, uint y) {
	at(x, y) = -at(x, y);
}

void Ising_2D::iterate() {
	uint x = gen_rand_u_int(&seed, size);
	uint y = gen_rand_u_int(&seed, size);
	double E_diff = delta_U(x, y);
	double rand = (double)(gen_rand_u_int(&seed, size)) / (double)size;
	double boltzmann = exp(-(double)(E_diff)/temperature);
	if ((E_diff <= 0) || (rand < boltzmann)) {
		flip(x, y);
	}
}

char Ising_2D::at(uint x, uint y) const {
	return table.at(x, y);
}

char & Ising_2D::at(uint x, uint y) {
	return table.at(x, y);
}

void Ising_2D::gen_random() {
	for (uint i = 0; i < size * size; i++) {
		table[i] = 2 * gen_rand_u_int(&seed, 2) - 1;
	}
}

void Ising_2D::handle_keys(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
	if (key == '\n' || key == 'n' || key == 'N') {
		for (uint i = 0; i < size * size; i++) {
			iterate();
		}
	}
}

void Ising_2D::draw() {
	const char * table_array = table.array();
	const uint32_t colors[] = { 0xFFFFFFFF, 0x000000FF };
	for (uint i = 0; i < size * size; i++) {
		pixels[i] = colors[(table_array[i] + 1) / 2];
	}
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, size, size, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f); glVertex2f(-1.f, -1.f);
		glTexCoord2f(1.f, 0.f); glVertex2f(1.f, -1.f);
		glTexCoord2f(1.f, 1.f); glVertex2f(1.f, 1.f);
		glTexCoord2f(0.f, 1.f); glVertex2f(-1.f, 1.f);
	glEnd();
	GLenum err;
	while((err = glGetError()) != GL_NO_ERROR) {
		printf("Error: %x\n", err);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

float Ising_2D::correlation(const std::vector<Coordinates>& circle_points) {
	float average = 0;
	float pairwise = 0;
	for (uint32_t i = 0; i < size; i++) {
		for (uint32_t j = 0; j < size; j++) {
			average += at(i, j);
			for (size_t k = 0; k < circle_points.size(); k++) {
				const Coordinates& c = circle_points[k];
				pairwise += at(i, j) * at((i + c.x + size) % size, (j + c.y + size) % size);
			}
		}
	}
	const float DOUBLE_COUNTING = 2.f;
	const float NUM_POINTS = size * size;
	const float NUM_CIRCLE = circle_points.size();
	average /= NUM_POINTS;
	pairwise /= DOUBLE_COUNTING * NUM_POINTS * NUM_CIRCLE;
	return pairwise - average * average;
}

double Ising_2D::get_temperature() const {
	return temperature;
}

uint32_t Ising_2D::get_size() const {
	return size;
}

}
