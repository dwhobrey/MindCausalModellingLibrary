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
#include "InputPad.h"
#include "OutputPad.h"
#include "Connector.h"

namespace Plato {

    InputConnection::InputConnection(OutputPad& outputPad, Connector& connector) {
        SourceOutputPad = &outputPad;
        OriginatingConnector = &connector;
    }

    InputConnection::~InputConnection() {
    }

    const ClassTypeInfo* InputPad::TypeInfo = NULL;

    void InputPad::Initializer() {
        TypeInfo = new ClassTypeInfo("InputPad",302,1,NULL,Pad::TypeInfo);
    }

    void InputPad::Finalizer() {
        delete TypeInfo;
    }

    InputPad::InputPad(Container* creator, Container& parent, Identifier& identity, PropertyScopesEnum scope) 
            : Pad(creator,parent,identity,scope) {
        InputConnections = new vector<InputConnection*>();
        Flags.IsInputPad = true;
    }

    InputPad::~InputPad() {
        vector<InputConnection*>::iterator itr;
        for(itr=InputConnections->begin();itr!=InputConnections->end();++itr) {
            delete *itr;
        }
        delete InputConnections;
    }

    bool InputPad::Add(OutputPad& outputPad, Connector& connector) {
        InputConnections->push_back(new InputConnection(outputPad,connector));
        return true;
    }
}
