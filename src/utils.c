/*
    check_curl - Nagios Curl-based check plugin
    Copyright (C) 2008  Andreas Baumann

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define _XOPEN_SOURCE 600
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "utils.h"

void remove_newlines( char *s ) {
	char *p;

	for( p = s; *p != '\0'; p++ )
		if( *p == '\r' || *p == '\n' )
			*p = ' ';
}
