#include "memory/mmu/cache.h"
#include "memory/memory.h"

#ifdef CACHE_ENABLED

CacheLine cache[NUM_OF_LINE];

// init the cache
void init_cache()
{
	// implement me in PA 3-1
	for(int i = 0; i < NUM_OF_LINE; i++)
	{
	    cache[i].validBit = false;
	}
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{
	// implement me in PA 3-1
	
	// 根据paddr找到标记、组号、与块内地址
	int tag = paddr >> 13;          // 19 bits
	int set = (paddr >> 6) & 0x7f;  // 7 bits
	int word = paddr & 0x3f;        // 6 bits
	
	// 根据组号，与组内的CacheLine比较标记和有效位，去定位相应的组（起始Cache行号）
	int line = -1;
	bool hit = false;
	for (int i = 0; i < LINE_PER_SET; i++) {
	    if (cache[LINE_PER_SET * set + i].validBit == true && cache[LINE_PER_SET * set + i].tag == tag)
	    {
	        hit = true;
	        line = LINE_PER_SET * set + i;
	        break;
	    }
	}

    // 若未命中则直接写主存，非写分配法中写缺失不导致Cache装载	
	// 若命中，根据全写法写Cache同时写主存
	if (hit == true)
	{
	    // 若数据不跨两个Cache行则直接写入Cache
	    if (word + len <= BYTE_PER_LINE)
	    {
	        memcpy(cache[line].data + word, &data, len);
	    }
	    // 若数据跨两个Cache行，则再次判断第二个Cache行是否应该写入
	    else
	    {
	        memcpy(cache[line].data + word, &data, BYTE_PER_LINE - word);
	        cache_write(paddr + BYTE_PER_LINE - word, word + len - BYTE_PER_LINE, data >> (8 * (BYTE_PER_LINE - word)));
	    }
	}
	
	memcpy(hw_mem + paddr, &data, len);
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{
	// implement me in PA 3-1
	
	// 根据paddr找到标记、组号、与块内地址
	int tag = paddr >> 13;          // 19 bits
	int set = (paddr >> 6) & 0x7f;  // 7 bits
	int word = paddr & 0x3f;        // 6 bits
	uint32_t result = 0;
	
	// 根据组号，与组内的CacheLine比较标记和有效位，去定位相应的组（起始Cache行号）
	int line = -1;
	bool hit = false;
	for (int i = 0; i < LINE_PER_SET; i++) {
	    if (cache[LINE_PER_SET * set + i].validBit == true && cache[LINE_PER_SET * set + i].tag == tag)
	    {
	        hit = true;
	        line = LINE_PER_SET * set + i;
	        break;
	    }
	}
	
	// 若命中则按照块内地址读cache，直接在cache中取数据
	// 若不命中，则按照物理内存读主存，把块搬入cache，填好标记和有效位，按照块内地址读cache
	if (hit == false)
	{
	    // 寻找空的cache行
	    for (int i = 0; i < LINE_PER_SET; i++) {
	        if (cache[LINE_PER_SET * set + i].validBit == false)
	        {
	            line = LINE_PER_SET * set + i;
	            break;
	        }
	    }
	    
	    // 若无空的cache行则使用随机替换策略空出一行
	    if (line == -1)
	    {
	        srand((unsigned)time(0));
	        line = LINE_PER_SET * set + (rand() % LINE_PER_SET);
	        cache[line].validBit = false;
	    }
	    
	   // 把块搬入cache，填好标记和有效位
	   memcpy(cache[line].data, hw_mem + paddr - word, BYTE_PER_LINE);
	   cache[line].validBit = true;
	   cache[line].tag = tag;
	}
	
	// 按照块内地址读cache
	// 若数据不跨两个Cache行则直接读Cache
	if (word + len <= BYTE_PER_LINE)
	{
	    memcpy(&result, cache[line].data + word, len);
	}
	// 若数据跨两个Cache行，则分两次读两个Cache行后，将结果按小端方式拼起来
	else
	{
	    uint32_t temp1 = 0, temp2 = 0;
	    memcpy(&temp1, cache[line].data + word, BYTE_PER_LINE - word);
	    temp2 = cache_read(paddr + BYTE_PER_LINE - word, word + len - BYTE_PER_LINE) << (8 * (BYTE_PER_LINE - word));
	    result = temp1 | temp2;
	}
	
	return result;
}

#endif