#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "ConfigurePhases.h"
#include "Property.h"
#include "Container.h"
#include "Alias.h"

#include "ContainerTest.h"

namespace Plato {

    class ContainerTestContainer : public Container {
        public:
            ContainerTestContainer(Identifier& identifier);
        };


    ContainerTestContainer::ContainerTestContainer(Identifier& identifier)
        : Container(*Container::PropertyTypes,NULL,NULL,identifier,PropertyScopes::Public) {
    }

    int ContainerTest::ConductUnitTest(string& message) {
        message = "";
        int failureCount = 0;
        bool passed = false;

#pragma region // Test of Container.
        message += "Container: ";
        for(;;) {
            try {
                const char* testItems[] = { "b23", "b21", "b24", "b22",NULL };
                string testAnswer = "( b23 b21 b24 b22)";

                ContainerTestContainer* ct = new ContainerTestContainer(*new Identifier("UnitTestContainer"));

                int i=0;
                const char* s;
                while((s=testItems[i++])!=NULL) {
                    Alias* a = new Alias(ct,(Container*)NULL,*new Identifier(s),PropertyScopes::Public);
                    ct->Add(*a);
                }

                vector<Property*>::iterator itr = ct->OrderedProperties->begin();
                string testResult = "(";
                for(itr=ct->OrderedProperties->begin();itr!=ct->OrderedProperties->end();++itr) {
                    string & tmp = (*itr)->Identity->StatusReport();
                    testResult += " " + tmp;
                    delete &tmp;
                }
                testResult += ")";
                bool result = testAnswer.compare(testResult)==0;

                // DEBUG: TO DO: Add a IdentifierEnumerator test.

                delete ct; // DEBUG: TO DO: Add a memory leak test.

                if (!result) {
                    message += "Addition test: <" + testAnswer + "> != <" + testResult + ">: Failed.";
                    break;
                }
            } catch (...) {
                message += "Exception: Failed.";
                break;
            }
            passed = true;
            break;
        }

        failureCount += passed ? 0 : 1;
#pragma endregion
        return failureCount;
    }
}
