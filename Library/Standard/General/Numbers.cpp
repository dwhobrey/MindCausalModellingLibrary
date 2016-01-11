#include "PlatoIncludes.h"
#include "Numbers.h"

namespace Plato {

    const float Numbers::MaxValue = FLT_MAX-1.0f;
    const float Numbers::MinValue = -(FLT_MAX-2.0f);
    const float Numbers::UnboundedValue = FLT_MIN;
    const float Numbers::OutOfRangeValue = FLT_MAX;
    const float Numbers::PI = (float)acos(-1.0f);
    const float Numbers::TwoPI = (float)(2.0 * PI);

    bool Numbers::IsUnbounded(float value) {
        return value==UnboundedValue;
    }

    bool Numbers::IsOutOfRange(float value) {
        return value==OutOfRangeValue;
    }

    bool Numbers::IsSpecial(float value) {
        return value==UnboundedValue || value==OutOfRangeValue;
    }

    bool Numbers::ToBool(const string& text) {
        try {
            return text.compare("true")==0;
        } catch(...) {
        }
        return false;
    }

    int Numbers::ToInt(const string& number) {
        return (int)(strtol(number.c_str(),NULL,0));
    }

    long Numbers::ToLong(const string& number) {
        return strtol(number.c_str(),NULL,0);
    }

    float Numbers::ToFloat(const string& number) {
        return (float)strtod(number.c_str(),NULL);
    }

    string& Numbers::ToString(int number) {
        char buffer[65];
        sprintf(buffer,"%d",number);
        return *new string(buffer);
    }

    string& Numbers::ToString(long number) {
        char buffer[65];
        sprintf(buffer,"%ld",number);
        return *new string(buffer);
    }

    string& Numbers::ToString(float number) {
        char buffer[65];
        sprintf(buffer, "%g", (double)number);
        return *new string(buffer);
    }


    bool Numbers::CheckValue(bool& v, bool lo, bool hi, bool defaultValue, bool reset) {
        if (v != lo && v != hi || reset)
            v = defaultValue;
        return v;
    }

    int Numbers::CheckValue(int& v, int lo, int hi, int defaultValue, bool reset) {
        if(v<lo||v>hi||reset)
            v = defaultValue;
        return v;
    }

    long Numbers::CheckValue(long& v, long lo, long hi, long defaultValue, bool reset) {
        if (v < lo || v > hi || reset)
            v = defaultValue;
        return v;
    }
}
