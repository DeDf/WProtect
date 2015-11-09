#ifndef _VMHANDLE_H_
#define _VMHANDLE_H_

#include <list>
#include <vector>
#include "AsmJit/AsmJit.h"
#include <stdio.h>

#define REG_NUMBER         25 //������Ĵ�������
#define VMSTACK_MIN_COUNT  25
/* 32λ eax,ebx,ecx,edx,ebp,esi,edi,eflag �� 8����ʱ�Ĵ��� + ��ת�Ĵ��� + ��Կ�Ĵ��� */

typedef struct _pcode_encryption_
{
  long key;
  AsmJit::Assembler *enfuc;
  typedef void ( *MyFn )( void *,void * );
  MyFn fn;
}vcode_encryption;

typedef struct _pcode_decryption_
{
    AsmJit::Assembler *defuc;
    int id;
}vcode_decryption;

typedef struct _encryption_ //������PCODE��ʱ�����
{
  typedef void ( *MyFn )( void * );

  unsigned long type; //���Ͱ������ܵļĴ������Ƿ���KEY
  AsmJit::Compiler  *enfuc;  //ebx�ļӽ�
  MyFn fn;
  int id;
}encryption;

typedef struct _decryption_  //����Ƕ�����
{
  AsmJit::Assembler  *defuc; //����ǽ���Key��
}decryption;

typedef struct _handle_info{

  std::vector <encryption> encode_key;
  std::vector <decryption> decode_key;
  std::list <vcode_encryption> encode_pcode;
  std::list <vcode_decryption> decode_pcode;

  ////////////����
#define READ_BYTE  1
#define READ_WORD  2
#define READ_DWORD 4
#define READ_QWORD 8

  unsigned char * buf;
  unsigned long size;
  unsigned long offset;
  unsigned long type; //1-8�Ǵ�esi��ȡ���ֽ�

#ifdef _DEBUG
  char handle_name[20];
#endif
  AsmJit::Label *label; //��ǩָ��
}handle_info;

class VMHandle{
public:
	VMHandle();
	~VMHandle();
public:
    AsmJit::Assembler a;
    long key;
    bool sign;

    void full_handle_info( handle_info & info,char flag );

    handle_info set_pc();
    handle_info push_stack_top_base();
    handle_info pop_stack_top_base();
    handle_info run_stack_code();
    handle_info check_stack();
    handle_info set_key();

    handle_info initialization(long pcode_base); //pcode��ַ
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
    
#ifdef _DEBUG
    handle_info int3();
#endif
    ///////label/////
    AsmJit::Label l_set_pc;
    AsmJit::Label l_push_stack_top_base;
    AsmJit::Label l_pop_stack_top_base;
    AsmJit::Label l_run_stack_code;
    AsmJit::Label l_check_stack;
    AsmJit::Label l_set_key;
    AsmJit::Label l_key_dispatch;

    AsmJit::Label l_initialization;
    AsmJit::Label l_dispatch;
    AsmJit::Label l_ret;

    AsmJit::Label l_b_read_mem;
    AsmJit::Label l_w_read_mem;
    AsmJit::Label l_d_read_mem;
    AsmJit::Label l_q_read_mem;

    AsmJit::Label l_b_write_mem;
    AsmJit::Label l_w_write_mem;
    AsmJit::Label l_d_write_mem;
    AsmJit::Label l_q_write_mem;
    
	AsmJit::Label l_b_read_stack; //byte
	AsmJit::Label l_w_read_stack; //word
	AsmJit::Label l_d_read_stack; //dword
    AsmJit::Label l_q_read_stack;

    AsmJit::Label l_b_write_stack;
    AsmJit::Label l_w_write_stack;
    AsmJit::Label l_d_write_stack;
    AsmJit::Label l_q_write_stack;

    AsmJit::Label l_b_push_reg;
    AsmJit::Label l_w_push_reg;
    AsmJit::Label l_d_push_reg;
    AsmJit::Label l_q_push_reg;

    AsmJit::Label l_b_pop_reg;
    AsmJit::Label l_w_pop_reg;
    AsmJit::Label l_d_pop_reg;
    AsmJit::Label l_q_pop_reg;

    AsmJit::Label l_b_push_imm;
    AsmJit::Label l_w_push_imm;
    AsmJit::Label l_d_push_imm;
    AsmJit::Label l_q_push_imm;

    AsmJit::Label l_b_shl;
    AsmJit::Label l_w_shl;
    AsmJit::Label l_d_shl;
    AsmJit::Label l_q_shl;

    AsmJit::Label l_b_shr;
    AsmJit::Label l_w_shr;
    AsmJit::Label l_d_shr;
    AsmJit::Label l_q_shr;

    AsmJit::Label l_shld;
    AsmJit::Label l_shrd;

    AsmJit::Label l_b_nand;
    AsmJit::Label l_w_nand;
    AsmJit::Label l_d_nand;
    AsmJit::Label l_q_nand;

    AsmJit::Label l_b_push_imm_sx;
    AsmJit::Label l_w_push_imm_sx;
    AsmJit::Label l_d_push_imm_sx;
    AsmJit::Label l_b_push_imm_zx;
    AsmJit::Label l_w_push_imm_zx;
    AsmJit::Label l_d_push_imm_zx;
    
    AsmJit::Label l_b_add;
    AsmJit::Label l_w_add;
    AsmJit::Label l_d_add;
    AsmJit::Label l_q_add;

    AsmJit::Label l_b_rol;
    AsmJit::Label l_w_rol;
    AsmJit::Label l_d_rol;
    AsmJit::Label l_q_rol;
    AsmJit::Label l_b_ror;
    AsmJit::Label l_w_ror;
    AsmJit::Label l_d_ror;
    AsmJit::Label l_q_ror;

    AsmJit::Label l_in;
    AsmJit::Label l_rdtsc;
    AsmJit::Label l_cpuid;
    AsmJit::Label l_fstsw;

#ifdef _DEBUG
    AsmJit::Label l_int3;
#endif

private:
    void read_pc_byte (handle_info &info);
    void read_pc_word (handle_info &info);
    void read_pc_dword(handle_info &info);
    void read_pc_qword(handle_info &info);
};


#endif /* _VMHANDLE_H_ */
