// a1810750
// tokeniser implementation for the workshop example language

#include "iobuffer.h"
#include "tokeniser-extras.h"

// to shorten the code
using namespace std ;
using namespace CS_IO_Buffers ;

// we are extending the Assignment_Tokeniser namespace

namespace Assignment_Tokeniser
{
    static void parse_identifier()
    {
        next_char_mustbe(cg_identifier);
        if (next_char_isa('$')) 
            next_char_mustbe(cg_letter);
        else 
            while(next_char_isa(cg_letter)) 
            read_next_char() ; 
    }

    static void parse_scientific()
    {

        // if (next_char_isa(cg_integer)){
        //     next_char_mustbe('.');
            while(next_char_isa(cg_digit))
                read_next_char() ; 
                if (next_char_isa(cg_exponent)){
                    next_char_mustbe(cg_eee) ; 
                    if (next_char_isa(cg_sign))
                        read_next_char() ; 
                    else if(next_char_isa(cg_integer))
                        read_next_char() ; 
                }
        // }
    }   

    // static void parse_exponent()
    // {

    // }

    static void parse_integer()
    {

        if (next_char_isa('0')){
             read_next_char() ;
         }
        else if(next_char_isa(cg_digit19)){
            read_next_char() ; 
            while (next_char_isa(cg_digit))
                read_next_char() ; 
        }
        else if (next_char_isa(cg_digit)){
                read_next_char() ; 
            }
        else if (next_char_isa('.')){
            parse_scientific(); 
        }

        else
            did_not_find_char(cg_integer);
    }
 
    static void parse_number(){

        if(next_char_isa(cg_integer)){
            parse_integer();
            if(next_char_isa('.')){
                read_next_char() ;
                parse_scientific();
            }
        }
        else{
            did_not_find_char(cg_number);
        }

}

    // static void parse_symbol(){


    //     if (next_char_isa('*')){
    //             read_next_char() ;
    //     } 

    //     else if (next_char_isa('~')){
    //             read_next_char() ;
            
    //     }

    //     else if (next_char_isa('<')){
    //         read_next_char();
    //         if (next_char_isa('<')){
    //             read_next_char();
    //             if (next_char_isa('<')){
    //               read_next_char() ;
    //             }
    //         }
    //     }

    //     else if (next_char_isa('>')){
    //         read_next_char();
    //         if (next_char_isa('>')){
    //             read_next_char();
    //             if (next_char_isa('>')){
    //               read_next_char() ;
    //             }
    //         }
    //     }
        
    //     else if (next_char_isa('=')){
    //         if (next_char_isa('=')){
    //         next_char_mustbe('=');
    //     }
    //     else
    //         read_next_char();
        
            
    //     }

    //     if(next_char_isa(cg_symbol)){
    //         read_next_char() ;
    //         } 

    // } 

    // static void parse_string(){

    // next_char_mustbe('"');
    // next_char_isa(cg_instring);
    // next_char_mustbe('"');

    // } 

    // token ::= ...
    static void parse_token()
    {
        if ( next_char_isa(EOF) ) ; 
        // else if (next_char_isa(cg_telegu) ) 
        //     parse_string(); 
        else if ( next_char_isa(cg_wspace) )
            read_next_char(); 

        else if ( next_char_isa(cg_symbol) ) 
            read_next_char();
            // parse_symbol();
        else if ( next_char_isa(cg_identifier) )
            parse_identifier(); 
        else if ( next_char_isa(cg_number) ) 
            parse_number(); 


        else
        did_not_find_start_of_token()  ;
    }







    // parse the next token in the input and return a new
    // Token object that describes its kind and spelling
    // Note: you must not call new_token() anywhere else in your program
    // Note: you should not modify this function
    //

    Token read_next_token()
    {
        parse_token() ;

        return new_token() ;
    }
}

