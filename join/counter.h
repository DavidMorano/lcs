/* count */


#include	"bitnum.h"



struct counter {
	BITNUM			*cnp ;
	unsigned LONG		*a ;
	int			nbits ;
	int			nwords ;
} ;


#define	COUNTER		struct counter



