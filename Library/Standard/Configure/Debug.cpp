#include "PlatoIncludes.h"
#include "Strings.h"
#include "Configuration.h"
#include "Debug.h"

namespace Plato {

    void Debug::Initializer() {
#if defined(_DEBUG) & defined(WIN32)
        _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
        _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDERR );
  
        int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
        tmpDbgFlag |= 
            _CRTDBG_ALLOC_MEM_DF // Enable debug heap allocations and use of memory block type identifiers.
            // | _CRTDBG_CHECK_CRT_DF // Track C run-time allocations.
            | _CRTDBG_LEAK_CHECK_DF // Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks.
            // | _CRTDBG_DELAY_FREE_MEM_DF // This catches stale ptr writes.
            // | _CRTDBG_CHECK_ALWAYS_DF // This slows down execution!
            // | _CRTDBG_CHECK_EVERY_1024_DF // The number of mallocs between leak checking.
         ;
        _CrtSetDbgFlag(tmpDbgFlag);

#if 0 // For setting a breakpoint on a particular memory allocation:
        static int memAllocNumber = 0;
        _CrtSetBreakAlloc(memAllocNumber);
        // Via debugger: Watch Name: {,,msvcr80d.dll}_crtBreakAlloc
#endif

#endif
    }

    void Debug::Finalizer() {
        // In debug mode, MSVC dumps leaks automatically.
#if 0
        _CrtDumpMemoryLeaks();
#endif
    }

    void Debug::WriteLine(int level, const char* format, ...) {
        if (Configuration::DebugLevel >= level) {
            va_list argPtr;
            va_start(argPtr, format);
            char* messagePtr = Strings::Sprintf(format,argPtr);
            cerr << messagePtr << "\n";
            delete [] messagePtr;
        }
    }
}
