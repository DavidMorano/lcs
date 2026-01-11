/* local1 */

/* perform a local search algorithm */


#define	F_DEBUG		0
#define	F_ALLSTRINGS	1
#define	F_TAKEBACK	0


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
#include	<ctime>
#include	<ctype.h>
#include	<cstring>
#include	<cstdlib>

#include	<bio.h>
#include	<bitops.h>

#include	"misc.h"
#include	"config.h"
#include	"defs.h"
#include	"ba.h"



/* defines */



/* external subroutines */

extern char	*malloc_sbuf() ;


/* external variables */

#if	F_DEBUG
extern struct global	g ;
#endif


/* forward references */

static int	expandstr() ;
static int	present() ;
static int	better() ;
static int	extend() ;

static void	taken(), taken_back() ;
static void	pa_init() ;
static void	inplay_init() ;

#if	F_DEBUG
static void	taken_print() ;
static void	eprint_string() ;
#endif


/* local structures */


/* local data */




int local1(gp,isp,osp)
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
	int	cslen ;			/* sequence length so far */
	int	tc ;			/* test character */
	int	srs = 0 ;		/* subroutine return status */
	int	esi ;

	char	**pa, **npa ;	/* array of pointers to current indices */
	char	*s_current, *csp ;


#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("/local1: entered\n") ;
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
	    eprintf("/local1: allocate\n") ;
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



/* expand out the shortest string first, find the shortest string */

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("/local1: finding shortest string\n") ;
#endif

	if (gp->verboselevel > 0)
	    bprintf(gp->ofp,"the number of characters \"inplay\" are:\n") ;

	esi = -1 ;
	for (i = 0 ; veclistget(&isp->vl,i,&sep) >= 0 ; i += 1) {

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("/local1: string %d\n",i) ;
#endif

	    if (gp->verboselevel == 0) {

	        if ((esi < 0) && (sep->len <= isp->minlen)) {

	            esep = sep ;
	            esi = i ;
	            break ;
	        }

	    } else {

#if	F_DEBUG
	        if (gp->debuglevel > 1)
	            eprintf("/local1: verbose\n") ;
#endif

	        if ((esi < 0) && (sep->len <= isp->minlen)) {

	            esep = sep ;
	            esi = i ;
	        }

#if	F_DEBUG
	        if (gp->debuglevel > 1)
	            eprintf("/local1: initing this string\n") ;
#endif

	        inplay_init(isp,i) ;

#if	F_DEBUG
	        if (gp->debuglevel > 1)
	            eprintf("/local1: %08X\n",sep->inplay.a[0]) ;
#endif

	        bprintf(gp->ofp,"\tstring %d, len=%d, inplay=%d\n",
	            i,sep->len,ba_numones(&sep->inplay)) ;

	    } /* end if (verbose) */

	} /* end for */

	bflush(gp->ofp) ;


#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("/local1: expanding shortest string\n") ;
#endif

	rs = expandstr(gp,isp,nstrings,esi,pa,npa,
	    s_current,osp) ;


#if	F_ALLSTRINGS

/* loop through all strings, each being the expanding string in turn */

	for (i = 0 ; i < nstrings ; i += 1) {

	    if (i == esi) continue ;

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("/local1: expand loop %d\n",i) ;
#endif

	    (void) times(&tms_now) ;

	    tics_cpu = tms_now.tms_utime + tms_now.tms_cutime ;
	    tics_cpu += (tms_now.tms_stime + tms_now.tms_cstime) ;
	    if ((tics_cpu / CLK_TCK) > t_end)
	        break ;

	    rs = expandstr(gp,isp,nstrings,i,pa,npa,
	        s_current,osp) ;

	    if (gp->debuglevel > 0)
	        bprintf(gp->efp,"%s: LCS expanding of string %d is len=%d\n",
	            gp->progname,
	            i,rs) ;

	} /* end for */

#endif /* F_ALLSTRINGS */


#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("/local1: out of loop\n") ;
#endif

done:

badmalloc5:

badmalloc4:
	free(s_current) ;

badmalloc3:
	free(npa) ;

badmalloc2:
	free(pa) ;

badmalloc1:
	return srs ;
}
/* end subroutine (local1) */



