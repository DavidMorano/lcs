/* ourstrs (Our Strings) */
/* lang=C99 */


#define	CF_DEBUGS	0		/* compile-time debugging */


/* revision history:

	= 1998-05-15, David A­D­ Morano
	This code was started.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */


#include	<envstandards.h>

#include	<sys/types.h>
#include	<cstdlib>
#include	<cstring>

#include	<vsystem.h>
#include	<localmisc.h>

#include	"ourstrs.h"


/* local defines */

#define	OURENT		OURSTRS_ENT


/* external subroutines */


/* external variables */


/* local structures */


/* forward references */

static int	ourent_start(OURENT *,BANUM *,cchar *,int) ;
static int	ourent_finish(OURENT *) ;


/* exported subroutines */


int ourstrs_start(OURSTRS *ssp,BANUM *bnp)
{
	const int	vo = VECHAND_OCOMPACT ;
	int		rs ;

#if	CF_DEBUGS
	eprintf("ourstrs_init: ent\n") ;
#endif

	ssp->bnp = bnp ;
	ssp->maxlen = 0 ;
	ssp->minlen = -1 ;
	rs = vechand_start(&ssp->vl,10,vo) ;

	return rs ;
}
/* end subroutine (ourstrs_start) */


int ourstrs_finish(OURSTRS *ssp)
{
	OURSTRS_ENT	*sep ;
	int		rs = SR_OK ;
	int		rs1 ;
	int		i ;

#if	CF_DEBUGS
	eprintf("ourstrs_finish: ent\n") ;
#endif

	ssp->maxlen = 0 ;
	ssp->minlen = -1 ;

	for (i = 0 ; vechand_get(&ssp->vl,i,&sep) >= 0 ; i += 1) {
	    if (sep != NULL) {
	        rs1 = ourent_finish(sep) ;
	        if (rs >= 0) rs = rs1 ;
	        rs1 = uc_free(sep) ;
	        if (rs >= 0) rs = rs1 ;
	    }
	} /* end for */

	ssp->bnp = NULL ;
	return vechand_finish(&ssp->vl) ;
}
/* end subroutine (ourstrs_finish) */


int ourstrs_add(OURSTRS *ssp,cchar *sp,int sl)
{
	int		rs ;
	int		size ;
	void		*p ;

#if	CF_DEBUGS
	eprintf("ourstrs_add: ent\n") ;
#endif

	if (sl < 0) sl = strlen(sp) ;

	size = sizeof(OURSTRS_ENT) ;
	if ((rs = uc_malloc(size,&p)) >= 0) {
	    OURSTRS_ENT	*sep = p ;
	    if ((rs = ourent_start(sep,ssp->bnp,sp,sl)) >= 0) {
	        if ((rs = vechand_add(&ssp->vl,sep)) >= 0) {
	            if (sl > ssp->maxlen) {
	                ssp->maxlen = sl ;
	            }
	            if ((ssp->minlen < 0) || (sl < ssp->minlen)) {
	                ssp->minlen = sl ;
	            }
	        } /* end if (vechand_add) */
	        if (rs < 0)
	            ourent_finish(sep) ;
	    } /* end if (ourent_start) */
	    if (rs < 0) {
	        uc_free(sep) ;
	    }
	} /* end if (m-a) */

#if	CF_DEBUGS
	eprintf("ourstrs_add: ret rs=%d\n",rs) ;
#endif

	return rs ;
}
/* end subroutine (ourstrs_add) */


int ourstrs_getstr(OURSTRS *ssp,int i,cchar **rpp)
{
	OURENT		*sep ;
	int		rs ;

#if	CF_DEBUGS
	eprintf("ourstrs_get: ent i=%d\n",i) ;
#endif

	if ((rs = vechand_get(&ssp->vl,i,&sep)) >= 0) {
	    if (rpp != NULL) *rpp = sep->sp ;
	    rs = sep->sl ;
	}
	return rs ;
}
/* end subroutine (ourstrs_get) */


int ourstrs_count(OURSTRS *ssp)
{
	return vechand_count(&ssp->vl) ;
}
/* end subroutine (ourstrs_count) */


/* local subroutines */


static int ourent_start(OURENT *sep,BANUM *bnp,cchar *sp,int sl)
{
	int		rs ;
	cchar		*ccp ;
	if ((rs = uc_mallocstrw(sp,sl,&ccp)) >= 0) {
	    const int	size = (sl * sizeof(int)) ;
	    void	*p ;
	    sep->sp = ccp ;
	    if ((rs = uc_malloc(size,&p)) >= 0) {
	        sep->taken = p ;
	        if ((rs = uc_malloc(size,&p)) >= 0) {
	            sep->oip = p ;
	            if ((rs = ba_start(&sep->inplay,bnp,sl)) >= 0) {
	                int	i ;
	                for (i = 0 ; i < sl ; i += 1) {
	                    sep->taken[i] = -1 ;
	                }
	                for (i = 0 ; i < sl ; i += 1) {
	                    sep->oip[i] = -1 ;
	                }
	                sep->sl = sl ;
	            } /* end if (ba) */
	            if (rs < 0) {
	                uc_free(sep->oip) ;
	                sep->oip = NULL ;
	            }
	        } /* end if (m-a) */
	        if (rs < 0) {
	            uc_free(sep->taken) ;
	            sep->taken = NULL ;
	        }
	    } /* end if (m-a) */
	    if (rs < 0) {
	        uc_free(sep->sp) ;
	        sep->sp = NULL ;
	    }
	} /* end if (m-a) */
	return rs ;
}
/* end subroutine (ourent_start) */


static int ourent_finish(OURENT *sep)
{
	int		rs = SR_OK ;
	int		rs1 ;
	if (sep->sp != NULL) {
	    rs1 = uc_free(sep->sp) ;
	    if (rs >= 0) rs = rs1 ;
	    sep->sp = NULL ;
	}
	if (sep->taken != NULL) {
	    rs1 = uc_free(sep->taken) ;
	    if (rs >= 0) rs = rs1 ;
	    sep->taken = NULL ;
	}
	rs1 = ba_finish(&sep->inplay) ;
	if (rs >= 0) rs = rs1 ;
	if (sep->oip != NULL) {
	    rs1 = uc_free(sep->oip) ;
	    if (rs >= 0) rs = rs1 ;
	    sep->oip = NULL ;
	}
	return rs ;
}
/* end subroutine (ourent_finish) */


