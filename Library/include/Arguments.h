#pragma once

namespace Plato {
    /// <summary>
    /// Base class for holding arguments.
    /// </summary>
    /// <remarks>
    /// Derivatives of this class are used to hold the arguments to an associated object's
    /// constructor. This is used to create new instances of the associated object at a
    /// later time to when the arguments were created.
    /// </remarks>
    class Arguments {
    public:
        /// <summary>
        /// Default constructor.
        /// </summary>
        Arguments();
        /// <summary>
        /// Default destructor.
        /// </summary>
        virtual ~Arguments();
        /// <summary>
        /// Copy arguments.
        /// </summary>
        virtual Arguments* Clone() const;
    };
}
