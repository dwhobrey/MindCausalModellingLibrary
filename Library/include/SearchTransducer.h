#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Property;
    class Container;
    class Transducer;
    class Connector;
    class ConnectorRelator;
    class InputPad;
    class OutputPad;

    /// <summary>
    /// A search transducer searches the input and output paths for the first matching transducer.
    /// </summary>
    class SearchTransducer : public Transducer {
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

        /// <summary>
        /// Construct a new search transducer.
        /// </summary>
        /// <param name="name">Name of transducer.</param>
        /// <param name="scope">The application range of this transducer.</param>
        /// <param name="resolutionMode">The phase in the configuration process when this transducer should be applied.</param>
        /// <param name="elementRelator">A ConnectorRelator that specifies the two sets of element kinds that this transducer
        /// can connect in an N-M-arity relational sense.
        /// </param>
        SearchTransducer(Identifier& name, PropertyScopesEnum scope, ResolutionModesEnum resolutionMode, ConnectorRelator& elementRelator);

        /// <summary>
        /// Search for a transducer to be used for the given To and From Pads.
        /// </summary>
        /// <remarks>
        /// Firstly checks if this transducer instance matches,
        /// then looks along paths to find a transducer that matches on i/o pads.
        /// </remarks>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="parent">The parent container for this connector.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the transducer to fetch, or NULL if scope checking not required.</param>
        /// <param name="connector">The connector being used to connect elements.</param>
        /// <param name="toPad">The destination Pad for the connection.</param>
        /// <param name="fromPad">The source Pad for the connection.</param>
        /// <returns>Returns an appropriate instance of a transducer, or NULL if a transducer was not found.</returns>
        virtual Transducer* Factory(ResolutionModesEnum resolutionMode, Container& parent, const Property* requester, Connector& connector,
            InputPad& toPad, OutputPad& fromPad);

    protected:
        /// <summary>
        /// Look along path to find a transducer that matches on i/o pads.
        /// </summary>
        /// <param name="path">The path to search up.</param>
        /// <param name="resolutionMode">Indicates the current resolution mode.</param>
        /// <param name="parent">The parent container for this connector.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the transducer to fetch, or NULL if scope checking not required.</param>
        /// <param name="connector">The connector being used to connect elements.</param>
        /// <param name="toPad">The destination Pad for the connection.</param>
        /// <param name="fromPad">The source Pad for the connection.</param>
        /// <returns>Returns the first matching transducer, or NULL if no match found.</returns>
        Transducer* SearchPathForTransducer(Property& path, ResolutionModesEnum resolutionMode, Container& parent, const Property* requester,
            Connector& connector, InputPad& toPad, OutputPad& fromPad);

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
