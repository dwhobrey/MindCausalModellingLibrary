#include "PlatoStandardIncludes.h"
#include "Thread.h"

namespace Plato::Standard {
#ifdef WIN32
    Thread::Thread(ThreadFunctionPtr threadFunctionPtr, void* functionArgs); {
        mThreadFunctionPtr = threadFunctionPtr;
        mFunctionArgs = functionArgs;
        mThread = (HANDLE)0;
        mIsOk = false;
    }
    Thread::~Thread() {
        if(mIsOk) {
            Abort();
        }
        if(!mIsOK && mThread!=(HANDLE)0) {
            CloseHandle(mThread);
        }
    }
    bool Thread::Start() {
        if(!mIsOk) { 
            if(mThreadFunctionPtr!=null) {
                mThread = CreateThread(nullptr,0,ThreadProc,(void*)this,0,&mThreadId);
                mIsOk = (mThread!=(HANDLE)0);               
            } else {
                mIsOk = false;
            }
        }
        return mIsOk;
    }
    DWORD WINAPI ThreadProc(void * pArgs) {
        if(pArgs!=null) {
            Thread* pThread = (Thread*)pArgs;
            pThread->mThreadFunctionPtr(mFunctionArgs);
        }
        return (DWORD)0;
    }
    bool Abort() {
        if(mIsOk) {
            mIsOk = (TerminateThread(mThread,(DWORD)-1)!=0);
            return !mIsOk;
        }
        return true;
    }
#else // POSIX
    Thread::Thread(ThreadFunctionPtr threadFunctionPtr, void* functionArgs); {
        mThreadFunctionPtr = threadFunctionPtr;
        mFunctionArgs = functionArgs;
        mIsOk = false;
    }
    Thread::~Thread() {
        if(mIsOk) {
            Abort();
        }
    }
    bool Thread::Start() {
        if(!mIsOk) { 
            if(mThreadFunctionPtr!=null) {
                mIsOk = (pthread_create(&mThread, nullptr, ThreadProc, (void *)this)==0); 
            } else {
                mIsOk = false;
            }
        }
        return mIsOk;
    }
    void* ThreadProc( void *pArgs ) {
        if(pArgs!=null) {
            Thread* pThread = (Thread*)pArgs;
            pThread->mThreadFunctionPtr(mFunctionArgs);
        }
        return (void*)NULL;
    }
    bool Abort() {
        if(mIsOk) {
            pthread_cancel(mThread);
            mIsOk = false;
        }
        return true;
    }    
#endif
}
