#ifndef ISING_H
#define ISING_H
#include "util.h"
#include <cstdint>

namespace ising {
	class Ising_2D {
	private:
		const uint32_t size;
		uint32_t texture;
		Array_2D<char> table;
		uint32_t * pixels;
		const double temperature;
		uint32_t seed;
		void gen_random();
	public:
		Ising_2D(uint32_t size, double temperature, uint32_t seed = 0);
		double delta_U(uint32_t x, uint32_t y);
		void flip(uint32_t x, uint32_t y);
		void iterate();
		char at(uint32_t x, uint32_t y) const;
		char & at(uint32_t x, uint32_t y);
		void handle_keys(unsigned char key, int x, int y);
		void draw();
	};
}

#endif
