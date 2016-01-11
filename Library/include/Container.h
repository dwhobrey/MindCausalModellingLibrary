#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Identifier;
    class IdentifierRegex;
    class IdentifierEnumerator;
    class ProducerListener;
    class Property;
    class Container;
    class Geometry;
    class Producer;
    class Restrictor;
    class Model;

    /// <summary>
    /// The Container base class is used to model an object with a collection of properties
    /// some of which may be sub-containers.
    /// </summary>
    /// <remarks>
    /// The Container object serves two purposes:
    /// 1) to present the objects of the application with a common interface,
    /// 2) to present a recursive container (e.g. directory like) interface.
    /// It manages a collection of type categorised properties.
    /// <list type="number">
    /// <item><description>Specifying and checking of permitted category types.</description></item>
    /// <item><description>Allow for items to be added, deleted and updated.</description></item>
    /// <item><description>Fast access via binary search of added property by type.</description></item>
    /// <item><description>Lookup across all property types.</description></item>
    /// <item><description>Maintains a sorted index into collection of all properties.</description></item>
    /// <item><description>Maintains a list of property addition order.</description></item>
    /// <item><description>Provides an event listener for monitoring and responding to
    /// operations that are applied to the container.</description></item>
    /// <item><description>These elements have a geometry and position in the enclosing region.</description></item>
    /// </list>
    /// </remarks>
    class Container : public Property {
    public:
        typedef hash_map <ClassTypeInfo::ConstClassTypeInfoPtr, vector<Property*>*, ClassTypeInfoHashArgs> HashMap;
        typedef hash_map <Identifier::ConstIdentifierPtr, Property*, IdentifierHashArgs> HashTable;
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
        /// Table entry = (key=ClassTypeInfo*).
        /// </summary>
        /// <remarks>
        /// This is used to initialise the CategorisedProperties table.
        /// </remarks>
        static const ClassTypeInfo::HashSet* PropertyTypes;
#pragma endregion

#pragma region // Instance variables.
    public:
        /// <summary>
        /// A flag indicating whether this container has restrictors.
        /// </summary>
        bool IsUnrestricted;
        /// <summary>
        /// When true this means the container contains subcontainers
        /// that may require emulation and configuration.
        /// </summary>
        bool IsComposite;
        /// <summary>
        /// When true the element requires emulation.
        /// </summary>
        bool DoEmulate;
        /// <summary>
        /// The state of configuration.
        /// </summary>
        ConfigurePhasesEnum ConfigurePhase;
        /// <summary>
        /// Topology of subelements.
        /// Then need enumerators for the element: 1) occupied = mElements, 2) vacant = VacantFilter ?.
        /// </summary>
        Geometry* ContainerGeometry;
        /// <summary>
        /// The model this container belongs to.
        /// </summary>
        Model* ContainerModel;
        /// <summary>
        /// The properties in the order they were added.
        /// </summary>
        vector<Property*>* OrderedProperties;
        /// <summary>
        /// The properties stored in a hash table for fast lookup.
        /// </summary>
        HashTable* HashedProperties;
    protected:
        /// <summary>
        /// <para>The Categorised Table structured by property type.</para>
        /// This is modelled as a hashtable of OrderedLists:
        /// entry = (key=ClassTypeInfo*,data=vector &lt; Property* &gt; ).
        /// </summary>
        HashMap* mCategorisedProperties;
        /// <summary>
        /// A cache of the types allowed: which may be derivatives of the Category Types.
        /// </summary>
        HashMap* mCategoryCache;
        /// <summary>
        /// An optional producer listener that creates new properties on demand.
        /// </summary>
        ProducerListener* mProducerListener;
        /// <summary>
        /// Counts the number of items added to list.
        /// </summary>
        int mLastOrder;
        /// <summary>
        /// Indicates that this is a clone of another container.
        /// </summary>
        bool mIsClone;
        /// <summary>
        /// Indicates that contents should be deleted when destroyed.
        /// </summary>
        bool mDoDelete;
#pragma endregion

#pragma region // Constructors.
    protected:
        /// <summary>
        /// Standard constructor used by derived classes.
        /// </summary>
        /// <param name="propertyTypes">The kinds of properties this container can contain.</param>
        /// <param name="parent">The hierarchical parent of this container.</param>
        /// <param name="creator">The container that created this container.</param>
        /// <param name="identifier">The identifier for this container.</param>
        /// <param name="scope">The scope of the container.</param>
        Container(const ClassTypeInfo::HashSet& propertyTypes, Container* creator = NULL, Container* parent = NULL,
            Identifier& identifier = Identifier::Generate("Container"), 
            PropertyScopesEnum scope = PropertyScopes::Public);
        
    public:
        /// <summary>
        /// Releases container resources.
        /// </summary>
        /// <remarks>
        /// Calls Clear(true) and then Destroy().
        /// It does nothing for Cloned containers.
        /// </remarks>
        virtual ~Container();
        /// <summary>
        /// Deletes the container's tables, ready for deconstruction.
        /// </summary>
        /// <remarks>
        /// It does not delete properties.
        /// </remarks>
        void Destroy();

    public:
        /// <summary>
        /// Shallow copies this containers contents into clone container.
        /// </summary>
        /// <remarks>
        /// The container's tables and vectors are copied by reference.
        /// The clone should only be used for reading, otherwise LastOrder will be out of step.
        /// The container's base Property values are not cloned.
        /// </remarks>
        void CloneContents(Container& cloneContainer) const;

        /// <summary>
        /// Clear out all the properties.
        /// </summary>
        /// <param name="doDelete">When true, property destructors are called.</param>
        void Clear(bool doDelete=true);

        /// <summary>
        /// Removes properties of this category type or category derived type.
        /// </summary>
        /// <param name="categoryType">The category of properties to clear.</param>
        /// <param name="doDelete">When true, property destructors are called.</param>
        void Clear(const ClassTypeInfo& categoryType,bool doDelete=true);
#pragma endregion

#pragma region // Initialisation methods.
    public:
        /// <summary>
        /// Creates a HashSet for initialising PropertyTypes 
        /// from a NULL terminated argument list of ClassTypeInfo pointers.
        /// </summary>
        /// <param name="first">The first class info object to add to hash map.</param>
        /// <returns>Returns a ClassTypeInfo::HashSet of the arguments.</returns>
        static ClassTypeInfo::HashSet* CreatePropertyTypes(const ClassTypeInfo* first,...);
#pragma endregion

#pragma region // Content methods.
    public:
        /// <summary>
        /// Users of container should call this before accessing container contents
        /// for the first time. This method checks if contents need refreshing.
        /// </summary>
        inline void AccessContents() { if(Flags.Refresh) RefreshContents(); };
    protected:
        /// <summary>
        /// Refresh container contents ready for access.
        /// </summary>
        /// <remarks>
        /// Overriding RefreshContents allows derived classes,
        /// such as Filter, to hook
        /// into container access and apply filter on demand.
        /// <para>
        /// This method is called by AccessContents when Flags.Refresh is true.
        /// The derived class should set Refresh flag to false, 
        /// then refresh the container's contents as necesary.
        /// </para>
        /// <para>
        /// Derived classes, such as Filter, lazily fetch the container contents
        /// on demand when this method is called.
        /// </para>
        /// <para>
        /// Technical notes:
        /// 1) In some cases it may be possible to cache results.
        /// 2) Filters may be chained.
        /// </para>
        /// </remarks>
        virtual void RefreshContents();
    public:    
        /// <summary>
        /// Determines whether a property with the given identifier is a member of the container.
        /// </summary>
        /// <param name="identifier">Identifier to check for.</param>
        /// <returns>Returns true if container has a property with this identifier.</returns>
        bool Contains(const Identifier& identifier) const;

        /// <summary>
        /// Determines whether the object is a property of this container.
        /// </summary>
        /// <param name="prop">Property to check for.</param>
        /// <returns>Returns true if container has property.</returns>
        bool IsMember(const Property& prop) const;

        /// <summary>
        /// Gets the number of properties in the table.
        /// </summary>
        inline int Count() const { return (int)(OrderedProperties->size()); }

        /// <summary>
        /// Gets the number of properties in the table of the specified category type.
        /// </summary>
        /// <param name="categoryType">The category of items to count.</param>
        /// <returns>Returns the number of items as an Integer.</returns>
        int CategoryCount(const ClassTypeInfo& categoryType) const;

        /// <summary>
        /// Gets an OrderedList of objects for the given category type.
        /// </summary>
        /// <param name="categoryType">The type of objects to fetch, or NULL to return master table.</param>
        /// <returns>Returns an OrderedList of the objects, or NULL if none of that type.</returns>
        inline vector<Property*>* GetCategory(const ClassTypeInfo& categoryType) const { 
            HashMap::iterator itr = mCategorisedProperties->find(&categoryType);
            return itr==mCategorisedProperties->end() ? NULL : itr->second; }

        /// <summary>
        /// Find the closest category type for the specified type.
        /// The type must be derived from or equal to one of the category types.
        /// Uses a cache to speed up subsequent checks.
        /// </summary>
        /// <remarks>
        /// Searches for type match with the lowest inheritance count.
        /// </remarks>
        /// <param name="type">The type to check for category membership.</param>
        /// <returns>Returns the category list that the type validated against, or NULL if not allowed.</returns>
        vector<Property*>* GetClosestCategory(const ClassTypeInfo& type);

        /// <summary>
        /// Searches the ordered list of properties for one with the given order.
        /// </summary>
        /// <param name="order">The order of the property to search for.</param>
        /// <returns>
        /// Returns the index of the property (in the sorted list), 
        /// or if not found it returns the bitwise complement of the closest index.
        /// </returns>
        int BinarySearch(int order) const;

        /// <summary>
        /// Searches the ordered list of properties for one with the given order.
        /// </summary>
        /// <param name="order">The order of the property to search for.</param>
        /// <param name="searchVector">The vector to perform the search on.</param>
        /// <returns>
        /// Returns the index of the property (in the sorted list), 
        /// or if not found it returns the bitwise complement of the closest index.
        /// </returns>
        int BinarySearch(int order, const vector<Property*> & searchVector) const;

        /// <summary>
        /// Uses a binary search to find the property with the given name.
        /// </summary>
        /// <param name="identifier">The name of the property to fetch.</param>
        /// <returns>Returns the property if found, otherwise NULL.</returns>
        Property* GetProperty(const Identifier& identifier) const;

        /// <summary>
        /// Fetches the specified property from the properties, if it exists.
        /// </summary>
        /// <param name="order">The order number of the property to fetch.</param>
        /// <returns>Returns the requested property, or NULL if not found.</returns>
        Property* GetProperty(int order) const;

        /// <summary>
        /// Helper method for the Geometry of this element.
        /// It also sets the Geometry's container to this element.
        /// </summary>
        /// <param name="geometry">The geometry for the container.</param>
        void SetGeometry(Geometry& geometry);

        /// <summary>
        /// Add a property to this container and set its default position.
        /// </summary>
        /// <param name="prop">The property to add.</param>
        /// <returns>Returns true if added.</returns>
        virtual bool Add(Property& prop);

        /// <summary>
        /// Add a container to this container and set its default position.
        /// </summary>
        /// <param name="container">The container to add.</param>
        /// <returns>Returns true if added.</returns>
        bool Add(Container& container);

        /// <summary>
        /// Add a producer.
        /// </summary>
        /// <remarks>
        /// Also sets up a ProducerListener if necessary.
        /// </remarks>
        /// <param name="producer">An instance of a producer.</param>
        /// <returns>Returns true if added ok.</returns>
        bool Add(Producer& producer);

        /// <summary>
        /// Add a restrictor.
        /// </summary>
        /// <param name="restrictor">An instance of the restrictor to add.</param>
        /// <returns>Returns true if added ok.</returns>
        bool Add(Restrictor& restrictor);        
        /// <summary>
        /// Removes the property from the container's properties.
        /// </summary>
        /// <param name="ident">The identifier of the object to remove.</param>
        /// <returns>Returns the property that was removed, or NULL if not found.</returns>
        Property* Remove(const Identifier& ident);
#pragma endregion

#pragma region // Access methods.
    public:
        /// <summary>
        /// Uses Directors to determine the property to fetch.
        /// Performs scope check and sets access mode.
        /// </summary>
        /// <remarks>
        /// This method invokes directors if available,
        /// otherwise it tries fetching the property with the identifier name.
        /// Directing continues until a non-container property is reached.
        /// </remarks>
        /// <param name="identifier">The identity of the Director to use, along with terminus Position and TypeName.
        /// If NULL or Name is empty, an appropriate implicit director name is used.</param>
        /// <param name="source">The property from which routing is required. Some Directors use this to determine route.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        /// <returns>Returns the requested property, or NULL if it does not exist or not created.</returns>
        Property* DirectToProperty(const Identifier* identifier, const Property* source, 
            const Property* requester, PropertyModesEnum mode);

    protected:
        /// <summary>
        /// A factory for creating properties.
        /// </summary>
        /// <remarks>
        /// This is called if the Property could not be created via a Producer.
        /// It is up to the method to call Container.Add as necessary.
        /// Factory must copy identifier.
        /// </remarks>
        /// <param name="identifier">Identifier of property to fetch.
        /// This will typically include a Name and Position, although the Type may not be set.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the properties to fetch, or NULL if scope checking not required.</param>
        /// <returns>Returns the requested property, or NULL if it could not be created.</returns>
        virtual Property* PropertyFactory(const Identifier& identifier, const Property* requester);

        /// <summary>
        /// Uses Producers to create a new property in the container.
        /// </summary>
        /// <remarks>
        /// This method is a hook for derived classes and is only called from GetProperty
        /// when in mode PropertyModes.Producing and GetProperty fails to find the requested property.
        /// This default implementation invokes the Create method of the ProducerListener,
        /// if there is one.
        /// It is up to the Producer to call Container.Add as necessary.
        /// </remarks>
        /// <param name="identifier">Identifier of property to fetch.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the properties to fetch, or NULL if scope checking not required.</param>
        /// <returns>Returns the requested property, or NULL if it could not be created.</returns>
        virtual Property* ProducerCreate(const Identifier& identifier, const Property* requester);

        /// <summary>
        /// Uses Producers to populate the container with the specified properties.
        /// </summary>
        /// <remarks>
        /// This method is a hook for derived classes and is only called from GetProperties
        /// when in mode PropertyModes.Producing.
        /// This default implementation invokes the Populate method of the ProducerListener,
        /// if there is one.
        /// It is up to the Producer to call Container.Add as necessary.
        /// </remarks>
        /// <param name="regexIdentifier">The regex Identifier for the properties to produce.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the properties to fetch, or NULL if scope checking not required.</param>
        virtual void ProducerPopulate(const IdentifierRegex& regexIdentifier, const Property* requester);

    public:
        /// <summary>
        /// Fetches the specified property from the properties, if it exists.
        /// Performs scope check and sets access mode.
        /// </summary>
        /// <remarks>
        /// This method invokes producers if appropriate.
        /// </remarks>
        /// <param name="identifier">Identifier of property to fetch.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested property.</param>
        /// <returns>Returns the requested property, or NULL if it does not exist or not created.</returns>
        Property* GetProperty(const Identifier& identifier, const Property* requester, PropertyModesEnum mode);

        /// <summary>
        /// Fetches the specified properties from the container, if any exist.
        /// Performs scope check and sets access mode.
        /// </summary>
        /// <remarks>
        /// This method invokes producers if appropriate.
        /// </remarks>
        /// <param name="regexIdentifier">The regex Identifier for the properties to fetch.</param>
        /// <param name="categoryType">The category of the table to fetch, or NULL for master table.</param>
        /// <param name="includeWhenMatches">Indicates whether to return items that match, or do not match.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the properties to fetch, or NULL if scope checking not required.</param>
        /// <param name="mode">The access mode for the requested properties.</param>
        /// <returns>Returns the requested properties, or NULL if none exist or created.</returns>
        IdentifierEnumerator* GetProperties(const IdentifierRegex& regexIdentifier, const ClassTypeInfo& categoryType, 
            bool includeWhenMatches, const Property* requester, PropertyModesEnum mode);

        /// <summary>
        /// Returns the hierarchical declaration order of the item in the
        /// container hierachy.
        /// </summary>
        /// <param name="parent">The parent container for the item, or NULL.</param>
        /// <param name="prop">The property whose orderID is required.</param>
        /// <returns>Returns the orderID as an int array.
        /// If the property does not exist in the parent,
        /// the last order entry in the array has a value of -1.
        /// If the parent is NULL, the order id of the property is 0.
        /// </returns>
        static int* GetOrderID(const Container* parent, const Property& prop);

        /// <summary>
        /// Compares two OrderID's.
        /// </summary>
        /// <param name="a">The first OrderID.</param>
        /// <param name="b">The second OrderID.</param>
        /// <returns>Returns &lt; 0, if a before b;
        /// 0 if a in same position as b;
        /// &gt; 0, if a after b. </returns>
        static int CompareToOrder(const int* a, const int* b);

        /// <summary>
        /// Compares two OrderID's and returns the index at which they first differ.
        /// </summary>
        /// <param name="a">The first OrderID.</param>
        /// <param name="b">The second OrderID.</param>
        /// <returns>Returns the index at which the OrderID's differ,
        /// or max Length if no difference up to min Length, or -1 if either is NULL.</returns>
        static int OrderDifferIndex(const int* a, const int* b);

        /// <summary>
        /// Get a property from the specified container hierarchy based on its' OrderID.
        /// </summary>
        /// <param name="container">The container to use as the hierarchy root.</param>
        /// <param name="orderID">The OrderID of the property to fetch.</param>
        /// <returns>Returns the property if it exists, otherwise NULL.</returns>
        static Property* GetProperty(const Container* container, const int* orderID);

        /// <summary>
        /// Recursively search up hierarchy for first property with the given identifier.
        /// </summary>
        /// <param name="searchContainer">Starting point for search.</param>
        /// <param name="identifier">The name of the property to find.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <returns>The property if found, otherwise NULL.</returns>
        static Property* FindProperty(Container* searchContainer, const Identifier& identifier, const Property* requester);

        /// <summary>
        /// Recursively search up or down hierarchy for first property with the given identifier.
        /// </summary>
        /// <param name="searchContainer">Starting point for search.</param>
        /// <param name="identifier">The name of the property to find.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="includeWhenMatches">When false, properties that do not match regex are searched.</param>
        /// <param name="searchUp">The search direction relative to the starting container.</param>
        /// <param name="parentContainer">Returns the container of the property if found, otherwise NULL.</param>
        /// <returns>The property if found, otherwise NULL.</returns>
        static Property* FindProperty(Container* searchContainer, const Identifier& identifier, const Property* requester,
            bool includeWhenMatches, bool searchUp, Container*& parentContainer);

        /// <summary>
        /// Recursively search up or down hierarchy for first property matching regex.
        /// </summary>
        /// <param name="searchContainer">Starting point for search.</param>
        /// <param name="regex">The name of the property to find. This can be a Regex.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the property to fetch, or NULL if scope checking not required.</param>
        /// <param name="includeWhenMatches">When false, properties that do not match regex are searched.</param>
        /// <param name="searchUp">The search direction relative to the starting container.</param>
        /// <param name="parentContainer">Returns the container of the property if found, otherwise NULL.</param>
        /// <returns>The property if found, otherwise NULL.</returns>
        static Property* FindProperty(Container* searchContainer, const IdentifierRegex& regex, const Property* requester,
            bool includeWhenMatches, bool searchUp, Container*& parentContainer);
#pragma endregion

#pragma region // Model updating methods.
    public:
        /// <summary>
        /// Called every epoch to update the state of the element.
        /// This may then call BirthDeath and Superposition methods?
        /// </summary>
        virtual void Update();

        /// <summary>
        /// Concerned with construction and destruction of elements.
        /// </summary>
        virtual void BirthDeath();

        /// <summary>
        /// Concerned with superposed elements and their interaction.
        /// </summary>
        virtual void Superposition();

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
