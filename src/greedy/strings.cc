/* strings */


#define	F_DEBUG		1



#include	<sys/types.h>
#include	<cstdlib>

#include	"misc.h"
#include	"config.h"
#include	"defs.h"


/* external subroutines */

extern char	*malloc_sbuf() ;


/* external variables */

#if	F_DEBUG
extern struct global	g ;
#endif




int strings_init(ssp)
struct strings	*ssp ;
{


#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("strings_init: entered\n") ;
#endif

	ssp->maxlen = 0 ;
	ssp->minlen = -1 ;
	return veclistinit(&ssp->vl,10,VLP_HOLES) ;
}
/* end subroutine (strings_init) */


int strings_add(ssp,s,len)
struct strings	*ssp ;
char		*s ;
int		len ;
{
	struct string	*sep ;

	int	rs ;
	int	*indexp ;

	char	*sp, *tp ;


#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("strings_add: entered\n") ;
#endif

	if ((sep = malloc(sizeof(struct string))) == NULL)
	    return BAD ;

	if ((sp = malloc_sbuf(s,len)) == NULL) {

	    (void) free(sep) ;

	    return BAD ;
	}


	if ((tp = malloc(len)) == NULL) {

	    (void) free(sep) ;

	    (void) free(sp) ;

	    return BAD ;
	}

	if ((indexp = malloc(len * sizeof(int))) == NULL) {

	    (void) free(sep) ;

	    (void) free(sp) ;

	    (void) free(tp) ;

	    return BAD ;
	}


	sep->s = sp ;
	sep->taken = tp ;
	sep->len = len ;
	rs = veclistadd(&ssp->vl,sep) ;

	if (rs >= 0) {

	    if (len > ssp->maxlen)
	        ssp->maxlen = len ;

	    if ((ssp->minlen < 0) || (len < ssp->minlen))
	        ssp->minlen = len ;

	} else {

	    (void) free(sep) ;

	    (void) free(sp) ;

	    (void) free(tp) ;

	    (void) free(indexp) ;

	} /* enf if */

#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("strings_add: added, rs=%d\n",rs) ;
#endif

	return rs ;
}
/* end subroutine (strings_add) */


int strings_free(ssp)
struct strings	*ssp ;
{
	struct string	*sep ;

	int	i ;


#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("strings_free: entered\n") ;
#endif

	ssp->maxlen = 0 ;
	ssp->minlen = -1 ;

	for (i = 0 ; veclistget(&ssp->vl,i,&sep) >= 0 ; i += 1) {

	    if (sep == NULL) continue ;

	    if (sep->s != NULL)
	        free(sep->s) ;

	    free(sep) ;

	} /* end for */

	return veclistfree(&ssp->vl) ;
}
/* end subroutine (strings_free) */



