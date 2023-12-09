/* main */

/* main subroutine for a Local Search LCS algorithm */


#define	F_DEBUGS	0
#define	F_DEBUG		0


/* revision history :

	= Dave Morano, May 06, 98
	Originally written.

*/


/*******************************************************************

	This is the main subroutine for a LCS algorithm.
	This subroutine will parse the input arguments
	and create a data structure with the input strings in
	it.

	Program Synopsis :

	program [file(s) ...] [-V] [-v[=level]]


*********************************************************************/



#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/param.h>
#include	<sys/times.h>
#include	<sys/time.h>
#include	<limits.h>
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

#define		NPARG		2	/* number of positional arguments */
#define		MAXARGINDEX	100

#define		NARGPRESENT	(MAXARGINDEX/8 + 1)
#define		LINELEN		200
#define		BUFLEN		(MAXPATHLEN + (2 * LINELEN))



/* externals */

extern int	optmatch() ;
extern int	cfdeci() ;
extern int	procfile() ;
extern int	random1() ;


/* forward references */

static void	helpfile() ;
static void	print_strings() ;


/* local structures */


/* global data */

struct global		g ;


/* local data */

/* define command option words */

static char *argopts[] = {
	"ROOT",
	"DEBUG",
	"VERSION",
	"VERBOSE",
	"HELP",
	NULL,
} ;

#define	ARGOPT_ROOT		0
#define	ARGOPT_DEBUG		1
#define	ARGOPT_VERSION		2
#define	ARGOPT_VERBOSE		3
#define	ARGOPT_HELP		4






