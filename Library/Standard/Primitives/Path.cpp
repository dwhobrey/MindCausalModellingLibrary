#include "PlatoIncludes.h"
#include "Regexes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "EnvironmentVariables.h"
#include "Configuration.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "Property.h"
#include "ThreadLocalVariable.h"
#include "ConfigurePhases.h"
#include "ResolutionModes.h"
#include "Container.h"
#include "Path.h"
#include "PathNode.h"
#include "PathEnumerator.h"
#include "Model.h"
#include "Error.h"

namespace Plato {

    const ClassTypeInfo* Path::TypeInfo = NULL;
    const string* Path::DirectoryAndGroupSeparatorString = NULL;
    const char* Path::ParentContainer = "..";
    const PathNode* Path::Absolute = NULL;
    const PathNode* Path::Relative = NULL;
    const PathNode* Path::Parent = NULL;
    const PathNode* Path::Owner = NULL;
    const PathNode* Path::Any = NULL;
    const PathNode* Path::Wild = NULL;
    const PathNode* Path::Start = NULL;
    const PathNode* Path::End = NULL;
    string* Path::mPathPrefix = NULL;
    Path::HashMap* Path::mStandardPathNodes = NULL;
 
    void Path::Initializer() {
        TypeInfo = new ClassTypeInfo("Path",6,1,NULL,NULL);
        string* s = new string(1,DirectorySeparator);
        *s += GroupSeparator;
        DirectoryAndGroupSeparatorString = s;
        Absolute = new PathNode(true,PathNodeKinds::Absolute, *new Identifier(Path::DirectorySeparator));
        Relative = new PathNode(true,PathNodeKinds::Relative, *new Identifier(Path::CurrentContainer));
        Parent = new PathNode(true,PathNodeKinds::Parent, *new Identifier(Path::ParentContainer,NULL,NULL));
        Owner = new PathNode(true,PathNodeKinds::Owner, *new Identifier(Path::OwnerContainer));
        Any = new PathNode(true,PathNodeKinds::Any, *new Identifier(*new string(*Regexes::AnyPatternString)));
        Wild = new PathNode(true,PathNodeKinds::Wild, *new Identifier(*new string(*Regexes::WildPatternString)));
        Start = new PathNode(true,PathNodeKinds::Start, *new Identifier(Path::StartPattern));
        End = new PathNode(true,PathNodeKinds::End, *new Identifier(Path::EndPattern));
        mStandardPathNodes = new HashMap();
        (*mStandardPathNodes)[Absolute->Identity->Name] = Absolute;
        (*mStandardPathNodes)[Relative->Identity->Name] = Relative;
        (*mStandardPathNodes)[Parent->Identity->Name] = Parent;
        (*mStandardPathNodes)[Owner->Identity->Name] = Owner;
        (*mStandardPathNodes)[Any->Identity->Name] = Any;
        (*mStandardPathNodes)[Wild->Identity->Name] = Wild;
        (*mStandardPathNodes)[Start->Identity->Name] = Start;
        (*mStandardPathNodes)[End->Identity->Name] = End;     
    }

    void Path::Finalizer() {
        delete TypeInfo;
        delete DirectoryAndGroupSeparatorString;
        delete mStandardPathNodes;
        delete Absolute;
        delete Relative;
        delete Parent;
        delete Owner;
        delete Any;
        delete Wild;
        delete Start; 
        delete End;
    }

#pragma region // Constructors.
    Path::Path(Container* creator, PathNode& pn) {
        Creator = creator;
        if (pn.Parent == NULL || pn.NodeKind!=PathNodeKinds::Normal) {
            HeadPathNode = pn.GetWritable();
        } else {
            HeadPathNode = Path::Relative->GetWritable();
            HeadPathNode->Append(*pn.GetWritable());
        }
        RefreshFlags();
    }

    Path::Path(const Path& path) {
        Creator = path.Creator;
        if (path.HeadPathNode != NULL) {
            HeadPathNode = new PathNode(*(path.HeadPathNode));
            PathNode* n = HeadPathNode;
            while (n != NULL) {
                n->ParentPath = this;
                n = n->Next;
            }
        } else {
            HeadPathNode = NULL;
        }
    }

    Path::~Path() {
        delete HeadPathNode;
    }
#pragma endregion

#pragma region // Accessors.
    PathNode* Path::Tail() {
        return (HeadPathNode == NULL) ? NULL : HeadPathNode->Tail();
    }

    const Identifier* Path::GetTailIdentity() {
        PathNode* p = Tail();
        return (p == NULL) ? NULL : p->Identity;
    }

    string* Path::GetPathPrefix() {
        if (mPathPrefix == NULL) {
            const Configuration* configuration = Model::Current().ModelConfiguration;
            if (configuration->PathPrefix == NULL) {
                Error::Log(true, *Model::Current().ModelErrorContext, *TypeInfo, "PathPrefix", 4, "PathPrefix was NULL.");
            } else {
                mPathPrefix = &EnvironmentVariables::ExpandString(*configuration,*(configuration->PathPrefix));
            }
        }
        return mPathPrefix;
    }
#pragma endregion

#pragma region // Private and public methods.
    void Path::RefreshFlags() {
        PathNode* n = HeadPathNode;
        if (n != NULL) {
            n->RefreshGroups();
            while (n != NULL) {
                n->ParentPath = this;
                n = n->Next;
            }
        }
    }

