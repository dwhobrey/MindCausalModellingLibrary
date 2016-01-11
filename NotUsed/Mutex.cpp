#include "PlatoStandardIncludes.h"
#include "Mutex.h"

namespace Plato::Standard {
#ifdef WIN32
    Mutex::Mutex() {
        mMutex = CreateMutex(nullptr,FALSE,nullptr);
        mIsOk = (mMutex!=(HANDLE)0);
    }
    Mutex::~Mutex() {
        if(mIsOK) {
            ReleaseMutex(mMutex);
        }
    }
    bool Mutex::Lock(int millisecondsTimeout) {
        if(mIsOK) {
            if(millisecondsTimeout<0) {
                millisecondsTimeout=INFINITE;
            }
            DWORD dwWaitResult = WaitForSingleObject(mMutex,millisecondsTimeout);
            switch (dwWaitResult) {
                // The thread got mutex ownership.
                case WAIT_OBJECT_0: return true;
                // Cannot get mutex ownership due to time-out.
                case WAIT_TIMEOUT: break; 
                // Got ownership of the abandoned mutex object.
                case WAIT_ABANDONED: break;
                default: ;
            }
        }
        return false;
    }
    void Mutex::Release() {
        if(mIsOK) {
            ReleaseMutex(mMutex);
        }
    }
#else // POSIX
    Mutex::Mutex() {
        mIsOk = (pthread_mutex_init(&mMutex, NULL)==0);
    }
    Mutex::~Mutex() {
        if(mIsOk) {
            pthread_mutex_destroy(&mMutex);
        }
    }
    bool Mutex::Lock(int millisecondsTimeout) {
        if(mIsOk) {
            if(millisecondsTimeout<0||millisecondsTimeout==INFINITE) {
                pthread_mutex_lock(&mMutex);
                return true;
            } else {
                struct timeval now;
                struct timespec timeout;
                gettimeofday(&now);
                timeout.tv_sec = now.tv_sec + millisecondsTimeout/1000;
                timeout.tv_nsec = now.tv_usec * 1000 + (millisecondsTimeout % 1000) * 1000000;
                int result = pthread_mutex_timedlock(&mMutex,&timeout);
                // Check timeout did not occurr.
                if (result != ETIMEDOUT) {
                    return true;    
                }
            }
        }
        return false;
    }
    void Mutex::Release() {
        if(mIsOk) {
            pthread_mutex_unlock(&mMutex);
        }
    }
#endif
}
