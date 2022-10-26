// convert Text into Tokens wrapped up in XML
#include "iobuffer.h"
#include "tokeniser-extras.h"
#include <unordered_map>
#include <iostream>
#include <vector>

// to shorten the code
using namespace std ;
using namespace CS_IO_Buffers ;

namespace Workshop_Tokeniser
{
    // check if a char matches another char or is a member of a character group
    // eg char_isa('3',cg_digit) returns true
    bool char_isa(int ch,int cg)
    {
        // first check for single character match
        if ( ch == cg ) {
            return true ;
        }

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

        case cg_identifier:          // characters that start rule identifier
            switch(ch)
            {
                case 'a'...'z': 
                return true ;

                case 'A'...'Z':
                return true ;

                case '0'...'9':  
                return true ;

            default:
                return false ;
            }

        case cg_letter:              // characters that start rule letter
            switch(ch)
            {
                case 'a'...'z':
                return true ;
                case 'A'...'Z':
                return true ;
            default:
                return false ;
            }

        case cg_alnum:               // characters that start rule alnum
            switch(ch)
            {
                case 'a'...'z':
                return true ;
                case 'A'...'Z':
                return true ;
                case '0' ... '9': 
                return true ;
            default:
                return false ;
            }
        case cg_integer:             // characters that start rule integer
            switch(ch)
            {
            case '0' ... '9':
            return true;

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
        case cg_op:                  // characters that start rule op
            switch(ch)
            {
                case '+' :
                return true ;
                case '-':
                return true ;
                case '*':
                return true ;
                case '/':
                return true ;
            default:
                return false ;
            }

        case cg_varop:               // characters that start rule varop
            switch(ch)
            {         
            case '<':    
            return true ;
            case '<'+'=':    
            return true ;     
            case '=':   
            return true ;     
            case '='+'=':   
            return true ;          
            case '!':   
            return true ;      
            case '!'+'=':    
            return true ;     
            case '>':   
            return true ;     
            case '>'+'=':   
            return true ;              
            default:
                return false ;
            }
        case cg_symbol:              // characters that start rule symbol
            switch(ch)
            {
                case '@': 
                return true ;
                case '{':
                return true ;
                case '}':
                return true ;
                case ':':
                return true ;
                case ';':
                return true ;
                case '.':
                return true ;
                case ',':
                return true ;
                case '"':
                return true ;
            default:
                return false ;
            }
        default:
            return false ;
        }
    }

    // work out the kind of a parsed token
    // this calls keyword_or_identifier(spelling) to check if an identifier is really a keyword
    // the spelling is a valid token
    // this is only called by new_token()
    TokenKind classify_spelling(string spelling)
    {
        if (spelling == ""){
            return tk_eoi;
        }

        switch (spelling[0])
        {
        case '\n':
        return tk_newline;
        case '\t':
        return tk_tab;
        case '\r':
        return tk_carriage_return;
        case ' ':
        return tk_space;

        case 'a' ... 'z':
        return keyword_or_identifier(spelling);
        case 'A' ... 'Z':
        return keyword_or_identifier(spelling);
        
        case '0' ... '9': 
        return tk_integer;

        case '+':
        return tk_add;

        case '-':
        return tk_sub;

        case '*':
        return tk_times;

        case '/':
        return tk_divide;

        case '@':
        return tk_at;

        case '{':
        return tk_lcb;

        case '}':
        return tk_rcb;

        case ':':
        return tk_colon;

        case ';':
        return tk_semi;

        case '.':
        return tk_dot;

        case ',':
        return tk_comma;

        case '"':
        return tk_dquote;

        case '<':
        return tk_lt;

        case '=':
        return tk_assign;

        case '!':
        return tk_gt;

        default:
            return tk_oops;
        }
       }

    string correct_spelling(TokenKind kind,string spelling)
    {
        return spelling ;
    }
}
