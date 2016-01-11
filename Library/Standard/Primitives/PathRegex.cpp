#include "PlatoIncludes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "Identifier.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "Property.h"
#include "ConfigurePhases.h"
#include "Container.h"
#include "PathNode.h"
#include "Path.h"
#include "PathRegex.h"

namespace Plato {

    PathRegex::CompareToRegexState::CompareToRegexState(Path* thisPath, Path* regexPath, Container* thisPosition, 
            Container* regexPosition) {
        mThisPath = thisPath;
        mRegexPath = regexPath;
        mThisPosition = thisPosition;
        mRegexPosition = regexPosition;
    }

    bool PathRegex::CompareToRegex(Property* prop, Path* regexPath, const Property* requester, PropertyModesEnum mode) {
        if (prop == NULL) {
            return (regexPath==NULL);
        }
        return CompareToRegex(&prop->GetPath(), regexPath, prop->Parent, NULL, requester, mode);
    }

    bool PathRegex::CompareToRegex(Path* fixedPath, Path* regexPath, Container* currentContainer, 
                                   PathNode* position, const Property* requester, PropertyModesEnum mode) {
        if (fixedPath == NULL && regexPath == NULL) {
            return true;
        }
        if (fixedPath == NULL || regexPath == NULL) {
            return false;
        }
        if (position == NULL) {
            position = fixedPath->HeadPathNode;
        }

        // Algorithm:
        // Let P = (Q=fixedPath|R=regexPath).
        // 0) Copy paths and work on copies.
        // 1) If P contains Search, unlazily find search node.
        //       Repeat until it contains no more Search nodes.
        //       ==> Find last search node, cut after it and evaluate path, then postfix remainder.
        // 2) Check if paths need converting to normal form (i.e. all PathNode's kind=Normal):
        //    i.e. expand prefixes. Note the following exceptions:
        //    a) If both paths have same prefix it can be cut: e.g. ., .. .
        //    b) If R prefix is "." or "..", convert R to normal form using parent nodes from Q.
        //    Descend to next PathNode and loop until NULL or Any node encountered.
        // 3) Compare PathNode chains according fullMatch flags.
        //    a) If an Any node is encountered, cut and stack:
        //       map P = A/Any/B to P = A/B, and stack path copies (A/Wild/Any/B, Q|R, position).
        //       Convert new P to normal form (similar to 2) from cut position and continue with 3).
        //    b) Return true if chain exhausted as per regex anchor nodes '^' and '$'.
        // 4) If stack not empty, pop (Q,R, position) and loop to 3).

        stack<CompareToRegexState*> & stateStack = *new stack<CompareToRegexState*>();

        // 0) Copy paths.
        Path* q = new Path(fixedPath->Creator,*position);
        Path* r = new Path(*regexPath);
        Container* qCurrent = currentContainer;
        Container* rCurrent = currentContainer;

        // 1) Remove any search nodes and reset current container if necessary.
        PerformSearch(qCurrent, q,requester,mode);
        PerformSearch(rCurrent, r,requester,mode);

        for(;;) {
            // 2) Check for possible match and chop matching nodes down to first Any.
            if (CutMatchingNodes(qCurrent, q, rCurrent, r, requester)) {
                // A possible match.
                // Expand separately as needed.
                PathNode* qAny = ExpandPath(qCurrent, q, requester);
                PathNode* rAny = ExpandPath(rCurrent, r, requester);

                // 3a) Check for Any, cut and stack accordingly.
                if (StackAny(qAny, q, r, qCurrent, rCurrent, false, stateStack)) {
                    // Loop to 2).
                    continue;
                }
                if (StackAny(rAny, q, r, qCurrent, rCurrent, true, stateStack)) {
                    // Loop to 2).
                    continue;
                }
                // 3b) Compare nodes according to fullMatch flag.
                if (ComparePaths(q, r)) {
                    return true;
                }
            }
            // 4) Current paths do not match, so check stack for other paths to compare.
            if (stateStack.size() > 0) {
                CompareToRegexState* state = stateStack.top();
                stateStack.pop();
                q = state->mThisPath;
                r = state->mRegexPath;
                qCurrent = state->mThisPosition;
                rCurrent = state->mRegexPosition;
            } else {
                break;
            }
        }

        return false;
    }

    bool PathRegex::ComparePaths(Path* q, Path* r) {
        if (q == NULL && r == NULL) {
            return true;
        }
        if (q == NULL || r == NULL) {
            return false;
        }
        PathNode* qn = q->HeadPathNode;
        PathNode* rn = r->HeadPathNode;
        PathNode* tmp = r->Tail();
        // When true, all nodes must match, otherwise all nodes in r must match first nodes of q.
        bool fullMatch = (tmp!=NULL && tmp->NodeKind==PathNodeKinds::End);

        while (qn != NULL && rn != NULL) {
            PathNodeKindsEnum nodeKind = qn->NodeKind;

            if (rn->NodeKind == PathNodeKinds::Any || qn->NodeKind == PathNodeKinds::Any)
                return false;

            if (rn->NodeKind != nodeKind)
                return false;

            switch (nodeKind) {
                case PathNodeKinds::Absolute:
                case PathNodeKinds::Relative:
                case PathNodeKinds::Parent:
                case PathNodeKinds::Owner:
                    break;
                case PathNodeKinds::Normal:
                    if (!qn->IdentifierMatches(*rn)) {
                        return false;
                    }
                    break;
                default:
                    return false;
            }
            qn = qn->Next;
            rn = rn->Next;
        }
        bool rnEnd = (rn == NULL || rn->NodeKind==PathNodeKinds::End);
        bool qnEnd = (qn == NULL || qn->NodeKind==PathNodeKinds::End);
        if (qnEnd && rnEnd) {
            return true;
        }
        if (!fullMatch && rnEnd) {
            return true;
        }
        return false;
    }

