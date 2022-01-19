#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `in' instructions here.
*/

make_instr_func(in_b)
{
    int len = 1;
    OPERAND DX, AX;

    DX.addr = REG_DX;
    DX.type = OPR_REG;
    DX.data_size = data_size;

    AX.addr = REG_AX;
    AX.type = OPR_REG;
    AX.data_size = 8;

    operand_read(&DX);
    // printf("dx = %x\nRead dx val = %x\n", cpu.edx, DX.val);
    AX.val = pio_read(DX.val, DX.data_size / 8);
    operand_write(&AX);

    return len;
}

make_instr_func(in_v)
{
    int len = 1;
    OPERAND DX, AX;

    DX.addr = REG_DX;
    DX.type = OPR_REG;
    DX.data_size = data_size;

    AX.addr = REG_AX;
    AX.type = OPR_REG;
    AX.data_size = data_size;

    operand_read(&DX);
    // printf("dx = %x\nRead dx val = %x\n", cpu.edx, DX.val);
    AX.val = pio_read(DX.val, DX.data_size / 8);
    operand_write(&AX);

    return len;
}
