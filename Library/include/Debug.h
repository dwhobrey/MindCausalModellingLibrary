#pragma once
namespace Plato {
    /// <summary>
    /// Debugging support.
    /// </summary>
    class Debug {
    public:
        /// <summary>
        /// Send output to console depending on debug level.
        /// A message will only be displayed if DebugLevel >= level.
        /// </summary>
        /// <remarks>The new line string "\n" is appended to the output.</remarks>
        /// <param name="level">The debug importance of this message,
        /// 0 = high importance, 10 = low importance.</param>
        /// <param name="format">The format string, i.e. message body.</param>
        /// <param name="...">The debug message arguments.</param>
        static void WriteLine(int level, const char* format, ...);

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
