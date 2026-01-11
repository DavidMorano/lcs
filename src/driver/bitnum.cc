/* bitnum */

/* perform some counting type operations */


#define	CF_DEBUG	0


/* revistion history :

	= Dave Morano, May 27, 1998
	This subroutine was originally written.


*/


/*******************************************************************

	This module does some arbitrary precision counter stuff.


*********************************************************************/


#include	<envstandards.h>

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/times.h>
#include	<climits>
#include	<fcntl.h>
#include	<ctime>
#include	<ctype.h>
#include	<cstring>
#include	<cstdlib>

#include	<usystem.h>
#include	<bfile.h>
#include	<bitops.h>
#include	<localmisc.h>

#include	"count.h"


/* local defines */

#define	MAX16	(1 << 16)


/* external subroutines */


/* forward references */

static int	numbits() ;


/* external variables */


/* local structures */


/* local data */





int bitnum_init(cnp)
BITNUM	*cnp ;
{
	int	i ;


	if ((cnp->num = (int *) malloc(MAX16 * sizeof(int))) == NULL)
	    return BAD ;

/* make it */

	for (i = 0 ; i < MAX16 ; i += 1)
	    cnp->num[i] = numbits(i) ;

	return OK ;
}


void bitnum_free(cnp)
BITNUM	*cnp ;
{


	if (cnp->num != NULL) {

	    free(cnp->num) ;

	    cnp->num = NULL ;
	}

}


static int numbits(int n)
{
	int	sum = 0 ;

	while (n) {
	    if (n & 1) sum += 1 ;
	    n = n >> 1 ;
	}

	return sum ;
}


