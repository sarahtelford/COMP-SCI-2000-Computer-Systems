//
//
// Author: a1810750
// Name:   Sarah Telford
//

#include "iobuffer.h"
#include "symbols.h"
#include "abstract-syntax-tree.h"

// to shorten our code:
using namespace std ;
using namespace CS_IO_Buffers ;
using namespace Jack_Compiler ;
    
#pragma clang diagnostic ignored "-Wunused-function"

// ***** WHAT TO DO *****
//
// MODIFY the skeleton code below to walk an abstract syntax tree, ast, of a Jack class
//        and pretty print the equivalent Jack source code.
//
// NOTE: the correct segment and offset is recorded with every variable in the ast
//       so the code generator does not need to use any symbol tables
//
// The skeleton code has one function per node in the abstract tree
//  - they each extract all fields into local variables
//  - they each call the appropriate print_* function to walk any sub-trees
//
// The structure of the skeleton code is just a suggestion
//  - you may want to change the parameters / results to suit your own logic
//  - you can change it as much as you like

// forward declarations of one function per node in the abstract syntax tree
static void print_class(ast t) ;
static void print_class_var_decs(ast t) ;
static void print_var_dec(ast t) ;
static void print_subr_decs(ast t) ;
static void print_subr(ast t) ;
static void print_constructor(ast t) ;
static void print_function(ast t) ;
static void print_method(ast t) ;
static void print_param_list(ast t) ;
static void print_subr_body(ast t) ;
static void print_var_decs(ast t) ;
static void print_statements(ast t) ;
static void print_statement(ast t) ;
static void print_let(ast t) ;
static void print_let_array(ast t) ;
static void print_if(ast t) ;
static void print_if_else(ast t) ;
static void print_while(ast t) ;
static void print_do(ast t) ;
static void print_return(ast t) ;
static void print_return_expr(ast t) ;
static void print_expr(ast t) ;
static void print_term(ast t) ;
static void print_int(ast t) ;
static void print_string(ast t) ;
static void print_bool(ast t) ;
static void print_null(ast t) ;
static void print_this(ast t) ;
static void print_unary_op(ast t) ;
static void print_var(ast t) ;
static void print_array_index(ast t) ;
static void print_call_as_function(ast t) ;
static void print_call_as_method(ast t) ;
static void print_subr_call(ast t) ;
static void print_expr_list(ast t) ;
static void print_infix_op(ast t) ;

// walk an ast class node with fields:
// class_name - a string
// statics    - ast vector of variable declarations
// fields     - ast vector of variable declarations
// subr_decs  - ast vector of subroutine declarations
//

static int indent = 0;             //setiing indent value
static int ifcounter = 0;          //create if counter to track brackets outputted 
static bool callstatic = 1;         //create bool that checks if static has been called  
static bool callprivate = 1;        //create bool that checks if private has been called  
static bool callfirst = 1;          //create bool that checks if it is the first call  

void indentTwo()            // creating indentation function
{
    for(int i = 0; i < indent; i++)
    {
        write_to_output("  ");
    }
}

static void print_class(ast t) // creating class formatting 
{
    string myclassname = get_class_class_name(t) ;          //initilise varaibles 
    ast statics = get_class_statics(t) ;
    ast fields = get_class_fields(t) ;
    ast subr_decs = get_class_subr_decs(t) ;

    string output = "class " + myclassname + "\n{";
    write_to_output(output);
    indent++;
    print_class_var_decs(statics);
    callstatic = 0;
    print_class_var_decs(fields);
    output = "\n// public:\n";
    write_to_output(output);
    print_subr_decs(subr_decs);
    output = "}\n";
    write_to_output(output);
    indent--;
}

// walk an ast class var decs node
// it is an ast vector of variable declarations
//
static void print_class_var_decs(ast t)
{
    int ndecs = size_of_class_var_decs(t);          //initilise varaibles 
    string output;

    if (ndecs != 0 && callprivate == 1)
    {
        output = "\n// private:\n";
        write_to_output(output);
        callprivate = 0;
    }
    if (ndecs != 0 && callstatic == 0)
    {
        output = "\n";
        write_to_output(output);
    }
    for (int i = 0; i < ndecs; i++)
    {
        indentTwo();
        print_var_dec(get_class_var_decs(t, i));
        output = " ;";
        write_to_output(output);
        output = "\n";
        write_to_output(output);
    }
}

