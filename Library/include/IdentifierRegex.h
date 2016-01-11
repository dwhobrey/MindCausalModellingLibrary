#pragma once

namespace Plato {

    class PointRegex;
    class RegexMatcher;
    class Identifier;

    /// <summary>
    /// This class holds intermediary precompiled identifier Regex terms
    /// in order to improve the efficieny of regex identifier matches in loops.
    /// </summary>
    class IdentifierRegex {
    public:
        /// <summary>
        /// The identifier's name.
        /// </summary>
        const string* Name;
        /// <summary>
        /// The simple TypeName.
        /// </summary>
        const string* TypeName;
        /// <summary>
        /// The identifier's Position.
        /// </summary>
        const PointRegex* Position;

    private:
        /// <summary>
        /// Compiled version of Identifier's Name.
        /// </summary>
        RegexMatcher* mRegexName;
        /// <summary>
        /// Compiled version of Identifier's TypeName.
        /// </summary>
        RegexMatcher* mRegexTypeName;
        /// <summary>
        /// Indicates whether the Name is the wild pattern.
        /// </summary>
        bool mNameIsWild;
        /// <summary>
        /// Cached flag indicating whether the Name is a regex.
        /// </summary>
        bool mNameIsRegex;
        /// <summary>
        /// Indicates whether TypeName is the wild pattern.
        /// </summary>
        bool mTypeNameIsWild;
        /// <summary>
        /// Cached flag indicating whether the TypeName is a regex.
        /// </summary>
        bool mTypeNameIsRegex;

    public:
        /// <summary>
        /// A regular expression identifier is used for matching against
        /// fixed identifiers.
        /// The parts of a regex identifier can be regular expressions, or NULL.
        /// </summary>
        /// <remarks>Regex Identifier's are not associated with a property,
        /// so Identifier.Property always returns NULL.
        /// The objects passed in are then owned by the identifier.</remarks>
        /// <param name="regexName">A regex identifier name.</param>
        /// <param name="regexPosition">A regex position.</param>
        /// <param name="regexTypeName">A regex simple type name.</param>
        IdentifierRegex(const string* regexName, const PointRegex* regexPosition=NULL, const string* regexTypeName=NULL);

        /// <summary>
        /// Copy constructor.
        /// </summary>
        /// <param name="regexIdentifier">The regex identifer to copy.</param>
        IdentifierRegex(const IdentifierRegex& regexIdentifier);

        /// <summary>
        /// Release resources.
        /// </summary>
        ~IdentifierRegex();

        /// <summary>
        /// Gets a string representation of the identifier.
        /// </summary>
        /// <returns>Returns the string "Name[Position]:TypeName". Null terms are skipped, so the return string may be empty.</returns>
        string& StatusReport() const;

        /// <summary>
        /// Determines whether the given identifier would match this regex identifier pattern.
        /// </summary>
        /// <remarks>
        /// Null regex terms are ignored in the match.
        /// Null identifier terms will only match the wild pattern.
        /// </remarks>
        /// <param name="ident">The identifier to check for a match.</param>
        /// <returns>Returns true if the identifier matches.</returns>
        bool Match(const Identifier& ident) const;

        /// <summary>
        /// Determines whether the given regex identifier would match this regex identifier pattern.
        /// </summary>
        /// <remarks>
        /// Null regex terms are ignored in the match.
        /// Null identifier terms will only match the wild pattern or a NULL regex term.
        /// Comparing two regex strings is limited to them both being the wild pattern.
        /// </remarks>
        /// <param name="regexIdentifier">The identifier to check for a match.</param>
        /// <returns>Returns true if the identifier matches.</returns>
        bool PatternMatch(const IdentifierRegex& regexIdentifier) const;
    };
}
