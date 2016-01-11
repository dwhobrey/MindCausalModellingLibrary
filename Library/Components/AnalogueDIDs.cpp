#include "PlatoIncludes.h"
#include "PlatoStandard.h"
#include "AnalogueDIDs.h"

namespace Plato {

    AndInputPad::AndInputPad(Container* creator, Container& parent, Identifier& identity, PropertyScopesEnum scope)
        : InputPad(creator,parent,identity,scope) {
    }

    AndOutputPad::AndOutputPad(Container* creator, Container& parent, Identifier& identity, PropertyScopesEnum scope)
        : OutputPad(creator,parent,identity,scope) {
    }

    AndBundle::AndBundle(Container* creator, Container& parent, Identifier& elementName, 
        PropertyScopesEnum scope, bool isInput) 
        : Bundle(creator,parent,elementName,scope,isInput) {
    }

    Property* AndBundle::PropertyFactory(Identifier& identifier, const Property* requester) {
        Property* p;
        if(mIsInput)
            p = new AndInputPad(this, *this, identifier, PropertyScopes::Public);
        else
            p = new AndOutputPad(this, *this, identifier, PropertyScopes::Public);
        Add(*p);
        return p;
    }

    AndDID::AndDID(Container* creator, Container* parent, Identifier& elementName, PropertyScopesEnum scope) 
        : Disposition(creator,parent,elementName,scope) {
        // TO DO.
    }

    void AndDID::Update() {
        Disposition::Update();
        // TO DO arithmetic multiply each input.
        // normalise etc.
    }

    bool AndDID::AddBundle(Identifier& name, bool isInput) {
        return Add(*new AndBundle(this,*this,name,PropertyScopes::Public,isInput));
    }

    OrDID::OrDID(Container* creator, Container* parent, Identifier& elementName, PropertyScopesEnum scope) 
        : Disposition(creator,parent,elementName,scope) {
        // TO DO.
    }

    void OrDID::Update() {
        Disposition::Update();
        // TO DO sum over inputs.
        // normalise etc.
    }
}
