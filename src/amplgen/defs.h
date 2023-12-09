/* greedy - definitions */



#include	<bio.h>
#include	<veclist.h>



struct gflags {
	uint	verbose : 1 ;
} ;

struct global {
	bfile		*efp ;
	bfile		*ofp ;
	struct gflags	f ;
	int		debuglevel ;
	int		verboselevel ;
	char		*programroot ;
	char		*helpfile ;
	char		*progname ;
} ;


/* struct to hold strings */

struct strings {
	int		maxlen ;	/* maximum string length encountered */
	int		minlen ;	/* minimum string length encountered */
	struct veclist	vl ;		/* list of strings */
} ;

struct string {
	char	*s ;
	int	len ;
} ;


	

