/* proginfo_setpiv SUPPORT */
/* charset=ISO8859-1 */
/* lang=C++20 (conformance reviewed) */

/* utility for KSH built-in commands */
/* last modified %G% version %I% */

#define	CF_DEBUGS	0		/* compile-time debugging */
#define	CF_DEBUG	0		/* switchable at invocation */

/* revision history:

	= 1998-03-01, David A­D­ Morano
	This subroutine was originally written.

*/

/* Copyright © 1998 David A­D­ Morano.  All rights reserved. */

/*******************************************************************************

  	Name:
	proginfo_setpiv

	Description:
	This subroutine is used for acquiring (setting) the
	program-root for KSH built-in commands.

	Synopsis:
	int proginfo_setpiv(proginfo *pip,cchar *pr,pivars *vars) noex

	Arguments:
	pip		program-information pointer
	pr		program root
	vars		array of program parameters

	Returns:
	>=0	length of PR
	<0	error (system-return)

*******************************************************************************/

#include	<envstandards.h>	/* MUST be first to configure */
#include	<sys/types.h>
#include	<sys/param.h>
#include	<unistd.h>
#include	<climits>
#include	<cstddef>		/* |nullptr_t| */
#include	<cstdlib>
#include	<cstring>
#include	<clanguage.h>
#include	<usysbase.h>
#include	<getx.h>
#include	<ids.h>
#include	<sfx.h>
#include	<snx.h>
#include	<mkpathx.h>
#include	<mkpr.h>		/* |getrootdname(3uc)| */
#include	<permx.h>		/* |permid(3uc)| */
#include	<localmisc.h>
#include	<libdebug.h>

#include	"defs.h"


/* local defines */

#define	PI	proginfo


/* external subroutines */

extern int	proginfo_getenv(PROGINFO *,cchar *,int,cchar **) noex ;


/* external variables */


/* local structures */


/* forward references */

local int	proginfo_setpiver(PI *,ids *,cchar *,pivars *) noex ;

local int	sfrootdirname(cchar *,int,cchar **) noex ;

local int	dircheck(ids *,cchar *) noex ;
local bool	isNotGoodDir(int) noex ;


/* local variables */


/* exported variables */


/* exported subroutines */

int proginfo_setpiv(PI *pip,cchar *pr,pivars *vars) noex {
	int		rs ;
	if (ids id ; (rs = ids_load(&id)) >= 0) {
	    rs = proginfo_setpiver(pip,&id,pr,vars) ;
	    ids_release(&id) ;
	} /* end if (ids) */
	return rs ;
}
/* end subroutine (proginfo_setpiv) */


/* local subroutines */

int proginfo_setpiver(PI *pip,ids *idp,cchar *pr,pivars *vars) noex {
	cint	plen = MAXPATHLEN ;
	int		rs = SR_OK ;
	int		rs1 ;
	int		pl = -1 ;
	int		prlen = 0 ;
	cchar		*cp = NULL ;
	char		rdn[MAXPATHLEN + 1] ;

#if	CF_DEBUG
	if (DEBUGLEVEL(3)) {
	    debugprintf("proginfo_setpiv: pr=%s\n",pr) ;
	    if (vars != NULL) {
	        debugprintf("proginfo_setpiv: vpr1=%s\n",vars->vpr1) ;
	        debugprintf("proginfo_setpiv: vpr2=%s\n",vars->vpr2) ;
	        debugprintf("proginfo_setpiv: vpr3=%s\n",vars->vpr3) ;
	        debugprintf("proginfo_setpiv: pr=%s\n",vars->pr) ;
	        debugprintf("proginfo_setpiv: vprname=%s\n",vars->vprname) ;
	    }
	}
#endif /* CF_DEBUG */

	if (pr == NULL) {
	    int	i ;

	    pl = -1 ;
	    rs1 = SR_NOTFOUND ;
	    for (i = 1 ; (rs >= 0) && isNotGoodDir(rs1) && (i <= 3) ; i += 1) {
	        cchar	*var  = NULL ;
	        switch (i) {
	        case 1:
	            var = vars->vpr1 ;
	            break ;
	        case 2:
	            var = vars->vpr2 ;
	            break ;
	        case 3:
	            var = vars->vpr3 ;
	            break ;
	        } /* end switch */
	        if ((var != NULL) && (var[0] != '\0')) {
	            if ((rs1 = proginfo_getenv(pip,var,-1,&cp)) >= 0) {
	                rs1 = dircheck(idp,cp) ;
	                if (! isNotGoodDir(rs1)) rs = rs1 ;
	            }
	        }
	    } /* end for */

	    if (rs1 >= 0) pr = cp ;
	} /* end if (straight out variables) */

	if ((rs >= 0) && (pr == NULL)) {
	    char	nn[MAXNAMELEN + 1] ;
	    char	dn[MAXHOSTNAMELEN + 1] ;

	    if ((rs1 = getnodedomain(nn,dn)) >= 0) {
	        cchar	**vpp = &pip->nodename ;
	        if ((rs = proginfo_setentry(pip,vpp,nn,-1)) >= 0) {
	            cchar	**vpp = &pip->domainname ;
	            if ((rs = proginfo_setentry(pip,vpp,dn,-1)) >= 0) {
	                rs1 = getrootdname(rdn,plen,vars->vprname,dn) ;
	                pl = rs1 ;
	            }
	        }

#if	CF_DEBUG
	        if (DEBUGLEVEL(3))
	            debugprintf("proginfo_setpiv: "
	                "getrootdname() rs=%d rootdname=%s\n",
	                rs1,rdn) ;
#endif

	        if ((rs >= 0) && (rs1 > 0))
	            pr = rdn ;

	    } /* end if (getnodedomain) */

	} /* end if (guess program root from domain name) */

/* try to see if a path was given at invocation */

	if ((rs >= 0) && (pr == NULL)) {

#if	CF_DEBUG
	    if (DEBUGLEVEL(3)) {
	        debugprintf("proginfo_setpiv: progename=%s\n",pip->progename) ;
	        debugprintf("proginfo_setpiv: progdname=%s\n",pip->progdname) ;
	    }
#endif

	    if (pip->progdname == NULL)
	        rs = proginfo_progdname(pip) ;

#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("proginfo_setpiv: _progdname() "
	            "rs=%d progdname=%s\n",
	            rs,pip->progdname) ;
#endif

	    if ((rs >= 0) && (pip->progdname != NULL)) {
	        cchar	*cp ;
	        pl = sfrootdirname(pip->progdname,-1,&cp) ;
#if	CF_DEBUG
	        if (DEBUGLEVEL(3)) {
	            debugprintf("proginfo_setpiv: sfrootdirname() pl=%d\n",pl) ;
	            debugprintf("proginfo_setpiv: p=>%t<\n",cp,pl) ;
	        }
#endif

	        if (pl > 0) pr = cp ;
	    }

#if	CF_DEBUG
	    if (DEBUGLEVEL(3))
	        debugprintf("proginfo_setpiv: sfrootdirname() "
	            "rs=%d pr=%s\n",rs,pr) ;
#endif

	} /* end if (set program-root from program d-name) */

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("proginfo_setpiv: mid pr=%s\n",pr) ;
#endif

