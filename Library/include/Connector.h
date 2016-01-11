#pragma once

namespace Plato {

    class Path;
    class Property;
    class ClassTypeInfo;
    class Translator;
    class Transducer;
    class Container;
    class PathEnumerator;
    class PathEnumeratorState;
    class Identifier;
    class IdentifierRegex;
    class Pad;
    class InputPad;
    class OutputPad;
    class GroupContainer;

#pragma region // ConnectorPatterns enum.
    namespace ConnectorPatterns {
        /// <summary>
        /// This enum defines the multiplicity of the connections between sets of output and input terminii,
        /// e.g. 1 to many, many to 1, many to many.
        /// </summary>
        enum Enum {
            /// <summary>
            /// One connection between input and output.
            /// </summary>
            OneToOne,
            /// <summary>
            /// Many output terminii connecting to one input.
            /// </summary>
            FanIn,
            /// <summary>
            /// Many input terminii connected from one output.
            /// </summary>
            FanOut,
            /// <summary>
            /// A combination of FanIn and FanOut: a many to many relation.
            /// </summary>
            Star
        };
    }
    typedef ConnectorPatterns::Enum ConnectorPatternsEnum;
#pragma endregion

#pragma region // ConnectorTerminus class.
    /// <summary>
    /// A connector is defined by its two end points.
    /// This class holds the details for an end point.
    /// </summary>
    class ConnectorTerminus {
    public:
        /// <summary>
        /// The time at which terminus resolution should be attempted for this end point.
        /// For Dynamic connectors, the ConnectorTerminus path specifies
        /// which elements to monitor for birth or death events.
        /// </summary>
        ResolutionModesEnum ResolutionMode;
        /// <summary>
        /// The path that specifies the elements this end point refers to.
        /// </summary>
        Path* TerminusPath;

        /// <summary>
        /// Constructs a new connection end point with the given values.
        /// </summary>
        /// <param name="resolutionMode">The moment of connection resolution for the end point.</param>
        /// <param name="terminusPath">A path specifying the elements this end point refers to.</param>
        ConnectorTerminus(ResolutionModesEnum resolutionMode, Path& terminusPath);
        /// <summary>
        /// Produces a string representation that identifies this connector terminus.
        /// </summary>
        /// <returns>Returns a string that includes the terminus's path.</returns>
        string& StatusReport();
    };
#pragma endregion

#pragma region // Connector class.
    /// <summary>
    ///  The standard connector that matches on element paths.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The element interconnect is specified via rules in order to allow
    /// a dynamic topology.
    /// </para>
    /// <para>
    /// The endpoints of connections are specified via symbolic paths.
    /// Refs to elements are cached once they are determined and only re-resolved when
    /// the connection context (e.g. rules in force or DID population) changes.
    /// </para>
    /// </remarks>
    class Connector : public Property {
#pragma region // Class variables.
    public:
        /// <summary>
        /// Class type information.
        /// </summary>
        static const ClassTypeInfo* TypeInfo;
        /// <summary>
        /// Gets the class type info for the dynamic class instance type.
        /// </summary>
        virtual const ClassTypeInfo* GetClassTypeInfo() const { return TypeInfo; }
#pragma endregion

#pragma region // Instance parameter variables.
        /// <summary>
        /// The connection end point specifying the ending pads of the connection.
        /// </summary>
    protected:
        ConnectorTerminus* mTo;
        /// <summary>
        /// The connection end point specifying the starting pads of the connection.
        /// </summary>
        ConnectorTerminus* mFrom;
        /// <summary>
        /// The multiplicity of the connections.
        /// </summary>
        ConnectorPatternsEnum mConnectorPattern;
        /// <summary>
        /// A translator for mapping the source geometry to the destination geometry.
        /// </summary>
        Translator* mTranslator;
        /// <summary>
        /// A transducer for connecting incompatible Pads.
        /// </summary>
        Transducer* mTransducer;
#pragma endregion

#pragma region // Instance state variables.
        /// <summary>
        /// A cache of the current resolution mode.
        /// </summary>
        ResolutionModesEnum mResolutionMode;
        /// <summary>
        /// A cache of the container in which this connector is being applied.
        /// This doubles as the requester for scoping purposes.
        /// </summary>
        /// <remarks>
        /// This will normally be the same as the Parent container.
        /// </remarks>
        Container* mApplicationContainer;
        /// <summary>
        /// The enumerator over the To path.
        /// </summary>
        PathEnumerator* mToEnumerator;
        /// <summary>
        /// The enumerator over the From path.
        /// </summary>
        PathEnumerator* mFromEnumerator;
        /// <summary>
        /// Flag to indicate whether currently evaluating To or From path.
        /// </summary>
        bool mIsTo;
        /// <summary>
        /// A flag used to lock access to the To enumerator.
        /// </summary>
        bool mToLock;
        /// <summary>
        /// A flag used to lock access to the From enumerator.
        /// </summary>
        bool mFromLock;
#pragma endregion

#pragma region // Constructors.
    public:
        /// <summary>
        /// General constructor for creating a new connection rule.
        /// </summary>
        /// <param name="creator">The container that created this connector.</param>
        /// <param name="parent">The hierarchical parent of this connector.</param>
        /// <param name="elementName">The name of the element.</param>
        /// <param name="scope">The scope of the element.</param>
        /// <param name="to">The destination end point for the connection.</param>
        /// <param name="from">The source end point for the connection.</param>
        /// <param name="pattern">The multiplicity of the connections.</param>
        /// <param name="translator">A translator for mapping the source geometry to the destination geometry.</param>
        Connector(Container* creator, Container* parent, Identifier& elementName, PropertyScopesEnum scope, 
            ConnectorTerminus& to, ConnectorTerminus& from, ConnectorPatternsEnum pattern, Translator* translator=NULL);

