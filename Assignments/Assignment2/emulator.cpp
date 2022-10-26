// a1810750
// emulate executing Hack CPU instructions
#include "iobuffer.h"
#include "symbols.h"
#include "hack_computer.h"
#include <bitset>
#include <vector>
#include <iostream>

// to make out programs a bit neater
using namespace std ;

using namespace CS_IO_Buffers;
using namespace CS_Symbol_Tables;
using namespace Hack_Computer;

// This program must emulate the execution of Hack CPU instructions
//
// To read from registers A, D or PC, use read_A(), read_D() or read_PC()
// To write to registers A, D or PC, use write_A(value), write_D(value) or write_PC(value)
//
// To read from ROM use read_ROM(address)
// To read from RAM use read_RAM(address)
// To write to RAM use write_RAM(address,value)
//
// All values returned by a read_*() function are 16-bit unsigned integers
// All parameters passed to a write_*() function are 16-bit unsigned integers
//

/*****************   REPLACE THE FOLLOWING CODE  ******************/


// disassemble an instruction - convert binary to symbolic form
// A instructions should be "@" followed by the value in decimal
// C instructions should be dest=alu;jump
// omit dest= if all destination bits are 0
// omit ;jump if all jump bits are 0
string disassemble_instruction(uint16_t instruction)
{
    //A - instruction
    if  ((instruction >> 15)  == 0)
    {
        string instrString = to_string(instruction);
        return "@" + instrString;
    }

    //c - instruction
    else if ((instruction >> 15) && (instruction >> 14) && (instruction >> 13))
    {
        string dest = destination((instruction >> 3) & 7);
        string jmp = jump((instruction >> 0) & 7);
        string compute = aluop((instruction >> 6) & 127);
        string combine =  dest  + compute + jmp;
        return combine;
    }

    else {
        return "** illegal instruction **" ;
    }
}

// emulate a cpu instruction - the Hack Computer has been initialised
// the PC contains the address in ROM of the instruction to execute
// if you want to produce additional output, use write_to_traces()
static void emulate_instruction()
{
    uint16_t pc = read_PC();
    uint16_t instr = read_ROM(pc);

    //A - instruction
    if ((instr >> 15)  == 0)
    {
        write_A(instr);
        write_PC(pc+1);
    }

    //C - instruction
    else if (instr & (7 << 13))
    {
        uint16_t y, y1, y2, x1, x2, x, fout, out, readA;
        int zx, nx, zy, ny, f, no, a, destination1, destination2, destination3, jmp1, jmp2, jmp3, ng;

        //get alu instructions from instruction
        a = ((instr & (1 << 12)) >> 12);
        zx = ((instr & (1 << 11)) >> 11);
        nx = ((instr & (1 << 10)) >> 10);
        zy = ((instr & (1 << 9)) >> 9);
        ny = ((instr & (1 << 8)) >> 8);
        f = ((instr & (1 << 7)) >> 7);
        no = ((instr & (1 << 6)) >> 6);
       
        x = read_D();
        readA = read_A();
        
        destination1 = ((instr & (1 << 5)) >> 5);
        destination2 = ((instr & (1 << 4)) >> 4);
        destination3 = ((instr & (1 << 3)) >> 3);

        jmp1 = ((instr & (1 << 2)) >> 2);
        jmp2 = ((instr & (1 << 1)) >> 1);
        jmp3 = ((instr & (1 << 0)) >> 0);

      
        if (a == 1) 
        {
            y = read_RAM(read_A());
        }

        else if (a == 0)
        {
            y = read_A();
        }

        if (zx == 0)
        {
             x1 = x;
        }

        else
        {
            x1 = 0;
        }

        if (nx == 0)
        {
             x2 = x1;
        }

        else
        {
            x2 = ~x1;
        }

        if (zy == 0)
        {
             y1 = y;
        }

        else
        {
            y1 = 0;
        }

        if (ny == 0)
        {
             y2 = y1;
        }

        else
        {
            y2 = ~y1;
        }

        if (f == 0)
        {
             fout = x2 & y2;
        }

        else
        {
             fout = x2 + y2;
        }

        if (no == 0)
        {
             out = fout;
        }

        else
        {
            out = ~fout;
        }

        if (destination1 == 1)
        {
            write_A(out);
        }

        if (destination2 == 1)
        {
            write_D(out);
        }

        if (destination3 == 1)
        {
            write_RAM(read_A(), out);
        }

        ng = ((out & (1 << 15)) >> 15);

        if (jmp1 == 0 && jmp2 == 0 && jmp3 == 0)
        {
            write_PC(pc+1);
        }

        else if (jmp1 == 0 && jmp2 == 0 && jmp3 == 1)
        {
            if (ng == 0 && out != 0)
            {
                write_PC(readA);
            }

            else
            {
                write_PC(pc+1);
            }
        }

        else if (jmp1 == 0 && jmp2 == 1 && jmp3 == 0)
        {
            if (out == 0)
            {
                write_PC(readA);
            }

            else
            {
                write_PC(pc+1);
            }
        }

         else if (jmp1 == 0 && jmp2 == 1 && jmp3 == 1)
         {

            if (ng == 0)
            {
                write_PC(readA);
            }

            else
            {
                write_PC(pc+1);
            }
        }

        else if (jmp1 == 1 && jmp2 == 0 && jmp3 == 0)
        {
            if (ng != 0)
            {
                write_PC(readA);
            }

            else
            {
                write_PC(pc+1);
            }
        }

        else if (jmp1 == 1 && jmp2 == 0 && jmp3 == 1)
        {
            if ( out != 0)
            {
                write_PC(readA);
            }

            else
            {
                write_PC(pc+1);
            }
        }

        else if (jmp1 == 1 && jmp2 == 1 && jmp3 == 0)
        {
            if (ng != 0 || out == 0)
            {
                write_PC(readA);
            }

            else
            {
                write_PC(pc+1);
            }
        }

        else if (jmp1 == 1 && jmp2 == 1 && jmp3 == 1)
        {
            write_PC(readA);
        }
    }

    else {

    }
}

/*****************        DOWN TO HERE         ******************/

// if the first command line argument is "D" the test output must display disassembled instructions
bool display_disassembled_instructions = false ;

// this function only calls disassemble_instruction if required
// it is called from more_cpu_tests() and is passed the instruction being emulated
string disassemble(uint16_t instruction)
{
    if ( display_disassembled_instructions ) return disassemble_instruction(instruction) ;

    // default is to display instruction in binary
    return "\"" + std::bitset<16>(instruction).to_string() + "\"" ;
}


// main program
int main(int argc,char **argv)
{
    // force all output to be immediate - the test scripts will override this
    config_output(iob_immediate) ;
    config_errors(iob_immediate) ;

    // is the first command line argument "D", ie does the test output include disassembly of the instruction being emulated
    display_disassembled_instructions = argc > 1 && string(argv[1]) == "D" ;

    // more_cpu_tests() will initialise the next test and return true
    // if there are no more tests to be run, it returns false
    // more_cpu_tests() also outputs the results of the previous test
    while ( more_cpu_tests(disassemble) )
    {
        emulate_instruction() ;
    }

    // flush the output and any errors
    print_output() ;
    print_errors() ;
}

