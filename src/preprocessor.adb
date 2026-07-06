-- preprocessor.adb: parse and compute CleanURCL preprocessor statements
-- Copyright (C) 2025-2026, Ada (Tape), <adadispenser@gmail.com>
--
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU Affero General Public License as published
-- by the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU Affero General Public License for more details.
--
-- You should have received a copy of the GNU Affero General Public License
-- along with this program.  If not, see <https://www.gnu.org/licenses/>.

with Ada.Text_IO;
with Interfaces.C;
with Interfaces.C_Streams;
with Ada.Text_IO.C_Streams;
with Ada.Strings.Unbounded;
with Ada.Containers.Vectors;
with Ada.Exceptions;

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

      for I in TokenVector.First_Index .. TokenVector.Last_Index loop
         Put_Line(To_String (TokenVector.Element (I)));
      end loop;

   end TokenTesting;

end Preprocessor;