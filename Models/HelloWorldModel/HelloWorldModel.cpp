#include "stdafx.h"
#include "HelloWorldModel.h"

HelloWorldModel::HelloWorldModel(char* arguments[]) 
        : Model("HelloWorldModel",arguments) {
    Trace::WriteLine(0, "HelloWorldModel.Constructor called.");
    cout << "Hello World!\n";
}

int main(int argc, char *argv[ ]) {
    Debug::Initializer();
    if(Configuration::CheckForTerminalOption(argv)) return 0;
    Trace::WriteLine(0, "HelloWorldModel,main: start.");
    HelloWorldModel* model = new HelloWorldModel(argv);
    int result = model->Configure();
    if(result==0) {
        result = model->Run(0);
    }
    delete model;
    Trace::WriteLine(0, "HelloWorldModel,main: end.");
    Debug::Finalizer();
    return result;
}