// walk an ast variable declaration with fields
// name - a string
// type - a string - "int", "char", "boolean" or a class name
// segment - a string
// offset - an int
// this is used for statics, fields, parameters and local variables
//
static void print_var_dec(ast t) // creating variable declaration with fields formatting
{
    string name = get_var_dec_name(t) ;                           //initilise variables
    string type = get_var_dec_type(t) ;
    string segment = get_var_dec_segment(t) ;
    int offset = get_var_dec_offset(t) ;
    string output;

    if (segment == "this")
    {
        segment = "field";
    }
    if (segment == "local")
    {
        segment = "var";
    }
    if (segment == "argument")
    {
        output = type + " " + name;
        write_to_output(output);
    }
    else{
        output = segment + " " + type + " " + name;
        write_to_output(output);
    }
}
// walk an ast class var decs node
// it is an ast vector of subroutine declarations
//
static void print_subr_decs(ast t)
{
    int size = size_of_subr_decs(t) ;
    for ( int i = 0 ; i < size ; i++ )
    {
        print_subr(get_subr_decs(t,i)) ;
    }
}

// walk an ast subroutine node with a single field
// subr - an ast constructor, ast function or ast method node
//
static void print_subr(ast t)
{
    ast subr = get_subr_subr(t) ;                   //initilise varaibles 

    switch(ast_node_kind(subr))
    {
    case ast_constructor:
        print_constructor(subr) ;
        break ;
    case ast_function:
        print_function(subr) ;
        break ;
    case ast_method:
        print_method(subr) ;
        break ;
    default:
        fatal_error(0,"Unexpected subroutine kind") ;
        break ;
    }
}

// walk an ast constructor node with fields
// vtype - a string - the name of the class
// name - a string
// param list - an ast vector of variable declarations
// subr body - an ast subr body node
//
static void print_constructor(ast t)
{
    string vtype = get_constructor_vtype(t) ;                   //initilise varaibles 
    string name = get_constructor_name(t) ;
    ast param_list = get_constructor_param_list(t) ;
    ast subr_body = get_constructor_subr_body(t) ;

    if (callfirst == 0)
    {
        write_to_output("\n");
    }

    callfirst = 0;
    indentTwo();
    string output = "constructor " + vtype + " " + name + "(";
    write_to_output(output);
    print_param_list(param_list) ;
    output = ")\n";
    write_to_output(output);
    indentTwo();
    output = "{\n";
    write_to_output(output);
    indent++;
    print_subr_body(subr_body) ;
    indent--;
    indentTwo();
    output = "}\n";
    write_to_output(output);
}

// walk an ast function node with fields
// vtype - a string - one of "void", "int", "char", "boolean" or the name of a class
// name - a string
// param list - an ast vector of variable declarations
// subr body - an ast subr body node
//
static void print_function(ast t)
{
    string vtype = get_function_vtype(t) ;                      //initilise varaibles 
    string name = get_function_name(t) ;
    ast param_list = get_function_param_list(t) ;
    ast subr_body = get_function_subr_body(t) ;

    if (callfirst == 0)
    {
        write_to_output("\n");
    }
    callfirst = 0;
    indentTwo();
    string output = "function " + vtype + " " + name + "(";
    write_to_output(output);
    print_param_list(param_list) ;
    output = ")\n";
    write_to_output(output);
    indentTwo();
    output = "{\n";
    write_to_output(output);
    indent++;
    print_subr_body(subr_body) ;
    indent--;
    indentTwo();
    output = "}\n";
    write_to_output(output);
}

// walk an ast method node with fields
// vtype - a string - one of "void", "int", "char", "boolean" or the name of a class
// name - a string
// param list - an ast vector of variable declarations
// subr body - an ast subr body node
//
static void print_method(ast t)
{
    string vtype = get_method_vtype(t) ;                    //initilise varaibles 
    string name = get_method_name(t) ;
    ast param_list = get_method_param_list(t) ;
    ast subr_body = get_method_subr_body(t) ;

   if (callfirst == 0)
    {
        write_to_output("\n");
    }

    callfirst = 0;
    indentTwo();
    string output = "method " + vtype + " " + name + "(";
    write_to_output(output);
    print_param_list(param_list) ;
    output = ")\n";
    write_to_output(output);
    indentTwo();
    output = "{\n";
    write_to_output(output);
    indent++;
    print_subr_body(subr_body) ;
    indent--;
    indentTwo();
    output = "}\n";
    write_to_output(output);
}

// walk an ast param list node
// it is an ast vector of variable declarations
//
static void print_param_list(ast t)
{
    int ndecs = size_of_param_list(t) ;                     //initilise varaibles 
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        print_var_dec(get_param_list(t,i)) ;
        if(i < ndecs - 1)
        {
            string output = ",";
            write_to_output(output);
        }
    }
}

