/* misc */

/* miscellaneous stuff which essentially every program wants ! */
/* last modified %G% version %I% */


/* revision history :

	= 00/02/15, Dave Morano
	This code was started.


*/




#ifndef	MISC_INCLUDE
#define	MISC_INCLUDE	1



#ifdef	TRUE
#undef	TRUE
#undef	FALSE
#endif

#define	TRUE		1
#define	FALSE		0

#ifdef	YES
#undef	TES
#undef	NO
#endif

#define	YES		1
#define	NO		0

#ifdef	OK
#undef	OK
#undef	BAD
#endif

#define	OK		0
#define	BAD		-1


#ifndef	NULL
#define	NULL		((void *) 0)
#endif

#ifndef	VOID
#define	VOID		void
#endif

#ifndef	VOLATILE
#define	VOLATILE	volatile
#endif

#ifndef	CONST
#define	CONST		const
#endif


#ifndef	MIN
#define	MIN(a,b)	(((a) < (b)) ? (a) : (b))
#endif

#ifndef	MAX
#define	MAX(a,b)	(((a) > (b)) ? (a) : (b))
#endif

#ifndef	UMIN
#define	UMIN(a,b)	(((unsigned long) (a)) < ((unsigned long) (b))) \
				 ? (a) : (b))
#endif

#ifndef	UMAX
#define	UMAX(a,b)	((((unsigned long) (a)) > ((unsigned long) (b))) \
				? (a) : (b))
#endif

#ifndef	ABS
#define	ABS(a)		(((a) < 0) ? (- (a)) : (a))
#endif

#ifndef	EQUIV
#define	EQUIV(a,b)	(((a) && (b)) || ((! (a)) && (! (b))))
#endif

#ifndef	XOR
#define	XOR(a,b)	(((a) && (! (b))) || ((! (a)) && (b)))
#endif



/* basic scalar types */

#define	CHAR	char
#define	BYTE	char
#define	SHORT	short
#define	INT	int

#ifdef	SOLARIS

#define	LONG	long long

#else

#define	LONG	long

#endif /* SOLARIS */

#define	UCHAR	unsigned char
#define	USHORT	unsigned short
#define	UINT	unsigned int

#ifndef	ULONG
#define	ULONG	unsigned LONG
#endif


#if	(! defined(__EXTENSIONS__)) && (! defined(P_MYID))
#if	defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE)

#ifndef	TYPES_UNSIGNED
#define	TYPES_UNSIGNED	1

typedef unsigned short	ushort ;
typedef unsigned int	uint ;
typedef unsigned long	ulong ;

#define	TYPE_USHORT	1
#define	TYPE_UINT	1
#define	TYPE_ULONG	1

#endif /* TYPES_UNSIGNED */

#endif
#endif


/* do it again ! */

#if	(! defined(TYPES_UNSIGNED)) && (! defined(P_MYID))
#define	TYPES_UNSIGNED	1

typedef unsigned short	ushort ;
typedef unsigned int	uint ;
typedef unsigned long	ulong ;

#define	TYPE_USHORT	1
#define	TYPE_UINT	1
#define	TYPE_ULONG	1

#endif /* TYPES_UNSIGNED */



#ifndef	TYPE_UCHAR
#define	TYPE_UCHAR	1

typedef unsigned char	uchar ;

#endif /* TYPE_UCHAR */


#ifndef	TYPE_LONG64
#define	TYPE_LONG64	1

typedef LONG	long64 ;

#endif /* TYPE_LONG64 */


#ifndef	TYPE_ULONG64
#define	TYPE_ULONG64	1

typedef ULONG	ulong64 ;

#endif /* TYPE_ULONG64 */



/* some limits !! */

#ifndef	LONG64_MIN
#define	LONG64_MIN	(-9223372036854775807L-1LL)
#endif

#ifndef	LONG64_MAX
#define	LONG64_MAX	9223372036854775807LL
#endif

#ifndef	ULONG64_MAX
#define	ULONG64_MAX	18446744073709551615ULL
#endif




#endif /* MISC_INCLUDE */



