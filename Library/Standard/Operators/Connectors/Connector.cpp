#include "PlatoIncludes.h"
#include "Numbers.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "PropertyScopes.h"
#include "PropertyModes.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "Property.h"
#include "ConfigurePhases.h"
#include "ResolutionModes.h"
#include "Container.h"
#include "Path.h"
#include "PathNode.h"
#include "Connector.h"
#include "Translator.h"
#include "Transducer.h"
#include "PathEnumerator.h"
#include "Pad.h"
#include "InputPad.h"
#include "OutputPad.h"
#include "GroupContainer.h"
#include "GroupTreeEnumerator.h"
#include "ThreadLocalVariable.h"
#include "Model.h"
#include "Error.h"
#include "Debug.h"
#include "Arguments.h"
#include "Filter.h"
#include "GroupFilter.h"
#include "PathNodeEnumerator.h"

namespace Plato {

    ConnectorTerminus::ConnectorTerminus(ResolutionModesEnum resolutionMode, Path& terminusPath) {
        ResolutionMode = resolutionMode;
        TerminusPath = &terminusPath;
    }
    string& ConnectorTerminus::StatusReport() {
        return TerminusPath->StatusReport();
    }

    const ClassTypeInfo* Connector::TypeInfo = NULL;

    void Connector::Initializer() {
        TypeInfo = new ClassTypeInfo("Connector",200,1,NULL,Property::TypeInfo);
    }

    void Connector::Finalizer() {
        delete TypeInfo;
    }

    Connector::Connector(Container* creator, Container* parent, Identifier& elementName, PropertyScopesEnum scope, 
            ConnectorTerminus& to, ConnectorTerminus& from, 
            ConnectorPatternsEnum pattern, Translator* translator) 
            : Property(creator, parent, elementName, scope) {
        Flags.ClassKind = ClassKinds::Connector;
        mTo = &to;
        mFrom = &from;
        mConnectorPattern = pattern;
        mTranslator = translator;
        mTransducer = NULL;
        mApplicationContainer = NULL;
        mResolutionMode = ResolutionModes::Now;
        mToEnumerator = NULL;
        mFromEnumerator = NULL;
        mIsTo = true;
        mToLock = false;
        mFromLock = false;
    }

    Connector::Connector(ConnectorTerminus& to, ConnectorTerminus& from, ConnectorPatternsEnum pattern,
            Translator* translator) 
            : Property(NULL, NULL, Identifier::Generate("Connector"), PropertyScopes::Private) {
        Flags.ClassKind = ClassKinds::Connector;
        mTo = &to;
        mFrom = &from;
        mConnectorPattern = pattern;
        mTranslator = translator;
        mTransducer = NULL;
        mApplicationContainer = NULL;
        mResolutionMode = ResolutionModes::Now;
        mToEnumerator = NULL;
        mFromEnumerator = NULL;
        mIsTo = true;
        mToLock = false;
        mFromLock = false;
    }

    Connector::Connector(Path& to, Path& from, ConnectorPatternsEnum pattern, Translator* translator)
        : Property(NULL, NULL, Identifier::Generate("Connector"), PropertyScopes::Private) {
        Flags.ClassKind = ClassKinds::Connector;
        mTo = new ConnectorTerminus(ResolutionModes::Now, to);
        mFrom = new ConnectorTerminus(ResolutionModes::Now, from);
        mConnectorPattern = pattern;
        mTranslator = translator;
        mTransducer = NULL;
        mApplicationContainer = NULL;
        mResolutionMode = ResolutionModes::Now;
        mToEnumerator = NULL;
        mFromEnumerator = NULL;
        mIsTo = true;
        mToLock = false;
        mFromLock = false;  
    }

    string& Connector::StatusReport() {
        string& sTo = mTo->StatusReport();
        string& sFrom = mFrom->StatusReport();
        string& sTrans = (mTranslator==NULL) ? *new string("NULL") : *new string(*(mTranslator->GetClassTypeInfo()->ClassName));
        string& sPath = GetPath().StatusReport();
        string& sPattern = Numbers::ToString((int)mConnectorPattern);
        string& result = *new string();
        result += "Connector(" + sPath + ", To=" +  sTo + ", From=" + sFrom; 
        result += ", Pattern=" + sPattern + ", Translator=" + sTrans + ")";
        delete &sTo;
        delete &sFrom;
        delete &sTrans;
        delete &sPath;
        delete &sPattern;
        return result;
    }

