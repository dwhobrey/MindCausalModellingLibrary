#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Container;
    class Path;

    namespace ClassKinds {
        /// <summary>
        /// These are labels used in Flags for commonly used class types in order to avoid dynamic casting.
        /// </summary>
        enum Enum {
            /// <summary>
            /// Class is a property, don't care what type.
            /// </summary>
            Property,
            /// <summary>
            /// Class is a Distributor.
            /// </summary>
            Distributor,
            /// <summary>
            /// Class is a Connector.
            /// </summary>
            Connector,
            /// <summary>
            /// Class is a Converter.
            /// </summary>
            Converter
        }; 
    }
    typedef ClassKinds::Enum ClassKindsEnum;

    /// <summary>
    /// As the base class for properties, this provides a default proxing implementation.
    /// </summary>
    /// <remarks>
    /// Models a labelled object, i.e. an object with a name.
    /// The Identifier class is used to model the name.
    /// </remarks>
    class Property {
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
        /// The NULL property is returned by proxied properties when the value could not be resolved.
        /// </summary>
        /// <remarks>
        /// The NullProperty is used when a method has to return a property rather than a NULL on error.
        /// </remarks>
        static Property* NullProperty;

#pragma region // Instance variables.
        /// <summary>
        /// Stores a reference to the object's creator.
        /// </summary>
        Container* Creator;
        /// <summary>
        /// The hierarchical parent of this property once its position has been resolved.
        /// </summary>
        Container* Parent;
        /// <summary>
        /// Store a reference to the property's identifier.
        /// </summary>
        Identifier* Identity;
        /// <summary>
        /// The visibility of this property in the container hierarchy.
        /// </summary>
        PropertyScopesEnum Scope;
        /// <summary>
        /// The level of this property in the container hierarchy.
        /// </summary>
        int Level;
        /// <summary>
        /// The rank of this property in its' parent container.
        /// </summary>
        int Order;
#pragma region // Property flags.
        /// <summary>
        /// Flags used to quickly determine property type.
        /// </summary>
        union {
            /// <summary>
            /// Access to all flags, used to set flags to zero.
            /// </summary>
            /// <remarks>
            /// Flags are currently packed into sizeof(int).
            /// <//remarks>
            int AllFlags;
            /// <summary>
            /// The individual flags.
            /// </summary>
            struct {
                /// <summary>
                /// Class contents require refreshing.
                /// </summary>
                bool Refresh : 1; //0x0001
                /// <summary>
                /// Class Value is proxied, e.g. via Alias or Link.
                /// <summary>
                bool IsProxied : 1; //0x0002
                /// <summary>
                /// Class is derived from Filter.
                /// </summary>
                bool IsFilter : 1; //0x0004
                /// <summary>
                /// Class is derived from Container.
                /// </summary>
                bool IsContainer : 1; //0x0008
                /// <summary>
                /// Class is derived from Director.
                /// </summary>
                bool IsDirector : 1; //0x0010
                /// <summary>
                /// Class is derived from Constructor.
                /// </summary>
                bool IsConstructor : 1; //0x0020
                /// <summary>
                /// Class is derived from Producer.
                /// <summary>
                bool IsProducer : 1; //0x0040
                /// <summary>
                /// Class is derived from Transducer.
                /// </summary>
                bool IsTransducer : 1; //0x0080
                /// <summary>
                /// Class is derived from Restrictor.
                /// </summary>
                bool IsRestrictor : 1; //0x0100
                /// <summary>
                /// Class is derived from GroupFilter.
                /// </summary>
                bool IsGroupFilter : 1; //0x0200
                /// <summary>
                /// Class is derived from GroupContainer.
                /// </summary>
                bool IsGroupContainer : 1; //0x0400
                /// <summary>
                /// Class is derived from InputPad.
                /// </summary>
                bool IsInputPad : 1; //0x0800
                /// <summary>
                /// Class is derived from OutputPad.
                /// </summary>
                bool IsOutputPad : 1; //0x1000
                /// <summary>
                /// Indicates the kind of class object this property refers to.
                /// </summary>
                ClassKindsEnum ClassKind : 8;
            } Flags;
        };
#pragma endregion
    protected:
        /// <summary>
        /// The path of this property.
        /// </summary>
        Path* mPropertyPath;
        /// <summary>
        /// The hierarchical declaration order of this property in the
        /// container hierachy.
        /// </summary>
        /// <remarks>
        /// This is as array of Order's, one for each level in the hierarchy:
        /// mOrderID[0] = Number of orders, 
        /// mOrderID[1..n] = Orders from Parent down to Child, i.e. this property.
        /// </remarks>
        int* mOrderID;
#pragma endregion

#pragma region // Constructors.
    public:
        /// <summary>
        /// General constructor for properties.
        /// </summary>
        /// <param name="creator">The object that created this property.</param>
        /// <param name="parent">The hierarchical parent of this property.</param>
        /// <param name="identifier">The identifier for this property.</param>
        /// <param name="scope">The visibility of the property.</param>
        Property(Container* creator, Container* parent, Identifier& identifier, PropertyScopesEnum scope);
        
        /// <summary>
        /// Deallocate property.
        /// </summary>
        virtual ~Property();
#pragma endregion

#pragma region // Accessors.
    protected:
        /// <summary>
        /// Resolve property to its proxied value.
        /// </summary>
        /// <remarks>
        /// This method either returns a valid proxied value, or the NullProperty.
        /// </remarks>
        virtual Property* GetProxy();
    public:
        /// <summary>
        /// Accessor for the property value, handle any proxies.
        /// </summary>
        /// <returns>Returns this if not proxied, or resolved proxy value, which may be the NullProperty.</returns>
        inline Property* GetValue() { return Flags.IsProxied ? GetProxy() : this; }
       
        /// <summary>
        /// Returns the hierarchical declaration order of the property in the
        /// container hierachy.
        /// </summary>
        int* GetOrderID();

        /// <summary>
        /// Gets the relative path for this property.
        /// </summary>
        Path& GetPath();
#pragma endregion

#pragma region // Property methods.
        /// <summary>
        /// Compares this to another property base on Identity.
        /// </summary>
        /// <param name="prop">The property to compare with this.</param>
        /// <returns>Returns &lt; 0, if this &lt; prop; 0 if this == prop;
        /// &gt; 0, if this &gt; prop. </returns>
        inline int CompareTo(const Property& prop) const { return Identity->CompareTo(*(prop.Identity)); }

        /// <summary>
        /// Determines whether this property is in scope with respect to the requester.
        /// </summary>
        /// <param name="requester">A property situated in the container hierarchy of
        /// this property.</param>
        /// <param name="skipParentChecking">When true, scope checks are not performed against
        /// the property's parents.</param>
        /// <returns>Returns true if this property can be seen by the requester.</returns>
        bool InScope(const Property* requester, bool skipParentChecking) const;
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

    /// <summary>
    /// Traits class for Property* hash_compare.
    /// </summary>
    class PropertyPtrHashCompare {	
    public:
	    enum {	// Parameters for hash table.
		    bucket_size = 4, // 0 < bucket_size.
		    min_buckets = 8	 // min_buckets = 2 ^^ N, 0 < N.
        };
        /// <summary>
        /// Default constructor.
        /// </summary>
	    PropertyPtrHashCompare() {
		}
        /// <summary>
        /// Fetches the hash code for the property.
        /// </summary>
        inline size_t operator()(const Property* & key) const { return key->Identity->HashCode; }
        /// <summary>
        /// Less comparator for properties, used for ordering purposes.
        /// </summary>
        inline bool operator()(const Property* &xKey, const Property* & yKey) const {
		    return xKey->Identity->CompareTo(*(yKey->Identity))<0;
		}
    };
}
