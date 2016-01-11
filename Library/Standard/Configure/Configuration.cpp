#include "PlatoIncludes.h"
#include "Strings.h"
#include "Numbers.h"
#include "Trace.h"
#include "CommandLine.h"
#include "Configuration.h"

namespace Plato {

#pragma region // VariableAccessor class.
    VariableAccessor::VariableAccessor(VariablePtr varPtr, AccessorPtr accPtr) {
        VarPtr = varPtr;
        AccPtr = accPtr;
        IsStatic = false;
    }

    VariableAccessor::VariableAccessor(const void* varPtr, AccessorPtr accPtr) {
        StaticPtr = varPtr;
        AccPtr = accPtr;
        IsStatic = true;
    }

    const void* VariableAccessor::IntAccessor(const void* varAdrs,const void* valuePtr,bool isSetting,bool isString) {
        if(isSetting) {
            *((int*)varAdrs) = isString ? Numbers::ToInt(*((string*)valuePtr)) : *((int*)valuePtr);
            return NULL;
        }
        return (isString ? (void*)&(Numbers::ToString(*((int*)varAdrs))) : varAdrs);
    }

    const void* VariableAccessor::LongAccessor(const void* varAdrs,const void* valuePtr,bool isSetting,bool isString) {
        if(isSetting) {
            *((long*)varAdrs) = isString ? Numbers::ToLong(*((string*)valuePtr)) : *((long*)valuePtr);
            return NULL;
        }
        return (isString ? (void*)&(Numbers::ToString(*((long*)varAdrs))) : varAdrs);
    }

    const void* VariableAccessor::BoolAccessor(const void* varAdrs,const void* valuePtr,bool isSetting,bool isString) {
        if(isSetting) {
            *((bool*)varAdrs) = isString ? Numbers::ToBool(*((string*)valuePtr)) : *((bool*)valuePtr);
            return NULL;
        }
        return (isString ? (void*)&(Numbers::ToString(*((bool*)varAdrs))) : varAdrs);
    }


    const void* VariableAccessor::StringAccessor(const void* varAdrs,const void* valuePtr,bool isSetting,bool isString) {
        if(isSetting) {
            *((string*)varAdrs) =  *((string*)valuePtr);
            return NULL;
        }
        return varAdrs;
    }
#pragma endregion

    const char* Configuration::Acknowledgements = 
         "Acknowledgements:\n"
         "Darren Whobrey, founder;\n"
         "Kostadino Zafirakos, model design;\n"
         "Ting Jie Yuan, superposition;\n"
         "Dixon Ng, interconnect;\n"
         "Jerry Huang, converters;\n"
         "Fred Yu-Han Chu, birth and death of DIDs;\n"
         "Howard Kuo, visualisation of DIDs;\n"
         "Yung-Hsiang Wu, visualisation of DIDs;\n"
         "Kyaw Ko-Ko Zin, scalability.";
    const char* Configuration::Copyright = "Copyright 1999-2008, Mild AI Organisation, www.mildai.org.";
    const char* Configuration::OptionUsage = 
        "Plato library options:\n"
        "--acknowledgements\t\tDisplay copyright and contribution information.\n"
        "--ArchivePath=FilePath\t\tLocal path to file containing previously archived application state.\n"
        "--BinarySerialize[=true|false]\tIndicates to serialise in binary rather than xml.\n"
        "--ConfigFile=FilePath\t\tUse this configuration file.\n"
        "--DebugLevel=Number\t\tSet to a postive integer for increasing debug information.\n"
        "--RunDiagnostics[=true|false]\tTurn diagnostic tests on or off.\n"
        "--EpochCount=Number\t\tThe number of epochs to run model for, default = -1, i.e. non-stop.\n"
        "--help\t\t\t\tDisplay this usage information.\n"
        "--PathPrefix=DirPath\t\tUse this as a prefix for file system paths.\n"
        "--PlatoPrefix=DirPath\t\tThe installation directory of the Plato library.\n"
        "--TraceLevel=Number\t\tSet to a postive integer for increasing trace information.\n"
        "--version\t\t\tDisplay version information."; 
    const char* Configuration::Version = "Plato Library, version:0.1.0.0";

    int Configuration::DebugLevel = 0;
    int Configuration::TraceLevel = 0;
    string* Configuration::mFullPathPtr = NULL;
    string* Configuration::mDirectoryPtr = NULL;
    Configuration::HashMap* Configuration::mVariableAccessorHashMap = NULL;

