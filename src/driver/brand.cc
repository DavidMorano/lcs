/* brand */

/* return a binary random number according to the specified probability */


#include	<envstandards.h>
#include	<cstdlib>


/* local subroutines */

#undef	MASK
#define	MASK	0x7FFFFFFF


/* exported subroutines */


int brand(int percent)
{
	int		n ;

	n = (int) ((random() & MASK) % 100) ;

	return ((int) (n < percent)) ;
}
/* end subroutine (brand) */


