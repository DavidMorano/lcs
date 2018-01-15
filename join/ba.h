/* ba (BitArray) */



#ifndef	BA_INCLUDE
#define	BA_INCLUDE	1



#include	"bitnum.h"



struct ba_bitarray {
	BITNUM			*cnp ;
	unsigned long		*a ;
	int			nbits ;
	int			nwords ;
} ;



#define	BA	struct ba_bitarray




#ifndef	BA_MASTER


extern int	ba_numsupport() ;


#endif /* BA_MASTER */

#endif /* BA_INCLUDE */



