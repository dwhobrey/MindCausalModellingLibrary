#pragma once

namespace Plato {

    // ClassTypeInfo class factories return properties, so need to forward declare Property.
    class Property;
    class ClassTypeInfoPtrHashCompare;
    class Arguments;

// ClassTypeInfoHashArgs & ClassTypeInfoHashCompare are used to deal with STL hash differences between MS STL and SGI STL.
#ifdef WIN32 
#define ClassTypeInfoHashArgs ClassTypeInfoPtrHashCompare
#define ClassTypeInfoHashCompare(a,b) ((a)<(b))
#else
#define ClassTypeInfoHashArgs ClassTypeInfoPtrHashCompare,ClassTypeInfoPtrHashCompare
#define ClassTypeInfoHashCompare(a,b) ((a)==(b))
#endif

    /// <summary>
    /// Holds ID information per class that is used to generate fixed class codes.
    /// </summary>
    class ClassTypeInfo {
    public:
        typedef const ClassTypeInfo* ConstClassTypeInfoPtr;
        /// <summary>
        /// The hashtable type for storing ClassTypeInfo's.
        /// </summary>
        typedef hash_set <ConstClassTypeInfoPtr, ClassTypeInfoHashArgs> HashSet;
        typedef hash_map <Strings::ConstStringPtr, const ClassTypeInfo*, StringHashArgs> HashMap;
        typedef pair<Strings::ConstStringPtr,const ClassTypeInfo*> Pair;
        typedef Property* (*FactoryDelegate)(Arguments*);

        /// <summary>
        /// The first field bit index for the library code in the TypeCode.
        /// </summary>
        static const int LibraryTypeCodeStartBit = 12;

        /// <summary>
        /// The maximum value for a class TypeCode field.
        /// </summary>
        static const int ClassTypeCodeMaximum = 0xfff; // That is: (1 << LibraryTypeCodeStartBit - 1)

        /// <summary>
        /// A cache of the type code  for the Filter base class.
        /// </summary>
        /// <remarks>
        /// This is used to set the IsFilter flag.
        /// </remarks>
        static size_t FilterTypeCode;
        /// <summary>
        /// Flag indicates that the class was derived from Filter.
        /// </summary>
        bool IsFilter;
        /// <summary>
        /// The ID of the class.
        /// </summary>
        int ClassID;
        /// <summary>
        /// The ID of the library the class belongs to.
        /// </summary>
        int LibraryID;
        /// <summary>
        /// The number of base classes this class has.
        /// </summary>
        int InheritanceCount;
        /// <summary>
        /// The type code for the class returned by ClassCatalogue.TypeCode().
        /// </summary>
        /// <remarks>
        /// Note: each ClassTypeInfo object's typecode is unique and made from
        /// combining the library ID and class ID: 0xLLCCC.
        /// </remarks>
        size_t TypeCode;
        /// <summary>
        /// The type name of the class.
        /// </summary>
        const string* ClassName;
        /// <summary>
        /// A factory for creating new class instances.
        /// </summary>
        FactoryDelegate Factory;
        /// <summary>
        /// A pointer to the class's immediate base class, or NULL if none.
        /// </summary>
        const ClassTypeInfo* BaseClassTypeInfo;
        /// <summary>
        /// A cache of the class's base type codes.
        /// </summary>
        /// <remarks>
        /// Index 0 is the super base class, InheritanceCount-1 is the immediate base.
        /// </remarks>
        size_t* InheritedTypeCodes;
        /// <summary>
        /// General constructor.
        /// </summary>
        /// <param name="className">The name of the class.</param>
        /// <param name="classID">The ID of the class.</param>
        /// <param name="libraryID">The ID of the library that contains the class.</param>
        /// <param name="factory">A factory for creating new class instances.</param>
        /// <param name="baseClassInfoPtr">The immediate base class that the class was derived from, or NULL.</param>
        ClassTypeInfo(const char* className, int classID, int libraryID, FactoryDelegate factory, const ClassTypeInfo* baseClassInfoPtr);
        /// <summary>
        /// Deallocate object.
        /// </summary>
        ~ClassTypeInfo();
    
        /// <summary>
        /// Produces a hash code for the type by combining library and class IDs.
        /// </summary>
        /// <remarks>
        /// Note: the hashcode is used for ordering purposes and so its value
        /// is unique for each library class.
        /// Specifically, hash code = 0xLLCCC.
        /// </remarks>
        static size_t GetTypeCode(int libraryID, int classID);

        /// <summary>
        /// Finds the closest base ClasssTypeInfo to this type given a table of ClassTypeInfo's.
        /// </summary>
        /// <param name="classes">A table of base ClassTypeInfo objects.</param>
        /// <returns>Returns the closest base type or NULL if none found.</returns>
        const ClassTypeInfo* GetClosestBase(const HashMap& classes);
        
        /// <summary>
        /// Works out the number of inherited classes between two types.
        /// </summary>
        /// <remarks>
        /// <para>
        /// Starts by checking the super-base first, proceeding to the immediate base.
        /// This is optimal for Plato since most inheritance checks are against the super-base.
        /// </para>
        /// <para>
        /// Returns zero if same class, 1 if base is immediate parent, etc.
        /// </para>
        /// </remarks>
        /// <param name="baseType">The base type.</param>
        /// <param name="derivedType">The derived type.</param>
        /// <returns>Returns the inclusive number of derived classes, or -1 if unrelated.</returns>
        static int GetInheritanceDifference(const ClassTypeInfo& baseType, const ClassTypeInfo& derivedType);

        /// <summary>
        /// Creates a ClassTypeInfo::HashMap from a NULL terminated argument list of ClassTypeInfo pointers.
        /// </summary>
        /// <param name="first">The first class info object to add to hash map.</param>
        /// <returns>Returns a ClassTypeInfo::HashMap of the arguments.</returns>
        static HashMap& CreateHashMap(const ClassTypeInfo* first,...);
    };

    /// <summary>
    /// Traits class for ClassTypeInfo* hash_compare.
    /// </summary>
    class ClassTypeInfoPtrHashCompare {	
    public:
	    enum {	// Parameters for hash table.
		    bucket_size = 4, // 0 < bucket_size.
		    min_buckets = 8	 // min_buckets = 2 ^^ N, 0 < N.
        };
        /// <summary>
        /// Default constructor.
        /// </summary>
	    ClassTypeInfoPtrHashCompare() {
		}
        /// <summary>
        /// Fetches the hash code for the ClassTypeInfo.
        /// </summary>
        /// <remarks>
        /// Returns the TypeCode for the class.
        /// </remarks>
        inline size_t operator()(const ClassTypeInfo::ConstClassTypeInfoPtr & key) const { return key->TypeCode; }
        /// <summary>
        /// Less comparator for ClassTypeInfo, used for ordering purposes.
        /// </summary>
        /// <remarks>
        /// Note: each ClassTypeInfo objects type code is unique.
        /// </remarks>
        inline bool operator()(const ClassTypeInfo::ConstClassTypeInfoPtr &xKey, const ClassTypeInfo::ConstClassTypeInfoPtr & yKey) const {
		    return ClassTypeInfoHashCompare(xKey->TypeCode,yKey->TypeCode);
		}
    };
}
