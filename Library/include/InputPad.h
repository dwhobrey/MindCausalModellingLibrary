#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Property;
    class Container;
    class Connector;
    class Pad;
    class OutputPad;

    /// <summary>
    /// Models an input connection from an output pad.
    /// </summary>
    class InputConnection {
    public:
        /// <summary>
        /// The source of this connection.
        /// </summary>
        OutputPad* SourceOutputPad;
        /// <summary>
        /// The originating Connector for this connection.
        /// </summary>
        Connector* OriginatingConnector;

        /// <summary>
        /// General constructor for new InputConnections.
        /// </summary>
        /// <param name="outputPad">The source of the new connection.</param>
        /// <param name="connector">The originating Connector for this connection.</param>
        InputConnection(OutputPad& outputPad, Connector& connector);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~InputConnection();
    };

    /// <summary>
    /// Models an input pad for a connection.
    /// May need to save state info per input pad.
    /// </summary>
    class InputPad : public Pad {
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
        /// A list of the input connectors connecting to this pad.
        /// </summary>
        vector<InputConnection*>* InputConnections;

        /// <summary>
        /// Create a new input pad.
        /// </summary>
        /// <param name="creator">The element that created this pad.</param>
        /// <param name="parent">The hierarchical parent of this pad.</param>
        /// <param name="identity">The name to assign to the new pad.</param>
        /// <param name="scope">The visibility of the pad.</param>
        InputPad(Container* creator, Container& parent, Identifier& identity, PropertyScopesEnum scope);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~InputPad();

        /// <summary>
        /// Add a new connection.
        /// </summary>
        /// <param name="outputPad">The source of the new connection.</param>
        /// <param name="connector">The originating Connector for this connection.</param>
        /// <returns>Returns true if connection added successfully.</returns>
        bool Add(OutputPad& outputPad, Connector& connector);

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
