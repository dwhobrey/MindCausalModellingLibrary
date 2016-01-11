#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Property;
    class Container;
    class GroupContainer;
    class Connector;
    class ConnectorRelator;
    class InputPad;
    class OutputPad;

    /// <summary>
    /// A transducer is applied between two sets of elements that are to be interconnected
    /// when the sets match the transducer's relator.
    /// A transducer may be applied multiple times to each set of elements in a container
    /// that matches the relator.
    /// </summary>
    /// <remarks>
    /// </remarks>
    class Transducer : public Container {
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
        /// <summary>
        /// The kinds of properties this container can hold.
        /// </summary>
        static const ClassTypeInfo::HashSet* PropertyTypes;
#pragma endregion
        /// <summary>
        /// The phase in the configuration process that this transducer should be applied.
        /// </summary>
        ResolutionModesEnum ResolutionMode;

        /// <summary>
        /// The relator that determines the elements this transducer applies to.
        /// </summary>
        ConnectorRelator* ElementRelator;

        /// <summary>
        /// Construct a new transducer.
        /// </summary>
        /// <param name="name">Name of transducer.</param>
        /// <param name="scope">The application range of this transducer.</param>
        /// <param name="resolutionMode">The phase in the configuration process when this transducer should be applied.</param>
        /// <param name="elementRelator">A ConnectorRelator that specifies the two sets of element kinds that this transducer
        /// can connect in an N-M-arity relational sense.
        /// </param>
        Transducer(Identifier& name, PropertyScopesEnum scope, ResolutionModesEnum resolutionMode,
            ConnectorRelator& elementRelator);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~Transducer();

        /// <summary>
        /// Create an instance of this transducer to be used for the given To and From Pads.
        /// </summary>
        /// <remarks>
        /// This default implementation calls the Matches method to check whether this instance
        /// can handle the To and From Pad transduction. If it can the current instance is returned.
        /// User derived classes may want to create a new instance per transduction.
        /// However, it is preferable to make transducer instances immutable and hence reusable.
        /// </remarks>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="parent">The parent container for this connector.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the transducer to fetch, or NULL if scope checking not required.</param>
        /// <param name="connector">The connector being used to connect elements.</param>
        /// <param name="toPad">The destination Pad for the connection.</param>
        /// <param name="fromPad">The source Pad for the connection.</param>
        /// <returns>Returns an appropriate instance of the transducer, or NULL if transduction not supported.</returns>
        virtual Transducer* Factory(ResolutionModesEnum resolutionMode, Container& parent, const Property* requester, 
            Connector& connector, InputPad& toPad, OutputPad& fromPad);

        /// <summary>
        /// Group transducer Apply method.
        /// Add conversion rules (etc) to owner element as necessary
        /// in order to connect two groups of elements.
        /// User derived class would then add sub elements as necessary.
        /// </summary>
        /// <remarks>
        /// DEBUG: Either transducer or caller should remove transduced elements from subsequent interconnect by the connector.
        /// </remarks>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="parent">The parent container for this connector.</param>
        /// <param name="connector">The connector being used to connect elements.</param>
        /// <param name="inputs">List of input elements to connect to output elements.</param>
        /// <param name="outputs">List of output elements to connect to input elements.</param>
        virtual void GroupApply(ResolutionModesEnum resolutionMode, Container& parent, Connector& connector, 
            GroupContainer& inputs, GroupContainer& outputs);

        /// <summary>
        /// Pair transducer Apply method.
        /// Add conversion rules (etc) to owner element as necessary
        /// in order to connect a pair of elements.
        /// User derived class would then add sub elements as necessary.
        /// </summary>
        /// <remarks>
        /// DEBUG: Either transducer or caller should remove transduced elements from subsequent interconnect by the connector.
        /// </remarks>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="parent">The parent container of the connector.</param>
        /// <param name="connector">The connector being used to connect elements.</param>
        /// <param name="input">The input element that will recieve the transduced output element.</param>
        /// <param name="output">The output element to transduce to the input element.</param>
        virtual void Apply(ResolutionModesEnum resolutionMode, Container& parent, Connector& connector,
            Property& input, Property& output);

        /// <summary>
        /// Group matching method.
        /// Determines whether this transducer applies to the (to,from) pair groups.
        /// </summary>
        /// <remarks>This default implementation applies the relator.</remarks>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="parent">The parent container of the connector.</param>
        /// <param name="connector">The connector being used to connect elements.</param>
        /// <param name="inputs">The destination elements for the transduced connection.</param>
        /// <param name="outputs">The source elements for the transduced connection.</param>
        /// <returns>Returns true if this transducer is applicable.</returns>
        virtual bool GroupMatches(ResolutionModesEnum resolutionMode, Container& parent, Connector& connector,
            GroupContainer& inputs, GroupContainer& outputs);

        /// <summary>
        /// Pair matching method.
        /// Determines whether this transducer applies to the (to,from) pair.
        /// </summary>
        /// <remarks>This default implementation applies the relator.</remarks>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="parent">The parent container of the connector.</param>
        /// <param name="connector">The connector being used to connect elements.</param>
        /// <param name="input">The destination for the transduced connection.</param>
        /// <param name="output">The source for the transduced connection.</param>
        /// <returns>Returns true if this transducer is applicable.</returns>
        virtual bool Matches(ResolutionModesEnum resolutionMode, Container& parent, Connector& connector,
            Property& input, Property& output);

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
