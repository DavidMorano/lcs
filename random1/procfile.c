/* procfile */

/* read in files with strings */


#define	F_DEBUG		0


/* revistion history :

	= Dave Morano, May 1998
	This subroutine was originally written.


*/


/*******************************************************************

	This subroutine will read in a file with zero or more strings 
	in it and place them into the string structure.


*********************************************************************/



#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<time.h>
#include	<ctype.h>
#include	<string.h>
#include	<stdlib.h>

#include	<bio.h>
#include	<bitops.h>

#include	"misc.h"
#include	"config.h"
#include	"defs.h"



/* local defines */

#define		LINELEN		14



/* external subroutines */

extern char	*malloc_sbuf() ;


/* forward references */


/* external variables */


/* local structures */


/* local data */





int procfile(gp,infname,fn,ssp)
struct global	*gp ;
char		infname[] ;
int		fn ;
struct strings	*ssp ;
{
	bfile		infile, *ifp = &infile ;

	struct stat	sb ;

	int	i ;
	int	len, l, rs ;
	int	linebuflen ;

	char	*linebuf, *lbp ;
	char	*cp ;


/* check the arguments */

	if ((infname == NULL) || (infname[0] == '-'))
	    infname = (char *) BIO_STDIN ;

	if ((rs = bopen(ifp,infname,"r",0666)) < 0)
	    goto badinfile ;

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("procfile: processing file %d\n",fn) ;
#endif


/* handle arbitrary sized strings, start with this */

	linebuflen = LINELEN ;
	linebuf = malloc(linebuflen + 1) ;


/* go through the loops */

	while ((len = bgetline(ifp,linebuf,linebuflen)) > 0) {

	    while ((len == linebuflen) && 
	        (linebuf[linebuflen - 1] != '\n')) {

	        linebuflen = linebuflen * 2 ;
	        linebuf = realloc(linebuf,linebuflen) ;

	        l = bgetline(ifp,linebuf + len,linebuflen - len) ;

	        if (l > 0)
	            len += l ;

	    } /* end while */

	    if (linebuf[len - 1] == '\n') len -= 1 ;

	    linebuf[len] = '\0' ;

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("procfile: len=%d LINE>%w\n",len,linebuf,len) ;
#endif

/* put it in the structure */

	    if ((rs = strings_add(ssp,linebuf,len)) < 0)
	        goto badstore ;

	} /* end while */


done:
	bclose(ifp) ;

	return 0 ;

badstore:
	bclose(ifp) ;

	return -2 ;

badinfile:
	return -1 ;
}
/* end subroutine (procfile) */



