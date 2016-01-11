#pragma once

namespace Plato {
// Path operator definitions.

#pragma region // Div "/" operator.

    // DEBUG: Need to add IdentifierRegex terms.
    // DEBUG: Need to add pointer args to operators.

    /// <summary>
    /// Binary "/" operator.
    /// Append a path node to the end of current path.
    /// </summary>
    /// <param name="c1">The current path node.</param>
    /// <param name="c2">The path node to append.</param>
    /// <returns>Returns the current path node.</returns>
    extern PathNode& operator /(const PathNode& c1, const PathNode& c2);

    /// <summary>
    /// Binary "/" operator.
    /// Append a path node to the end of current path.
    /// </summary>
    /// <param name="c1">The current path node.</param>
    /// <param name="c2">The path node to append.</param>
    /// <returns>Returns the current path node.</returns>
    extern PathNode& operator /(const PathNode& c1, const PathNode* c2);

    /// <summary>
    /// Binary "/" operator.
    /// Append a normal path node with "name" to end of current path.
    /// Checks whether a standard PathNode is being referred to.
    /// </summary>
    /// <param name="c1">The prior path node to add to.</param>
    /// <param name="identifier">The name to give to the new node.</param>
    /// <returns>Returns the prior path node with the new node added.</returns>
    extern PathNode& operator /(const PathNode& c1, const Identifier& identifier);
    
    /// <summary>
    /// Binary "/" operator.
    /// Append a normal path node with "name" to end of current path.
    /// Checks whether a standard PathNode is being referred to.
    /// </summary>
    /// <param name="c1">The prior path node to add to.</param>
    /// <param name="identifier">The name to give to the new node.</param>
    /// <returns>Returns the prior path node with the new node added.</returns>
    extern PathNode& operator /(const PathNode& c1, const Identifier* identifier);

    /// <summary>
    /// Binary "/" operator.
    /// Append a regex path node with to end of current path.
    /// </summary>
    /// <param name="c1">The prior path node to add to.</param>
    /// <param name="identifierRegex">The regex name to give to the new node.</param>
    /// <returns>Returns the prior path node with the new node added.</returns>
    extern PathNode& operator /(const PathNode& c1, const IdentifierRegex& identifierRegex);

    /// <summary>
    /// Binary "/" operator for adding string path.
    /// </summary>
    /// <param name="leftPathNode">The prior path node to add to.</param>
    /// <param name="ident">The ident to add to the path node chain.</param>
    /// <returns>Returns the prior path node with the new ident node added.</returns>
    extern PathNode& operator /(const PathNode& leftPathNode, const string& ident);

    /// <summary>
    /// Binary "/" operator for adding string path.
    /// </summary>
    /// <param name="leftPathNode">The prior path node to add to.</param>
    /// <param name="ident">The ident to add to the path node chain.</param>
    /// <returns>Returns the prior path node with the new ident node added.</returns>
    extern PathNode& operator /(const PathNode& leftPathNode, const char* ident);
#pragma endregion

#pragma region // Grouping "|" operator.
    /// <summary>
    /// Binary "|" grouping operator.
    /// Behaves exactly like the "/" operator, but introduces a group boundary.
    /// </summary>
    /// <param name="c1">The current path node.</param>
    /// <param name="c2">The path node to append.</param>
    /// <returns>Returns the current path node.</returns>
    extern PathNode& operator |(const PathNode& c1, const PathNode& c2);

    /// <summary>
    /// Binary "|" grouping operator.
    /// Behaves exactly like the binary "/" operator, but introduces a group boundary.
    /// </summary>
    /// <param name="leftPathNode">The prior path node to add to.</param>
    /// <param name="identifier">The name to give to the new node.</param>
    /// <returns>Returns the prior path node with the new node added.</returns>
    extern PathNode& operator |(const PathNode& leftPathNode, const Identifier& identifier);

    /// <summary>
    /// Binary "|" grouping operator.
    /// Behaves exactly like the binary "/" operator, but introduces a group boundary.
    /// </summary>
    /// <param name="leftPathNode">The prior path node to add to.</param>
    /// <param name="identifierRegex">The regex name to give to the new node.</param>
    /// <returns>Returns the prior path node with the new node added.</returns>
    extern PathNode& operator |(const PathNode& leftPathNode, const IdentifierRegex& identifierRegex);

    /// <summary>
    /// Binary "|" grouping operator.
    /// Behaves exactly like the binary "/" operator, but introduces a group boundary.
    /// </summary>
    /// <param name="leftPathNode">The prior path node to add to.</param>
    /// <param name="ident">The ident to add to the path node chain.</param>
    /// <returns>Returns the prior path node with the new ident node added.</returns>
    extern PathNode& operator |(const PathNode& leftPathNode, const string& ident);
 
    /// <summary>
    /// Binary "|" grouping operator.
    /// Behaves exactly like the binary "/" operator, but introduces a group boundary.
    /// </summary>
    /// <param name="leftPathNode">The prior path node to add to.</param>
    /// <param name="ident">The ident to add to the path node chain.</param>
    /// <returns>Returns the prior path node with the new ident node added.</returns>
    extern PathNode& operator |(const PathNode& leftPathNode, const char* ident);
#pragma endregion

#pragma region // Search and Director operators. 
    // DBEUG: Need to add Identifier as a Search parameter, etc.
    // DEBUG: Need to add a RegexSearch node.

