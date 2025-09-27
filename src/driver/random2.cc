/* random2 */

/* perform a local search algorithm */


#define	CF_DEBUG	0		/* compile-time debugging */
#define	CF_TEST		0		/* run-time debgging */


/* revistion history :

	= 1998-05-27, Dave Morano
	This subroutine was originally written.


*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

        This subroutine takes as input a list of strings and produces as output
        a list of strings which are the longest common subsequences of the input
        strings. This subroutine performs a local search type of algorithm to
        find the best LCS possible within a local neighborhood.

	Arguments:
	- pointer to the program's global data
	- pointer to input 'strings' structure
	- pointer to output 'strings' structure

	Returns:
	<0	error (type indicated by return value)
	>=0	length of subsequence found


*******************************************************************************/


#include	<envstandards.h>

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/times.h>
#include	<climits>
#include	<fcntl.h>
#include	<time.h>
#include	<cstring>
#include	<cstdlib>

#include	<usystem.h>
#include	<bfile.h>
#include	<localmisc.h>

#include	"config.h"
#include	"defs.h"


/* local defines */


/* external subroutines */

extern int	brand() ;

extern char	*malloc_sbuf() ;


/* external variables */


/* forward references */

static int	expandstr() ;
static int	present() ;
static int	better() ;
static int	extend() ;

static void	taken(), taken_back() ;
static void	pa_init() ;
static void	inplay_init() ;

#if	CF_DEBUG
static void	taken_print() ;
static void	eprint_string() ;
#endif


/* local structures */


/* local variables */

static int slen[] = {
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 
	20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95,
	-1,
} ;

static int percent[] = {
	98, 96, 94, 92, 90, 88, 86, 84, 82, 80,
	75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5,
	-1,
} ;


/* exported subroutines */


