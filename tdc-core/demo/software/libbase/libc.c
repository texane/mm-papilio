/*
 * Milkymist SoC (Software)
 * Copyright (C) 2007, 2008, 2009 Sebastien Bourdeauducq
 * Copyright (C) Linux kernel developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>

/**
 * strchr - Find the first occurrence of a character in a string
 * @s: The string to be searched
 * @c: The character to search for
 */
char *strchr(const char *s, int c)
{
	for (; *s != (char)c; ++s)
		if (*s == '\0')
			return NULL;
	return (char *)s;
}

/**
 * strrchr - Find the last occurrence of a character in a string
 * @s: The string to be searched
 * @c: The character to search for
 */
char *strrchr(const char *s, int c)
{
       const char *p = s + strlen(s);
       do {
           if (*p == (char)c)
               return (char *)p;
       } while (--p >= s);
       return NULL;
}

/**
 * strnchr - Find a character in a length limited string
 * @s: The string to be searched
 * @count: The number of characters to be searched
 * @c: The character to search for
 */
char *strnchr(const char *s, size_t count, int c)
{
	for (; count-- && *s != '\0'; ++s)
		if (*s == (char)c)
			return (char *)s;
	return NULL;
}

/**
 * strcpy - Copy a %NUL terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 */
char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}

/**
 * strncpy - Copy a length-limited, %NUL-terminated string
 * @dest: Where to copy the string to
 * @src: Where to copy the string from
 * @count: The maximum number of bytes to copy
 *
 * The result is not %NUL-terminated if the source exceeds
 * @count bytes.
 *
 * In the case where the length of @src is less than  that  of
 * count, the remainder of @dest will be padded with %NUL.
 *
 */
char *strncpy(char *dest, const char *src, size_t count)
{
	char *tmp = dest;
	
	while (count) {
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
		count--;
	}
	return dest;
}

/**
 * strcmp - Compare two strings
 * @cs: One string
 * @ct: Another string
 */
int strcmp(const char *cs, const char *ct)
{
	signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}
	return __res;
}

/**
 * strncmp - Compare two strings using the first characters only
 * @cs: One string
 * @ct: Another string
 * @count: Number of characters
 */
int strncmp(const char *cs, const char *ct, size_t count)
{
	signed char __res;
	size_t n;

	n = 0;
	__res = 0;
	while (n < count) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
		n++;
	}
	return __res;
}

/**
 * strlen - Find the length of a string
 * @s: The string to be sized
 */
