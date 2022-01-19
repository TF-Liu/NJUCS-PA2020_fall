#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/

static void instr_execute_1op() 
{
    OPERAND temp;
    temp.data_size = data_size;
    temp.type = OPR_MEM;
    temp.sreg = SREG_DS;
    temp.addr = cpu.esp;
    operand_read(&temp);
    opr_src.val = temp.val;
	operand_write(&opr_src);
	cpu.esp += data_size / 8;
}

make_instr_impl_1op(pop, r, v)

make_instr_func(popa) {
	print_asm_0("popa", "", 1);

	opr_src.data_size = 32;
	opr_src.type = OPR_MEM;
	opr_src.sreg = SREG_DS;

	opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.edi = opr_src.val;
	cpu.esp += data_size / 8;

	opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.esi = opr_src.val;
	cpu.esp += data_size / 8;

	opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.ebp = opr_src.val;
	cpu.esp += data_size / 8;

	opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.esp = opr_src.val;
	cpu.esp += data_size / 8;

	opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.ebx = opr_src.val;
	cpu.esp += data_size / 8;

	opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.edx = opr_src.val;
	cpu.esp += data_size / 8;

	opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.ecx = opr_src.val;
	cpu.esp += data_size / 8;

	opr_src.addr = cpu.esp;
	operand_read(&opr_src);
	cpu.eax = opr_src.val;
	cpu.esp += data_size / 8;

    return 1;
}
