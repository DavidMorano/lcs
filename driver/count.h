/* count */


/* revision history:

	= 1998-02-15, David A­D­ Morano
	This code was started.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */


#ifndef	COUNT_INCLUDE
#define	COUNT_INCLUDE	1


#include	<envstandards.h>	/* MUST be first to configure */

#include	<sys/types.h>

#include	<localmisc.h>


/* local object defines */

#define	COUNT		struct count_bitarray
#define	COUNT_NUM	struct count_num


struct count_num {
	int		*num ;
} ;

struct count_bitarray {
	COUNT_NUM	*cnp ;
	ULONG		*a ;
	int		nbits ;
	int		nwords ;
} ;


#if	(! defined(COUNT_MASTER)) || (COUNT_MASTER == 0)

#ifdef	__cplusplus
extern "C" {
#endif

extern int	count_start(COUNT *,COUNT_NUM *,int) ;
extern int	count_setones(COUNT *) ;
extern int	count_zero(COUNT *) ;
extern int	count_countdown(COUNT *) ;
extern int	count_and(COUNT *,COUNT *) ;
extern int	count_finish(COUNT *) ;

/* helpers functions */

extern int	countnum_prepare(COUNT_NUM *cnp) ;
extern int	countnum_forsake(COUNT_NUM *cnp) ;

#ifdef	__cplusplus
}
#endif

#endif /* COUNT_MASTER */

#endif /* COUNT_INCLUDE */


