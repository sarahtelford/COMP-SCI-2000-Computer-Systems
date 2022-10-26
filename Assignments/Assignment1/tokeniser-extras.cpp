// a1810750
#include "iobuffer.h"
#include "tokeniser-extras.h"

// to shorten the code
using namespace std ;
using namespace CS_IO_Buffers ;

namespace Assignment_Tokeniser
{
    // check if a char matches another char or is a member of a character group
    // eg char_isa('3',cg_digit) returns true
    bool char_isa(int ch,int cg)
    {
        // single character groups use their character as their value
        // do not add them to the switch statement

        if ( ch == cg ) return true ;

        switch(cg)
        {
        case cg_wspace:              // characters that start rule wspace
            switch(ch)
            {
                case '\t':
                return true ;

                case '\n':
                return true ;

                case '\r':
                return true ;

                case ' ':
                return true ;

            default:
                return false ;
            }

        case cg_symbol:             // characters that start rule symbol
            switch(ch)
                {
                case '@':
                    return true;
                case '~':
                    return true;
                case '=':
                    return true;
                case '-':
                    return true;
                case '+':
                    return true;
                case '/':
                    return true;

                case '*':
                    return true;
                case '<':
                    return true;
                case '>':
                    return true;


                case '*'+'=':
                    return true;
                case '~'+'=':
                    return true;
                case '='+'=':
                    return true;
                case '<'+'<'+'<':
                    return true;
                case '<'+'<':
                    return true;
                case '>'+'>'+'>':
                    return true;
                case '>'+'>':
                    return true;
                case '{':
                    return true;
                case '}':
                    return true;
                case '(':
                    return true;
                case ')':
                    return true;
                case '[':
                    return true;
                case ']':
                    return true;
                case '.':
                    return true;

                default:
                    return false ;
            }
                                    // complete a switch statement for each of the following
        case cg_identifier:         // characters that start rule identifier
           switch(ch)
                {
                case 'a'...'z': 
                return true ;

                case 'A'...'Z':
                return true ;

                case '$':
                return true;

            default:
                return false ;
                }


        case cg_integer:             // characters that start rule integer
            switch(ch)
            {
            case '0'...'9':
                return true ;

            default:
                return false ;
            }

        case cg_digit19:             // characters that start rule digit19
            switch(ch)
            {
            case '1'...'9':
                return true ;
            default:
                return false ;
            }

        case cg_digit:               // characters that start rule digit
            switch(ch)
            {
            case '0'...'9':
                return true ;
            default:
                return false ;
            }

        case cg_letter:             // characters that start rule letter
            switch(ch)
            {
            case 'a'...'z': 
                return true ;
            case 'A'...'Z':
                return true ;
            case '0'...'9':  
                return true ;
            case '$':
                return true;
            case '-':
                return true;
            case '.':
                return true;

            default:
                return false ;
            }

        case cg_scientific:           // characters that start rule exponent
            switch(ch)
                {
            case '+':
                return true; 
            case '-':
                return true; 
            case 'E':
                return true; 
            case 'e':
                return true; 
            case '.':
                return true;
            default:
                return false; 
            } 

        case cg_eee:                // characters that start rule eee
            switch(ch)
            {
            case 'e':
                return true; 
            case 'E':
                return true; 
            default:
                return false; 
            } 

        case cg_sign:               // characters that start rule eee
            switch(ch)
            {
            case '+':
                return true; 
            case '-':
                return true; 
            default:
                return false; 
            } 


        case cg_exponent:         // characters that start rule scientific
            switch(ch)
            {
            case 'e':
                return true; 
            case 'E':
                return true; 
            case '-':
                return true;
            case '+':
                return true; 
            default:
                return false; 
            } 

        case cg_number:             // characters that start rule number
            switch(ch)
            {
            case '+':
                return true; 
            case '-':
                return true; 
            case 'E':
                return true; 
            case 'e':
                return true; 
            case '0' ... '9':
                return true;
            case '.':
                return true;
            default:
                return false; 
            }
                


        case cg_comment_char:       // characters that start rule comment_char
            //     switch(ch)
            // {
            //     case ' ':
            //     return true; 
            //     case '!'
            //     return true; 
            //     case '#' - '~':
            //     return true; 
            //     case '!'
            //     return true; 
            //     default:
            //     return false; 
            // } 


                 // characters that start rule armenian
       
        case cg_instring:           // characters that start rule instring
        switch(ch)
            {
                case ' ':
                return true; 
                case '!':
                return true; 
                case '#' - '~':
                return true; 
                case 0x0C00 ... 0x0C7F:
                return true; 
                default:
                return false; 
            } 

        case cg_telegu:
                switch(ch)
            {
                case 0x0C00 ... 0x0C7F:
                return true; 
                default:
                return false; 
            } 

        case cg_armenian:  
          switch(ch)
            {
                case 0x0530 ... 0x058F:
                return true; 
                case 0xFB13 ... 0xFB17:
                return true; 
                default:
                return false ;
            } 

        default:
            return false ;
        }
    }

    // work out the kind of a parsed token, this may call keyword_or_identifier()
    // the spelling is a valid token or "" if at end of input
    TokenKind classify_spelling(string spelling)
    {
        if ( spelling == "" ) return tk_eoi ;

        switch(spelling[0])
            {
            case '\t':
            return tk_tab ;

            case '\n':
            return tk_newline ;

            case '\r':
            return tk_carriage_return ;

            case ' ': 
            return tk_space ;

            case '0' ... '9': 
            return tk_integer;

            case '@':
                return tk_at;

            case '~':
                if(spelling[1] == '='){
                    return tk_ne;
                }
                else {
                    return tk_not;
                }

            case '=':
                if(spelling[1] == '='){
                    return tk_eq;
                }
                else{ 
                    return tk_assign;
                }

            case '-':
                return tk_sub;
            case '+':
                return tk_add;
            case '/':
                return tk_div;
            case '{':
                return tk_lcb;
            case '}':
                return tk_rcb;
            case '(':
                return tk_lrb;
            case ')':
                return tk_rrb;
            case '[':
                return tk_lsb;
            case ']':
                return tk_rsb;
            case '.':
                return tk_dot;

            case '$': 
            return keyword_or_identifier(spelling); 

            case 'a' ... 'z':
                return keyword_or_identifier(spelling); 

            case'A' ... 'Z':
            return keyword_or_identifier(spelling); 

            case '*':
                switch(spelling[1]){
                case '=': 
                    return tk_mult;
                }
                
            case '<': 
                if(spelling.length() == 3){
                    if (spelling[2] == '<'){
                        return tk_ll_shift;
                    }

                    else {
                        return tk_la_shift;
                    }
        
                }

            case '>':
                if(spelling.length() == 3){
                    if (spelling[2] == '>'){
                        return tk_rl_shift;
                    }

                    else {
                        return tk_ra_shift;
                    }
        
                }


            case 0x0530 ... 0x058F:
                return tk_hash_comment; 
            
            case 0xFB13 ... 0xFB17:
                return tk_hash_comment; 

            case 0x0C00 ... 0x0C7F:
                return tk_eol_comment; 

            default:
            return tk_oops ;
        } 

        return tk_identifier;
    }

    // work out the correct spelling to use in the Token object being created by new_token()
    // the spelling is a valid token and kind is its kind
    string correct_spelling(TokenKind kind,string spelling)
    {
        if ( spelling == "" ) return "" ;

        return spelling ;
    }

}
