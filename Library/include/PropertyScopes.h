#pragma once

namespace Plato {

    namespace PropertyScopes {
        /// <summary>
        /// The scope of a property of a container can be restricted to certain
        /// containers in the container hierarchy.
        /// </summary>
        enum Enum {
            /// <summary>
            /// Visible to all items in container hierarchy.
            /// </summary>
            Public,
            /// <summary>
            /// Visible to all items apart from containing container.
            /// </summary>
            External,
            /// <summary>
            /// Visible to all items apart from containing container
            /// and its descendents.
            /// </summary>
            Unrelated,
            /// <summary>
            /// Visible to containing container and its descendents.
            /// </summary>
            Protected,
            /// <summary>
            /// Visible to containing container and its properties only.
            /// </summary>
            Private,
            /// <summary>
            /// Visible to the containing container only.
            /// </summary>
            Self
        };
    }
    typedef PropertyScopes::Enum PropertyScopesEnum;
}