int random2(GLOBAL *gp,OURSTRS *isp,OURSTRS *osp)
{
	OURSTRS	*esep, *sep ;
	struct tms	tms_now ;
	time_t		t_end ;
	clock_t		tics_cpu ;
	int		rs ;
	int		i, j, k ;
	int		nstrings ;
	int		tc ;		/* test character */
	int		srs = 0 ;	/* subroutine return status */

	char	**pa, **npa ;	/* array of pointers to current indices */
	char	*s_current, *csp ;


#if	CF_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("random1: ent\n") ;
#endif

	bflush(gp->ofp) ;

	(void) times(&tms_now) ;

	tics_cpu = tms_now.tms_utime + tms_now.tms_cutime ;
	tics_cpu += (tms_now.tms_stime + tms_now.tms_cstime) ;
	t_end = (tics_cpu / CLK_TCK) + (gp->maxtime * 60) ;

/* get the number of input strings */

	if ((nstrings = veclistcount(&isp->vl)) <= 0)
	    return -1 ;

/* allocate the pointer array */

#if	CF_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("random1: allocate\n") ;
#endif

	if ((pa = (char **) malloc((nstrings + 1) * sizeof(char *))) == NULL) {

	    srs = -2 ;
	    goto badmalloc1 ;
	}

	if ((npa = (char **) malloc((nstrings + 1) * sizeof(char *))) == NULL) {

	    srs = -2 ;
	    goto badmalloc2 ;
	}

	if ((s_current = malloc(isp->maxlen + 1)) == NULL) {

	    srs = -2 ;
	    goto badmalloc3 ;
	}



/* loop through all strings, each being the expanding string in turn */

	for (i = 0 ; i < nstrings ; i += 1) {

/* try different random percentages */

	    for (k = 0 ; percent[k] >= 0 ; k += 1) {


	        (void) times(&tms_now) ;

	        tics_cpu = tms_now.tms_utime + tms_now.tms_cutime ;
	        tics_cpu += (tms_now.tms_stime + tms_now.tms_cstime) ;
	        if ((tics_cpu / CLK_TCK) >= t_end)
	            goto timeout ;


/* try different random seed lengths */

	        for (j = 0 ; slen[j] >= 0 ; j += 1) {

	            int	seedlen ;


#ifdef	COMMENT
	            veclistget(&isp->vl,i,&esep) ;
#else
	            esep = isp->vl.va[i] ;
#endif

	            seedlen = esep->len * slen[j] / 100 ;


	            rs = expandstr(gp,isp,nstrings,i,pa,npa,
	                s_current,seedlen,percent[k],osp) ;



	        } /* end for (seed length) */

	    } /* end for (different percentages) */

	} /* end for (expanded string) */



#if	CF_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("random1: out of loop\n") ;
#endif

timeout:
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
/* end subroutine (random1) */


/* expand along a given string */
static int expandstr(GLOBAL *gp,isp,n,esi,pa,
		npa,s_current,slen,percent,osp)
struct global	*gp ;
int		n, esi ;
OURSTRSs	*isp, *osp ;
char		**pa, **npa ;
char		s_current[] ;
int		slen, percent ;
{
	OURSTRS	*esep, *sep ;

	int	rs ;
	int	i, j, oj ;
	int	cslen, cslen0 ;		/* sequence length so far */
	int	tc ;			/* test character */
	int	srs = 0 ;		/* subroutine return status */
	int	lcs_len ;
	int	search_len ;

	char	*csp ;


#if	CF_DEBUG && 0
	if (gp->debuglevel > 1)
	    eprintf("expandstr: ent\n") ;
#endif

	if ((esi < 0) || (esi >= n))
	    return -4 ;

	csp = s_current ;
	cslen = 0 ;
	lcs_len = osp->maxlen ;
	pa_init(&isp->vl,pa) ;

/* expand along the string specified by 'esi' */

	if ((rs = veclistget(&isp->vl,esi,&esep)) < 0) {

	    srs = -3 ;
	    goto badstrings ;
	}

#if	CF_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: estring len=%d, lcs_len=%d\n",
	        esep->len,lcs_len) ;
#endif

/* within the selected string to expand, choose some random characters */

	search_len = slen ;
	for (j = 0 ; j < search_len ; j += 1)
	    esep->taken[j] = (brand(percent)) ? 0 : -1 ;

	for ( ; j < esep->len ; j += 1)
	    esep->taken[j] = 0 ;

#if	CF_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: random_len=%d\n",
	        search_len) ;
#endif


/* find a possible solution in what we have so far */

	for (j = 0 ; j < search_len ; j += 1) {

	    if (esep->taken[j] < 0) continue ;

	    gp->iterations += 1 ;


	    tc = esep->s[j] ;
	    if (present(gp,&isp->vl,n,esi,tc,pa,npa,lcs_len - cslen - 1)) {

	        *csp++ = tc ;
	        npa[esi] = esep->s + j + 1 ;
	        memcpy(pa,npa,n * sizeof(char *)) ;

	        taken(&isp->vl,pa,cslen) ;

	        cslen += 1 ;

	    } else {
	        esep->taken[j] = -1 ;
	    }

	} /* end for */

#if	CF_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: got the random LCS so far, cslen=%d\n",
	        cslen) ;
#endif

/* try to extend this with the "add-on" local neighborhood */

	cslen = extend(gp,isp,n,lcs_len,pa,npa,s_current,cslen,j + 1,esep,esi) ;

#if	CF_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: out of extending (1), cslen=%d\n",
	        cslen) ;
#endif


/* is this LCS better than any found so far ? */

	if (cslen >= lcs_len) {

	    if ((rs = better(osp,s_current,cslen)) < 0)
	        goto badstore ;

	    lcs_len = cslen ;
	}


	cslen0 = cslen ;


#ifdef	COMMENT

/* OK, now let's look around the neighborhood for different paths */

