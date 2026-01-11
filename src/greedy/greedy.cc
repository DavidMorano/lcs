/* greedy */

/* perform the greedy algorithm */


#define	F_DEBUG		1


/* revistion history :

	= Dave Morano, May 1998
	This subroutine was originally written.


*/


/*******************************************************************

	This subroutine takes as input a list of strings
	and produces as output a list of strings which are
	the longest common subsequences of the input strings.
	Since this routine only implements a Greedy algorithm,
	it will find the longest possible subsequence it can
	after finding an initial common subsequence and then
	it will stop.  It will not find all of the common 
	subsequences of the same length as the longest found.

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
#include	<fcntl.h>
#include	<ctime>
#include	<ctype.h>
#include	<cstring>
#include	<cstdlib>

#include	<bio.h>
#include	<bitops.h>

#include	"misc.h"
#include	"config.h"
#include	"defs.h"



/* defines */

#define		LINELEN		200



/* external subroutines */

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




int greedy(gp,isp,osp)
struct global	*gp ;
struct strings	*isp, *osp ;
{
	struct string	*esep, *sep ;

	int	i, j, jj ;
	int	rs ;
	int	nstrings ;
	int	cslen ;			/* sequence length so far */
	int	tc ;			/* test character */
	int	srs = 0 ;		/* subroutine return status */
	int	lcs_len ;

	char	**pa, **npa ;	/* array of pointers to current indices */
	char	*cp, *sp ;
	char	*current, *csp ;


	gp->iterations = 0 ;

	osp->maxlen = 0 ;
	osp->minlen = 0 ;

/* get the number of input strings */

	if ((nstrings = veclistcount(&isp->vl)) <= 0)
	    return -1 ;

	lcs_len = osp->maxlen ;

/* allocate the pointer array */

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("greedy: allocate\n") ;
#endif

	if ((pa = (char **) malloc(nstrings * sizeof(char *))) == NULL) {

	    srs = -2 ;
	    goto badmalloc1 ;
	}

	if ((npa = (char **) malloc(nstrings * sizeof(char *))) == NULL) {

	    srs = -2 ;
	    goto badmalloc2 ;
	}


/* create storage to hold the longest sequence found */

	if ((current = malloc(isp->maxlen + 1)) == NULL) {

	    srs = -2 ;
	    goto badmalloc4 ;
	}


#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("greedy: start to find a common one\n") ;
#endif

	csp = current ;
	cslen = 0 ;

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("greedy: initpa\n") ;
#endif

	    initpa(&isp->vl,pa) ;

/* find the shortest string to expand along */

	for (i = 0 ; veclistget(&isp->vl,i,&esep) >= 0 ; i += 1) {

		if (esep == NULL) continue ;

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("greedy: finding string to expand\n") ;
#endif

		if (esep->len == isp->minlen) break ;

	} /* end for */


/* go through the loops expanding the selected string */

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("greedy: first loop, 1st string len=%d\n",esep->len) ;
#endif

	for (j = 0 ; j < (esep->len - lcs_len + 1) ; j += 1) {

	gp->iterations += 1 ;

	    tc = esep->s[j] ;
	    npa[i] = esep->s + j ;

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("greedy: present\n") ;
#endif

	    if (present(&isp->vl,nstrings,i,tc,pa,npa,lcs_len - cslen - 1))
	        break ;

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("greedy: bottom loop\n",j) ;
#endif

	} /* end for */

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("greedy: out of loop\n") ;
#endif

	if (j >= esep->len)
	    goto none ;

	*csp++ = tc ;
	cslen += 1 ;
	memcpy(pa,npa,nstrings * sizeof(char *)) ;

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("greedy: found a subsequence at j=%d\n",j) ;
#endif


/* we found a common subsequence so let's try to extend it ! */

	for (jj = j + 1 ; jj < esep->len ; jj += 1) {

	gp->iterations += 1 ;

	    tc = esep->s[jj] ;
	    npa[i] = esep->s + jj ;
	    if (present(&isp->vl,nstrings,i,tc,pa,npa,lcs_len - cslen - 1)) {

	        *csp++ = tc ;
	        cslen += 1 ;
	        memcpy(pa,npa,nstrings * sizeof(char *)) ;

	    }

	} /* end for */

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("greedy: store common sequence for output\n") ;
#endif

/* put it in the structure */

	if ((rs = strings_add(osp,current,cslen)) < 0) {

	    srs = -3 ;
	    goto badstore ;
	}

	if (osp->maxlen < cslen)
	    osp->maxlen = cslen ;


#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("greedy: returning len=%d\n",cslen) ;
#endif

	srs = cslen ;

/* we are out of here */
none:
badstore:
badstrings:
	free(current) ;

badmalloc4:

badmalloc3:
	free(npa) ;

badmalloc2:
	free(pa) ;

badmalloc1:
	return srs ;

}
/* end subroutine (greedy) */


/* subroutine to see if a character is present in all (other) strings */
static int present(vlp,nstrings,ni,tc,pa,npa,minleft)
struct veclist	*vlp ;
int		nstrings, ni, tc ;
char		**pa, **npa ;
int		minleft ;
{
	struct string	*sep ;

	int	i, remaining ;

	char	*csp, *ncsp ;


	for (i = 0 ; i < nstrings ; i += 1) {

	    char	*s ;


	    if (i == ni) continue ;

	    veclistget(vlp,i,&sep) ;

	    s = sep->s ;
	    csp = pa[i] ;
	    remaining = sep->len - (csp - s) - 1 ;
	    if ((minleft >= 0) && (remaining < minleft))
	        return NO ;

	    if ((ncsp = strchr(pa[i],tc)) == NULL)
	        return NO ;

	    npa[i] = ncsp + 1 ;
		remaining = npa[i] - s ;
	    if ((minleft >= 0) && (remaining < minleft))
	        return NO ;

	} /* end for */

	return YES ;
}
/* end subroutine (present) */


static void initpa(vlp,pa)
struct veclist	*vlp ;
char		**pa ;
{
	struct string	*sep ;

	int	i ;


	for (i = 0 ; veclistget(vlp,i,&sep) >= 0 ; i += 1)
	    pa[i] = sep->s ;

}
/* end subroutine (initpa) */



