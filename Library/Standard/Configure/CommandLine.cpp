#include "PlatoIncludes.h"
#include "Strings.h"
#include "Numbers.h"
#include "Trace.h"
#include "CommandLine.h"

namespace Plato {

    char** CommandLine::mDefaultCommandLineArguments = NULL;

    bool CommandLine::mDefaultHasExecutable = true;

    CommandLine::CommandLine(char** rawCommandLineArguments, bool hasExecutable) {
        mRawCommandLineArguments = rawCommandLineArguments == NULL ? mDefaultCommandLineArguments : rawCommandLineArguments;
        mHasExecutable = hasExecutable;
        mExecutable = NULL;
        mCommandLineOptions = new HashMap();
        mArguments = new vector<const string*>();

        // Convert command line options and arguments to a hashtable of (key,value) pairs
        // for options, and an array list of arguments.
        // Command line options must be of the form: --OptionName=Value.
        // The first non option is treated as the start of the arguments.

        try {
            char** arguments = mRawCommandLineArguments;
            bool isFirstArg = mHasExecutable;
            bool isArg = false;
            char** argvp = (char**)arguments;
            const char* arg;
            while ((arg = *argvp++)!=NULL) {
                bool isOption = false;
                // Strip off leading "/", "-" or "--".
                if(*arg=='/') {
                    ++arg;
                    isOption = true;
                }
                while (*arg=='-') {
                    ++arg;
                    isOption = true;
                }
                int index;
                string s(arg);
                string::size_type findIndex = s.find('=');
                if (findIndex == 0) {
                    continue;
                } else if(findIndex==string::npos) {
                    index = -1;
                } else {
                    index = (int)findIndex;
                }
                string* key = NULL;
                string* val = NULL;

                // Translate the first item, i.e. the name of executable:
                if (isFirstArg) {
                    isFirstArg = false;
                    val = new string(s);
                    mExecutable = val;
                    key = new string("Executable");
                } else {
                    if (index > 0) {
                        isOption = true;
                        key = new string(s.substr(0, index));
                        ++index;
                        val = (index<(int)s.size()) ? new string(s.substr(index + 1)) : new string("");
                    } else {
                        key = new string(s);
                    }
                    isArg |= !isOption;
                    if (isArg) {
                        mArguments->push_back(key);
                        continue;
                    }
                }
                (*mCommandLineOptions)[key] = val;
            }
        } catch (...) {
            Trace::WriteLine(1,"CommandLine.Parse: exception.");
        }
    }

    CommandLine::~CommandLine() {
        delete mArguments;
        HashMap::iterator itr;
        while((itr=mCommandLineOptions->begin())!=mCommandLineOptions->end()) {
            const string* key = itr->first;
            const string* val = itr->second;
            mCommandLineOptions->erase(itr);
            delete key;
            delete val;
        }
        delete mCommandLineOptions;
    }

    void CommandLine::SetDefaultCommandLineArguments(char** arguments, bool hasExecutable) {
        mDefaultCommandLineArguments = arguments;
        mDefaultHasExecutable = hasExecutable;
    }

    char** CommandLine::GetDefaultCommandLineArguments() {
        return mDefaultCommandLineArguments;
    }

    char** CommandLine::GetCommandLineArguments() {
        return mRawCommandLineArguments;
    }

    const string* CommandLine::GetOptionValue(const string& optionName) {
        HashMap::iterator itr = mCommandLineOptions->find(&optionName);
        if(itr!=mCommandLineOptions->end()) {
            return itr->second;
        }
        return NULL;
    }

    const string& CommandLine::GetOptionValue(const string& optionName, const string& defaultValue) {
        try {
            HashMap::iterator itr = mCommandLineOptions->find(&optionName);
            if(itr!=mCommandLineOptions->end()) {
                const string* pValue = itr->second;
                if (pValue!=NULL) {
                    return *pValue;
                }
            }
        } catch(...) {
        }
        return defaultValue;
    }

    long CommandLine::GetOptionValue(const string& optionName, long defaultValue) {
        try {
            HashMap::iterator itr = mCommandLineOptions->find(&optionName);
            if(itr!=mCommandLineOptions->end()) {
                const string* pValue = itr->second;
                if (pValue!=NULL) {
                    return Numbers::ToLong(*pValue);
                }
            }
        } catch(...) {
        }
        return defaultValue;
    }

    bool CommandLine::GetOptionValue(const string& optionName, bool defaultValue) {
        try {
            HashMap::iterator itr = mCommandLineOptions->find(&optionName);
            if(itr!=mCommandLineOptions->end()) {
                const string* pValue = itr->second;
                if (pValue!=NULL) {
                    return Numbers::ToBool(*pValue);
                }
            }
        } catch(...) {
        }
        return defaultValue;
    }

    const string* CommandLine::GetExecutable() {
        return mExecutable;
    }

    vector<const string*>& CommandLine::GetArguments() {
        return *mArguments;
    }

    CommandLine::HashMap& CommandLine::GetCommandLineOptions() {
        return *mCommandLineOptions;
    }

    bool CommandLine::ContainsOption(const string& optionName) {
        HashMap::iterator itr = mCommandLineOptions->find(&optionName);
        return itr!=mCommandLineOptions->end();
    }
}
