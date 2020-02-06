#ifndef UTIL_H
#define UTIL_H
#include <cstdint>
#include <algorithm>

namespace ising {
	uint32_t gen_rand_u_int(uint32_t* seed, uint32_t max_element);

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

}

#endif
