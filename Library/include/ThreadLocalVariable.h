#pragma once

namespace Plato {
    /// <summary>
    /// Portable thread local storage implementation.
    /// </summary>
    class ThreadLocalVariable {
    public:
#ifdef WIN32
        typedef DWORD Key;
        typedef LPVOID ValuePtr;
        inline static bool AllocateKey(Key* pKey) { return ((*pKey = TlsAlloc()) != TLS_OUT_OF_INDEXES); }
        inline static Key GetKey() { return TlsAlloc(); } 
        inline static ValuePtr GetVariableValue(Key key) { return TlsGetValue(key); }
        inline static void SetVariableValue(Key key,const ValuePtr pValue) { TlsSetValue(key,pValue); }
#else // POSIX
        typedef pthread_key_t Key;
        typedef void* ValuePtr;
        inline static Key GetKey() { Key key; pthread_key_create(&key,NULL); return key; }
        inline static bool AllocateKey(Key* pKey) { return (pthread_key_create(pKey, NULL)==0); }
        inline static ValuePtr GetVariableValue(Key key) { return pthread_getspecific(key); }
        inline static void SetVariableValue(Key key,const ValuePtr pValue) { pthread_setspecific(key, pValue);  }       
#endif
    };
}
