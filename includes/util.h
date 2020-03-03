#ifndef UTIL_H
#define UTIL_H
#include <cstdint>
#include <algorithm>
#include <vector>

namespace ising {
	uint32_t gen_rand_u_int(uint32_t* seed, uint32_t max_element);

	struct Coordinates {
		short x, y;
		Coordinates(short x_, short y_) : x(x_), y(y_) {}
		Coordinates operator+(const Coordinates a);
		Coordinates operator+=(const Coordinates a);
	};

	template <typename T>
	class Array_2D {
	private:
		const uint32_t rows, cols;
		T * table;
	public:
		Array_2D(uint32_t r, uint32_t c, T init = 0) : rows(r), cols(c), table(new T[rows * cols]) {
			std::fill(table, table + rows + cols, init);
		}
		T at(uint32_t r, uint32_t c) const {
			return table[r * cols + c];
		}
		T& at(uint32_t r, uint32_t c) {
			return table[r * cols + c];
		}
		T operator[](uint32_t i) const {
			return table[i];
		}
		T & operator[](uint32_t i) {
			return table[i];
		}
		const T * array() {
			return table;
		}
		~Array_2D() {
			delete[] table;
		}
	};

	std::vector<Coordinates> gen_circle_pixels(short radius);
	Array_2D<uint32_t> gen_circle_bitmap(short radius, short size, Coordinates center = {0, 0});
	void draw_circle_test(Array_2D<uint32_t>& table, short size, uint32_t texture);

}

#endif