    /// <summary>
    /// Creates a Director PathNode for use in paths.
    /// </summary>
    /// <remarks>
    /// The route identifier is a normal, non regex, identifier.
    /// </remarks>
    /// <param name="routeIdentifier">The name of the route or property to direct to.</param>
    /// <returns>Returns a director PathNode.</returns>
    extern PathNode& Director(string& routeIdentifier);

    /// <summary>
    /// Creates a Search PathNode for use in paths.
    /// </summary>
    /// <remarks>
    /// The search identifier is a normal, non regex, identifier.
    /// </remarks>
    /// <param name="searchIdentifier">The name of the route or property to search for.</param>
    /// <param name="searchUp">When true, search proceeds up the hierachy first, otherwise down.</param>
    /// <returns>Returns a search PathNode.</returns>
    extern PathNode& Search(string& searchIdentifier, bool searchUp=true);
#pragma endregion

#pragma region // Methods for specifying filters in paths via type or path.
    /// <summary>
    /// Returns a PathNode for the given path to a filter alias.
    /// </summary>
    /// <param name="filterPath">The path to an alias for a filter type or filter derived object
    /// with which to initialise the PathNode.</param>
    /// <param name="arguments">The optional arguments with which to construct the filter object, if necessary.</param>
    /// <returns>Returns a PathNode with the given filter path.</returns>
    extern PathNode& Filter(const Path& filterPath, Arguments* arguments=NULL);

    /// <summary>
    /// Returns a PathNode for the given filter type.
    /// </summary>
    /// <param name="filterType">The filter Type with which to initialise the PathNode.</param>
    /// <param name="arguments">The optional arguments with which to construct the filter object, if necessary.</param>
    /// <returns>Returns a PathNode with the given filter type.</returns>
    extern PathNode& Filter(const ClassTypeInfo& filterType, Arguments* arguments=NULL);    
#pragma endregion

#pragma region // Methods for specifying standard filters in paths.
    /// <summary>
    /// Applies a NameFilter to the path container properties.
    /// </summary>
    /// <param name="regexName">A regex name to match against identifiers of container properties.</param>
    /// <param name="includeWhenMatches">Indicates whether to add items that pass filter, or do not pass.</param>
    /// <returns>Returns a PathNode for a NameFilter.</returns>
    extern PathNode& FilterName(string& regexName, bool includeWhenMatches=true);
    
    /// <summary>
    /// Applies a NameFilter to the path container properties.
    /// </summary>
    /// <param name="regexName">A regex name to match against identifiers of container properties.</param>
    /// <param name="includeWhenMatches">Indicates whether to add items that pass filter, or do not pass.</param>
    /// <returns>Returns a PathNode for a NameFilter.</returns>
    extern PathNode& FilterName(const char* regexName, bool includeWhenMatches=true);

    /// <summary>
    /// Applies a PathFilter to the path container properties.
    /// </summary>
    /// <param name="path">A regex path to match against paths of container properties.</param>
    /// <param name="includeWhenMatches">Indicates whether to add items that pass filter, or do not pass.</param>
    /// <returns>Returns a PathNode for a PathFilter.</returns>
    extern PathNode& FilterPath(Path& path, bool includeWhenMatches=true);

    /// <summary>
    /// Applies a PointFilter to the path container properties.
    /// </summary>
    /// <param name="point">A regx point to match against container properties.</param>
    /// <param name="includeWhenMatches">Indicates whether to add items that pass filter, or do not pass.</param>
    /// <returns>Returns a PathNode for a PointFilter.</returns>
    extern PathNode& FilterPoint(PointRegex& point, bool includeWhenMatches=true);

    /// <summary>
    /// Applies a SearchFilter to the path container properties.
    /// </summary>
    /// <param name="regexIdentifier">A regex Identifier to match during  search for container properties.</param>
    /// <param name="searchUp">Indicates search should proceed up the container hierarchy first.</param>
    /// <param name="includeWhenMatches">Indicates whether to add items that pass filter, or do not pass.</param>
    /// <returns>Returns a PathNode for a SearchFilter.</returns>
    extern PathNode& FilterSearch(IdentifierRegex& regexIdentifier, bool searchUp=true, bool includeWhenMatches=true);

    /// <summary>
    /// Applies a TypeFilter to the path container properties.
    /// </summary>
    /// <param name="classType">A class type to match against container properties.</param>
    /// <param name="includeWhenMatches">Indicates whether to add items that pass filter, or do not pass.</param>
    /// <returns>Returns a PathNode for a TypeFilter.</returns>
    extern PathNode& FilterType(const ClassTypeInfo& classType, bool includeWhenMatches=true);
    
    /// <summary>
    /// Applies a TypeFilter to the path container properties.
    /// </summary>
    /// <param name="regexClassName">A regex class name to match against container properties.</param>
    /// <param name="includeWhenMatches">Indicates whether to add items that pass filter, or do not pass.</param>
    /// <returns>Returns a PathNode for a TypeFilter.</returns>
    extern PathNode& FilterType(const string& regexClassName, bool includeWhenMatches=true);
#pragma endregion
}
