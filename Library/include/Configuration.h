#pragma once

namespace Plato {

    class Configuration;
    class CommandLine;

#pragma region // VariableAccessor class.
    /// <summary>
    /// This class is used for accessing configuration variables by string name.
    /// </summary>
    class VariableAccessor {   
    public:
        /// <summary>
        /// An alias for a pointer to a class member.
        /// </summary>
        typedef const void* Configuration::*VariablePtr;
        /// <summary>
        /// An alias for a pointer to a variable accessor function.
        /// </summary>
        typedef const void* (*AccessorPtr)(const void* varAdrs,const void* valuePtr,bool isSetting,bool isString);
        /// <summary>
        /// Indicates that the variable is a static class member.
        /// </summary>
        bool IsStatic;
        union {
            /// <summary>
            /// A pointer to the variable to be accessed.
            /// </summary>
            VariablePtr VarPtr;
            /// <summary>
            /// A pointer to a static variable to be accessed.
            /// </summary>
            const void* StaticPtr;
        };
        /// <summary>
        /// A pointer to the accessor function for the variable.
        /// </summary>
        AccessorPtr AccPtr;
        /// <summary>
        /// Constructus a new VariableAccessor for a Configuration variable.
        /// </summary>
        VariableAccessor(VariablePtr varPtr, AccessorPtr accPtr);
        /// <summary>
        /// Constructus a new VariableAccessor for a static Configuration variable.
        /// </summary>
        VariableAccessor(const void* varPtr, AccessorPtr accPtr);
        /// <summary>
        /// An accessor function for string variables.
        /// </summary>
        /// <param name="varAdrs">The address of the variable to access.</param>
        /// <param name="valuePtr">A pointer to the new value, if setting.</param>
        /// <param name="isSetting">Indicates when the variable is being set.</param>
        /// <param name="isString">Indicates that the input value is a string,
        /// or the return value should be a string.</param>
        /// <returns>
        /// Returns a pointer to the variable value. 
        /// If isString=true, return ptr value will be to a new string, which the caller owns.
        /// If isString=false, return ptr value will be the address of the variable.
        /// </returns>
        static const void* StringAccessor(const void* varAdrs,const void* valuePtr,bool isSetting,bool isString);
        /// <summary>
        /// An accessor function for int variables.
        /// </summary>
        /// <param name="varAdrs">The address of the variable to access.</param>
        /// <param name="valuePtr">A pointer to the new value, if setting.</param>
        /// <param name="isSetting">Indicates when the variable is being set.</param>
        /// <param name="isString">Indicates that the input value is a string,
        /// or the return value should be a string.</param>
        /// <returns>
        /// Returns a pointer to the variable value. 
        /// If isString=true, return ptr value will be to a new string, which the caller owns.
        /// If isString=false, return ptr value will be the address of the variable.
        /// </returns>
        static const void* IntAccessor(const void* varAdrs,const void* valuePtr,bool isSetting,bool isString);
        /// <summary>
        /// An accessor function for long variables.
        /// </summary>
        /// <param name="varAdrs">The address of the variable to access.</param>
        /// <param name="valuePtr">A pointer to the new value, if setting.</param>
        /// <param name="isSetting">Indicates when the variable is being set.</param>
        /// <param name="isString">Indicates that the input value is a string,
        /// or the return value should be a string.</param>
        /// <returns>
        /// Returns a pointer to the variable value. 
        /// If isString=true, return ptr value will be to a new string, which the caller owns.
        /// If isString=false, return ptr value will be the address of the variable.
        /// </returns>
        static const void* LongAccessor(const void* varAdrs,const void* valuePtr,bool isSetting,bool isString);
        /// <summary>
        /// An accessor function for bool variables.
        /// </summary>
        /// <param name="varAdrs">The address of the variable to access.</param>
        /// <param name="valuePtr">A pointer to the new value, if setting.</param>
        /// <param name="isSetting">Indicates when the variable is being set.</param>
        /// <param name="isString">Indicates that the input value is a string,
        /// or the return value should be a string.</param>
        /// <returns>
        /// Returns a pointer to the variable value. 
        /// If isString=true, return ptr value will be to a new string, which the caller owns.
        /// If isString=false, return ptr value will be the address of the variable.
        /// </returns>
        static const void* BoolAccessor(const void* varAdrs,const void* valuePtr,bool isSetting,bool isString);
    };
#pragma endregion

    /// <summary>
    /// This manages all the configuration properties for a model.
    /// </summary>
    class Configuration {

#pragma region // Information fields.
    public:
        /// <summary>
        /// A list of those that have contributed to the Plato project in one way or another.
        /// </summary>
        static const char* Acknowledgements;
        /// <summary>
        /// The copyright notice.
        /// </summary>
        static const char* Copyright;
        /// <summary>
        ///  Options recognised by the library.
        /// </summary>
        static const char* OptionUsage; 
        /// <summary>
        /// The name, version and date of this build of the library.
        /// </summary>
        static const char* Version;
#pragma endregion

