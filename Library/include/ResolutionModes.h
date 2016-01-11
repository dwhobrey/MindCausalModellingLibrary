#pragma once

namespace Plato {
    namespace ResolutionModes {
        /// <summary>
        /// The elements an operation applies to depends on when,
        /// during the configuration and emulation process, it is applied.
        /// An operator can specify when it is to be applied by setting its resolution mode.
        /// </summary>
        /// <remarks>
        /// While configuration will only ever be in one resolution mode at a time,
        /// this enumeration is a set of flags in order to allow operators to specify a
        /// combination of modes.
        /// </remarks>
        enum Enum {
            /// <summary>
            /// No resolution required.
            /// </summary>
            None = 0,
            /// <summary>
            /// Apply the operator when first encountered in the configuration process.
            /// See Model.Update for the stages and phases of configuration.
            /// </summary>
            Now = 1,
            /// <summary>
            /// Apply the operator at the end of the configuration process.
            /// See Model.Update for the stages and phases of configuration.
            /// </summary>
            Final = 2,
            /// <summary>
            /// Apply the operator whenever an element birth or death occurs.
            /// </summary>
            Dynamic = 4,
            /// <summary>
            /// A combination of all the modes.
            /// </summary>
            All = Now | Final | Dynamic
        };
    }

    typedef ResolutionModes::Enum ResolutionModesEnum;
}
