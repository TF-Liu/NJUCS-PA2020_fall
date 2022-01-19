#include "cpu/instr.h"

make_instr_func(jmp_near)
{
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;

    operand_read(&rel);

    int offset = sign_ext(rel.val, data_size);
    // thank Ting Xu from CS'17 for finding this bug
    print_asm_1("jmp", "", 1 + data_size / 8, &rel);

    cpu.eip += offset;

    return 1 + data_size / 8;
}

make_instr_func(jmp_far_imm)
{
    OPERAND rel, cs;
    
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = data_size;
    rel.addr = eip + 1;
    operand_read(&rel);
    
    cs.type = OPR_IMM;
    cs.sreg = SREG_CS;
    cs.data_size = 16;
    cs.addr = eip + 1 + data_size / 8;
    operand_read(&cs);
    
    print_asm_1("jmp", "", 1 + data_size / 8 + 2, &rel);
    
    cpu.cs.val = cs.val;
    load_sreg(cs.sreg);
    cpu.eip = sign_ext(rel.val, data_size);

    return 0;
}

make_instr_func(jmp_short)
{
    OPERAND rel;
    rel.type = OPR_IMM;
    rel.sreg = SREG_CS;
    rel.data_size = 8;
    rel.addr = eip + 1;

    operand_read(&rel);

    int offset = sign_ext(rel.val, rel.data_size);
    print_asm_1("jmp", "", 1 + data_size / 8, &rel);

    cpu.eip += offset;

    return 1 + rel.data_size / 8;
}

make_instr_func(jmp_near_indirect)
{
    OPERAND rel;
    int len = 1;
    rel.data_size = data_size;
    len += modrm_rm(eip + 1, &rel);
    operand_read(&rel);

    print_asm_1("jmp", "", len, &rel);

    cpu.eip = rel.val & (0xFFFFFFFF >> (32 - data_size));

    return 0;
}