/*
 * Worldvisions Weaver Software:
 *   Copyright (C) 1997-2002 Net Integration Technologies, Inc.
 * 
 * A class for managing error numbers and strings.  See wverror.h.
 */
#include "wverror.h"
#include <assert.h>

#ifdef _WIN32
#include "windows.h"
#endif

WvErrorBase::~WvErrorBase()
{
    // nothing special
}


WvString WvErrorBase::errstr() const
{
    int errnum = geterr();
    
    if (errnum < 0)
    {
	assert(!!errstring);
	return errstring;
    }
    else
    {
	if (!!errstring) return errstring;
#ifndef _WIN32
	return strerror(errnum);
#else
	if (errnum >= WSABASEERR && errnum < WSABASEERR+2000)
	{
	    char msg[4096];
	    const HMODULE module = GetModuleHandle("winsock.dll");
	    DWORD result = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, module, errnum, 0, msg, sizeof(msg), 0);
	    if (result)
		return msg;
	    else
	    {
		DWORD e = GetLastError();
		return WvString("Unknown format %s for error %s", e, errnum);
	    }
	}
	else
	    return strerror(errnum);
#endif
    }
}


void WvErrorBase::seterr(int _errnum)
{
    if (!errnum)
    {
        assert(_errnum != -1 || !!errstring
	    && "attempt to set errnum to -1 without also setting errstring");
#ifdef _WIN32
	if (_errnum == WSAECONNABORTED)
	    _errnum = WSAECONNREFUSED; // work around WINE bug
#endif
	errnum = _errnum;
    }
}


void WvErrorBase::seterr(WvStringParm specialerr)
{
    assert(!!specialerr);
    if (!errnum)
    {
	errstring = specialerr;
	seterr(-1);
    }
}


void WvErrorBase::seterr(const WvErrorBase &err)
{
    if (err.geterr() > 0)
	seterr(err.geterr());
    else if (err.geterr() < 0)
	seterr(err.errstr());
}


void WvErrorBase::seterr_both(int _errnum, WvStringParm specialerr)
{
    assert(!!specialerr);
    if (!errnum)
    {
	errstring = specialerr;
	seterr(_errnum);
    }
}
