#include <sdk.h>
#include "console.h"

// configure a gpio 	gpio_output(PIN);
// write a gpio  	gpio_write(PIN, STATE);
// delay a bit		delay_ms()

int main(void) 
{
  wdt_disable();

  int tx = 3, rx = 2, baud = 115200;
  uart_init(1, tx, rx, baud);

  for (;;) 
  {
	ConsoleService();
//    uint8_t c = 0;
//    if (uart_read(0, &c)) uart_write(1, c);
//    if (uart_read(1, &c)) uart_write(0, c);
  }

  return 0;
}
