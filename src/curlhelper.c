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

#include <string.h>		/* for memcpy */
#include <stdlib.h>		/* for malloc, free */

#include "cmdline.h"

#include "curlhelper.h"

int curlhelp_initbuffer( curlhelp_curlbuf *buf ) {
	buf->bufsize = 2048;
	buf->buflen = 0;
	buf->buf = (char *)malloc( (size_t)2048 ); 
	if( buf->buf == NULL ) return -1;
	return 0;
}

int curlhelp_buffer_callback( void *buffer, size_t size, size_t nmemb, void *stream ) {
	curlhelp_curlbuf *buf = (curlhelp_curlbuf *)stream;

	while( buf->bufsize < buf->buflen + size * nmemb + 1 ) {
		buf->bufsize *= buf->bufsize * 1.3;
		buf->buf = (char *)realloc( buf->buf, buf->bufsize );
		if( buf->buf == NULL ) {
			return -1;
		}
	}

	memcpy( buf->buf + buf->buflen, buffer, size * nmemb );
	buf->buflen += size * nmemb;
	buf->buf[buf->buflen] = '\0';

	return size * nmemb;
}

void curlhelp_freebuffer( curlhelp_curlbuf *buf ) {
	free( buf->buf );
	buf->buf = NULL;
}

int curlhelp_parse_statusline( char *buf, curlhelp_statusline *status_line ) {
	char *first_line_end;
	char *p;
	size_t first_line_len;
	char *pp;

	first_line_end = strstr( buf, "\r\n" );
	if( first_line_end == NULL ) return -1;

	first_line_len = first_line_end - buf;
	status_line->first_line = (char *)malloc( first_line_len + 1 );
	if( status_line->first_line == NULL ) return -1;
	memcpy( status_line->first_line, buf, first_line_len );
	status_line->first_line[first_line_len] = '\0';

	/* protocol and version: "HTTP/x.x" SP */

	p = strtok( status_line->first_line, "/" );
	if( p == NULL ) { free( status_line->first_line ); return -1; }
	if( strcmp( p, "HTTP" ) != 0 ) { free( status_line->first_line ); return -1; }

	p = strtok( NULL, "." );
	if( p == NULL ) { free( status_line->first_line ); return -1; }
	status_line->http_major = strtol( p, &pp, 10 );
	if( *pp != '\0' ) { free( status_line->first_line ); return -1; }

	p = strtok( NULL, " " );
	if( p == NULL ) { free( status_line->first_line ); return -1; }
	status_line->http_minor = strtol( p, &pp, 10 );
	if( *pp != '\0' ) { free( status_line->first_line ); return -1; }

	/* status code: "404" or "404.1", then SP */

	p = strtok( NULL, " ." );
	if( p == NULL ) { free( status_line->first_line ); return -1; }
	if( strchr( p, '.' ) != NULL ) {
		char *ppp;
		ppp = strtok( p, "." );
		status_line->http_code = strtol( ppp, &pp, 10 );
		if( *pp != '\0' ) { free( status_line->first_line ); return -1; }

		ppp = strtok( NULL, "" );
		status_line->http_subcode = strtol( ppp, &pp, 10 );
		if( *pp != '\0' ) { free( status_line->first_line ); return -1; }
	} else {
		status_line->http_code = strtol( p, &pp, 10 );
		status_line->http_subcode = -1;
		if( *pp != '\0' ) { free( status_line->first_line ); return -1; }
	}

	/* Human readable message: "Not Found" CRLF */

	p = strtok( NULL, "" );
	if( p == NULL ) { free( status_line->first_line ); return -1; }
	status_line->msg = p;

	return 0;
}

void curlhelp_free_statusline( curlhelp_statusline *status_line ) {
	free( status_line->first_line );
}
