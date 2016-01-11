#pragma once

namespace Plato {

    class Property;
    class Container;
    class Identifier;
    class IdentifierPtrHashCompare;
    class PathNode;
    class PathEnumerator;

    /// <summary>
    /// Models the path to a container. Similar conceptually to a file path.
    /// e.g. path="Mind/AttentionMechanism/Visual/Focus".
    /// Path node names may contain regular expressions.
    /// </summary>
    class Path {
#pragma region // Class variables.
    public:
        /// <summary>
        /// Class type information.
        /// </summary>
        static const ClassTypeInfo* TypeInfo;
#pragma endregion
    public:
        /// <summary>
        /// Type used to hold table of standard PathNodes.
        /// </summary>
        typedef hash_map<Strings::ConstStringPtr,const PathNode*, StringHashArgs> HashMap;

#pragma region // Fields.
    public:
        /// <summary>
        /// The character used to separate container levels, such as directories.
        /// The Plato library adopts this character independent of platform
        /// since the binary division operator "/" is used in the Path class and Hierarchy.
        /// </summary>
        static const char DirectorySeparator = '/';

        /// <summary>
        /// The character used to separate container levels whilst marking a group boundary.
        /// </summary>
        static const char GroupSeparator = '|';

        /// <summary>
        /// The two directory separators, used when spliting paths into nodes.
        /// </summary>
        static const string* DirectoryAndGroupSeparatorString;

        /// <summary>
        /// This regex pattern matches an imaginary parent to the head of the path.
        /// </summary>
        static const char StartPattern = '^';

        /// <summary>
        /// This regex pattern matches an imaginary child to the tail of the path.
        /// </summary>
        static const char EndPattern = '$';

        /// <summary>
        /// The identifer used to indicate path node refers to the parent container.
        /// </summary>
        static const char* ParentContainer;

        /// <summary>
        /// The identifer used to indicate path node refers to the current container.
        /// </summary>
        static const char CurrentContainer = '.';

        /// <summary>
        /// The character used to denote the owner container in paths.
        /// </summary>
        static const char OwnerContainer = '~';

        /// <summary>
        /// The character used to denote the director prefix in paths.
        /// </summary>
        static const char DirectorPrefix = '&';

        /// <summary>
        /// The character used to denote the search prefix in paths.
        /// </summary>
        static const char SearchPrefix = '!';

        /// <summary>
        /// The operator used to introduce filters in paths.
        /// </summary>
        static const char FilterOperator = '+';

        /// <summary>
        /// Standard PathNode used to indicate path starts from the root container.
        /// </summary>
        static const PathNode* Absolute;
        /// <summary>
        /// Standard PathNode used to indicate the node refers to the current container.
        /// The current container is the (enclosing) container from which the path is applied,
        /// i.e. at the point when a path method is invoked. This may be different to the Creator container.
        /// </summary>
        static const PathNode* Relative;

        /// <summary>
        /// Standard PathNode used to indicate the node refers to the parent of the current container.
        /// </summary>
        static const PathNode* Parent;

        /// <summary>
        /// Standard PathNode used to indicate the node refers to the container it was created in.
        /// </summary>
        static const PathNode* Owner;

        /// <summary>
        /// Standard PathNode used to indicate the node refers to the regex match on any number of levels.
        /// </summary>
        static const PathNode* Any;

        /// <summary>
        /// Standard PathNode used to indicate the node refers to the regex match on any identifier.
        /// </summary>
        static const PathNode* Wild;

        /// <summary>
        /// Standard PathNode used to indicate the node refers to the regex match on the start of the path.
        /// </summary>
        static const PathNode* Start;

        /// <summary>
        /// Standard PathNode used to indicate the node refers to the regex match on the end of the path.
        /// </summary>
        static const PathNode* End;
#pragma endregion

#pragma region // Class variables.
    private:
        /// <summary>
        /// Hashtable for converting from a standard PathNode name to it's PathNode object.
        /// </summary>
        static HashMap* mStandardPathNodes;

        /// <summary>
        /// An immutable copy of the base dir.
        /// </summary>
        static string* mPathPrefix;
#pragma endregion
        
#pragma region // Instance variables.
    public:
        /// <summary>
        /// The path broken into a chain of PathNodes.
        /// </summary>
        PathNode* HeadPathNode;

        /// <summary>
        /// Stores a reference to the object's creator.
        /// </summary>
        Container* Creator;
#pragma endregion

#pragma region // Constructors.
    public:
        /// <summary>
        /// Construct a new Path starting at the PathNode.
        /// If the PathNode is at the Head of the chain, or is not PathNodeKinds::Normal,
        /// an absolute path is constructed, otherwise a relative path is constructed.
        /// </summary>
        /// <remarks>
        /// The PathNode chain is reused. Caller must copy chain first if necessary.
        /// To create a path for a property use constructor with PathNode.GetPathNodes().
        /// </remarks>
        /// <param name="creator">The object that created this object.</param>
        /// <param name="pn">A PathNode in a chain, from which to construct the new path.</param>
        Path(Container* creator, PathNode& pn);

        /// <summary>
        /// Copy constructor: creates a new Path by deep copying from path argument.
        /// </summary>
        /// <remarks>This deep copies the PathNode chain.</remarks>
        /// <param name="path">The path to copy.</param>
        Path(const Path& path);

