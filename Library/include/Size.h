#pragma once

namespace Plato {
    /// <summary>
    /// Size class for holding size values.
    /// This class represents the size (width, height, depth, arc) of an element.
    /// The size is measured in absolute units.
    /// DEBUG: TO DO: is a relative (to parent) percentage unit needed?
    /// </summary>
    class Size {
    public:
        /// <summary>
        /// A Size object representing Unity.
        /// </summary>
        static const Size* Unity;

        /// <summary>
        /// An array for holding the size of each dimension of an element.
        /// </summary>
        float Sizes[4];

        /// <summary>
        /// Generic constructor for creating new Size objects.
        /// </summary>
        /// <param name="s0">Size of the first dimension.</param>
        /// <param name="s1">Size of the second dimension.</param>
        /// <param name="s2">Size of the third dimension.</param>
        /// <param name="numberOfIDs">The number of IDs permitted per point.</param>
        Size(float s0, float s1, float s2, float numberOfIDs);

        /// <summary>
        /// Copy constructor.
        /// </summary>
        Size(const Size& size);
 
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
