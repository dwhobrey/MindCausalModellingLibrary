#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "ConfigurePhases.h"
#include "Property.h"
#include "Container.h"
#include "ResolutionModes.h"
#include "Pad.h"
#include "OutputPad.h"

namespace Plato {

    const ClassTypeInfo* OutputPad::TypeInfo = NULL;

    int OutputPad::CurrentBuffer = 0;

    void OutputPad::Initializer() {
        TypeInfo = new ClassTypeInfo("OutputPad",301,1,NULL,Pad::TypeInfo);
    }

    void OutputPad::Finalizer() {
        delete TypeInfo;
    }

    OutputPad::OutputPad(Container* creator, Container& parent, Identifier& identity, PropertyScopesEnum scope) 
            : Pad(creator,parent,identity,scope) {
        Flags.IsOutputPad = true;
    }

    OutputPad::OutputPad(Container& parent, Identifier& identity, PropertyScopesEnum scope) 
            : Pad(&parent,parent,identity,scope) {
        Flags.IsOutputPad = true;
    }

    OutputPad::OutputPad(Container& parent, const char* identity, PropertyScopesEnum scope) 
            : Pad(&parent,parent,*new Identifier(identity),scope) {
        Flags.IsOutputPad = true;
    }

    OutputPad::~OutputPad() {
    }
}
