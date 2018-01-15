/* ba (BitArray) */



#ifndef	INC_BA
#define	INC_BA	1


struct ba_num {
	int			*num ;
} ;


struct ba_bitarray {
	struct ba_num		*cnp ;
	unsigned long		*a ;
	int			nbits ;
	int			nwords ;
} ;




extern int	ba_numsupport() ;


#endif /* INC_BA */



