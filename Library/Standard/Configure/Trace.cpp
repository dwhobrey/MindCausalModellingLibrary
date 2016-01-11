#include "PlatoIncludes.h"
#include "Strings.h"
#include "Configuration.h"
#include "Trace.h"

namespace Plato {
    void Trace::WriteLine(int level, const char* format, ...) {
        if (Configuration::TraceLevel >= level) {
            va_list argPtr;
            va_start(argPtr, format);
            char* messagePtr = Strings::Sprintf(format,argPtr);
            cerr << messagePtr << "\n";
            delete [] messagePtr;
        }
    }
}
