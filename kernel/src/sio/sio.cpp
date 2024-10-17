// SPDX-License-Identifier: MPL-2.0

#include "memutil.hpp"

#include "sio/sio.hpp"
#include "gs/gs.hpp"

namespace sio
{
	void sio_putc(char c)
	{
		while ((_l32(SIO_ISR) & 0xf000) == 0x8000)
			;

		_s8(SIO_TXFIFO, c);
	}

	void sio_puts(const char* str)
	{
		const char* c = str;
		while (*c)
		{
			sio_putc(*c);
			c++;
		}
	}

	char sio_getc(bool block)
	{
		if (block)
		{
			while ((_l32(SIO_ISR) & 0xf00) == 0)
				;
		}
		else
		{
			if ((_l32(SIO_ISR) & 0xf00) == 0)
			{
				return -1;
			}
		}

		char b = _l8(SIO_RXFIFO);
		_s8(SIO_ISR, 7);
		return b;
	}

	int sio_getsn(char* str, size_t len)
	{
		char* s = str;

		while (--len)
		{
			int c;

			c = sio_getc(true);
			if (c == '\n' || c == '\r')
				break;

			*s++ = c;
		}

		*s = '\0';
		return len;
	}
} // namespace sio

// Required by printf
void _putchar(char c)
{
	sio::sio_putc(c);
	gs::vterm_putc(c);
}
