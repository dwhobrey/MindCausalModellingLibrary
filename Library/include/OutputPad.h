#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Property;
    class Container;
    class Pad;

    /// <summary>
    /// Models an output pad.
    /// </summary>
    /// <remarks>
    /// Handles buffering of output values during emulation Update.
    /// Source of output value alternates between two buffers.
    /// The buffers are swapped when the emulation epoch number changes.
    /// The emulation epoch number should be changed at the start of each emulation Update.
    /// Note: the DID may want to store some state info per output pad,
    /// so we'd need to add a data member.
    /// </remarks>
    class OutputPad : public Pad {
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
        /// Takes a value of 0 or 1, set via SetBuffer.
        /// </summary>
    private:
        static int CurrentBuffer;

        /// <summary>
        /// A binary buffer for DID output values.
        /// </summary>
        float OutputValue[2];

    public:
        /// <summary>
        /// Create a new output pad.
        /// </summary>
        /// <param name="creator">The element that created this pad.</param>
        /// <param name="parent">The hierarchical parent of this pad.</param>
        /// <param name="identity">The name to assign to the new pad.</param>
        /// <param name="scope">The visibility of the pad.</param>
        OutputPad(Container* creator, Container& parent, Identifier& identity, 
            PropertyScopesEnum scope = PropertyScopes::Public);

        /// <summary>
        /// Create a new output pad.
        /// </summary>
        /// <param name="parent">The element that created this pad and is the parent.</param>
        /// <param name="identity">The name to assign to the new pad.</param>
        /// <param name="scope">The visibility of the pad.</param>
        OutputPad(Container& parent, Identifier& identity, 
            PropertyScopesEnum scope = PropertyScopes::Public);

        /// <summary>
        /// Create a new output pad.
        /// </summary>
        /// <param name="parent">The element that created this pad and is the parent.</param>
        /// <param name="identity">The name to assign to the new pad. Must not be NULL.
        /// A new Identifier is created to wrap name.</param>
        /// <param name="scope">The visibility of the pad.</param>
        OutputPad(Container& parent, const char* identity, 
            PropertyScopesEnum scope = PropertyScopes::Public);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~OutputPad();

        /// <summary>
        /// Sets the current output buffer.
        /// </summary>
        /// <param name="epochNumber">Used to set current buffer number to (epochNumber mod 2).</param>
        inline static void SetBuffer(long epochNumber) { CurrentBuffer = (int)(epochNumber & 1); }

        /// <summary>
        /// The GetOutputValue always uses the opposite buffer to the SetValue.
        /// </summary>
        /// <returns>Returns the current output value.</returns>
        inline float GetOutputValue() { return OutputValue[CurrentBuffer]; }

        /// <summary>
        /// Set output value. Takes effect next epoch.
        /// </summary>
        /// <param name="outputValue">The new output value.</param>
        inline void SetOutputValue(float outputValue) { OutputValue[1-CurrentBuffer] = outputValue; }

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
