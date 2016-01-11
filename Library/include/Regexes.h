#pragma once

namespace Plato {

    /// <summary>
    /// A wrapper class around the underlying regular expression package.
    /// </summary>
    /// <remarks>
    /// Hides the package implementation.
    /// </remarks>
    class RegexMatcher {

    public:
        union {
            /// <summary>
            /// The regular expression pattern in char array form.
            /// </summary>
            const char* PatternCharPtr;
            /// <summary>
            /// The regular expression pattern in string form.
            /// </summary>
            const string* PatternString;
        };
        /// <summary>
        /// When true, indicates that this RegexMatcher instance should use the string pattern
        /// otherwise it should use the char array pattern.
        /// </summary>
        bool IsPatternString;

    private:
        /// <summary>
        /// The regular expression as represented by the regex package.
        /// </summary>
        void* RegexPatternPtr;
        /// <summary>
        /// Extra information for use by the regex package.
        /// </summary>
        void* RegexExtraPtr;

    public:
        /// <summary>
        /// Construct a new Regex object for pattern.
        /// </summary>
        /// <remarks>The caller is responsible for deleting pattern.</remarks>
        RegexMatcher(const char* pattern);
        /// <summary>
        /// Construct a new Regex object for pattern.
        /// </summary>
        /// <remarks>The caller is responsible for deleting pattern.</remarks>
        RegexMatcher(const string* pattern);
        /// <summary>
        /// Deallocates Regex objects.
        /// </summary>
        ~RegexMatcher();
        /// <summary>
        /// Determines whether the text satisfies the pattern.
        /// </summary>
        /// <param name="text">The text to check for a regex match.</param>
        /// <returns>Returns true if text satisfied the pattern.</returns>
        bool Matches(const char* text);
        /// <summary>
        /// Determines whether the text satisfies the pattern.
        /// </summary>
        /// <param name="text">The text to check for a regex match.</param>
        /// <returns>Returns true if text satisfied the pattern.</returns>
        bool Matches(const string& text);
    };

    /// <summary>
    /// Regular Expression Helper Class.
    /// </summary>
    class Regexes {
    public:
        /// <summary>
        /// The characters whose occurrence in a string imply
        /// it is a regular expression rather than just an identifier.
        /// </summary>
        static const char RegexSpecialCharacters[12];

        /// <summary>
        /// This regex pattern should match a single identifier.
        /// </summary>
        static const string* WildPatternString;

        /// <summary>
        /// This regex pattern should match a sequence of identifiers (e.g. a path).
        /// </summary>
        static const string* AnyPatternString;

        /// <summary>
        /// Determine whether input text matches the regular expression.
        /// </summary>
        /// <remarks>To match all of the input text use the start and end anchors '^' and '$': "^text$".</remarks>
        /// <param name="inputText">The text to examine.</param>
        /// <param name="regularExpression">The regular expression to match against.</param>
        /// <returns>Returns true if a regular expression match occurs within the input text.</returns>
        static bool Matches(const char* inputText, const char* regularExpression);

        /// <summary>
        /// Determine whether input text matches the regular expression.
        /// </summary>
        /// <remarks>To match all of the input text use the start and end anchors '^' and '$': "^text$".</remarks>
        /// <param name="inputText">The text to examine.</param>
        /// <param name="regularExpression">The regular expression to match against.</param>
        /// <returns>Returns true if a regular expression match occurs within the input text.</returns>
        static bool Matches(const string& inputText, const char* regularExpression);

        /// <summary>
        /// Determine whether input text matches the regular expression.
        /// </summary>
        /// <remarks>To match all of the input text use the start and end anchors '^' and '$': "^text$".</remarks>
        /// <param name="inputText">The text to examine.</param>
        /// <param name="regularExpression">The regular expression to match against.</param>
        /// <returns>Returns true if a regular expression match occurs within the input text.</returns>
        static bool Matches(const string& inputText, const string& regularExpression);

        /// <summary>
        /// Determines whether the input text is a regular expression
        /// or a simple string.
        /// </summary>
        /// <param name="inputText">The text to check for being a regular expression.</param>
        /// <returns>Returns true if the text is a regular expression.</returns>
        static bool IsRegex(const string& inputText);

        /// <summary>
        /// Determines whether the input text is a regular expression
        /// or a simple string.
        /// </summary>
        /// <param name="inputText">The text to check for being a regular expression.</param>
        /// <returns>Returns true if the text is a regular expression.</returns>
        static bool IsRegex(const char* inputText);

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
