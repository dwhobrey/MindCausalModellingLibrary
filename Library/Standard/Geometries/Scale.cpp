#include "PlatoIncludes.h"
#include "Scale.h"

namespace Plato {

    const Scale* Scale::Unity = NULL;

    void Scale::Initializer() {
        Unity = new Scale(1.0f, 1.0f, 1.0f);
    }

    void Scale::Finalizer() {
        delete Unity;
    }

    Scale::Scale(float s0, float s1, float s2) {
        ScalingFactors[0] = s0;
        ScalingFactors[1] = s1;
        ScalingFactors[2] = s2;
    }

    Scale::Scale(const Scale& scale) {
        ScalingFactors[0] = scale.ScalingFactors[0];
        ScalingFactors[1] = scale.ScalingFactors[1];
        ScalingFactors[2] = scale.ScalingFactors[2];
    }
}
