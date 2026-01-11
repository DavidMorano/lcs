/* counter */

/* perform some counting type operations */


#define	F_DEBUGS	0


/* revistion history :

	= Dave Morano, May 27, 1998
	This subroutine was originally written.


*/


/*******************************************************************

	This module does some arbitrary precision counter stuff.


*********************************************************************/




#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/times.h>
#include	<climits>
#include	<fcntl.h>
#include	<ctime>
#include	<ctype.h>
#include	<cstring>
#include	<cstdlib>

#include	<bio.h>
#include	<bitops.h>

#include	"misc.h"
#include	"counter.h"



/* local defines */

#define	MAX16	(1 << 16)
#define	NUMBITS	64



/* external subroutines */


/* forward references */


/* external variables */


/* local structures */


/* local data */






int counter_init(cp,cnp,n)
COUNTER	*cp ;
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


void counter_setones(cp)
COUNTER	*cp ;
{
	int	j ;


	memset(cp->a,(~0),(cp->nwords - 1) * sizeof(LONG)) ;

	for (j = 0 ; j < (cp->nbits % NUMBITS) ; j += 1)
	    BSETL(cp->a + (cp->nwords - 1),j) ;

}


void counter_countdown(cp)
COUNTER	*cp ;
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


int counter_getnum(cp)
COUNTER	*cp ;
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


void counter_free(cp)
COUNTER	*cp ;
{


	if (cp->a != NULL) {

	    free(cp->a) ;

	    cp->a = NULL ;
	}

	cp->cnp = NULL ;
	cp->nbits = 0 ;
	cp->nwords = 0 ;
}



