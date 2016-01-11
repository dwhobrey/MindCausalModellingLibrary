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
#include "Pad.h"
#include "InputPad.h"
#include "OutputPad.h"
#include "Bundle.h"
#include "Disposition.h"

namespace Plato {

    const ClassTypeInfo* Disposition::TypeInfo = NULL;
    const ClassTypeInfo::HashSet* Disposition::PropertyTypes = NULL;

    void Disposition::Initializer() {
        TypeInfo = new ClassTypeInfo("Disposition",304,1,NULL,Container::TypeInfo);
        PropertyTypes = Container::CreatePropertyTypes(
            Disposition::TypeInfo, Bundle::TypeInfo, OutputPad::TypeInfo, InputPad::TypeInfo,
            NULL);
    }

    void Disposition::Finalizer() {
        delete TypeInfo;
        delete PropertyTypes;
    }

    Disposition::Disposition(Container* creator, Container* parent, Identifier& elementName, PropertyScopesEnum scope) 
        : Container(*PropertyTypes, creator, parent, elementName, scope) {
        IsSuperposed = false;
        IsInputSafe = true;
    }

    Disposition::~Disposition() {
    }

    void Disposition::Update() {
        // Basically, this would allow the DID to modify its state
        // and update its outputs as necessary.

        // For superposed DIDs need to figure out an elegant way
        // of superimposing DIDS in the region.
        // Maybe what this means is that for any common outputs
        // the output value needs to be set to a linear combination of
        // the superimposed DIDs pre superimposed outputs?
        // Also, should superimposed DIDs interact in some other way,
        // e.g., should their state value be influenced by the other
        // DIDs - this may cause evolution problems by squashing signal values.

        // The inputs to the DID would be determined by calling a function,
        // such as GetInputs(), which return an InputPads list - perhaps.
        // We'd want to cache the inputs, although the list could change...
    }
}
