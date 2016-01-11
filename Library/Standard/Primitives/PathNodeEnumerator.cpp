#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Debug.h"
#include "Identifier.h"
#include "IdentifierRegex.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "Property.h"
#include "ConfigurePhases.h"
#include "Container.h"
#include "Arguments.h"
#include "PropertyEnumerator.h"
#include "Filter.h"
#include "ScopeEnumerator.h"
#include "PathNodeEnumerator.h"
#include "PathNode.h"
#include "Path.h"
#include "Filter.h"
#include "IdentifierFilter.h"

namespace Plato {

    PathNodeEnumerator::PathNodeEnumerator(PathNode& currentPathNode, Container* nodeCurrentContainer, 
            const Property* requester, PropertyModesEnum mode) {
        mPathNode = new PathNode(currentPathNode);
        mNodeCurrentContainer = nodeCurrentContainer;
        mRequester = requester;
        mMode = mode;
        mNodeEnumerator = NULL;
        mFilterContainer = NULL;
        Reset();
    }

    PathNodeEnumerator::~PathNodeEnumerator() {
        delete mPathNode;
        delete mNodeEnumerator;
        delete mFilterContainer;
    }

    void PathNodeEnumerator::Reset() {
        Current = NULL;
        NodeGroupFilter = NULL;
        mWorkingContainer = NULL;
        if(mNodeEnumerator!=NULL) {
            delete mNodeEnumerator;
            mNodeEnumerator = NULL;
        }
        if(mFilterContainer!=NULL) {
            delete mFilterContainer;
            mFilterContainer = NULL;
        }
        mIsReset = true;
    }

    bool PathNodeEnumerator::MoveNext() {
        // Working out what is next item from path node:
        // 0) Move to required container: (e.g. Owner, Parent, etc.), see NodeKind.
        // 1) Apply match against mNodeIdentifier - which may be a filter.
        // 2) Apply filter.
        // 3) Get enumerator for filter.
        // 4) Return values via enumerator.
        while (mIsReset) {
            mIsReset = false;
            mNodeEnumerator = NULL;
            // Check if this node refers to a single container or many.
            switch(mPathNode->NodeKind) {
                case PathNodeKinds::Wild:
                    // A Wild node acts like a filter that allows all through, in which case skip over filtering.
                    mWorkingContainer = mNodeCurrentContainer;
                    if (mWorkingContainer == NULL) {
                        return false;
                    }
                    mNodeEnumerator = new ScopeEnumerator(*mWorkingContainer,mRequester);
                    break;
                case PathNodeKinds::Regex:
                    mWorkingContainer = mNodeCurrentContainer;
                    if (mWorkingContainer == NULL) {
                        return false;
                    }
                    // Check if need to set up a filter on the fly.
                    if(mPathNode->AdditionalArguments==NULL) {
                        mPathNode->AdditionalArguments = new IdentifierFilterArguments(*new IdentifierRegex(*(mPathNode->RegexIdentity)),true);
                        mPathNode->mFilterType = IdentifierFilter::TypeInfo;
                    }
                case PathNodeKinds::Filter: {
                        mWorkingContainer = mNodeCurrentContainer;
                        if (mWorkingContainer == NULL) {
                            return false;
                        }
                        // Apply filter to get final enumeration.
                        FilterArguments* filterArguments = new FilterArguments(mPathNode->ParentPath->Creator,mWorkingContainer,mWorkingContainer,
                                                                    mRequester,mMode,mPathNode->AdditionalArguments);
                        mFilterContainer = mPathNode->CreateFilter(*filterArguments);
                        if (mFilterContainer == NULL) {
                            delete filterArguments;
                            return false;
                        }
                        if(mFilterContainer->Flags.IsGroupFilter) {
                            NodeGroupFilter = (GroupFilter*)mFilterContainer;
                        }
                        mFilterContainer->AccessContents();
                        mNodeEnumerator = new ScopeEnumerator(*mFilterContainer,mRequester);
                    }
                    break;
                default: // Node refers to a single object.
                    Current = mPathNode->GetProperty(mNodeCurrentContainer,mRequester,mMode,mWorkingContainer);
                    return Current != NULL;
            }
        }

        if (mNodeEnumerator != NULL && mNodeEnumerator->MoveNext()) {
            Current = mNodeEnumerator->Current;
        } else {
            Current = NULL;
        }
        return Current != NULL;
    }
}
