#include "PlatoIncludes.h"
#include "Regexes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "InitializerCatalogue.h"
#include "Archiver.h"
#include "EnvironmentVariables.h"
#include "CommandLine.h"
#include "Configuration.h"
#include "Identifier.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "Property.h"
#include "Error.h"
#include "ErrorContext.h"
#include "ThreadLocalVariable.h"
#include "ConfigurePhases.h"
#include "ResolutionModes.h"
#include "Container.h"
#include "Path.h"
#include "Model.h"
#include "Diagnostics.h"
#include "Trace.h"
#include "Debug.h"
#include "Connector.h"
#include "Distributor.h"
#include "Converter.h"
#include "Restrictor.h"

namespace Plato {

    const ClassTypeInfo* Model::TypeInfo = NULL;
    const ClassTypeInfo::HashSet* Model::PropertyTypes = NULL;
    ThreadLocalVariable::Key Model::mThreadContextKey = NULL;
    int Model::mNumberOfModels = 0;

    void Model::Initializer() {
        TypeInfo = new ClassTypeInfo("Model",3,1,NULL,Container::TypeInfo);
        PropertyTypes = Container::CreatePropertyTypes(Property::TypeInfo,NULL);
        mThreadContextKey = ThreadLocalVariable::GetKey();
    }

    void Model::Finalizer() {
        delete TypeInfo;
        delete PropertyTypes;
    }

    const ClassTypeInfo::HashSet& Model::InitLibrary() {
        if(mNumberOfModels++==0) {
            InitializerCatalogue::Initialize();
        }
        return *PropertyTypes;
    }

    Model::Model(const char* name, char* arguments[]) 
        : Container(InitLibrary(), NULL, NULL, *new Identifier(name,NULL,NULL), PropertyScopes::Public) {
        StopEvent = false;
        IsListeningToContainers = false;
        EpochTime = 0;
        ConfigurePhase = ConfigurePhases::Unconfigured;
        ResolutionMode = ResolutionModes::None;
        CurrentConnector = NULL;
        CurrentConnectorStack = new ConnectorStack();
        CurrentContainerStack = new ContainerStack();
        ModelErrorContext = new ErrorContext(ErrorContext::DefaultStackSize);
        ThreadLocalVariable::SetVariableValue(mThreadContextKey,(ThreadLocalVariable::ValuePtr)this);
        ModelConfiguration = new Configuration(name, arguments);

        if(mNumberOfModels==1) {
            signal(SIGINT,SignalHandler);
            if (ModelConfiguration->RunDiagnostics) {
                Diagnostics::ConductTests();
            }
        }

        if (ModelErrorContext->Count() > 0) {
            string& message = ModelErrorContext->StatusReport();
            Trace::WriteLine(1, "Model.Model: there were startup errors:%s", message.c_str());
            delete &message;
        }
    }

    Model::~Model() {
        try {
            if (ModelErrorContext->Count() > 0) {
                string& tmp1 = StatusReport();
                string& tmp2 = ModelErrorContext->StatusReport();
                Trace::WriteLine(1, "Model.Destructor: %s, reported errors:\n%s", tmp1.c_str(),tmp2.c_str());
                delete &tmp2;
                delete &tmp1;
            }

            int exitCode = ModelErrorContext->ExitCode;
            if (exitCode != 0) {
                string& tmp1 = StatusReport();
                Trace::WriteLine(1, "Model.Destructor: %s, exit code=%d.\n",tmp1.c_str(),exitCode);
                delete &tmp1;
            }
            delete CurrentContainerStack;
            delete CurrentConnectorStack;
            delete ModelErrorContext;
            delete ModelConfiguration;
            if(--mNumberOfModels==0) {
                signal(SIGINT,SIG_DFL);
                InitializerCatalogue::Finalize();
            }
        } catch(...) {
            Trace::WriteLine(0, "Model.Destructor: exception occurred:\n");
        }
    }

    void Model::SignalHandler(int sig) {
        if(sig==SIGINT) {
            signal(SIGINT,SignalHandler);
            // Interrupt model.
            Model::Current().StopEvent;
        }
    }

#pragma region // General methods.
    void Model::PushCurrentConnector(Connector& connector) {
        CurrentConnectorStack->push(&connector);
        CurrentConnector = &connector;
    }

