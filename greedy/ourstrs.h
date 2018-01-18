/* ourstrs (Our Strings) */


/* revision history:

	= 1998-02-15, David A­D­ Morano
	This code was started.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */


#ifndef	OURSTRS_INCLUDE
#define	OURSTRS_INCLUDE		1


#include	<envstandards.h>	/* MUST be first to configure */

#include	<sys/types.h>

#include	<ba.h>
#include	<vechand.h>
#include	<localmisc.h>


/* local object defines */

#define	OURSTRS		struct ourstrs_head
#define	OURSTRS_ENT	struct ourstrs_ent


struct ourstrs_head {
	BA_NUM		*bnp ;
	vechand		vl ;		/* list of ourstrs */
	int		maxlen ;	/* maximum string length encountered */
	int		minlen ;	/* minimum string length encountered */
} ;

struct ourstrs_ent {
	BA		inplay ;	/* the "inplay" list */
	int		*taken ;	/* the "taken" array for input use */
	int		*oip ;		/* the "index" array for output use */
	cchar		*sp ;		/* the string */
	int		sl ;		/* the string length */
} ;


#if	(! defined(OURSTRS_MASTER)) || (OURSTRS_MASTER == 0)

#ifdef	__cplusplus
extern "C" {
#endif

extern int	ourstrs_start(OURSTRS *,BA_NUM *) ;
extern int	ourstrs_add(OURSTRS *,cchar *,int) ;
extern int	ourstrs_getstr(OURSTRS *,int,cchar **) ;
extern int	ourstrs_count(OURSTRS *) ;
extern int	ourstrs_finish(OURSTRS *) ;

#ifdef	__cplusplus
}
#endif

#endif /* OURSTRS_MASTER */

#endif /* OURSTRS_INCLUDE */


