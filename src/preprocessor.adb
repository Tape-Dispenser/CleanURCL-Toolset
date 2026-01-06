with Ada.Text_IO;
with Interfaces.C;
with Interfaces.C_Streams;
with Ada.Text_IO.C_Streams;
with Ada.Strings.Unbounded;
with Ada.Containers.Vectors;

use Ada.Text_IO;
use Interfaces.C_Streams;
use Ada.Text_IO.C_Streams;
use Interfaces.C;
use Ada.Strings.Unbounded;



package body Preprocessor is


   package Token_Vector is new Ada.Containers.Vectors (
    Index_Type => Natural,
    Element_Type => Ada.Strings.Unbounded.Unbounded_String
  );


   procedure HelloWorld is
      C_Out_File : Ada.Text_IO.File_Type;
   begin
      Open (
         File => C_Out_File, 
         Mode => Ada.Text_IO.Out_File, 
         C_Stream => Interfaces.C_Streams.stdout
      );
      Put_Line (C_Out_File, "Hello from Ada!");
   end HelloWorld;

   procedure Print_Line (Text : String) is
      C_Out_File : Ada.Text_IO.File_Type;
   begin
      Open (
         File => C_Out_File, 
         Mode => Ada.Text_IO.Out_File, 
         C_Stream => Interfaces.C_Streams.stdout
      );
      Put_Line (C_Out_File, Text);
   end Print_Line;

   procedure TokenTesting (C_String : Interfaces.C.char_array) is
      Ada_String : String := Interfaces.C.To_Ada (
         Item => C_String,
         Trim_Nul => True
      );
      Token : Ada.Strings.Unbounded.Unbounded_String := Ada.Strings.Unbounded.To_Unbounded_String ("");
      TokenVector : Token_Vector.Vector;
   begin
      for char : Character of Ada_String loop
         if char = ' ' 
         then
            TokenVector.Append(Token);
            Token := Ada.Strings.Unbounded.To_Unbounded_String ("");
         else
            Append (Token, char);
         end if;
      end loop;
      TokenVector.Append(Token);

      for Tok : Ada.Strings.Unbounded.Unbounded_String of TokenVector loop
         Print_Line (To_String (Tok));
      end loop;
   end TokenTesting;

end Preprocessor;