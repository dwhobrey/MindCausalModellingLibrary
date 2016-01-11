#pragma once

namespace Plato {
    /// <summary>
    /// Scale class for holding scaling factors.
    /// This class represents the degree to which an element should be scaled when overlaid in the parent space.
    /// </summary>
    class Scale {
    public:
        /// <summary>
        /// A Scale object that signifies unit scaling, which is effectively no scaling!
        /// </summary>
        static const Scale* Unity;

        /// <summary>
        /// An array for holding the scale factor for each dimension of an element.
        /// </summary>
        float ScalingFactors[3];

        /// <summary>
        /// Construct a new Scale object.
        /// The Parameters must all be greater than zero.
        /// </summary>
        /// <param name="s0">Factor to scale first coordinate by.</param>
        /// <param name="s1">Factor to scale second coordinate by.</param>
        /// <param name="s2">Factor to scale third coordinate by.</param>
        Scale(float s0, float s1, float s2);

        /// <summary>
        /// Copy constructor.
        /// </summary>
        Scale(const Scale& scale);

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