    void Configuration::Initializer() {
        mFullPathPtr = NULL;
        mDirectoryPtr = NULL;
        mVariableAccessorHashMap = new HashMap();
        string* s; // This is to avoid a VS /W4:C4709 warning.
        (*mVariableAccessorHashMap)[s=new string("ArchivePath")]=new VariableAccessor((VariableAccessor::VariablePtr)&Configuration::ArchivePath,VariableAccessor::StringAccessor);
        (*mVariableAccessorHashMap)[s=new string("BinarySerialize")]=new VariableAccessor((VariableAccessor::VariablePtr)&Configuration::BinarySerialize,VariableAccessor::BoolAccessor);
        (*mVariableAccessorHashMap)[s=new string("ConfigFile")]=new VariableAccessor((VariableAccessor::VariablePtr)&Configuration::ConfigFile,VariableAccessor::StringAccessor);
        (*mVariableAccessorHashMap)[s=new string("DebugLevel")]=new VariableAccessor((const void*)&Configuration::DebugLevel,VariableAccessor::IntAccessor);
        (*mVariableAccessorHashMap)[s=new string("RunDiagnostics")]=new VariableAccessor((VariableAccessor::VariablePtr)&Configuration::RunDiagnostics,VariableAccessor::BoolAccessor);
        (*mVariableAccessorHashMap)[s=new string("EpochCount")]=new VariableAccessor((VariableAccessor::VariablePtr)&Configuration::EpochCount,VariableAccessor::LongAccessor);
        (*mVariableAccessorHashMap)[s=new string("PathPrefix")]=new VariableAccessor((VariableAccessor::VariablePtr)&Configuration::PathPrefix,VariableAccessor::StringAccessor);
        (*mVariableAccessorHashMap)[s=new string("PlatoPrefix")]=new VariableAccessor((VariableAccessor::VariablePtr)&Configuration::PlatoPrefix,VariableAccessor::StringAccessor);
        (*mVariableAccessorHashMap)[s=new string("TraceLevel")]=new VariableAccessor((const void*)&Configuration::TraceLevel,VariableAccessor::IntAccessor);
    }

    void Configuration::Finalizer() {
        delete mFullPathPtr;
        delete mDirectoryPtr;
        HashMap::iterator itr;
        while((itr=mVariableAccessorHashMap->begin())!=mVariableAccessorHashMap->end()) {
            const string* key = itr->first;
            const VariableAccessor* val = itr->second;
            mVariableAccessorHashMap->erase(itr);
            delete key;
            delete val;
        }
        delete mVariableAccessorHashMap;
    }

    Configuration::Configuration(const string& modelName, char* arguments[]) {
        DebugLevel = 0;
        TraceLevel = 0;
        BinarySerialize = false;
        RunDiagnostics = false;
        EpochCount = 0;
        ArchivePath = NULL;
        ConfigFile = NULL;
        PathPrefix = NULL;
        PlatoPrefix = NULL;
        ModelParameters = NULL;
        CheckOptionValues(true);
        ModelParameters = new CommandLine(arguments);

        // Work out a config file to open.
        const string* newConfigFile = ModelParameters->GetOptionValue("ConfigFile");
        try {
            // Initialise the name of the config file to load.
            if (newConfigFile==NULL || newConfigFile->empty()) {
                string* tmp = new string(GetExecutableDirectory());
                *tmp += PlatformDirectorySeparator + modelName + ".config";
                newConfigFile = tmp;
            }
        } catch (...) {
            Trace::WriteLine(1, "Configuration: Configuration: setting ConfigFile exception.");
            string* tmp = new string(modelName);
            *tmp += ".config";
            newConfigFile =  tmp;
        }
        // Find an appropriate config file that exists.
        ifstream* textFile = new ifstream(newConfigFile->c_str());
        if(textFile->fail()) {
            delete textFile;
            textFile = new ifstream(ConfigFile->c_str());
            if(textFile->fail()) {
                delete ConfigFile;
                ConfigFile = newConfigFile;
            } else {
                delete newConfigFile;
            }
        } else {
            delete ConfigFile;
            ConfigFile = newConfigFile;
        }
        delete textFile;
        LoadConfigFile();
    }

    Configuration::~Configuration() {
        delete ArchivePath;
        delete ConfigFile;
        delete PathPrefix;
        delete PlatoPrefix;
        delete ModelParameters;
    }

