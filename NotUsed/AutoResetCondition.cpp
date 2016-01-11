#include "PlatoStandardIncludes.h"
#include "AutoResetCondition.h"

namespace Plato::Standard {
#ifdef WIN32
    AutoResetCondition::AutoResetCondition() {
        mEventHandle = CreateEvent(nullptr,false,false,nullptr);
        mIsOk = (mEventHandle!=(HANDLE)0);
    }
    AutoResetCondition::~AutoResetCondition() {
        if(mIsOk) {
            CloseHandle(mEventHandle);
        }
    }
    bool AutoResetCondition::WaitOne(int millisecondsTimeout) {
        if(mIsOk) {
            if(millisecondsTimeout<0) {
                millisecondsTimeout = INFINITE;
            }
            DWORD dwResult = WaitForSingleObject(mEventHandle,millisecondsTimeout);
            switch (dwResult) {
                // The condition variable was signaled.
                case WAIT_OBJECT_0: return true;
                // The time-out interval elapsed and the variable was not signalled.
                case WAIT_TIMEOUT: break; 
                // Got ownership of an abandoned mutex object?!
                case WAIT_ABANDONED: break;
                default: ;
            }
        }
        return false;    
    }
    bool AutoResetCondition::Set() {
        if(mIsOk) {
            return SetEvent(mEventHandle);
        }
        return false;
    }
    bool AutoResetCondition::SignalAndWait(AutoResetCondition& signalCondition,AutoResetCondition& waitCondition) {
        if(signalCondition.mIsOk && waitCondition.mIsOk) {
            DWORD dwResult = SignalObjectAndWait(signalCondition.mEventHandle, waitCondition.mEventHandle, INFINITE, false);
            switch (dwResult) {
                // The condition variable was signaled.
                case WAIT_OBJECT_0: return true;
                // The time-out interval elapsed and the variable was not signalled.
                case WAIT_TIMEOUT: break; 
                // Got ownership of an abandoned mutex object?!
                case WAIT_ABANDONED: break;
                default: ;
            }
        }
        return false;
    }
#else // POSIX
    AutoResetCondition::AutoResetCondition(bool manualReset, bool initialState) {
        mIsOk = (pthread_cond_init(&mCondition, nullptr)==0);
        if(mIsOk) {
            mIsOk = (pthread_mutex_init(&mMutex, NULL)==0);
            if(!mIsOk) {
                pthread_cond_destroy(&mCondition);
            }
        }
    }
    AutoResetCondition::~AutoResetCondition() {
        if(mIsOk) {
            int result = pthread_cond_destroy(&mCondition);
            if(result!=0) {
                // Some threads are still waiting on condition, so not destroyed.
            }
            pthread_mutex_destroy(&mMutex);
        }
    }
    bool AutoResetCondition::WaitOne(int millisecondsTimeout) {
        if(mIsOk) {
            pthread_mutex_lock(&mMutex);
            int result = 0;
            if(millisecondsTimeout<0||millisecondsTimeout==INFINITE) {
                pthread_cond_wait(&mCondition, &mMutex);
            } else {
                struct timeval now;
                struct timespec timeout;
                gettimeofday(&now);
                timeout.tv_sec = now.tv_sec + millisecondsTimeout/1000;
                timeout.tv_nsec = now.tv_usec * 1000 + (millisecondsTimeout % 1000) * 1000000;
                result = pthread_cond_timedwait(&mCondition, &mMutex, &timeout);
            }
            pthread_mutex_unlock(&mMutex);
            // Check timeout did not occurr.
            if (result != ETIMEDOUT) {
                return true;    
            }
        }
        return false;
    }
    bool AutoResetCondition::Set() {
        if(mIsOk) {
            pthread_mutex_lock(&mMutex);
            pthread_cond_signal(&mCondition);
            pthread_mutex_unlock(&mMutex);
        }
    }
    bool AutoResetCondition::SignalAndWait(AutoResetCondition& signalCondition,AutoResetCondition& waitCondition) {
        if(signalCondition.mIsOk && waitCondition.mIsOk) {
            // This will probably lead to deadlock...
            pthread_mutex_lock(&(signalCondition.mMutex));
            pthread_mutex_lock(&(waitCondition.mMutex));
            pthread_cond_signal(&(signalCondition.mCondition));
            pthread_mutex_unlock(&(signalCondition.mMutex));
            pthread_cond_wait(&(waitCondition.mCondition), &(waitCondition.mMutex));
            pthread_mutex_unlock(&(waitCondition.mMutex));
            return true;
        }
        return false;
    }
#endif
}