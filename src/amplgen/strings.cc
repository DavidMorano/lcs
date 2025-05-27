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

extern struct global	g ;




int strings_add(vlp,s,len)
struct veclist	*vlp ;
char		*s ;
int		len ;
{
	struct string	*sp ;

	int	rs ;

	char	*cp ;


	cp = malloc_sbuf(s,len) ;

	if (cp == NULL) return BAD ;

	sp = malloc(sizeof(struct string)) ;

	if (sp == NULL) {

		(void) free(cp) ;

		return BAD ;
	}

	sp->s = cp ;
	sp->len = len ;
	rs = veclistadd(vlp,sp) ;

#if	F_DEBUG
	if (g.debuglevel > 1)
	eprintf("strings_add: added, rs=%d\n",rs) ;
#endif

	return rs ;
}
/* end subroutine (strings_add) */