    void Configuration::CheckOptionValues(bool reset) {
        Strings::CheckValue(ArchivePath, "", reset);
        Numbers::CheckValue(BinarySerialize, false, true, false, reset);
        Strings::CheckValue(ConfigFile, "Plato.config", reset);
        Numbers::CheckValue(DebugLevel, 0, 10, 10, reset);
        Numbers::CheckValue(RunDiagnostics, false, true, true, reset);
        Numbers::CheckValue(EpochCount, 0, LONG_MAX, -1, reset);
        Strings::CheckValue(PathPrefix, "$(PlatoPrefix)Plato", reset);
        Strings::CheckValue(PlatoPrefix, GetExecutableDirectory(), reset);
        Numbers::CheckValue(TraceLevel, 0, 10, 10, reset);
    }

    const void* Configuration::GetProperty(const string& name,bool isString) const {
        HashMap::iterator itr = mVariableAccessorHashMap->find(&name);
        if(itr!=mVariableAccessorHashMap->end()) {
            const VariableAccessor* pVA = itr->second;
            if(pVA!=NULL) {
                if(pVA->IsStatic) {
                    return (*(pVA->AccPtr))(pVA->StaticPtr,NULL,false,isString);
                }
                return (*(pVA->AccPtr))((const void*)&(this->*(pVA->VarPtr)),NULL,false,isString); 
            }
        }
        return NULL;
    }

    void Configuration::SetProperty(const string& name, const void* value,bool isString) {
        HashMap::iterator itr = mVariableAccessorHashMap->find(&name);
        if(itr!=mVariableAccessorHashMap->end()) {
            const VariableAccessor* pVA = itr->second;
            if(pVA!=NULL) {
                if(pVA->IsStatic) {
                    (*(pVA->AccPtr))(pVA->StaticPtr,value,true,isString);
                } else {
                    (*(pVA->AccPtr))((const void*)&(this->*(pVA->VarPtr)),value,true,isString);
                }
            }
        }
    }

    const string& Configuration::GetExecutableFullPath() {
        if (mFullPathPtr == NULL) {
            mFullPathPtr = &(GetExecutablePath());
        }
        return *mFullPathPtr;
    }

    const string& Configuration::GetExecutableDirectory() {
        if (mDirectoryPtr == NULL) {
            string* path = &GetDirectoryPath(GetExecutableFullPath());
            // DEBUG: kludge for when developing: MS regularly cleans Debug dir.
            if (Strings::EndsWith(*path,"debug")) {
                string* tmp = path;
                path = &GetDirectoryPath(*path);
                delete tmp;
            }
            mDirectoryPtr = path;
        }
        return *mDirectoryPtr;
    }

    string& Configuration::GetExecutableFileName() {
        return GetPathTail(GetExecutableFullPath());
    }

    bool Configuration::CheckForTerminalOption(char **arguments) {
        if(arguments!=NULL) {
            const char* arg;
            while ((arg = *arguments++)!=NULL) {
                while (*arg=='-'||*arg=='/') ++arg;
                // Check if the help option was used.
                if(strcmp(arg,"?")==0 || strcmp(arg,"help")==0) {
                    cout << Version << ", " << Copyright << "\n" << OptionUsage;
                    return true;
                } 
                // Check if the version option was used.
                if (strcmp(arg,"version")==0) {
                    cout << Version << ", " << Copyright;
                    return true;
                }
                // Check if acknowledgements option was used.
                if (strcmp(arg,"acknowledgements")==0) {
                    cout << Version << ", " << Copyright << "\n" << Acknowledgements;
                    return true;
                }
            }
        }
        return false;
    }

    void Configuration::LoadConfigFile() {

        // Open the config file, if possible, and load in configurable variables.
        if (ConfigFile != NULL) {
            vector<string*>* varVector = ReadTextFile(*ConfigFile);
            if(varVector!=NULL) {
                vector<string*>::iterator itr;
                for(itr=varVector->begin();itr!=varVector->end();itr++) {
                    string* pS = *itr;
                    string::size_type index = pS->find('=');
                    if (index!=string::npos) {
                        string* key = new string(pS->substr(0, index));
                        ++index;
                        string* val = new string((index<pS->size()) ? pS->substr(index) : "");
                        SetProperty(*key,(void*)val,true);
                        delete key;
                        delete val;
                    }
                    delete pS;
                }
                delete varVector;
            }
        }

        // Update the config values from the model arguments.
        CommandLine::HashMap& hm = ModelParameters->GetCommandLineOptions();
        CommandLine::HashMap::iterator itr;
        for(itr=hm.begin();itr!=hm.end();itr++) {
            const string* pS = itr->first;
            const string* pV = itr->second;
            try {
                SetProperty(*pS, (const void*)pV,true);
            } catch (...) {
                Trace::WriteLine(1, "Configuration.LoadConfigFile: setting property: %s, exception ignored.", pS->c_str());
            }
        }

        // Check the property values are valid and in range.
        CheckOptionValues(false);
    }