    bool Connector::NeedsResolving() {
        // This could check whether the connection operation has been performed
        // on either terminus.
        return (mTo->ResolutionMode & mResolutionMode) != 0
            || (mFrom->ResolutionMode & mResolutionMode) != 0;
    }

    bool Connector::BuildGroupTrees() {
        // Set up special width-first descent and group handling path enumerators.
        Property* requester = mApplicationContainer; // Set requester to application container value.
        mToEnumerator = new PathEnumerator(*mTo->TerminusPath, mApplicationContainer, requester,
            PropertyModes::AllInput, true, this);
        mFromEnumerator = new PathEnumerator(*mFrom->TerminusPath, mApplicationContainer, requester,
            PropertyModes::AllOutput, true, this);
        mToLock = false;
        mFromLock = false;

        // Make sure group trees have been fully built by exhausting the enumerators;
        bool wasError = false;

        mIsTo = true;
        while (InvokeEnumerator(true, wasError) && !wasError) ;

        mIsTo = false;
        while (InvokeEnumerator(false, wasError) && !wasError) ;

        return false;
    }

    bool Connector::ApplyTranslators() {

        // Set up group tree enumerators over terminal groups.
        GroupContainer* toTree = mToEnumerator->PathGroupTree;
        GroupContainer* fromTree = mFromEnumerator->PathGroupTree;
        GroupTreeEnumerator* toTreeEnumerator = new GroupTreeEnumerator(*toTree, true);
        GroupTreeEnumerator* fromTreeEnumerator = new GroupTreeEnumerator(*fromTree, true);

        GroupContainer* currentToGroup;
        GroupContainer* currentFromGroup;
        Property* p;

        // Apply translator to pairs of terminal To and From groups.
        if (mTranslator != NULL) {
            for(;;) {
                p = toTreeEnumerator->Next();
                if(p==NULL) break;
                if(!p->Flags.IsGroupContainer) break;
                currentToGroup = (GroupContainer*)p;
                p = fromTreeEnumerator->Next();
                if(p==NULL) break;
                if(!p->Flags.IsGroupContainer) break;
                currentFromGroup = (GroupContainer*)p;
                mTranslator->OrderGroup(*this, *currentToGroup, *currentFromGroup);
            }
        }
        return false;
    }

    bool Connector::AddConnection(InputPad& toPad, OutputPad& fromPad) {
        // Check for transducer.
        Property* requester = mApplicationContainer; // Set requester to application container value.
        Transducer* t = mTransducer == NULL ? NULL : mTransducer->Factory(mResolutionMode, *mApplicationContainer, requester, *this, toPad, fromPad);
        if (t != NULL) {
            t->Apply(mResolutionMode, *mApplicationContainer, *this, toPad, fromPad);
        } else {
            // Connect.
            toPad.Add(fromPad, *this);
        }
        return false;
    }

    bool Connector::ConnectGroupsOneToOne(GroupContainer& toGroup, GroupContainer& fromGroup) {

        vector<Property*> * toProperties = toGroup.OrderedProperties;
        vector<Property*>::iterator toItr = toProperties->begin();
        vector<Property*>::iterator toEnd = toProperties->end();

        vector<Property*> * fromProperties = fromGroup.OrderedProperties;
        vector<Property*>::iterator fromItr = fromProperties->begin();
        vector<Property*>::iterator fromEnd = fromProperties->end();

        Property* currentToProperty;
        Property* currentFromProperty;

        while (toItr!=toEnd && fromItr!=fromEnd) {
            currentToProperty = (*toItr++)->GetValue();
            currentFromProperty = (*fromItr++)->GetValue();
            // Contractually, by this stage: To should be a property derived from InputPad,
            // and From should be a property derived from OutputPad.
            if(currentToProperty->Flags.IsInputPad && currentFromProperty->Flags.IsOutputPad) {
                if (AddConnection(*((InputPad*)currentToProperty), *((OutputPad*)currentFromProperty))) {
                    return true;
                }
            } else {
                Error::Log(false, *Model::Current().ModelErrorContext, *GetClassTypeInfo(), "ConnectGroupsOneToOne", 1, 
                    "Path terminus not derived from Pad for connector: %s, To=%s, From={2}.", 
                    StatusReport().c_str(), currentToProperty->GetClassTypeInfo()->ClassName->c_str(), 
                    currentFromProperty->GetClassTypeInfo()->ClassName->c_str());
            }
        }
        return false;
    }

