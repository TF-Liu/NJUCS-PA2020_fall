#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `out' instructions here.
*/

make_instr_func(out_b)
{
    int len = 1;
    OPERAND DX, AX;

    DX.addr = REG_DX;
    DX.type = OPR_REG;
    DX.data_size = data_size;

    AX.addr = REG_AX;
    AX.type = OPR_REG;
    AX.data_size = 8;

    operand_read(&AX);
    operand_read(&DX);
    // printf("dx = %x\nRead dx val = %x\n", cpu.edx, DX.val);
    pio_write(DX.val, AX.data_size / 8, AX.val);
    return len;
}

make_instr_func(out_v)
{
    int len = 1;
    OPERAND DX, AX;

    DX.addr = REG_DX;
    DX.type = OPR_REG;
    DX.data_size = data_size;

    AX.addr = REG_AX;
    AX.type = OPR_REG;
    AX.data_size = data_size;

    operand_read(&AX);
    operand_read(&DX);
    // printf("dx = %x\nRead dx val = %x\n", cpu.edx, DX.val);
    pio_write(DX.val, AX.data_size / 8, AX.val);
    return len;
}
