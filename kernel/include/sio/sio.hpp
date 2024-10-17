// SPDX-License-Identifier: MPL-2.0

#pragma once

#define SIO_IER 0x1000f120
#define SIO_ISR 0x1000f130
#define SIO_TXFIFO 0x1000f180
#define SIO_RXFIFO 0x1000f1C0

namespace sio
{
	void sio_putc(char c);
	void sio_puts(const char* str);

	char sio_getc(bool block = true);
	int sio_getsn(char* str, size_t len);
} // namespace sio

// Required by printf
extern "C" void _putchar(char c);
