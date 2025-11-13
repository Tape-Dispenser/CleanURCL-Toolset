with Interfaces.C.Strings;
package Preprocessor is
   function Process_Statement(statement, A, B, C : Interfaces.C.Strings.chars_ptr) return Integer;
   pragma Export(C, Process_Statement, "processStatement");
end Preprocessor;