/* ba (BitArray) */

/* perform some bit array type operations */


#define	F_DEBUG		0


/* revistion history :

	= Dave Morano, May 27, 1998
	This subroutine was originally written.


*/


/*******************************************************************

	This module does some bit array type stuff.


*********************************************************************/



#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/times.h>
#include	<limits.h>
#include	<fcntl.h>
#include	<time.h>
#include	<ctype.h>
#include	<string.h>
#include	<stdlib.h>

#include	<bio.h>
#include	<bitops.h>

#include	"misc.h"
#include	"ba.h"



/* local defines */

#define	MAX16	(1 << 16)
#define	NUMBITS	64




/* external subroutines */


/* external variables */


/* local structures */


/* forward references */


/* local data */






int ba_init(cp,cnp,n)
BA	*cp ;
BITNUM	*cnp ;
int	n ;
{
	int	i, nw ;


	nw = (n / NUMBITS) + 1 ;

	cp->cnp = NULL ;
	if ((cp->a = (unsigned LONG *) malloc(nw * sizeof(LONG))) == NULL)
	    return BAD ;

	for (i = 0 ; i < nw ; i += 1)
	    cp->a[i] = 0 ;

	cp->cnp = cnp ;
	cp->nbits = n ;
	cp->nwords = nw ;
	return OK ;
}


void ba_setones(cp)
BA	*cp ;
{
	int	j ;


	memset(cp->a,(~0),(cp->nwords - 1) * sizeof(LONG)) ;

	for (j = 0 ; j < (cp->nbits % NUMBITS) ; j += 1)
	    BSETL(cp->a + (cp->nwords - 1),j) ;

}


void ba_zero(cp)
BA	*cp ;
{
	int	j ;


#if	F_DEBUG
	eprintf("ba_zero: entered\n") ;
#endif

#if	F_DEBUG
	eprintf("ba_zero: nwords=%d, nbits=%d\n",cp->nwords,cp->nbits) ;
#endif

	memset(cp->a,0,(cp->nwords - 1) * sizeof(LONG)) ;

	for (j = 0 ; j < (cp->nbits % NUMBITS) ; j += 1)
	    BCLRL(cp->a + (cp->nwords - 1),j) ;

}


void ba_countdown(cp)
BA	*cp ;
{
	int	r = 0 ;
	int	f_borrow ;
	int	f_msb1, f_msb2 ;


	do {

	    f_msb1 = cp->a[r] & 0x7FFFFFFFFFFFFFFF ;
	    cp->a[r] -= 1 ;
	    f_msb2 = cp->a[r] & 0x7FFFFFFFFFFFFFFF ;
	    f_borrow = (! f_msb1) && f_msb2 ;
	    r += 1 ;

	} while (f_borrow && (r < cp->nwords)) ;

}


void ba_and(cp1,cp2)
BA	*cp1, *cp2 ;
{
	int	i, nw ;
	int	f_borrow ;
	int	f_msb1, f_msb2 ;

	long	t ;


	nw = MIN(cp1->nwords,cp2->nwords) ;

	for (i = 0 ; i < nw ; i += 1)
		cp1->a[i] = cp1->a[i] & cp2->a[i] ;

}


int ba_numones(cp)
BA	*cp ;
{
	int	i ;
	int	sum = 0 ;
	int	high, low ;

	int	*tp ;


	tp = (cp->cnp)->num ;
	for (i = 0 ; i < cp->nwords ; i += 1) {

	    sum += tp[cp->a[i] & (MAX16 - 1)] ;
	    sum += tp[(cp->a[i] >> 16) & (MAX16 - 1)] ;
	    sum += tp[(cp->a[i] >> 32) & (MAX16 - 1)] ;
	    sum += tp[(cp->a[i] >> 48) & (MAX16 - 1)] ;

	} /* end for */

	return sum ;
}


void ba_free(cp)
BA	*cp ;
{


	if (cp->a != NULL) {

	    free(cp->a) ;

	    cp->a = NULL ;
	}

	cp->cnp = NULL ;
	cp->nbits = 0 ;
	cp->nwords = 0 ;
}
/* end subroutine (ba_free) */