    bool PathRegex::StackAny(PathNode* pAnyNode, Path* q, Path* r, Container* qCurrent, Container* rCurrent, 
            bool isRegex, stack<CompareToRegexState*> & stateStack) {
        if (pAnyNode != NULL) {
            // Create a new path and cut Any from current path:
            // AnyPattern/B ==> {p = B, wildAny = WildPattern/AnyPattern/B}.
            Path* p = isRegex ? r : q;
            PathNode* wildAny = new PathNode(*(p->HeadPathNode));
            PathNode* pAny = (*wildAny)[pAnyNode->Index()];
            pAny->Insert(*new PathNode(*Path::Wild));
            pAnyNode->Remove();
            p = new Path(q->Creator,*wildAny);
            stateStack.push(new CompareToRegexState(isRegex ? q : p, isRegex ? p : r, qCurrent, rCurrent));
            return true;
        }
        return false;
    }

    bool PathRegex::CutMatchingNodes(Container* & qCurrent, Path* & q, Container* & rCurrent, Path* & r, const Property* requester) {
        PathNode* qn = q->HeadPathNode;
        PathNode* rn = r->HeadPathNode;
        while (qn != NULL && rn != NULL) {
            PathNodeKindsEnum nodeKind = qn->NodeKind;

            if (rn->NodeKind == PathNodeKinds::Any || qn->NodeKind == PathNodeKinds::Any) {
                break;
            }

            if (rn->NodeKind != nodeKind) {
                break;
            }

            switch (nodeKind) {
                case PathNodeKinds::Parent:
                    if (qCurrent != NULL) {
                        qCurrent = qCurrent->Parent;
                    }
                    if (rCurrent != NULL) {
                        rCurrent = rCurrent->Parent;
                    }
                    qn->Remove();
                    rn->Remove();
                    break;
                case PathNodeKinds::Absolute:
                case PathNodeKinds::Relative:
                case PathNodeKinds::Owner:
                case PathNodeKinds::Start:
                case PathNodeKinds::End:
                    qn->Remove();
                    rn->Remove();
                    break;
                case PathNodeKinds::Normal:
                    if (qn->IdentifierMatches(*rn)) {
                        qn->Remove();
                        rn->Remove();
                    } else {
                        return false;
                    }
                    break;
                case PathNodeKinds::Director:
                    if (rn->NodeKind == qn->NodeKind) {
                        if (qn->IdentifierMatches(*rn)) {
                            qn->Remove();
                            rn->Remove();
                        } else {
                            return false;
                        }
                    } else {
                        // Different node kinds: expand.
                        q = ExpandNode(qCurrent, qn, requester);
                        r = ExpandNode(rCurrent, rn, requester);
                    }
                    break;
                default:
                    // What about Filters?
                    break;
            }
            if (q == NULL || r == NULL) {
                break;
            }
            qn = q->HeadPathNode;
            rn = r->HeadPathNode;
        }
        return true;
    }

    PathNode* PathRegex::ExpandPath(Container* & currentContainer, Path* & path, const Property* requester) {
        if (path == NULL) {
            return NULL;
        }
        PathNode* node = path->HeadPathNode;
        while (node != NULL) {
            if (node->NodeKind == PathNodeKinds::Any) {
                return node;
            }
            switch(node->NodeKind) {
                case PathNodeKinds::Normal:
                case PathNodeKinds::Absolute:
                case PathNodeKinds::End:
                case PathNodeKinds::Start:
                    node = node->Next;
                    continue;
                default:
                    break;
            }
            path = ExpandNode(currentContainer, node, requester);
            if (path == NULL) {
                break;
            }
        }
        return NULL;
    }

    Path* PathRegex::ExpandNode(Container* & currentContainer, PathNode* & node, const Property* requester) {
        PathNode* next = node->Next;
        switch (node->NodeKind) {
            case PathNodeKinds::Relative:
                node->Remove();
                node = next;
                return node->ParentPath;
            case PathNodeKinds::Parent: {
                    PathNode* qp = node->Parent;
                    if (qp != NULL) {
                        if (currentContainer != NULL)
                        currentContainer = currentContainer->Parent;
                        node->Remove();
                        node = next;
                        return node->ParentPath;
                    }
                    if (currentContainer != NULL) {
                        currentContainer = currentContainer->Parent;
                    }
                }
                break;
            case PathNodeKinds::Owner:
                currentContainer = node->ParentPath->Creator;
                break;
            case PathNodeKinds::Director:
                break;
            default:
                return node->ParentPath;
        }
        Path* path = new Path(currentContainer,PathNode::GetPathNodes(currentContainer, NULL));
        path->Append(next);
        node = next;
        return path;
    }

    void PathRegex::PerformSearch(Container* & currentContainer, Path* & path, const Property* requester, PropertyModesEnum mode) {
        if (path == NULL) {
            return;
        }
        // Find the last search node.
        PathNode* n = path->HeadPathNode;
        PathNode* s = NULL;
        while (n != NULL) {
            if (n->NodeKind == PathNodeKinds::Filter) {
                // DEBUG: TO DO: Check if a search filter.
                s = n;
            }
            n = n->Next;
        }
        // Chop off any nodes after last search node.
        n = s->Next;
        s->Next = NULL;
        // Evaluate path up to search node.
        PathNode* branch = NULL;
        Container* c;
        path->GetProperty(currentContainer, requester, mode, branch, c);
        // Construct a new path for c / p.
        path = new Path(currentContainer,PathNode::GetPathNodes(c, NULL));
        // Add old tail.
        path->Append(n);
        currentContainer = NULL;
        return;
    }
}
