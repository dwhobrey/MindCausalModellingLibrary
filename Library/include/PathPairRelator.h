#pragma once

namespace Plato {

    class Identifier;
    class Property;
    class Container;
    class GroupContainer;
    class Path;
    class Connector;
    class ConnectorRelator;

    /// <summary>
    /// Matches a pair of elements by matching on regex paths.
    /// </summary>
    class PathPairRelator : ConnectorRelator {
    public:
        /// <summary>
        /// A regex path to match against the input path of a connector.
        /// </summary>
        Path* InputRegexPath;
        /// <summary>
        /// A regex path to match against the output path of a connector.
        /// </summary>
        Path* OutputRegexPath;
        /// <summary>
        /// The property requesting the relator test.
        /// </summary>
        const Property* Requester;

        /// <summary>
        /// Constructs a new PathPairRelator, for example, for determining whether a
        /// transducer applies to a pair of connection terminii elements.
        /// </summary>
        /// <param name="parent">A reference to the container that this relator is being applied to.</param>
        /// <param name="requester">A property situated in the container hierarchy of
        /// the properties to test, or NULL if scope checking not required.</param>
        /// <param name="inputRegexPath">A regex path to match against the input property.
        /// For example,".*/.+:InputPad", matches any path with a member derived from InputPad.</param>
        /// <param name="outputRegexPath">A regex path to match against the output property.
        /// For example,".*/.+:OutputPad", matches any path with a member derived from OutputPad.</param>
        PathPairRelator(Container& parent, const Property* requester, Path& inputRegexPath, Path& outputRegexPath);

        /// <summary>
        /// Pair matching method.
        /// Determine whether the relation holds across the specified pair of elements.
        /// </summary>
        /// <param name="connector">The connector being used to interconnect the pair of elements.</param>
        /// <param name="input">The destination for the transduced connection.</param>
        /// <param name="output">The source for the transduced connection.</param>
        /// <returns>Returns true if the relation holds.</returns>
        virtual bool MatchAndValidRelation(Connector& connector, Property& input, Property& output);
    };
}