// walk an ast subr body node with fields
// decs - an ast vector of variable declarations
// body - an ast vector of statement nodes
//
static void print_subr_body(ast t)
{
    ast decs = get_subr_body_decs(t) ;                      //initilise varaibles 
    ast body = get_subr_body_body(t) ;

    print_var_decs(decs) ;
    print_statements(body) ;
}

// walk an ast param list node
// it is an ast vector of variable declarations
//
static void print_var_decs(ast t)
{
    int ndecs = size_of_var_decs(t) ;
    string output;
    for ( int i = 0 ; i < ndecs ; i++ )
    {
        indentTwo();
        print_var_dec(get_var_decs(t,i)) ;
        output = " ;\n";
        write_to_output(output);
    }
    if (ndecs != 0){
        output = "\n";
        write_to_output(output);
    }
}

// walk an ast statements node
// it is an ast vector of statement nodes
//
static void print_statements(ast t)
{
    int nstatements = size_of_statements(t) ;           //initilise varaibles 
    for ( int i = 0 ; i < nstatements ; i++ )
    {
        print_statement(get_statements(t,i)) ;
    }
}

// walk an ast statement node with a single field
// statement - one of the following ast nodes, let, let array, if, if else, while, do, return, return expr or statements
//
static void print_statement(ast t)
{
    ast statement = get_statement_statement(t) ;        //initilise varaibles 

    switch(ast_node_kind(statement))
    {
    case ast_let:
        print_let(statement) ;
        break ;
    case ast_let_array:
        print_let_array(statement) ;
        break ;
    case ast_if:
        ifcounter++;
        print_if(statement) ;
        ifcounter--;
        break ;
    case ast_if_else:
        ifcounter++;
        print_if_else(statement) ;
        ifcounter--;
        break ;
    case ast_while:
        ifcounter++;
        print_while(statement) ;
        ifcounter--;
        break ;
    case ast_do:
        print_do(statement) ;
        break ;
    case ast_return:
        print_return(statement) ;
        break ;
    case ast_return_expr:
        print_return_expr(statement) ;
        break ;
    case ast_statements:
        print_statements(statement) ;
        break ;
    default:
        fatal_error(0,"Unexpected statement kind") ;
        break ;
    }
}

// walk an ast let node with fields
// var  - an ast variable
// expr - an ast expr node
//
static void print_let(ast t)
{
    ast var = get_let_var(t) ;              //initilise varaibles 
    ast expr = get_let_expr(t) ;

    indentTwo();
    string output = "let ";
    write_to_output(output);
    print_var(var) ;
    output = " = ";
    write_to_output(output);
    print_expr(expr) ;
    output = " ;\n";
    write_to_output(output);
}

// walk an ast let array node with fields
// var    - an ast variable
// index  - an ast expr node
// expr   - an ast expr node
//
static void print_let_array(ast t)
{
    ast var = get_let_array_var(t) ;                //initilise varaibles 
    ast index = get_let_array_index(t) ;
    ast expr = get_let_array_expr(t) ;

    indentTwo();
    string output = "let ";
    write_to_output(output);
    print_var(var) ;
    output = "[";
    write_to_output(output);
    print_expr(index);
    output = "]";
    write_to_output(output);
    output = " = ";
    write_to_output(output);
    print_expr(expr) ;
    output = " ;\n";
    write_to_output(output);
}

// walk an ast if node with fields
// condition - an ast expr node
// if true   - an ast statements node
//
static void print_if(ast t)
{
    ast condition = get_if_condition(t) ;               //initilise varaibles 
    ast if_true = get_if_if_true(t) ;

    indentTwo();
    string output = "if (";
    write_to_output(output);
    print_expr(condition) ;
    output = ")\n";
    write_to_output(output);
    indentTwo();
    output = "{\n";
    write_to_output(output);
    indent++;
    print_statements(if_true) ;
    indent--;
    indentTwo();
    output = "}\n";
    write_to_output(output);

    if (ifcounter == 1){
        output = "\n";
        write_to_output(output);
    }
}

// walk an ast if else node with fields
// condition - an ast expr node
// if true   - an ast statements node
// if else   - an ast statements node
//
static void print_if_else(ast t)
{
    ast condition = get_if_else_condition(t) ;                      //initilise varaibles 
    ast if_true = get_if_else_if_true(t) ;
    ast if_false = get_if_else_if_false(t) ;

    indentTwo();
    string output = "if (";
    write_to_output(output);
    print_expr(condition) ;
    output = ")\n";
    write_to_output(output);
    indentTwo();
    output = "{\n";
    write_to_output(output);
    indent++;
    print_statements(if_true) ;
    indent--;
    indentTwo();
    output = "}\n";
    write_to_output(output);
    indentTwo();
    output = "else\n";
    write_to_output(output);
    indentTwo();
    output = "{\n";
    write_to_output(output);
    indent++;
    print_statements(if_false) ;
    indent--;
    indentTwo();
    output = "}\n";
    write_to_output(output);

    if (ifcounter == 1){
        output = "\n";
    }
}

