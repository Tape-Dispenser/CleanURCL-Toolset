with Interfaces.C.Strings;
package Preprocessor is
   function Process_Statement(statement, a, b, c : Interfaces.C.Strings.chars_ptr) return Integer;
   pragma Export(C, Process_Statement, "processStatement");
   
   function Tokenize(statement : String) return Unbounded_String_Vectors.Vector;
   
   function Is_Same_Token(Char, Token_Type : Character) return Boolean;
end Preprocessor;