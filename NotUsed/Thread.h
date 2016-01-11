#pragma once

namespace Plato::Standard {
    /// <summary>
    /// Portable thread implementation.
    /// </summary>
    public class Thread {
    public:
        typedef void (*ThreadFunctionPtr)(void *);
        /// <summary>
        /// The exit value of the thread.
        /// </summary>
        int ExitCode;
        /// <summary>
        /// Creates a new thread.
        /// </summary>
        Thread(ThreadFunctionPtr threadFunctionPtr, void* functionArgs);
        /// <summary>
        /// Checks thread is released before deallocating.
        /// </summary>
        ~Thread();
        /// <summary>
        /// Starts the thread.
        /// </summary>
        /// <returns>Returns true if thread started successfully.</returns>
        bool Start();
        /// <summary>
        /// Terminates the thread.
        /// </summary>
        /// <returns>Returns true if thread terminated successfully.</returns>
        bool Abort();
    private:
        /// <summary>
        /// A pointer to the argument for the thread function.
        /// </summary>
        void * mFunctionArgs;
        /// <summary>
        /// The function to be executed by the thread.
        /// </summary>
        ThreadFunctionPtr mThreadFunctionPtr;
        /// <summary>
        /// Set to true when the thread handle is valid.
        /// </summary>
        bool mIsOk;
#ifdef WIN32
        /// <summary>
        /// A win32 handle for the thread object.
        /// </summary>
        HANDLE mThread;
        /// <summary>
        /// Thread id.
        /// </summary>
        DWORD mThreadId;
        /// <summary>
        /// System dependent thread function signature.
        /// </summary>
        /// <param name="pArgs">This is set to a pointer to the Thread object.</returns>
        /// <returns>Always returns zero.</returns>
        static DWORD WINAPI ThreadProc(void * pArgs ); 
#else // POSIX
        /// <summary>
        /// A POSIX thread object.
        /// </summary>
        pthread_t mThread;
        /// <summary>
        /// System dependent thread function signature.
        /// </summary>
        /// <param name="pArgs">This is set to a pointer to the Thread object.</returns>
        /// <returns>Always returns null.</returns>
        static void * ThreadProc(void * pArgs);
#endif
    };
}