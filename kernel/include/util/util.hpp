// SPDX-License-Identifier: MPL-2.0

#pragma once

constexpr int kstrcmp(const char* a, const char* b)
{
	while (*a && *b && *a == *b)
	{
		a++;
		b++;
	}
	return *a - *b;
}

constexpr int kstrncmp(const char* a, const char* b, size_t n)
{
	while (n-- && *a && *b && *a == *b)
	{
		a++;
		b++;
	}
	return n ? *a - *b : 0;
}

constexpr char* kstrstr(const char* str1, const char* str2)
{
	while (*str1)
	{
		const char* s1 = str1;
		const char* s2 = str2;
		while (*s1 && *s2 && *s1 == *s2)
		{
			s1++;
			s2++;
		}
		if (!*s2)
			return (char*)str1;
		str1++;
	}
	return nullptr;
}

constexpr size_t kstrlen(const char* str)
{
	size_t len = 0;
	while (*str++)
		len++;
	return len;
}

constexpr void memcpy(void* dest, const void* src, size_t count)
{
	char* d = (char*)dest;
	const char* s = (const char*)src;
	while (count--)
		*d++ = *s++;
}

constexpr void memset(void* dest, int val, size_t count)
{
	char* d = (char*)dest;
	while (count--)
		*d++ = val;
}

constexpr long strtol(const char* str, char** endptr, int base)
{
	long ret = 0;
	bool neg = false;

	if (*str == '-')
	{
		neg = true;
		str++;
	}

	while (*str)
	{
		int digit = 0;
		if (*str >= '0' && *str <= '9')
		{
			digit = *str - '0';
		}
		else if (base == 16 && *str >= 'a' && *str <= 'f')
		{
			digit = 10 + (*str - 'a');
		}
		else if (base == 16 && *str >= 'A' && *str <= 'F')
		{
			digit = 10 + (*str - 'A');
		}
		else
		{
			break;
		}

		ret = ret * base + digit;
		str++;
	}

	if (endptr)
	{
		*endptr = (char*)str;
	}

	return neg ? -ret : ret;
}
