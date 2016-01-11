#pragma once
/// <summary>
/// This model explores the basic structure needed to produce a primary mind.
/// </summary>
class PrimaryMindModel : public Model {
public:
    /// <summary>
    /// Constructor for new model instances.
    /// Create the main element hierarchy of the model.
    /// </summary>
    /// <param name="arguments">The command line arguments.</param>
    PrimaryMindModel(char* arguments[]);
};

/// <summary>
/// Standard entry point.
/// </summary>
/// <param name="argc">Number of args</param>
/// <param name="argv">Normal command line options for model and plato library.</param>
/// <returns>Returns zero on success.</returns>
int main(int argc, char* argv[]);
