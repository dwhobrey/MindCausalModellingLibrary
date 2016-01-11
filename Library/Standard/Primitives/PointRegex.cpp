#include "PlatoIncludes.h"
#include "Numbers.h"
#include "Point.h"
#include "PointSpace.h"
#include "PointRegex.h"

namespace Plato {

    PointRegex::PointRegex(PointSpace& pointSpace) : mPointSpace(pointSpace) {
        Status = PointRegexStatus::Simple;
    }

    string& PointRegex::StatusReport() const {
        string& result = *new string("");
        string& tmp = Numbers::ToString((int)Status);
        result += "[" + tmp + "]";
        delete &tmp;
        return result;
    }

    bool PointRegex::PatternMatch(const PointRegex& regexPoint) const {
        // DEBUG: TO DO: Compare two regex points.
        return false;
    }
}