    bool Connector::ConnectGroupsFanIn(GroupContainer& toGroup, GroupContainer& fromGroup) {

        vector<Property*> * toProperties = toGroup.OrderedProperties;
        vector<Property*>::iterator toItr = toProperties->begin();
        vector<Property*>::iterator toEnd = toProperties->end();

        vector<Property*> * fromProperties = fromGroup.OrderedProperties;
        vector<Property*>::iterator fromItr = fromProperties->begin();
        vector<Property*>::iterator fromEnd = fromProperties->end();

        Property* currentToProperty;
        Property* currentFromProperty;
            
        while (toItr!=toEnd) {
            currentToProperty = (*toItr++)->GetValue();
            while (fromItr!=fromEnd) {
                currentFromProperty = (*fromItr++)->GetValue();
                // Contractually, by this stage: To should be a property derived from InputPad,
                // and From should be a property derived from OutputPad.
                if(currentToProperty->Flags.IsInputPad && currentFromProperty->Flags.IsOutputPad) {
                    if (AddConnection(*(InputPad*)currentToProperty, *(OutputPad*)currentFromProperty)) {
                        return true;
                    }
                } else {
                    Error::Log(false, *Model::Current().ModelErrorContext, *GetClassTypeInfo(), "ConnectGroupsFanIn", 1, 
                        "Path terminus not derived from Pad for connector: %s, To=%s, From={2}.", 
                        StatusReport().c_str(), currentToProperty->GetClassTypeInfo()->ClassName->c_str(), 
                        currentFromProperty->GetClassTypeInfo()->ClassName->c_str());
                }
            }
            fromItr = fromProperties->begin();
        }
        return false;
    }

    bool Connector::ConnectGroupsFanOut(GroupContainer& toGroup, GroupContainer& fromGroup) {
        vector<Property*> * toProperties = toGroup.OrderedProperties;
        vector<Property*>::iterator toItr = toProperties->begin();
        vector<Property*>::iterator toEnd = toProperties->end();

        vector<Property*> * fromProperties = fromGroup.OrderedProperties;
        vector<Property*>::iterator fromItr = fromProperties->begin();
        vector<Property*>::iterator fromEnd = fromProperties->end();

        Property* currentToProperty;
        Property* currentFromProperty;

        while (fromItr!=fromEnd) {
            currentFromProperty = (*fromItr++)->GetValue();
            while (toItr!=toEnd) {
                currentToProperty = (*toItr++)->GetValue();
                // Contractually, by this stage: To should be a property derived from InputPad,
                // and From should be a property derived from OutputPad.
                if(currentToProperty->Flags.IsInputPad && currentFromProperty->Flags.IsOutputPad) {
                    if (AddConnection(*(InputPad*)currentToProperty, *(OutputPad*)currentFromProperty)) {
                        return true;
                    }
                } else {
                    Error::Log(false, *Model::Current().ModelErrorContext, *GetClassTypeInfo(), "ConnectGroupsFanOut", 1, 
                        "Path terminus not derived from Pad for connector: %s, To=%s, From={2}.", 
                        StatusReport().c_str(), currentToProperty->GetClassTypeInfo()->ClassName->c_str(), 
                        currentFromProperty->GetClassTypeInfo()->ClassName->c_str());
                }
            }
            toItr = toProperties->begin();
        }
        return false;
    }

    bool Connector::ConnectGroupsStar(GroupContainer& toGroup, GroupContainer& fromGroup) {
        return ConnectGroupsFanIn(toGroup, fromGroup);
    }

