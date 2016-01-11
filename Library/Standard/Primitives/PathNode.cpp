#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Debug.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "Point.h"
#include "PointSpace.h"
#include "PointRegex.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "Property.h"
#include "Arguments.h"
#include "ThreadLocalVariable.h"
#include "ConfigurePhases.h"
#include "ResolutionModes.h"
#include "Container.h"
#include "Path.h"
#include "PathNode.h"
#include "PathNodeEnumerator.h"
#include "Model.h"
#include "Error.h"
#include "Filter.h"
#include "Constructor.h"

namespace Plato {

    const ClassTypeInfo* PathNode::TypeInfo = NULL;

    void PathNode::Initializer() {
        TypeInfo = new ClassTypeInfo("PathNode",7,1,NULL,NULL);
    }

    void PathNode::Finalizer() {
        delete TypeInfo;
    }

    PathNode::PathNode(bool isConst, PathNodeKindsEnum nodeKind, const Identifier& nodeIdentifier) {
        mIsConst = isConst;
        IsUnbounded = false;
        GroupBoundary = false;
        GroupNumber = 0;
        NodeKind = nodeKind;
        Next = NULL;
        Parent = NULL;
        ParentPath = NULL;
        NodeProperty = NULL;
        Identity = &nodeIdentifier;
        mFilterType = NULL;
        mFilterPath = NULL;
        AdditionalArguments = NULL;
    }

    PathNode::PathNode(const Identifier& nodeIdentifier) {
        mIsConst = false;
        IsUnbounded = false;
        GroupBoundary = false;
        GroupNumber = 0;
        NodeKind = PathNodeKinds::Normal;
        Next = NULL;
        Parent = NULL;
        ParentPath = NULL;
        NodeProperty = NULL;
        Identity = &nodeIdentifier;
        mFilterType = NULL;
        mFilterPath = NULL;
        AdditionalArguments = NULL;
   }

    PathNode::PathNode(const Identifier& nodeIdentifier, PathNode* parent) {
        mIsConst = false;
        IsUnbounded = false;
        GroupBoundary = false;
        GroupNumber = 0;
        NodeKind = PathNodeKinds::Normal;
        Next = NULL;
        Parent = parent;
        if(Parent!=NULL) {
            Parent->Next = this;
        }
        ParentPath = NULL;
        NodeProperty = NULL;
        Identity = &nodeIdentifier;
        mFilterType = NULL;
        mFilterPath = NULL;
        AdditionalArguments = NULL;
    }

    PathNode::PathNode(const IdentifierRegex& regexIdentifier, PathNode* parent) {
        mIsConst = false;
        const PointRegex* p = regexIdentifier.Position;
        IsUnbounded = p!=NULL && p->Status==PointRegexStatus::Unbounded;
        GroupBoundary = false;
        GroupNumber = 0;
        NodeKind = PathNodeKinds::Regex;
        Next = NULL;
        Parent = parent;
        if(Parent!=NULL) {
            Parent->Next = this;
        }
        ParentPath = NULL;
        NodeProperty = NULL;
        RegexIdentity = &regexIdentifier;
        mFilterType = NULL;
        mFilterPath = NULL;
        AdditionalArguments = NULL;
    }

    PathNode::PathNode(const PathNode& pathNode) {
        mIsConst = pathNode.mIsConst;
        IsUnbounded = pathNode.IsUnbounded;
        GroupBoundary = pathNode.GroupBoundary;
        GroupNumber = pathNode.GroupNumber;
        NodeKind = pathNode.NodeKind;
        Next = pathNode.Next;
        Parent = pathNode.Parent;
        ParentPath = pathNode.ParentPath;
        NodeProperty = pathNode.NodeProperty;
        if(NodeKind==PathNodeKinds::Regex) {
            RegexIdentity = new IdentifierRegex(*pathNode.RegexIdentity);
        } else if (pathNode.Identity!=NULL) {
            Identity = new Identifier(*pathNode.Identity);
        } else {
            Identity = NULL;
        }
        mFilterType = pathNode.mFilterType;
        mFilterPath = pathNode.mFilterPath==NULL ? NULL : new Path(*mFilterPath);
        AdditionalArguments = pathNode.AdditionalArguments==NULL ? NULL : pathNode.AdditionalArguments->Clone();
        if (Next != NULL) {
            Next = new PathNode(*Next);
            Next->Parent = this;
        }
    }

    PathNode::PathNode(const ClassTypeInfo& filterType, Arguments* arguments) {
        mIsConst = false;
        IsUnbounded = false;
        GroupBoundary = false;
        GroupNumber = 0;
        NodeKind = PathNodeKinds::Filter;
        Next = NULL;
        Parent = NULL;
        ParentPath = NULL;
        NodeProperty = NULL;
        Identity = NULL;
        mFilterType = &filterType;
        mFilterPath = NULL;
        AdditionalArguments = arguments;
    }