    Property* Path::GetProperty(Container* currentContainer, const Property* requester, PropertyModesEnum mode, 
                PathNode*& pathNode, Container*& contextContainer) const {
        PathNode* n = HeadPathNode;
        Property* p = currentContainer;
        contextContainer = NULL;
        while (n != NULL) {
            p = n->GetProperty(currentContainer, requester, mode, contextContainer);
            n->NodeProperty = p;
            if (p == NULL) {
                // Branch search failed, check if multivalued.
                if(contextContainer!=NULL) {
                    p = currentContainer;
                    break;
                }
                // Not multivalued, so return Head pathNode and NULL.
                n = HeadPathNode;
                break;
            }
            Property* v = p->GetValue();
            if(v->Flags.IsContainer) {
                Container* container = (Container*)v;
                if (n->Next == NULL) {
                    // Check for implicit Directors.
                    if ((mode & PropertyModes::Directing) != 0) {
                        Property* q = container->DirectToProperty(NULL, requester, requester, mode);
                        if (q != NULL) {
                            contextContainer = q->Parent;
                            p = q;
                        }
                    }
                    break;
                }
                currentContainer = container;
                n = n->Next;
            } else {
                break;
            }
        }
        pathNode = n;
        return p;
    }

    Property* Path::GetProperty(Container* currentContainer, const Property* requester, PropertyModesEnum mode) const {
        PathNode* n;
        Container* c;
        return GetProperty(currentContainer, requester, mode, n, c);
    }

    PathEnumerator* Path::GetEnumerator(Container* currentContainer, const Property* requester, PropertyModesEnum mode) {
        return new PathEnumerator(*this,currentContainer,requester,mode,false,NULL);
    }

    const PathNode* Path::GetStandardNode(const string* name) {
        HashMap::iterator itr = mStandardPathNodes->find(name);
        return itr==mStandardPathNodes->end() ? NULL : itr->second; 
    }

    void Path::Append(PathNode* node) {
        if (node != NULL) {
            PathNode* currentTail = Tail();
            if (currentTail != NULL) {
                currentTail->Next = node;
                node->Parent = currentTail;
            } else {
                HeadPathNode = node;
                node->Parent = NULL;
            }
            RefreshFlags();
        }
    }

    int Path::CompareTo(const Path& absolutePath) const {
        // DEBUG: This could call ToAbsolute...
        return -1;
    }

    int Path::Compare(const Path* a, const Path* b) {
        if (a != NULL) {
            if (b == NULL) {
                return 1;
            }
            return a->CompareTo(*b);
        } else if (b != NULL) {
            return -1;
        }
        return 0;
    }

    Path* Path::ToAbsolute(Container* currentContainer) {
        // DEBUG: TO DO.
        return NULL;
    }

    string& Path::StatusReport() const {
        return (HeadPathNode == NULL ? *new string("") : HeadPathNode->StatusReport());
    }
#pragma endregion

#pragma region // Static helper methods for working with Paths.
    string* Path::GetPropertyName(const string& path) {
        string::size_type dirSepIndex = path.find_last_of(*DirectoryAndGroupSeparatorString);
        if (dirSepIndex != string::npos && ++dirSepIndex<path.size()) {
            string* propertyName = new string(path.substr(dirSepIndex));
            Strings::Trim(*propertyName);
            if(!propertyName->empty()) {
                return propertyName;
            }
            delete propertyName;
        }
        return NULL;
    }

    string& Path::GetContainerPath(const string& path) {
        string::size_type dirSepIndex = path.find_last_of(*DirectoryAndGroupSeparatorString);
        if (dirSepIndex!=string::npos && dirSepIndex > 0) {
            string& containerPath = *new string(path);
            Strings::Trim(containerPath.erase(dirSepIndex));
            if(!containerPath.empty()) {
                return containerPath;
            }
            delete &containerPath;
        }
        return *new string(1,DirectorySeparator);
    }

    string* Path::GetPropertyNameAndContainerPath(const string& path, string*& containerPath) {
        string* propertyName = NULL;
        string::size_type dirSepIndex = path.find_last_of(*DirectoryAndGroupSeparatorString);
        if (dirSepIndex != string::npos) {
            string::size_type propIndex = 1+dirSepIndex;
            if(propIndex<path.size()) {
                propertyName = new string(path.substr(propIndex));
                Strings::Trim(*propertyName);
                if (propertyName->empty()) {
                    propertyName = NULL;
                }
            }
        }
        if (dirSepIndex!=string::npos && dirSepIndex > 0) {
            containerPath = new string(path);
            Strings::Trim(containerPath->erase(dirSepIndex));
            if (containerPath->empty()) {
                containerPath = new string(1,DirectorySeparator);
            }
        } else {
            containerPath = new string(1,DirectorySeparator);
        }
        return propertyName;
    }
#pragma endregion
}
