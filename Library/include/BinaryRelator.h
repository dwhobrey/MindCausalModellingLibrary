#pragma once

namespace Plato {

    class Property;
    class Container;
    class Identifier;
    class IdentifierRegex;
    class Point;
    class Relator;

    /// <summary>
    /// Models a relation between two elements where the position of the second
    /// element is given relative to the first via a relative spherical point.
    /// </summary>
    class BinaryRelator : public ProximityRelator {
    public:
        /// <summary>
        /// The relative position of the dependent point.
        /// </summary>
        const Point* RelativePosition;

        /// <summary>
        /// The name of element A.
        /// </summary>
        Identifier* ElementNameA;

        /// <summary>
        /// The name of element B.
        /// </summary>
        Identifier* ElementNameB;

        /// <summary>
        /// Constructs a new relation between two elements.
        /// </summary>
        /// <param name="relativePosition">The position of B relative to A,
        /// encoded as a spherical point.</param>
        /// <param name="elementNameA">Name of the first element.</param>
        /// <param name="elementNameB">Name of the second element.</param>
        BinaryRelator(const Point& relativePosition, Identifier& elementNameA, Identifier& elementNameB);
    };

    /// <summary>
    /// Models two elements where the first is on the left of the second.
    /// Note: we might want to specify elements via general wildcards,
    /// hence the use of string names rather than element refs.
    /// </summary>
    class NextXRelator : public BinaryRelator {
    public:
        /// <summary>
        /// A relative point that corresponds to a position to the left of a reference point.
        /// </summary>
        static const Point* NextXPosition;

        /// <summary>
        /// Construct a new relational locator.
        /// </summary>
        /// <param name="elementNameA">Name of first element.</param>
        /// <param name="elementNameB">Name of second element.</param>
        NextXRelator(Identifier& elementNameA, Identifier& elementNameB);

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
