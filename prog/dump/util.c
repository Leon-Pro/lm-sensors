/*
    util.c - helper functions
    Copyright (C) 2006 Jean Delvare <khali@linux-fr.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/

#include <stdio.h>
#include "util.h"

/* To keep glibc2 happy */
#if defined(__GLIBC__) && __GLIBC__ == 2 && __GLIBC_MINOR__ >= 0
#include <sys/io.h>
#else
#include <asm/io.h>
#endif

/* Return 1 if we should continue, 0 if we should abort */
int user_ack(int def)
{
	char s[2];
	int ret;

	if (!fgets(s, 2, stdin))
		return 0; /* Nack by default */

	switch (s[0]) {
	case 'y':
	case 'Y':
		ret = 1;
		break;
	case 'n':
	case 'N':
		ret = 0;
		break;
	default:
		ret = def;
	}

	/* Flush extra characters */
	while (s[0] != '\n') {
		int c = fgetc(stdin);
		if (c == EOF) {
			ret = 0;
			break;
		}
		s[0] = c;
	}

	return ret;
}

/* I/O read of specified size */
unsigned long inx(int addr, int width)
{
	switch (width) {
	case 2:
		return inw(addr);
		break;
	case 4:
		return inl(addr);
		break;
	default:
		return inb(addr);
	}
}
