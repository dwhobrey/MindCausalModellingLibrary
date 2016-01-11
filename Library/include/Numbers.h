#pragma once

namespace Plato {

    /// <summary>
    /// Number Helper Classes.
    /// </summary>
    class Numbers {
    public:
        /// <summary>
        /// Value to indicate maximum number value (approx 3.4e+38).
        /// </summary>
        static const float MaxValue;

        /// <summary>
        /// Value to indicate minimum number value (approx -(MaxValue-1)).
        /// </summary>
        static const float MinValue;

        /// <summary>
        /// Value to indicate value or range is unbounded.
        /// </summary>
        static const float UnboundedValue;

        /// <summary>
        /// Value to indicate value is out of range.
        /// </summary>
        static const float OutOfRangeValue;

        /// <summary>
        /// Value for PI.
        /// </summary>
        static const float PI;

        /// <summary>
        /// Number of radians in a circle, upper value for polar angles.
        /// </summary>
        static const float TwoPI;

        /// <summary>
        /// Checks if value is unbounded.
        /// </summary>
        /// <param name="value">The value to check.</param>
        /// <returns>Returns true if value is unbounded.</returns>
        static bool IsUnbounded(float value);

        /// <summary>
        /// Checks if value is out of range.
        /// </summary>
        /// <param name="value">The value to check.</param>
        /// <returns>Returns true if value is out of range.</returns>
        static bool IsOutOfRange(float value);

        /// <summary>
        /// Checks if value is a special value, 
        /// such as OutOfRange or UnboundedValue.
        /// </summary>
        /// <param name="value">The value to check.</param>
        /// <returns>Returns true if value is special.</returns>
        static bool IsSpecial(float value);

        /// <summary>
        /// Converts a string representation of a bool to internal binary form.
        /// </summary>
        /// <param name="text">The string representation of a bool value to convert.</param>
        /// <returns>Returns the string as a bool, or false if unable to convert.</returns>
        static bool ToBool(const string& text);

        /// <summary>
        /// Converts a string representation of an int to internal binary form.
        /// </summary>
        /// <param name="number">The string number to convert.</param>
        /// <returns>Returns the number as an int, or zero if unable to convert.</returns>
        static int ToInt(const string& number);

        /// <summary>
        /// Converts a string representation of a long to internal binary form.
        /// </summary>
        /// <param name="number">The string number to convert.</param>
        /// <returns>Returns the number as a long, or zero if unable to convert.</returns>
        static long ToLong(const string& number);

        /// <summary>
        /// Converts a string representation of a float to internal binary form.
        /// </summary>
        /// <param name="number">The string number to convert.</param>
        /// <returns>Returns the number as a float, or zero if unable to convert.</returns>
        static float ToFloat(const string& number);

        /// <summary>
        /// Converts a binary number into string form.
        /// </summary>
        /// <remarks>The caller owns the new string.</remarks>
        /// <param name="number">The binary number to convert.</param>
        /// <returns>Returns the string form of number.</returns>
        static string& ToString(int number);

        /// <summary>
        /// Converts a binary number into string form.
        /// </summary>
        /// <remarks>The caller owns the new string.</remarks>
        /// <param name="number">The binary number to convert.</param>
        /// <returns>Returns the string form of number.</returns>
        static string& ToString(long number);

        /// <summary>
        /// Converts a binary number into string form.
        /// </summary>
        /// <remarks>The caller owns the new string.</remarks>
        /// <param name="number">The binary number to convert.</param>
        /// <returns>Returns the string form of number.</returns>
        static string& ToString(float number);

        /// <summary>
        /// Check the bool value is in range,
        /// and initialise if it is not.
        /// </summary>
        /// <param name="v">The variable to check.</param>
        /// <param name="lo">The lowest value of range.</param>
        /// <param name="hi">The highest value of range.</param>
        /// <param name="defaultValue">It's default value.</param>
        /// <param name="reset">When true, forces default value to be assigned to variable.</param>
        /// <returns>Returns revised value.</returns>
        static bool CheckValue(bool& v, bool lo, bool hi, bool defaultValue, bool reset);

        /// <summary>
        /// Check the integer value is in range,
        /// and initialise if it is not.
        /// </summary>
        /// <param name="v">The variable to check.</param>
        /// <param name="lo">The lowest value of range.</param>
        /// <param name="hi">The highest value of range.</param>
        /// <param name="defaultValue">It's default value.</param>
        /// <param name="reset">When true, forces default value to be assigned to variable.</param>
        /// <returns>Returns revised value.</returns>
        static int CheckValue(int& v, int lo, int hi, int defaultValue, bool reset);

        /// <summary>
        /// Check the long is in range,
        /// and initialise if it is not.
        /// </summary>
        /// <param name="v">The variable to check.</param>
        /// <param name="lo">The lowest value of range.</param>
        /// <param name="hi">The highest value of range.</param>
        /// <param name="defaultValue">It's default value.</param>
        /// <param name="reset">When true, forces default value to be assigned to variable.</param>
        /// <returns>Returns revised value.</returns>
        static long CheckValue(long& v, long lo, long hi, long defaultValue, bool reset);
    };
}
