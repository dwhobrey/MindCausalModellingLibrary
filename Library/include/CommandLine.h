#pragma once

namespace Plato {

    /// <summary>
    /// The main purpose of this class is to parse command line
    /// options and arguments into a hashtable of (key,value) pairs
    /// for the options and an array for the arguments.
    /// </summary>
    /// <remarks> Command line syntax:
    /// <list type="table">
    /// <item><description> [executable] [ options ] [ arguments ] </description></item>
    /// <item><description> options: [ options ] ' ' option </description></item>
    /// <item><description> option: '-'['-'] identifer [ '=' value ] </description></item>
    /// <item><description> arguments: [ arguments ] ' ' argument </description></item>
    /// <item><description> argument: string </description></item>
    /// </list>
    /// </remarks>
    class CommandLine {

    public:
        /// <summary>
        /// The hashtable type for storing (string, value) pairs.
        /// </summary>
        typedef hash_map <Strings::ConstStringPtr, Strings::ConstStringPtr, StringHashArgs> HashMap;

#pragma region // Class Variables.
    private:
        /// <summary>
        /// This is normally set to the application's command-line arguments.
        /// </summary>
        static char** mDefaultCommandLineArguments;
        /// <summary>
        /// Indicates when the default arguments starts with the executable path.
        /// </summary>
        static bool mDefaultHasExecutable;
#pragma endregion

#pragma region // Instance Variables.
    private:
        /// <summary>
        /// The command line options preceeding any arguments after they have been split
        /// into (key=string*,value=string*) pairs.
        /// The key is the corresponding ClassProperties derived class variable name.
        /// </summary>
        HashMap* mCommandLineOptions;

        /// <summary>
        /// The arguments found on the command line.
        /// </summary>
        vector<const string*>* mArguments;

        /// <summary>
        /// The original command line arguments before any processing.
        /// </summary>
        char** mRawCommandLineArguments;

        /// <summary>
        /// Indicates whether the raw command line arguments includes an initial executable element.
        /// </summary>
        bool mHasExecutable;

        /// <summary>
        /// A cache of the command line executable - if there was one, otherwise NULL.
        /// </summary>
        const string* mExecutable;
#pragma endregion

#pragma region // Constructor.
    public:
        /// <summary>
        /// Constructor for parsing the given list of options and arguments.
        /// </summary>
        /// <param name="rawCommandLineArguments">The options and arguments to parse.
        /// If NULL, this will be set to the default command line arguments.</param>
        /// <param name="hasExecutable">When true, indicates that the first element is the application executable.</param>
        CommandLine(char** rawCommandLineArguments=mDefaultCommandLineArguments, bool hasExecutable=mDefaultHasExecutable);
        /// <summary>
        /// Deallocate options hash map etc.
        /// </summary>
        ~CommandLine();
#pragma endregion

#pragma region // Accessors.
    public:
        /// <summary>
        /// Accessor for the default command line arguments.
        /// By default, this is set to Environment.GetCommandLineArgs().
        /// An application may override this at startup before Configuration
        /// is called.
        /// </summary>
        static void SetDefaultCommandLineArguments(char** arguments, bool hasExecutable);
        /// <summary>
        /// Gets the default command line arguments.
        /// </summary>
        /// <remarks>These will either be the environment arguments,
        /// or an alternative set of arguments that was set by the application.</remarks>
        /// <returns>Returns the command line arguments as a string array.</returns>
        char** GetDefaultCommandLineArguments();

        /// <summary>
        /// Gets the command line arguments as they would be given on the command line.
        /// </summary>
        /// <returns>Returns the command line arguments as a string array.</returns>
        char** GetCommandLineArguments();

        /// <summary>
        /// Gets the value of the command line option with the given name.
        /// </summary>
        /// <param name="optionName">Option name to get value of.</param>
        /// <returns>Returns the value of the named option as a string, or NULL if value not set.</returns>
        const string* GetOptionValue(const string& optionName);

        /// <summary>
        /// Gets the value of the command line option with the given name.
        /// Returns the default value if option not found.
        /// </summary>
        /// <param name="optionName">Option name to get value of.</param>
        /// <param name="defaultValue">The default value for the option if not found.</param>
        /// <returns>Returns the value cast to a string.</returns>
        const string& GetOptionValue(const string& optionName, const string& defaultValue);

        /// <summary>
        /// Gets the value of the command line option with the given name.
        /// Returns the default value if option not found.
        /// </summary>
        /// <param name="optionName">Option name to get value of.</param>
        /// <param name="defaultValue">The default value for the option if not found.</param>
        /// <returns>Returns the value cast to a long.</returns>
        long GetOptionValue(const string& optionName, long defaultValue);

        /// <summary>
        /// Gets the value of the command line option with the given name.
        /// Returns the default value if option not found.
        /// </summary>
        /// <param name="optionName">Option name to get value of.</param>
        /// <param name="defaultValue">The default value for the option if not found.</param>
        /// <returns>Returns the value cast to a bool.</returns>
        bool GetOptionValue(const string& optionName, bool defaultValue);

        /// <summary>
        /// Gets the command line executable, if there was one.
        /// </summary>
        /// <returns>Returns the executable or NULL if none present.</returns>
        const string* GetExecutable();

        /// <summary>
        /// Gets the arguments, i.e. non-options.
        /// </summary>
        /// <returns>Returns a vector of the arguments found, which may be empty.</returns>
        vector<const string*>& GetArguments();
        
        /// <summary>
        /// Gets the command line options.
        /// </summary>
        /// <returns>Returns the options in a CommandLine::HashMap.</returns>
        HashMap& GetCommandLineOptions();

#pragma endregion

#pragma region // Public methods.
    public:
        /// <summary>
        /// Checks whether a command line option is present.
        /// </summary>
        /// <param name="optionName">Option name to check for.</param>
        /// <returns>Returns true if option is present.</returns>
        bool ContainsOption(const string& optionName);
#pragma endregion
    };
}