        /// <summary>
        /// Standard constructor for creating a new connection rule.
        /// </summary>
        /// <param name="to">The destination end point for the connection.</param>
        /// <param name="from">The source end point for the connection.</param>
        /// <param name="pattern">The multiplicity of the connections.</param>
        /// <param name="translator">A translator for mapping the source geometry to the destination geometry.</param>
        Connector(ConnectorTerminus& to, ConnectorTerminus& from, ConnectorPatternsEnum pattern, Translator* translator=NULL);

        /// <summary>
        /// Convenience constructor for creating a new connection rule with default range and resolution values.
        /// </summary>
        /// <param name="to">A symbolic path specifying the source pads.</param>
        /// <param name="from">A symbolic path specifying the destination pads.</param>
        /// <param name="pattern">The multiplicity of the connections.</param>
        /// <param name="translator">A translator for mapping the source geometry to the destination geometry.</param>
        /// <remarks>
        /// The symbolic paths may contain filters or wildcards and be sensitive to runtime changes in context.
        /// </remarks>
        Connector(Path& to, Path& from, ConnectorPatternsEnum pattern, Translator* translator=NULL);
#pragma endregion

    public:
        /// <summary>
        /// Produces a string representation that identifies this connector.
        /// </summary>
        /// <returns>Returns a string that includes the connector's path and initialisation details.</returns>
        string& StatusReport();

    protected:
        /// <summary>
        /// Checks whether this connector requires resolving.
        /// </summary>
        /// <remarks>
        /// Note, if the resolution mode is "Final" and the connector has
        /// not been resolved yet (i.e. "Now" not called), it should be
        /// brought up to date.
        /// </remarks>
        /// <returns>Returns true if this connector requires resolving.</returns>
        virtual bool NeedsResolving();

        /// <summary>
        /// Build the group trees for the input and output paths.
        /// </summary>
        /// <returns>Returns true if there was an error.</returns>
        virtual bool BuildGroupTrees();

        /// <summary>
        /// Apply translators to pairs of terminal To and From groups.
        /// </summary>
        /// <returns>Returns true if there was an error.</returns>
        virtual bool ApplyTranslators();

        /// <summary>
        /// Add a connection between the To and From pads.
        /// </summary>
        /// <remarks>Checks if a transducer is required, otherwise calls To Pads Add method.</remarks>
        /// <param name="toPad">The destination of the connection.</param>
        /// <param name="fromPad">The source of the connection.</param>
        /// <returns>Returns true if there was an error.</returns>
        virtual bool AddConnection(InputPad& toPad, OutputPad& fromPad);

        /// <summary>
        /// Add One-to-One connections between a pair of To and From groups.
        /// </summary>
        /// <param name="toGroup">The destination group of Pads.</param>
        /// <param name="fromGroup">The source group of Pads.</param>
        /// <returns>Returns true if there was an error.</returns>
        virtual bool ConnectGroupsOneToOne(GroupContainer& toGroup, GroupContainer& fromGroup);

        /// <summary>
        /// Add Fan-In connections between a pair of To and From groups.
        /// </summary>
        /// <remarks>Each member of the To group receives inputs from all members of the From group.</remarks>
        /// <param name="toGroup">The destination group of Pads.</param>
        /// <param name="fromGroup">The source group of Pads.</param>
        /// <returns>Returns true if there was an error.</returns>
        virtual bool ConnectGroupsFanIn(GroupContainer& toGroup, GroupContainer& fromGroup);

        /// <summary>
        /// Add Fan-Out connections between a pair of To and From groups.
        /// </summary>
        /// <remarks>Each member of the From group sends outputs to all members of the To group.</remarks>
        /// <param name="toGroup">The destination group of Pads.</param>
        /// <param name="fromGroup">The source group of Pads.</param>
        /// <returns>Returns true if there was an error.</returns>
        virtual bool ConnectGroupsFanOut(GroupContainer& toGroup, GroupContainer& fromGroup);

        /// <summary>
        /// Add Star connections between a pair of To and From groups.
        /// </summary>
        /// <remarks>This implementation simply calls ConnectGroupsFanIn.</remarks>
        /// <param name="toGroup">The destination group of Pads.</param>
        /// <param name="fromGroup">The source group of Pads.</param>
        /// <returns>Returns true if there was an error.</returns>
        virtual bool ConnectGroupsStar(GroupContainer& toGroup, GroupContainer& fromGroup);