    bool Connector::ConnectGroupTrees() {
        // Set up group tree enumerators over terminal groups.
        GroupContainer* toTree = mToEnumerator->PathGroupTree;
        GroupContainer* fromTree = mFromEnumerator->PathGroupTree;
        GroupTreeEnumerator* toTreeEnumerator = new GroupTreeEnumerator(*toTree, true);
        GroupTreeEnumerator* fromTreeEnumerator = new GroupTreeEnumerator(*fromTree, true);

        // Finally, traverse group trees, by terminal groups, making connections.
        GroupContainer* currentToGroup;
        GroupContainer* currentFromGroup;
        Property* p;

        for(;;) {
            p = toTreeEnumerator->Next();
            if(p==NULL || !p->Flags.IsGroupContainer) break;
            currentToGroup = (GroupContainer*)p;
            p = fromTreeEnumerator->Next();
            if(p==NULL || !p->Flags.IsGroupContainer) break;
            currentFromGroup = (GroupContainer*)p;

            switch (mConnectorPattern) {
                case ConnectorPatterns::OneToOne:
                    if (ConnectGroupsOneToOne(*currentToGroup, *currentFromGroup)) {
                        return true;
                    }
                    break;
                case ConnectorPatterns::FanIn:
                    if (ConnectGroupsFanIn(*currentToGroup, *currentFromGroup)) {
                        return true;
                    }
                    break;
                case ConnectorPatterns::FanOut:
                    if (ConnectGroupsFanOut(*currentToGroup, *currentFromGroup)) {
                        return true;
                    }
                    break;
                case ConnectorPatterns::Star:
                    if (ConnectGroupsStar(*currentToGroup, *currentFromGroup)) {
                        return true;
                    }
                    break;
                default:
                    break;
            }
        }
        return false;
    }

    void Connector::Apply(ResolutionModesEnum resolutionMode, Container& applicationContainer) {

        mResolutionMode = resolutionMode;
        mApplicationContainer = &applicationContainer;

        if (!NeedsResolving()) {
            return;
        }

        Model & model = Model::Current();
        model.PushCurrentConnector(*this);

        // Build the group trees for the To and From paths.
        if (BuildGroupTrees()) {
            goto byebye;
        }

        // Apply translator to pairs of terminal To and From groups.
        if (ApplyTranslators()) {
            goto byebye;
        }

        // Make connections by adding connections between terminal group pairs.
        ConnectGroupTrees();

        byebye:
        model.PopCurrentConnector();
    }

    bool Connector::CheckGroup(PathEnumeratorState* state, PathEnumerator* pathEnumerator) {
        if (state == NULL || pathEnumerator == NULL) {
            return false;
        }
        bool isTo = (pathEnumerator==mToEnumerator);

        // Group size formula: ultimately, cardinality of GS == coGS.
        // If this pathNode is unbounded, then GS = coGS,
        // else if this pathNode is not unbounded,
        //      then GS = container.Count * RepeatCount == coGS,
        // where RepeatCount = 1, if co-group is unbounded,
        //       RepeatCount = coGS / container.Count.

        GroupContainer* group = state->PathGroupTree;
        int count = group->Count();

        if (state->CurrentPathNode->IsUnbounded) {
            int coGS = GetGroupCount(!isTo, group->Property::GetOrderID());
            if (count < coGS) {
                state->CountRemaining = coGS - count;
                return true;
            }
        } else {
            GroupContainer* bGroup = GetGroup(!isTo,group->Property::GetOrderID());
            if (!bGroup->IsUnbounded) {
                if (CheckComplete(!isTo, bGroup)) {
                    int coGS = bGroup->Count();
                    if (count < coGS) {
                        state->CountRemaining = coGS - count;
                        return true;
                    }
                } else {
                    // Path group cardinaility error: unable to complete group.
                    // For now, fall through and apply GroupFilter.
                }
            }
        }

        // Now check group elements are in order.
        GroupFilter* gf = state->NodeEnumerator->NodeGroupFilter;
        if (gf != NULL) {
            // Call GroupFilter - which should apply any necessary order transforms.
            gf->Arguments->Source = group;
            // This causes the GroupFilter's OrderGroup method to be invoked via the RefreshContents method.
            gf->AccessContents();
        }

        // Set IsComplete accordingly.
        group->IsComplete = true;
        return false;
    }

    int Connector::GetAppendCount(Container& container, const IdentifierRegex& regexIdentifier) {
        GroupContainer* group = GetCurrentGroup(mIsTo);
        if (group == NULL) {
            return -1;
        }
        // Return the size of the corresponding co-group.
        return GetGroupCount(!mIsTo,group->Property::GetOrderID());
    }

    int Connector::GetGroupCount(bool isTo) {
        GroupContainer* group = GetCurrentGroup(isTo);
        if (group == NULL) {
            return -1;
        }
        return GetGroupCount(isTo, group->Property::GetOrderID());
    }

    int Connector::GetGroupCount(bool isTo, int* groupID) {
        if (groupID == NULL) {
            return -1;
        }
        // DEBUG: TO DO: Connector.GetGroupCount: Efficiency issue: Add a parameter to InvokeEnumerator
        // to improve efficiency, otherwise GetProperty called each iteration.

        // Get sub group whose size determines group count.
        GroupContainer* group = GetGroup(isTo, groupID);

        if (CheckComplete(isTo, group)) {
            return group->Count();
        }
        return -1;
    }

