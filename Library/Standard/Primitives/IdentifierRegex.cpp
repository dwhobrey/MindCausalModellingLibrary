#include "PlatoIncludes.h"
#include "Regexes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Point.h"
#include "PointSpace.h"
#include "PointRegex.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyScopes.h"
#include "Property.h"

namespace Plato {

    IdentifierRegex::IdentifierRegex(const string* regexName, const PointRegex* regexPosition, const string* regexTypeName) {
        Name = regexName;
        Position = regexPosition;
        TypeName = regexTypeName;
        if (Name == NULL) {
            mRegexName = NULL;
            mNameIsWild = true;
            mNameIsRegex = true;
        } else {
            mRegexName = new RegexMatcher(Name);
            mNameIsWild = Name->compare(*Regexes::WildPatternString)==0;
            mNameIsRegex = mNameIsWild || Regexes::IsRegex(*Name);
        }
        if (TypeName == NULL) {
            mRegexTypeName = NULL;
            mTypeNameIsWild = true;
            mTypeNameIsRegex = true;
        } else {
            mRegexTypeName = new RegexMatcher(TypeName);
            mTypeNameIsWild = TypeName->compare(*Regexes::WildPatternString)==0;
            mTypeNameIsRegex = mTypeNameIsWild || Regexes::IsRegex(*TypeName);
        }
    }

    IdentifierRegex::IdentifierRegex(const IdentifierRegex& regexIdentifier) {
        const string* s = regexIdentifier.Name;
        if(s==NULL) {
            Name = NULL;
            mRegexName = NULL;
            mNameIsWild = true;
            mNameIsRegex = true;
        } else {
            Name = new string(*s);
            mRegexName = new RegexMatcher(Name);
            mNameIsWild = regexIdentifier.mNameIsWild;
            mNameIsRegex = regexIdentifier.mNameIsRegex;
        }
        const PointRegex* p = regexIdentifier.Position;
        Position = p==NULL ? NULL : new PointRegex(*p);
        s = regexIdentifier.TypeName;
        if (s == NULL) {
            TypeName = NULL;
            mRegexTypeName = NULL;
            mTypeNameIsWild = true;
            mTypeNameIsRegex = true;
        } else {
            TypeName = new string(*s);
            mRegexTypeName = new RegexMatcher(TypeName);
            mTypeNameIsWild = regexIdentifier.mTypeNameIsWild;
            mTypeNameIsRegex = regexIdentifier.mTypeNameIsRegex;
        }
    }

    IdentifierRegex::~IdentifierRegex() {
        delete Name;
        delete TypeName;
        delete Position;
        delete mRegexName;
        delete mRegexTypeName;
    }

    string& IdentifierRegex::StatusReport() const {
        string& result = *new string(Name==NULL ? "" : *Name);
        if (Position != NULL) {
            result += Position->StatusReport();
        }
        if(TypeName != NULL) {
            result += ":" + *TypeName;
        }
        return result;
    }

    bool IdentifierRegex::Match(const Identifier& ident) const {
        if (mRegexName != NULL) {
            const string* name = ident.Name;
            if (name == NULL) {
                if (!mNameIsWild) {
                    return false;
                }
            } else {
                if(!mRegexName->Matches(*name)) {
                    return false;
                }
            }
        }
        if (Position != NULL) {
            const Point* position = ident.Position;
            if (position == NULL) {
                if (Position->Status!=PointRegexStatus::Wild) {
                    return false;
                }
            } else {
                if(!Position->Match(*position)) {
                    return false;
                }
            }
        }
        if (mRegexTypeName != NULL) {
            Property*p = ident.PropertyPtr;
            if(p==NULL) {
                if (!mTypeNameIsWild) {
                    return false;
                }  
            } else {
                const string& typeName = *(p->GetClassTypeInfo()->ClassName);
                if(!mRegexTypeName->Matches(typeName)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool IdentifierRegex::PatternMatch(const IdentifierRegex& regexIdentifier) const {
        const string* name = regexIdentifier.Name;
        if (mRegexName != NULL) {
            if (name == NULL) {
                if (!mNameIsWild) {
                    return false;
                }
            } else {
                if (mNameIsRegex) {
                    if (regexIdentifier.mNameIsRegex) {
                        if (!mNameIsWild && name->compare(*Regexes::WildPatternString)!=0) {
                            return false;
                        }
                    } else {
                        if(!mRegexName->Matches(*name)) {
                            return false;
                        }
                    }
                } else if (regexIdentifier.mNameIsRegex) {
                    RegexMatcher rm(name);              
                    if(!rm.Matches(*Name)) {
                        return false;
                    }
                } else {
                    if (!Name->compare(*name)==0) {
                        return false;
                    }
                }
            }
        } else if (name != NULL) {
            if (name->compare(*Regexes::WildPatternString)!=0) {
                return false;
            }
        }
        const PointRegex* position = regexIdentifier.Position;
        if (Position != NULL) {
            if (position == NULL) {
                if (Position->Status!=PointRegexStatus::Wild) {
                    return false;
                }
            } else if (!Position->PatternMatch(*position)) {
                return false;
            }
        } else if (position != NULL) {
            if (position->Status!=PointRegexStatus::Wild) {
                return false;
            }
        }
        const string* typeName = regexIdentifier.TypeName;
        if (mRegexTypeName != NULL) {
            if (typeName == NULL) {
                if (!mTypeNameIsWild) {
                    return false;
                }
            } else {
                if (mTypeNameIsRegex) {
                    if (regexIdentifier.mTypeNameIsRegex) {
                        if (!mTypeNameIsWild && typeName->compare(*Regexes::WildPatternString)!=0) {
                            return false;
                        }
                    } else {
                        if(!mRegexTypeName->Matches(*typeName)) {
                            return false;
                        }
                    }
                } else if (regexIdentifier.mTypeNameIsRegex) {
                    RegexMatcher rm(typeName);             
                    if(!rm.Matches(*TypeName)) {
                        return false;
                    }
                } else {
                    if (TypeName->compare(*typeName)!=0) {
                        return false;
                    }
                }
            }
        } else if (typeName != NULL) {
            if (typeName->compare(*Regexes::WildPatternString)!=0) {
                return false;
            }
        }
        return true;
    }
#if 0 // Old code.
    bool Identifier::CompareToRegex(const Identifier& regexIdent) {
        int c;
        string* name = regexIdent.Name;
        if (Name != NULL && name != NULL) {
            if(!Regexes::Matches(Name, *name)) {
                return false;
            }
        }
        Point* position = regexIdent.Position;
        if (Position != NULL && position != NULL) {
            c = Position->CompareTo(*position);
            if (c != 0) {
                return false;
            }
        }
        return true;
    }

    bool Identifier::CompareToRegex(const string& regularExpression) {
        string& tmp = StatusReport();
        bool result = Regexes::Matches(tmp, regularExpression);
        delete tmp;
        return result;
    }
#endif
}
