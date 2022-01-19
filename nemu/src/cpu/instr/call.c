#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/

make_instr_func(call_near)
{
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;

    operand_read(&rel);
    
    // push eip(这个eip已经指向下一个指令，ret时只需要pop即可)
    cpu.esp = cpu.esp - 4;
    vaddr_write(cpu.esp, SREG_CS, 4, cpu.eip + 1 + data_size / 8);
    
    print_asm_1("call", "", 1 + data_size / 8, &rel);
    
    // eip = eip + rel
    int offset = sign_ext(rel.val, data_size);
    cpu.eip += offset;

    return 1 + data_size / 8;
}

make_instr_func(call_near_indirect)
{
    OPERAND rel;
    int len = 1;
    rel.data_size = data_size;
    len += modrm_rm(eip + 1, &rel);
    operand_read(&rel);
    
    // push eip(这个eip已经指向下一个指令，ret时只需要pop即可)
    cpu.esp = cpu.esp - 4;
    vaddr_write(cpu.esp, SREG_CS, 4, cpu.eip + len);
    
    print_asm_1("call", "", 1 + data_size / 8, &rel);
    
    // cpu.eip =  [r/m32]
    cpu.eip = rel.val;

    return 0;
}