    bool Connector::CheckComplete(bool isTo, GroupContainer* group) {
        if (group == NULL) {
            return false;
        }
        bool wasError = false;

        // Check that group is complete, invoke enumerator as necessary.
        while (!group->IsComplete) {
            bool wasEnd = !InvokeEnumerator(isTo, wasError);
            if (wasError) {
                goto ReportError;
            }
            if (wasEnd)
            break;
        }
        if (group->IsComplete) {
            return true;
        }

    ReportError:
        Error::Log(false, *Model::Current().ModelErrorContext, *GetClassTypeInfo(), "CheckComplete", 2, 
            "Unable to complete group for connector: %s, %s-path.", StatusReport().c_str(), (isTo ? "To" : "From"));
        return false;
    }

    GroupContainer* Connector::GetCurrentGroup(bool isTo) {
        // Set 'a' to required path enumerator.
        PathEnumerator* a = isTo ? mToEnumerator : mFromEnumerator;
        if (a == NULL) {
            Debug::WriteLine(1, "Connector.GetCurrentGroup: NULL enumerator.");
            return NULL;
        }
        // Work out groupID of required group.
        PathEnumeratorState* state = a->CurrentState();
        if (state == NULL) {
            Debug::WriteLine(1, "Connector.GetCurrentGroup: NULL enumerator state.");
            return NULL;
        }
        GroupContainer* group = state->PathGroupTree;
        if (group == NULL) {
            Debug::WriteLine(1, "Connector.GetCurrentGroup: NULL group property.");
        }
        return group;
    }

    GroupContainer* Connector::GetGroup(bool isTo, int* groupID) {
        if (groupID == NULL) {
            return NULL;
        }
        bool wasError = false;
        const char* errorMessage = NULL;
        GroupContainer* group;
        Property* p;

        // DEBUG: TO DO: Connector.GetGroupCount: Efficiency issue: Add a parameter to InvokeEnumerator
        // to improve efficiency, otherwise GetProperty called each iteration.

        // Get group tree root, invoke enumerator as necessary.
        PathEnumerator* a = isTo ? mToEnumerator : mFromEnumerator;
        GroupContainer* treeRoot = a->PathGroupTree;
        while (treeRoot == NULL) {
            bool wasEnd = !InvokeEnumerator(isTo, wasError);
            if (wasError) {
                errorMessage = "no group tree";
                goto ReportError;
            }
            treeRoot = a->PathGroupTree;
            if (wasEnd) {
                break;
            }
        }
        if (treeRoot == NULL) {
            goto ReportError;
        }

        // Get sub group, invoke enumerator as necessary.
        p = Container::GetProperty(treeRoot, groupID);
        if(p!=NULL && !p->Flags.IsGroupContainer) {
            errorMessage =  "property not a group";
            goto ReportError;
        }
        group = (GroupContainer*)p;

        while (group == NULL) {
            bool wasEnd = !InvokeEnumerator(isTo, wasError);
            if (wasError) {
                errorMessage = "group not found";
                goto ReportError;
            }
            p = Container::GetProperty(treeRoot, groupID);
            if(p!=NULL && !p->Flags.IsGroupContainer) {
                errorMessage =  "property not a group";
                goto ReportError;
            }
            group = (GroupContainer*)p;
            if (wasEnd) {
                break;
            }
        }
        if (group != NULL) {
            return group;
        }
        errorMessage = "group not found";

    ReportError:
        Error::Log(false, *Model::Current().ModelErrorContext, *GetClassTypeInfo(), "GetGroup", 3, 
            "Unable to fetch group for connector: %s, %s, %s-path.", StatusReport().c_str(), errorMessage, (isTo ? "To" : "From"));
        return NULL;
    }

    bool Connector::InvokeEnumerator(bool isTo, bool& wasError) {
        if (isTo) {
            if (mToLock) {
                wasError = true;
                return false;
            }
            mToLock = true;
            bool result = mToEnumerator->MoveNext();
            mToLock = false;
            wasError = false;
            return result;
        } else {
            if (mFromLock) {
                wasError = true;
                return false;
            }
            mFromLock = true;
            bool result = mFromEnumerator->MoveNext();
            mFromLock = false;
            wasError = false;
            return result;
        }
    }
}
