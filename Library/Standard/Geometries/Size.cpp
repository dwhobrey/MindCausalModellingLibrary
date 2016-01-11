#include "PlatoIncludes.h"
#include "Size.h"

namespace Plato {
        
    const Size* Size::Unity = NULL;

    void Size::Initializer() {
        Unity = new Size(1.0f, 1.0f, 1.0f, 1.0f);
    }

    void Size::Finalizer() {
        delete Unity;
    }

    Size::Size(float s0, float s1, float s2, float numberOfIDs) {
        Sizes[0] = s0;
        Sizes[1] = s1;
        Sizes[2] = s2;
        Sizes[3] = numberOfIDs;
    }

    Size::Size(const Size& size) {
        Sizes[0] = size.Sizes[0];
        Sizes[1] = size.Sizes[1];
        Sizes[2] = size.Sizes[2];
        Sizes[3] = size.Sizes[3];
    }

}
