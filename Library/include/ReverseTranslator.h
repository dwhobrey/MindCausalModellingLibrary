#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class Property;
    class Container;
    class GroupContainer;
    class Connector;
    class Translator;

    /// <summary>
    /// Reverse the order of the topology's coordinates.
    /// </summary>
    class ReverseTranslator : public Translator {
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
        /// Constructs a new reverse translator.
        /// </summary>
        ReverseTranslator();

        /// <summary>
        /// Deallocate object.
        /// </summary>
        virtual ~ReverseTranslator();

        /// <summary>
        /// Reverse the order of the elements in the From group.
        /// </summary>
        /// <param name="connector">The Connector being resolved.</param>
        /// <param name="toGroup">The destination group whose elements are to be ordered.</param>
        /// <param name="fromGroup">The source group whose elements are to be ordered.</param>
        virtual void OrderGroup(Connector& connector, GroupContainer& toGroup, GroupContainer& fromGroup);

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
