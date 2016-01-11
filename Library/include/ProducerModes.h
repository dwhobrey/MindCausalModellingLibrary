#pragma once

namespace Plato {
    namespace ProducerModes {
        /// <summary>
        /// This enumerates the context from which a producer might be called.
        /// </summary>
        enum Enum {
            /// <summary>
            /// A property request was made during a normal (non-connection) path traversal.
            /// </summary>
            Traversing = 1,
            /// <summary>
            /// A property request was made during connection path traversal.
            /// </summary>
            Connecting = 2,
            /// <summary>
            /// Represents any producer mode.
            /// </summary>
            Any = 3
        };
    }
    typedef ProducerModes::Enum ProducerModesEnum;
}
