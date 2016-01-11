#pragma once

namespace Plato {

#pragma region // CatalogueCode enum.
    namespace CatalogueCodeFields {
        /// <summary>
        /// The fields that make up a Catalogue Code.
        /// </summary>
        enum Enum {
            /// <summary>
            /// The library field.
            /// </summary>
            Library,
            /// <summary>
            /// The class field.
            /// </summary>
            Class,
            /// <summary>
            /// The statement field.
            /// </summary>
            Statement
        };
    }
    typedef CatalogueCodeFields::Enum CatalogueCodeFieldsEnum;
#pragma endregion

    /// <summary>
    /// This class is used to manage a table of
    /// the libraries and classes used in an application.
    /// It is used for generating constant type IDs and error codes.
    /// </summary>
    class ClassCatalogue {
    public:
        /// <summary>
        /// The maximum value for a catalogue code field.
        /// </summary>
        static const int CatalogueCodeFieldMaximum[3];

        /// <summary>
        /// The first field value for a field within a catalogue code.
        /// </summary>
        static const int CatalogueCodeFieldBase[3];

        /// <summary>
        /// The first field bit index for a field within a catalogue code.
        /// </summary>
        static const int CatalogueCodeFieldStartBit[3];

        /// <summary>
        /// Combine the member catalogue ID's into a single catalogue code.
        /// </summary>
        /// <remarks>
        /// Default formula for a 31-bit int code is as follows (sign bit always zero) in hex: 0xLLCCCSSS.
        /// <list type="table">
        /// <item><description>LL  = Library code   [0..127]  - catalogued libraries per application limited to less than 128.</description></item>
        /// <item><description>CCC = Class code     [0..4095] - libraries limited to less than 4096 catalogued classes.</description></item>
        /// <item><description>SSS = Statement code [0..4095] - classes limited to less than 4096 catalogued statements.</description></item>
        /// </list>
        /// Note, a zero field value is used to signify an out of range or unknown member ID.
        /// If a member catalogue ID is out of range it is set to zero.
        /// </remarks>
        /// <param name="libraryID"></param>
        /// <param name="classID"></param>
        /// <param name="statementID"></param>
        /// <returns>Returns the composite catalogue code for the given member catalogue IDs.</returns>
        static int CatalogueCode(int libraryID, int classID, int statementID);

        /// <summary>
        /// Extract the value of the specified field from the catalogue code.
        /// </summary>
        /// <param name="catalogueCodeField">The catalogue field to extract.</param>
        /// <param name="catalogueCode">The catalogue code to extract the field from.</param>
        /// <returns>Returns the field value.</returns>
        static int GetCatalogueField(CatalogueCodeFieldsEnum catalogueCodeField, int catalogueCode);

        /// <summary>
        /// Set the field value of a catalogue code.
        /// </summary>
        /// <param name="catalogueCodeField">The categlogue field to set.</param>
        /// <param name="catalogueCode">The catalogue code to merge with the field value.</param>
        /// <param name="fieldValue">The value to set the field to. If this is out of range the field will be set to zero.</param>
        /// <returns>Returns a new catgelogue code based on the given catalogue code and the field value.</returns>
        static int SetCatalogueField(CatalogueCodeFieldsEnum catalogueCodeField, int catalogueCode, int fieldValue);

        /// <summary>
        /// Searches for class name in ClassCatalogue table.
        /// </summary>
        /// <returns>Returns the ClassTypeInfo object, or NULL if not found.</returns>
        static const ClassTypeInfo* FindEntry(const string& className);

        /// <summary>
        /// The library's table of class names and class IDs.
        /// </summary>
        /// <remarks>
        /// Classes requiring IDs or that report errors should add an entry to this table.
        /// The table must be in alphabetical order.
        /// </remarks>
        static ClassTypeInfo::HashMap* ClassCatalogueTable;
        /// <summary>
        /// Adds an entry to the catalogue.
        /// </summary>
        /// <remarks>The catalogue then owns className string.</remarks>
        /// <param name="classInfoPtr">The class info to add to the table.</param>
        static void Add(const ClassTypeInfo* classInfoPtr);

    private:
        friend class InitializerCatalogue;
        /// <summary>
        /// The class initializer.
        /// </summary>
        static void Initializer();
        /// <summary>
        /// The class finalizer.
        /// </summary>
        static void Finalizer();
    };
}
