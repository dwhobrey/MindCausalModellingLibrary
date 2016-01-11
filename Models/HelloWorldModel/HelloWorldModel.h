#pragma once
/// <summary>
/// Simple test of an application derived from Model.
/// This example shows how to create models that can be executed by running: HelloWorldModel.exe
/// </summary>
class HelloWorldModel : public Model {
public:
    /// <summary>
    /// Constructor for new model instances.
    /// </summary>
    /// <param name="arguments">The command line arguments.</param>
    HelloWorldModel(char* arguments[]);
};

/// <summary>
/// Standard entry point.
/// </summary>
/// <param name="argc">Number of args</param>
/// <param name="argv">Normal command-line options for application and plato library.</param>
/// <returns>Returns zero on success.</returns>
int main(int argc, char* argv[]);
