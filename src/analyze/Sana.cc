/* main (Sana) */


#define	F_DEBUG		0


#include	<cstdio>



#define	LEN	30


int main()
{
	int	i ;
	int	rs ;

	int	i_l1[LEN], i_l2[LEN] ;
	int	i_e1[LEN], i_e3[LEN] ;
	int	i_r2[LEN], i_r3[LEN] ;
	int	junk ;

	double	l1[LEN], l2[LEN] ;
	double	e1[LEN], e3[LEN] ;
	double	r2[LEN], r3[LEN] ;
	double	standard ;


	for (i = 0 ; i < (LEN + 1) ; i += 1) {

		rs = scanf("%d %d %d %d %d %d %d",
			i_r2 + i, &junk,
			i_l2 + i,
			i_r3 + i,
			i_e3 + i,
			i_l1 + i,
			i_e1 + i) ;

		if (rs == EOF) break ;

	} /* end for */

	if (i > LEN) {

		printf("something wrong\n") ;

		goto done ;
	}


	printf("r2 l2 r3 e3 l1 e1\n") ;

	for (i = 0 ; i < LEN ; i += 1) {

		standard = (double) i_l1[i] ;
		
		l1[i] = 100.0 * ((double) i_l1[i]) / standard ;
		l2[i] = 100.0 * ((double) i_l2[i]) / standard ;
		r2[i] = 100.0 * ((double) i_r2[i]) / standard ;
		r3[i] = 100.0 * ((double) i_r3[i]) / standard ;
		e1[i] = 100.0 * ((double) i_e1[i]) / standard ;
		e3[i] = 100.0 * ((double) i_e3[i]) / standard ;


	printf("%i:%5.2f:%5.2f:%5.2f:%5.2f:%5.2f:%5.2f\n",
		(i + 1),r2[i],l2[i],r3[i],e3[i],l1[i],e1[i]) ;

#if	F_DEBUG
	if (i == (LEN - 1)) {

	printf("extra\n") ;
	printf("%d %d %d %d %d %d\n",
		i_r2[i],i_l2[i],i_r3[i],i_e3[i],i_l1[i],i_e1[i]) ;

	}
#endif /* F_DEBUG */

	} /* end for */


done:
	fclose(stdout) ;

	return 0 ;
}




