/* ampl */

/* generate AMPL code from the input */


#define	F_DEBUG		1
#define	F_EXHAUSTIVE	0


/* revistion history :

	= Dave Morano, 98/05/16
	This subroutine was originally written.


*/


/*******************************************************************

	This subroutine takes as input a list of strings
	and produces as output a file containing the corresponding
	AMPL language needed to solve for the LCS in the
	input strings.

	Arguments :
	- pointer to the program's global data
	- pointer to input 'strings' structure
	- pointer to output 'strings' structure

	Returns :
	<0	error (type indicated by return value)
	>=0	length of subsequence found


*********************************************************************/



#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/param.h>
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



/* defines */

#define	LINELEN		200
#define	BUFLEN		2048



/* external subroutines */

extern int	bufprintf() ;

extern char	*malloc_sbuf() ;


/* forward references */

static int	present() ;

static void	initpa() ;


/* external variables */


/* local structures */


/* local data */

struct info {
	char	*ip ;
} ;




int ampl_dat(gp,isp,f_display,afname,ifname)
struct global	*gp ;
struct strings	*isp ;
int		f_display ;
char		afname[], ifname[] ;
{
	bfile	afile, *afp = &afile ;

	struct string	*sep ;

	int	i, j ;
	int	rs ;
	int	nstrings ;
	int	len ;


/* get the number of input strings */

	if ((nstrings = veclistcount(&isp->vl)) <= 0)
	    return -1 ;


/* create a file to hold the AMPL language */

	if ((rs = mktmpfile("/tmp/ampliXXXXXXXXX",0644,ifname)) < 0)
	    goto badtmpfile ;

	if ((rs = bopen(afp,ifname,"wct",0644)) < 0)
	    goto badopen ;


#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("greedy: start to find a common one\n") ;
#endif

/* put out the common stuff */

	bprintf(afp,"# AMPL source code (machine generated)\n") ;

	bprintf(afp,"param N := %d ;\n",isp->maxlen) ;


/* put out code specific to each input string */

	for (i = 0 ; veclistget(&isp->vl,i,&sep) >= 0 ; i += 1) {

	    if (sep == NULL) continue ;

	    bprintf(afp,"param s%d :=\n",i + 1) ;

	    for (j = 0 ; j < sep->len ; j += 1) {

	        bprintf(afp," [%d] %d",j,sep->s[j]) ;

	        if ((j % 8) == 7)
	            bprintf(afp,"\n") ;

	    } /* end for */

	    for ( ; j < isp->maxlen ; j += 1) {

	        bprintf(afp," [%d] %d",j,0) ;

	        if ((j % 8) == 7)
	            bprintf(afp,"\n") ;

	    } /* end for */

	    bprintf(afp,";\n\n") ;

	} /* end for */


	bclose(afp) ;

/* what do we have so far */

	if ((gp->verboselevel > 2) &&
		((rs = bopen(afp,ifname,"r",0666)) >= 0)) {

		char	buf[BUFLEN + 1] ;


		bcopyfile(afp,gp->ofp,buf,BUFLEN) ;

		bclose(afp) ;

	} /* end if (verbosity) */


/* make the AMPL control file */

	if ((rs = mktmpfile("/tmp/amplcXXXXXXXXX",0644,afname)) < 0)
	    goto badtmpfile2 ;

	if ((rs = bopen(afp,afname,"wct",0644)) < 0)
	    goto badopen2 ;

	bprintf(afp,"# AMPL control file\n") ;

	bprintf(afp,"model lcs.mod ;\n") ;

	bprintf(afp,"data %s ;\n",ifname) ;

	bprintf(afp,"solve ;\n") ;

	if (f_display)
	bprintf(afp,"display s1o ;\n") ;

	bprintf(afp,"quit ;\n") ;


	bclose(afp) ;


/* we are out of here */

	return OK ;

badopen:
	unlink(ifname) ;

	return -3 ;

badtmpfile:
	return -2 ;

badopen2:
	unlink(afname) ;

	return -5 ;

badtmpfile2:
	return -4 ;
}
/* end subroutine (ampl_dat) */


/* do the AMPL ! */

#define	CMDBUFLEN	(MAXPATHLEN * 2)

int ampl(gp,afname)
struct global	*gp ;
char		afname[] ;
{
	bfile	afile, *afp = &afile ;

	int	rs, len ;

	char	linebuf[LINELEN + 1] ;
	char	cmdbuf[CMDBUFLEN + 1] ;


	if (gp->verboselevel > 2) {

	    if ((rs = bopen(afp,afname,"r",0666)) >= 0) {

	        while ((len = bgetline(afp,linebuf,LINELEN)) > 0)
	            bwrite(gp->ofp,linebuf,len) ;


	        bclose(afp) ;

	    } else
	        bprintf(gp->efp,
	            "%s: could not open AMPL input file (rs %d)\n",
	            gp->progname,rs) ;

	} /* end if (verbosity) */


/* do the deed ! */

	bufprintf(cmdbuf,CMDBUFLEN,"ampl %s",afname) ;

	rs = system(cmdbuf) ;

	return rs ;
}
/* end subroutine (ampl) */




