#include "log.h"
#include <string.h>

static void logc(int c) {
  extern void uart_write(int, unsigned char);
  uart_write(0, (unsigned char) c);
}

int putchar(int c) {
  logc(c);
  return c;
}

static void logs(const char *buf, size_t len) {
  for (size_t i = 0; i < len; i++) logc(buf[i]);
}

static unsigned char nibble(unsigned char c) {
  return (unsigned char) (c < 10 ? c + '0' : c + 'W');
}

#define ISPRINT(x) ((x) >= ' ' && (x) <= '~')
void hexdump(const void *buf, size_t len) {
  const unsigned char *p = buf;
  unsigned char ascii[16] = "", alen = 0;
  for (size_t i = 0; i < len; i++) {
    if ((i % 16) == 0) {
      // Print buffered ascii chars
      if (i > 0) logs("  ", 2), logs((char *) ascii, 16), logc('\n'), alen = 0;
      // Print hex address, then \t
      logc(nibble((i >> 12) & 15)), logc(nibble((i >> 8) & 15)),
          logc(nibble((i >> 4) & 15)), logc('0'), logs("   ", 3);
    }
    logc(nibble(p[i] >> 4)), logc(nibble(p[i] & 15));  // Two nibbles, e.g. c5
    logc(' ');                                         // Space after hex number
    ascii[alen++] = ISPRINT(p[i]) ? p[i] : '.';        // Add to the ascii buf
  }
  while (alen < 16) logs("   ", 3), ascii[alen++] = ' ';
  logc('\t'), logs((char *) ascii, 16), logc('\n');
}

#if 0
int putchar(int c) {
  return c;
}

static void logx(unsigned long v) {
  unsigned bits = sizeof(v) * 8, show = 0;
  while (bits) {
    bits -= 4;
    unsigned char c = (v >> bits) & 15;
    if (show == 0 && c == 0) continue;
    logc(nibble(c));
    show = 1;
  }
  if (show == 0) logc('0');
}

static void logd(unsigned long v) {
  unsigned long d = 1000000000;
  while (v < d) d /= 10;
  while (d > 1) {
    unsigned long m = v / d;
    v -= m * d;
    d /= 10;
    logc((unsigned char) (m + '0'));
  }
  logc((unsigned char) (v + '0'));
}

void vlog(const char *fmt, va_list ap) {
  int c, precision;
  while ((c = *fmt++) != '\0') {
    if (c != '%') {
      logc(c);
      continue;
    }
    c = *fmt++, precision = 0;
    if (c == '.') {
      precision = *fmt++ - '0';
      if (precision + '0' == '*') precision = va_arg(ap, int);
      c = *fmt++;
    }
    switch (c) {
      case 's': {
        const char *s = va_arg(ap, char *);
        logs(s, precision > 0 ? (size_t) precision : strlen(s));
        break;
      }
      case 'c':
        logc((unsigned char) va_arg(ap, int));
        break;
      case 'u':
        logd((unsigned long) va_arg(ap, unsigned));
        break;
      case 'p':
        logc('0');
        logc('x');
        logx((unsigned long) va_arg(ap, void *));
        break;
      case 'd': {
        int v = va_arg(ap, int);
        if (v < 0) v = -v, logc('-');
        logd((unsigned long) v);
        break;
      }
      case 'x':
        logx(va_arg(ap, unsigned long));
        break;
      case 0:
        return;
      default:
        logc('%');
        logc(c);
        break;
    }
  }
}

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vlog(fmt, ap);
  va_end(ap);
  return 0;
}
#endif
