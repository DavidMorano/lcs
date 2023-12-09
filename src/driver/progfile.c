/* progfile */
/* lang=C99 */

/* read in files with strings */


#define	CF_DEBUGS	0		/* compile-time debugging */
#define	CF_DEBUG	1		/* run-time debugging */


/* revistion history :

	= 1998-05-06, Dave Morano
	This subroutine was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

        This subroutine will read in a file with zero or more strings in it and
        place them into the string structure.


*******************************************************************************/


#include	<envstandards.h>

#include	<sys/types.h>
#include	<string.h>
#include	<stdlib.h>

#include	<vsystem.h>
#include	<bfile.h>
#include	<localmisc.h>

#include	"ourstrs.h"
#include	"config.h"
#include	"defs.h"


/* defines */

#ifndef	LINEBUFLEN
#define	LINEBUFLEN	2048
#endif


/* external subroutines */

#if	CF_DEBUGS || CF_DEBUG
extern int	debugprintf(cchar *,...) ;
#endif


/* forward references */


/* external variables */


/* local structures */


/* local variables */


/* exported subroutines */


/* ARGSUSED */
int progfile(GLOBAL *gp,cchar *ifn,int fn,OURSTRS *ssp)
{
	bfile		infile, *ifp = &infile ;
	int		rs ;
	int		rs1 ;

#if	CF_DEBUG
	if (gp->debuglevel > 1)
	    debugprintf("progfile: processing file %d\n",fn) ;
#endif

	if ((ifn == NULL) || (ifn[0] == '-'))
	    ifn = BFILE_STDIN ;

	if ((rs = bopen(ifp,ifn,"r",0666)) >= 0) {
	    int		llen = LINEBUFLEN ;
	    char	*lbuf ;
	    if ((rs = uc_malloc((llen+1),&lbuf)) >= 0) {
	        int	len ;
	        int	l ;
		char	*p ;

/* go through the loops */

	        while ((len = breadline(ifp,lbuf,llen)) > 0) {

	            while ((len == llen) && 
	                (lbuf[llen - 1] != '\n')) {

	                llen = llen * 2 ;
	                uc_realloc(lbuf,llen,&p) ;
			lbuf = p ;

	                l = breadline(ifp,lbuf + len,llen - len) ;

	                if (l > 0)
	                    len += l ;

	            } /* end while */

	            if (lbuf[len - 1] == '\n') len -= 1 ;
	            lbuf[len] = '\0' ;

#if	CF_DEBUG
	            if (gp->debuglevel > 1)
	                debugprintf("progfile: len=%d LINE>%t\n",len,lbuf,len) ;
#endif

/* put it in the structure */

	            rs = ourstrs_add(ssp,lbuf,len) ;

	            if (rs < 0) break ;
	        } /* end while */

	        rs1 = uc_free(lbuf) ;
	        if (rs >= 0) rs = rs1 ;
	    } /* end if (m-a-f) */
	    rs1 = bclose(ifp) ;
	    if (rs >= 0) rs = rs1 ;
	} /* end if (bfile) */

	return rs ;
}
/* end subroutine (progfile) */