        /// <summary>
        /// Release resources.
        /// </summary>
        ~Path();
#pragma endregion

#pragma region // Accessors.
    public:
        /// <summary>
        /// Getter for the left most node in the path.
        /// </summary>
        /// <returns>Returns the PathNode object at the end of the chain.</returns>
        PathNode* Tail();

        /// <summary>
        /// Getter for the name of this path. This is the name of the tail path node.
        /// </summary>
        const Identifier* GetTailIdentity();

        /// <summary>
        /// Accessor for the path prefix to add to paths when accessing
        /// the host FS.
        /// </summary>
        static string* GetPathPrefix();
#pragma endregion

#pragma region // Private and public methods.
    public:
        /// <summary>
        /// Traverses down the path trunk to the terminal leaf property,
        /// or the first container whose path node branches,
        /// i.e. contains filters or regex.
        /// </summary>
        /// <param name="currentContainer">The position in the container hierarchy to use as the current, relative position.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        /// <param name="pathNode">Out parameter for the PathNode corresponding to the first branch container or leaf object.</param>
        /// <param name="contextContainer">Out parameter for the parent container of the property.</param>
        /// <returns>Returns the first branch container, or property if no branch found.</returns>
        Property* GetProperty(Container* currentContainer, const Property* requester, PropertyModesEnum mode,
                PathNode*& pathNode, Container*& contextContainer) const;

        /// <summary>
        /// Gets the property this path resolves to.
        /// </summary>
        /// <param name="currentContainer">The position in the container hierarchy to use as current context.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        /// <returns>Returns the property, or NULL if path cannot be resolved or is multivalued.</returns>
        Property* GetProperty(Container* currentContainer, const Property* requester, PropertyModesEnum mode) const;

        /// <summary>
        /// Gets an enumerator for the path given the current container as context.
        /// </summary>
        /// <param name="currentContainer">The position in the container hierarchy to use as current, relative position.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        /// <returns>Returns an enumerator over the elements referred to by the path.</returns>
        PathEnumerator* GetEnumerator(Container* currentContainer, const Property* requester, PropertyModesEnum mode);

        /// <summary>
        /// Returns the standard path node if the name matches the name of one of the standard nodes.
        /// </summary>
        /// <param name="name">The name of a standard node.</param>
        /// <returns>Returns the standard path node for that name, or NULL if not found.</returns>
        static const PathNode* GetStandardNode(const string* name);

        /// <summary>
        /// Append a PathNode to this path and refresh flags and string value.
        /// </summary>
        /// <param name="node">The node to append to this path. Does nothing if NULL.</param>
        void Append(PathNode* node);

        /// <summary>
        /// Compares two absolute paths alphabetically.
        /// </summary>
        /// <param name="absolutePath"></param>
        /// <returns>Returns &lt; 0, if this &lt; obj; 0 if this == obj;
        /// &gt; 0, if this &gt; obj. </returns>
        int CompareTo(const Path& absolutePath) const;

        /// <summary>
        /// Compares two paths alphabetically, allowing either to be NULL.
        /// </summary>
        /// <param name="a">The first path to compare.</param>
        /// <param name="b">The second path to compare.</param>
        /// <returns>Returns &lt; 0, if a &lt; b; 0 if a == b; &gt; 0, if a &gt; b.</returns>
        static int Compare(const Path* a, const Path* b);

        /// <summary>
        /// Converts a path in non-normal form, e.g. contains prefixes, to absolute form.
        /// </summary>
        /// <param name="currentContainer">The current container for resolving any prefixes found.</param>
        /// <returns>Returns either the original path (if already in normal form) or a new path that
        /// is in normal form. Regex and search nodes are not resolved.</returns>
        Path* ToAbsolute(Container* currentContainer);

        /// <summary>
        /// Represents the path details in symbolic string format.
        /// </summary>
        /// <returns>Returns a textual representation of path content, see PathNode.ToString.</returns>
        string& StatusReport() const;

    private:
        /// <summary>
        /// Sets the following flags:
        /// 1) Path property of each PathNode to this.
        /// </summary>
        void RefreshFlags();
#pragma endregion

#pragma region // Static helper methods for working with Paths.
    public:
        /// <summary>
        /// Extracts the property name from the path, if any. 
        /// Strips any trailing directory separator first.
        /// </summary>
        /// <param name="path">The path to split into directory:propertyname.</param>
        /// <returns>Returns the property name for the path, or NULL if no name found.</returns>
        static string* GetPropertyName(const string& path);

        /// <summary>
        /// Extracts the directory prefix from the path, if any.
        /// </summary>
        /// <param name="path">The path to split into directory:propertyname.</param>
        /// <returns>Returns the directory information for the path.</returns>
        static string& GetContainerPath(const string& path);

        /// <summary>
        /// Extracts the property name and container path.
        /// </summary>
        /// <param name="path">The path to split into containerpath:propertyname.</param>
        /// <param name="containerPath">Out parameter, which is the container path prefix.</param>
        /// <returns>Returns the property name for the path, or NULL if no name found.</returns>
        static string* GetPropertyNameAndContainerPath(const string& path, string*& containerPath);
#pragma endregion

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
