#include "PlatoIncludes.h"
#include "Strings.h"

namespace Plato {

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

    const string* Strings::EmptyString = NULL;

    void Strings::Initializer() {
        EmptyString = new string("");
    }

    void Strings::Finalizer() {
        delete EmptyString;
    }

    // This was developed by Paul Hsieh.
    uint32_t Strings::SuperFastHash(const char * data, int len) {
        uint32_t hash = len, tmp;
        int rem;

        if (len <= 0 || data == NULL) return 0;

        rem = len & 3;
        len >>= 2;

        /* Main loop */
        for (;len > 0; len--) {
            hash  += get16bits (data);
            tmp    = (get16bits (data+2) << 11) ^ hash;
            hash   = (hash << 16) ^ tmp;
            data  += 2*sizeof (uint16_t);
            hash  += hash >> 11;
        }

        /* Handle end cases */
        switch (rem) {
            case 3: hash += get16bits (data);
                    hash ^= hash << 16;
                    hash ^= data[sizeof (uint16_t)] << 18;
                    hash += hash >> 11;
                    break;
            case 2: hash += get16bits (data);
                    hash ^= hash << 11;
                    hash += hash >> 17;
                    break;
            case 1: hash += *data;
                    hash ^= hash << 10;
                    hash += hash >> 1;
        }

        /* Force "avalanching" of final 127 bits */
        hash ^= hash << 3;
        hash += hash >> 5;
        hash ^= hash << 4;
        hash += hash >> 17;
        hash ^= hash << 25;
        hash += hash >> 6;

        return hash;
    }

    string& Strings::Trim(string& s, const char* trimCharacters, int trimLen) {
        const char* r = s.data();
        string::size_type pos = s.size();
        // Trim from end first.
        string::size_type n = 0;
        while(pos > 0) {
            char c = r[--pos];
            const char* p = trimCharacters;
            int k = trimLen;
            while(--k >= 0) { 
                if(*p++ == c) {
                    ++n; 
                    break;
                } 
            } 
            if(k < 0) {
                break;
            }
        }
        if(n != 0) {
            s.erase(pos,n);
        }
        // Now trim from start.
        n = 0;
        while(n < pos) {
            char c = r[n];
            const char* p = trimCharacters;
            int k = trimLen;
            while(--k >= 0) { 
                if(*p++ == c) {
                    ++n; 
                    break;
                } 
            } 
            if(k < 0) {
                break;
            }
        }
        if(n != 0) {
            s.erase(0,n);
        }
        return s;
    }

    string& Strings::TrimEnd(string& s, const char* trimCharacters, int trimLen) {
        const char* r = s.data();
        string::size_type pos = s.size();
    
        string::size_type n = 0;
        while(pos > 0) {
            char c = r[--pos];
            const char* p = trimCharacters;
            int k = trimLen;
            while(--k >= 0) { 
                if(*p++ == c) {
                    ++n; 
                    break;
                } 
            } 
            if(k < 0) {
                break;
            }
        }
        if(n != 0) {
            s.erase(pos,n);
        }
        return s;
    }

    string& Strings::Replace(const string& s, const string& oldText, const string& newText) {
        string::size_type oldLen = oldText.size();
        string::size_type index = 0, lastIndex = 0;
        string* r = new string("");
        string::size_type findIndex;
        while((findIndex=s.find(oldText,lastIndex))!=string::npos) {
            index = findIndex;
            string* sub = new string(s.data()+lastIndex,index-lastIndex);
            *r += *sub;
            *r += newText;
            delete sub;
            lastIndex = index + oldLen;
        }
        if(lastIndex<s.size()) {
            *r += s.substr(lastIndex);
        }
        return *r;
    }

    bool Strings::EndsWith(const string& s, const char* tail) {
        size_t len = strlen(tail);
        string::size_type index = s.size()-len;
        return index>=0 && s.compare(index,len,tail)==0;
    }

    char* Strings::Sprintf(const char* format,va_list argPtr) {
        va_list tmpPtr = argPtr;
#ifdef WIN32
        int len = _vscprintf(format,argPtr)+1;
        char* charBuffer = new char[len];
        vsprintf_s(charBuffer,sizeof(char)*len,format,tmpPtr);
#else
        char* charBuffer = new char[1024];
        vsprintf(charBuffer,format,tmpPtr);
#endif
        return charBuffer;
    }

    string& Strings::Format(const char* format, ...) {
        va_list argPtr;
        va_start(argPtr, format);
        char* charPtr = Sprintf(format,argPtr);
        string& str = *new string(charPtr);
        delete charPtr;
        return str;
    }

    void Strings::CheckValue(const string*& s, const string& defaultValue, bool reset) {
        if (s==NULL || s->empty() || reset) {
            if(s!=NULL) {
                delete s;
            }
            s = new string(defaultValue);
        }
    }

    void Strings::CheckValue(const string*& s, const char* defaultValue, bool reset) {
        if (s==NULL || s->empty() || reset) {
            if(s!=NULL) {
                delete s;
            }
            s = new string(defaultValue);
        }
    }
   
    void Strings::DeleteStringVectorContents(vector<string*>& strings) {
        vector<string*>::iterator itr;
        for(itr=strings.begin();itr!=strings.end();++itr) {
            delete *itr;
        }
    }
}
