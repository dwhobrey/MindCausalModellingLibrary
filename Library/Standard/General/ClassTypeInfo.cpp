#include "PlatoIncludes.h"
#include "Strings.h"
#include "Arguments.h"
#include "ClassTypeInfo.h"

namespace Plato {

    size_t ClassTypeInfo::FilterTypeCode = 0;

    ClassTypeInfo::ClassTypeInfo(const char* className, int classID, int libraryID, 
        FactoryDelegate factory, const ClassTypeInfo* baseClassTypeInfo) {
        ClassName = new string(className);
        ClassID = classID;
        LibraryID = libraryID;
        Factory = factory;
        BaseClassTypeInfo = baseClassTypeInfo;
        IsFilter = false;
        InheritanceCount = 0;
        TypeCode = GetTypeCode(libraryID,classID);
        while(baseClassTypeInfo!=NULL) {
            ++InheritanceCount;
            baseClassTypeInfo = baseClassTypeInfo->BaseClassTypeInfo;
        }
        if(InheritanceCount>0) {
            InheritedTypeCodes = new size_t[InheritanceCount];
            baseClassTypeInfo = BaseClassTypeInfo;
            int pos = InheritanceCount;
            while(baseClassTypeInfo!=NULL) {
                size_t typeCode = baseClassTypeInfo->TypeCode;
                if(typeCode==FilterTypeCode) IsFilter=true;
                InheritedTypeCodes[--pos] = typeCode;
                baseClassTypeInfo = baseClassTypeInfo->BaseClassTypeInfo;
            }
        } else {
            InheritedTypeCodes = NULL;
        }
        if(FilterTypeCode==0 && ClassName->compare("Filter")==0) {
            FilterTypeCode = TypeCode;
            IsFilter = true;
        }
    }

    ClassTypeInfo::~ClassTypeInfo() {
        delete ClassName;
        delete [] InheritedTypeCodes;
    }

    size_t ClassTypeInfo::GetTypeCode(int libraryID, int classID) {
        if (libraryID < 0) {
            libraryID = 0;
        }
        if (classID < 0 || classID > ClassTypeCodeMaximum) {
            classID = 0;
        }
        return (size_t)((libraryID << LibraryTypeCodeStartBit) + classID);
    }



    int ClassTypeInfo::GetInheritanceDifference(const ClassTypeInfo& baseType, const ClassTypeInfo& derivedType) {
        size_t baseCode = baseType.TypeCode;
        if(baseCode==derivedType.TypeCode) {
            return 0;
        }
        const size_t* pTypeCodes = derivedType.InheritedTypeCodes;
        int pos = derivedType.InheritanceCount;
        int n = 0;
        while(n++ < pos) {
            if(*pTypeCodes == baseCode) return n;
            ++pTypeCodes;
        }
        return -1;
    }

    const ClassTypeInfo* ClassTypeInfo::GetClosestBase(const ClassTypeInfo::HashMap& classes) {
        int minInheritanceCount = -1;
        ClassTypeInfo::HashMap::const_iterator itr;
        const ClassTypeInfo* lastType = NULL;
        for(itr=classes.begin();itr!=classes.end();++itr) {
            const ClassTypeInfo* p = itr->second;
            int inheritanceCount = GetInheritanceDifference(*p,*this);
            if(inheritanceCount>=0) {
                if (minInheritanceCount < 0 || inheritanceCount < minInheritanceCount) {
                    lastType = p;
                    minInheritanceCount = inheritanceCount;
                }
            }
        }
        return lastType;
    }

    ClassTypeInfo::HashMap& ClassTypeInfo::CreateHashMap(const ClassTypeInfo* first,...) {
        va_list argPtr;
        va_start(argPtr, first);
        HashMap& hm = *new HashMap();
        while(first!=NULL) {
            hm[first->ClassName] = first;
            first = (ClassTypeInfo*)(va_arg(argPtr,ClassTypeInfo*));
        }
        return hm;
    }
}