    void Model::PopCurrentConnector() {
        if (CurrentConnectorStack->empty()) {
            Debug::WriteLine(1, "Model.PopCurrentConnector, stack is empty.");
            CurrentConnector = NULL;
            return;
        }
        CurrentConnectorStack->pop();
        if (CurrentConnectorStack->empty()) {
            CurrentConnector = NULL;
            return;
        }
        CurrentConnector = CurrentConnectorStack->top();
    }

    void* Model::GetPropertyValue(string& name) {
        Identifier ident(* new string(name));
        Property* p = Container::GetProperty(ident, (const Property*)NULL, PropertyModes::Traversing);
        if (p != NULL) {
            return p->GetValue();
        }   
        return NULL;
    }

    string& Model::StatusReport() {
        string& tmp = Identity->StatusReport();
        string* result = new string();
        *result += "Model(Identity=" + tmp + ")";
        delete &tmp;
        return *result;
    }
#pragma endregion

#pragma region // Model control methods.
    int Model::Save(string* archivePath) {
        if (archivePath != NULL) {
            ModelConfiguration->ArchivePath = archivePath;
        }
        int result = Archiver::Serialize(*(ModelConfiguration->ArchivePath), ModelConfiguration->BinarySerialize, (void*)this);
        if (result != 0) {
            return Error::Log(false, *ModelErrorContext, *GetClassTypeInfo(), "Save", 8, "Serializing model returned non-zero result: %d.", result);
        }
        return 0;
    }

    Model* Model::Load(string& archivePath) {
        try {
            // Try to deserialise model.
            void* obj = Archiver::Deserialize(archivePath);
            return (Model*)obj;              
        } catch (...) {
            Trace::WriteLine(1, "Model.Load exception.");
        }
        return NULL;
    }

    int Model::Run(long numberOfEpochs) {
        try {
            if (numberOfEpochs < 0) {
                while (!StopEvent)  {
                    ++EpochTime;
                    Update();
                }
            } else if (numberOfEpochs > 0) {
                while (numberOfEpochs-- > 0 && !StopEvent) {
                    ++EpochTime;
                    Update();
                }
            }
        } catch (...) {
            return Error::Log(false, *ModelErrorContext, *GetClassTypeInfo(), "Run", 11, "Exception.");
        }
        return 0;
    }
#pragma endregion

#pragma region // Model emulation - update methods.
    void Model::Update() {
        Update(*OrderedProperties);
    }

    void Model::Update(vector<Property*>& properties) {
        vector<Property*>::iterator itr;
        for(itr=properties.begin();itr!=properties.end();++itr) {
            Property* p = *itr;
            if (p->Flags.IsContainer) {
                Container* c = (Container*)p;
                if(c->DoEmulate) {
                    c->Update();
                }
            }
        }
    }
#pragma endregion

#pragma region // Model construction methods.
    int Model::Configure() {
        if (ModelErrorContext->Count() > 0) return 1;

        IsListeningToContainers = true;

        ResolutionMode = ResolutionModes::Now;
        ConfigurePhase = ConfigurePhases::DistributorsNow;
        ApplyDistributors(*this);
        ConfigurePhase = ConfigurePhases::ConnectorsNow;
        ApplyConnectors(*this);
        ConfigurePhase = ConfigurePhases::ConvertersNow;
        ApplyConverters(*this);

        ResolutionMode = ResolutionModes::Final;
        ConfigurePhase = ConfigurePhases::ConnectorsFinal;
        ApplyConnectors(*this);
        ConfigurePhase = ConfigurePhases::ConvertersFinal;
        ApplyConverters(*this);

        IsListeningToContainers = false;

        return (ModelErrorContext->Count() > 0) ? 1 : 0;
    }

