#pragma once

namespace Plato {

    class ClassTypeInfo;
    class Configuration;
    class ErrorContext;
    class Property;
    class Container;
    class Model;
    class Connector;

    /// <summary>
    /// This is the base class for Models.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The Model class performs model configuration
    /// and state updating when moving to the next epoch.
    /// </para>
    /// <para>
    /// Model construction is performed in two steps:
    /// <list type="number">
    /// <item><description>
    /// Construction of main Region hierarchy in model's constructor.
    /// </description></item>
    /// <item><description>
    /// Configuration of elements through method Configure.
    /// </description></item>
    /// </para>
    /// </remarks>
    class Model : public Container {
    public:
        /// <summary>
        /// The type of the current connector stack.
        /// </summary>
        typedef stack<Connector*> ConnectorStack;
        /// <summary>
        /// The type of the current container stack.
        /// </summary>
        typedef stack<Container*> ContainerStack;

#pragma region // Class methods & variables.
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

        /// <summary>
        /// Fetches the Model object for the current thread.
        /// </summary>
        inline static Model& Current() { return *((Model*)ThreadLocalVariable::GetVariableValue(mThreadContextKey)); }

    private:
        /// <summary>
        /// The thread local variable key used to determine current model for thread.
        /// </summary>
        static ThreadLocalVariable::Key mThreadContextKey;

        /// <summary>
        /// Tracks the number of model objects.
        /// </summary>
        /// <remarks>
        /// This is used to control library initialization and finalization.
        /// </remarks>
        static int mNumberOfModels;
#pragma endregion

#pragma region // Instance variables.
    public:
        /// <summary>
        /// Signal that is set to true to indicate processor should stop.
        /// </summary>
        bool StopEvent;
        /// <summary>
        /// Indicates when a model is monitoring addition events on GeometricContainers.
        /// </summary>
        bool IsListeningToContainers;
        /// <summary>
        /// The number of epochs the model has been run for.
        /// </summary>
        long EpochTime;
        /// <summary>
        /// The current configure phase.
        /// </summary>
        ConfigurePhasesEnum ConfigurePhase;
        /// <summary>
        /// The current resolution mode during configuration process.
        /// </summary>
        ResolutionModesEnum ResolutionMode;
        /// <summary>
        /// The configurable properties of the model.
        /// </summary>
        Configuration* ModelConfiguration;
        /// <summary>
        /// The error context associated with the model's threads.
        /// </summary>
        ErrorContext* ModelErrorContext;
        /// <summary>
        /// A cache of the current connector being processed for model.
        /// </summary>
        Connector* CurrentConnector;
        /// <summary>
        /// A stack of the Connectors in the process of being resolved.
        /// </summary>
        /// <remarks>Used to track the current connector for the model.</remarks>
        ConnectorStack* CurrentConnectorStack;
        /// <summary>
        /// When traversing the container hierarchy a stack is maintained of the prior
        /// CurrentContainer. Similar to pushd and popd.
        /// </summary>
        ContainerStack* CurrentContainerStack;
#pragma endregion

#pragma region // Constructors.
    public:
        /// <summary>
        /// Model process objects are created to encapsulate the context of a running model.
        /// </summary>
        /// <remarks>
        /// The derived class's constructor should create the main element hierarchy.
        /// It should cause the hierarchy to be constructed via a chain reaction in
        /// which the child containers are instantiated during the
        /// construction of the parent container, and so on. 
        /// If the construction fails an error should be added to the model's
        /// error context. This will prevent the configuration step proceeding.
        /// </remarks>
        /// <param name="modelName">The name of the model.</param>
        /// <param name="arguments">The NULL terminated arguments to pass to the model, or NULL.</param>
        Model(const char*modelName, char* arguments[]);
        /// <summary>
        /// Make sure model gets unloaded.
        /// </summary>
        virtual ~Model();
#pragma endregion

#pragma region // General methods.
    public:
        /// <summary>
        /// The present working container in the hierarchy.
        /// This will change often, e.g. whenever the container context changes during run-time.
        /// </summary>
        inline Container* GetCurrentContainer(){
            return CurrentContainerStack->empty() ? NULL : CurrentContainerStack->top();
        }
        /// <summary>
        /// Make this connector the current connector being processed.
        /// </summary>
        /// <param name="connector">The connector to push onto the stack and make current.</param>
        void PushCurrentConnector(Connector& connector);
        /// <summary>
        /// Restore the current connector to its previous value.
        /// </summary>
        void PopCurrentConnector();
        /// <summary>
        /// Attempts to fetch the named property's value from the model container.
        /// </summary>
        /// <param name="name">Name of property to fetch value of.</param>
        /// <returns>Returns the property's value, or NULL if unable to find.</returns>
        void* GetPropertyValue(string& name);
        /// <summary>
        /// Produces a summary descriptio of this model object.
        /// </summary>
        /// <returns>Returns information on the model.</returns>
        string& StatusReport();
#pragma endregion

#pragma region // Model control methods.
    public:
        /// <summary>
        /// Save model state to file.
        /// </summary>
        /// <param name="archivePath">The path and filename to save state to, or NULL to use prior value.</param>
        /// <returns>Returns 0 if successful, otherwise error code.</returns>
        int Save(string* archivePath);