int main(argc,argv)
int	argc ;
char	*argv[] ;
{
	bfile		outfile, *ofp = &outfile ;
	bfile		errfile, *efp = &errfile ;

	struct tms	tms1, tms2 ;

	struct timeval	tv1, tv2 ;

	struct global	*gp = &g ;

	struct veclist	*vlp ;

	struct strings	s, s_out ;

	struct string	*rp ;

	double	fsec, fseconds, fsumtics ;

	clock_t	tics, tics1, tics2 ;

	long	sec, usec ;

	int	argr, argl, aol, avl ;
	int	maxai, pan, npa, kwi, i ;
	int	maxstrings = -1 ;
	int	f_optminus, f_optplus, f_optequal ;
	int	f_extra = FALSE ;
	int	f_version = FALSE ;
	int	f_usage = FALSE ;
	int	line, len, l, rs ;
	int	f_help = FALSE ;

	char	*argp, *aop, *avp ;
	char	argpresent[NARGPRESENT] ;
	char	buf[BUFLEN + 1] ;
	char	timebuf[100] ;
	char	*infname = NULL ;
	char	*outfname = NULL ;
	char	*cp ;


	g.progname = argv[0] ;
	bopen(efp,BIO_STDERR,"wca",0666) ;

	g.efp = efp ;
	g.ofp = ofp ;
	g.debuglevel = 0 ;
	g.verboselevel = 0 ;
	g.programroot = NULL ;
	g.helpfile = NULL ;
	g.maxtime = DEFMAXTIME ;

	g.f.verbose = FALSE ;

	f_help = FALSE ;


/* process program arguments */

	for (i = 0 ; i < NARGPRESENT ; i += 1) argpresent[i] = 0 ;

	npa = 0 ;			/* number of positional so far */
	maxai = 0 ;
	i = 0 ;
	argr = argc - 1 ;
	while (argr > 0) {

	    argp = argv[++i] ;
	    argr -= 1 ;
	    argl = strlen(argp) ;

	    f_optminus = (*argp == '-') ;
	    f_optplus = (*argp == '+') ;
	    if ((argl > 0) && (f_optminus || f_optplus)) {

	        if (argl > 1) {

	            if (isdigit(argp[1])) {

	                if (cfdeci(argp + 1,argl - 1,&maxstrings))
	                    goto badargvalue ;

	            } else {

#if	F_DEBUGS
	                eprintf("main: got an option\n") ;
#endif

	                aop = argp + 1 ;
	                aol = argl - 1 ;
	                f_optequal = FALSE ;
	                if ((avp = strchr(aop,'=')) != NULL) {

#if	F_DEBUGS
	                    eprintf("main: got an option key w/ a value\n") ;
#endif

	                    aol = avp - aop ;
	                    avp += 1 ;
	                    avl = aop + argl - 1 - avp ;
	                    f_optequal = TRUE ;

	                } else
	                    avl = 0 ;

/* do we have a keyword match or should we assume only key letters ? */

#if	F_DEBUGS
	                eprintf("main: about to check for a key word match\n") ;
#endif

	                if ((kwi = optmatch(argopts,aop,aol)) >= 0) {

#if	F_DEBUGS
	                    eprintf("main: got an option keyword, kwi=%d\n",
	                        kwi) ;
#endif

	                    switch (kwi) {

/* program root */
	                    case ARGOPT_ROOT:
	                        if (f_optequal) {

	                            f_optequal = FALSE ;
	                            if (avl) g.programroot = avp ;

	                        } else {

	                            if (argr <= 0) goto badargnum ;

	                            argp = argv[++i] ;
	                            argr -= 1 ;
	                            argl = strlen(argp) ;

	                            if (argl) g.programroot = argp ;

	                        }

	                        break ;

/* debug level */
	                    case ARGOPT_DEBUG:
	                        g.debuglevel = 1 ;
	                        if (f_optequal) {

#if	F_DEBUGS
	                            eprintf(
	                                "main: debug flag, avp=\"%W\"\n",
	                                avp,avl) ;
#endif

	                            f_optequal = FALSE ;
	                            if ((avl > 0) &&
	                                (cfdeci(avp,avl,
	                                &g.debuglevel) < 0))
	                                goto badargvalue ;

	                        }

	                        break ;

	                    case ARGOPT_VERSION:
	                        f_version = TRUE ;
	                        break ;

	                    case ARGOPT_VERBOSE:
	                        g.f.verbose = TRUE ;
	                        if (f_optequal) {

	                            f_optequal = FALSE ;
	                            if ((avl > 0) &&
	                                (cfdeci(avp,avl, &g.verboselevel) < 0))
	                                goto badargvalue ;

	                        }
	                        break ;

/* help file */
	                    case ARGOPT_HELP:
	                        if (f_optequal) {

	                            f_optequal = FALSE ;
	                            if (avl) g.helpfile = avp ;

	                        }

	                        f_help  = TRUE ;
	                        break ;

	                    } /* end switch (key words) */

	                } else {

#if	F_DEBUGS
	                    eprintf("main: got an option key letter\n") ;
#endif

	                    while (aol--) {

#if	F_DEBUGS
	                        eprintf("main: option key letters\n") ;
#endif

	                        switch (*aop) {

	                        case 'D':
	                            g.debuglevel = 1 ;
	                            if (f_optequal) {

	                                f_optequal = FALSE ;
	                                if (cfdeci(avp,avl, 
	                                    &g.debuglevel) != OK)
	                                    goto badargvalue ;

	                            }

	                            break ;

	                        case 'V':
	                            f_version = TRUE ;
	                            break ;

	                        case 'm':
	                            if (f_optequal) {

	                                f_optequal = FALSE ;
	                                if (cfdeci(avp,avl, 
	                                    &g.maxtime) != OK)
	                                    goto badargvalue ;

	                            } else {

	                                if (argr <= 0) goto badargnum ;

	                                argp = argv[++i] ;
	                                argr -= 1 ;
	                                argl = strlen(argp) ;

	                                if (cfdeci(argp,argl, 
	                                    &g.maxtime) != OK)
	                                    goto badargvalue ;

	                            }

	                            break ;

	                        case 'v':
	                            g.f.verbose = TRUE ;
	                            g.verboselevel = 1 ;
	                            if (f_optequal) {

	                                f_optequal = FALSE ;
	                                if (cfdeci(avp,avl, 
	                                    &g.verboselevel) != OK)
	                                    goto badargvalue ;

	                            }

	                            break ;

	                        default:
	                            bprintf(efp,"%s : unknown option - %c\n",
	                                g.progname,*aop) ;

	                        case '?':
	                            f_usage = TRUE ;

	                        } /* end switch */

	                        aop += 1 ;

	                    } /* end while */

	                } /* end if (individual option key letters) */

	            } /* end if (digits as argument or not) */

	        } else {

/* we have a plus or minux sign character alone on the command line */

	            if (i < MAXARGINDEX) {

	                BSET(argpresent,i) ;
	                maxai = i ;
	                npa += 1 ;	/* increment position count */

	            }

	        } /* end if */

	    } else {

	        if (i < MAXARGINDEX) {

	            BSET(argpresent,i) ;
	            maxai = i ;
	            npa += 1 ;

	        } else {

	            if (! f_extra) {

	                f_extra = TRUE ;
	                bprintf(efp,"%s: extra arguments ignored\n",
	                    g.progname) ;

	            }
	        }

	    } /* end if (key letter/word or positional) */

	} /* end while (all command line argument processing) */


	if (g.debuglevel > 0) bprintf(efp,
	    "%s: finished parsing arguments\n",
	    g.progname) ;


/* get our program root (if we have one) */

	if (g.programroot == NULL) {

	    if ((g.programroot = getenv(ROOTVAR)) == NULL)
	        g.programroot = PROGRAMROOT ;

	}


/* continue w/ the trivia argument processing stuff */

	if (f_version) {

	    bprintf(efp,"%s: version %s\n",
	        g.progname,VERSION) ;

	}

	if (f_usage) goto usage ;

	if (f_version) goto exit ;


	if (f_help) {

	    if (g.helpfile == NULL) {

	        l = bufprintf(buf,BUFLEN,"%s/%s",
	            g.programroot,HELPFILE) ;

	        g.helpfile = (char *) malloc_sbuf(buf,l) ;

	    }

	    helpfile(g.helpfile,g.efp) ;

	    goto exit ;

	}


	if (g.debuglevel > 0)
	    bprintf(efp,"%s: debuglevel %d\n",
	        g.progname,g.debuglevel) ;


/* how many lines per page */


/* open output file */

	if ((outfname == NULL) || (outfname[0] == '-'))
	    outfname = (char *) BIO_STDOUT ;

	if ((rs = bopen(ofp,outfname,"wct",0666)) < 0)
	    goto badoutfile ;


/* perform initialization processing */

	gp->iterations = 0 ;
	strings_init(&s) ;


/* read in the input files */

#if	F_DEBUG
	if (g.debuglevel > 1) eprintf(
	    "main: checking for positional arguments\n") ;
#endif

	pan = 0 ;
	if (npa > 0) {

	    for (i = 0 ; i <= maxai ; i += 1) {

	        if (BTST(argpresent,i)) {

#if	F_DEBUG
	            if (g.debuglevel > 1) eprintf(
	                "main: got a positional argument i=%d pan=%d arg=%s\n",
	                i,pan,argv[i]) ;
#endif


	            rs = procfile(&g,argv[i],pan + 1,&s) ;

	            if (rs < 0) {

	                bprintf(g.efp,"%s: error processing file \"%s\"\n",
	                    g.progname,argv[i]) ;

	            }

	            pan += 1 ;

	        } /* end if (got a positional argument) */

	    } /* end for (loading positional arguments) */

	} else {

	    rs = procfile(&g,"-",pan + 1,&s) ;

	    if (rs < 0) {

	        bprintf(g.efp,"%s: error processing file \"%s\"\n",
	            g.progname,argv[i]) ;

	    }

	    pan += 1 ;

	} /* end if (reading in the input files) */


	if ((g.debuglevel > 0) || g.f.verbose) {

	    bprintf(ofp,"files processed - %d\n",
	        pan) ;

	}


	if (g.verboselevel > 0) {

	    bprintf(ofp,"there were %d input strings\n",
	        veclistcount(&s.vl)) ;

	    bprintf(ofp,"the longest input string is %d characters\n",
	        s.maxlen) ;

	    bprintf(ofp,"the shortest input string is %d characters\n",
	        s.minlen) ;

	    bprintf(ofp,"the maximum time for this run is %d minutes\n",
	        g.maxtime) ;

	}

	if (g.verboselevel > 2) {

	    bprintf(ofp,"input strings are :\n") ;

	    for (i = 0 ; veclistget(&s.vl,i,&rp) >= 0 ; i += 1) {

	        if (rp == NULL) continue ;

	        bprintf(ofp,"\tinput string %d, len=%d\n",i,rp->len) ;

	        print_strings(ofp,rp->s,rp->len) ;

	    } /* end for */

	} /* end if (verbose) */


#if	F_DEBUG
	if (g.debuglevel > 1) eprintf(
	    "main: output string initialization\n") ;
#endif

	strings_init(&s_out) ;


	(void) times(&tms1) ;

	(void) gettimeofday(&tv1,NULL) ;

	random1(&g,&s,&s_out) ;

	(void) gettimeofday(&tv2,NULL) ;

	(void) times(&tms2) ;


/* print out the results */

	bprintf(ofp,"the LCSes found :\n") ;

	for (i = 0 ; veclistget(&s_out.vl,i,&rp) >= 0 ; i += 1) {

	    if (rp == NULL) continue ;

	    bprintf(ofp,"\tLCS %d, len=%d\n",i,rp->len) ;

	    print_strings(ofp,rp->s,rp->len) ;

	} /* end for */


/* real time */

	sec = tv2.tv_sec - tv1.tv_sec ;
	usec = tv2.tv_usec - tv1.tv_usec ;
	if (usec < 0) {

#if	F_DEBUG
	    if (g.debuglevel > 1) eprintf(
	        "main: extra\n") ;
#endif

	    usec += 1000000 ;
	    sec -= 1 ;
	}

#if	F_DEBUG
	if (g.debuglevel > 1) eprintf(
	    "main: sec=%d usec=%d\n",
	    sec,usec) ;
#endif

	fsec = ((double) sec) + (((double) usec) / 1e6) ;

#if	F_DEBUG
	if (g.debuglevel > 1) eprintf(
	    "main: fsec=%16.6f\n",
	    fsec) ;
#endif

/* CPU time */

	tics1 = tms1.tms_utime + tms1.tms_stime ;
	tics1 += (tms1.tms_cutime + tms1.tms_cstime) ;

	tics2 = tms2.tms_utime + tms2.tms_stime ;
	tics2 += (tms2.tms_cutime + tms2.tms_cstime) ;

	tics = tics2 - tics1 ;

#if	F_DEBUG
	if (g.debuglevel > 1) eprintf(
	    "main: tics=%d tics1=%d tics2=%d\n",
	    tics,tics1,tics2) ;
#endif

	fsumtics = ((double) tics) ;
	fseconds = fsumtics / ((double) CLK_TCK) ;

#if	F_DEBUG
	if (g.debuglevel > 1) eprintf(
	    "main: tics=%d sumtics=%16.6f CLK_TCK=%d\n",
	    tics,fsumtics,CLK_TCK) ;
#endif

/* whichever is less ! */

	bprintf(ofp,"running time was %16.6f seconds \n",
	    ((fseconds < fsec) ? fseconds : fsec)) ;

	bprintf(ofp,"iterations %d\n",
	    gp->iterations) ;



/* let's get out of here !! */
done:
	strings_free(&s_out) ;

	strings_free(&s) ;

	bclose(ofp) ;


/* close off and get out ! */
exit:
	bclose(efp) ;

	return OK ;

/* what are we about ? */
usage:
	bprintf(efp,
	    "%s: USAGE> %s [-V] [-v[=level]] [files ...]\n",
	    g.progname,g.progname) ;

	bprintf(efp,
	    "\t-V          print program version and exit\n") ;

	bprintf(efp,
	    "\t-v=level    verbosity to level, or 1 if level not present\n") ;

	goto badret ;

badargnum:
	bprintf(efp,"%s: not enough arguments specified\n",g.progname) ;

	goto badret ;

badargvalue:
	bprintf(efp,"%s: bad argument value was specified\n",
	    g.progname) ;

	goto badret ;

badinfile:
	bprintf(efp,"%s: could not open input file \"%s\" (rs %d)\n",
	    g.progname,infname,rs) ;

	goto badret ;

badoutfile:
	bprintf(efp,"%s: could not open output file \"%s\" (rs %d)\n",
	    g.progname,outfname,rs) ;

	goto badret ;

badret:
	bclose(ofp) ;

	bclose(efp) ;

	return BAD ;
}
/* end subroutine (main) */



static void helpfile(f,ofp)
char	f[] ;
bfile	*ofp ;
{
	bfile	file, *ifp = &file ;

	char	buf[BUFLEN + 1] ;


	if ((f == NULL) || (f[0] == '\0')) return ;

	if (bopen(ifp,f,"r",0666) >= 0) {

	    bcopyfile(ifp,ofp,buf,BUFLEN) ;

	    bclose(ifp) ;

	}

}
/* end subroutine (helpfile) */


static void print_strings(fp,s,len)
bfile	*fp ;
char	s[] ;
int	len ;
{
	int	plen ;


	while (len > 0) {

	    plen = MIN(len,60) ;

	    bprintf(fp,"\t%W\n",s,plen) ;

	    len -= plen ;
	    s += plen ;

	}

}
/* end subroutine (print_lcs) */



