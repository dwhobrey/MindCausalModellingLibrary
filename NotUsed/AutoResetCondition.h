#pragma once

namespace Plato::Standard {
    /// <summary>
    /// Portable Condition Variable implementation.
    /// </summary>
    public class AutoResetCondition {
    public:
        /// <summary>
        /// Creates a new condition variable.
        /// </summary>
        AutoResetCondition();
        /// <summary>
        /// Checks variable is released before deallocating.
        /// </summary>
        ~AutoResetCondition();
        /// <summary>
        /// Waits for the variable to be signaled.
        /// </summary>
        /// <param name="millisecondsTimeout">The time to wait in milliseconds before returning.
        /// A value of -1 means wait indefinitely.</param>
        /// <returns>Returns true if a signal was received.</returns>
        bool WaitOne(int millisecondsTimeout=(-1));
        /// <summary>
        /// Sets the state of the event to nonsignaled, causing threads to block.
        /// </summary>
        /// <returns>Returns true if the operation succeeded.</returns>
        bool Reset(); 
        /// <summary>
        /// Sets the state of the event to signaled, allowing one or more waiting threads to proceed. 
        /// </summary>
        /// <returns>Returns true if the operation succeeded.</returns>
        bool Set();
        /// <summary>
        /// Atomically signals and waits.
        /// </summary>
        /// <remarks>
        /// Calls Set() on the signal condition, then calls WaitOne() on the wait condition.
        /// </remarks>
        /// <returns>Returns true if both the signal and the wait complete successfully;
        /// if the wait does not complete, the method does not return.</returns>
        static bool SignalAndWait(AutoResetCondition& signalCondition,AutoResetCondition& waitCondition);

    private:
        /// <summary>
        /// Set to true when the event handle is valid.
        /// </summary>
        bool mIsOk;
#ifdef WIN32
        /// <summary>
        /// A win32 handle for the event object.
        /// </summary>
        HANDLE mEventHandle;
#else // POSIX
        /// <summary>
        /// A POSIX condition object.
        /// </summary>
        pthread_cond_t mCondition;
        /// <summary>
        /// The mutex object associated with the condition variable.
        /// </summary>
        pthread_mutex_t mMutex;
#endif
    };
}