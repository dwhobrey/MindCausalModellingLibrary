#pragma once

namespace Plato {

    class Configuration;

    /// <summary>
    /// Manages initialisation and substitution of environment variables in strings.
    /// </summary>
    class EnvironmentVariables {
    public:
        /// <summary>
        /// Gets the value of the environment variable without consulting configuration or command line arguments.
        /// </summary>
        /// <param name="name">The name of the variable to fetch.</param>
        /// <returns>Returns the value of the variable or the empty string.</returns>
        static string& GetRawVariable(const string &name);

        /// <summary>
        /// Gets the value of an environment variable.
        /// First checks if value was given on the command line.
        /// Next checks environment.
        /// Finally checks for a default value.
        /// </summary>
        /// <param name="configuration">The configuration to check for overridden environment variables.</param>
        /// <param name="name">The name of the variable to get the value of.</param>
        /// <returns>Returns the variable's value, or NULL if not set.</returns>
        static string& GetVariable(const Configuration& configuration, const string& name);

        /// <summary>
        /// Substitutes the named environment variables appearing in the string with their values.
        /// Environment variables must be enclosed by brackets and preceeded by the dollar character.
        /// Uses the EnvironmentVariables.GetVariable method for resolving values.
        /// </summary>
        /// <param name="configuration">The configuration to check for overridden environment variables.</param>
        /// <param name="text">The string to perform substitution on.</param>
        /// <returns>Returns the expanded form of the text.</returns>
        static string& ExpandString(const Configuration& configuration, const string& text);
    };
}

