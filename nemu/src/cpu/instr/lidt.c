#include "cpu/instr.h"
/*
Put the implementations of `lidt' instructions here.
*/

make_instr_func(lidt) 
{
    int len = 1;
    
    OPERAND mem, limit, base;
    
    mem.type = OPR_MEM;
    mem.data_size = data_size;
    len += modrm_rm(eip + 1, &mem);
    
    limit.data_size = 16;
    limit.type = OPR_MEM;
    limit.addr = mem.addr;
    limit.sreg = SREG_CS;
    operand_read(&limit);
    
    base.data_size = (data_size == 32) ? 32 : 24;
    base.type = OPR_MEM;
    base.addr = mem.addr + limit.data_size / 8;
    base.sreg = SREG_CS;
    operand_read(&base);
    
    cpu.idtr.base = base.val;
    cpu.idtr.limit = limit.val;
    
    print_asm_1("lidt", "", 1 + data_size / 8, &mem);
    
    return len;
}
