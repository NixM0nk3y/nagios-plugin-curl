#if defined LINUX
#if OS_MAJOR_VERSION == 2
#if OS_MINOR_VERSION == 6
#define _XOPEN_SOURCE 600
#define HAVE_STDBOOL_H
#define HAVE_STDINT_H
#define HAVE_VSNPRINTF
#define HAVE_SNPRINTF
#define HAVE_VASPRINTF
#define HAVE_ASPRINTF
#define HAVE_STRDUP
#define HAVE_LOCKF
#else
	#error unknown platform
#endif /* defined OS_MINOR_VERSION == 6 */
#else
	#error unknown platform
#endif /* defined OS_MAJOR_VERSION == 2 */
#endif /* defined LINUX */

#if defined FREEBSD
#if OS_MAJOR_VERSION == 7
#if OS_MINOR_VERSION == 0
#define _XOPEN_SOURCE 600
#define HAVE_STDBOOL_H
#define HAVE_STDINT_H
#define HAVE_VSNPRINTF
#define HAVE_SNPRINTF
#define HAVE_VASPRINTF
#define HAVE_ASPRINTF
#define HAVE_STRDUP
#define HAVE_LOCKF
#else
	#error unknown platform
#endif /* defined OS_MINOR_VERSION == 0 */
#else
#if OS_MAJOR_VERSION == 6
#if OS_MINOR_VERSION == 2
#define _XOPEN_SOURCE 600
#define HAVE_STDBOOL_H
#define HAVE_STDINT_H
#define HAVE_VSNPRINTF
#define HAVE_SNPRINTF
#define HAVE_STRDUP
#define HAVE_LOCKF
#else
	#error unknown platform
#endif /* defined OS_MINOR_VERSION == 2 */
#else
	#error unknown platform
#endif /* defined OS_MAJOR_VERSION == 6 */
#endif /* defined OS_MAJOR_VERSION == 7 */
#endif /* defined FREEBSD */

#if defined OPENBSD
#if OS_MAJOR_VERSION == 4
#if OS_MINOR_VERSION >= 2 && OS_MINOR_VERSION <= 3
#define _XOPEN_SOURCE 600
#define HAVE_STDBOOL_H
#define HAVE_STDINT_H
#define HAVE_VSNPRINTF
#define HAVE_SNPRINTF
#define HAVE_VASPRINTF
#define HAVE_ASPRINTF
#define HAVE_STRDUP
#define HAVE_LOCKF
#else
	#error unknown platform
#endif /* defined OS_MINOR_VERSION >= 2 && OS_MINOR_VERSION <= 3 */
#else
	#error unknown platform
#endif /* defined OS_MAJOR_VERSION == 4 */
#endif /* defined OPENBSD */

#if defined SUNOS
#if OS_MAJOR_VERSION == 5
#if OS_MINOR_VERSION == 8
#if !defined __cplusplus
#define _XOPEN_SOURCE 600
#define __EXTENSIONS__
#endif
#define HAVE_SNPRINTF
#define HAVE_VSNPRINTF
#define HAVE_LOCKF
#define HAVE_ENUM_BOOL
#define HAVE_LINK_H
#else
#if OS_MINOR_VERSION == 10
#if !defined __cplusplus 
#define _XOPEN_SOURCE 600
#define __EXTENSIONS__
#endif
#define HAVE_SNPRINTF
#define HAVE_VSNPRINTF
#define HAVE_LOCKF
#define HAVE_STDBOOL_H
#define HAVE_STDINT_H
#define HAVE_STRERROR_R
#else
	#error unknown platform
#endif /* OS_MINOR_VERSION == 10 */
#endif /* OS_MINOR_VERSION == 8 */
#else
	#error unknown platform
#endif /* OS_MAJOR_VERSION == 5 */
#endif /* defined SUNOS */

#if defined CYGWIN
#if OS_MAJOR_VERSION == 5
#if OS_MINOR_VERSION == 0
#define _XOPEN_SOURCE 600
#define HAVE_ENUM_BOOL
#else
	#error unknown platform
#endif /* OS_MINOR_VERSION == 0 */
#else
	#error unknown platform
#endif /* OS_MAJOR_VERSION == 5 */
#endif /* defined CYGWIN */
