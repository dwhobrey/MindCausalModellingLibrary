#include "PlatoIncludes.h"
#include "Numbers.h"
#include "Archiver.h"
#include "ArchiverTest.h"

namespace Plato {

    ArchiverTest::ArchieTestClass::ArchieTestClass(int a, string& b) : mB(b) {
        mA = a;
    }

    ArchiverTest::ArchieTestClass::~ArchieTestClass() {
        delete &mB;
    }

    string& ArchiverTest::ArchieTestClass::StatusReport() {
        string& num = Numbers::ToString(mA);
        string& result = *new string("(" + num + "," + mB + ")");
        delete &num;
        return result;
    }

    int ArchiverTest::ConductUnitTest(string& message) {

        message = "";
        int failureCount = 0;
#if 0 // DEBUG: Remove when archiver working.
        bool passed = false;

#pragma region // Test of Archiver Serialise and Deserialise.
        message += "Serialise and Deserialise: ";
        try {
            ArchieTestClass* s = NULL;
            ArchieTestClass* d = NULL;
            for(;;) {
                string& abc = *new string("ABC");
                s = new ArchieTestClass(123,abc);
                string path = "/tmp/archietest.xml";

                void* pVoid = (void*)s;
                int result = Archiver::Serialize(path,false,pVoid);
                if(result!=0) {
                    message += "Serializing test object: Failed.";
                    break;
                }
                pVoid = Archiver::Deserialize(path);
                if (pVoid==NULL) {
                    message += "Deserializing test object: Failed.";
                    break;
                }
                d = (ArchieTestClass*)(pVoid);
                if (d == NULL) {
                    message += "Converting type of deserialised test object: Failed.";
                    break;
                }
                if (d->mA != s->mA || d->mB.compare(s->mB)!=0) {
                    string& ds = d->StatusReport();
                    string& ss = s->StatusReport();
                    message += "Content of deserialised test object differs: " + ds + "!=" + ss + ".";
                    delete &ds;
                    delete &ss;
                    break;
                }
                passed = true;
                break;
            }
            delete s;
            delete d;
        } catch (...) {
            message += "Archiver exception: Failed.";
        }

        failureCount += passed ? 0 : 1;
#endif
#pragma endregion

        return failureCount;
    }
}
