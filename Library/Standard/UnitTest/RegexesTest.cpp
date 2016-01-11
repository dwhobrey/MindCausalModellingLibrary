#include "PlatoIncludes.h"
#include "Regexes.h"

#include "RegexesTest.h"

namespace Plato {

    int RegexesTest::ConductUnitTest(string& message) {

        message = "";
        int failureCount = 0;
        bool passed = false;

        message += "Regexes: ";
        try {
            for(;;) {
                if(!Regexes::Matches("abcde","abcde")) {
                    message += "Matching('abcde','abcde'): Failed.";
                    break;
                }
                if(!Regexes::Matches("abcde","^abcde$")) {
                    message += "Matching('abcde','^abcde$'): Failed.";
                    break;
                }
                if(!Regexes::Matches("abcde", "^abc.*$")) {
                    message += "Matches('abcde','^abc.*$'): Failed.";
                    break;
                }
                if(!Regexes::Matches("abcde", ".+")) {
                    message += "Matches('abcde','.+'): Failed.";
                    break;
                }
                if(!Regexes::IsRegex("abcde.")) {
                    message += "IsRegex('abcde.'): Failed.";
                    break;
                }
                if(Regexes::IsRegex("abcde")) {
                    message += "IsRegex('abcde'): Failed.";
                    break;
                }
                passed = true;
                break;
            }
        } catch (...) {
            message += "Regexes exception: Failed.";
        }

        failureCount += passed ? 0 : 1;
        return failureCount;
    }
}
