#include "PlatoIncludes.h"
#include "Diagnostics.h"
#include "DiagnosticsCatalogue.h"

#include "ArchiverTest.h"
#include "RegexesTest.h"
#include "ContainerTest.h"
#include "PathTest.h"

namespace Plato {

    vector<DiagnosticsCatalogueEntry*>* DiagnosticsCatalogue::DiagnosticsCatalogueTable = NULL;
     
    void DiagnosticsCatalogue::Initializer() {
        DiagnosticsCatalogueTable = new vector<DiagnosticsCatalogueEntry*>();
        DiagnosticsCatalogueTable->push_back(new DiagnosticsCatalogueEntry("RegexesTest",RegexesTest::ConductUnitTest));
        DiagnosticsCatalogueTable->push_back(new DiagnosticsCatalogueEntry("ArchiverTest",ArchiverTest::ConductUnitTest));
        DiagnosticsCatalogueTable->push_back(new DiagnosticsCatalogueEntry("ContainerTest",ContainerTest::ConductUnitTest));
        DiagnosticsCatalogueTable->push_back(new DiagnosticsCatalogueEntry("PathTest",PathTest::ConductUnitTest));
    }

    void DiagnosticsCatalogue::Finalizer() {
        vector<DiagnosticsCatalogueEntry*>::iterator itr;
        for(itr=DiagnosticsCatalogueTable->begin();itr!=DiagnosticsCatalogueTable->end();++itr) {
            delete *itr;
        }
        delete DiagnosticsCatalogueTable;
    }
}
