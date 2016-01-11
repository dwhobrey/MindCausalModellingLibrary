#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "ClassCatalogue.h"
#include "ErrorContext.h"
#include "ErrorMessage.h"
#include "Error.h"

namespace Plato {

#if defined(DEBUG)
    bool Error::EchoFlag = true;
#else
    bool Error::EchoFlag = false;
#endif

    int Error::Log(bool echoToConsole, ErrorContext& errorContext, const ClassTypeInfo& classInfo,
        const char* methodName, int errorIndex, const char* errorFormatMessage, ...) {
        // Convert the message and arguments to a single string.
        va_list argPtr;
        va_start(argPtr,errorFormatMessage);
        char* messagePtr = Strings::Sprintf(errorFormatMessage,argPtr);

        // Work out the method and the type of the class that called the Log method.
        int errorCode = ClassCatalogue::CatalogueCode(classInfo.ClassID, classInfo.LibraryID,errorIndex);
        string* errorMessage = new string();
        *errorMessage += *(classInfo.ClassName) + "." + methodName + "." + messagePtr;
        delete [] messagePtr;

        // Generate a context qualified error message.
        ErrorMessage* em = new ErrorMessage(errorCode, *errorMessage);
        // Log the error to stack or console as required.
        errorContext.Add(*em);
        if (echoToConsole||EchoFlag) {
            string& s = em->StatusReport();
            cerr << s << "\n";
            delete &s;
        }
        return errorCode;
    }
}