static int expandstr(gp,isp,n,esi,pa,npa,s_current,osp)
struct global	*gp ;
int		n, esi ;
struct strings	*isp, *osp ;
char		**pa, **npa ;
char		s_current[] ;
{
	struct string	*esep, *sep ;

	int	i, j, jj, oj ;
	int	rs ;
	int	cslen ;			/* sequence length so far */
	int	tc ;			/* test character */
	int	srs = 0 ;		/* subroutine return status */
	int	lcs_len = osp->maxlen ;
	int	search_len ;
	int	*tp ;

	char	*csp ;


#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: entered, esi=%d\n",esi) ;
#endif

	if ((esi < 0) || (esi >= n))
	    return -4 ;

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: start to find a common one\n") ;
#endif

#if	F_DEBUG
	if (gp->debuglevel > 1)
	taken_print(gp,isp) ;
#endif

/* initialize some stuff */

	csp = s_current ;
	cslen = 0 ;

	pa_init(&isp->vl,pa) ;

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

#if	F_DEBUG
	if (gp->debuglevel > 1)
	taken_print(gp,isp) ;
#endif

/* loop through all characters of the chosen "expand" string, finding a CS */

	search_len = esep->len ;
	for (j = 0 ; j < search_len ; j += 1) {

	    gp->iterations += 1 ;

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("expandstr: top loop\n",j) ;
#endif

	    tc = esep->s[j] ;

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("expandstr: calling present\n") ;
#endif

	    if (present(gp,&isp->vl,n,esi,tc,pa,npa,lcs_len - cslen - 1))
	        break ;

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("expandstr: bottom loop\n",j) ;
#endif

	} /* end for */

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: out of loop\n") ;
#endif

	if (j >= search_len)
	    goto none ;

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: found a subsequence at j=%d\n",j) ;
#endif

	*csp++ = tc ;
	    npa[esi] = esep->s + j + 1 ;
	memcpy(pa,npa,n * sizeof(char *)) ;

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: calling taken\n") ;
#endif

#if	F_DEBUG
	if (gp->debuglevel > 1)
	taken_print(gp,isp) ;
#endif

	taken(&isp->vl,pa,cslen) ;

	cslen += 1 ;


#if	F_DEBUG
	if (gp->debuglevel > 1)
	taken_print(gp,isp) ;
#endif

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: calling extend\n") ;
#endif

	cslen = extend(gp,isp,n,lcs_len,pa,npa,
		s_current,cslen,j + 1,esep,esi) ;

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: back from extend\n") ;
#endif


/* is this LCS better than any found so far ? */

	if (cslen >= lcs_len) {

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("expandstr: got a good or better one\n") ;
#endif

	    if ((rs = better(osp,s_current,cslen)) < 0)
	        goto badstore ;

	    lcs_len = cslen ;

	} /* end if */


/* OK, now let's look around the neighborhood for different paths */

#if	F_TAKEBACK

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: rolling back\n") ;
#endif

	for (oj = (cslen - 1) ; oj >= 0 ; oj -= 1) {


#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: rolling back %d\n",oj) ;
#endif

#if	F_DEBUG
	if (gp->debuglevel > 1)
		taken_print(gp,isp) ;
#endif

/* roll back this solution to the last common subsequence character */

		j = esep->oip[oj] ;
	    taken_back(&isp->vl,pa,oj) ;

		pa[esi] = esep->s + j + 1 ;

#if	F_DEBUG
	if (gp->debuglevel > 1)
		taken_print(gp,isp) ;
#endif

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("expandstr: now extending\n") ;
#endif

	    cslen = extend(gp,isp,n,lcs_len,pa,npa,
	        s_current,oj,j + 1,esep,esi) ;


/* is this LCS better than any found so far ? */

	    if (cslen >= lcs_len) {

#if	F_DEBUG
	        if (gp->debuglevel > 1) {

	            eprintf("expandstr: we actually got one at %d\n",j) ;

			(void) eprint_string(s_current,cslen) ;

		}
#endif

	        if ((rs = better(osp,s_current,cslen)) < 0)
	            goto badstore ;

	        lcs_len = cslen ;

	    } /* end if */


	} /* end for (rolling back) */

#endif /* F_TAKEBACK */


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


#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("extend: entered, esi=%d, cslen=%d, csj=%d\n",
	        esi,cslen,csj) ;
#endif

	csp = s_current + cslen ;

