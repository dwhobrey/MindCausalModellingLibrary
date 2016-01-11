#pragma once

namespace Plato::Standard {
    /// <summary>
    /// Portable mutex implementation.
    /// </summary>
    public class Mutex {
    public:
        /// <summary>
        /// Creates a new mutex.
        /// </summary>
        Mutex();
        /// <summary>
        /// Checks mutex is released before deallocating.
        /// </summary>
        ~Mutex();
        /// <summary>
        /// Claims the mutex, waiting as necessary.
        /// </summary>
        /// <param name="millisecondsTimeout">The time to wait in milliseconds before returning.
        /// A value of -1 means wait indefinitely.</param>
        /// <returns>Returns true if claimed successfully.</returns>
        bool Lock(int millisecondsTimeout=(-1));
        /// <summary>
        /// Release the mutex.
        /// </summary>
        void Release(); 
    private:
        /// <summary>
        /// Set to true when the mutex handle is valid.
        /// </summary>
        bool mIsOk;
#ifdef WIN32
        /// <summary>
        /// A win32 handle for the mutex object.
        /// </summary>
        HANDLE mMutex;
#else // POSIX
        /// <summary>
        /// A POSIX mutex object.
        /// </summary>
        pthread_mutex_t mMutex;
#endif
    };
}