/* default is a fixed string (from the initialization variables) */

	if ((rs >= 0) && (pr == NULL)) {
	    pr = vars->pr ;
	    pl = -1 ;
	}

/* enter it in if we have found it */

	if ((rs >= 0) && (pr != NULL)) {
	    rs = proginfo_setprogroot(pip,pr,pl) ;
	    prlen = rs ;
	}

#if	CF_DEBUG
	if (DEBUGLEVEL(3))
	    debugprintf("proginfo_setpiv: ret rs=%d prlen=%u pr=%s\n",
	        rs,prlen,pip->pr) ;
#endif

	return (rs >= 0) ? prlen : rs ;
}
/* end subroutine (proginfo_setpiver) */

local int sfrootdirname(char *dp,int dl,cchar **rpp) noex {
	int		bl ;
	int		sl = -1 ;
	int		f ;
	cchar	*sp = NULL ;
	cchar	*bp ;

	if (rpp != NULL)
	    *rpp = NULL ;

#if	CF_DEBUGS
	debugprintf("sfrootdirname: d=%t\n",dp,dl) ;
#endif

	bl = sfbasename(dp,dl,&bp) ;

#if	CF_DEBUGS
	debugprintf("sfrootdirname: b=%t\n",bp,bl) ;
#endif

	f = ((bl == 3) && (strncmp(bp,"bin",bl) == 0)) ;

	if (! f)
	    f = ((bl == 4) && (strncmp(bp,"sbin",bl) == 0)) ;

#if	CF_DEBUGS
	debugprintf("sfrootdirname: f=%u\n",f) ;
#endif

	if (f) {
	    sl = sfdirname(dp,dl,&sp) ;

#if	CF_DEBUGS
	    debugprintf("sfrootdirname: pr=%t\n",sp,sl) ;
#endif
	    if ((sl >= 0) && (rpp != NULL)) *rpp = sp ;
	}

#if	CF_DEBUGS
	debugprintf("sfrootdirname: ret sl=%d\n",sl) ;
#endif
	return sl ;
}
/* end subroutine (sfrootdirname) */

local int dircheck(ids *idp,cchar *dname) noex {
	int		rs ;
	if (ustat sb ; (rs = u_stat(dname,&sb)) >= 0) {
	    if (S_ISDIR(sb.st_mode)) {
	        cint	am = (R_OK|X_OK) ;
	        rs = permid(idp,&sb,am) ;
	    } else {
	        rs = SR_NOTDIR ;
	    }
	}
	return rs ;
}
/* end subroutine (dircheck) */

local bool isNotGoodDir(int rs) noex {
	bool f = FALSE ;
	f = f || isNotPresent(rs) ;
	f = f || (rs == SR_NOTDIR) ;
	return f ;
}
/* end if (isNotGoodDir) */


