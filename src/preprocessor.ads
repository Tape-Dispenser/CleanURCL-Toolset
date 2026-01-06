with Interfaces.C;
with Ada.Containers.Vectors;
with Ada.Strings.Unbounded;



package Preprocessor is

   procedure HelloWorld;
   pragma Export(C, HelloWorld, "hello_world");

   procedure TokenTesting (C_String : Interfaces.C.char_array);
   pragma Export (C, TokenTesting, "token_testing");

   procedure Print_Line (Text : String);

end Preprocessor;