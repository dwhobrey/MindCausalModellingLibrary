#pragma once

namespace Plato {

    class Property;
    class Container;
    class Identifier;
    class IdentifierRegex;
    class Point;
    class Relator;

#pragma region // ProximityRelator class.
    /// <summary>
    /// Proximity relators are used by the proximity converters for specifying relational patterns
    /// that are used as constraints between the positions of elements when determining
    /// whether a converter is applicable.
    /// </summary>
    class ProximityRelator : public Relator {
    public:
        /// <summary>
        /// Set to true if the relator is symmetric in its arguments,
        /// i.e. the order of arguments has no effect on validity.
        /// </summary>
        bool IsSymmetric;
        /// <summary>
        /// A reference to the container that this relator is being applied to.
        /// </summary>
        Container* Parent;

        /// <summary>
        /// This holds the corresponding element pattern for each dimension of the relationship.
        /// </summary>
        vector<IdentifierRegex*>* Patterns;

        /// <summary>
        /// Standard constructor.
        /// </summary>
        /// <param name="patterns">The pattern of identifiers this relator matches.</param>
        ProximityRelator(vector<IdentifierRegex*>* patterns);

        /// <summary>
        /// Deallocate object.
        /// </summary>
        ~ProximityRelator();

        /// <summary>
        /// Returns the arity of this relator.
        /// This is typically the dimension of the relation, e.g. number of element patterns.
        /// Some relators may have a variable arity with an upper limit set by the
        /// number of elements in the parent container.
        /// </summary>
        int GetArity();

        /// <summary>
        /// Determine whether the relation holds across the specified ordered tuple of elements.
        /// </summary>
        /// <remarks>
        /// This method assumes that the elements have already been tested against the patterns, e.g. via Matches.
        /// This default implementation always returns true.
        /// </remarks>
        /// <param name="elements">
        /// An ordered tuple of elements upon which to test whether the relation holds.
        /// Only tuples of elements that have matched the pattern list should be passed in.
        /// </param>
        /// <returns>Returns true if the relation holds.</returns>
        virtual bool ValidRelation(vector<Property*>& elements);

        /// <summary>
        /// Determines whether the elements match the relator patterns
        /// when compared in indexical order.
        /// </summary>
        /// <param name="elements">An array of elements whose Length must equal Arity.</param>
        /// <returns>Returns true if there are no relator patterns, or the elements match the patterns.</returns>
        bool Matches(vector<Property*>& elements);

        /// <summary>
        /// Determine whether the relation holds across the specified ordered tuples of elements.
        /// </summary>
        /// <remarks>
        /// This implementation calls Matches and ValidRelation.
        /// </remarks>
        /// <param name="elements">An array of elements whose Length must equal Arity.</param>
        /// <returns>Returns true if the relation holds.</returns>
        bool MatchAndValidRelation(vector<Property*>& elements);

        /// <summary>
        /// Search for pattern in vector.
        /// </summary>
        /// <param name="pattern">The pattern to search for.</param>
        /// <param name="searchVector">The vector to search.</param>
        /// <returns>Returns its' index if found, or the one's complement if not found.</returns>
        static int BinarySearch(vector<int>& pattern, vector<vector<int>*>& searchVector);

        /// <summary>
        /// Find all the unique tuple of elements that match this relator's patterns.
        /// </summary>
        /// <remarks>
        /// <para>
        /// The objective is to find all the unique element tuples that match this relator.
        /// </para>
        /// <para>
        /// Algorithm:
        /// 1) For the relator build a list of lists of matches for each pattern in relator:
        ///    a) For each pattern in relator loop over elements finding those that match.
        ///    b) Store element-order index (EOI) for speed in lists.
        /// 2) Compute unique sets:
        ///    For the relator, if arity is n, build a list of n-arity tuples:
        ///    a) Construct an n-hierarchy of loops to construct tuples.
        ///    b) Sort tuples on first n elements.
        ///    c) Remove duplicates.
        /// </para>
        /// </remarks>
        /// <param name="container">The container upon which to apply the relator.</param>
        /// <returns>Returns a list of tuples.</returns>
        vector<vector<Property*>*>* FindMatches(Container& container);
    };
#pragma endregion

#pragma region // Example Proximity Relators: DEBUG this needs fixing.
    /// <summary>
    /// Models the positioning of elements via coordinates.
    /// Specifies the position of a subelement in its parent coordinate space.
    /// </summary>
    /// <remarks>
    /// A PointLocator is used when the position of an element needs to
    /// be specified in terms of a relator when used in a converter.
    /// DEBUG: The Identifer parameter may be redundant.
    /// </remarks>
    class PointRelator : public ProximityRelator {
    public:
        /// <summary>
        /// The name of the element.
        /// </summary>
        Identifier* ElementIdent;
        /// <summary>
        /// The position of the element given as a point coordinate.
        /// </summary>
        const Point* ElementPoint;

        /// <summary>
        /// Constructs a new PointLocator for a named element
        /// located at a specified point.
        /// </summary>
        /// <param name="elementIdent">The name of the element.</param>
        /// <param name="elementPoint">The location given as a point.</param>
        PointRelator(Identifier& elementIdent, const Point& elementPoint);
    };

    // DEBUG: What about higher arity relators?
#pragma endregion
}
