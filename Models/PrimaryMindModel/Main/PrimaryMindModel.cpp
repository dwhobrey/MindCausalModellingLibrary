#include "stdafx.h"
#include "PrimaryMindModel.h"

PrimaryMindModel::PrimaryMindModel(char* arguments[]) 
        : Model("PrimaryMindModel",arguments) {
    Trace::WriteLine(0, "PrimaryMindModel.Constructor called.");
}

int main(int argc, char *argv[ ]) {
    Debug::Initializer();
    if(Configuration::CheckForTerminalOption(argv)) return 0;
    Trace::WriteLine(0, "PrimaryMindModel,main: start.");
    PrimaryMindModel* model = new PrimaryMindModel(argv);
    int result = model->Configure(); 
    if(result==0) {
        result = model->Run(model->ModelConfiguration->EpochCount);
    }
    delete model;
    Trace::WriteLine(0, "PrimaryMindModel,main: end.");
    Debug::Finalizer();
    return result;
}