// walk an ast while node with fields
// condition - an ast expr node
// body      - an ast statements node
//
static void print_while(ast t)
{
    ast condition = get_while_condition(t) ;                    //initilise varaibles 
    ast body = get_while_body(t) ;

    indentTwo();
    string output = "while (";
    write_to_output(output);
    print_expr(condition) ;
    output = ")\n";
    write_to_output(output);
    indentTwo();
    output = "{\n";
    write_to_output(output);
    indent++;
    print_statements(body) ;
    indent--;
    indentTwo();
    output = "}\n";
    write_to_output(output);

    if (ifcounter == 1){
        output = "\n";
        write_to_output(output);
    }
}

// walk an ast do node with a single field
// call - an ast call as function node or an ast call as method node
//
static void print_do(ast t)
{
    ast call = get_do_call(t) ;                         //initilise varaibles 
    string output;

    indentTwo();
    switch(ast_node_kind(call))
    {
    case ast_call_as_function:
        output = "do ";
        write_to_output(output);
        print_call_as_function(call) ;
        output = " ;\n";
        write_to_output(output);
        break ;
    case ast_call_as_method:
        output = "do ";
        write_to_output(output);
        print_call_as_method(call) ;
        output = " ;\n";
        write_to_output(output);
        break ;
    default:
        fatal_error(0,"Unexpected call kind") ;
        break ;
    }
}

// walk an ast return node, it has not fields
//
static void print_return(ast t)
{
    indentTwo();
    string output = "return ;\n";                       //initilise varaibles 
    write_to_output(output);
}

// walk an ast return expr node with a single field
// expr - an ast expr node
//
static void print_return_expr(ast t)
{
    ast expr = get_return_expr(t) ;                 //initilise varaibles 
    indentTwo();
    string output = "return ";
    write_to_output(output);
    print_expr(expr) ;
    output = " ;\n";
    write_to_output(output);
}

// walk an ast param list node
// it is an ast vector of ast term and infix op nodes
// it must be of odd length > 0, ie 1, 3, 5, 7, ...
// all elements at even indices are an ast term node
// all elements at odd indices are an ast infix op
//
static void print_expr(ast t)
{
    int term_ops = size_of_expr(t) ;
    for ( int i = 0 ; i < term_ops ; i++ )
    {
        ast term_op = get_expr(t,i) ;
        if ( i % 2 == 0 )
        {
            print_term(term_op) ;
        }
        else
        {
            print_infix_op(term_op) ;
        }
    }
}

// walk an ast term node with a single field
// term - one of the following ast nodes:
//        int, string, bool, null, this, expr, unary op,
//        variable, .array index, call as function or call as method
//
static void print_term(ast t)
{
    ast term = get_term_term(t) ;

    switch(ast_node_kind(term))
    {
    case ast_int:
        print_int(term) ;
        break ;
    case ast_string:
        print_string(term) ;
        break ;
    case ast_bool:
        print_bool(term) ;
        break ;
    case ast_null:
        print_null(term) ;
        break ;
    case ast_this:
        print_this(term) ;
        break ;
    case ast_expr:
        print_expr(term) ;
        break ;
    case ast_unary_op:
        print_unary_op(term) ;
        break ;
    case ast_var:
        print_var(term) ;
        break ;
    case ast_array_index:
        print_array_index(term) ;
        break ;
    case ast_call_as_function:
        print_call_as_function(term) ;
        break ;
    case ast_call_as_method:
        print_call_as_method(term) ;
        break ;
    default:
        fatal_error(0,"Unexpected term kind") ;
        break ;
    }
}

// walk an ast int node with a single field
// constant - an integer in the range -32,768 to 32,767
//
static void print_int(ast t)
{
    int _constant = get_int_constant(t) ;
    string output;
    if (0 <= _constant && _constant <= 32767)
    {
        output = to_string(_constant);
        write_to_output(output);
    }
    else if (_constant >= -32767)
    {
        output = "(" + to_string(_constant) + ")";
        write_to_output(output);
    }
    else
    {
        output = "(16384 + 16384)";
        write_to_output(output);
    }
}

