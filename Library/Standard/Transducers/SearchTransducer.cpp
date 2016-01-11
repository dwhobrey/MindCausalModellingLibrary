#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Property.h"
#include "ConfigurePhases.h"
#include "ResolutionModes.h"
#include "Container.h"
#include "Transducer.h"
#include "Relator.h"
#include "ConnectorRelator.h"
#include "Pad.h"
#include "InputPad.h"
#include "OutputPad.h"

#include "SearchTransducer.h"

namespace Plato {

    const ClassTypeInfo* SearchTransducer::TypeInfo = NULL;

    void SearchTransducer::Initializer() {
        TypeInfo = new ClassTypeInfo("SearchTransducer",240,1,NULL,Transducer::TypeInfo);
    }

    void SearchTransducer::Finalizer() {
        delete TypeInfo;
    }

    SearchTransducer::SearchTransducer(Identifier& name, PropertyScopesEnum scope, ResolutionModesEnum resolutionMode, ConnectorRelator& elementRelator)
        : Transducer(name, scope, resolutionMode, elementRelator) {
    }

    Transducer* SearchTransducer::Factory(ResolutionModesEnum resolutionMode, Container& parent, const Property* requester, 
            Connector& connector, InputPad& toPad, OutputPad& fromPad) {

        // Check if this instance can handle the To and From transduction.
        if (Matches(resolutionMode, parent, connector, toPad, fromPad)) {
            return this;
        }

        // Try searching To path first.
        Transducer* transducer = SearchPathForTransducer(toPad, resolutionMode, parent, requester, connector, toPad, fromPad);
        if (transducer != NULL) {
            return transducer;
        }

        // Finally search From path.
        return SearchPathForTransducer(fromPad, resolutionMode, parent, requester, connector, toPad, fromPad);
    }

    Transducer* SearchTransducer::SearchPathForTransducer(Property& path, ResolutionModesEnum resolutionMode, 
            Container& parent, const Property* requester, Connector& connector, InputPad& toPad, OutputPad& fromPad) {
        Container* c = path.Flags.IsContainer ? (Container*)&path : path.Parent;
        while (c != NULL) {
            vector<Property*>* transducers = c->GetCategory(*Transducer::TypeInfo);
            if (transducers != NULL) {
                vector<Property*>::iterator itr;
                for(itr=transducers->begin();itr!=transducers->end();++itr) {
                    Property* p = (*itr)->GetValue();
                    if (p->Flags.IsTransducer && p->InScope(requester, false)) {
                        Transducer* t = (Transducer*)p;
                        if (t->Matches(resolutionMode, *Parent, connector, toPad, fromPad)) {
                            return t;
                        }
                    }
                }
            }
            c = c->Parent;
        }
        return NULL;
    }
}
