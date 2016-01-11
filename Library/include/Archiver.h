#pragma once

namespace Plato {

    /// <summary>
    /// Handles serialising objects to and from the Hierarchy.
    /// </summary>
    class Archiver {
    public:
        /// <summary>
        /// Fetches an object from the Hierarchy via deserialisation.
        /// </summary>
        /// <remarks>Automatically determines whether archive is in binary or xml text format.</remarks>
        /// <param name="archivePath">The relative path and file name of serialized object to fetch.</param>
        /// <returns>Returns the deserialized object, if successful, otherwise NULL.</returns>
        static void* Deserialize(const string& archivePath);

        /// <summary>
        /// Saves an object to the Hierarchy via serialisation.
        /// </summary>
        /// <param name="archivePath">The relative path and file name to save object to.</param>
        /// <param name="inBinary">If true, object is saved to destination file in binary format, otherwise text.</param>
        /// <param name="obj">The object to serialise.</param>
        /// <returns>Returns zero if successful, otherwise an error code.</returns>
        static int Serialize(const string& archivePath, bool inBinary, void* obj);
    };
}
