#pragma once
namespace Plato {
    /// <summary>
    /// Tracing support.
    /// </summary>
    class Trace {
    public:
        /// <summary>
        /// Send output to console depending on trace level.
        /// A message will only be displayed if TraceLevel >= level.
        /// </summary>
        /// <remarks>The new line string "\n" is appended to the output.</remarks>
        /// <param name="level">The trace importance of this message,
        /// 0 = high importance, 10 = low importance.</param>
        /// <param name="format">The format string, i.e. message body.</param>
        /// <param name="...">The trace message arguments.</param>
        static void WriteLine(int level, const char* format, ...);
    };
}