/* we have a common subsequence so let's try to extend it ! */

	for (jj = csj ; jj < esep->len ; jj += 1) {

	    gp->iterations += 1 ;

	    tc = esep->s[jj] ;

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("extend: adding loop %d, tc=%c\n",jj,tc) ;
#endif

	    if (present(gp,&isp->vl,n,esi,tc,pa,npa,lcs_len - cslen - 1)) {

#if	F_DEBUG
	        if (gp->debuglevel > 1)
	            eprintf("extend: got one j=%d, tc=%c\n",jj,tc) ;
#endif

	        *csp++ = tc ;
	        npa[esi] = esep->s + jj + 1 ;
	        memcpy(pa,npa,n * sizeof(char *)) ;

#if	F_DEBUG
	        if (gp->debuglevel > 1)
	            eprintf("extend: about to take cslen=%d, npa[%d]=%08X\n",
	                cslen,esi,npa[esi]) ;
#endif

	        taken(&isp->vl,pa,cslen) ;

#if	F_DEBUG
	        if (gp->debuglevel > 1)
	            eprintf("extend: taken returned\n") ;
#endif

	        cslen += 1 ;

	    } /* end if */

#if	F_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("extend: bottom loop\n") ;
#endif

	} /* end for */

#if	F_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("extend: exiting %d\n",cslen) ;
#endif

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


#if	F_DEBUG && 0
	if (gp->debuglevel > 1)
	    eprintf("present: entered\n") ;
#endif

	for (i = 0 ; i < nstrings ; i += 1) {

	    char	*s ;


#if	F_DEBUG && 0
	    if (gp->debuglevel > 1)
	        eprintf("present: loop %d\n",i) ;
#endif

	    if (i == esi) continue ;

#ifdef	COMMENT
	    veclistget(vlp,i,&sep) ;
#else
	    sep = vlp->va[i] ;
#endif

	    s = sep->s ;
	    csp = pa[i] ;
	    remaining = sep->len - (csp - s) - 1 ;
	    if ((minleft >= 0) && (remaining < minleft))
	        return NO ;

	    if ((ncsp = strchr(pa[i],tc)) == NULL)
	        return NO ;

	    npa[i] = ncsp + 1 ;
	    remaining = sep->len - (ncsp - s) ;
	    if ((minleft >= 0) && (remaining < minleft))
	        return NO ;

	} /* end for */

	return YES ;
}
/* end subroutine (present) */


/* update the "taken" array from the information in 'pa' */
static void taken(vlp,pa,oj)
struct veclist	*vlp ;
char		**pa ;
int		oj ;
{
	struct string	*sep ;

	int	i, index ;


#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("taken: entered %d\n",oj) ;
#endif

#if	defined(COMMENT) || F_DEBUG
	for (i = 0 ; veclistget(vlp,i,&sep) >= 0 ; i += 1) {

#if	F_DEBUG
	    if (g.debuglevel > 1)
	        eprintf("taken: %d s=%08X pa=%08X\n",
	            i,sep->s,pa[i]) ;
#endif

	    index = pa[i] - sep->s - 1 ;

#if	F_DEBUG
	    if (g.debuglevel > 1) {

	        if (index < 0)
	            eprintf("taken: %d bad index %d\n",i,index) ;

	        else
	            eprintf("taken: %d index %d\n",i,index) ;

	    }
#endif

	    sep->taken[index] = oj ;
	    sep->oip[oj] = index ;

	} /* end for */
#else
	for (i = 0 ; (sep = vlp->va[i]) != NULL ; i += 1) {

	    index = pa[i] - sep->s - 1 ;
	    sep->taken[index] = oj ;
	    sep->oip[oj] = index ;

	} /* end for */
#endif

}
/* end subroutine (taken) */


/* take back the last move */
static void taken_back(vlp,pa,oj)
struct veclist	*vlp ;
char		**pa ;
int		oj ;
{
	struct string	*sep ;

	int	i, j, jj, index ;


#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("taken_back: entered, oj=%d\n",oj) ;
#endif

	i = 0 ;
	while (TRUE) {

#ifdef	COMMENT
	    if (veclistget(vlp,i,&sep) < 0) break ;
#else
	    if ((sep = vlp->va[i]) == NULL) break ;
#endif

#if	F_DEBUG
	    if (g.debuglevel > 1)
	        eprintf("taken_back: string %d\n",i) ;
#endif

	    for (jj = sep->oip[oj] ; jj < sep->len ; jj += 1)
	        sep->taken[jj] = -1 ;

	    if (oj <= 0)
	        pa[i] = sep->s ;

	    else
	        pa[i] = sep->s + sep->oip[oj - 1] + 1 ;

	    i += 1 ;

	} /* end while */

}
/* end subroutine (taken_back) */