        /// <summary>
        /// The platform specific directory separator char.
        /// </summary>
        static const char PlatformDirectorySeparator = 
#ifdef WIN32
            '\\';
#else
            '/';
#endif

#pragma region // Option instance variables.
    public:
        /// <summary>
        /// Set to a non-zero value for debug info.
        /// </summary>
        static int DebugLevel;
        /// <summary>
        /// Set to a non-zero value for trace info.
        /// </summary>
        static int TraceLevel;
        /// <summary>
        /// When set to true, serialization is in binary format, otherwise XML.
        /// </summary>
        bool BinarySerialize;
        /// <summary>
        /// When true, diagnostics are performed when constructing a model.
        /// </summary>
        bool RunDiagnostics;
        /// <summary>
        /// The number of epochs to run task for.
        /// </summary>
        long EpochCount;
        /// <summary>
        /// The local path to the serialised state data to restore/save model from/to.
        /// </summary>
        const string* ArchivePath;
        /// <summary>
        /// The name of the config file that was loaded.
        /// If NULL, this will be initialised to a default value.
        /// </summary>
        const string* ConfigFile;
        /// <summary>
        /// A directory prefix to append to paths when referring to host files.
        /// </summary>
        const string* PathPrefix;
        /// <summary>
        /// The installation directory of the Plato system,
        /// used as a path prefix to prepend to hard paths.
        /// The default value is the drive upon which Plato was installed, such as "C:\".
        /// </summary>
        const string* PlatoPrefix;
#pragma endregion

#pragma region // General instance variables.
    public:
        /// <summary>
        /// The parsed options and arguments for the model.
        /// </summary>
        CommandLine* ModelParameters;
#pragma endregion

#pragma region // Class variables.
    private:
        /// <summary>
        /// Type used for mapping configurable variable names to its associated accessor.
        /// </summary>
        typedef hash_map <Strings::ConstStringPtr, const VariableAccessor*, StringHashArgs> HashMap;
        /// <summary>
        /// The full path of the executable for the model.
        /// </summary>
        static string* mFullPathPtr;
        /// <summary>
        /// The full path of the directory containing the application executable.
        /// </summary>
        static string* mDirectoryPtr;
        /// <summary>
        /// Maps variable name to a VariableAccessor for accessing property variable.
        /// </summary>
        static HashMap* mVariableAccessorHashMap;
#pragma endregion

#pragma region // Constructors.
    public:
        /// <summary>
        /// Sets up a new set of options with specified values.
        /// </summary>
        /// <remarks>
        /// <para>
        /// An options value is determined in the following steps: 1) from a value passed in via the arguments, 
        /// 2) or from a configuration file, 3) otherwise it is given a default value.
        /// </para>
        /// <para>
        /// The following options are recognised (note: option names are case sensitive):
        /// <list type="table">
        /// <listheader><term>Option Name</term><description>Description</description></listheader>
        /// <item><term><b>--acknowledgements</b></term><description>Display copyright and contribution information.</description></item>
        /// <item><term><b>--ArchivePath=</b><i>FilePath</i></term><description>Local path to file containing previously archived model state.</description></item>
        /// <item><term><b>--BinarySerialize=</b><i>true|false</i></term><description>Indicates to serialise in binary rather than xml.</description></item>
        /// <item><term><b>--ConfigFile=</b><i>FilePath</i></term><description>Use this configuration file.</description></item>
        /// <item><term><b>--DebugLevel=</b><i>Number</i></term><description>Set to a postive integer for increasing debug information.</description></item>
        /// <item><term><b>--RunDiagnostics=</b><i>true|false</i></term><description>Turn diagnostic tests on or off.</description></item>
        /// <item><term><b>--EpochCount=</b><i>Number</i></term><description>The number of epochs to run model for, default = -1, i.e. non-stop.</description></item>
        /// <item><term><b>--help</b></term><description>Display usage information.</description></item>
        /// <item><term><b>--PathPrefix=</b><i>DirPath</i></term><description>Use this as a prefix for file system paths.</description></item>
        /// <item><term><b>--PlatoPrefix=</b><i>DirPath</i></term><description>The installation directory of the Plato system.</description></item>
        /// <item><term><b>--TraceLevel=</b><i>Number</i></term><description>Set to a postive integer for increasing trace information.</description></item>
        /// <item><term><b>--version</b></term><description>Display version information.</description></item>
        /// </list>
        /// </para>
        /// </remarks>
        /// <example>
        /// <para><b>Example: Running a model</b></para>
        /// <para>
        /// The following command starts a model, called <c>HelloWorldModel.exe</c>, 
        /// with the model arguments <c>a</c> and <c>2</c>
        /// and the following option: <c>--EpochCount=100</c>.
        /// <code>HelloWorldModel --EpochCount=100 a 2</code>
        /// <seealso cref="Model::Model"/>
        /// </para>
        /// </example>
        /// <param name="modelName">The name of the model this configuration belongs to.
        /// Used to set the default config file name.</param>
        /// <param name="arguments">The arguments to pass to the model, or NULL.</param>
        Configuration(const string& modelName, char* arguments[]);

