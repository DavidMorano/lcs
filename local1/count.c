/* count */

/* perform some counting type operations */


#define	F_DEBUG		0


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
#include	<limits.h>
#include	<fcntl.h>
#include	<time.h>
#include	<ctype.h>
#include	<string.h>
#include	<stdlib.h>

#include	<bio.h>
#include	<bitops.h>

#include	"misc.h"
#include	"count.h"



/* defines */

#define	MAX16	(1 << 16)



/* external subroutines */


/* forward references */

static int	numbits() ;


/* external variables */


/* local structures */


/* local data */





int countprepare(cnp)
struct count_num	*cnp ;
{
	int	i ;


	if ((cnp->num = (int *) malloc(MAX16 * sizeof(int))) == NULL)
	    return BAD ;

/* make it */

	for (i = 0 ; i < MAX16 ; i += 1)
	    cnp->num[i] = numbits(i) ;

	return OK ;
}


static int numbits(n)
int	n ;
{
	int	sum = 0 ;


	while (n) {

	    if (n & 1) sum += 1 ;

	    n = n >> 1 ;
	}

	return sum ;
}


void countforsake(cnp)
struct count_num	*cnp ;
{


	if (cnp->num != NULL) {

	    free(cnp->num) ;

	    cnp->num = NULL ;
	}

}


int countinit(cp,cnp,n)
struct count		*cp ;
struct count_num	*cnp ;
int			n ;
{
	int	i, nw ;


	nw = (n / 32) + 1 ;

	cp->cnp = NULL ;
	if ((cp->a = (unsigned long *) malloc(nw * sizeof(long))) == NULL)
	    return BAD ;

	for (i = 0 ; i < nw ; i += 1)
	    cp->a[i] = 0 ;

	cp->cnp = cnp ;
	cp->nbits = n ;
	cp->nwords = nw ;
	return OK ;
}


void countones(cp)
struct count	*cp ;
{
	int	j ;


	memset(cp->a,(~0),(cp->nwords - 1) * sizeof(long)) ;

	for (j = 0 ; j < (cp->nbits % 32) ; j += 1)
	    BSETL(cp->a + (cp->nwords - 1),j) ;

}


void countdown(cp)
struct count	*cp ;
{
	int	r = 0 ;
	int	f_borrow ;
	int	f_msb1, f_msb2 ;


	do {

	    f_msb1 = cp->a[r] & 0x7FFFFFFF ;
	    cp->a[r] -= 1 ;
	    f_msb2 = cp->a[r] & 0x7FFFFFFF ;
	    f_borrow = (! f_msb1) && f_msb2 ;
	    r += 1 ;

	} while (f_borrow && (r < cp->nwords)) ;

}


int countnum(cp)
struct count	*cp ;
{
	int	i ;
	int	sum = 0 ;
	int	high, low ;

	int	*tp ;


	tp = (cp->cnp)->num ;
	for (i = 0 ; i < cp->nwords ; i += 1) {

	    sum += tp[cp->a[i] & (MAX16 - 1)] ;
	    sum += tp[(cp->a[i] >> 16) & (MAX16 - 1)] ;

	} /* end for */

	return sum ;
}


void countfree(cp)
struct count	*cp ;
{


	if (cp->a != NULL) {

	    free(cp->a) ;

	    cp->a = NULL ;
	}

	cp->cnp = NULL ;
	cp->nbits = 0 ;
	cp->nwords = 0 ;
}




