/* count */

/* perform some counting type operations */


#define	CF_DEBUG	0		/* compile-time debugging */


/* revistion history :

	= 1998-05-27, Dave Morano
	This subroutine was originally written.


*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	This module does some arbitrary precision counter stuff.


*******************************************************************************/


#include	<envstandards.h>

#include	<sys/types.h>
#include	<climits>
#include	<cstring>
#include	<cstdlib>

#include	<usystem.h>
#include	<baops.h>
#include	<localmisc.h>

#include	"count.h"


/* local defines */

#ifndef	MAX16
#define	MAX16	(1 << 16)
#endif


/* external subroutines */


/* forward references */

static int	numbits(int) ;


/* external variables */


/* local structures */


/* local variables */


/* exported subroutines */


int countnum_prepare(COUNT_NUM *cnp)
{
	const int	size = (MAX16 * sizeof(int)) ;
	int		rs ;
	void		*p ;

	if ((rs = uc_malloc(size,&p)) >= 0) {
	    int		i ;
	    cnp->num = p ;
	    for (i = 0 ; i < MAX16 ; i += 1) {
	        cnp->num[i] = numbits(i) ;
	    }
	} /* end if (m-a) */

	return rs ;
}
/* end subroutine (countnum_prepare) */


int countnum_forsake(COUNT_NUM *cnp)
{
	int		rs = SR_OK ;
	int		rs1 ;
	if (cnp->num != NULL) {
	    rs1 = uc_free(cnp->num) ;
	    if (rs >= 0) rs = rs1 ;
	    cnp->num = NULL ;
	}
	return rs ;
}
/* end subroutine (countnum_forsake) */


int count_start(COUNT *cp,COUNT_NUM *cnp,int n)
{
	const int	nw = ((n / 32) + 1) ;
	int		rs ;
	int		size ;
	void		*p ;

	size = (nw * sizeof(long)) ;

	if ((rs = uc_malloc(size,&p)) >= 0) {
	    int		i ;
	    cp->a = p ;
	    for (i = 0 ; i < nw ; i += 1) {
	        cp->a[i] = 0 ;
	    }
	    cp->cnp = cnp ;
	    cp->nbits = n ;
	    cp->nwords = nw ;
	} /* end if (m-a) */

	return rs ;
}
/* end subroutine (count_start) */


int count_ones(COUNT *cp)
{
	int		j ;

	memset(cp->a,(~0),(cp->nwords - 1) * sizeof(long)) ;

	for (j = 0 ; j < (cp->nbits % 32) ; j += 1) {
	    BASETL(cp->a + (cp->nwords - 1),j) ;
	}

	return SR_OK ;
}
/* end subroutine (count_ones) */


int count_down(COUNT *cp)
{
	int		r = 0 ;
	int		f_borrow ;
	int		f_msb1, f_msb2 ;

	do {

	    f_msb1 = cp->a[r] & 0x7FFFFFFF ;
	    cp->a[r] -= 1 ;
	    f_msb2 = cp->a[r] & 0x7FFFFFFF ;
	    f_borrow = (! f_msb1) && f_msb2 ;
	    r += 1 ;

	} while (f_borrow && (r < cp->nwords)) ;

	return SR_OK ;
}
/* end subroutine (count_down) */


int count_num(COUNT *cp)
{
	int		i ;
	int		sum = 0 ;
	int		*tp ;

	tp = (cp->cnp)->num ;
	for (i = 0 ; i < cp->nwords ; i += 1) {
	    sum += tp[cp->a[i] & (MAX16 - 1)] ;
	    sum += tp[(cp->a[i] >> 16) & (MAX16 - 1)] ;
	} /* end for */

	return sum ;
}
/* end subroutine (count_num) */


int count_finish(COUNT *cp)
{
	int		rs = SR_OK ;
	int		rs1 ;

	if (cp->a != NULL) {
	    rs1 = uc_free(cp->a) ;
	    if (rs >= 0) rs = rs1 ;
	    cp->a = NULL ;
	}

	cp->cnp = NULL ;
	cp->nbits = 0 ;
	cp->nwords = 0 ;
	return rs ;
}
/* end subroutine (count_finish) */


/* local subroutines */


static int numbits(int n)
{
	int		sum = 0 ;

	while (n) {
	    if (n & 1) sum += 1 ;
	    n = n >> 1 ;
	}

	return sum ;
}
/* end subroutine (numbits) */


