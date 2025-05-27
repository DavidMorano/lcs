/* random1 */

/* perform a local search algorithm */


#define	F_DEBUG		0
#define	F_TEST		0


/* revistion history :

	= Dave Morano, May 27, 1998
	This subroutine was originally written.


*/


/*******************************************************************

	This subroutine takes as input a list of strings
	and produces as output a list of strings which are
	the longest common subsequences of the input strings.
	This subroutine performs a local search type of
	algorithm to find the best LCS possible within a
	local neighborhood.

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
#include	<sys/times.h>
#include	<climits>
#include	<fcntl.h>
#include	<time.h>
#include	<ctype.h>
#include	<cstring>
#include	<cstdlib>

#include	<bio.h>
#include	<bitops.h>

#include	"misc.h"
#include	"config.h"
#include	"defs.h"



/* defines */



/* external subroutines */

extern int	brand() ;

extern char	*malloc_sbuf() ;


/* forward references */

static int	present() ;
static int	better() ;
static int	extend() ;

static void	taken() ;
static void	initpa() ;


/* external variables */


/* local structures */


/* local data */




int random1(gp,isp,osp)
struct global	*gp ;
struct strings	*isp, *osp ;
{
	struct string	*esep, *sep ;

	struct tms	tms_now ;

	time_t	t_end ;

	clock_t	tics_cpu ;

	int	i, j, jj ;
	int	rs ;
	int	nstrings ;
	int	tc ;			/* test character */
	int	srs = 0 ;		/* subroutine return status */
	int	*indexp ;

	char	**pa, **npa ;	/* array of pointers to current indices */
	char	*s_current, *s_include, *csp ;


#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("random1: entered\n") ;
#endif


	(void) times(&tms_now) ;

	tics_cpu = tms_now.tms_utime + tms_now.tms_cutime ;
	tics_cpu += (tms_now.tms_stime + tms_now.tms_cstime) ;
	t_end = (tics_cpu / CLK_TCK) + (gp->maxtime * 60) ;

/* get the number of input strings */

	if ((nstrings = veclistcount(&isp->vl)) <= 0)
	    return -1 ;

/* allocate the pointer array */

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("random1: allocate\n") ;
#endif

	if ((pa = (char **) malloc(nstrings * sizeof(char *))) == NULL) {

	    srs = -2 ;
	    goto badmalloc1 ;
	}

	if ((npa = (char **) malloc(nstrings * sizeof(char *))) == NULL) {

	    srs = -2 ;
	    goto badmalloc2 ;
	}

	if ((s_current = malloc(isp->maxlen + 1)) == NULL) {

	    srs = -2 ;
	    goto badmalloc3 ;
	}

	if ((s_include = malloc(isp->maxlen + 1)) == NULL) {

	    srs = -2 ;
	    goto badmalloc4 ;
	}

	if ((indexp = malloc(isp->maxlen + 1)) == NULL) {

	    srs = -2 ;
	    goto badmalloc5 ;
	}


/* loop through all strings, each being the expanding string in turn */

	for (i = 0 ; i < nstrings ; i += 1) {

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("random1: expand loop %d\n",i) ;
#endif

	    (void) times(&tms_now) ;

	    tics_cpu = tms_now.tms_utime + tms_now.tms_cutime ;
	    tics_cpu += (tms_now.tms_stime + tms_now.tms_cstime) ;
	    if ((tics_cpu / CLK_TCK) > t_end)
	        break ;

	    rs = expandstr(gp,isp,nstrings,i,pa,npa,
	        s_current,s_include,indexp,osp) ;

	    if (gp->debuglevel > 0)
	        bprintf(gp->efp,"%s: LCS expanding of string %d is len=%d\n",
	            gp->progname,
	            i,rs) ;

	} /* end for */

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("random1: out of loop\n") ;
#endif

done:
	free(indexp) ;

badmalloc5:
	free(s_include) ;

badmalloc4:
	free(s_current) ;

badmalloc3:
	free(npa) ;

badmalloc2:
	free(pa) ;

badmalloc1:
	return srs ;
}
/* end subroutine (random1) */


/* expand along a given string */
int expandstr(gp,isp,n,esi,pa,npa,s_current,s_include,indexp,osp)
struct global	*gp ;
int		n, esi ;
struct strings	*isp, *osp ;
char		**pa, **npa ;
char		s_current[] ;
char		s_include[] ;
int		*indexp ;
{
	struct string	*esep, *sep ;

	int	i, j, jj ;
	int	rs ;
	int	cslen ;			/* sequence length so far */
	int	tc ;			/* test character */
	int	srs = 0 ;		/* subroutine return status */
	int	lcs_len ;
	int	search_len ;

	char	*csp ;


#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: entered\n") ;
#endif

	if ((esi < 0) || (esi >= n))
	    return -4 ;

	csp = s_current ;
	cslen = 0 ;
	lcs_len = osp->maxlen ;
	initpa(&isp->vl,pa) ;

/* expand along the string specified by 'esi' */

	if ((rs = veclistget(&isp->vl,esi,&esep)) < 0) {

	    srs = -3 ;
	    goto badstrings ;
	}

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: first loop, 1st string len=%d\n",
	        esep->len) ;
