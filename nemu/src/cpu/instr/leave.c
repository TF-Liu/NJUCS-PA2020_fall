#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/

make_instr_func(leave)
{
    print_asm_0("leave", "", 1);
    
    // mov ebp, esp
    cpu.esp = cpu.ebp;
    
    //pop ebp
    cpu.ebp = vaddr_read(cpu.esp, SREG_CS, 4);
    cpu.esp += 4;
    
    return 1;
}
