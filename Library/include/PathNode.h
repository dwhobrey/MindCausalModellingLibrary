#pragma once

namespace Plato {

    class Path;
    class Property;
    class Container;
    class Identifier;
    class IdentifierRegex;
    class ClassTypeInfo;
    class Arguments;
    class FilterArguments;
    class Point;
    class PathNodeEnumerator;

#pragma region // PathNodeKinds Enumeration.
    namespace PathNodeKinds {
        /// <summary>
        /// PathNodes have a meaning that depends on their form.
        /// This enumeration is used to indicate the semantics of a node.
        /// </summary>
        /// <remarks>
        /// The node's identifier is set to a value that simplifies
        /// logic elsewhere, such as enumerating over a PathNode chain.
        /// </remarks>
        enum Enum {
            /// <summary>
            /// The node is an identifier,
            /// whose string value is stored in the identifier.
            /// </summary>
            Normal,
            /// <summary>
            /// The node has a regex identifier, but is not the Any or Wild pattern.
            /// </summary>
            Regex,
            /// <summary>
            /// Node is the "Any" regex pattern: Path::AnyPattern.
            /// </summary>
            Any,
            /// <summary>
            /// Node is the "Wild" regex pattern: Path::WildPattern.
            /// </summary>
            Wild,
            /// <summary>
            /// The node is the root anchor: Path::DirectorySeparator.
            /// The identifier is set to empty.
            /// </summary>
            Absolute,
            /// <summary>
            /// The node is the relative anchor: Path.CurrentContainer.
            /// The identifier is set to the anchor string.
            /// </summary>
            Relative,
            /// <summary>
            /// The node is the parent anchor: Path.ParentContainer.
            /// The identifier is set to the anchor string.
            /// </summary>
            Parent,
            /// <summary>
            /// "~" Move to container created in.
            /// </summary>
            Owner,
            /// <summary>
            /// "!-" Search up (default) or down for named container.
            /// </summary>
            Search,
            /// <summary>
            /// "&amp;" Use a Director for route to property.
            /// </summary>
            Director,
            /// <summary>
            /// The node is a filter node.
            /// </summary>
            Filter,
            /// <summary>
            /// The node is the start of path anchor: Path.StartPattern.
            /// The identifier is set to the anchor string.
            /// </summary>
            Start,
            /// <summary>
            /// The node is the end of path anchor: Path.EndPattern.
            /// The identifier is set to the anchor string.
            /// </summary>
            End
        };
    }
    typedef PathNodeKinds::Enum PathNodeKindsEnum;
#pragma endregion

    /// <summary>
    /// Paths are internally represented as a chain of PathNodes.
    /// This class also defines operators for including filters in paths in
    /// a similar spirit to UNIX paths.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The components of a path may include filters for selecting specific subcontainers.
    /// Filters are introduced through "+" and "-" operators for specifying whether the filter results are to
    /// be included or excluded.
    /// A chain of filters may be specified.
    /// </para>
    /// <para>
    /// A Path, once created, is generally immutable, implying filter nodes are too.
    /// For example, PathNodes may be enumerated over many times, and
    /// concurrently, by Path enumerators. Hence, there are no copy constructors.
    /// </para>
    /// </remarks>
    class PathNode {
#pragma region // Class variables.
    public:
        /// <summary>
        /// Class type information.
        /// </summary>
        static const ClassTypeInfo* TypeInfo;
#pragma endregion
#pragma region // Variables.
    protected:
        /// <summary>
        /// Indicates that this node cannot be changed, i.e. it is immutable.
        /// </summary>
        bool mIsConst;
    public:
        /// <summary>
        /// Indicates when the node is potentially unbounded,
        /// i.e. a wild position was given.
        /// </summary>
        /// <remarks>
        /// Only Regex nodes can be unbounded.
        /// </remarks>
        bool IsUnbounded;

        /// <summary>
        /// Indicates that this node was preceeded by a group boundary.
        /// </summary>
        bool GroupBoundary;

        /// <summary>
        /// The group number of this node.
        /// In a chain of path nodes, the tail is group number zero,
        /// the parent nodes would have higher group numbers if separated by a
        /// group boundary.
        /// </summary>
        int GroupNumber;

        /// <summary>
        /// The function of this path node, e.g. a path anchor or container name.
        /// </summary>
        PathNodeKindsEnum NodeKind;

