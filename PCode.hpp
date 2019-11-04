/*
 *   Copyrigth (C) Mon May 20 01:02:51 2013 XiaoWei
 *
 *                       handsomexiaowei@gmail.com
 */

#ifndef _PCODE_H_
#define _PCODE_H_

#include <iostream>
#include <list>
#include <vector>
using namespace std;

#include "VMHandle.h"
#include "VOperand.hpp"
//#include "CryptoPP/crc.h"

typedef struct V_HANDLE_CONTEXT_
{
    std::vector <encryption> *encode_key;
    std::list <vcode_encryption> *encode_pcode;
    long type;
    unsigned char handle_i;  // handle_index

}VMHandleInfo,*pVMHandleInfo;

typedef struct _VM_HANDLE_
{
  VMHandleInfo  b_read_stack; 
  VMHandleInfo  w_read_stack; 
  VMHandleInfo  d_read_stack;  
  
  VMHandleInfo  b_write_stack;
  VMHandleInfo  w_write_stack;
  VMHandleInfo  d_write_stack;

  VMHandleInfo  b_push_reg;
  VMHandleInfo  w_push_reg;
  VMHandleInfo  d_push_reg;

  VMHandleInfo  b_pop_reg;
  VMHandleInfo  w_pop_reg;
  VMHandleInfo  d_pop_reg;

  VMHandleInfo  b_push_imm;
  VMHandleInfo  w_push_imm;
  VMHandleInfo  d_push_imm;

  VMHandleInfo  b_shl;
  VMHandleInfo  w_shl;
  VMHandleInfo  d_shl;

  VMHandleInfo  b_shr;
  VMHandleInfo  w_shr;
  VMHandleInfo  d_shr;

  VMHandleInfo  shld;
  VMHandleInfo  shrd;

  VMHandleInfo  b_nand;
  VMHandleInfo  w_nand;
  VMHandleInfo  d_nand;
  
  VMHandleInfo  set_pc;
  VMHandleInfo  ret;
  VMHandleInfo  in;
  VMHandleInfo  rdtsc;
  VMHandleInfo  cpuid;
  VMHandleInfo  check_stack;
  VMHandleInfo  dispatch;
  VMHandleInfo  push_stack_top_base;
  VMHandleInfo  b_read_mem;
  VMHandleInfo  w_read_mem;
  VMHandleInfo  d_read_mem;

  VMHandleInfo  b_write_mem;
  VMHandleInfo  w_write_mem;
  VMHandleInfo  d_write_mem;

  VMHandleInfo pop_stack_top_base;
  VMHandleInfo b_push_imm_sx;
  VMHandleInfo w_push_imm_sx;

  VMHandleInfo b_push_imm_zx;
  VMHandleInfo w_push_imm_zx;

  VMHandleInfo b_add;
  VMHandleInfo w_add;
  VMHandleInfo d_add;

  VMHandleInfo b_rol;
  VMHandleInfo w_rol;
  VMHandleInfo d_rol;

  VMHandleInfo b_ror;
  VMHandleInfo w_ror;
  VMHandleInfo d_ror;

  VMHandleInfo set_key;
  VMHandleInfo run_stack;

  VMHandleInfo fstsw;
  VMHandleInfo int3;

#ifdef PROTECT_X64
  VMHandleInfo d_push_imm_sx;  
  VMHandleInfo d_push_imm_zx;  
  VMHandleInfo  q_read_stack;
  VMHandleInfo  q_write_stack;
  VMHandleInfo  q_push_reg;
  VMHandleInfo  q_pop_reg;
  VMHandleInfo  q_push_imm;
  VMHandleInfo  q_shl;
  VMHandleInfo  q_shr;
  VMHandleInfo  q_nand;
  VMHandleInfo  q_read_mem;
  VMHandleInfo  q_write_mem;
  VMHandleInfo q_rol;
  VMHandleInfo q_ror;
  VMHandleInfo q_add;
#endif
}vm_handle,*pvm_handle;
  
typedef struct _VM_BUF_INFO_
{
  unsigned char * buf;
  unsigned long size;
  unsigned long offset;
  bool sign;  //true��ʾ�� flase��ʾ��
  vm_handle handle_table;
}vm_buf_info,*pvm_buf_info;


