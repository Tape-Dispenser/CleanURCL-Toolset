with Ada.Containers;
with Ada.Containers.Indefinite_Vectors;
with Interfaces.C.Strings;
with Ada.Strings.Unbounded;
with Ada.Text_IO;
with Ada.Strings.Unbounded;
with Ada.Strings.Unbounded.Text_IO;
with Ada.Wide_Characters.Handling;



package body Preprocessor is

   package Unbounded_String_Vectors is new Ada.Containers.Indefinite_Vectors(
      Element_Type => Ada.Strings.Unbounded.Unbounded_String);

   function Process_Statement(statement, a, b, c : Interfaces.C.Strings.chars_ptr) return Integer is
      Tokens : Unbounded_String_Vectors.Vector;
      Statement : String := Value(statement);
      Operand_A : String := Value(a);
      Operand_B : String := Value(b);
      Operand_C : String := Value(c);
   begin
      Tokens := Tokenize(Statement);
      Put_Line("Statement:");
      Put_Line(Statement);
      Put_Line("Tokens:");
      for Token of Tokens loop
         Put_Line(To_String(Token));
      end loop;
      return 0;
   end Process_Statement;

   function Tokenize(statement : String) return Unbounded_String_Vectors.Vector is
      Tokens : Unbounded_String_Vectors.Vector;
      Current_Token : Unbounded_String;
      In_Token : Boolean := False;
      Token_Type : Character := Character'Val(0);
      Last_Token_Type : Character := Character'Val(0);
      Valid_Operator_Chars : String := "*/+-<>=!&|";
   begin
      for Char of statement loop
         if In_Token
         then
            if Is_Same_Token(Char, Token_Type)
            then
               Append(Current_Token, Char);
            else
               -- add current token to tokens, current character is part of a new token
               Unbounded_String_Vectors.Append(Tokens, Current_Token);
               goto New_Token;
            end if;
         else
            <<New_Token>>
            -- reset token
            Current_Token := Null_Unbounded_String;
            Last_Token_Type := Token_Type;
            Token_Type := Character'Val(0);
            Append(Current_Token, Char);
            if Is_Alphanumeric(Char) 
            then
               -- start of Value token
               Token_Type := 'V';
               In_Token := True;
               goto Continue_Loop;
            elsif Char = '~' 
            then
               Token_Type := 'M';
            elsif Char = '-' 
            then
               -- if previous token is not a Value or Subvalue End token then - is a Modifier, otherwise it is an operator
               if Last_Token_Type /= 'V' and Last_Token_Type /= 'E'
               then
                  Token_Type := 'M';
               else
                  Token_Type := 'O';
                  In_Token := True;
                  goto Continue_Loop;
               end if;
            elsif Index(Valid_Operator_Chars, Char) /= 0 then
               Token_Type := 'O';
               In_Token := True;
               goto Continue_Loop;
            elsif Char = '('
            then
               Token_Type := 'S';
            elsif Char = ')'
            then
               Token_Type := 'E';
            end if;
            -- add token to tokens (only reached if token could only be 1 character long)
            Unbounded_String_Vectors.Append(Tokens, Current_Token);
         end if;
         <<Continue_Loop>>
      end loop;
      return Tokens;
   end Tokenize;

   function Is_Same_Token(Char, Token_Type : Character) return Boolean is
      Valid_Operator_Chars : String := "*/+-<>=!&|";
      Invalid_Token_Type_Error : exception;
      Error_Message : Unbounded_String;
   begin
      case Token_Type is
         when 'V' =>
            if Is_Alphanumeric(Char)
            then
               return True;
            end if;
         when 'M' =>
            if Is_Letter(Char)
            then
               return True;
            end if;
         when 'O' =>
            if Index(Valid_Operator_Chars, Char) /= 0 then
               return True;
            end if;
         when 'S' =>
            return False;
         when 'E' =>
            return False;
         when others =>
            Error_Message := "Error: Invalid token type """ & Char & """";
            raise Invalid_Token_Type_Error with "";

      end case;
   end Is_Same_Token;

end Preprocessor;