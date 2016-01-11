#pragma once

namespace Plato {
    /// <summary>
    /// Unit testing class for Archiver.
    /// </summary>
    class ArchiverTest {
    public:
        /// <summary>
        /// A serializable test class.
        /// </summary>
        class ArchieTestClass {
        public:
            /// <summary>
            ///
            /// </summary>
            int mA;
            /// <summary>
            ///
            /// </summary>
            string& mB;
            /// <summary>
            ///
            /// </summary>
            /// <param name="a"></param>
            /// <param name="b"></param>
            ArchieTestClass(int a, string& b);
            /// <summary>
            /// </summary>
            ~ArchieTestClass();
            /// <summary>
            ///
            /// </summary>
            /// <returns></returns>
            virtual string& StatusReport();
        };

        /// <summary>
        ///  The Unit Testing method.
        /// </summary>
        /// <param name="message">Diagnostic message.</param>
        /// <returns>Returns the number of failures, or zero on success.</returns>
        static int ConductUnitTest(string& message);
    };
}
