#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "avr.hpp"

enum EVoltage {
	E_12V,
	E_9V,
	E_5V
};

const uint8_t D_PLUS_3V3 = PB0;
const uint8_t D_PLUS_0V6 = PB1;
const uint8_t D_MINUS_3V3 = PB3;
const uint8_t D_MINUS_0V6 = PB2;


int main(void) {
	EVoltage voltage = E_12V;

	/** init with hi-z (this is redundant)
	IO(DDRB,
		D_PLUS_3V3, 0,
		D_PLUS_0V6, 0,
		D_MINUS_3V3, 0,
		D_MINUS_0V6, 0
	);
	IO(PORTB,
		D_PLUS_3V3, 0,
		D_PLUS_0V6, 0,
		D_MINUS_3V3, 0,
		D_MINUS_0V6, 0
	);
	*/

	// wait until stable connection
	_delay_ms(1000);

	// reset line (D+ = 0V, D- = 0V)
	IO(DDRB,
		D_PLUS_0V6, 1,
		D_MINUS_0V6, 1
	);
	_delay_ms(100);

	// D+ = 0.6V, D- = 0.6V for 1.25s
	IO(PORTB, 
		D_PLUS_0V6, 1,
		D_MINUS_0V6, 1
	);
	_delay_ms(1500);

	// D- to 0V for 1ms
	IO(PORTB, D_MINUS_0V6, 0);
	_delay_ms(2);

	// Set voltage
	// (now: D+ = 0.6V, D- = 0)
	// D+ must not be low; it cause reset.
	switch (voltage) {
		case E_12V:
			// D+ = 0.6V, D- = 0.6V
			IO(PORTB, D_MINUS_0V6, 1);
			break;
		case E_9V:
			// D+ = 3.3V, D- = 0.6V
			IO(PORTB, D_MINUS_3V3, 1);
			IO(DDRB,
				D_PLUS_3V3, 1,
				D_PLUS_0V6, 0
			);
			IO(PORTB, D_MINUS_0V6, 0);

			IO(PORTB, D_MINUS_0V6, 1);
			break;
		default:
			// nothing to do
			break;
	}

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();

	for (;;) {
		// nothing to do
	}
}
