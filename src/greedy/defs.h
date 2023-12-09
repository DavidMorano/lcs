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
	int		maxtime ;
	int		iterations ;
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
	char	*s ;			/* the string */
	char	*taken ;		/* the "taken" array for input use */
	int	*i ;			/* the "index" array for output use */
	int	len ;
} ;


	

