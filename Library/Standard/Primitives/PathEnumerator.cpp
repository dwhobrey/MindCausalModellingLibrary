#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "Property.h"
#include "ConfigurePhases.h"
#include "Container.h"
#include "GroupContainer.h"
#include "Path.h"
#include "PathNode.h"
#include "PathEnumerator.h"
#include "PathNodeEnumerator.h"
#include "ResolutionModes.h"
#include "Connector.h"

namespace Plato {

    PathEnumeratorState::PathEnumeratorState(Container* container, PathNode* pathNode, bool skipEnumerator, 
            const Property* requester, PropertyModesEnum mode, GroupContainer* pathGroupTree, int countRemaining, bool doDelete) {
        CurrentContainer = container;
        PathGroupTree = pathGroupTree;
        CurrentPathNode = pathNode;
        CountRemaining = countRemaining;
        DoDelete = doDelete;
        NodeEnumerator = (skipEnumerator ? NULL : pathNode->GetEnumerator(container, requester, mode));
    }

    PathEnumeratorState::~PathEnumeratorState() {
        delete NodeEnumerator;
        if(DoDelete && CurrentPathNode!=NULL) {
            delete CurrentPathNode->Head();
        }
    }

    PathEnumerator::PathEnumerator(Path& path, Container* pathCurrentContainer, const Property* requester, 
        PropertyModesEnum mode, bool isWidthFirstDescent, Connector* groupHandler) {
        mPath = new Path(path);
        mPathCurrentContainer = pathCurrentContainer;
        mRequester = requester;
        mMode = mode;
        mIsWidthFirstDescent = isWidthFirstDescent;
        mGroupHandler = groupHandler;
        PathEnumeratorStateList = new vector<PathEnumeratorState*>();
        PathGroupTree = NULL;
        mIsReset = true;
        Current = NULL;
    }

    PathEnumerator::~PathEnumerator() {
        Reset();
        delete mPath;
        delete PathEnumeratorStateList;
    }

    PathEnumeratorState* PathEnumerator::CurrentState() {
        vector<PathEnumeratorState*>::size_type count = PathEnumeratorStateList->size();
        if (count > 0) {
            return (*PathEnumeratorStateList)[mIsWidthFirstDescent ? 0 : count - 1];
        }
        return NULL;
    }

    void PathEnumerator::Reset() {
        mIsReset = true;
        Current = NULL;
        delete PathGroupTree;
        PathGroupTree = NULL;
        vector<PathEnumeratorState*>::iterator itr = PathEnumeratorStateList->begin();
        for(itr = PathEnumeratorStateList->begin();itr!=PathEnumeratorStateList->end();++itr) {
            delete *itr;
        }
        PathEnumeratorStateList->clear();
    }