    void Model::ApplyDistributors(Container& container) {
        // Note, this method may add to a container in the recursion stack
        // so looping using int index's is used since this works when there are additions.

        // Descend to sub elements first - in declaration order.
        vector<Property*>::size_type index = 0;
        vector<Property*> * properties = container.OrderedProperties;
        while(index<properties->size()) {
            Property* p = (*properties)[index++]->GetValue();
            if(p->Flags.IsContainer) {
                Container* e = (Container*)p;
                if (e->IsComposite) {
                    ApplyDistributors(*e);
                }
            }
        }

        // Now apply this element's distributors, if any.
        properties = container.GetCategory(*Distributor::TypeInfo);
        if (properties != NULL) {
            index = 0;
            while(index<properties->size()) {
                Distributor* d = (Distributor*)((*properties)[index++]->GetValue());
                if (d != NULL) {
                    if (container.IsUnrestricted || Restrictor::IsAllowed(*d,ResolutionMode,container)) {
                        d->Apply(ResolutionMode, container);
                    }
                }
            }
        }
        container.ConfigurePhase = ConfigurePhase;
    }

    void Model::ApplyConnectors(Container& container) {
        // Descend to sub elements first - in declaration order.
        vector<Property*>::size_type index = 0;
        vector<Property*> * properties = container.OrderedProperties;
        while(index<properties->size()) {
            Property* p = (*properties)[index++]->GetValue();
            if(p->Flags.IsContainer) {
                Container* e = (Container*)p;
                if (e->IsComposite) {
                    ApplyConnectors(*e);
                }
            }
        }

        // Now apply this element's connectors, if any.
        properties = container.GetCategory(*Connector::TypeInfo);
        if (properties != NULL) {
            index = 0;
            while(index<properties->size()) {
                Connector* c = (Connector*)((*properties)[index++]->GetValue());
                if(c != NULL) {
                    if (container.IsUnrestricted || Restrictor::IsAllowed(*c, ResolutionMode, container)) {
                        c->Apply(ResolutionMode, container);
                    }
                }
            }
        }
        container.ConfigurePhase = ConfigurePhase;
    }

    void Model::ApplyConverters(Container& container) {
        // Descend to sub elements first - in declaration order.
        vector<Property*>::size_type index = 0;
        vector<Property*> * properties = container.OrderedProperties;
        while(index<properties->size()) {
            Property* p = (*properties)[index++]->GetValue();
            if(p->Flags.IsContainer) {
                Container* e = (Container*)p;
                if (e->IsComposite) {
                    ApplyConverters(*e);
                }
            }
        }

        // Now apply this element's converters, if any.
        properties = container.GetCategory(*Converter::TypeInfo);
        if (properties != NULL) {
            index = 0;
            while(index<properties->size()) {
                Converter* c = (Converter*)((*properties)[index++]->GetValue());
                if (c != NULL) {
                    if (container.IsUnrestricted || Restrictor::IsAllowed(*c, ResolutionMode, container)) {
                        c->Apply(ResolutionMode, container);
                    }
                }
            }
        }
        container.ConfigurePhase = ConfigurePhase;
    }

    void Model::ApplyDistributorsAndConnectors(Container& container) {
        vector<Property*>::size_type index = 0;
        vector<Property*> * properties = container.OrderedProperties;
        while(index<properties->size()) {
            Property* p = (*properties)[index++]->GetValue();
            switch(p->Flags.ClassKind) {
                case ClassKinds::Distributor: {
                    Distributor* d = (Distributor*)p;
                    if (container.IsUnrestricted || Restrictor::IsAllowed(*d, ResolutionMode, container)) {
                        d->Apply(ResolutionMode, container);
                    }
                }
                break;
                case ClassKinds::Connector: {
                    Connector* c = (Connector*)p;
                    if (container.IsUnrestricted || Restrictor::IsAllowed(*c, ResolutionMode, container)) {
                        c->Apply(ResolutionMode, container);
                    }
                }
                break;
                default:
                    if(p->Flags.IsContainer) {
                        Container* element = (Container*)p;
                        if (element->IsComposite) {
                            ApplyDistributorsAndConnectors(*element);
                        }
                    }
            }
        }
        container.ConfigurePhase = ConfigurePhase;
    }