    PathNode::PathNode(const Path& filterPath, Arguments* arguments) {
        mIsConst = false;
        IsUnbounded = false;
        GroupBoundary = false;
        GroupNumber = 0;
        NodeKind = PathNodeKinds::Filter;
        Next = NULL;
        Parent = NULL;
        ParentPath = NULL;
        NodeProperty = NULL;
        Identity = NULL;
        mFilterType = NULL;
        mFilterPath = &filterPath;
        AdditionalArguments = arguments;
    }
    
    PathNode::~PathNode() {
        delete Next;
        if(NodeKind==PathNodeKinds::Regex) {
            delete RegexIdentity;
        } else {
            delete Identity;
        }
        delete mFilterPath;
        delete AdditionalArguments;
    }

    void PathNode::Reset() {
        delete Next;
        if(NodeKind==PathNodeKinds::Regex) {
            delete RegexIdentity;
        } else {
            delete Identity;
        }
        delete mFilterPath;
        delete AdditionalArguments;
        mIsConst = false;
        IsUnbounded = false;
        GroupBoundary = false;
        GroupNumber = 0;
        NodeKind = PathNodeKinds::Normal;
        Next = NULL;
        Parent = NULL;
        ParentPath = NULL;
        NodeProperty = NULL;
        Identity = NULL;
        mFilterType = NULL;
        mFilterPath = NULL;
        AdditionalArguments = NULL;
    }

    PathNode* PathNode::Tail() {
        PathNode* p = this;
        while (p->Next != NULL) p = p->Next;
        return p;
    }

    PathNode* PathNode::Head() {
        PathNode* p = this;
        while (p->Parent != NULL) p = p->Parent;
        return p;
    }

    int PathNode::Length() {
        int length = 1;
        PathNode* p = Head();
        while (p->Next != NULL) {
            ++length;
            p = p->Next;
        }
        return length;
    }

    int PathNode::Index() {
        int index = 0;
        PathNode* p = this;
        while (p->Parent != NULL) {
            ++index;
            p = p->Parent;
        }
        return index;
    }

    PathNode* PathNode::operator[](int index) {
        PathNode* p = Head();
        while (p!=NULL && index-- > 0) {
            p = p->Next;
        }
        return p;
    }

    void PathNode::Remove() {
        if (ParentPath != NULL) {
            if (this==ParentPath->HeadPathNode) {
                ParentPath->HeadPathNode = Next;
            }
        }
        if (Next != NULL) {
            Next->Parent = Parent;
        }
        if (Parent != NULL) {
            Parent->Next = Next;
            Parent = NULL;
        }
        Next = NULL;
    }

    void PathNode::Insert(PathNode& pathNode) {
        if (ParentPath != NULL) {
            if (this==ParentPath->HeadPathNode) {
                ParentPath->HeadPathNode = &pathNode;
            }
        }
        pathNode.ParentPath = ParentPath;
        pathNode.Next = this;
        pathNode.Parent = Parent;
        if(Parent!=NULL) {
            Parent->Next = &pathNode;
        }
        Parent = &pathNode;
    }

    void PathNode::Append(PathNode& pathNode) {
        PathNode* p = Tail();
        p->Next = &pathNode;
        pathNode.Parent = p;
    }

    string& PathNode::StatusReport() {
        string& prefix = (NodeKind==PathNodeKinds::Filter) ? *new string("") 
            : (NodeKind==PathNodeKinds::Regex ? RegexIdentity->StatusReport() 
                    : Identity->StatusReport());
        char separator = (Next!=NULL && Next->GroupBoundary) ? Path::GroupSeparator : Path::DirectorySeparator;
        switch (NodeKind) {
            case PathNodeKinds::Normal:
                break;
            case PathNodeKinds::Regex:
                break;
            case PathNodeKinds::Wild:
                break;
            case PathNodeKinds::Any:
                break;
            case PathNodeKinds::Absolute:
                if(prefix.size()==1 && prefix[0]==Path::DirectorySeparator) {
                    if (Next == NULL) {
                        ;
                    } else {
                        separator = '\0';
                    }
                } else {                    
                    prefix.insert(0,0,Path::DirectorySeparator);
                }
                break;
            case PathNodeKinds::Relative:
                break;
            case PathNodeKinds::Parent:
                break;
            case PathNodeKinds::Owner:
                prefix.insert(0,0,Path::OwnerContainer);
                break;
            case PathNodeKinds::Director:
                prefix.insert(0,0,Path::DirectorPrefix);
                break;
            case PathNodeKinds::Filter:
                prefix += "Filter:"
                    + (mFilterType != NULL ? ("typeof(" + *(mFilterType->ClassName) + ")") 
                                           : ("path(" + mFilterPath->StatusReport() + ")") ) 
                    + (AdditionalArguments == NULL ? "" : "(args)" )
                    ;
                break;
            default: ;
        }
        if(Next!=NULL) {
            string& tmp = Next->StatusReport();
            if(separator!='\0') {
                prefix +=separator;
            }
            prefix += tmp;
            delete &tmp;
        }
        return prefix;
    }

