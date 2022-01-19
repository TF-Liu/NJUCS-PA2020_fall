#include "cpu/instr.h"
/*
Put the implementations of `ret' instructions here.
*/

make_instr_func(ret_near)
{
    print_asm_0("ret", "", 0);
    
    // pop eip
    // 实现时灵活指定指令长度为0，来规避cpu.eip += instr_len
    cpu.eip = vaddr_read(cpu.esp, SREG_CS, 4);
    cpu.esp = cpu.esp + 4;

    return 0;
}

make_instr_func(ret_near_imm16)
{
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = 16;
    rel.addr = eip + 1;
    operand_read(&rel);
    
    print_asm_1("ret", "", 2, &rel);
    
    // pop eip
    cpu.eip = vaddr_read(cpu.esp, SREG_CS, 4);
    cpu.esp = cpu.esp + 4;
    
    //esp += imm16
    cpu.esp += rel.val;
    
    return 0;
}
