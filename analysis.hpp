/*
 *   Copyright (C) Wed May 29 04:00:49 2013 XiaoWei
 *
 *                       handsomexiaowei@gmail.com
 */

#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "Libudis86/udis86.h"

typedef struct ANALYSIS_OPTIONS
{
	long addr;
    void *buf;
    long size;
}CodeBufferInfo,*pCodeBufferInfo;

#ifdef PC_NONE
#undef PC_NONE
#endif
#define PC_NONE -1
#define PC_RET  -2
#define PC_EXIT -3
#define PC_MEM  -4
#define PC_MIN -10000

#define PC_LABEL_MAX  10000
#define TYPE_LABEL 1
#define TYPE_ADDR  2   
#define TYPE_MEM   3
#define TYPE_RET   4
#define TYPE_CALL  5

enum OPCODE_ATTRIBUTE{
    OPCODE_ATTRIBUTE_NORMAL,
    OPCODE_ATTRIBUTE_CALL,
    OPCODE_ATTRIBUTE_EXTERNAL_CALL,
    OPCODE_ATTRIBUTE_JMP,
    OPCODE_ATTRIBUTE_EXTERNAL_JMP,
    OPCODE_ATTRIBUTE_JCC,
    OPCODE_ATTRIBUTE_EXTERNAL_JCC,
    OPCODE_ATTRIBUTE_RET,
    OPCODE_ATTRIBUTE_EXIT
};

typedef struct ASSEMBLER_PIECE_INFO   //�������Ϣ �� 
{
  ud_t u;  //����������Ϣ
}AssemblerInfo,*pAssemblerInfo;

typedef struct ASSEMBLER_TREE
{
  //long address; //���ַ
  bool set_key;
  long key;
  long jcc_addr; //��ת���ĵ�ַ
  long next_instruction; //��һ��ָ����ת��ַ mov eax,1  jmp hello mov eax,2 hello: ret    mov eax,2�������
  long reloc_address;   //�嶨λ��
  long id; //�����id ��0��ʼ
  long base; //����鿪ʼ��ַ  //���ַ
  long size; //������С

  // std::list<ud_t>asmpiect;
  long eflag;
  //pAssemblerInfo asmpiece;  //ָ�����ͷ
	ASSEMBLER_TREE *LeftChild; //��ֽ
	ASSEMBLER_TREE *RightChild; //�Һ�ֽ
	std::vector <ud_t> asmpiece;
}AssemblerTree,*pAssemblerTree;
 
typedef struct _AddressArray
{
  private:
   long * a_array;
   size_t a_size;

  public:
   _AddressArray(int _size)
   {
     a_array = new long[_size];
     a_size = _size;
   }
   
   ~_AddressArray()
   {
#ifndef _DEBUG  // ����Debug����Զ��ͷ��ڴ棡
     if(a_array != NULL)
       delete [] a_array;
#endif
   }

   long &operator[](size_t _index)
   {
     if (_index >= a_size || a_array == NULL) {
       printf("Address_Array�������Խ��");
     }
     return a_array[_index];
   }

   size_t get_size()
   {
     return a_size;
   }

   void set_size(size_t _size)
   {
     a_size = _size;
   }
}AddressArray;
      

typedef struct _CodePiece
{
  _CodePiece():is_jcc(false){}
  ~_CodePiece(){}
  void set_label(int id)
  {
    label = id;
  }
  void set_jmplabel(int jmpid)
  {
    jmp_to_label = jmpid;
  }
  void add_assemble(ud_t & ud)
  {
    piece.push_back(ud);
  }
  int get_jmplabel()
  {
    return jmp_to_label;
  }
  int get_label()
  {
    return label;
  }
  void set_is_jcc(bool _is_jcc)
  {
    is_jcc = _is_jcc;
  }
  bool get_is_jcc()
  {
    return is_jcc;
  }
  std::list<ud_t> & get_piece()
  {
    return piece;
  }

  void set_opcode_attribute(OPCODE_ATTRIBUTE attribute)
  {
      opcode_attribute = attribute;
  }
  OPCODE_ATTRIBUTE get_opcode_attribute()
  {
      return opcode_attribute;
  }

private:
  int label;
  int jmp_to_label;
  OPCODE_ATTRIBUTE opcode_attribute;
  bool is_jcc;
  std::list<ud_t> piece;
}CodePiece;


class Analysis
{
public:
    long base;
    long size;
    pAssemblerTree root;
    long block_count;
    const bool L;// = false;
    const bool R;// = true;
    //
    Analysis();
    virtual ~Analysis();
    size_t reloc_asm_address(long old_address,long new_address); //����һ���ɹ��ض�λ���� 0Ϊû�ҵ�
    void reloc_asm_address();
    AddressArray analysis_code_piece_address(pCodeBufferInfo pinfo); 
    void disasm(pCodeBufferInfo pinfo,std::vector<CodePiece> & code);
    void printf_piece(std::vector<CodePiece>& var_piece);
    pAssemblerTree add_tree(pAssemblerTree parent, pAssemblerTree child,bool l_r);
    pAssemblerTree disasm(pCodeBufferInfo pinfo);
    void set_label_address(pCodeBufferInfo pinfo,AddressArray & _addra,std::map<long,int> &_map);
    void link_tree();

    pAssemblerTree find_tree(long addr); //����һ����ַ
    pAssemblerTree get_tree(long id);
    void analysis_address_table(pCodeBufferInfo pinfo,std::vector<long> & addr_entry_table,long addr_min,long addr_max);
    void clean_tree();

    void set_eflag();

    bool check_address(long addr);
};
#endif /* _ANALYSIS_H_ */