    void Model::ApplyAll(Container& container) {
        vector<Property*>::size_type index = 0;
        vector<Property*> * properties = container.OrderedProperties;
        while(index<properties->size()) {
            Property* p = (*properties)[index++]->GetValue();
            switch(p->Flags.ClassKind) {
                case ClassKinds::Distributor: {
                        Distributor* d = (Distributor*)p;
                        if (container.IsUnrestricted || Restrictor::IsAllowed(*d, ResolutionMode, container)) {
                            d->Apply(ResolutionMode, container);
                        }
                    }
                    break;
                case ClassKinds::Connector: {
                        Connector* c = (Connector*)p;
                        if (container.IsUnrestricted || Restrictor::IsAllowed(*c, ResolutionMode, container)) {
                            c->Apply(ResolutionMode, container);
                        }
                    }
                    break;
                case ClassKinds::Converter: {
                        Converter* c = (Converter*)p;
                        if (container.IsUnrestricted || Restrictor::IsAllowed(*c, ResolutionMode, container)) {
                            c->Apply(ResolutionMode, container);
                        }
                    }
                    break;
                default:
                    if(p->Flags.IsContainer) {
                        Container* element = (Container*)p;
                        if (element->IsComposite) {
                            ApplyAll(*element);
                        }
                    }
            }
        }
        container.ConfigurePhase = ConfigurePhase;
    }

    void Model::AddedProperty(Container& container, Property& prop) {
        Property* p = prop.GetValue();
        ConfigurePhasesEnum parentPhase;
        ResolutionModesEnum tmpResolutionMode;
        Distributor* d;
        switch(ConfigurePhase) {
            case ConfigurePhases::DistributorsNow:
                // Apply any new distributors the property may contain.
                // First check if new property is the root of a sub-tree added before current position.
                // Note, for this phase, additions to current container will be to the end
                // and therefore after current position.
                // A Container configured before the current position will have a CP value equal to mCP.
                if (container.ConfigurePhase >= ConfigurePhase) {
                    if(p->Flags.ClassKind==ClassKinds::Distributor) {
                        d = (Distributor*)p;
                        if (container.IsUnrestricted || Restrictor::IsAllowed(*d, ResolutionMode, container)) {
                            d->Apply(ResolutionMode, container);
                        }
                    } else if(p->Flags.IsContainer) {
                        Container* element = (Container*)p;
                        if (element->IsComposite) {
                            ApplyDistributors(*element);
                        }
                    }
                }
                break;

            case ConfigurePhases::ConnectorsNow:
            case ConfigurePhases::ConnectorsFinal:
                parentPhase = container.ConfigurePhase;
                // Ignore new property if it is not the root of a sub-tree.
                if (parentPhase == ConfigurePhases::Unconfigured) {
                    return;
                }
                // Check if a distributor was added, apply it now if it was.
                if(p->Flags.ClassKind==ClassKinds::Distributor) {
                    d = (Distributor*)p;
                    if (container.IsUnrestricted || Restrictor::IsAllowed(*d, ResolutionMode, container)) {
                        d->Apply(ResolutionMode, container);
                    }
                } else {
                    // If addition is before current position:
                    //   Do nows, and finals if in final mode.
                    // else Do nows if in final mode (or Dynamic mode?).
                    tmpResolutionMode = ResolutionMode;
                    if (parentPhase < ConfigurePhase) {
                        // Addition was after current position.
                        if (ResolutionMode == ResolutionModes::Now) {
                            return;
                        }
                        ResolutionMode = ResolutionModes::Now;
                    }
                    if(p->Flags.ClassKind==ClassKinds::Connector) {
                        Connector* c = (Connector*)p;                        
                        if (container.IsUnrestricted || Restrictor::IsAllowed(*c, ResolutionMode, container)) {
                            c->Apply(ResolutionMode, container);
                        }
                    } else if(p->Flags.IsContainer) {
                        Container* element = (Container*)p;
                        if (element->IsComposite) {
                            ApplyDistributorsAndConnectors(*element);
                        }
                    }
                    ResolutionMode = tmpResolutionMode;
                }
                break;

            case ConfigurePhases::ConvertersNow:
            case ConfigurePhases::ConvertersFinal:
                parentPhase = container.ConfigurePhase;
                // Ignore new property if it is not the root of a sub-tree.
                if (parentPhase == ConfigurePhases::Unconfigured) {
                    return;
                }
                // Check if a distributor was added, apply it now if it was.
                if(p->Flags.ClassKind==ClassKinds::Distributor) {
                    d = (Distributor*)p;
                    if (container.IsUnrestricted || Restrictor::IsAllowed(*d, ResolutionMode, container)) {
                        d->Apply(ResolutionMode, container);
                    }
                } else {
                    // If addition is before current position:
                    //   Do nows, and finals if in final mode.
                    // else Do nows if in final mode (or Dynamic mode?).
                    tmpResolutionMode = ResolutionMode;
                    if (parentPhase < ConfigurePhase) {
                        // Addition was after current position.
                        if (ResolutionMode == ResolutionModes::Now) {
                            return;
                        }
                        ResolutionMode = ResolutionModes::Now;
                    }
                    if(p->Flags.ClassKind==ClassKinds::Connector) {
                        Connector* c = (Connector*)p;                        
                        if (container.IsUnrestricted || Restrictor::IsAllowed(*c, ResolutionMode, container)) {
                            c->Apply(ResolutionMode, container);
                        }
                    } else if(p->Flags.ClassKind==ClassKinds::Converter) {
                        Converter* v = (Converter*)p;                            
                        if (container.IsUnrestricted || Restrictor::IsAllowed(*v, ResolutionMode, container)) {
                            v->Apply(ResolutionMode, container);
                        }
                    } else if(p->Flags.IsContainer) {
                        Container* element = (Container*)p; 
                        if (element->IsComposite) {
                            ApplyAll(*element);
                        }
                    }
                    ResolutionMode = tmpResolutionMode;
                }
                break;
        }
    }
#pragma endregion

#pragma region // Deprecated methods.
    Container* Model::AddViaPath(Property* prop, const string& path) {
        Container* c = &Current();
        Property* result = c;
        const char *p = path.data(); int len = (int)path.size();
        const char *q = p, *r = p + len, *s;
        while(q!=r) {
            s = q;
            while(q!=r) {
                switch(*q) {
                    case Path::DirectorySeparator:
                    case Path::GroupSeparator:
                        goto IdentEnd;
                    default: ++q;
                }
            }
            IdentEnd:
            len = (int)(q - s);
            if(len==0) {
                if(q==r) break;
                ++q;
            } else {
                Identifier ident(* new string(s,len));
                result = c->GetProperty(ident, NULL, PropertyModes::All);
                if (q==r || result == NULL) {
                    break;
                }
                Property* v = result->GetValue();
                if(!v->Flags.IsContainer) {
                    return NULL;
                }
                c = (Container*)v;
            }
        }        
        if (result != NULL && prop != NULL) {
            Property* v = result->GetValue();
            if(v->Flags.IsContainer) {
                c = (Container*)v;
                c->Add(*prop);
            }
        }
        return c;
    }

