#include "util.h"
#include <vector>
#include <cstdio>
#include <GL/gl.h>
#include <GL/glu.h>

namespace ising {
// This is 32 bit unsigned xorshift
uint32_t gen_rand_u_int(uint32_t* seed, uint32_t max_element) {
	uint32_t x = *seed;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	*seed = x;
	return ((double)x / (uint32_t)(-1)) * max_element;
}

Coordinates Coordinates::operator+(const Coordinates a) {
	short new_x = x + a.x;
	short new_y = y + a.y;
	return {new_x, new_y};
}

Coordinates Coordinates::operator+=(const Coordinates a){
	x += a.x;
	y += a.y;
	return *this;
}

std::vector<Coordinates> gen_circle_pixels(short radius) {
    short x = radius;
    short y = 0;
    short err = 0;
	std::vector<Coordinates> offset_list;
	offset_list.reserve(8 * radius);

    while (x >= y) {
		offset_list.emplace_back( x,  y);
		offset_list.emplace_back( x, -y);
		offset_list.emplace_back(-x, -y);
		offset_list.emplace_back(-x,  y);
		offset_list.emplace_back( y,  x);
		offset_list.emplace_back( y, -x);
		offset_list.emplace_back(-y, -x);
		offset_list.emplace_back(-y,  x);

		if (err <= 0) {
		    y += 1;
		    err += 2*y + 1;
		} else {
		    x -= 1;
		    err -= 2*x + 1;
		}
    }
	return offset_list;
}

Array_2D<uint32_t> gen_circle_bitmap(short radius, short size, Coordinates center) {
	Array_2D<uint32_t> table(size, size, 0x00000000);
	//uint32_t colors[] = { 0xA8A7A7FF, 0xCC527AFF, 0xE8175DFF, 0x474747FF, 0x363636FF };
	uint32_t colors[] = { 0xFF0000FF, 0x0000FFFF };
	const uint32_t num_colors = sizeof(colors) / sizeof(colors[0]);
	std::vector<Coordinates> coords = gen_circle_pixels(radius);
	for (size_t i = 0; i < coords.size(); i++) {
		Coordinates coord = coords[i] + center;
		table.at((coord.x + size) % size, (coord.y + size) % size) = colors[radius % num_colors];
	}
	return table;
}

void draw_circle_test(Array_2D<uint32_t>& table, short size, uint32_t texture) {
	const uint32_t * pixels = table.array();
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, size, size, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	float prop_size = (float)size/(float)size;
	glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f); glVertex2f(-prop_size, -prop_size);
		glTexCoord2f(1.f, 0.f); glVertex2f( prop_size, -prop_size);
		glTexCoord2f(1.f, 1.f); glVertex2f( prop_size,  prop_size);
		glTexCoord2f(0.f, 1.f); glVertex2f(-prop_size,  prop_size);
	glEnd();
	GLenum err;
	while((err = glGetError()) != GL_NO_ERROR) {
	  printf("Error: %x\n", err);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

}