    bool PathEnumerator::MoveNext() {
        // Pseudo algorithm:
        // 1) On Reset, take current pathnode and set up state on new stack.
        // 1a) Check if AnyPattern node, if so:
        //       push onto stack new path with path node WildPattern inserted before AnyPattern
        //       and cut AnyPattern from current path in stack (original path should be preserved).
        // 2) Get enumerator for node (a local check for match will be performed).
        // 3) If tail pathnode
        //       For each item, return item and make ready to be positioned to next.
        // 4) Else if not tail pathnode
        //       For each item, check if has children,
        //          if it does, push position, move to child path node and loop to 1).
        //          else skip item.
        // 5) Pop and discard top stack.
        // 6) If stack not empty, restore state from stack and loop to 3).
        // 7) Otherwise it is the end of enumeration.

        if (mIsReset) {
            Container* container = mPathCurrentContainer;
            PathNode* hn = mPath->HeadPathNode;
            if (hn == NULL) {
                return false;
            }
            mIsReset = false;
            // Descend down path as far as possible before an enumerator is needed.
            Container* c;
            Property* prop = mPath->GetProperty(container, mRequester, mMode, hn, c);
            if(prop==NULL) {
                return false;
            }
            GroupContainer* groupTree = GroupContainer::Factory(prop, hn, PathGroupTree);
            Property* v = prop->GetValue();
            if(v->Flags.IsContainer) {
                c = (Container*)v;
            }
            if (c!=NULL) {
                container = c;
            } else {
                Current = prop;
                return true;
            }
            // Prime the stack.
            PathEnumeratorStateList->push_back(new PathEnumeratorState(container, hn, hn->NodeKind==PathNodeKinds::Any,mRequester,mMode,groupTree,-1,false));
        }

        vector<PathEnumeratorState*>::size_type count;
        while ((count=PathEnumeratorStateList->size()) != 0) {
            PathEnumeratorState* state = (*PathEnumeratorStateList)[mIsWidthFirstDescent ? 0 : count - 1];
            if (state->CurrentPathNode->NodeKind==PathNodeKinds::Any) {
                // Set up two new path node chains:
                // A/AnyPattern/B ==> {tmpCut = A/B, tmpWild = A/WildPattern/AnyPattern/B}.
                PathNode* cn = state->CurrentPathNode;
                Container* container = state->CurrentContainer;
                PathEnumeratorStateList->erase(PathEnumeratorStateList->begin()+(mIsWidthFirstDescent ? 0 : count - 1));

                PathNode* tmpCut = new PathNode(*(cn->Head()));
                PathNode* tmpWild = new PathNode(*tmpCut);
                // Remove Any node from tmpCut.
                int anyIndex = cn->Index();
                PathNode* pAny = (*tmpCut)[anyIndex];
                PathNode* pCut = pAny->Next;
                pAny->Remove();
                delete pAny;
                // Insert Wild node into tmpWild.
                pAny = (*tmpWild)[anyIndex];
                PathNode* pWild = new PathNode(*Path::Wild);
                pAny->Insert(*pWild);

                // Need to make tmpCut top of stack, then tmpWild, depending on mIsWidthFirstDescent.
                GroupContainer* groupTree = state->PathGroupTree;
                PathEnumeratorState* wildState = new PathEnumeratorState(container, pWild, false, mRequester, mMode, groupTree,-1,true);
                PathEnumeratorState* cutState = new PathEnumeratorState(container, pCut, false, mRequester, mMode, groupTree,-1,true);
                delete state;
                if (mIsWidthFirstDescent) {
                    state = wildState;
                    PathEnumeratorStateList->insert(PathEnumeratorStateList->begin(),cutState);
                    PathEnumeratorStateList->insert(PathEnumeratorStateList->begin(),state);
                } else {
                    state = cutState;
                    PathEnumeratorStateList->push_back(wildState);
                    PathEnumeratorStateList->push_back(state);
                }
            }

            PathNode* currentNode = state->CurrentPathNode;

            if (state->CountRemaining!=0 && state->NodeEnumerator->MoveNext()) {
                --state->CountRemaining;
                PathNode* pn = currentNode->Next;
                GroupContainer* groupTree = state->PathGroupTree;
                Property* prop = state->NodeEnumerator->Current;
                if (pn == NULL) {
                    // Check for implicit Directors.
                    if(prop!=NULL && prop->Flags.IsContainer) {
                        Container* container = (Container*)prop;
                        if (container!=NULL && ((mMode & PropertyModes::Directing) != 0)) {
                            Property* q = container->DirectToProperty(NULL, mRequester, mRequester, mMode);
                            if (q != NULL) {
                                prop = q;
                            }
                        }
                    }
                    Current = prop;
                    groupTree->AddProperty(prop);
                    return true;
                } else {
                    Property* p = prop->GetValue();
                    if(p->Flags.IsContainer) {
                        Container* container = (Container*)p;
                        if (container!=NULL) {
                            if (pn->GroupBoundary) {
                                groupTree = groupTree->AddGroup(prop,currentNode->GroupNumber);
                            }
                            // Add child path node to stack.
                            state = new PathEnumeratorState(container, pn, false, mRequester, mMode, groupTree,-1,false);
                            if (mIsWidthFirstDescent && !pn->GroupBoundary) {
                                // Force non boundary nodes to be next one resolved.
                                PathEnumeratorStateList->insert(PathEnumeratorStateList->begin(), state);
                            } else {
                                // Add boundary nodes to end of stack and all nodes when depth first descent.
                                PathEnumeratorStateList->push_back(state);
                            }
                            continue;
                        }
                    }
                }
            }

            // Check if current state needs more processing.
            if (currentNode->GroupBoundary || currentNode->Parent==NULL || currentNode->Next==NULL) {
                // End of group boundary encountered, call GroupHandler.
                if (mGroupHandler != NULL && mGroupHandler->CheckGroup(state,this)) {
                    // Further traversal of state is required, so leave on stack.
                    continue;
                }
            }

            // Remove current state from stack and delete.
            count = PathEnumeratorStateList->size();
            if (count != 0) {
                PathEnumeratorStateList->erase(PathEnumeratorStateList->begin()+(mIsWidthFirstDescent ? 0 : --count));
            }
            delete state;
        }
        return false;
    }
}
