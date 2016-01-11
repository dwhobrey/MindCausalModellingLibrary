#pragma once

namespace Plato {

    /// <summary>
    /// String Helper Classes.
    /// </summary>
    class Strings {
    public:
        typedef const string* ConstStringPtr;
        /// <summary>
        /// An empty string.
        /// </summary>
        static const string* EmptyString;
        /// <summary>
        /// Produces a hash code for the string data.
        /// </summary>
        /// <remarks>
        /// All characters in string are used in hash function.
        /// This was developed by Paul Hsieh.
        /// </remarks>
        static uint32_t SuperFastHash(const char * data, int len);

        /// <summary>
        /// Trims characters from ends of string.
        /// </summary>
        /// <param name="s">The string to trim.</param>
        /// <param name="trimCharacters">The characters to trim.</param>
        /// <param name="trimLen">The number of trim characters.</param>
        /// <returns>Returns a ref to the orignal string after trimming.</returns>
        static string& Trim(string& s, const char* trimCharacters=" ",int trimLen=1);

        /// <summary>
        /// Trims characters from the end of a string.
        /// </summary>
        /// <param name="s">The string to trim.</param>
        /// <param name="trimCharacters">The characters to trim.</param>
        /// <param name="trimLen">The number of trim characters.</param>
        /// <returns>Returns a ref to the orignal string after trimming.</returns>
        static string& TrimEnd(string& s, const char* trimCharacters=" ",int trimLen=1);

        /// <summary>
        /// Replace all occurrences of oldText by newText in string.
        /// </summary>
        /// <param name="s">The string to perform the replacement on.</param>
        /// <param name="oldText">The old text to remove.</param>
        /// <param name="newText">The new text to insert.</param>
        /// <returns>Returns a new string with replacements as necessary.</returns>
        static string& Replace(const string& s, const string& oldText, const string& newText);

        /// <summary>
        /// Determines whether the string ends with the specified tail.
        /// </summary>
        /// <param name="s">The string to examine.</param>
        /// <param name="tail">The string to compare against end.</param>
        static bool EndsWith(const string& s, const char* tail);

        /// <summary>
        /// Produces a formatted string according to the format pattern and
        /// variable argument list.
        /// </summary>
        /// <returns>Returns the formatted string, which caller must delete.</returns>
        static char* Sprintf(const char* format,va_list argPtr);

        /// <summary>
        /// Produces a formatted string according to the format pattern and
        /// variable argument list.
        /// </summary>
        /// <returns>Returns the formatted string, which caller must delete.</returns>
        static string& Format(const char* format, ...);

        /// <summary>
        /// Check the string value is not empty,
        /// and initialise if it is.
        /// </summary>
        /// <param name="s">The string variable to check, which may be NULL.</param>
        /// <param name="defaultValue">It's default value.</param>
        /// <param name="reset">When true, forces default value to be assigned to variable.</param>
        static void CheckValue(const string*& s, const string& defaultValue, bool reset);

        /// <summary>
        /// Check the string value is not empty,
        /// and initialise if it is.
        /// </summary>
        /// <param name="s">The string variable to check, which may be NULL.</param>
        /// <param name="defaultValue">It's default value.</param>
        /// <param name="reset">When true, forces default value to be assigned to variable.</param>
        static void CheckValue(const string*& s, const char* defaultValue, bool reset);

        /// <summary>
        /// Deletes the strings in a vector.
        /// </summary>
        /// <param name="strings">The vector whose content is to be deleted.</param>
        static void DeleteStringVectorContents(vector<string*>& strings);

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

// StringHashArgs & StringHashCompare are used to deal with STL hash differences between MS STL and SGI STL.
#ifdef WIN32 
#define StringHashArgs StringPtrHashCompare
#define StringHashCompare(a,b) ((a)<(b))
#else
#define StringHashArgs StringPtrHashCompare,StringPtrHashCompare
#define StringHashCompare(a,b) ((a)==(b))
#endif

    /// <summary>
    /// Traits class for string* hash_compare.
    /// </summary>
    class StringPtrHashCompare {	
    public:
	    enum {	// Parameters for hash table.
		    bucket_size = 4, // 0 < bucket_size.
		    min_buckets = 8	 // min_buckets = 2 ^^ N, 0 < N.
        };
        /// <summary>
        /// Default constructor.
        /// </summary>
	    StringPtrHashCompare() {
		}
        /// <summary>
        /// Fetches the hash code for the String.
        /// </summary>
        inline size_t operator()(const Strings::ConstStringPtr & key) const  {
            return (size_t)(Strings::SuperFastHash(key->data(),(int)(key->size())));
		}
        /// <summary>
        /// Comparator for string, used for ordering purposes, applies StringHashCompare.
        /// </summary>
        inline bool operator()(const Strings::ConstStringPtr &xKey, const Strings::ConstStringPtr & yKey) const {
		    return StringHashCompare(xKey->compare(*yKey),0);
		}
    };
}
