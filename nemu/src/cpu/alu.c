#include "cpu/cpu.h"

void set_CF_add(uint32_t result, uint32_t src, size_t data_size);
void set_CF_adc(uint32_t result, uint32_t src, size_t data_size);
void set_CF_sub(uint32_t result, uint32_t dest, size_t data_size);
void set_CF_sbb(uint32_t result, uint32_t dest, size_t data_size);
void set_CF_shl(uint32_t src, uint32_t dest, size_t data_size);
void set_CF_shr(uint32_t src, uint32_t dest, size_t data_size);
void set_CF_mul(uint64_t result, size_t data_size);
void set_CF_imul(int64_t result, size_t data_size);
void set_ZF(uint32_t result, size_t data_size);
void set_SF(uint32_t result, size_t data_size);
void set_PF(uint32_t result);
void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size);
void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size);
void set_OF_mul(uint64_t result, size_t data_size);
void set_OF_imul(int64_t result, size_t data_size);

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest + src;                               // 获取计算结果
	//printf("src = %d, dest = %d, res = %d, data_size = %d\n",src , dest, res, data_size);
	set_CF_add(res, src, data_size);                // 设置标志位
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_add(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));  // 高位清零并返回
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest + src + cpu.eflags.CF;
	set_CF_adc(res, src, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_add(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest - src;
	set_CF_sub(res, dest, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_sub(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest - (src + cpu.eflags.CF);
	set_CF_sbb(res, dest, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_sub(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	uint64_t res = 0;
	uint64_t src1 = src;                                        // 32位整数相乘结果还是32位，所以要定义成64位整数
	uint64_t dest1 = dest;
	res = dest1 * src1;
	set_CF_mul(res, data_size);
	set_OF_mul(res, data_size); 
	return res & (0xFFFFFFFFFFFFFFFF >> (64 - 2 * data_size));  // 减去两倍data_size是为了保留两倍data_size位
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	int64_t res = 0;
	int64_t src1 = src;
	int64_t dest1 = dest;
	res = dest1 * src1;
	set_CF_imul(res, data_size);
	set_OF_imul(res, data_size);
	return res;                 // 因为此处为有符号整数，所以高位不清零
#endif
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest / src;
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	int32_t res = 0;
	res = dest / src;
	return res;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	uint32_t res = 0;
	res = dest % src;
	return res;         // 因为函数没有传入data_size，所以高位不清零
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	int32_t res = 0;
	res = dest % src;
	return res;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest & src;
	cpu.eflags.CF = 0;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	cpu.eflags.OF = 0;
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest ^ src;
	cpu.eflags.CF = 0;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	cpu.eflags.OF = 0;
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest | src;
	cpu.eflags.CF = 0;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	cpu.eflags.OF = 0;
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest << src;
	set_CF_shl(src, dest, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	uint32_t res = 0;
	res = (dest >> src) & (0xFFFFFFFF >> (32 - data_size + src));   // 逻辑右移，需要高位补零，且多补src位
	set_CF_shr(src, dest, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	return res;
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	uint32_t res = 0;
	int sign = (dest >> (data_size - 1)) % 2;
	if(sign == 0)
	    // 正数算术右移与逻辑右移相同
	    res = (dest >> src) & (0xFFFFFFFF >> (32 - data_size + src));
	else
	{
	    // 负数算术右移，需要高位补符，然后根据data_size高位清零
	    res = (dest >> src) | (0xFFFFFFFF << (data_size - src));    // 补符
	    res = res & (0xFFFFFFFF >> (32 - data_size));               // 高位清零
	}
	set_CF_shr(src, dest, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	return res;
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest << src;
	set_CF_shl(src, dest, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

void set_CF_add(uint32_t result, uint32_t src, size_t data_size)
{
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    cpu.eflags.CF = result < src;
}

void set_CF_adc(uint32_t result, uint32_t src, size_t data_size)
{
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    // 注意adc与add在判断CF上的区别
    if(cpu.eflags.CF == 0)
        cpu.eflags.CF = result < src;
    else
        cpu.eflags.CF = result <= src;
}

void set_CF_sub(uint32_t result, uint32_t dest, size_t data_size)
{
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    cpu.eflags.CF = result > dest;
}

void set_CF_sbb(uint32_t result, uint32_t dest, size_t data_size)
{
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    if(cpu.eflags.CF == 0)
        cpu.eflags.CF = result > dest;
    else
        cpu.eflags.CF = result >= dest;
}

void set_CF_shl(uint32_t src, uint32_t dest, size_t data_size)
{
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    uint32_t res = dest >> (data_size - src);                           // CF是看移出去的最后一位
    cpu.eflags.CF = (res % 2);
}

void set_CF_shr(uint32_t src, uint32_t dest, size_t data_size)
{
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    uint32_t res = dest >> (src - 1);
    cpu.eflags.CF = (res % 2);
}

void set_CF_mul(uint64_t result, size_t data_size)
{
    if((result >> data_size) != 0)
        cpu.eflags.CF = 1;
    else
        cpu.eflags.CF = 0;
}

void set_CF_imul(int64_t result, size_t data_size)
{
    if((result >> data_size) != 0)
        cpu.eflags.CF = 1;
    else
        cpu.eflags.CF = 0;
}

void set_ZF(uint32_t result, size_t data_size)
{
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    cpu.eflags.ZF = (result == 0);
}

void set_SF(uint32_t result, size_t data_size)
{
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    cpu.eflags.SF = sign(result);
    
}

void set_PF(uint32_t result)
{
    result = result & 0xFF;
    int number_1 = 0;
    for(int i = 0; i < 8; i ++)
    {
        if(result % 2 == 1)
            number_1 ++;
        result /= 2;
    }
    cpu.eflags.PF = !(number_1 % 2);
    
}

void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size)
{
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    
    if(sign(src) == sign(dest))
    {
        if(sign(src) != sign(result))
            cpu.eflags.OF = 1;
        else
            cpu.eflags.OF = 0;
    }
    else
    {
        cpu.eflags.OF = 0;
    }
}

void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size)
{
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    
    if(sign(src) != sign(dest))
    {
        if(sign(src) == sign(result))
            cpu.eflags.OF = 1;
        else
            cpu.eflags.OF = 0;
    }
    else
    {
        cpu.eflags.OF = 0;
    }
}

void set_OF_mul(uint64_t result, size_t data_size)
{
    if((result >> data_size) != 0)
        cpu.eflags.OF = 1;
    else
        cpu.eflags.OF = 0;
}

void set_OF_imul(int64_t result, size_t data_size)
{
    if((result >> data_size) != 0)
        cpu.eflags.OF = 1;
    else
        cpu.eflags.OF = 0;
}