#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/

static void instr_execute_1op() 
{
    OPERAND temp;
    temp.data_size = data_size;
    temp.sreg = SREG_DS;
    temp.type = OPR_MEM;
    operand_read(&opr_src);
	cpu.esp -= data_size / 8;
    temp.addr = cpu.esp;
	temp.val = sign_ext(opr_src.val, opr_src.data_size);
	operand_write(&temp);
}

make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, i, v)
make_instr_impl_1op(push, i, b)
make_instr_impl_1op(push, rm, v)

make_instr_func(pusha){
	print_asm_0("pusha", "", 1);

	opr_dest.data_size = 32;
	opr_dest.type = OPR_MEM;
	opr_dest.sreg = SREG_DS;

	cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.eax;
	operand_write(&opr_dest);

    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.ecx;
	operand_write(&opr_dest);

    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.edx;
	operand_write(&opr_dest);

    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.ebx;
	operand_write(&opr_dest);

    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.esp;
	operand_write(&opr_dest);

    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.ebp;
	operand_write(&opr_dest);

    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.esi;
	operand_write(&opr_dest);

    cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;
	opr_dest.val = cpu.edi;
	operand_write(&opr_dest);

    return 1;
}
