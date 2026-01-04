/* localmisc HEADER */
/* charset=ISO8859-1 */
/* lang=C20 */

/* miscellaneous stuff which essentially every program wants! */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-15, David A­D­ Morano
	This code was started to make life easier on the outside
	(outside of Lucent Technologies).  This file largely contains
	those things (defines) that I have found to be either useful
	or problematic in the past.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	LOCALMISC_INCLUDE
#define	LOCALMISC_INCLUDE


#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>
#include	<sys/param.h>
#include	<limits.h>
#include	<stdlib.h>		/* |size_t| */
#include	<string.h>		/* |memset(3c)| */
#include	<clanguage.h>


#ifndef	TRUE
#define	TRUE		1
#endif

#ifndef	FALSE
#define	FALSE		0
#endif

#ifndef	YES
#define	YES		1
#endif

#ifndef	NO
#define	NO		0
#endif

#ifndef	OK
#define	OK		0
#endif

#ifndef	BAD
#define	BAD		-1
#endif


#ifndef	MIN
#define	MIN(a,b)	(((a) < (b)) ? (a) : (b))
#endif

#ifndef	MAX
#define	MAX(a,b)	(((a) > (b)) ? (a) : (b))
#endif

#ifndef	ABS
#define	ABS(a)		(((a) < 0) ? (- (a)) : (a))
#endif

#ifndef	LEQUIV /* should be operator » !^^ « */
#define	LEQUIV(a,b)	(((a) && (b)) || ((! (a)) && (! (b))))
#endif

#ifndef	LXOR /* should be operator » ^^ « */
#define	LXOR(a,b)	(((a) && (! (b))) || ((! (a)) && (b)))
#endif

#ifndef	BFLOOR
#define	BFLOOR(v,m)	((v) & (~ ((m) - 1)))
#endif

#ifndef	BCEIL
#define	BCEIL(v,m)	(((v) + ((m) - 1)) & (~ ((m) - 1)))
#endif

#ifndef	CEILINT
#define	CEILINT(v)	BCEIL((v),int(sizeof(int)))
#endif

#ifndef	CONMSGHDR_SPACE
#define	CONMSGHDR_SPACE(len) \
    			CEILINT(int(sizeof(struct cmsghdr)) + CEILINT(len))
#endif

#ifndef	CONMSGHDR_LEN
#define	CONMSGHDR_LEN(len)	\
    			CEILINT(int(sizeof(struct cmsghdr)) + (len))
#endif

#ifndef	MKCHAR
#define	MKCHAR(ch)	((ch) & 0xff)
#endif

#ifndef	MKBOOL
#define	MKBOOL(exp)	((exp) != 0)
#endif

#ifndef	UC
#define	UC(ch)		((unsigned char) (ch))
#endif

#ifndef	MODP2
#define	MODP2(v,n)	((v) & ((n) - 1))
#endif

/* basic scalar types */

#ifndef	LONG
#define	LONG		long
#endif

#ifndef	SCHAR
#define	SCHAR		signed char
#endif

#ifndef	UCHAR
#define	UCHAR		unsigned char
#endif

#ifndef	USHORT
#define	USHORT		unsigned short
#endif

#ifndef	UINT
#define	UINT		unsigned int
#endif

#ifndef	ULONG
#define	ULONG		unsigned long
#endif

#ifndef	TYPEDEF_SCHAR
#define	TYPEDEF_SCHAR
typedef signed char		schar ;
#endif /* TYPEDEF_SCHAR */

#ifndef	TYPEDEF_UCHAR
#define	TYPEDEF_UCHAR
typedef unsigned char		uchar ;
#endif

#ifndef	TYPEDEF_USHORT
#define	TYPEDEF_USHORT
typedef unsigned short		ushort ;
#endif

#ifndef	TYPEDEF_UINT
#define	TYPEDEF_UINT
typedef unsigned int		uint ;
#endif

#ifndef	TYPEDEF_ULONG
#define	TYPEDEF_ULONG
typedef unsigned long		ulong ;
#endif

#ifndef	TYPEDEF_CSCHAR
#define	TYPEDEF_CSCHAR
typedef const signed char	cschar ;
#endif /* TYPEDEF_SCHAR */

#ifndef	TYPEDEF_CCHAR
#define	TYPEDEF_CCHAR
typedef const char		cchar ;
#endif /* TYPEDEF_SCHAR */

#ifndef	TYPEDEF_CSHORT
#define	TYPEDEF_CSHORT
typedef const short		cshort ;
#endif /* TYPEDEF_CSHORT */

#ifndef	TYPEDEF_CINT
#define	TYPEDEF_CINT
typedef const int		cint ;
#endif /* TYPEDEF_CINT */

#ifndef	TYPEDEF_CLONG
#define	TYPEDEF_CLONG
typedef const long		clong ;
#endif /* TYPEDEF_CLONG */

#ifndef	TYPEDEF_CUCHAR
#define	TYPEDEF_CUCHAR
typedef const unsigned char	cuchar ;
#endif /* TYPEDEF_UCHAR */

#ifndef	TYPEDEF_CUSHORT
#define	TYPEDEF_CUSHORT
typedef const unsigned short	cushort ;
#endif

#ifndef	TYPEDEF_CUINT
#define	TYPEDEF_CUINT
typedef const unsigned int	cuint ;
#endif

#ifndef	TYPEDEF_CULONG
#define	TYPEDEF_CULONG
typedef const unsigned long	culong ;
#endif

#ifndef	TYPEDEF_USTIME
#define	TYPEDEF_USTIME
typedef time_t			ustime ;
#endif

