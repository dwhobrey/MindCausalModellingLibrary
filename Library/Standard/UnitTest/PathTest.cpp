#include "PlatoIncludes.h"
#include "Regexes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "EnvironmentVariables.h"
#include "Configuration.h"
#include "Identifier.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "ConfigurePhases.h"
#include "Property.h"
#include "Container.h"
#include "Path.h"
#include "PathNode.h"
#include "PathEnumerator.h"
#include "PointSpace.h"
#include "PointRegex.h"
#include "PathOperators.h"
#include "PathTest.h"
#include "Alias.h"
#include "ContainerEnumerator.h"

namespace Plato {

    class PathTestContainer : public Container {
    public:
        PathTestContainer(Identifier& identifier);
    };

    PathTestContainer::PathTestContainer(Identifier& identifier)
        : Container(*Container::PropertyTypes,NULL,NULL,identifier,PropertyScopes::Public) {
    }

    int PathTest::ConductUnitTest(string& message) {
        message = "";
        int failureCount = 0;
        bool passed = false;

#pragma region // Test of Path.
        message += "Path: ";
        for(;;) {
            // Test of path construction.
            string& testPathString = *new string("/tmp/PathTest/Filter:typeof(NameFilter)(args)|b..");
            try {
                Path* explicitPath = new Path(NULL,*Path::Absolute / "tmp" / "PathTest" / FilterName(".2") | "b..") ;
                string& explicitPathString = explicitPath->StatusReport();
                delete explicitPath;
                if (testPathString.compare(explicitPathString)!=0) {
                    message += "Explicit path construction (" + testPathString + "!=" + explicitPathString + "): Failed";
                    delete &explicitPathString;
                    delete &testPathString;
                    break;
                }
                delete &explicitPathString;
            } catch (...) {
                message += "Path construction: Failed";
                delete &testPathString;
                break;
            }
            delete &testPathString;

            // Test of path enumeration.
            PathTestContainer* d = new PathTestContainer(*new Identifier("tmp"));
            if(d==NULL) {
                message += "Constructing test container: Failed.";
                break;
            }

            for(int i=0;i<3;i++) {
                string& dirname = *new string("a");
                dirname+= (char)('1'+i);
                PathTestContainer* c = new PathTestContainer(*new Identifier(dirname));
                d->Add(*c);
                for(int k=0;k<3;k++) {
                    string& b = *new string("b");
                    b+= (char)('1'+i);
                    b+= (char)('1'+k);
                    Alias* a = new Alias(c,(Container*)NULL,*new Identifier(b),PropertyScopes::Public);
                    c->Add(*a);
                }
            }

            string& testEnumResult = *new string("( b21 b22 b23)");
            string* props = new string("(");
            try {
                Path* p = new Path(NULL,*Path::Relative / ".2" / "b.."); 
                if (p == NULL) {
                    message += "Path construction for enumeration: Failed";
                    break;
                }

                PathEnumerator* pie = p->GetEnumerator(d,NULL,PropertyModes::Traversing);
                while (pie->MoveNext()) {
                    Property* a = pie->Current;
                    *props += " " + *(a->Identity->Name);
                }
                delete pie;
                delete p;
                *props +=")";
                if (props->compare(testEnumResult)!=0) {
                    message +="Path enumeration results differ: "+testEnumResult+" and "+*props+": Failed.";
                    break;
                }
            } catch (...) {
                message += "Path enumeration: Failed.";
                break;
            }
            delete props;
            delete &testEnumResult;

            // Test Any paths.
            string& testAnyResult = *new string("( b21 b22 b23)");
            props = new string("(");
            try {
                Path* p = new Path(NULL,*Path::Relative / Path::Any / "b2.");
                if (p == NULL) {
                    message += "Path construction for Any: Failed";
                    break;
                }

                PathEnumerator* pie = p->GetEnumerator(d,NULL,PropertyModes::Traversing);
                while (pie->MoveNext()) {
                    Property* a = pie->Current;
                    *props += " " + *(a->Identity->Name);
                }
                delete pie;
                delete p;
                *props +=")";
                if (props->compare(testAnyResult)!=0) {
                    message +="Path Any results differ: "+testAnyResult+" and "+*props+": Failed.";
                    break;
                }
            } catch (...) {
                message += "Path any: Failed.";
                break;
            }
            delete props;
            delete &testAnyResult;

            // Test of filters.
            // TO DO.

            // Finally, tidy up after tests.
            try {
                // This will recursively delete container hierarchy.
                delete d;
            } catch(...) {
                message += "Remove directory: Failed.";
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
