#include "PlatoIncludes.h"
#include "Regexes.h"
#include "Strings.h"
#include "ClassTypeInfo.h"
#include "EnvironmentVariables.h"
#include "Configuration.h"
#include "Identifier.h"
#include "PropertyModes.h"
#include "PropertyScopes.h"
#include "ConfigurePhases.h"
#include "Property.h"
#include "Container.h"
#include "Path.h"
#include "PathNode.h"
#include "Search.h"

namespace Plato {

    Property* Search::FindProperty(Path& propertyPath, SearchStrategiesEnum searchStrategy, 
                                    Container* currentContainer, const Property* requester, PropertyModesEnum mode) {
        Property* p = NULL;
        PathNode* n = propertyPath.HeadPathNode;
        // Check if path is a simple single identifier.
        if (n!=NULL && n->Next==NULL && n->NodeKind==PathNodeKinds::Normal) {
            const Identifier* identifier = propertyPath.GetTailIdentity();
            if (searchStrategy == SearchStrategies::Public) {
                // Begin by searching up container hierarchy for property name.
                Container* e = currentContainer;
                while (e != NULL && p == NULL) {
                    p = e->GetProperty(*identifier,requester,mode);
                    // Check scope.
                    if (p != NULL) {
                        if (p->Scope == PropertyScopes::Private && e != currentContainer) {
                            p = NULL;
                        }
                    }
                    if (p == NULL) {
                        e = e->Parent;
                    }
                }
            } else {
                // Must be Protected or Private strategy search.
                p = currentContainer->GetProperty(*identifier,requester,mode);
                // Everything would be in scope for current container.
            }

            if (p == NULL && (searchStrategy == SearchStrategies::Public 
                                    || searchStrategy == SearchStrategies::Protected)) {
                // Search standard places if hierarchy search fails.
                // DEBUG: TO DO: FindProperty: where should standard search places be located?
                // 1) in root container, or as a global?
                // Also need to allow per application properties etc.
                p = NULL; // mStandardProperties[name];
            }
        } else {
            // Case when non-simple, i.e. specific container in hierarchy, or regex.
            // Deal with Current and Absolute paths, plus combinations, e.g. "./..".
            Path* path = new Path(propertyPath);
            p = path->GetProperty(currentContainer,requester,mode);
        }
        return p;
    }
}