        /// <summary>
        /// Load model state from file.
        /// </summary>
        /// <param name="archivePath">The path and filename to load state from, or NULL to use prior value.</param>
        /// <returns>Returns model object if successful, otherwise NULL.</returns>
        static Model* Load(string& archivePath);

        /// <summary>
        /// Run the model for the specified number of epochs.
        /// </summary>
        /// <remarks>
        /// The Run method can be called repeatedly for certain types of models.
        /// </remarks>
        /// <param name="numberOfEpochs">Number of epochs to run model for.
        /// If numberOfEpochs &lt; 0, the number of epochs will be infinite.
        /// </param>
        /// <returns>Returns 0 if successful, otherwise error code.</returns>
        int Run(long numberOfEpochs);
#pragma endregion

#pragma region // Epoch updating methods.
    public:
        /// <summary>
        /// This updates the model moving it to the next epoch.
        /// </summary>
        /// <remarks>
        /// Need to add some remarks on the order of the phase process.
        /// Statement on the order in which elements are resolved,
        /// such as Regions, Phenomenon, Flows, Dids,
        /// in a depth first, then declarative order.
        /// </remarks>
        virtual void Update();

        /// <summary>
        /// Helper method that calls Update on the objects in the enumeration.
        /// </summary>
        /// <param name="properties">The properties that implement Update().</param>
        static void Update(vector<Property*>& properties);
#pragma endregion

#pragma region // Methods for the configuration process.
    public:
        /// <summary>
        /// This performs secondary model configuration, such as distributing flows.
        /// </summary>
        /// <returns>Returns zero if successfully configured, otherwise an error code.</returns>
        /// <remarks>
        /// The stages in the configuration algorithm are as follows:
        /// <list type="number">
        /// <item><term>Distribution stage.</term>
        ///   <description>
        ///   Depth-width descent on element tree applying Distributors.
        ///   <list type="number">
        ///   <item><description>
        ///   Applying Distributor may distribute more (secondary) distributors,
        ///   so, perform any secondary distributors.
        ///   </description></item>
        ///   <item><description>
        ///   Note, the later phases may involve converters and so more distributors.
        ///   </description>
        ///   </list>
        ///   </description></item>
        ///   <item><term>"Now" stage.</term><description>
        ///   <list type="number">        
        ///   <item><description>
        ///   Connector phase. Depth-width descent applying connectors.
        ///   <br>Apply any "now" connectors.
        ///   </description></item>
        ///   <item><description>
        ///   Converter phase. Depth-width descent applying converters.
        ///   <br>Apply any "now" converters.
        ///   </description></item>
        ///   </list>
        ///   </description></item>
        ///   <item><term>"Final" stage.</term><description>
        ///   <list type="number">
        ///   <item><description>
        ///   Connector phase. Depth-width descent applying connectors.
        ///   <br>Apply any "final" connectors.
        ///   </description></item>
        ///   <item><description>
        ///   Converter phase. Depth-width descent applying converters.
        ///   <br>Apply any "final" converters.
        ///   </description></item>
        ///   </list>
        ///   </description></item>
        /// </list>
        /// Dealing with converters:
        ///   <list type="number">
        ///   <item><description>
        ///   Converter could add elements before or after current position.
        ///   Use-cases: a) local converter, b) final converter, c) global reach converter.
        ///   </description></item>
        ///   <item><description>
        ///   This would lead to resolving times: "now", "default", "final".
        ///   "default" means, apply now unless element is after current position.
        ///   e.g. a converter makes additions to elements after current position.
        ///   </description></item>
        ///   <item><description>
        ///   Catch additions via ContainerAddDelegate.
        ///   <list type="number">
        ///   <item><description>
        ///   If addition is before current position,
        ///             then push state and recurse back to it doing:
        ///             Distributors and now's, and finals if in final stage.
        ///   </description></item>
        ///   <item><description>
        ///   If addition is after current position,
        ///             then do Distributors,
        ///             if in final stage, do any now's,
        ///             otherwise leave nows or finals for when encountered normally later.
        ///   </description></item>
        ///   </list>
        ///   </description></item>
        /// </list>
        /// </remarks>
        int Configure();

