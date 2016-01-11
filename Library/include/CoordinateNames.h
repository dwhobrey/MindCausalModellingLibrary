#pragma once

namespace Plato {

    namespace CoordinateNames {
        /// <summary>
        /// This is an enumeration of coordindate names.
        /// It is used, for example, for selecting the coordinate
        /// upon which to perform or retrieve information.
        /// </summary>
        enum Enum {
            /// <summary>
            /// Refers to the X coordinate.
            /// </summary>
            X = 0,
            /// <summary>
            /// Refers to the Radius coordinate.
            /// </summary>
            Radius = 0,
            /// <summary>
            /// Refers to the first coordinate.
            /// </summary>
            A = 0,
            /// <summary>
            /// Refers to the Y coordinate.
            /// </summary>
            Y = 1,
            /// <summary>
            /// Refers to the Phi coordinate.
            /// </summary>
            Phi = 1,
            /// <summary>
            /// Refers to the second coordinate.
            /// </summary>
            B = 1,
            /// <summary>
            /// Refers to the Z coordinate.
            /// </summary>
            Z = 2,
            /// <summary>
            /// Refers to the Theta coordinate.
            /// </summary>
            Theta = 2,
            /// <summary>
            /// Refers to the third coordinate.
            /// </summary>
            C = 2,
            /// <summary>
            /// Refers to the OverlayID coordinate.
            /// </summary>
            OverLayID = 3
        };
    }
    typedef CoordinateNames::Enum CoordinateNamesEnum;
}
