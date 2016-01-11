#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "ClassCatalogue.h"

namespace Plato {

    ClassTypeInfo::HashMap* ClassCatalogue::ClassCatalogueTable = NULL;
    const int ClassCatalogue::CatalogueCodeFieldMaximum[] = { 0x7f, 0xfff, 0xfff };
    const int ClassCatalogue::CatalogueCodeFieldBase[] = { 0x1000000, 0x1000, 0x1 };
    const int ClassCatalogue::CatalogueCodeFieldStartBit[] = { 24, 12, 0 };

    void ClassCatalogue::Initializer() {
        ClassCatalogueTable = new ClassTypeInfo::HashMap();
    }

    void ClassCatalogue::Finalizer() {
        delete ClassCatalogueTable;
    }

    const ClassTypeInfo* ClassCatalogue::FindEntry(const string& className) {
        ClassTypeInfo::HashMap::iterator itr = ClassCatalogueTable->find(&className);
        return (itr==ClassCatalogueTable->end()) ? NULL : itr->second;
    }

    void ClassCatalogue::Add(const ClassTypeInfo* classInfoPtr) {
        (*ClassCatalogueTable)[classInfoPtr->ClassName] = classInfoPtr;
    }

    int ClassCatalogue::CatalogueCode(int libraryID, int classID, int statementID) {

        if (libraryID < 0 || libraryID > CatalogueCodeFieldMaximum[(int)(CatalogueCodeFields::Library)]) {
            libraryID = 0;
        }
        if (classID < 0 || classID > CatalogueCodeFieldMaximum[(int)(CatalogueCodeFields::Class)]) {
            classID = 0;
        }
        if (statementID < 0 || statementID > CatalogueCodeFieldMaximum[(int)(CatalogueCodeFields::Statement)]) {
            statementID = 0;
        }
        return (libraryID << CatalogueCodeFieldStartBit[(int)(CatalogueCodeFields::Library)])
            + (classID << CatalogueCodeFieldStartBit[(int)(CatalogueCodeFields::Class)])
            + (statementID << CatalogueCodeFieldStartBit[(int)(CatalogueCodeFields::Statement)])
            ;
    }

    int ClassCatalogue::GetCatalogueField(CatalogueCodeFieldsEnum catalogueCodeField, int catalogueCode) {
        return (catalogueCode >> CatalogueCodeFieldStartBit[(int)(catalogueCodeField)])
            & CatalogueCodeFieldMaximum[(int)(catalogueCodeField)];
    }

    int ClassCatalogue::SetCatalogueField(CatalogueCodeFieldsEnum catalogueCodeField, int catalogueCode, int fieldValue) {
        int maxID = CatalogueCodeFieldMaximum[(int)(catalogueCodeField)];
        int startBit = CatalogueCodeFieldStartBit[(int)(catalogueCodeField)];
        int mask = maxID << startBit;
        if (fieldValue < 0 || fieldValue > maxID) {
            fieldValue = 0;
        }
        catalogueCode |= mask;
        catalogueCode ^= mask;
        return catalogueCode | (fieldValue << startBit);
    }
}