#ifndef	TYPEDEF_UNIXTIME
#define	TYPEDEF_UNIXTIME
typedef time_t			unixtime ;
#endif

#ifndef	TYPEDEF_CUSTIME
#define	TYPEDEF_CUSTIME
typedef const time_t		custime ;
#endif

#ifndef	TYPEDEF_CUNIXTIME
#define	TYPEDEF_CUNIXTIME
typedef const time_t		cunixtime ;
#endif

#ifndef	TYPEDEF_CC
#define	TYPEDEF_CC
typedef const char		cc ;
#endif

/* C-language limits */

#ifndef	INT64_MIN
#define	INT64_MIN	(-9223372036854775807L-1LL)
#endif

#ifndef	INT64_MAX
#define	INT64_MAX	9223372036854775807LL
#endif

#ifndef	UINT64_MAX
#define	UINT64_MAX	18446744073709551615ULL
#endif

/* it would be nice if the implemenation had these */

#ifndef	SHORT_MIN
#ifdef	SHRT_MIN
#define	SHORT_MIN	SHRT_MIN
#else
#define	SHORT_MIN	(-32768)	/* min value of a "short int" */
#endif
#endif

#ifndef	SHORT_MAX
#ifdef	SHRT_MAX
#define	SHORT_MAX	SHRT_MAX
#else
#define	SHORT_MAX	32767		/* max value of a "short int" */
#endif
#endif

#ifndef	USHORT_MAX
#ifdef	USHRT_MAX
#define	USHORT_MAX	USHRT_MAX
#else
#define	USHORT_MAX	65535		/* max value of "unsigned short int" */
#endif
#endif


/* parameters */

#ifndef	MAXARGLEN
#ifdef	ARG_MAX
#define	MAXARGLEN	ARG_MAX
#else
#define	MAXARGLEN	(256 * 1024)
#endif
#endif

#ifndef	MAXLINELEN
#ifdef	LINE_MAX
#define	MAXLINELEN	LINE_MAX
#else
#define	MAXLINELEN	(2*1024)
#endif
#endif

#ifndef	MAXNAMELEN
#ifdef	NAME_MAX
#define	MAXNAMELEN	NAME_MAX
#else
#define	MAXNAMELEN	256
#endif
#endif

#ifndef	MAXPATHLEN
#ifdef	PATH_MAX
#define	MAXPATHLEN	PATH_MAX
#else
#define	MAXPATHLEN	1024
#endif
#endif

/* timezone (zoneinfo) name */
#ifndef	TZNAMELEN
#ifdef	TZNAME_MAX
#define	TZNAMELEN	TZNAME_MAX
#else
#define	TZNAMELEN	256
#endif
#endif

#ifndef	NODENAMELEN
#define	NODENAMELEN	256
#endif

#ifndef	HOSTNAMELEN
#define	HOSTNAMELEN	1024
#endif

/* timezone abbreviation */
#ifndef	TZABBRLEN
#define	TZABBRLEN	8
#endif

/* log-ID (for logging) */
#ifndef	LOGIDLEN
#define	LOGIDLEN	15
#endif

/* mail alias-length */
#ifndef	MAILALIASLEN
#define	MAILALIASLEN	64
#endif

/* this is (long) depracated from UNIX® */
#ifndef	NOFILE
#define	NOFILE		20		/* UNIX® number of files */
#endif

#ifndef	TIMEBUFLEN
#define	TIMEBUFLEN	80		/* can hold? all known date strings */
#endif

/* common digit base (2, 8, 10, 16) buffer lengths (convenience defines) */

#ifndef	BINBUFLEN
#define	BINBUFLEN	256		/* can hold |int256_t| in binary */
#endif

#ifndef	OCTBUFLEN
#define	OCTBUFLEN	86		/* can hold |int256_t| in octal */
#endif

#ifndef	DECBUFLEN
#define	DECBUFLEN	78		/* can hold |int256_t| in decimal */
#endif

#ifndef	HEXBUFLEN
#define	HEXBUFLEN	64		/* can hold |int256_t| in hexadecimal */
#endif

#ifndef	DIGBUFLEN
#define	DIGBUFLEN	MAX(MAX(MAX(BINBUFLEN,OCTBUFLEN),DECBUFLEN),HEXBUFLEN)
#endif

#ifndef	REALNAMELEN
#define	REALNAMELEN	100		/* "real" name length */
#endif

#ifndef	COLUMNS
#define	COLUMNS		80		/* historical terminal columns */
#endif

#ifndef	POLL_INTMULT
#define	POLL_INTMULT	1000		/* poll-interval-multiplier (to secs) */
#endif

#ifndef	NYEARS_CENTURY
#define	NYEARS_CENTURY	100		/* years in a century */
#endif

#ifndef	NTABCOLS
#define	NTABCOLS	8		/* eight columns per TAB character */
#endif

#define	eol		'\n'

#ifndef	SUBROUTINE_LEQUIV
#define	SUBROUTINE_LEQUIV

static inline bool lequiv(bool a1,bool a2) noex {
	return LEQUIV(a1,a2) ;
}

static inline bool lxor(bool a1,bool a2) noex {
	return LXOR(a1,a2) ;
}

#endif /* SUBROUTINE_LEQUIV */

#ifdef	__cplusplus
#else
#ifndef	SUBROUTINE_MEMCLEAR
#define	SUBROUTINE_MEMCLEAR
static inline int memclear(void *objp,int sz) noex {
    	csize	objs = (size_t) sz ;
    	memset(objp,0,objs) ;
	return sz ;
}
#endif /* SUBROUTINE_MEMCLEAR */
#endif /* __splusplus */


#endif /* LOCALMISC_INCLUDE */


