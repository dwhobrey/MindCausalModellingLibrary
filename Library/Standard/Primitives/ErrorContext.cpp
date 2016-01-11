#include "PlatoIncludes.h"
#include "Numbers.h"
#include "ErrorMessage.h"
#include "ErrorContext.h"

namespace Plato {

    ErrorContext::ErrorContext(int stackSize) {
        mErrorMessageStack = new vector<ErrorMessage*>();
        if (stackSize <= 0) {
            stackSize = DefaultStackSize;
        }
        mStackSize = stackSize;
        ExitCode = 0;
    }

    ErrorContext::~ErrorContext() {
        for (vector<ErrorMessage*>::iterator itr = mErrorMessageStack->begin();
            itr != mErrorMessageStack->end(); ++itr)  {
            delete *itr;
        }
        delete mErrorMessageStack;
    }

    int ErrorContext::Count() {
        return (int)mErrorMessageStack->size();
    }

    void ErrorContext::Add(ErrorMessage& errorMessage) {
        if ((int)mErrorMessageStack->size() < mStackSize) {
            mErrorMessageStack->push_back(&errorMessage);
        }
    }

    ErrorMessage* ErrorContext::CodeToMessage(int errorCode) {
        vector<ErrorMessage*>::iterator itr;
        for(itr=mErrorMessageStack->begin();itr != mErrorMessageStack->end(); ++itr) {
            if ((*itr)->mErrorCode == errorCode) {
                return *itr;
            }
        }
        return NULL;
    }

    string& ErrorContext::StatusReport() {
        string& errors = *new string("ErrorStack:{");
        vector<ErrorMessage*>::iterator itr;
        for(itr=mErrorMessageStack->begin(); itr != mErrorMessageStack->end();++itr) {
            string& tmp = (*itr)->StatusReport();
            errors += "\n" + tmp;
            delete &tmp;
        }
        string& num = Numbers::ToString(ExitCode);
        errors += "},\nExitCode=" + num + ".";
        delete &num;
        return errors;
    }
}
