#pragma once

namespace Plato {

    namespace ConfigurePhases {
        /// <summary>
        /// Indicates the current configuration phase or configuration state of an element.
        /// </summary>
        enum Enum {
            /// <summary>
            /// Configuration has not started.
            /// </summary>
            Unconfigured,
            /// <summary>
            /// Currently configuring distributors.
            /// </summary>
            DistributorsNow,
            /// <summary>
            /// Currently configuring connectors.
            /// </summary>
            ConnectorsNow,
            /// <summary>
            /// Currently configuring converters.
            /// </summary>
            ConvertersNow,
            /// <summary>
            /// Currently configuring connectors.
            /// </summary>
            ConnectorsFinal,
            /// <summary>
            /// Currently configuring converters.
            /// </summary>
            ConvertersFinal,
            /// <summary>
            /// Indicates the element is configured.
            /// </summary>
            Configured
        };
    }
    typedef ConfigurePhases::Enum ConfigurePhasesEnum;
}