#if	CF_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: rolling back, cslen=%d\n",cslen) ;
#endif

	for (oj = (cslen - 1) ; oj >= 0 ; oj -= 1) {


#if	CF_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("expandstr: rolling back, oj=%d\n",oj) ;
#endif

#if	CF_DEBUG
	    if (gp->debuglevel > 2)
	        taken_print(gp,isp) ;
#endif

/* roll back this solution to the specified previous solution */

	    j = esep->oip[oj] ;
	    taken_back(&isp->vl,pa,oj) ;

	    pa[esi] = esep->s + j + 1 ;

#if	CF_DEBUG
	    if (gp->debuglevel > 2)
	        taken_print(gp,isp) ;
#endif

#if	CF_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("expandstr: now extending, cslen=%d csj=%d\n",
	            oj,j) ;
#endif

	    cslen = extend(gp,isp,n,lcs_len,pa,npa,
	        s_current,oj,j + 1,esep,esi) ;

#if	CF_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("expandstr: out of extending (2), cslen=%d\n",
	            cslen) ;
#endif


/* is this LCS better than any found so far ? */

	    if (cslen >= lcs_len) {

#if	CF_DEBUG
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

#endif /* COMMENT */


/* we are out of here */

#if	CF_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("expandstr: returning len=%d\n",cslen) ;
#endif

	srs = MAX(cslen,cslen0) ;

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
OURSTRSs	*isp ;
int		n ;
int		lcs_len ;
char		**pa, **npa ;
char		s_current[] ;
OURSTRS	*esep ;
int		cslen, csj, esi ;
{
	int	jj, tc ;

	char	*csp ;


#if	CF_DEBUG
	if (gp->debuglevel > 1)
	    eprintf("extend: ent, esi=%d, cslen=%d, csj=%d\n",
	        esi,cslen,csj) ;
#endif

	csp = s_current + cslen ;

/* we have a common subsequence so let's try to extend it ! */

	for (jj = csj ; jj < esep->len ; jj += 1) {

	    gp->iterations += 1 ;

	    tc = esep->s[jj] ;

#if	CF_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("extend: adding loop %d, tc=%c\n",jj,tc) ;
#endif

	    if (present(gp,&isp->vl,n,esi,tc,pa,npa,lcs_len - cslen - 1)) {

#if	CF_DEBUG
	        if (gp->debuglevel > 1)
	            eprintf("extend: got one j=%d, tc=%c\n",jj,tc) ;
#endif

	        *csp++ = tc ;
	        npa[esi] = esep->s + jj + 1 ;
	        memcpy(pa,npa,n * sizeof(char *)) ;

#if	CF_DEBUG
	        if (gp->debuglevel > 1)
	            eprintf("extend: about to take cslen=%d, npa[%d]=%08X\n",
	                cslen,esi,npa[esi]) ;
#endif

	        taken(&isp->vl,pa,cslen) ;

#if	CF_DEBUG
	        if (gp->debuglevel > 1)
	            eprintf("extend: taken returned\n") ;
#endif

	        cslen += 1 ;

	    } /* end if */

#if	CF_DEBUG
	    if (gp->debuglevel > 1)
	        eprintf("extend: bottom loop\n") ;
#endif

	} /* end for */

#if	CF_DEBUG
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
	OURSTRS	*sep ;
	int		i, remaining ;
	char		*csp, *ncsp ;

#if	CF_DEBUG && 0
	if (gp->debuglevel > 1)
	    eprintf("present: ent\n") ;
#endif

	for (i = 0 ; i < nstrings ; i += 1) {
	    char	*s ;

#if	CF_DEBUG && 0
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
	OURSTRS	*sep ;
	int		i, index ;

#if	CF_DEBUG
	if (g.debuglevel > 1)
	    eprintf("taken: ent %d\n",oj) ;
#endif

#if	defined(COMMENT) || CF_DEBUG
	for (i = 0 ; veclistget(vlp,i,&sep) >= 0 ; i += 1) {

#if	CF_DEBUG
	    if (g.debuglevel > 1)
	        eprintf("taken: %d s=%08X pa=%08X\n",
	            i,sep->s,pa[i]) ;
#endif

	    index = pa[i] - sep->s - 1 ;

#if	CF_DEBUG
	    if (g.debuglevel > 1) {

	        if (index < 0)
	            eprintf("taken: %d bad index %d\n",i,index) ;

	        else
	            eprintf("taken: %d index %d\n",i,index) ;

	    }
#endif /* CF_DEBUG */

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
	OURSTRS	*sep ;

	int	i, j, jj, index ;


#if	CF_DEBUG
	if (g.debuglevel > 1)
	    eprintf("taken_back: ent, oj=%d\n",oj) ;
#endif

	i = 0 ;
	while (TRUE) {

#ifdef	COMMENT
	    if (veclistget(vlp,i,&sep) < 0) break ;
#else
	    if ((sep = vlp->va[i]) == NULL) break ;
#endif

#if	CF_DEBUG
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
	OURSTRS	*sep ;

	int	i ;


	for (i = 0 ; veclistget(vlp,i,&sep) >= 0 ; i += 1)
	    pa[i] = sep->s ;

}
/* end subroutine (pa_init) */


static int better(osp,s_current,cslen)
OURSTRSs	*osp ;
char		s_current[] ;
int		cslen ;
{
	OURSTRS	*sep ;

	int	lcs_len ;
	int	i, rs ;


#if	CF_DEBUG
	if (g.debuglevel > 1)
	    eprintf("better: ent\n") ;
#endif

	lcs_len = osp->maxlen ;
	if (cslen >= lcs_len) {

#if	CF_DEBUG
	    if (g.debuglevel > 1)
	        eprintf("better: got one\n") ;
#endif

	    if (cslen > lcs_len) {

#if	CF_DEBUG
	        if (g.debuglevel > 1)
	            eprintf("better: a better one\n") ;
#endif

	        strings_free(osp) ;

#if	CF_DEBUG
	        if (g.debuglevel > 1)
	            eprintf("better: freed previous\n") ;
#endif

	        strings_init(osp) ;

#if	CF_DEBUG
	        if (g.debuglevel > 1)
	            eprintf("better: storing better one\n") ;
#endif

	        if (strings_add(osp,s_current,cslen) < 0)
	            return BAD ;

#if	CF_DEBUG
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


/* set the "inplay" bit array */
void inplay_init(ssp,esi)
OURSTRSs	*ssp ;
int		esi ;
{
	OURSTRS	*esep, *sep ;
	int		rs ;
	int		i, j ;
	int		alen ;

#if	CF_DEBUG
	if (g.debuglevel > 1)
	    eprintf("inplay_init: ent\n") ;
#endif

/* get a pointer to the expanding string */

	veclistget(&ssp->vl,esi,&esep) ;

/* clear all "inplay" bits */

#if	CF_DEBUG
	if (g.debuglevel > 1)
	    eprintf("inplay_init: zeroing\n") ;
#endif

	ba_zero(&esep->inplay) ;


/* expand and set bits which matter */

#if	CF_DEBUG
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

#if	CF_DEBUG
	if (g.debuglevel > 1)
	    eprintf("inplay_init: exiting\n") ;
#endif

}
/* end subroutine (inplay_init) */


#if	CF_DEBUG

static void taken_print(gp,isp)
struct global	*gp ;
OURSTRSs	*isp ;
{

	if (gp->debuglevel > 1) {
	    OURSTRS	*dsp ;
	    int		di, dj ;

	    eprintf("taken_print: taken array\n") ;

	    for (di = 0 ; veclistget(&isp->vl,di,&dsp) >= 0 ; di += 1) {

	        eprintf("taken_print: %d>",di) ;

	        for (dj = 0 ; dj < MIN(dsp->len,60) ; dj += 1)
	            eprintf(" %d",dsp->taken[dj]) ;

	        eprintf("\n") ;

	    }

	}
}

#endif /* CF_DEBUG */


static void eprint_string(s,len)
char	s[] ;
int	len ;
{
	int		plen ;

	while (len > 0) {
	    plen = MIN(len,60) ;
	    eprintf("\t%t\n",s,plen) ;
	    len -= plen ;
	    s += plen ;
	}

}
/* end subroutine (eprint_string) */