        /// <summary>
        /// The next node in the path node chain.
        /// </summary>
        PathNode* Next;

        /// <summary>
        /// The parent PathNode in the path node chain.
        /// </summary>
        PathNode* Parent;

        /// <summary>
        /// The Path object this node belongs to.
        /// </summary>
        Path* ParentPath;

        /// <summary>
        /// The property associated with this node when part of a Path node chain
        /// following a Path.GetProperty.
        /// </summary>
        /// <remarks>
        /// Note, this is set independently by methods such as Path.GetProperty,
        /// and may not always be set, or current. If required, users should
        /// access the value immediately after calling Path.GetProperty.
        /// </remarks>
        Property* NodeProperty;

#pragma region // Variables dependent on kind of PathNode.
        union {
            /// <summary>
            /// A name used to match against property names.
            /// </summary>
            const Identifier* Identity;
            /// <summary>
            /// A regex name used to match against property names.
            /// </summary>
            const IdentifierRegex* RegexIdentity;
        };
#pragma endregion

#pragma region // Variables for filter PathNodes.
    protected:
        friend class PathNodeEnumerator;
        /// <summary>
        /// The Type object for the filter being wrapped.
        /// </summary>
        const ClassTypeInfo* mFilterType;

        /// <summary>
        /// A path to a filter type or filter derived object, declared in a container via an alias.
        /// </summary>
        const Path* mFilterPath;
    public:
        /// <summary>
        /// The arguments to apply to filter constructor.
        /// </summary>
        Arguments* AdditionalArguments;
#pragma endregion
#pragma endregion

#pragma region // Constructors.
    public:
        /// <summary>
        /// Specialised constructor for creating the standard path nodes.
        /// </summary>
        /// <param name="isConst">Indicates node is immutable.</param>
        /// <param name="nodeKind">Indicates the kind of node.</param>
        /// <param name="nodeIdentifier">The name of this node.</param>
        PathNode(bool isConst, PathNodeKindsEnum nodeKind, const Identifier& nodeIdentifier);

        /// <summary>
        /// Specialised constructor for creating normal path nodes.
        /// </summary>
        /// <param name="nodeIdentifier">The name of this node.</param>
        PathNode(const Identifier& nodeIdentifier);

        /// <summary>
        /// Specialised constructor for creating new path nodes with string node name.
        /// </summary>
        /// <param name="nodeIdentifier">The name of this node: this may be a regex.</param>
        /// <param name="parent">The parent PathNode in the path node chain.</param>
        PathNode(const Identifier& nodeIdentifier, PathNode* parent);

        /// <summary>
        /// Specialised constructor for creating regex path nodes.
        /// </summary>
        /// <param name="regexIdentifier">A regex identifier.</param>
        /// <param name="parent">The parent PathNode in the path node chain.</param>
        PathNode(const IdentifierRegex& regexIdentifier, PathNode* parent);

        /// <summary>
        /// Copy constructor for PathNode.
        /// Creates a new path node by shallow copying path node members.
        /// </summary>
        /// <remarks>
        /// Deep copies Next and sets Parent accordingly.
        /// </remarks>
        /// <param name="pathNode">The path node to copy.</param>
        PathNode(const PathNode& pathNode);

        /// <summary>
        /// Construct a new filter node object that wraps the specified filter given by its type.
        /// </summary>
        /// <param name="filterType">The type of the filter to construct and use, must be derived from Filter.</param>
        /// <param name="arguments">The optional arguments to apply to the filter constructor.</param>
        PathNode(const ClassTypeInfo& filterType, Arguments* arguments);

        /// <summary>
        /// Construct a new filter node object that wraps the specified filter type given by a path.
        /// </summary>
        /// <param name="filterPath">The path to an alias that declares a filter type or Filter derived object.</param>
        /// <param name="arguments">The optional arguments to apply to the filter constructor, if required.</param>
        PathNode(const Path& filterPath, Arguments* arguments);

        /// <summary>
        /// Deallocated node.
        /// </summary>
        ~PathNode();

    protected:
        /// <summary>
        /// Clears the path node object.
        /// </summary>
        void Reset();
#pragma endregion

#pragma region // Accessors.
    public:
        /// <summary>
        /// Getter for the right most node in the path.
        /// It returns the PathNode object at the end of the chain.
        /// </summary>
        PathNode* Tail();

