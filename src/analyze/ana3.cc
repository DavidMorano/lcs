/* main (ana3) */


#define	F_DEBUG		0
#define	F_LOOK		0
#define	F_GRAP		0
#define	F_LOG		1


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


#if	F_LOOK
	printf("l1 l2 r2 r3 e1 e3\n") ;
#endif

	for (i = 0 ; i < LEN ; i += 1) {

		standard = (double) i_r3[i] ;
		
		l1[i] = 100.0 * ((double) i_l1[i]) / standard ;
		l2[i] = 100.0 * ((double) i_l2[i]) / standard ;
		r2[i] = 100.0 * ((double) i_r2[i]) / standard ;
		r3[i] = 100.0 * ((double) i_r3[i]) / standard ;
		e1[i] = 100.0 * ((double) i_e1[i]) / standard ;
		e3[i] = 100.0 * ((double) i_e3[i]) / standard ;


#if	F_LOOK
	printf("%i:%5.2f:%5.2f:%5.2f:%5.2f:%5.2f:%5.2f\n",
		(i + 1),l1[i],l2[i],r2[i],r3[i],e1[i],e3[i]) ;
#endif

#if	F_DEBUG
	if (i == (LEN - 1)) {

	printf("extra\n") ;
	printf("%d %d %d %d %d %d\n",
		i_r2[i],i_l2[i],i_r3[i],i_e3[i],i_l1[i],i_e1[i]) ;

	}
#endif /* F_DEBUG */

	} /* end for */


#if	F_LOG
	printf("draw solid star\n") ;
	for (i = 0 ; i < LEN ; i += 1)
		printf("%d %d\n", (i + 1),i_l1[i]) ;

	printf("new solid square\n") ;
	for (i = 0 ; i < LEN ; i += 1)
		printf("%d %d\n", (i + 1),i_l2[i]) ;

	printf("new solid delta\n") ;
	for (i = 0 ; i < LEN ; i += 1)
		printf("%d %d\n", (i + 1),i_r2[i]) ;

	printf("new solid box\n") ;
	for (i = 0 ; i < LEN ; i += 1)
		printf("%d %d\n", (i + 1),i_r3[i]) ;

	printf("new solid times\n") ;
	for (i = 0 ; i < LEN ; i += 1)
		printf("%d %d\n", (i + 1),i_e1[i]) ;

	printf("new solid plus\n") ;
	for (i = 0 ; i < LEN ; i += 1)
		printf("%d %d\n", (i + 1),i_e3[i]) ;

#endif /* F_LOG */


#if	F_GRAP
	printf("l1\n") ;
	for (i = 0 ; i < LEN ; i += 1)
		printf("%d %5.2f\n", (i + 1),l1[i]) ;

	printf("l2\n") ;
	for (i = 0 ; i < LEN ; i += 1)
		printf("%d %5.2f\n", (i + 1),l2[i]) ;

	printf("r2\n") ;
	for (i = 0 ; i < LEN ; i += 1)
		printf("%d %5.2f\n", (i + 1),r2[i]) ;

	printf("r3\n") ;
	for (i = 0 ; i < LEN ; i += 1)
		printf("%d %5.2f\n", (i + 1),r3[i]) ;

	printf("e1\n") ;
	for (i = 0 ; i < LEN ; i += 1)
		printf("%d %5.2f\n", (i + 1),e1[i]) ;

	printf("e3\n") ;
	for (i = 0 ; i < LEN ; i += 1)
		printf("%d %5.2f\n", (i + 1),e3[i]) ;

#endif /* F_GRAP */


done:
	fclose(stdout) ;

	return 0 ;
}
/* end subroutine (main) */




