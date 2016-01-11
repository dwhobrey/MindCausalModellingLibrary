#pragma once

namespace Plato::Standard {

    /// <summary>
    /// Simple object to use for locking on with 'lock' statement.
    /// </summary>
    public class LockObject {

    private:
        /// <summary>
        /// The user defined state of the lock.
        /// </summary>
        bool mIsSet;
        /// <summary>
        /// Mutex used to guard setting state.
        /// </summary>
        Mutex mMutex;

        /// <summary>
        /// Creates a new lock object with the given state.
        /// </summary>
        /// <param name="isSet">Sets the state of the lock.</param>
    public:
        LockObject(bool isSet=false);

        /// <summary>
        /// Deallocate lock object.
        /// </summary>
        ~LockObject();

        /// <summary>
        /// Locks the object. Waits if necessary.
        /// </summary>
        void Lock();
        /// <summary>
        /// Unlocks the object.
        /// </summary>
        void Release();

        /// <summary>
        /// Accessor for the locks state.
        /// </summary>
        bool IsSet();

        /// <summary>
        /// Sets the locks state.
        /// </summary>
        void Set(bool value=true);

        /// <summary>
        /// Sets the locks state to false.
        /// </summary>
        void Reset();
    };
}
