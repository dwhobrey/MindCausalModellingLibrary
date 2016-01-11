#include "PlatoIncludes.h"
#include "Archiver.h"

namespace Plato {

    void* Archiver::Deserialize(const string& archivePath) {
        void *obj = NULL;
        if(!archivePath.empty()) {
            // Open file and deserialize the object from it.
            // DEBUG: TO DO.
        }
        return obj;
    }

    int Archiver::Serialize(const string& archivePath, bool inBinary, void* obj) {
        int result = 0;
        if (!archivePath.empty() && obj != NULL) {
            // Open file and serialize the object into it.
            // DEBUG: TO DO.
        }
        return result;
    }
}
