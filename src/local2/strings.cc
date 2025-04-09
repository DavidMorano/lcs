/* strings */


#define	F_DEBUG		0



#include	<sys/types.h>
#include	<stdlib.h>
#include	<string.h>

#include	"misc.h"
#include	"config.h"
#include	"defs.h"


/* external subroutines */

extern char	*malloc_sbuf() ;


/* external variables */

#if	F_DEBUG
extern struct global	g ;
#endif




int strings_init(ssp,bnp)
struct strings	*ssp ;
struct ba_num	*bnp ;
{
	int	rs ;


#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("strings_init: entered\n") ;
#endif

	ssp->bnp = bnp ;
	ssp->maxlen = 0 ;
	ssp->minlen = -1 ;
	if ((rs = veclistinit(&ssp->vl,10,VLP_HOLES)) < 0)
	    goto bad1 ;

	return rs ;

bad1:
	return rs ;
}
/* end subroutine (strings_init) */


int strings_add(ssp,s,len)
struct strings	*ssp ;
char		*s ;
int		len ;
{
	struct string	*sep ;

	int	i, rs = BAD ;


#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("strings_add: entered\n") ;
#endif

	if ((sep = (struct string *) malloc(sizeof(struct string))) == NULL)
	    goto bad1 ;

	if ((sep->s = malloc_sbuf(s,len)) == NULL) 
		goto bad2 ;

	if ((sep->taken = (int *) malloc(len * sizeof(int))) == NULL) 
		goto bad3 ;

	if ((sep->oip = malloc(len * sizeof(int))) == NULL) 
		goto bad4 ;


	if ((rs = ba_init(&sep->inplay,ssp->bnp)) < 0)
	    goto bad5 ;


	for (i = 0 ; i < len ; i += 1)
		sep->taken[i] = -1 ;

	for (i = 0 ; i < len ; i += 1)
		sep->oip[i] = -1 ;


	sep->len = len ;
	if ((rs = veclistadd(&ssp->vl,sep)) < 0)
	    goto bad6 ;


	if (len > ssp->maxlen)
	    ssp->maxlen = len ;

	if ((ssp->minlen < 0) || (len < ssp->minlen))
	    ssp->minlen = len ;

#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("strings_add: added, rs=%d\n",rs) ;
#endif

	return rs ;

bad6:
	(void) ba_free(&sep->inplay) ;

bad5:
	(void) free(sep->oip) ;

bad4:
	(void) free(sep->taken) ;

bad3:
	(void) free(sep->s) ;

bad2:
	(void) free(sep) ;

bad1:
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

	    if (sep->taken != NULL)
	        free(sep->taken) ;

	    ba_free(&sep->inplay) ;

	    if (sep->oip)
	        free(sep->oip) ;

	    free(sep) ;

	} /* end for */

	ssp->bnp = NULL ;

	return veclistfree(&ssp->vl) ;
}
/* end subroutine (strings_free) */



