#pragma once

#include <cstdio>
#include <cassert>

#define DBG 1

#if DBG
#define DBGLOG(note) printf("\x1b[32m[DEBUG]: %s:%i    %s: \"%s\"\n\x1b[39m", __FILE__, __LINE__, __PRETTY_FUNCTION__, note);
#define DBGTRACE()   printf("\x1b[32m[DEBUG]: %s:%i    %s\n\x1b[39m", __FILE__, __LINE__, __PRETTY_FUNCTION__);
#else
#define DBGLOG(note)
#define DBGTRACE()
#endif