typedef struct _V_REG_
{
  long reg_id;
  long reg_offset;
  long status;
}vm_reg,*pvm_reg;


typedef struct _VM_REG_CONTEXT_
{
  vm_reg vr[REG_NUMBER];
  pvm_reg nax;
  pvm_reg nbx;
  pvm_reg ncx;
  pvm_reg ndx;
  pvm_reg nsp;
  pvm_reg nbp;
  pvm_reg ndi;
  pvm_reg nsi;
  pvm_reg eflag;
  pvm_reg r8;
  pvm_reg r9;
  pvm_reg r10;
  pvm_reg r11;
  pvm_reg r12;
  pvm_reg r13;
  pvm_reg r14;
  pvm_reg r15;
}vm_reg_context,*pvm_reg_context;

typedef struct _REGISTER_STORE_
{
  long register_array[REG_NUMBER];
  long register_count;

  _REGISTER_STORE_(  )
  {
    memset( register_array, T_INVALID, sizeof(long)*REG_NUMBER );
    register_count = 0;
  }

  ~_REGISTER_STORE_(  )
  {
  }

  void operator = ( const _REGISTER_STORE_ & rhs )
  {
    register_count = rhs.register_count;
    memcpy( &register_array,&rhs.register_array,sizeof( long )*REG_NUMBER );
  }

  long get_key()
  {
      //CryptoPP::CRC32 crc;

    int _key = 0;
    for (int i = 0; i < register_count; i++)
    {
        //crc.Update((byte*)&register_array[i],4);
    }
    //crc.Final((byte*)&_key);
    return _key;
  }
}RegisterStore,*pRegisterStore;

#define DEFAULT_ALLOC_SIZE 0x150
  
class PCode
{
public:
  PCode();
  PCode(vm_handle table,bool sign);
  virtual ~PCode();

  RegisterStore register_store_in;   //������ʱ��Ĵ�������˳��
  RegisterStore register_store_out;  //��ȥ��ʱ��Ĵ�������˳��
  vm_buf_info pcode_info;
  vm_reg_context v_reg_context;

  void count_vmcode_begin();
  size_t count_vmcode_end();
  
  void check_pcode_buf();

  void db(unsigned char  b);
  void dw(unsigned short w);
  void dd(unsigned int   d);
  void dq(unsigned long long q);
  void da(long a);

  void init_handle_table(vm_handle table);
  void init_sign(bool b);
  char get_rand_reg();
  void v_push_register(long _register,bool _disable = true);
  void v_pop_register(long _register);
  pvm_reg find_invalid_register();
  void call_encode_pcode_fn(vcode_encryption::MyFn fn,void *data,long * ikey);
  size_t get_code_size();
  void out_log_file(char * file_name);

#ifdef PROTECT_X64
  unsigned long _key_;  // = rand(); //PCode��KEY ������VM KEY ����ԭʼKEY
  unsigned long key;    // = _key_;  //���KEY�Ǳ仯��
  const unsigned long get_original_key();
  const unsigned long get_current_key();
  void set_key(const unsigned long _key);
#else
  unsigned int _key_;
  unsigned int key;
  const unsigned int get_original_key();
  const unsigned int get_current_key();
  void set_key(const unsigned int _key);
#endif

  pVMHandleInfo current_instruction;

  FILE *v_log;
  char reg_name[256];
  FILE *get_log(){ return v_log; }
  void set_register_name(long _register);
  void out_info(char *_sz);

  void operator = ( const PCode & rhs )
  {
    memcpy( &register_store_in.register_array,
        &rhs.register_store_out.register_array,
        sizeof( long )*REG_NUMBER );
    register_store_in.register_count = rhs.register_store_out.register_count;

    if ( _key_ != key )
    {
      printf("�����Ѿ���ʼ����,�޷��ı�Key\n");    
      throw;
      return;
    }
    _key_ = rhs.key;
    key = rhs.key;    
  }
  
  long r_pc_size; //����ָ��Ҫ��ȡpc���ٸ��ֽ�

private:
  size_t vmcode_size;  //����
  bool is_begin_vmcode;
};

#endif /* _PCODE_H_ */
