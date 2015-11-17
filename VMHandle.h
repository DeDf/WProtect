#ifndef _VMHANDLE_H_
#define _VMHANDLE_H_

#include <stdio.h>
#include <list>
#include <vector>
#include "AsmJit/AsmJit.h"
using namespace AsmJit;

#define REG_NUMBER         25 //虚拟机寄存器数量
#define VMSTACK_MIN_COUNT  25
/* 32位 eax,ebx,ecx,edx,ebp,esi,edi,eflag 和 8个临时寄存器 + 跳转寄存器 + 密钥寄存器 */

typedef struct _pcode_encryption_
{
  long key;
  Assembler *enfuc;
  typedef void ( *MyFn )( void *,void * );
  MyFn fn;
}vcode_encryption;

typedef struct _pcode_decryption_
{
    Assembler *defuc;
}vcode_decryption;

typedef struct _encryption_  //加密在PCODE的时候加密
{
  int key;
  Compiler  *enfuc;  //ebx的加解
  typedef void ( *MyFn )( void * );
  MyFn en_fn;
}encryption;

typedef struct _decryption_  //解密嵌入程序
{
  Assembler  *defuc; //这个是解密Key的
}decryption;

////////////类型
#define READ_BYTE  1
#define READ_WORD  2
#define READ_DWORD 4
#define READ_QWORD 8

typedef struct _handle_info
{
    Label *label; //标签指针

    char handle_name[20];

    unsigned char *buf;
    unsigned long size;
    unsigned long offset;
    unsigned long type; //1-8是从esi读取的字节

    std::vector <encryption> encode;
    std::vector <decryption> decode_key;
    std::list <vcode_encryption> encode_pcode;
    std::list <vcode_decryption> decode_pcode;
  
}handle_info;

class VMHandle{
public:
	VMHandle();
	~VMHandle();
    //
    Assembler a;
    long key;
    bool sign;

    void full_handle_info( handle_info & info,char flag );

    handle_info set_pc();
    handle_info push_stack_top_base();
    handle_info pop_stack_top_base();
    handle_info run_stack_code();
    handle_info check_stack();
    handle_info set_key();

    handle_info initialization(long pcode_base); //pcode地址
    handle_info dispatch(long table_addr);
    handle_info ret();

	handle_info b_read_stack(); //byte
	handle_info w_read_stack(); //word
	handle_info d_read_stack(); //dword
    handle_info q_read_stack();

    handle_info b_write_stack();
    handle_info w_write_stack();
    handle_info d_write_stack();
    handle_info q_write_stack();

    handle_info b_push_reg();
    handle_info w_puah_reg();
    handle_info d_push_reg();
    handle_info q_push_reg();

    handle_info b_pop_reg();
    handle_info w_pop_reg();
    handle_info d_pop_reg();
    handle_info q_pop_reg();

    handle_info b_push_imm();
    handle_info w_push_imm();
    handle_info d_push_imm();
    handle_info q_push_imm();

    handle_info b_shl();
    handle_info w_shl();
    handle_info d_shl();
    handle_info q_shl();

    handle_info b_shr();
    handle_info w_shr();
    handle_info d_shr();
    handle_info q_shr();

    handle_info shld();
    handle_info shrd();

    handle_info b_nand();
    handle_info w_nand();
    handle_info d_nand();
    handle_info q_nand();

    handle_info b_read_mem();
    handle_info w_read_mem();
    handle_info d_read_mem();
    handle_info q_read_mem();

    handle_info b_write_mem();
    handle_info w_write_mem();
    handle_info d_write_mem();
    handle_info q_write_mem();

    handle_info b_push_imm_sx();
    handle_info w_push_imm_sx();
    handle_info d_push_imm_sx();
    handle_info b_push_imm_zx();
    handle_info w_push_imm_zx();
    handle_info d_push_imm_zx();
    
    handle_info b_add();
    handle_info w_add();
    handle_info d_add();
    handle_info q_add();

    handle_info b_rol();
    handle_info w_rol();
    handle_info d_rol();
    handle_info q_rol();

    handle_info b_ror();
    handle_info w_ror();
    handle_info d_ror();
    handle_info q_ror();

    handle_info b_bool();
    handle_info w_bool();
    handle_info d_bool();
    handle_info q_bool();

    handle_info cpuid();
    handle_info in();
    handle_info rdtsc();
    handle_info fstsw();
    handle_info int3();

    ///////label/////
    Label l_set_pc;
    Label l_push_stack_top_base;
    Label l_pop_stack_top_base;
    Label l_run_stack_code;
    Label l_check_stack;
    Label l_set_key;
    Label l_key_dispatch;

    Label l_initialization;
    Label l_dispatch;
    Label l_ret;

    Label l_b_read_mem;
    Label l_w_read_mem;
    Label l_d_read_mem;
    Label l_q_read_mem;

    Label l_b_write_mem;
    Label l_w_write_mem;
    Label l_d_write_mem;
    Label l_q_write_mem;
    
	Label l_b_read_stack; //byte
	Label l_w_read_stack; //word
	Label l_d_read_stack; //dword
    Label l_q_read_stack;

    Label l_b_write_stack;
    Label l_w_write_stack;
    Label l_d_write_stack;
    Label l_q_write_stack;

    Label l_b_push_reg;
    Label l_w_push_reg;
    Label l_d_push_reg;
    Label l_q_push_reg;

    Label l_b_pop_reg;
    Label l_w_pop_reg;
    Label l_d_pop_reg;
    Label l_q_pop_reg;

    Label l_b_push_imm;
    Label l_w_push_imm;
    Label l_d_push_imm;
    Label l_q_push_imm;

    Label l_b_shl;
    Label l_w_shl;
    Label l_d_shl;
    Label l_q_shl;

    Label l_b_shr;
    Label l_w_shr;
    Label l_d_shr;
    Label l_q_shr;

    Label l_shld;
    Label l_shrd;

    Label l_b_nand;
    Label l_w_nand;
    Label l_d_nand;
    Label l_q_nand;

    Label l_b_push_imm_sx;
    Label l_w_push_imm_sx;
    Label l_d_push_imm_sx;
    Label l_b_push_imm_zx;
    Label l_w_push_imm_zx;
    Label l_d_push_imm_zx;
    
    Label l_b_add;
    Label l_w_add;
    Label l_d_add;
    Label l_q_add;

    Label l_b_rol;
    Label l_w_rol;
    Label l_d_rol;
    Label l_q_rol;
    Label l_b_ror;
    Label l_w_ror;
    Label l_d_ror;
    Label l_q_ror;

    Label l_in;
    Label l_rdtsc;
    Label l_cpuid;
    Label l_fstsw;
    Label l_int3;

private:
    void read_pc_byte (handle_info &info);
    void read_pc_word (handle_info &info);
    void read_pc_dword(handle_info &info);
    void read_pc_qword(handle_info &info);
};

#endif /* _VMHANDLE_H_ */
