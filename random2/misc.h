/* last modified %G% version %I% */


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

#undef	CONST
#if	defined(SYSV)
#define	CONST	const
#else
#define	CONST	/* nothing */
#endif

#if	defined(_POSIX_C_SOURCE)
#define	TYPEDEF_UCHAR
typedef unsigned char	uchar ;
#define	TYPEDEF_USHORT
typedef unsigned short	ushort ;
#define	TYPEDEF_ULONG
typedef unsigned long	ulong ;
#define	TYPEDEF_UINT
typedef unsigned int	uint ;
#endif

#ifndef	TYPEDEF_UCHAR
#define	TYPEDEF_UCHAR
typedef unsigned char	uchar ;
#endif

#if	defined(BSD) && (! defined(TYPEDEF_ULONG))
#define	TYPEDEF_ULONG
typedef unsigned long	ulong ;
#endif


typedef unsigned char	u_c ;
typedef unsigned int	u_i ;
typedef unsigned long	u_l ;
typedef unsigned short	u_s ;

#define	UC		(unsigned char)
#define	US		(unsigned short)
#define	UL		(unsigned long)
#define	UI		(unsigned int)

#if	! defined(UCHAR)
#define	UCHAR		(unsigned char)
#endif

#if	! defined(USHORT)
#define	USHORT		(unsigned short)
#endif

#if	! defined(ULONG)
#define	ULONG		(unsigned long)
#endif

#if	! defined(UINT)
#define	UINT		(unsigned int)
#endif

#define	UGT(a,b)	 (((unsigned long) (a)) > ((unsigned long) (b)))
#define	UGE(a,b)	 (((unsigned long) (a)) >= ((unsigned long) (b)))
#define	ULT(a,b)	 (((unsigned long) (a)) < ((unsigned long) (b)))
#define	ULE(a,b)	 (((unsigned long) (a)) <= ((unsigned long) (b)))
#define	UEQ(a,b)	 (((unsigned long) (a)) == ((unsigned long) (b)))
#define	UNE(a,b)	 (((unsigned long) (a)) != ((unsigned long) (b)))