    private:
#pragma region // Configure: Distribution stage methods.
        /// <summary>
        /// Performs the distribution phase of configuration.
        /// </summary>
        /// <param name="container">The container to check for distributors.</param>
        void ApplyDistributors(Container& container);
#pragma endregion

#pragma region // Configure: Connector phase methods.
        /// <summary>
        /// Performs the connector phase of configuration.
        /// </summary>
        /// <param name="container">The container to check for connectors.</param>
        void ApplyConnectors(Container& container);
#pragma endregion

#pragma region // Configure: Converter phase methods.
        /// <summary>
        /// Performs the converter phase of configuration.
        /// </summary>
        /// <param name="container">The container to check for converters.</param>
        void ApplyConverters(Container& container);
#pragma endregion

#pragma region // Methods for dealing with properties added on the fly.
        /// <summary>
        /// Applies the Distributor and Connector steps in a configuration phase to a container.
        /// </summary>
        /// <param name="container">The container to apply the configuration steps to.</param>
        void ApplyDistributorsAndConnectors(Container& container);

        /// <summary>
        /// Applies all the steps in a configuration phase to a container.
        /// </summary>
        /// <param name="container">The container to apply the configuration steps to.</param>
        void ApplyAll(Container& container);

     public:
        /// <summary>
        /// ContainerAddDelegate.
        /// Called when a property was successfully added to a container.
        /// </summary>
        /// <remarks>
        /// Check whether property needs to be configured.
        /// </remarks>
        /// <param name="container">The container on which the event occurred.</param>
        /// <param name="prop">The property that was added.</param>
        void AddedProperty(Container& container, Property& prop);
#pragma endregion

#pragma endregion

#pragma region // Deprecated methods: use Path(const string& path).
    public:
        /// <summary>
        /// Add property at the location given by path relative to model root.
        /// Intermediate containers must already exist or be creatable by a Producer.
        /// </summary>
        /// <param name="prop">The property to add, or NULL to simply create path.</param>
        /// <param name="path">Absolute path of location to add property.</param>
        /// <returns>
        /// Returns the container to which the property was added if addition succeeded, otherwise NULL.
        /// </returns>
        static Container* AddViaPath(Property* prop, const string& path);
        /// <summary>
        /// Gets the property given by the path.
        /// </summary>
        /// <remarks>If intermediate path members do not exist, NULL is returned.</remarks>
        /// <param name="path">Absolute path location of property to fetch.</param>
        /// <returns>Returns the required property if successful, otherwise NULL.</returns>
        static Property* GetViaPath(const string& path);

        /// <summary>
        /// Helper method to fetch the parent container of a property.
        /// </summary>
        /// <param name="path">The property path, includes property name.</param>
        /// <param name="propertyName">Out parameter, the property name extracted from the path.</param>
        /// <returns>Returns the container holding this property, or NULL if property not found.</returns>
        static Container* GetPropertyParent(const string& path, string*& propertyName);

        /// <summary>
        /// Deletes the property and it's contents from the model.
        /// </summary>
        /// <param name="path">Path of property to delete.</param>
        /// <returns>Returns the property that was removed, or NULL.</returns>
        static Property* Remove(const string& path);
#pragma endregion

    private:
        friend class InitializerCatalogue;
        /// <summary>
        /// Checks whether library needs initializing.
        /// </summary>
        /// <returns>Returns the property types permitted in models.</returns>
        static const ClassTypeInfo::HashSet& InitLibrary();
        /// <summary>
        /// The class initializer.
        /// </summary>
        static void Initializer();
        /// <summary>
        /// The class finalizer.
        /// </summary>
        static void Finalizer();
        /// <summary>
        /// Signal handler for catching Ctrl-C.
        /// </summary>
        /// <param name="sig">The signal id.</param>
        static void SignalHandler(int sig);
    };
}
