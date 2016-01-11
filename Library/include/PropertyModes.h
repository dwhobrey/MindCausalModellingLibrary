#pragma once

namespace Plato {

    namespace PropertyModes {
        /// <summary>
        /// When accessing the property container hierarchy, the caller must
        /// specify the kind of access they require.
        /// </summary>
        enum Enum {
            /// <summary>
            /// Caller is exploring the property hierarchy and requires read access only.
            /// </summary>
            None = 0,
            /// <summary>
            /// Set: This is an Input or Output access.
            /// Input or output flag is significant.
            /// </summary>
            IsIO = 1,
            /// <summary>
            /// <para>Unset: This is an output path.</para>
            /// <para>Set: This is an input path.</para>
            /// </summary>
            Input = 2,
            /// <summary>
            /// <para>
            /// Unset: Caller requires read access only.
            /// </para>
            /// <para>
            /// Set: Caller is creating properties and requires read and write access.
            /// </para>
            /// </summary>
            Writing = 4,
            /// <summary>
            /// <para>Unset: The use of Directors is disabled.</para>
            /// <para>Set: The use of Directors is enabled.</para>
            /// </summary>
            Directing = 8,
            /// <summary>
            /// <para>Unset: The use of Producers is disabled.</para>
            /// <para>Set: The use of Producers is enabled.</para>
            /// </summary>
            Producing = 16,
            /// <summary>
            /// Currently equivalent to the Directing flag.
            /// Input or Output flag is not significant.
            /// </summary>
            Traversing = Directing,
            /// <summary>
            /// The combination of Writing, Directing and Producing.
            /// Input or Output flag is not significant.
            /// </summary>
            All = Writing | Directing | Producing,
            /// <summary>
            /// The combination of IsIO, Writing, Directing and Producing.
            /// </summary>
            AllOutput = IsIO | Writing | Directing | Producing,
            /// <summary>
            /// The combination of Input, IsIO, Writing, Directing and Producing.
            /// </summary>
            AllInput = Input | IsIO | Writing | Directing | Producing
        };
    }
    typedef PropertyModes::Enum PropertyModesEnum;
}
