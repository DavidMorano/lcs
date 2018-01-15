/* greedy - definitions */



#include	<bio.h>
#include	<veclist.h>

#include	"ba.h"




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
	struct ba_num	*bnp ;
	int		maxlen ;	/* maximum string length encountered */
	int		minlen ;	/* minimum string length encountered */
	struct veclist	vl ;		/* list of strings */
} ;

struct string {
	struct ba_bitarray	inplay ;	/* the "inplay" list */
	int	*taken ;		/* the "taken" array for input use */
	int	*oip ;			/* the "index" array for output use */
	int	len ;
	char	*s ;			/* the string */
} ;


	

