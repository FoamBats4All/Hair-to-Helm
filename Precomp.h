#ifndef HELMTOHAIR_PRECOMP_H
#define HELMTOHAIR_PRECOMP_H

#ifdef _MSC_VER
#pragma once
#endif

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE_GLOBALS
#define _STRSAFE_NO_DEPRECATE

#pragma warning(disable:4995)	// Warning C4995 : name was marked as #pragma deprecated

// ...?
#include <winsock2.h>
#include <windows.h>
#include <windowsx.h>
#undef GetFirstChild
#include <shlobj.h>
#include <process.h>
#include <tchar.h>
#include <strsafe.h>
#include <hash_map>
#include <sys/types.h>
#include <sys/stat.h>

// C Libraries.
#include <climits>		// Sizes of integral types.
#include <cmath>		// C numerics library.
#include <io.h>			// ...
#include <cstdio>		// C IO.
#include <cstdlib>		// C Standard General Utilities Library.
#include <cstddef>		// C Standard definitions.
#include <ctime>		// C Time Library.
#include <cfloat>		// Characteristics of floating-point types.
#include <direct.h>		// ???
#include <fcntl.h>		// ???
#include <share.h>		// ???

// STL Containers.
#include <bitset>		// Bit arrays.
#include <deque>		// Double-ended queue.
#include <list>			// Linked lists.
#include <queue>		// FIFO queue.
#include <map>			// Associative arrays.
#include <set>			// Trees.
#include <stack>		// LIFO stack.
#include <vector>		// Dynamic arrays.

// Miscellaneous.
#include <algorithm>	// Standard Template Library: Algorithms.
#include <exception>	// Standard exception class.
#include <functional>	// Function objects.
#include <string>		// C++ Strings library.

// Input/Output.
#include <iostream>		// Communication through standard input/output (cin/cout).
#include <fstream>		// File stream manipulation.
#include <sstream>		// Manipulate std::string as streams.
#include <iomanip>		// Formatting options.

// Encrypt?
#ifdef ENCRYPT
#include <protect.h>
#endif

// Skywing's utilities.
#include "../SkywingUtils/SkywingUtils.h"	// Skywing's utilitiy libraries.

// Skywing's NWN2 Data Library.
#include "../NWNBaseLib/NWNBaseLib.h"		// Basic NWN constants/APIs.
#include "../NWN2MathLib/NWN2MathLib.h"		// Mathematical NWN2 functions (such as pathfinding).
#include "../Granny2Lib/Granny2Lib.h"		// Manipulation of *.gr2 files.
#include "../NWN2DataLib/NWN2DataLib.h"		// Neverwinter Nights 2 data access API.

// Foam's utilities.
#include "../FoamUtils/FoamUtils.h"			// Foam's utility libraries.

// Boost libraries.
#include <boost\program_options.hpp>		// Reading program options (ini files).
#include <boost\filesystem.hpp>				// Reading/checking paths.
#include <boost\lexical_cast.hpp>			// Typecasting and conversions.
#include <boost\algorithm\string\trim.hpp>	// Trimming strings.

// ...
#define ARGUMENT_PRESENT( x )  ( (x) )

// Maths.
#define PI                 3.14159265f
#define DEGREES_TO_RADIANS 0.0174532925f

#endif