static void pa_init(vlp,pa)
struct veclist	*vlp ;
char		**pa ;
{
	struct string	*sep ;

	int	i ;


	for (i = 0 ; veclistget(vlp,i,&sep) >= 0 ; i += 1)
	    pa[i] = sep->s ;

}
/* end subroutine (pa_init) */


static int better(osp,s_current,cslen)
struct strings	*osp ;
char		s_current[] ;
int		cslen ;
{
	struct string	*sep ;

	int	lcs_len ;
	int	i, rs ;


#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("better: entered\n") ;
#endif

	lcs_len = osp->maxlen ;
	if (cslen >= lcs_len) {

#if	F_DEBUG
	    if (g.debuglevel > 1)
	        eprintf("better: got one\n") ;
#endif

	    if (cslen > lcs_len) {

#if	F_DEBUG
	        if (g.debuglevel > 1)
	            eprintf("better: a better one\n") ;
#endif

	        strings_free(osp) ;

#if	F_DEBUG
	        if (g.debuglevel > 1)
	            eprintf("better: freed previous\n") ;
#endif

	        strings_init(osp) ;

#if	F_DEBUG
	        if (g.debuglevel > 1)
	            eprintf("better: storing better one\n") ;
#endif

	        if (strings_add(osp,s_current,cslen) < 0)
	            return BAD ;

#if	F_DEBUG
	        if (g.debuglevel > 1)
	            eprintf("better: done storing better one\n") ;
#endif

	    } else {

/* verify that this is a new unique solution */

	        for (i = 0 ; (rs = veclistget(&osp->vl,i,&sep)) >= 0 ; i += 1) {

	            if ((cslen == sep->len) &&
	                (strncmp(s_current,sep->s,cslen) == 0)) break ;

	        } /* end for */

	        if (rs < 0) {

	            if (strings_add(osp,s_current,cslen) < 0)
	                return BAD ;

	        }

	    } /* end if */

	} /* end if (found a good or better LCS) */

	return OK ;
}
/* end subroutine (better) */


/* in set the "inplay" bit array */
void inplay_init(ssp,esi)
struct strings	*ssp ;
int		esi ;
{
	struct string	*esep, *sep ;

	int	rs, i, j ;
	int	alen ;


#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("inplay_init: entered\n") ;
#endif

/* get a pointer to the expanding string */

	veclistget(&ssp->vl,esi,&esep) ;

/* clear all "inplay" bits */

#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("inplay_init: zeroing\n") ;
#endif

	ba_zero(&esep->inplay) ;


/* expand and set bits which matter */

#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("inplay_init: looping\n") ;
#endif

	for (j = 0 ; j < esep->len ; j += 1) {

	    for (i = 0 ; (rs = veclistget(&ssp->vl,i,&sep)) >= 0 ; i += 1) {

	        if (i == esi) continue ;

	        if (strchr(sep->s,esep->s[j]) == NULL) break ;

	    } /* end for */

	    if (rs < 0)
	        BSETL(esep->inplay.a,j) ;

	} /* end for */

#if	F_DEBUG
	if (g.debuglevel > 1)
	    eprintf("inplay_init: exiting\n") ;
#endif

}
/* end subroutine (inplay_init) */


#if	F_DEBUG

static void taken_print(gp,isp)
struct global	*gp ;
struct strings	*isp ;
{


	if (gp->debuglevel > 1) {

	    struct string	*dsp ;

	    int	di, dj ;


	    eprintf("taken_print: taken array\n") ;

	    for (di = 0 ; veclistget(&isp->vl,di,&dsp) >= 0 ; di += 1) {

	        eprintf("taken_print: %d>",di) ;

	        for (dj = 0 ; dj < MIN(dsp->len,60) ; dj += 1)
	            eprintf(" %d",dsp->taken[dj]) ;

	        eprintf("\n") ;

	    }

	}
}

#endif /* F_DEBUG */


static void eprint_string(s,len)
char	s[] ;
int	len ;
{
	int	plen ;


	while (len > 0) {

	    plen = MIN(len,60) ;

	    eprintf("\t%W\n",s,plen) ;

	    len -= plen ;
	    s += plen ;

	}

}
/* end subroutine (eprint_string) */



