#include "PlatoIncludes.h"
#include "Strings.h"
#include "ErrorMessage.h"

namespace Plato {

    ErrorMessage::ErrorMessage(int errorCode, const string& errorMessage) {
        mErrorCode = errorCode;
        mErrorMessage = &errorMessage;
    }

    ErrorMessage::~ErrorMessage() {
        delete mErrorMessage;
    }

    string& ErrorMessage::StatusReport() {
        return Strings::Format("E%x:%s",mErrorCode,mErrorMessage->c_str());
    }
}

