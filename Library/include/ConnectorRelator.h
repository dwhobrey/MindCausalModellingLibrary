#pragma once

namespace Plato {

    class Property;
    class Identifier;
    class Container;
    class GroupContainer;
    class Relator;
    class Connector;

    /// <summary>
    /// Connector relators are used by Transducers for specifying
    /// a 1-1-arity or M-N-arity relational pattern between two elements or sets of elements,
    /// for example, a pair of elements or a set of input and a set of output elements
    /// that are to be interconnected.
    /// </summary>
    class ConnectorRelator : public Relator {
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
        /// Default constructor.
        /// </summary>
        /// <param name="parent">A reference to the container that this relator is being applied to.</param>
        ConnectorRelator(Container& parent);

        /// <summary>
        /// Group matching method.
        /// Determine whether the relation holds across the specified ordered tuples of elements.
        /// </summary>
        /// <remarks>
        /// Connector properties determine how the input and output sets are to be connected.
        /// This default implementation always returns false.
        /// </remarks>
        /// <param name="connector">The connector being used to interconnect the set of elements.</param>
        /// <param name="inputs">An ordered tuple of input elements upon which to test whether the relation holds.</param>
        /// <param name="outputs">An ordered tuple of output elements upon which to test whether the relation holds.</param>
        /// <returns>Returns true if the relation holds.</returns>
        virtual bool MatchAndValidRelation(Connector& connector, GroupContainer& inputs, GroupContainer& outputs);

        /// <summary>
        /// Pair matching method.
        /// Determine whether the relation holds across the specified pair of elements.
        /// </summary>
        /// <remarks>
        /// Connector properties determine how the input and output elements are to be connected.
        /// This default implementation always returns false.
        /// </remarks>
        /// <param name="connector">The connector being used to interconnect the pair of elements.</param>
        /// <param name="input">The destination for the transduced connection.</param>
        /// <param name="output">The source for the transduced connection.</param>
        /// <returns>Returns true if the relation holds.</returns>
        virtual bool MatchAndValidRelation(Connector& connector, Property& input, Property& output);

        /// <summary>
        /// Accessor for the input arity of this relator.
        /// This is the input dimension of the relation, e.g. number of input element patterns.
        /// Some relators may have a variable arity with an upper limit set by the
        /// number of elements in the connector input path.
        /// </summary>
        /// <remarks>This default implementation returns 1.</remarks>
        virtual int GetInputArity();

        /// <summary>
        /// Accessor for the output arity of this relator.
        /// This is the output dimension of the relation, e.g. number of output element patterns.
        /// Some relators may have a variable arity with an upper limit set by the
        /// number of elements in the connector output path.
        /// </summary>
        /// <remarks>This default implementation returns 1.</remarks>
        virtual int GetOutputArity();
    };
}
