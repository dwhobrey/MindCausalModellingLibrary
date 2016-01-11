#include "PlatoIncludes.h"
#include "Strings.h"
#include "Regexes.h"
#include "ClassTypeInfo.h"
#include "Debug.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "Point.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "Property.h"
#include "Arguments.h"
#include "Error.h"
#include "ThreadLocalVariable.h"
#include "ConfigurePhases.h"
#include "ResolutionModes.h"
#include "Container.h"
#include "Filter.h"
#include "Path.h"
#include "PathNode.h"
#include "Model.h"
#include "NameFilter.h"
#include "PathFilter.h"
#include "PointFilter.h"
#include "TypeFilter.h"
#include "TypeNameFilter.h"
#include "SearchFilter.h"

namespace Plato {
#pragma region // Div "/" operator.
    PathNode& operator /(const PathNode& c1, const PathNode& c2) {
        PathNode* n1 = c1.GetWritable();
        PathNode* n2 = c2.GetWritable();
        PathNode* currentTail = n1->Tail();
        currentTail->Next = n2;
        n2->Parent = currentTail;
        return *n1;
    }

    PathNode& operator /(const PathNode& c1, const PathNode* c2) {
        return c1 / *c2;
    }

    PathNode& operator /(const PathNode& c1, const Identifier& identifier) {
        if(identifier.Position!=NULL) {
            return c1 / *new PathNode(identifier);
        }
        const PathNode* c2 = Path::GetStandardNode(identifier.Name);
        // Note: ident will be leaked if on heap and standard.
        return c1 / ((c2 == NULL) ? *new PathNode(identifier) : *c2);
    }

    PathNode& operator /(const PathNode& c1, const Identifier* identifier) {
        return c1 / *identifier; 
    }

    PathNode& operator /(const PathNode& leftPathNode, const string& ident) {
        // Note: ident will be leaked if on heap and standard.
        const PathNode* c2 = Path::GetStandardNode(&ident);
        if(c2!=NULL) {
            return leftPathNode / *c2;
        }
        return leftPathNode / *new PathNode(*new Identifier(ident));
    }

    PathNode& operator /(const PathNode& c1, const IdentifierRegex& identifierRegex) {
        PathNode* n1 = c1.GetWritable();
        PathNode* currentTail = n1->Tail();
        currentTail->Next = new PathNode(identifierRegex, currentTail);
        return *n1;
    }

    PathNode& operator /(const PathNode& leftPathNode, const char* ident) {
        string* str = new string(ident);
        const PathNode* c2 = Path::GetStandardNode(str);
        if(c2!=NULL) {
            delete str;
            return leftPathNode / *c2;
        }
        if(Regexes::IsRegex(*str)) {
            return leftPathNode / *new IdentifierRegex(str);
        }
        return leftPathNode / *new Identifier(*str);
    }

#pragma endregion

#pragma region // Grouping "|" operator.
    PathNode& operator |(const PathNode& c1, const PathNode& c2) {
        PathNode* n2 = c2.GetWritable();
        n2->GroupBoundary = true;
        return c1 / n2;
    }

    PathNode& operator |(const PathNode& leftPathNode, Identifier& identifier) {
        PathNode& c1 = leftPathNode / identifier;
        PathNode* c2 = c1.Tail();
        c2->GroupBoundary = true;
        return c1;
    }

    PathNode& operator |(const PathNode& leftPathNode, IdentifierRegex& identifierRegex) {
        PathNode& c1 = leftPathNode / identifierRegex;
        PathNode* c2 = c1.Tail();
        c2->GroupBoundary = true;
        return c1;
    }

    PathNode& operator |(const PathNode& leftPathNode, const string& ident) {
        return leftPathNode | *new Identifier(ident);
    }

    PathNode& operator |(const PathNode& leftPathNode, const char* ident) {
        // Test for regex ident.
        string* str = new string(ident);
        if(Regexes::IsRegex(*str)) {
            return leftPathNode | *new IdentifierRegex(str);
        }
        return leftPathNode | *new Identifier(*str);
    }

#pragma endregion

#pragma region // Director operator. 
    PathNode& Director(string& routeIdentifier) {
        return *new PathNode(false,PathNodeKinds::Director,*new Identifier(routeIdentifier));
    }
#pragma endregion

#pragma region // Methods for specifying filters in paths via type or path.
    PathNode& Filter(const Path& filterPath, Arguments* arguments) {
        return *new PathNode(filterPath, arguments);
    }

    PathNode& Filter(const ClassTypeInfo& filterType, Arguments* arguments) {
        if (!filterType.IsFilter) {
            Error::Log(false, *Model::Current().ModelErrorContext, *Path::TypeInfo, "Filter", 11, 
                "Filter type must be derived from Filter: %s.", filterType.ClassName->c_str());
        }
        return *new PathNode(filterType, arguments);
    }
#pragma endregion

#pragma region // Methods for specifying standard filters in paths.
    PathNode& FilterName(string& regexName, bool includeWhenMatches) {
        return *new PathNode(*NameFilter::TypeInfo, new NameFilterArguments(regexName,includeWhenMatches));
    }
    PathNode& FilterName(const char* regexName, bool includeWhenMatches) {
        return *new PathNode(*NameFilter::TypeInfo, new NameFilterArguments(*new string(regexName),includeWhenMatches));
    }

    PathNode& FilterPath(Path& path, bool includeWhenMatches) {
        return *new PathNode(*PathFilter::TypeInfo, new PathFilterArguments(path,includeWhenMatches));
    }

    PathNode& FilterPoint(PointRegex& point, bool includeWhenMatches) {
        return *new PathNode(*PointFilter::TypeInfo, new PointFilterArguments(point,includeWhenMatches));
    }

    PathNode& FilterSearch(IdentifierRegex& regexIdentifier, bool searchUp, bool includeWhenMatches) {
        return *new PathNode(*SearchFilter::TypeInfo, new SearchFilterArguments(regexIdentifier,searchUp,includeWhenMatches));    
    }

    PathNode& FilterType(const ClassTypeInfo& comparisonType, bool includeWhenMatches) {
        return *new PathNode(*TypeFilter::TypeInfo, new TypeFilterArguments(comparisonType,includeWhenMatches));
    }
    
    PathNode& FilterType(const string& regexClassName, bool includeWhenMatches) {
        return *new PathNode(*TypeNameFilter::TypeInfo, new TypeNameFilterArguments(regexClassName,includeWhenMatches));
    }
#pragma endregion
}