    Property* Model::GetViaPath(const string& path) {
        Container* c = &Current();
        Property* result = c;
        const char *p = path.data(); int len = (int)path.size();
        const char *q = p, *r = p + len, *s;
        while(q!=r) {
            s = q;
            while(q!=r) {
                switch(*q) {
                    case Path::DirectorySeparator:
                    case Path::GroupSeparator:
                        goto IdentEnd;
                    default: ++q;
                }
            }
            IdentEnd:
            len = (int)(q - s);
            if(len==0) {
                if(q==r) break;
                ++q;
            } else {
                Identifier ident(* new string(s,len));
                result = c->GetProperty(ident, NULL, PropertyModes::Traversing);
                if (q==r || result == NULL) {
                    break;
                }
                Property* v = result->GetValue();
                if(!v->Flags.IsContainer) {
                    return NULL;
                }
                c = (Container*)v;
            }
        }
        return result;
    }

    Container* Model::GetPropertyParent(const string& path, string*& propertyName) {
        propertyName = NULL;
        if (path.empty()) {
            return NULL;
        }
        string* containerPath = NULL;
        propertyName = Path::GetPropertyNameAndContainerPath(path, containerPath);
        Property* p = GetViaPath(*containerPath);
        if (p == NULL) {
            return NULL;
        }
        p = p->GetValue();
        return p->Flags.IsContainer ? (Container*)p : NULL;
    }

    Property* Model::Remove(const string& path) {
        string* propertyName = NULL;
        Container* c = GetPropertyParent(path, propertyName);
        if(c==NULL||propertyName==NULL) {
            delete propertyName;
            return NULL;
        }
        Identifier ident(*propertyName);
        return c->Remove(ident);
    }    
#pragma endregion
}
