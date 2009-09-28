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

#include <stdio.h>		/* for printf */
#include <stdlib.h>		/* for exit */
#include <string.h>		/* for strstr */

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include "cmdline.h"

#include "common.h"
#include "curlhelper.h"
#include "utils.h"

#define DEFAULT_TIMEOUT (double)10.0

int main( int argc, char *argv[] ) {
	struct gengetopt_args_info args_info;
	curlhelp_curlbuf body_buf;
	curlhelp_curlbuf header_buf;
	CURL *curl;
	char b[2048];
	char b2[2048];
	CURLcode res;
	char errbuf[CURL_ERROR_SIZE+1];
	struct curl_slist *header_list = NULL;
	long code;
	double total_time;
	const char *level_str = "OK";
	int exit_state = STATE_OK;
	char perfstring[1024];
	curlhelp_statusline status_line;

	cmdline_parser_init( &args_info );

	/* read command line arguments (to get the name of the configuration file,
	 * do it without checking for required arguments
	 */
	if( cmdline_parser2( argc, argv, &args_info, 0, 0, 0 ) != 0 ) {
		printf( "HTTP CRITICAL - unable to parse arguments\n" );
		fprintf( stderr, "\n%s\n", gengetopt_args_info_usage );
		cmdline_parser_free( &args_info );
		exit( STATE_UNKNOWN );
	}

	if( args_info.config_file_given ) {
		/* read command line options from file, allow override of configuration
		 * options from the command line and check for required options
 		 */
		if( cmdline_parser_configfile( args_info.config_file_arg, &args_info, 1, 0, 1 ) != 0 ) {
			printf( "HTTP CRITICAL - unable to read '%s'\n", args_info.config_file_arg );
			fprintf( stderr, "\n%s\n", gengetopt_args_info_usage );
			cmdline_parser_free( &args_info );
			exit( STATE_UNKNOWN );
		}
	} else {
		/* read command line options again, this time checking for required options */
		if( cmdline_parser2( argc, argv, &args_info, 1, 0, 1 ) != 0 ) {
			printf( "HTTP CRITICAL - illegal arguments, check help page\n" );
			fprintf( stderr, "\n%s\n", gengetopt_args_info_usage );
			cmdline_parser_free( &args_info );
			exit( STATE_UNKNOWN );
		}
	}

	/* start up curl */
	if( curl_global_init( (long)CURL_GLOBAL_ALL ) != CURLE_OK ) {
		printf( "HTTP CRITICAL - curl_global_init failed!\n" );
		cmdline_parser_free( &args_info );
		exit( STATE_UNKNOWN );
	}
	if( ( curl = curl_easy_init( ) ) == NULL ) {
		printf( "HTTP CRITICAL - curl_easy_init failed!\n" );
		cmdline_parser_free( &args_info );
		curl_global_cleanup( );
		exit( STATE_UNKNOWN );
	}

	/* -v: verbosity */
	curl_easy_setopt( curl, CURLOPT_VERBOSE, args_info.verbose_given );

	/* initialize buffer for body of the answer */
	if( curlhelp_initbuffer( &body_buf ) < 0 ) {
		printf( "HTTP CRITICAL - out of memory allocating a buffer\n" );
		cmdline_parser_free( &args_info );
		curl_global_cleanup( );
		exit( STATE_UNKNOWN );
	}
	curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, curlhelp_buffer_callback );
	curl_easy_setopt( curl, CURLOPT_WRITEDATA, (void *)&body_buf );

	/* initialize buffer for header of the answer */
	if( curlhelp_initbuffer( &header_buf ) < 0 ) {
		printf( "HTTP CRITICAL - out of memory allocating a buffer\n" );
		curlhelp_freebuffer( &body_buf );
		cmdline_parser_free( &args_info );
		curl_global_cleanup( );
		exit( STATE_UNKNOWN );
	}
	curl_easy_setopt( curl, CURLOPT_HEADERFUNCTION, curlhelp_buffer_callback );
	curl_easy_setopt( curl, CURLOPT_WRITEHEADER , (void *)&header_buf );

	/* authentication */
	if( args_info.authorization_given )
		curl_easy_setopt( curl, CURLOPT_USERPWD, args_info.authorization_arg );

	/* user agent */
	if( args_info.useragent_given )
		curl_easy_setopt( curl, CURLOPT_USERAGENT, args_info.useragent_arg );

	/* compose URL */
	snprintf( b, (size_t)2048, "http%s://%s%s",
		args_info.ssl_given ? "s" : "",
		args_info.ip_arg,
		args_info.url_given ? args_info.url_arg : "/" );
	curl_easy_setopt( curl, CURLOPT_URL, b );

	/* set port */
	if( args_info.port_given )
		curl_easy_setopt( curl, CURLOPT_PORT, args_info.port_arg );

	/* compose HTTP headers */
	if( args_info.host_given ) {
		snprintf( b2, (size_t)2048, "Host: %s", args_info.host_arg );
		header_list = curl_slist_append( header_list, b );
	}
	curl_easy_setopt( curl, CURLOPT_HTTPHEADER, header_list );

	/* set the error buffer */
	curl_easy_setopt( curl, CURLOPT_ERRORBUFFER, errbuf );

	/* set timeouts */
	{
		long timeout;

		if( args_info.timeout_given )
			timeout = args_info.timeout_arg;
		else
			timeout = DEFAULT_TIMEOUT;

		curl_easy_setopt( curl, CURLOPT_CONNECTTIMEOUT, timeout );
		curl_easy_setopt( curl, CURLOPT_TIMEOUT, timeout );
	}
	
	/* --insecure: choose level of CA chain validation (SSL) */
	curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 2 );
	if( args_info.insecure_given ) {
		curl_easy_setopt( curl, CURLOPT_SSL_VERIFYHOST, 1 );
		curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0 );
	}

	/* do the request */
	res = curl_easy_perform( curl );	

	/* terminate buffer, print it if verbose */
	if( args_info.verbose_given ) {
		puts( "--- HEADER ---" );
		puts( header_buf.buf );
	}
	if( args_info.verbose_given ) {
		puts( "--- BODY ---" );
		puts( body_buf.buf );
	}

	/* free header list, we don't need it anymore */
	curl_slist_free_all( header_list );

	/* Curl errors, result in critical Nagios state */
	if( res != CURLE_OK ) {
		remove_newlines( errbuf );
		printf( "HTTP CRITICAL - %s (error: %d)\n", errbuf, res );
		curlhelp_freebuffer( &body_buf );
		curlhelp_freebuffer( &header_buf );
		curl_easy_cleanup( curl );
		curl_global_cleanup( );
		exit( STATE_CRITICAL );
	}

	/* we got the data and we executed the request in a given time, so we can append
	 * performance data to the answer always
	 */
	curl_easy_getinfo( curl, CURLINFO_TOTAL_TIME, &total_time );
	snprintf( perfstring, (size_t)1024, "time=%.6gs;%.6g;%.6g;%.6g size=%dB;;;0",
		total_time,
		args_info.warning_given ? args_info.warning_arg : 0.0,
		args_info.critical_given ? args_info.critical_arg : 0.0,
		0.0,
		(int)body_buf.buflen );

	/* -s: check if the excepted string matches */
	if( args_info.string_given ) {
		if( strstr( body_buf.buf, args_info.string_arg ) == NULL ) {
			printf( "HTTP CRITICAL - string not found|%s\n", perfstring );
			curlhelp_freebuffer( &body_buf );
			curlhelp_freebuffer( &header_buf );
			curl_easy_cleanup( curl );
			curl_global_cleanup( );
			exit( STATE_CRITICAL );
		}
	}

	/* -w, -c: check warning and critical level */
	if( args_info.critical_given && ( total_time >  args_info.critical_arg ) ) {
		level_str = "CRITICAL";
		exit_state = STATE_CRITICAL;
	}
	if( !( exit_state == STATE_CRITICAL ) && args_info.warning_given && ( total_time > args_info.warning_arg ) ) {
		level_str = "WARNING";
		exit_state = STATE_WARNING;
	}

	/* get status line of answer, check sanity of HTTP code */
	if( curlhelp_parse_statusline( header_buf.buf, &status_line ) < 0 ) {
		printf( "HTTP CRITICAL HTTP/1.x %ld unknown - Unparseable status line in %.3g seconds response time|%s\n",
			 code, total_time, perfstring );
		curlhelp_freebuffer( &body_buf );
		curlhelp_freebuffer( &header_buf );
		curl_easy_cleanup( curl );
		curl_global_cleanup( );
		exit( STATE_CRITICAL );
	}
	curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &code );
	if( status_line.http_code != code ) {
		printf( "HTTP CRITICAL HTTP/%d.%d %d %s - different HTTP codes (cUrl has %ld) in %.3g seconds response time|%s\n",
			status_line.http_major, status_line.http_minor,
			status_line.http_code, status_line.msg, code,
			total_time, perfstring );
		curlhelp_free_statusline( &status_line );
		curlhelp_freebuffer( &body_buf );
		curlhelp_freebuffer( &header_buf );
		curl_easy_cleanup( curl );
		curl_global_cleanup( );
		exit( STATE_CRITICAL );
	}

	/* check status codes, set exit status accordingly */
	if( code >= 600 || code <= 100 ) {
		printf( "HTTP CRITICAL HTTP/%d.%d %d %s - Illegal status code (status line: %.40s)\n",
			status_line.http_major, status_line.http_minor,
			status_line.http_code, status_line.msg,
			status_line.first_line );
		curlhelp_free_statusline( &status_line );
		curlhelp_freebuffer( &body_buf );
		curlhelp_freebuffer( &header_buf );
		curl_easy_cleanup( curl );
		curl_global_cleanup( );
		exit( STATE_CRITICAL );
	}
	if( code >= 500 ) {
		level_str = "CRITICAL";
		exit_state = STATE_CRITICAL;
	} else if( code >= 400 ) {
		level_str = "WARNING";
		exit_state = STATE_WARNING;
	}

	printf( "HTTP %s HTTP/%d.%d %d %s - %.3g seconds response time|%s\n",
		level_str, status_line.http_major, status_line.http_minor,
		status_line.http_code, status_line.msg,
		total_time, perfstring );

	curlhelp_free_statusline( &status_line );
	curlhelp_freebuffer( &body_buf );
	curlhelp_freebuffer( &header_buf );
	curl_easy_cleanup( curl );
	curl_global_cleanup( );
	exit( exit_state );
}
