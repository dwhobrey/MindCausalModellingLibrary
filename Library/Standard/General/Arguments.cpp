#include "PlatoIncludes.h"
#include "Arguments.h"

namespace Plato {

    Arguments::Arguments() {
    }

    Arguments::~Arguments() {
    }

    Arguments* Arguments::Clone() const {
        return new Arguments();
    }
}