// walk an ast string node with a single field
// constant - a string
//
static void print_string(ast t)
{
    string _constant = get_string_constant(t) ;                     //initilise varaibles 
    string output = " " + _constant + " ";
    write_to_output(output);
}

// walk an ast bool node with a single field
// constant - either true or false
//
static void print_bool(ast t)
{
    bool _constant = get_bool_t_or_f(t) ;
    string output;

    if (_constant == true)                          //change output based on if the constant is true or false 
    {
        output = "true";
        write_to_output(output);
    }
    else
    {
        output = "false";
        write_to_output(output);
    }
}

// walk an ast null node, it has not fields
//
static void print_null(ast t)
{
    string output = "null";                                 //initilise varaibles 
    write_to_output(output);
}

// walk an ast this node, it has not fields
//
static void print_this(ast t)
{
    string output = "this";                             //initilise varaibles 
    write_to_output(output);
}

// walk an ast unary op node with fields
// op   - a string, either "-" or "~"
// term - an ast term node
//        int, string, bool, null, this, expr, unary op,
//        variable, array index, call as function or call as method
//
static void print_unary_op(ast t)
{
    string uop = get_unary_op_op(t);                    //initilise varaibles 
    ast term = get_unary_op_term(t) ;
    string output = uop;
    write_to_output(output);
    print_term(term) ;
}

// walk an ast variable node with fields
// name - a string
// type - a string - "int", "char", "boolean" or a class name
// segment - a string
// offset - an int
//
static void print_var(ast t)
{
    string name = get_var_name(t) ;                 //initilise varaibles 
    string output = name;
    write_to_output(output);
}

// walk an ast array index node with fields
// var   - an ast variable node
// index - an ast expr node
//
static void print_array_index(ast t)
{
    ast var = get_array_index_var(t) ;              //initilise varaibles 
    ast index = get_array_index_index(t) ;

    print_var(var) ;
    string output = "[";
    write_to_output(output);
    print_expr(index) ;
    output = "]";
    write_to_output(output);
}

// walk an ast subr call as method with fields
// class name - a string
// call       - an ast subr call node
//
static void print_call_as_function(ast t)
{
    string class_name = get_call_as_function_class_name(t) ;                //initilise varaibles 
    ast subr_call = get_call_as_function_subr_call(t) ;

    string output = class_name + "::";
    write_to_output(output);
    print_subr_call(subr_call) ;
}

// walk an ast subr call as method with fields
// class name - a string
// var        - an ast variable or ast this node, hidden first parameter of the call
// call       - an ast subr call node
//
static void print_call_as_method(ast t)
{
    string class_name = get_call_as_method_class_name(t) ;                  //initilise varaibles 
    ast var = get_call_as_method_var(t) ;
    ast subr_call = get_call_as_method_subr_call(t) ;
    string output;

    switch(ast_node_kind(var))
    {
    case ast_this:
        print_this(var) ;
        output = ".";
        write_to_output(output);
        break ;
    case ast_var:
        print_var(var) ;
        output = ".";
        write_to_output(output);
        break ;
    default:
        fatal_error(0,"Expected var or this") ;
        break ;
    }
    print_subr_call(subr_call) ;
}

// walk an ast subr call node with fields
// name      - a string
// expr list - a vector of ast expr nodes
//
static void print_subr_call(ast t)
{
    string subr_name = get_subr_call_subr_name(t) ;                     //initilise varaibles 
    ast expr_list = get_subr_call_expr_list(t) ; 

    string output = subr_name + "(";
    write_to_output(output);
    print_expr_list(expr_list);
    output = ")";
    write_to_output(output);
}

// walk an ast expr list node
// it is an ast vector of ast expr nodes
//
static void print_expr_list(ast t)
{
    int nexpressions = size_of_expr_list(t) ;                       //initilise varaibles 

    for ( int i = 0 ; i < nexpressions ; i++ )
    {
        print_expr(get_expr_list(t,i)) ;
        if (i < nexpressions-1)
        {
            string output = ",";
            write_to_output(output);
        }
    }
}

// walk an ast infix op node with a single field
// op - a string - one of "+", "-", "*", "/", "&", "|", "<", ">", "="
//
static void print_infix_op(ast t)
{
    string op = get_infix_op_op(t) ;                        //initilise varaibles 
    string output = " " + op;
    write_to_output(output);

    if (op == "<" || op == ">" || op == "="){
        output = "= ";
        write_to_output(output);
    }
    else {
        output = " ";
        write_to_output(output) ;
    }
}

// main program
int main(int argc,char **argv)
{
    // walk an AST parsed from XML and pretty print equivalent Jack code
    print_class(ast_parse_xml()) ;

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}