#endif

/* within the selected string to expand, chose some random characters */

	search_len = MIN((esep->len - lcs_len),(esep->len - 1)) ;
	for (j = 0 ; j < search_len ; j += 1)
	    esep->taken[j] = (char) brand(70) ;

	for ( ; j < esep->len ; j += 1)
	    esep->taken[j] = TRUE ;


/* find a possible solution in what we have so far */

	for (j = 0 ; j < search_len ; j += 1) {

	    gp->iterations += 1 ;

	    if (! esep->taken[j]) continue ;

	    tc = esep->s[j] ;
	    npa[esi] = esep->s + j + 1 ;

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("expandstr: considering '%c', cslen=%d\n",
	            tc,cslen) ;
#endif

	    if (present(gp,&isp->vl,n,esi,tc,pa,npa,lcs_len - cslen - 1)) {

#if	F_DEBUG
	        if (gp->debuglevel > 1)
	            eprintf("expandstr: found common '%c', cslen=%d\n",
	                tc,cslen) ;
#endif

	        *csp++ = tc ;
	        cslen += 1 ;
	        memcpy(pa,npa,n * sizeof(char *)) ;

	        taken(&isp->vl,pa) ;

	    } else
	        esep->taken[j] = FALSE ;

	} /* end for */

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: got the random LCS so far, cslen=%d\n",
	        cslen) ;
#endif

/* try to extend this with the "add tail" local neighborhood */

	cslen = extend(gp,isp,n,lcs_len,pa,npa,s_current,cslen,j + 1,esep,esi) ;

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: after expand, cslen=%d\n",
	        cslen) ;
#endif


/* is this LCS better than any found so far ? */

	if (cslen >= lcs_len) {

	    if ((rs = better(osp,s_current,cslen)) < 0)
	        goto badstore ;

	    lcs_len = cslen ;
	}


/* we are out of here */

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: returning len=%d\n",cslen) ;
#endif

	srs = cslen ;

/* we are out of here */
done:
none:
badstrings:
	return srs ;

badstore:
	srs = -5 ;
	goto done ;

}
/* end subroutine (expandstr) */



static int extend(gp,isp,n,lcs_len,pa,npa,s_current,cslen,csj,esep,esi)
struct global	*gp ;
struct strings	*isp ;
int		n ;
int		lcs_len ;
char		**pa, **npa ;
char		s_current[] ;
struct string	*esep ;
int		cslen, csj, esi ;
{
	int	jj, tc ;

	char	*csp ;


	csp = s_current + csj ;

/* we have a common subsequence so let's try to extend it ! */

	for (jj = csj ; jj < esep->len ; jj += 1) {

	    gp->iterations += 1 ;

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("extend: adding loop %d\n",jj) ;
#endif

	    tc = esep->s[jj] ;
	    npa[esi] = esep->s + jj + 1 ;
	    if (present(gp,&isp->vl,n,esi,tc,pa,npa,lcs_len - cslen - 1)) {

	        *csp++ = tc ;
	        cslen += 1 ;
	        memcpy(pa,npa,n * sizeof(char *)) ;

	        taken(&isp->vl,pa) ;

	    } /* end if */

	} /* end for */

	return cslen ;
}
/* end subroutine (extend) */


/* subroutine to see if a character is present in all (other) strings */
static int present(gp,vlp,nstrings,esi,tc,pa,npa,minleft)
struct global	*gp ;
struct veclist	*vlp ;
int		nstrings, esi, tc ;
char		**pa, **npa ;
int		minleft ;
{
	struct string	*sep ;

	int	i, remaining ;

	char	*csp, *ncsp ;


#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("present: entered\n") ;
#endif

	for (i = 0 ; i < nstrings ; i += 1) {

	    char	*s ;


#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("present: loop %d\n",i) ;
#endif

	    if (i == esi) continue ;

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


/* update the "taken" array from the information in 'pa' */
static void taken(vlp,pa)
struct veclist	*vlp ;
char		**pa ;
{
	struct string	*sep ;

	int	i, index ;


	for (i = 0 ; veclistget(vlp,i,&sep) >= 0 ; i += 1) {

	    index = pa[i] - sep->s - 1 ;
	    sep->taken[index] = TRUE ;

	} /* end for */

}
/* end subroutine (taken) */


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


static int better(osp,s_current,cslen)
struct strings	*osp ;
char		s_current[] ;
int		cslen ;
{
	int	lcs_len ;


	lcs_len = osp->maxlen ;
	if (cslen >= lcs_len) {

	    if (cslen > lcs_len) {

	        strings_free(osp) ;

	        strings_init(osp) ;

	    }

	    if (strings_add(osp,s_current,cslen) < 0)
	        return BAD ;

	} /* end if (found a good or better LCS) */

	return OK ;
}
/* end subroutine (better) */



