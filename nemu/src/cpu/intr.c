#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

static void push(uint32_t num)
{
    OPERAND temp;
    temp.data_size = data_size;
    temp.sreg = SREG_CS;
    temp.type = OPR_MEM;
	cpu.esp -= data_size / 8;
    temp.addr = cpu.esp;
	temp.val = sign_ext(num, data_size);
	operand_write(&temp);
}

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
    // Trigger an exception/interrupt with 'intr_no'
    // 'intr_no' is the index to the IDT

    // Push EFLAGS, CS, and EIP
    push(cpu.eflags.val);
	push(cpu.cs.val);
	push(cpu.eip);

    // Find the IDT entry using 'intr_no'
    paddr_t addr = (paddr_t) hw_mem + page_translate(segment_translate(cpu.idtr.base + (intr_no << 3), SREG_DS));
	GateDesc* gatedesc = (void*) addr;
    assert(gatedesc -> present);

    // Clear IF if it is an interrupt
	if (gatedesc -> type == 0xe) {
		cpu.eflags.IF = 0;
	}

    // Set CS:EIP to the entry of the interrupt handler
    // need to reload CS with load_sreg()
    cpu.eip = (gatedesc -> offset_31_16 << 16) + gatedesc -> offset_15_0;

#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}

