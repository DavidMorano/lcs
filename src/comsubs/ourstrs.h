/* ourstrs SUPPORT (Our Strings) */
/* charset=ISO8859-1 */
/* lang=C20 */

/* string manipulations */
/* version %I% last-modified %G% */


/* revision history:

	= 1998-02-15, David A­D­ Morano
	This code was started.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

#ifndef	OURSTRS_INCLUDE
#define	OURSTRS_INCLUDE	


#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<ba.h>
#include	<vechand.h>


/* local object defines */
#define	OURSTRS		struct ourstrs_head
#define	OURSTRS_ENT	struct ourstrs_entry


struct ourstrs_head {
	vechand		*vlp ;		/* list of ourstrs */
	int		maxlen ;	/* maximum string length encountered */
	int		minlen ;	/* minimum string length encountered */
} ; /* end struct */

struct ourstrs_entry {
	BA		inplay ;	/* the "inplay" list */
	int		*taken ;	/* the "taken" array for input use */
	int		*oip ;		/* the "index" array for output use */
	cchar		*sp ;		/* the string */
	int		sl ;		/* the string length */
} ; /* end struct */

typedef OURSTRS		ourstrs ;
typedef	OURSTRS_ENT	ourstrs_ent ;

EXTERNC_begin

extern int	ourstrs_start(ourstrs *) noex ;
extern int	ourstrs_add(ourstrs *,cchar *,int) noex ;
extern int	ourstrs_getstr(ourstrs *,int,cchar **) noex ;
extern int	ourstrs_count(ourstrs *) noex ;
extern int	ourstrs_finish(ourstrs *) noex ;

EXTERNC_end


#endif /* OURSTRS_INCLUDE */