size_t strlen(const char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

/**
 * strnlen - Find the length of a length-limited string
 * @s: The string to be sized
 * @count: The maximum number of bytes to search
 */
size_t strnlen(const char *s, size_t count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

/**
 * memcmp - Compare two areas of memory
 * @cs: One area of memory
 * @ct: Another area of memory
 * @count: The size of the area.
 */
int memcmp(const void *cs, const void *ct, size_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

/**
 * memset - Fill a region of memory with the given value
 * @s: Pointer to the start of the area.
 * @c: The byte to fill the area with
 * @count: The size of the area.
 */
void *memset(void *s, int c, size_t count)
{
	char *xs = s;

	while (count--)
		*xs++ = c;
	return s;
}

/**
 * memcpy - Copies one area of memory to another
 * @dest: Destination
 * @src: Source
 * @n: The size to copy.
 */
void *memcpy(void *dest, const void *src, size_t n)
{
	char *_dest = (char *)dest;
	char *_src = (char *)src;
	size_t i;

	for(i=0;i<n;i++)
		_dest[i] = _src[i];

	return dest;
}

/**
 * memmove - Copies one area of memory to another, overlap possible
 * @dest: Destination
 * @src: Source
 * @n: The size to copy.
 */
void *memmove(void *dest, const void *src, size_t count)
{
	char *tmp, *s;

	if(dest <= src) {
		tmp = (char *) dest;
		s = (char *) src;
		while(count--)
			*tmp++ = *s++;
	} else {
		tmp = (char *)dest + count;
		s = (char *)src + count;
		while(count--)
			*--tmp = *--s;
	}

	return dest;
}


/**
 * strtoul - convert a string to an unsigned long
 * @nptr: The start of the string
 * @endptr: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
unsigned long strtoul(const char *nptr, char **endptr, int base)
{
	unsigned long result = 0,value;

	if (!base) {
		base = 10;
		if (*nptr == '0') {
			base = 8;
			nptr++;
			if ((toupper(*nptr) == 'X') && isxdigit(nptr[1])) {
				nptr++;
				base = 16;
			}
		}
	} else if (base == 16) {
		if (nptr[0] == '0' && toupper(nptr[1]) == 'X')
			nptr += 2;
	}
	while (isxdigit(*nptr) &&
	       (value = isdigit(*nptr) ? *nptr-'0' : toupper(*nptr)-'A'+10) < base) {
		result = result*base + value;
		nptr++;
	}
	if (endptr)
		*endptr = (char *)nptr;
	return result;
}

/**
 * strtol - convert a string to a signed long
 * @nptr: The start of the string
 * @endptr: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 */
long strtol(const char *nptr, char **endptr, int base)
{
	if(*nptr=='-')
		return -strtoul(nptr+1,endptr,base);
	return strtoul(nptr,endptr,base);
}

int skip_atoi(const char **s)
{
	int i=0;

	while (isdigit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

char *number(char *buf, char *end, unsigned long num, int base, int size, int precision, int type)
{
	char c,sign,tmp[66];
	const char *digits;
	static const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	static const char large_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	digits = (type & PRINTF_LARGE) ? large_digits : small_digits;
	if (type & PRINTF_LEFT)
		type &= ~PRINTF_ZEROPAD;
	if (base < 2 || base > 36)
		return NULL;
	c = (type & PRINTF_ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & PRINTF_SIGN) {
		if ((signed long) num < 0) {
			sign = '-';
			num = - (signed long) num;
			size--;
		} else if (type & PRINTF_PLUS) {
			sign = '+';
			size--;
		} else if (type & PRINTF_SPACE) {
			sign = ' ';
			size--;
		}
	}
	if (type & PRINTF_SPECIAL) {
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}
	i = 0;
	if (num == 0)
		tmp[i++]='0';
	else while (num != 0) {
		tmp[i++] = digits[num % base];
		num = num / base;
	}
	if (i > precision)
		precision = i;
	size -= precision;
	if (!(type&(PRINTF_ZEROPAD+PRINTF_LEFT))) {
		while(size-->0) {
			if (buf < end)
				*buf = ' ';
			++buf;
		}
	}
	if (sign) {
		if (buf < end)
			*buf = sign;
		++buf;
	}
	if (type & PRINTF_SPECIAL) {
		if (base==8) {
			if (buf < end)
				*buf = '0';
			++buf;
		} else if (base==16) {
			if (buf < end)
				*buf = '0';
			++buf;
			if (buf < end)
				*buf = digits[33];
			++buf;
		}
	}
	if (!(type & PRINTF_LEFT)) {
		while (size-- > 0) {
			if (buf < end)
				*buf = c;
			++buf;
		}
	}
	while (i < precision--) {
		if (buf < end)
			*buf = '0';
		++buf;
	}
	while (i-- > 0) {
		if (buf < end)
			*buf = tmp[i];
		++buf;
	}
	while (size-- > 0) {
		if (buf < end)
			*buf = ' ';
		++buf;
	}
	return buf;
}

/**
 * vscnprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @size: The size of the buffer, including the trailing null space
 * @fmt: The format string to use
 * @args: Arguments for the format string
 *
 * The return value is the number of characters which have been written into
 * the @buf not including the trailing '\0'. If @size is <= 0 the function
 * returns 0.
 *
 * Call this function if you are already dealing with a va_list.
 * You probably want scnprintf() instead.
 */
int vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
	int i;

	i=vsnprintf(buf,size,fmt,args);
	return (i >= size) ? (size - 1) : i;
}


/**
 * snprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @size: The size of the buffer, including the trailing null space
 * @fmt: The format string to use
 * @...: Arguments for the format string
 *
 * The return value is the number of characters which would be
 * generated for the given input, excluding the trailing null,
 * as per ISO C99.  If the return is greater than or equal to
 * @size, the resulting string is truncated.
 */
int snprintf(char * buf, size_t size, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsnprintf(buf,size,fmt,args);
	va_end(args);
	return i;
}

/**
 * scnprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @size: The size of the buffer, including the trailing null space
 * @fmt: The format string to use
 * @...: Arguments for the format string
 *
 * The return value is the number of characters written into @buf not including
 * the trailing '\0'. If @size is <= 0 the function returns 0.
 */

int scnprintf(char * buf, size_t size, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = vsnprintf(buf, size, fmt, args);
	va_end(args);
	return (i >= size) ? (size - 1) : i;
}

/**
 * vsprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @fmt: The format string to use
 * @args: Arguments for the format string
 *
 * The function returns the number of characters written
 * into @buf. Use vsnprintf() or vscnprintf() in order to avoid
 * buffer overflows.
 *
 * Call this function if you are already dealing with a va_list.
 * You probably want sprintf() instead.
 */
int vsprintf(char *buf, const char *fmt, va_list args)
{
	return vsnprintf(buf, INT_MAX, fmt, args);
}

/**
 * sprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @fmt: The format string to use
 * @...: Arguments for the format string
 *
 * The function returns the number of characters written
 * into @buf. Use snprintf() or scnprintf() in order to avoid
 * buffer overflows.
 */
int sprintf(char * buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsnprintf(buf, INT_MAX, fmt, args);
	va_end(args);
	return i;
}

/**
 * rand - Returns a pseudo random number
 */

static unsigned int seed;
unsigned int rand()
{
	seed = 129 * seed + 907633385;
	return seed;
}
