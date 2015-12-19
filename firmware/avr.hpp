#include <avr/io.h>

static inline void IO_write(volatile uint8_t& reg, uint8_t mask, uint8_t vals) {
	reg = (reg & ~mask) | vals;
}

template <class T, class U, class... Rest>
static inline void IO_write(volatile uint8_t& reg, uint8_t mask, uint8_t vals, T pin, U val, Rest... rest) {
	IO_write(
		reg,
		mask | (1<<pin),
		vals | (val<<pin),

		rest...
	);
}

template <class... Rest>
static inline void IO(volatile uint8_t& reg, Rest... args) {
	uint8_t mask = 0;
	uint8_t vals = 0;
	IO_write(
		reg,
		mask,
		vals,

		args...
	);
}

template<class T>
static inline bool IO(volatile uint8_t& reg, T pin) {
	return reg & pin;
}

