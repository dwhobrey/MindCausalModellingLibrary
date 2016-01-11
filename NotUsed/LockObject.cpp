#include "PlatoStandardIncludes.h"
#include "LockObject.h"

namespace Plato:Standard {

    LockObject::LockObject(bool isSet) {
        mIsSet = isSet;
    }

    LockObject::~LockObject() {
    }

    bool LockObject::IsSet() {
        return mIsSet;
    }

    void LockObject::Set(bool value) {
        mIsSet = value;
    }

    void LockObject::Reset() {
        mIsSet = false;
    }
    void LockObject::Lock() {
        mMutex.Lock();
    }
    void LockObject::Release() {
        mMutex.Release();
    }
}