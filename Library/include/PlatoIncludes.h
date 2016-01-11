#pragma once

#ifdef WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#define WIN32_LEAN_AND_MEAN
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include "Windows.h"
#include <direct.h>
#include <hash_set>
#include <hash_map>
using namespace stdext;
#else // POSIX
#include "pthread.h"
#include "time.h"
#include "unistd.h"
#undef __DEPRECATED
#include <ext/hash_set>
#include <ext/hash_map>
using namespace __gnu_cxx;
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>
#include <signal.h>
#include <cstdlib>
#include <cfloat>
#include <cmath>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "pstdint.h" 

using namespace std;
