/* ourstrs SUPPORT (Our Strings) */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* string manipulations */
/* version %I% last-modified %G% */

#define	CF_DEBUGS	0		/* compile-time debugging */

/* revision history:

	= 1998-05-15, David A­D­ Morano
	This code was started.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

	Object:
	ourstrs

	Description:
	Some string manipulations.

*******************************************************************************/

#include	<envstandards.h>	/* ordered first to configure */
#include	<sys/types.h>
#include	<cstddef>
#include	<cstdlib>
#include	<cstring>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<ucmem.h>
#include	<localmisc.h>
#include	<libdebug.h>		/* LIBDEBUG */

#include	"ourstrs.h"

#pragma		GCC dependency		"mod/libutil.ccm"

import libutil ;			/* |lenstr(3u)| */

/* local defines */

#define	OURENT		ourstrs_ent


/* imported namespaces */

using libuc::mem ;			/* variable */


/* local typedefs */


/* external subroutines */

typedef ourstrs_ent *	entp ;


/* external variables */


/* local structures */


/* forward references */

local int	ourent_start(OURENT *,cchar *,int) noex ;
local int	ourent_finish(OURENT *) noex ;


/* local variables */


/* exported variables */


/* exported subroutines */

int ourstrs_start(ourstrs *op) noex {
	cnothrow	nt{} ;
    	cint		vn = 10 ;
	cint		vo = vechandm.compact ;
	int		rs = SR_FAULT ;
	if (op) {
	    rs = SR_NOMEM ;
	    if (op->vlp = new(nt) vechand ; op->vlp) {
	        op->maxlen = 0 ;
	        op->minlen = -1 ;
	        rs = vechand_start(op->vlp,vn,vo) ;
		if (rs < 0) {
		    delete op->vlp ;
		    op->vlp = nullptr ;
		}
	    } /* end if (memory-allocation) */
	} /* end if (non-null) */
	return rs ;
}
/* end subroutine (ourstrs_start) */

int ourstrs_finish(ourstrs *op) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	op->maxlen = 0 ;
	op->minlen = -1 ;
	void *vp{} ;
	for (int i = 0 ; vechand_get(op->vlp,i,&vp) >= 0 ; i += 1) {
	    ourstrs_ent	*sep = entp(vp) ;
	    if (sep) {
		{
	            rs1 = ourent_finish(sep) ;
	            if (rs >= 0) rs = rs1 ;
		}
		{
	            rs1 = mem.free(sep) ;
	            if (rs >= 0) rs = rs1 ;
		}
	    }
	} /* end for */
	if (op->vlp) {
	    {
		rs1 = vechand_finish(op->vlp) ;
		if (rs >= 0) rs = rs1 ;
	    }
	    {
		delete op->vlp ;
		op->vlp = nullptr ;
	    }
	}
	return vechand_finish(op->vlp) ;
}
/* end subroutine (ourstrs_finish) */

int ourstrs_add(ourstrs *op,cchar *sp,int sl) noex {
	int		rs ;
	int		sz ;

#if	CF_DEBUGS
	eprintf("ourstrs_add: ent\n") ;
#endif

	if (sl < 0) sl = lenstr(sp) ;

	sz = szof(ourstrs_ent) ;
	if (void *vp ; (rs = mem.mall(sz,&vp)) >= 0) {
	    ourstrs_ent	*sep = entp(vp) ;
	    if ((rs = ourent_start(sep,sp,sl)) >= 0) {
	        if ((rs = vechand_add(op->vlp,sep)) >= 0) {
	            if (sl > op->maxlen) {
	                op->maxlen = sl ;
	            }
	            if ((op->minlen < 0) || (sl < op->minlen)) {
	                op->minlen = sl ;
	            }
	        } /* end if (vechand_add) */
	        if (rs < 0)
	            ourent_finish(sep) ;
	    } /* end if (ourent_start) */
	    if (rs < 0) {
	        mem.free(sep) ;
	    }
	} /* end if (m-a) */

#if	CF_DEBUGS
	eprintf("ourstrs_add: ret rs=%d\n",rs) ;
#endif

	return rs ;
}
/* end subroutine (ourstrs_add) */

int ourstrs_getstr(ourstrs *op,int i,cchar **rpp) noex {
	int		rs ;
	void *vp{} ;
	if ((rs = vechand_get(op->vlp,i,&vp)) >= 0) {
	    ourstrs_ent	*sep = entp(vp) ;
	    if (vp) {
	        if (rpp) {
		    *rpp = sep->sp ;
	        }
	        rs = sep->sl ;
	    }
	}
	return rs ;
}
/* end subroutine (ourstrs_get) */

int ourstrs_count(ourstrs *op) noex {
	return vechand_count(op->vlp) ;
}
/* end subroutine (ourstrs_count) */


/* local subroutines */

local int ourent_start(OURENT *sep,cchar *sp,int sl) noex {
	int		rs ;
	if (cchar *cp ; (rs = mem.strw(sp,sl,&cp)) >= 0) {
	    cint	sz = (sl * szof(int)) ;
	    sep->sp = cp ;
	    if (void *vp ; (rs = mem.mall(sz,&vp)) >= 0) {
	        sep->taken = intp(vp) ;
	        if ((rs = mem.mall(sz,&vp)) >= 0) {
	            sep->oip = intp(vp) ;
	            if ((rs = ba_start(&sep->inplay,sl)) >= 0) {
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
	                mem.free(sep->oip) ;
	                sep->oip = nullptr ;
	            }
	        } /* end if (m-a) */
	        if (rs < 0) {
	            mem.free(sep->taken) ;
	            sep->taken = nullptr ;
	        }
	    } /* end if (m-a) */
	    if (rs < 0) {
		void *vp = voidp(sep->sp) ;
	        mem.free(vp) ;
	        sep->sp = nullptr ;
	    }
	} /* end if (m-a) */
	return rs ;
}
/* end subroutine (ourent_start) */

local int ourent_finish(OURENT *sep) noex {
	int		rs = SR_OK ;
	int		rs1 ;
	if (sep->sp) {
	    void *vp = voidp(sep->sp) ;
	    mem.free(vp) ;
	    if (rs >= 0) rs = rs1 ;
	    sep->sp = nullptr ;
	}
	if (sep->taken) {
	    rs1 = mem.free(sep->taken) ;
	    if (rs >= 0) rs = rs1 ;
	    sep->taken = nullptr ;
	}
	{
	    rs1 = ba_finish(&sep->inplay) ;
	    if (rs >= 0) rs = rs1 ;
	}
	if (sep->oip) {
	    rs1 = mem.free(sep->oip) ;
	    if (rs >= 0) rs = rs1 ;
	    sep->oip = nullptr ;
	}
	return rs ;
}
/* end subroutine (ourent_finish) */


