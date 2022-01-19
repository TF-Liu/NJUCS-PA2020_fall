#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	uint32_t dir = (laddr >> 22) & 0x3ff;
	uint32_t page = (laddr >> 12) & 0x3ff;
	uint32_t offset = laddr & 0xfff;
	
	PDE *pde_header = (void *)(hw_mem + (cpu.cr3.pdbr << 12));
	PDE target_pde = pde_header[dir];
	assert(target_pde.present);
	
	PTE *pte_header = (void *)(hw_mem + (target_pde.page_frame << 12));
	PTE target_pte = pte_header[page];
	assert(target_pte.present);
	
	return (target_pte.page_frame << 12) + offset;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
