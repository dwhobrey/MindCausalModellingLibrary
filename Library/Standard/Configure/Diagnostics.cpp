#include "PlatoIncludes.h"
#include "Trace.h"
#include "Diagnostics.h"
#include "DiagnosticsCatalogue.h"

namespace Plato {

    DiagnosticsCatalogueEntry::DiagnosticsCatalogueEntry(const string& className, 
            DiagnosticsCatalogueEntry::ConductUnitTestDelegate unitTestDelegate) 
            : mClassName(className) {
        mUnitTestDelegate = unitTestDelegate;
    }

    int Diagnostics::ConductTests() {
        Trace::WriteLine(1, "Diagnostics.ConductTests: starting unit tests...");
        int failureCount = 0;
        string failureMessage = "";
        // Call each unit test class in turn.
        vector<DiagnosticsCatalogueEntry*>::iterator itr;
        string& message = *new string("");
        for(itr=DiagnosticsCatalogue::DiagnosticsCatalogueTable->begin();
            itr!=DiagnosticsCatalogue::DiagnosticsCatalogueTable->end();itr++) {
            int unitFailures = 0;
            message.clear();
            try {
                unitFailures = (*itr)->mUnitTestDelegate(message);
            } catch (...) {
                unitFailures = 1;
                message = "Unit test failed for:" + (*itr)->mClassName + ".";
            }

            if (unitFailures > 0) {
                failureCount += unitFailures;
                failureMessage += message + "\n";
            }
        }
        delete &message;
        if (failureCount > 0) {
            Trace::WriteLine(1,"Failures encountered: %d.\n%s", failureCount, failureMessage.c_str());
        } else {
            Trace::WriteLine(1, "Diagnostics.ConductTests: unit tests completed successfully.");
        }
        return failureCount;
    }
}