        /// <summary>
        /// Connect pairs of input and output terminal groups in group tree.
        /// </summary>
        /// <returns>Returns true if there was an error.</returns>
        virtual bool ConnectGroupTrees();

    public:
        /// <summary>
        /// Perform the connection operation.
        /// </summary>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="applicationContainer">The parent container for this connector.
        /// This doubles as the requester for scoping purposes.</param>
        virtual void Apply(ResolutionModesEnum resolutionMode, Container& applicationContainer);

        /// <summary>
        /// Checks the group is complete and in order.
        /// </summary>
        /// <remarks>
        /// This method is called by PathEnumerator when a group has been traversed.
        /// It checks the group contains the correct number of members and that
        /// they are in order. If additional elements are required the PathEnumeratorState
        /// state is adjusted. Unwanted elements are removed from the group
        /// tree and state list.
        /// <para>
        /// The necessary number of members must be added depending on the co-group's size
        /// and connector multiplicity (FanIn, FanOut, OneToOne).
        /// </para>
        /// </remarks>
        /// <param name="state">The current state of the PathEnumerator.</param>
        /// <param name="pathEnumerator">The PathEnumerator being evaluated.</param>
        /// <returns>Returns true if the current state should be re-applied.</returns>
        virtual bool CheckGroup(PathEnumeratorState* state, PathEnumerator* pathEnumerator);

        /// <summary>
        /// Determines the number of new members for a group during Connector Append resolution.
        /// </summary>
        /// <remarks>
        /// <para>Called by a Producer during unbounded size determination.</para>
        /// <para>
        /// This implementation simply returns the size of the co-group.
        /// Derived classes may want to return a size dependent on the regexIdentifier.
        /// </para>
        /// </remarks>
        /// <param name="container">The container to which the append is being applied.</param>
        /// <param name="regexIdentifier">The regex identifier that specified the append.</param>
        virtual int GetAppendCount(Container& container, const IdentifierRegex& regexIdentifier);

    protected:
        /// <summary>
        /// Determines the current group count for the specified connector path
        /// given by the current state of the Connector and corresponding PathEnumerator.
        /// </summary>
        /// <remarks>
        /// This method can only be called during Connector evaluation.
        /// </remarks>
        /// <param name="isTo">Indicates whether the group belongs to the To or From Connector path.</param>
        /// <returns>Returns the group count, or -1 if an error occurred.</returns>
        int GetGroupCount(bool isTo);

        /// <summary>
        /// Determines the group count for the specified group ID.
        /// </summary>
        /// <remarks>
        /// This method can only be called during Connector evaluation.
        /// The group's enumerator is invoked as necessary until it is complete.
        /// </remarks>
        /// <param name="isTo">Indicates whether the group belongs to the To or From Connector path.</param>
        /// <param name="groupID">The groupID of the required group.
        /// The ID is relative to the root of the group tree for the path.</param>
        /// <returns>Returns the group count, or -1 if an error occurred.</returns>
        int GetGroupCount(bool isTo, int* groupID);

        /// <summary>
        /// Checks that the specified group is complete.
        /// Invokes its enumerator as necessary.
        /// </summary>
        /// <remarks>
        /// This method can only be called during Connector evaluation.
        /// </remarks>
        /// <param name="isTo">Indicates whether the group belongs to the To or From Connector path.</param>
        /// <param name="group">The group to check for completion.</param>
        /// <returns>Returns true when complete, otherwise false indicating unable to complete group.</returns>
        bool CheckComplete(bool isTo, GroupContainer* group);

        /// <summary>
        /// Fetches the current group for the specified Connector path.
        /// </summary>
        /// <remarks>
        /// This method can only be called during Connector evaluation.
        /// </remarks>
        /// <param name="isTo">Indicates whether the group belongs to the To or From Connector path.</param>
        /// <returns>Returns the group, or NULL if an error occurred.</returns>
        GroupContainer* GetCurrentGroup(bool isTo);

        /// <summary>
        /// Fetches the group for the specified group ID.
        /// </summary>
        /// <remarks>
        /// This method can only be called during Connector evaluation.
        /// </remarks>
        /// <param name="isTo">Indicates whether the group belongs to the To or From Connector Path.</param>
        /// <param name="groupID">The groupID of the required group.
        /// The ID is relative to the root of the group tree for the path.</param>
        /// <returns>Returns the group, or NULL if an error occurred.</returns>
        GroupContainer* GetGroup(bool isTo, int* groupID);

        /// <summary>
        /// Invokes the specified connector path enumerator.
        /// </summary>
        /// <remarks>
        /// Checks for recursion or end of enumeration.
        /// </remarks>
        /// <param name="isTo">When true, the To enumerator is invoked, otherwise the From enumerator.</param>
        /// <param name="wasError">Returns true if a recursion lock occurs.</param>
        /// <returns>Returns true if able to move to next enumeration item.</returns>
        bool InvokeEnumerator(bool isTo, bool& wasError);

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
#pragma endregion
}
