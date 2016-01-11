#pragma once

namespace Plato {

    class Property;

    /// <summary>
    /// Director properties are used to route non-terminal property requests.
    /// </summary>
    /// <remarks>
    /// A director property is invoked in two circumstances:
    /// <list>
    /// <item>Implicitly, when an attempt is made to access a property via a non-terminal path.</item>
    /// <item>Explicitly in paths using the Director prefix.</item>
    /// </list>
    /// The director can either route the request to an element or ignore the access attempt.
    /// </remarks>
    class Director : public Property {

#pragma region // Fields.
    public:
        /// <summary>
        /// The path name to use for routing non-terminal input paths.
        /// </summary>
        static const string* InputName;
        /// <summary>
        /// The path name to use for routing non-terminal output paths.
        /// </summary>
        static const string* OutputName;
        /// <summary>
        /// The path name to use for routing
        /// non-terminal paths when neither input or output.
        /// </summary>
        static const string* DefaultName;
        /// <summary>
        /// The path name of the default director to use for routing
        /// non-terminal input paths.
        /// </summary>
        static const string* InputDirectorName;
        /// <summary>
        /// The path name of the default director to use for routing
        /// non-terminal output paths.
        /// </summary>
        static const string* OutputDirectorName;
        /// <summary>
        /// The path name of the default director to use for routing
        /// non-terminal paths when neither input or output.
        /// </summary>
        static const string* DefaultDirectorName;
#pragma endregion

        /// <summary>
        /// Construct a new director for the parent container.
        /// </summary>
        /// <param name="creator">The container that created this director.</param>
        /// <param name="parent">The hierarchical parent of this alias.</param>
        /// <param name="directorName">Name of this director.</param>
        /// <param name="scope">The application range of this director.</param>
        Director(Container* creator, Container* parent, Identifier& directorName, PropertyScopesEnum scope);

        /// <summary>
        /// Provides an ongoing route from the container and source to another terminus.
        /// </summary>
        /// <remarks>
        /// The TypeName and Position of the identifier refer to that of the required terminus property.
        /// </remarks>
        /// <param name="identifier">The Name of this Director, along with terminus Position and TypeName.
        /// If NULL, the router must determine position and type of terminus.</param>
        /// <param name="source">The property from which routing is required.</param>
        /// <param name="requester">The property that instigated the path, e.g. the source.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        /// <returns>Returns an ongoing path, or NULL if unable to route.</returns>
        virtual Property* Route(const Identifier* identifier, const Property* source, const Property* requester, PropertyModesEnum mode);

    private:
        friend class InitializerCatalogue;
        /// <summary>
        /// The class initializer.
        /// </summary>
        static void Initializer();
        /// <summary>
        /// The class finalizer.
        /// </summary>
        static void Finalizer();
    };
}
