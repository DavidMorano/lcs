/* main (ampl) */

/* perform the Greedy algorithm to find the longest common subset of strings */


#define	F_DEBUGS	0
#define	F_DEBUG		1


/* revision history :

	= Dave Morano, May 06, 98
	Originally written.

*/


/*******************************************************************

	This program will find the longest common sequence
	given a file with strings in it.

	Synopsis :

	ampl [file(s) ...] [-V] [-v[=level]]


*********************************************************************/



#include	<sys/types.h>
#include	<sys/stat.h>
#include	<sys/param.h>
#include	<sys/times.h>
#include	<sys/ctime>
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



/* defines */

#define		NPARG		2	/* number of positional arguments */
#define		MAXARGINDEX	100

#define		NARGPRESENT	(MAXARGINDEX/8 + 1)
#define		LINELEN		200
#define		BUFLEN		(MAXPATHLEN + (2 * LINELEN))



/* externals */

extern int	optmatch() ;
extern int	cfdec() ;
extern int	procfile() ;
extern int	ampl_dat(), ampl() ;


/* forward references */

void	helpfile() ;


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
	int	f_display = FALSE ;
	int	f_help = FALSE ;
	int	line, len, l, rs ;

	char	*argp, *aop, *avp ;
	char	argpresent[NARGPRESENT] ;
	char	buf[BUFLEN + 1] ;
	char	timebuf[100] ;
	char	cfname[MAXPATHLEN + 1], dfname[MAXPATHLEN + 1] ;
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

	                if (cfdec(argp + 1,argl - 1,&maxstrings))
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
	                                (cfdec(avp,avl,
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
	                                (cfdec(avp,avl, &g.verboselevel) < 0))
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
	                                if (cfdec(avp,avl, &g.debuglevel) != OK)
	                                    goto badargvalue ;

	                            }

	                            break ;

	                        case 'V':
	                            f_version = TRUE ;
	                            break ;

	                        case 'v':
	                            g.f.verbose = TRUE ;
	                            if (f_optequal) {

	                                f_optequal = FALSE ;
	                                if (cfdec(avp,avl, 
	                                    &g.verboselevel) != OK)
	                                    goto badargvalue ;

	                            }

	                            break ;

				case 's':
					f_display = TRUE ;
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

	s.maxlen = 0 ;
	s.minlen = -1 ;
	veclistinit(&s.vl,10,VLP_HOLES) ;


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

	                if (g.f.verbose)
	                    bprintf(g.efp,"%s: error processing file \"%s\"\n",
	                        g.progname,argv[i]) ;

	            }

	            pan += 1 ;

	        } /* end if (got a positional argument) */

	    } /* end for (loading positional arguments) */

	} else {

	    rs = procfile(&g,"-",pan + 1,&s) ;

	    if (rs < 0) {

	        if (g.f.verbose)
	            bprintf(g.efp,"%s: error processing file \"%s\"\n",
	                g.progname,argv[i]) ;

	    }

	    pan += 1 ;

	} /* end if (reading in the input files) */


	if ((g.debuglevel > 0) || g.f.verbose) {

	    bprintf(efp,"%s: files processed %d\n",
	        g.progname,pan) ;

	}


	if (g.verboselevel > 1) {

	    bprintf(ofp,"there were %d input strings\n",
	        veclistcount(&s.vl)) ;

	    bprintf(ofp,"the longest input string is %d characters\n",
	        s.maxlen) ;

	    bprintf(ofp,"the shortest input string is %d characters\n",
	        s.minlen) ;

	}

	if (g.verboselevel > 2) {

	    bprintf(ofp,"input strings are :\n") ;

	    for (i = 0 ; veclistget(&s.vl,i,&rp) >= 0 ; i += 1) {

#if	F_DEBUG
	        if (g.debuglevel > 1) eprintf(
	            "main: in between\n") ;
#endif

	        if (rp == NULL) continue ;

	        bprintf(ofp,"%s\n",rp->s) ;

	    } /* end for */

	} /* end if (verbose) */


#if	F_DEBUG
	if (g.debuglevel > 1) eprintf(
	    "main: output string initialization\n") ;
#endif

	s_out.maxlen = 0 ;
	s_out.minlen = -1 ;
	veclistinit(&s_out.vl) ;


/* is the AMPL program is this user's PATH ? */

	if ((rs = getfiledirs(NULL,"ampl","x",NULL)) <= 0) {

		bprintf(ofp,"AMPL was NOT in the execution PATH !!\n") ;

		goto badampl ;
	}

/* create the control and data file for the AMPL processor */

	if ((rs = ampl_dat(&g,&s,f_display,cfname,dfname)) < 0)
	    goto badampldat ;


/* do the thing */

#if	F_DEBUG
	if (g.debuglevel > 1) eprintf(
	    "main: ampl\n") ;
#endif

/* flush some file streams so that AMPL doesn't mess them up */

	bflush(ofp) ;

	bflush(efp) ;


/* continue with the AMPL run as intended */

	(void) times(&tms1) ;

	(void) gettimeofday(&tv1,NULL) ;

	rs = ampl(&g,cfname) ;

	(void) gettimeofday(&tv2,NULL) ;

	(void) times(&tms2) ;

#if	F_DEBUG
	if (g.debuglevel > 1) eprintf(
	    "main: print results\n") ;
#endif

	if (rs |= 0) {

		bprintf(ofp,
		"AMPL return status (stat= %d)\n",rs) ;
	}


/* delete the created AMPL files for this problem */

	unlink(cfname) ;

	unlink(dfname) ;


/* print out the results */

#ifdef	COMMENT
	bprintf(ofp,"the longest common subsequence was :\n") ;

	for (i = 0 ; veclistget(&s_out.vl,i,&rp) >= 0 ; i += 1) {

#if	F_DEBUG
	    if (g.debuglevel > 1) eprintf(
	        "main: in between\n") ;
#endif

	    if (rp == NULL) continue ;

	    bprintf(ofp,"%s\n",rp->s) ;

	} /* end for */
#endif /* COMMENT */

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

	tics1 = tms1.tms_cutime + tms1.tms_cstime ;
	tics2 = tms2.tms_cutime + tms2.tms_cstime ;
	tics = tics2 - tics1 ;
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



/* let's get out of here !! */
done:
	veclistfree(&s_out.vl) ;

	veclistfree(&s.vl) ;

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

badampl:
	bprintf(efp,"%s: the AMPL program is not in the execution PATH !\n",
	    g.progname) ;

	goto badret ;

badampldat:
	bprintf(efp,"%s: could create the AMPL input file (rs %d)\n",
	    g.progname,rs) ;

	goto badret ;

/* we're bad */
badret:
	bclose(ofp) ;

	bclose(efp) ;

	return BAD ;
}
/* end subroutine (main) */



void helpfile(f,ofp)
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



