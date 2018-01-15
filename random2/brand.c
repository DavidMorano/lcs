/* brand */

/* return a binary random number according to the specified probability */


#define	MASK	0x7FFFFFFF


extern long	random() ;


int brand(percent)
int	percent ;
{
	int	n ;


	n = (int) ((random() & MASK) % 100) ;

	return ((int) (n < percent)) ;
}