    bool PathNode::IdentifierMatches(PathNode& regexPathNode) {
        return regexPathNode.NodeKind == PathNodeKinds::Wild 
            || (regexPathNode.NodeKind==PathNodeKinds::Regex ? 
                    regexPathNode.RegexIdentity->Match(*Identity) 
                    : (regexPathNode.Identity->CompareTo(*Identity)==0)
                );
    }

    void PathNode::RefreshGroups() {
        PathNode* n = Tail();
        int groupNumber = 0;
        while (n != NULL) {
            n->GroupNumber = groupNumber;
            if (n->GroupBoundary) {
                ++groupNumber;
            }
            n = n->Parent;
        }
    }

    PathNode* PathNode::GetWritable() const {
        if (mIsConst) {
            PathNode* p = new PathNode(*this);
            p->mIsConst = false;
            return p;
        }
        void* q = (void*)this;
        return (PathNode*)q;
    }

    PathNodeEnumerator* PathNode::GetEnumerator(Container* currentContainer, 
            const Property* requester, PropertyModesEnum mode) {
        return new PathNodeEnumerator(*this, currentContainer,requester,mode);
    }

    PathNode& PathNode::GetPathNodes(Container* container, Property* prop) {
        PathNode* lastNode = (prop == NULL) ? NULL : new PathNode(*prop->Identity);
        if (container == NULL && prop != NULL) {
            container = prop->Parent;
        }
        while (container != NULL) {
            PathNode* n = new PathNode(*container->Identity);
            if (lastNode != NULL) {
                lastNode->Parent = n;
                n->Next = lastNode;
            }
            lastNode = n;
            container = container->Parent;
        }
        PathNode* pathNode = Path::Absolute->GetWritable();
        pathNode->Append(*lastNode);
        return *pathNode;
    }

    Property* PathNode::GetProperty(Container* nodeCurrentContainer, const Property* requester, 
                PropertyModesEnum mode, Container*& contextContainer) {
        switch (NodeKind) {
            case PathNodeKinds::Normal:
                contextContainer = nodeCurrentContainer;
                if(contextContainer!=NULL) {
                    return contextContainer->GetProperty(*Identity,requester,mode);
                }
                break;
            case PathNodeKinds::Regex:
                contextContainer = nodeCurrentContainer;
                break;
            case PathNodeKinds::Any:
                contextContainer = nodeCurrentContainer;
                break;
            case PathNodeKinds::Wild:
                contextContainer = nodeCurrentContainer;
                break;
            case PathNodeKinds::Absolute:
                return contextContainer = &(Model::Current());
            case PathNodeKinds::Relative:
                return contextContainer = nodeCurrentContainer;
            case PathNodeKinds::Parent:
                if(nodeCurrentContainer!=NULL) {
                    contextContainer = nodeCurrentContainer->Parent;
                } else {
                    contextContainer = NULL;
                }
                return contextContainer;
            case PathNodeKinds::Owner:
                return contextContainer = ParentPath->Creator;
            case PathNodeKinds::Director:
                contextContainer = nodeCurrentContainer;
                if(contextContainer!=NULL) {
                    return contextContainer->DirectToProperty(Identity, requester, requester, mode);
                }
                break;
            case PathNodeKinds::Filter:
                contextContainer = nodeCurrentContainer;
                break;
            case PathNodeKinds::Start:
                return contextContainer = &(Model::Current());
            case PathNodeKinds::End:
                return contextContainer = &(Model::Current());
        }
        return NULL;
    }

    Container* PathNode::CreateFilter(FilterArguments& filterArguments) {
        if (mFilterType == NULL) {
            // Construct from path.
            // Note, PathNode objects are immutable because they may be used concurrently by
            // enumerators with a different context, e.g. when the Path.Any node appears in the path.
            // This also means that the type, when resolved from a path, should not be cached,
            // since it may depend on context.
            if (mFilterPath == NULL) {
                Debug::WriteLine(1, "PathNode.CreateFilter: NULL FilterPath and FilterType encountered.");
                return NULL;
            }
            // Retrieve property from Path object.
            Property* p = mFilterPath->GetProperty(filterArguments.Parent, filterArguments.Requester, PropertyModes::Traversing);
            if (p == NULL) {
                Error::Log(false, *Model::Current().ModelErrorContext, *TypeInfo, "CreateFilter", 1, 
                    "Filter path did not resolve to an object: %s.", mFilterPath->StatusReport().c_str());
                return NULL;
            }
            // Check property value is a Filter type.
            p = p->GetValue();
            if (p == Property::NullProperty) {
                Error::Log(false, *Model::Current().ModelErrorContext, *TypeInfo, "CreateFilter", 2, 
                    "Filter path property value was NULL: %s.", mFilterPath->StatusReport().c_str());
                return NULL;
            }
            if (!p->Flags.IsConstructor) {
                Error::Log(false, *Model::Current().ModelErrorContext, *TypeInfo, "CreateFilter", 3, 
                    "Filter path property value was not a constructor: %s.", mFilterPath->StatusReport().c_str());
                return NULL;       
            }       
            return (Container*)((Constructor*)p)->Factory(&filterArguments);
        }  
        return (Container*)(*(mFilterType->Factory))(&filterArguments);
    }
}
