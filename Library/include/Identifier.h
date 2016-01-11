#pragma once

namespace Plato {

    class Property;
    class Point;

// IdentifierHashArgs & IdentifierHashCompare are used to deal with STL hash differences between MS STL and SGI STL.
#ifdef WIN32 
#define IdentifierHashArgs IdentifierPtrHashCompare
#define IdentifierHashCompare(a,b) ((a)<(b))
#else
#define IdentifierHashArgs IdentifierPtrHashCompare,IdentifierPtrHashCompare
#define IdentifierHashCompare(a,b) ((a)==(b))
#endif

    /// <summary>
    /// Models an identifier used as a label for a Property.
    /// This is made up of three terms:
    /// 1) a string Name, which may be optional if identifier is used as a regex.
    /// 2) an optional Point Position suffix,
    /// 3) an optional string TypeName suffix.
    /// Each term is independent. The optional terms may be NULL.
    /// </summary>
    /// <remarks>
    /// A property may optionally be associated with the identifier.
    /// </remarks>
    class Identifier {
    public:
        typedef const Identifier* ConstIdentifierPtr;
#pragma region // Class variables.
    private:
        /// <summary>
        /// A counter used to generate unique identifiers on demand.
        /// </summary>
        static int mLastIdentifierNumber;
#pragma endregion

#pragma region // Instance variables.
    public:
        /// <summary>
        /// The property associated with this identifier, or NULL if none.
        /// </summary>
        Property* PropertyPtr;

        /// <summary>
        /// The identifier's name.
        /// </summary>
        const string* Name;

        /// <summary>
        /// The Position value stored as a point.
        /// </summary>
        const Point* Position;

        /// <summary>
        /// Cache of Name's hashcode.
        /// </summary>
        size_t NameHashCode;

        /// <summary>
        /// Cache of the identifier's hash code.
        /// </summary>
        size_t HashCode;
#pragma endregion

#pragma region // Constructors.
    public:
        /// <summary>
        /// Construct a new identifier for a property.
        /// </summary>
        /// <remarks>
        /// A Name string is constructed from name,
        /// whereas Position is copied by reference.
        /// </remarks>
        /// <param name="name">The property's name.</param>
        /// <param name="position">The property's position value.</param>
        /// <param name="prop">The property to associate with this identifier.</param>
        Identifier(const char* name, const Point* position, Property* prop=NULL);

        /// <summary>
        /// Construct a new identifier for a property.
        /// </summary>
        /// <remarks>The identifier takes ownership of the parameter objects.</remarks>
        /// <param name="name">The property's name.</param>
        /// <param name="position">The property's position value.</param>
        /// <param name="prop">The property to associate with this identifier.</param>
        Identifier(const string& name, const Point* position, Property* prop=NULL);

        /// <summary>
        /// Construct a new identifier for a standard PathNode.
        /// </summary>
        /// <remarks>Converts name to a string and sets position to NULL.</remarks>
        /// <param name="name">The name of the standard node.</param>
        Identifier(char name);

        /// <summary>
        /// Construct a new identifier.
        /// </summary>
        /// <remarks>
        /// A Name string is constructed from name.
        /// </remarks>
        /// <param name="name">The property's name.</param>
        Identifier(const char* name);

        /// <summary>
        /// Construct a new identifier for a property.
        /// </summary>
        /// <remarks>The identifier takes ownership of the name string.</remarks>
        /// <param name="name">The property's name.</param>
        Identifier(const string& name);

        /// <summary>
        /// Copy constructor for an Identifier.
        /// </summary>
        /// <remarks>
        /// Performs a deep copy.
        /// </remarks>
        /// <param name="ident">The identifier to copy.</param>
        Identifier(const Identifier& ident);
        /// <summary>
        /// Deallocates resources.
        /// </summary>
        ~Identifier();
#pragma endregion

#pragma region // Accessors.
    public:
        /// <summary>
        /// Accessor for the identifier's Name.
        /// </summary>
        void SetName(const string& name);

        /// <summary>
        /// Accessor for the position part of the identifier.
        /// </summary>
        void SetPosition(const Point& position);

        /// <summary>
        /// Accessor for the property associated with this identifier.
        /// </summary>
        /// <remarks>
        /// Sets the TypeName term accordingly.
        /// The property value may be NULL if no property is associated with the identifier.
        /// </remarks>
        void SetProperty(Property* prop);
#pragma endregion

        /// <summary>
        /// Gets a string representation of the identifier.
        /// </summary>
        /// <returns>Returns the string "Name[Position]", which may be empty.</returns>
        string& StatusReport() const;

        /// <summary>
        /// Compares this identifier to another.
        /// </summary>
        /// <remarks>
        /// The order of comparison is: Name,Position.
        /// If a term is NULL it is ignored in the comparison.
        /// </remarks>
        /// <param name="ident">The identifier to compare this with.</param>
        /// <returns>Returns &lt; 0, if this &lt; obj; 0 if this == obj; &gt; 0, if this &gt; obj. </returns>
        int CompareTo(const Identifier& ident) const;

#pragma region // DEBUG: These methods show be phased out for ones that use Points and Container.LastOrderID.
        /// <summary>
        /// Auto generates a new unique number for creating identifier names.
        /// </summary>
        static string& GetIdentifierNumber();

        /// <summary>
        /// Auto generates a new identifier value.
        /// </summary>
        static string& GetIdentifierValue();

        /// <summary>
        /// Generates a new Identifier with a unique value.
        /// </summary>
        static Identifier& Generate();

        /// <summary>
        /// Generates a new Identifier with the Name = name + IdentifierNumber.
        /// </summary>
        /// <param name="name">The prefix to add to IdentifierNumber used to form Name.</param>
        static Identifier& Generate(const char* name);
#pragma endregion
    };

    /// <summary>
    /// Traits class for Identifier* hash_compare.
    /// </summary>
    class IdentifierPtrHashCompare {	
    public:
	    enum {	// Parameters for hash table.
		    bucket_size = 4, // 0 < bucket_size.
		    min_buckets = 8	 // min_buckets = 2 ^^ N, 0 < N.
        };
        /// <summary>
        /// Default constructor.
        /// </summary>
	    IdentifierPtrHashCompare() {
		}
        /// <summary>
        /// Fetches the hash code for the Identifier.
        /// </summary>
        inline size_t operator()(const Identifier::ConstIdentifierPtr & key) const {
		    return key->HashCode;
		}
        /// <summary>
        /// Less comparator for Identifiers, used for ordering purposes.
        /// </summary>
        inline bool operator()(const Identifier::ConstIdentifierPtr &xKey, const Identifier::ConstIdentifierPtr & yKey) const {
		    return IdentifierHashCompare(xKey->CompareTo(*yKey),0);
		}
    };
}
