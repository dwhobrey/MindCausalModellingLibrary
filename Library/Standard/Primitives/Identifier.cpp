#include "PlatoIncludes.h"
#include "Strings.h"
#include "Numbers.h"
#include "Point.h"
#include "Identifier.h"

namespace Plato {

    int Identifier::mLastIdentifierNumber = 0;

    Identifier::Identifier(const char* name, const Point* position, Property* prop) {
        if (name != NULL) {
            Name = new string(name);
            NameHashCode = Strings::SuperFastHash(Name->data(),(int)Name->size());
        } else {
            Name = NULL;
            NameHashCode = 0;
        }
        HashCode = NameHashCode;
        Position = position;
        if (Position != NULL) {
            HashCode ^= Position->HashCode;
        } 
        PropertyPtr = prop;
    }

    Identifier::Identifier(const string& name, const Point* position, Property* prop) {
        Name = &name;
        NameHashCode = Strings::SuperFastHash(Name->data(),(int)Name->size());
        HashCode = NameHashCode;
        Position = position;
        if (Position != NULL) {
            HashCode ^= Position->HashCode;
        } 
        PropertyPtr = prop;
    }

    Identifier::Identifier(char name) {
        Name = new string(1,name);
        NameHashCode = Strings::SuperFastHash(Name->data(),(int)Name->size());
        HashCode = NameHashCode;
        Position = NULL;
        PropertyPtr = NULL;
    }

    Identifier::Identifier(const char* name) {
        if (name != NULL) {
            Name = new string(name);
            NameHashCode = Strings::SuperFastHash(Name->data(),(int)Name->size());
        } else {
            Name = NULL;
            NameHashCode = 0;
        }
        HashCode = NameHashCode;
        Position = NULL;
        PropertyPtr = NULL;
    }

    Identifier::Identifier(const string& name) {
        Name = &name;
        NameHashCode = Strings::SuperFastHash(Name->data(),(int)Name->size());
        HashCode = NameHashCode;
        Position = NULL;
        PropertyPtr = NULL;
    }

    Identifier::Identifier(const Identifier& ident) {
        Name = new string(*ident.Name);
        const Point* p = ident.Position;
        Position = p==NULL ? NULL : new Point(*p);
        PropertyPtr = ident.PropertyPtr;
        NameHashCode = ident.NameHashCode;
        HashCode = ident.HashCode;
    }

    Identifier::~Identifier() {
        delete Name;
        delete Position;
    }

    void Identifier::SetName(const string& name) {
        delete Name;
        Name = &name;
        NameHashCode = Strings::SuperFastHash(Name->data(),(int)Name->size());
        HashCode = NameHashCode;
        if (Position != NULL) {
            HashCode ^= Position->HashCode;
        }     
    }

    void Identifier::SetPosition(const Point& position) {
        delete Position;
        Position = &position;
        HashCode = NameHashCode;
        if (Position != NULL) {
            HashCode ^= Position->HashCode;
        }     
    }

    void Identifier::SetProperty(Property* prop) {
        PropertyPtr = prop;
    }

    string& Identifier::StatusReport() const {
        string& result = *new string(Name==NULL ? "" : *Name);
        if (Position != NULL) {
            string& tmp = Position->StatusReport();
            result += tmp;
            delete &tmp;
        }
        return result;
    }

    int Identifier::CompareTo(const Identifier& ident) const {
        int c;
        const string* name = ident.Name;
        if (Name != NULL && name != NULL) {
            c = Name->compare(*name);
            if (c != 0) {
                return c;
            }
        }
        const Point* position = ident.Position;
        if (Position != NULL && position != NULL) {
            c = Position->CompareTo(*position);
            if (c != 0) {
                return c;
            }
        }
        return 0;
    }

    string& Identifier::GetIdentifierNumber() {
        return Numbers::ToString(mLastIdentifierNumber++);
    }

    string& Identifier::GetIdentifierValue() {
        string& tmp = GetIdentifierNumber();
        tmp.insert(0,"Identifier");
        return tmp;
    }

    Identifier& Identifier::Generate() {
        return *new Identifier(GetIdentifierValue(),NULL,NULL);
    }

    Identifier& Identifier::Generate(const char* name) {
        string& tmp = GetIdentifierNumber();
        tmp.insert(0,name);
        return *new Identifier(tmp,NULL,NULL);
    }
}
