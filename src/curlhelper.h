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

#ifndef __CURLHELPER_H
#define __CURLHELPER_H

#include <sys/types.h>		/* for size_t */

/* callbacks for body, header, debug output */

typedef struct {
	char *buf;
	size_t buflen;
	size_t bufsize;
} curlhelp_curlbuf;

int curlhelp_initbuffer( curlhelp_curlbuf *buf );
int curlhelp_buffer_callback( void *buffer, size_t size, size_t nmemb, void *stream );
void curlhelp_freebuffer( curlhelp_curlbuf *buf );

/* parsing */

typedef struct {
	int http_major;		/* major version of the protocol, always 1 (HTTP/0.9
				 * never reached the big internet most likely) */
	int http_minor;		/* minor version of the protocol, usually 0 or 1 */
	int http_code;		/* HTTP return code as in RFC 2145 */
	int http_subcode;	/* Microsoft IIS extension, HTTP subcodes, see
				 * http://support.microsoft.com/kb/318380/en-us */
	char *msg;		/* the human readable message */
	char *first_line;	/* a copy of the first line */
} curlhelp_statusline;

int curlhelp_parse_statusline( char *buf, curlhelp_statusline *status_line );
void curlhelp_free_statusline( curlhelp_statusline *status_line );

#endif /* __CURLHELPER_H */
