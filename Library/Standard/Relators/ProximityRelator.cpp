#include "PlatoIncludes.h"
#include "Strings.h"
#include "Numbers.h"
#include "ClassTypeInfo.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "ConfigurePhases.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "Property.h"
#include "Container.h"
#include "Relator.h"
#include "ProximityRelator.h"

namespace Plato {

    ProximityRelator::ProximityRelator(vector<IdentifierRegex*>* patterns) {
        IsSymmetric = false;
        Parent = NULL;
        Patterns = patterns;
    }

    ProximityRelator::~ProximityRelator() {
        if(Patterns!=NULL) {
            vector<IdentifierRegex*>::iterator itr;
            for(itr=Patterns->begin();itr!=Patterns->end();++itr) {
                delete *itr;
            }
            delete Patterns;
        }
    }

    int ProximityRelator::GetArity() {
        if (Patterns != NULL) {
            return (int)Patterns->size();
        }
        return 0;
    }

    bool ProximityRelator::ValidRelation(vector<Property*>& elements) {
        return true;
    }

    bool ProximityRelator::Matches(vector<Property*>& elements) {
        if (Patterns == NULL)
            return true;
        int arity = (int)Patterns->size();
        if ((int)(elements.size())!=arity) {
            return false;
        }
        for (int patternIndex = 0; patternIndex < arity; ++patternIndex) {
            Property* element = elements[patternIndex];
            if (element != NULL && !(*Patterns)[patternIndex]->Match(*element->Identity)) {
                return false;
            }
        }
        return true;
    }

    bool ProximityRelator::MatchAndValidRelation(vector<Property*>& elements) {
        return Matches(elements) && ValidRelation(elements);
    }

    int ProximityRelator::BinarySearch(vector<int>& pattern, vector<vector<int>*>& searchVector) {
        vector<vector<int>*>::iterator pVector = searchVector.begin();
        int len = (int)searchVector.size();
        int first = 0;
        int result = 0;
        int half,middle;
        int cmpLen,patLen = (int)pattern.size();
        vector<int> *p;
        while (len > 0) {
            half = len >> 1;
            middle = first + half;
            p = pVector[middle];            
            cmpLen = (int)p->size();
            if(cmpLen>patLen) cmpLen = patLen;
            for(int k=0;k<cmpLen;++k) {
                result = pattern[k] - (*p)[k];
                if(result!=0) break;
            }
            if (result == 0) {
                return middle;
            } else if (result > 0) {
                first = middle;
                ++first;
                len = len - half - 1;
            } else {
                len = half;
            }
        }
        return ~first;
    }

    vector<vector<Property*>*>* ProximityRelator::FindMatches(Container& container) {
        if (Patterns == NULL) {
            return NULL;
        }
        vector<vector<int>*>& tuples = *new vector<vector<int>*>();
        Parent = &container;
        int elementOrder;
        int arity = GetArity();
        vector<vector<int>*>& patternMatches = *new vector<vector<int>*>(arity);
        for (int patternIndex = 0; patternIndex < arity; ++patternIndex) {
            patternMatches[patternIndex] = new vector<int>();
        }
        vector<Property*>& orderToElement = *new vector<Property*>(container.Count());
        vector<Property*>::iterator itr;
        vector<Property*>* properties = container.OrderedProperties; 
        elementOrder = 0;
        for(itr=properties->begin();itr!=properties->end();++itr) {
            Property* element = *itr;
            if (element != NULL) {
                Identifier* ident = element->Identity;
                for (int patternIndex = 0; patternIndex < arity; ++patternIndex) {
                    if ((*Patterns)[patternIndex]->Match(*ident)) {
                        patternMatches[patternIndex]->push_back(elementOrder);
                    }
                }
            }
            orderToElement[elementOrder++] = element;
        }

        // Generate tuples from pattern matches.
        // If IsSymmetric, order of elements in tuple is not significant.
        // Element can only appear once in tuple.
        vector<int>& tuple = *new vector<int>(arity);
        vector<int>& trace = *new vector<int>(arity);
        for (int k = 0; k < arity; k++) {
            tuple[k] = 0;
            trace[k] = 0;
        }
        bool isSymmetric = IsSymmetric;
        int upperIndex = arity - 1;
        int traceIndex = 0;
        int tupleIndex;
        vector<int>& elements = *(patternMatches[traceIndex]);
        for(;;) {
            int elementIndex = trace[traceIndex];
            if (elementIndex >= (int)elements.size()) {
                if (traceIndex == 0) {
                    // Finished finding tuples.
                    break;
                } else {
                    // Backtrack to parent.
                    --traceIndex;
                    ++(trace[traceIndex]);
                    elements = *(patternMatches[traceIndex]);
                }
            } else {
                elementOrder = elements[elementIndex];
                // Quicky check not already in tuple.
                bool notUnique = false;
                for (int j = 0; j < traceIndex; ++j) {
                    if (tuple[j] == elementOrder) {
                        notUnique = true;
                        break;
                    }
                }
                if (notUnique) {
                    // Skip to next element;
                    ++(trace[traceIndex]);
                } else {
                    tuple[traceIndex] = elementOrder;
                    if (traceIndex == upperIndex) {
                        // Add a new tuple.
                        vector<int>& tmp = *new vector<int>(tuple);
                        if (isSymmetric) {
                            sort(tmp.begin(),tmp.end());
                        }
                        tupleIndex = BinarySearch(tmp,tuples);
                        if (tupleIndex<0) {
                            tuples.insert(tuples.begin()+ ~tupleIndex,&tmp);
                        } else {
                            delete &tmp;
                        }
                        // Move to next element;
                        ++(trace[traceIndex]);
                    } else {
                        // Move to next dimension.
                        ++traceIndex;
                        elements = *(patternMatches[traceIndex]);
                        trace[traceIndex] = 0;
                    }
                }
            }
        }
        delete &tuple;
        delete &trace;

        // Use orderToElement to produce a List of Property[]'s.
        // Call Valid to check each tuple satisfies relator.
        vector<vector<Property*>*>& elementTuples = *new vector<vector<Property*>*>(tuples.size());
        vector<vector<int>*>::iterator tup;
        for(tup=tuples.begin();tup!=tuples.end();++tup) {
            tuple = *(*tup);
            vector<Property*>& tmp = *new vector<Property*>(arity);
            for (int j = 0; j < arity; ++j) {
                tmp[j] = orderToElement[tuple[j]];
            }
            if (ValidRelation(tmp)) {
                elementTuples.push_back(&tmp);
            } else {
                delete &tmp;
            }
            delete &tuple;
        }
        // Tidy up.
        delete &orderToElement;
        delete &tuples;
        vector<vector<int>*>::iterator pat;
        for(pat=patternMatches.begin();pat!=patternMatches.end();++pat) {
            delete *pat;
        }
        delete &patternMatches;

        return &elementTuples;
    }

    PointRelator::PointRelator(Identifier& elementIdent, const Point& elementPoint) 
        : ProximityRelator(NULL) {
        ElementIdent = &elementIdent;
        ElementPoint = &elementPoint;
    }
}