        /// <summary>
        /// Getter for the left most node in the path.
        /// It returns the PathNode object at the start of the chain.
        /// </summary>
        PathNode* Head();

        /// <summary>
        /// Returns the length of the path node chain.
        /// </summary>
        int Length();

        /// <summary>
        /// Returns the index of this path node in its parent chain.
        /// </summary>
        int Index();

        /// <summary>
        /// An indexer for a path node chain.
        /// The head of the chain is at index zero.
        /// </summary>
        /// <param name="index">The index of the path node to fetch.</param>
        /// <returns>Returns the required path node, or NULL if index out of range.</returns>
        PathNode* operator[](int index);
#pragma endregion

#pragma region // Public general methods.
    public:
        /// <summary>
        /// Remove this path node from chain.
        /// Also clears the node.
        /// </summary>
        void Remove();

        /// <summary>
        /// Insert path node into chain, before current node.
        /// Also adjusts Path.Head and Path.Path, if necessary.
        /// </summary>
        /// <remarks>Caller must update other variables accordingly.</remarks>
        /// <param name="pathNode">The path node to add.</param>
        void Insert(PathNode& pathNode);

        /// <summary>
        ///  Add path node to tail of chain.
        /// </summary>
        /// <remarks>Caller must update other variables accordingly.</remarks>
        /// <param name="pathNode">The path node to append.</param>
        void Append(PathNode& pathNode);

        /// <summary>
        /// Compares the Identifier of a path node for a regex match.
        /// </summary>
        /// <param name="regexPathNode">The regex path node whose Identifier is to be compared.</param>
        /// <returns>Returns true if this identifier matches regex identifier.</returns>
        bool IdentifierMatches(PathNode& regexPathNode);

        /// <summary>
        /// Updates the group numbers for the chain of path nodes in which this node is a member.
        /// </summary>
        void RefreshGroups();

        /// <summary>
        /// Fetches a mutable version of this node.
        /// Note, if this node is part of a chain, the user may need to copy the path too.
        /// </summary>
        /// <returns>Returns a mutable version of the node.</returns>
        PathNode* GetWritable() const;

        /// <summary>
        /// Represents the node details in symbolic string format.
        /// </summary>
        /// <returns>Returns a textual representation of node content.</returns>
        string& StatusReport();
#pragma endregion

#pragma region // Not so general methods.
    public:
        /// <summary>
        /// Gets an enumerator for the path node given the current container as context.
        /// </summary>
        /// <param name="currentContainer">The position in the container hierarchy to use as current, relative position.</param>
        /// <returns>Returns an enumerator over the elements referred to by the path node.</returns>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        PathNodeEnumerator* GetEnumerator(Container* currentContainer, const Property* requester, PropertyModesEnum mode);

        /// <summary>
        /// Constructs a new chain of PathNodes that leads to the container or property.
        /// </summary>
        /// <remarks>If both parameters are NULL, it returns the Absolute PathNode.</remarks>
        /// <param name="container">The parent container for the property, or NULL to use property's Parent.</param>
        /// <param name="prop">The property to construct a path for, or NULL.</param>
        static PathNode& GetPathNodes(Container* container, Property* prop);

        /// <summary>
        /// Fetch the property that this path node refers to.
        /// When PathNode is multivalued the context container is set, but NULL is returned.
        /// If an error occurs, or no property found, then NULL is returned for both the 
        /// context container and property value.
        /// </summary>
        /// <param name="nodeCurrentContainer">The relative position in the container hierarchy to use
        /// with the path node.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        /// <param name="contextContainer">The context container for the node is returned.</param>
        /// <returns>Returns the property this PathNode designates, or NULL if multivalued or an error occurs.</returns>
        Property* GetProperty(Container* nodeCurrentContainer, const Property* requester, PropertyModesEnum mode, Container*& contextContainer);

        /// <summary>
        /// Construct a new instance of the node's Filter.
        /// </summary>
        /// <remarks>
        /// Depending on how the filter node object was constructed,
        /// the Filter is created either from a Filter derived type or the type pointed to by a path.
        /// </remarks>
        /// <param name="filterArguments">The arguments to pass to filter factory.</param>
        /// <returns>Returns the new filter if successful, otherwise NULL.</returns>
        Container* CreateFilter(FilterArguments& filterArguments);
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
