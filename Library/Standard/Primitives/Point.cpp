#include "PlatoIncludes.h"
#include "Numbers.h"
#include "Point.h"

namespace Plato {

    const string* Point::OutOfRangePatternString = NULL;
    const string* Point::UnboundedPatternString = NULL;

    void Point::Initializer() {
        OutOfRangePatternString = new string(".+");
        UnboundedPatternString = new string(".*");
    }

    void Point::Finalizer() {
        delete OutOfRangePatternString;
        delete UnboundedPatternString;
    }

    Point::Point(float x, float y, float z, float overlayID) {
        Coordinates[0] = x;
        Coordinates[1] = y;
        Coordinates[2] = z;
        Coordinates[3] = overlayID;
        HashCode = *((size_t*)&x) ^ *((size_t*)&y) ^ *((size_t*)&z) ^ *((size_t*)&overlayID);
        Status = PointStatus::Normal;
    }

    Point::Point(PointStatusEnum status, float x, float y, float z, float overlayID) {
        Status = status;
        Coordinates[0] = x;
        Coordinates[1] = y;
        Coordinates[2] = z;
        Coordinates[3] = overlayID;
        HashCode = *((size_t*)&x) ^ *((size_t*)&y) ^ *((size_t*)&z) ^ *((size_t*)&overlayID);
    }

    Point::Point(const string& position) {
        int coordinateIndex = 0;
        int len = (int)position.size();
        HashCode = 0;
        string::size_type findIndex = position.find(PositionSeparator);
        if(findIndex!=string::npos) {
            int sepIndex = (int)findIndex;
            while (sepIndex >= 0) {
                int nextIndex, sepEnd;
                string::size_type findIndex = position.find(PositionSeparator, sepIndex + 1);
                if(findIndex==string::npos) {
                    nextIndex = -1;
                    sepEnd = len;
                } else {
                    nextIndex = (int)findIndex;
                    sepEnd = nextIndex;
                }
                int partIndex = sepIndex+1;
                sepIndex = nextIndex;
                float val;
                if(partIndex<len) {
                    int partLen = sepEnd - partIndex;
                    string part = position.substr(partIndex, partLen);
                    if(partLen==2) {
                        if (part.compare(*OutOfRangePatternString)==0) {
                            Status = PointStatus::OutOfRange;
                            HashCode = 0;
                            coordinateIndex = 0;
                            break;
                        } else if (part.compare(*UnboundedPatternString)==0) {
                            Status = PointStatus::Unbounded;
                            HashCode = 0;
                            coordinateIndex = 0;
                            break;
                        }
                    }
                    val = Numbers::ToFloat(part);
                    HashCode ^= *((size_t*)&val);
                } else {
                    val = 0.0f;
                }
                Coordinates[coordinateIndex++] = val;
            }
        }
        while (coordinateIndex < NumberOfCoordinates) { 
            Coordinates[coordinateIndex++] = 0.0f; 
        }
    }

    Point::Point(const Point& point) {
        int n = NumberOfCoordinates;
        while(n-- > 0) {
            Coordinates[n] = point.Coordinates[n];
        }
        Status = point.Status;
        HashCode = point.HashCode;
    }

    void Point::UpdateHashCode() {
        HashCode = *((size_t*)Coordinates+1) ^ *((size_t*)Coordinates+2) 
            ^ *((size_t*)Coordinates+3) ^ *((size_t*)Coordinates+4);
    }

    string& Point::StatusReport() const {
        string& result = *new string("");
        switch(Status) {
            case PointStatus::Normal: {
                    int n = NumberOfCoordinates;
                    while (--n >= 0 && Coordinates[n] == 0.0f) ;
                    for(int k=0;k<=n;++k) {
                        string& tmp = Numbers::ToString(Coordinates[k]);
                        result += PositionSeparator + tmp;
                        delete &tmp;
                    }
                }
                break;
            case PointStatus::OutOfRange:
                result += PositionSeparator + *OutOfRangePatternString;
                break;
            case PointStatus::Unbounded:
                result += PositionSeparator + *UnboundedPatternString;
                break;
        }
        return result;
    }

    bool Point::Equals(const Point& point) const {
        int n = NumberOfCoordinates;
        while (n-- > 0) {
            if (Coordinates[n] != point.Coordinates[n])
                return false;
        }
        return true;
    }

    bool Point::SpatialCoordinatesEquals(const Point& tc) const {
        int n = NumberOfSpatialCoordinates;
        while (n-- > 0) {
            if (Coordinates[n] != tc.Coordinates[n])
            return false;
        }
        return true;
    }

    bool Point::OverlayEquals(const Point& tc) const {
        int k = OverlayField;
        return (Coordinates[k] == tc.Coordinates[k]);
    }

    bool Point::CoordinatesEquals(const Point& tc) const {
        int n = NumberOfCoordinates;
        while (n-- > 0) {
            if (Coordinates[n] != tc.Coordinates[n])
            return false;
        }
        return true;
    }

    int Point::Compare(const Point* a, const Point* b) {
        if (a != NULL) {
            if (b == NULL) {
                return 1;
            }
            return a->CompareTo(*b);
        }
        else if (b != NULL) {
            return -1;
        }
        return 0;
    }

    int Point::CompareTo(const Point& point) const {
        // DEBUG: TO DO.
        return 0;
    }

    PathNode* Point::Node(string& name, float coordinates) {
        //DEBUG: TO DO: Point.Node: decide how local modelling features might be added.
        return NULL;
    }
}
