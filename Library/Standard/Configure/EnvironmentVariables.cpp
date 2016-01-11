#include "PlatoIncludes.h"
#include "Strings.h"
#include "CommandLine.h"
#include "Configuration.h"
#include "EnvironmentVariables.h"

namespace Plato {

    string& EnvironmentVariables::GetRawVariable(const string &name) {
        const char* varName = name.c_str();
#ifdef WIN32
        size_t requiredSize;
        getenv_s( &requiredSize, NULL, 0, varName);
        char* varValue = new char[requiredSize];
        if (varValue==NULL) {
            return *new string("SizeOverflow");
        }
        getenv_s( &requiredSize, varValue, requiredSize, varName );
        string* s = new string(varValue);
        delete [] varValue;
#else
        char* varValue = getenv(varName);
        if (varValue==NULL) {
            return *new string("SizeOverflow");
        }
        string* s = new string(varValue);
#endif
        return *s;
    }

    string& EnvironmentVariables::GetVariable(const Configuration& configuration, const string& name) {
        CommandLine* cl = configuration.ModelParameters;
        const string* pS;
        if (cl->ContainsOption(name)) {
            pS = cl->GetOptionValue(name);
            if(pS!=NULL) {
                return *new string(*pS);
            }
        }
        string& val = GetRawVariable(name);
        if(!val.empty()) {
            return val;
        }
        pS = (const string*)(configuration.GetProperty(name,true));
        if(pS==NULL) {
            return *new string("");
        }
        return *new string(*pS);
    }

    string& EnvironmentVariables::ExpandString(const Configuration& configuration, const string& text) {
        string* s = new string(text);
        string::size_type index = 0;
        while ((index = s->find('$',index)) != string::npos) {
            string::size_type closingBracketIndex = s->find(')');
            if ((s->size() < index + 4) || ((*s)[index + 1] != '(') 
                || (closingBracketIndex < index) || (closingBracketIndex==string::npos)) {
                ++index;
                continue;
            }
            string* oldValue = new string(s->substr(index, 1 + closingBracketIndex - index));
            string* envName = new string(s->substr(index + 2, closingBracketIndex - index - 2));
            if (envName->size() > 0) {
                string* envValue = &(GetVariable(configuration,*envName));
                string* r = &(Strings::Replace(*s, *oldValue, *envValue));
                delete s;
                delete envValue;
                s = r;
            }
            delete oldValue;
            delete envName;
            ++index;
        }
        return *s;
    }
}