        /// <summary>
        /// Release model configuration resources.
        /// </summary>
        ~Configuration();

    private:
        /// <summary>
        /// Checks option values are valid.
        /// </summary>
        /// <param name="reset">When true, forces default value to be assigned to options.</param>
        /// <remarks>This also assigns default values.
        /// When adding new configuration variables, add an entry to this method.
        /// </remarks>
        void CheckOptionValues(bool reset);
#pragma endregion

#pragma region // Accessors.
    public:
        /// <summary>
        /// Get the full path and name of the application executable.
        /// </summary>
        static const string& GetExecutableFullPath();

        /// <summary>
        /// Get the directory path to where the application executable resided.
        /// </summary>
        static const string& GetExecutableDirectory();

        /// <summary>
        /// Get the file name and extension of the application executable.
        /// </summary>
        static string& GetExecutableFileName();
#pragma endregion

#pragma region // Public methods.
    public:
        /// <summary>
        /// Checks if an informational option was used on the command line that
        /// implies the model should not start up.
        /// </summary>
        /// <remarks>
        /// If a terminal option, such as "help" is encountered, a message is sent to stdout.
        /// </remarks>
        /// <param name="arguments">The NULL terminated array of string arguments to check, or NULL.</param>
        /// <returns>Returns true if a terminal option was encountered.</returns>
        static bool CheckForTerminalOption(char **arguments);

        /// <summary>
        /// Fetch a configuration value via its name.
        /// </summary>
        /// <param name="name">The name of the property to find.</param>
        /// <param name="isString">Indicates whether to return it's value
        /// as a pointer to a string, or a pointer to it's binary value.</param>
        /// <returns>If found, returns a pointer to the value, otherwise NULL.</returns>
        const void* GetProperty(const string& name,bool isString) const;

        /// <summary>
        /// Set the name of a configuration value via its name.
        /// </summary>
        /// <param name="name">The name of the property to set.</param>
        /// <param name="value">A pointer to the new value.</param>
        /// <param name="isString">Indicates whether the new value is a
        /// string or in binary form.</param>
        void SetProperty(const string& name, const void* value,bool isString);

        /// <summary>
        /// Read in the model properties from the config file
        /// and then merge the new properties with the default values
        /// and any model arguments.
        /// </summary>
        /// <remarks>
        /// Each line of the config file can be either a comment or set a variable.
        /// Comment lines must start with a hash character, '#'.
        /// Lines setting a variable, must start with the variable name, which is
        /// case sensitive, followed by the equals characters, '=', and then the
        /// variable's value, which is the string of all characters after the
        /// equals character upto the end of line.
        /// </remarks>
        void LoadConfigFile();

        /// <summary>
        /// Save configuration to the config file.
        /// </summary>
        /// <returns>Returns true if an error occurred.</returns>
        bool SaveConfigFile();

        /// <summary>
        /// Reads a text file into an array of strings.
        /// </summary>
        /// <remarks>
        /// Lines starting with a hash character, '#', are ignored.
        /// </remarks>
        /// <param name="fileName">The path and name of file to read.</param>
        /// <returns>Returns a vector of strings, or NULL if unable to read. Each entry is a line of text.<returns>
        static vector<string*>* ReadTextFile(const string& fileName);
#pragma endregion

    private:
        /// <summary>
        /// A system dependent function to find path to executable.
        /// </summary>
        /// <returns>Returns either the path or empty string.</returns>
        static string& GetExecutablePath();
        /// <summary>
        /// A system dependent function to get the current working directory.
        /// </summary>
        /// <returns>Returns the current working directory</returns>
        static string& GetCurrentWorkingDirectory();

        /// <summary>
        /// Removes the last term in the path (a directory or file name).
        /// </summary>
        /// <param name="path">The path to remove the last term from.</param>
        /// <returns>Returns a ref to a new string.</returns>
        static string& GetDirectoryPath(const string& path);

        /// <summary>
        /// Returns the last term in the path (a directory or file name).
        /// </summary>
        /// <param name="path">The path to return the last term from.</param>
        /// <returns>Returns a ref to a new string.</returns>
        static string& GetPathTail(const string& path);

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
