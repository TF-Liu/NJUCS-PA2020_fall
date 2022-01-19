#include "cpu/instr.h"
/*
Put the implementations of `iret' instructions here.
*/

static uint32_t pop()
{
    OPERAND temp;
    temp.data_size = data_size;
    temp.type = OPR_MEM;
    temp.sreg = SREG_DS;
    temp.addr = cpu.esp;
    operand_read(&temp);
	cpu.esp += data_size / 8;
    return temp.val;
}

make_instr_func(iret)
{
    cpu.eip = pop();
    cpu.cs.val = pop();
    cpu.eflags.val = pop();
    print_asm_0("iret","",1);
    return 0;
}