    bool Configuration::SaveConfigFile() {
        // DEBUG: TO DO.
        return true;
    }

    vector<string*>* Configuration::ReadTextFile(const string& fileName) {
        const int BufferSize = 500;
        vector<string*>* lines = new vector<string*>();
        try {
            ifstream textFile( fileName.c_str() );
            ifstream::char_type buffer[BufferSize];
            if(!textFile.fail()) { 
                try {
                    for(;;) {
                        textFile.getline(buffer,BufferSize*sizeof(ifstream::char_type));
                        if(textFile.fail()) break;
                        string& line = *new string(buffer);
                        Strings::Trim(line);
                        if (line.empty()||line[0]=='#') {
                            delete &line;
                            continue;
                        }
                        lines->push_back(&line);
                    }
                } catch (...) {
                    Trace::WriteLine(1,"Configuration.ReadTextFile: warning: reading exception.");
                }
            }
        } catch (...) {
            Trace::WriteLine(1,"Configuration.ReadTextFile: warning: opening exception.");
            delete lines;
            return NULL;
        }
        return lines;
    }

    string& Configuration::GetDirectoryPath(const string& path) {
        string::size_type index = path.find_last_of("\\/");
        if(index!=string::npos&&index>0) {
            return *new string(path.substr(0,index)); // DEBUG: Possible memory leak/
        }
        return *new string(path);
    }

    string& Configuration::GetPathTail(const string& path) {
        string::size_type index = path.find_last_of("\\/");
        if(index!=string::npos) {
            ++index;
            if(index<path.size()) {
                return *new string(path.substr(0,index)); // DEBUG: Possible memory leak/
            }
            return *new string("");
        }
        return *new string(path);
    }

    string& Configuration::GetCurrentWorkingDirectory() {
        char buffer[512];
#ifdef WIN32
        return *(new string(_getcwd(buffer,(int)(sizeof(buffer)))));
#else
        return *(new string(getcwd(buffer,sizeof(buffer))));
#endif
    }

#pragma region // GetExecutablePath.

#ifdef WIN32
    string& Configuration::GetExecutablePath() {
        string& path = *new string("");
        char buffer[512];
        DWORD len = GetModuleFileNameA(NULL,buffer,(DWORD)sizeof(buffer));
        if(len>0) path += buffer;
        return path;
    }
#endif

#ifdef MAC // Thanks to benacler, Lazio, Italy.
    string& Configuration::GetExecutablePath() {
        string& path = new string("");
        ProcessSerialNumber PSN;
        ProcessInfoRec pinfo;
        FSSpec pspec;
        FSRef fsr;
        OSStatus err;
        // set up process serial number.
        PSN.highLongOfPSN = 0;
        PSN.lowLongOfPSN = kCurrentProcess;
        // set up info block.
        pinfo.processInfoLength = sizeof(pinfo);
        pinfo.processName = NULL;
        pinfo.processAppSpec = &pspec;
        // grab the vrefnum and directory.
        err = GetProcessInformation(&PSN, &pinfo);
        if (! err ) {
            char c_path[2048];
            FSSpec fss2;
            int tocopy;
            err = FSMakeFSSpec(pspec.vRefNum, pspec.parID, 0, &fss2);
            if ( ! err ) {
                err = FSpMakeFSRef(&fss2, &fsr);
                if ( ! err ) {
                    char c_path[2049];
                    err = (OSErr)FSRefMakePath(&fsr, (UInt8*)c_path, 2048);
                    if (! err ) {
                        path += c_path;
                    }
                }
            }
        }
        return path;
    }
#endif

#if defined(POSIX) || defined(__CYGWIN__) // Thanks to benacler, Lazio, Italy.
    string& Configuration::GetExecutablePath() {
        pid_t pid = getpid();
        char buf[10];
        sprintf(buf,"%d",pid);
        string _link = "/proc/";
        _link.append( buf );
        _link.append( "/exe");
        char proc[512];
        int ch = readlink(_link.c_str(),proc,512);
        if (ch != -1) { 
            proc[ch] = 0; 
            string tmp = proc; 
            string::size_type t = tmp.find_last_of("/");
            return * new string(tmp.substr(0,t));
        }
        return *new string("");
    }
#endif
#pragma endregion
}
