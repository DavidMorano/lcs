
/* perform initialization processing */

	banum_prepare(&bitarray_support) ;

	gp->iterations = 0 ;
	ourstrs_start(&s,&bitarray_support) ;


/* read in the input files */

#if	CF_DEBUG
	if (g.debuglevel > 1) eprintf(
	    "main: checking for positional arguments\n") ;
#endif

	pan = 0 ;
	if (npa > 0) {

	    for (i = 0 ; i <= maxai ; i += 1) {

	        if (BATST(argpresent,i)) {

#if	CF_DEBUG
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

	    bprintf(ofp,"files processed %d\n",
	        pan) ;

	}


	if (g.verboselevel > 0) {

	    bprintf(ofp,"there were %d input strings\n",
	        vechand_count(&s.vl)) ;

	    bprintf(ofp,"the longest input string is %d characters\n",
	        s.maxlen) ;

	    bprintf(ofp,"the shortest input string is %d characters\n",
	        s.minlen) ;

	    bprintf(ofp,"the maximum time for this run is %d minutes\n",
	        g.maxtime) ;

	}

	if (g.verboselevel > 2) {
	    int		sl ;
	    cchar	*sp ;
	    bprintf(ofp,"input strings are:\n") ;
	    for (i = 0 ; sl = ourstrs_get(&s.i,&sp) >= 0 ; i += 1) {
	        if (sp != NULL) {
	            bprintf(ofp,"\tinput string %u sl=%d\n",i,sl) ;
	            bprintstr(ofp,sp,sl) ;
		}
	    } /* end for */
	} /* end if (verbose) */


#if	CF_DEBUG
	if (g.debuglevel > 1) eprintf(
	    "main: output string initialization\n") ;
#endif

	ourstrs_start(&s_out,&bitarray_support) ;


	(void) times(&tms1) ;

	(void) gettimeofday(&tv1,NULL) ;

	random3(&g,&s,&s_out) ;

	(void) gettimeofday(&tv2,NULL) ;

	(void) times(&tms2) ;

/* print out the results */

	bprintf(ofp,"the LCSes found :\n") ;

	for (i = 0 ; vechand_get(&s_out.vl,i,&rp) >= 0 ; i += 1) {
	    if (rp == NULL) continue ;

	    bprintf(ofp,"\tLCS %d, len=%d\n",i,rp->len) ;

	    print_string(ofp,rp->s,rp->len) ;

	} /* end for */

/* real time */

	sec = tv2.tv_sec - tv1.tv_sec ;
	usec = tv2.tv_usec - tv1.tv_usec ;
	if (usec < 0) {

#if	CF_DEBUG
	    if (g.debuglevel > 1) eprintf(
	        "main: extra\n") ;
#endif

	    usec += 1000000 ;
	    sec -= 1 ;
	}

#if	CF_DEBUG
	if (g.debuglevel > 1) eprintf(
	    "main: sec=%d usec=%d\n",
	    sec,usec) ;
#endif

	fsec = ((double) sec) + (((double) usec) / 1e6) ;

#if	CF_DEBUG
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

#if	CF_DEBUG
	if (g.debuglevel > 1) eprintf(
	    "main: tics=%d tics1=%d tics2=%d\n",
	    tics,tics1,tics2) ;
#endif

	fsumtics = ((double) tics) ;
	fseconds = fsumtics / ((double) CLK_TCK) ;

#if	CF_DEBUG
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
	ourstrs_finish(&s_out) ;

	ourstrs_finish(&s) ;


