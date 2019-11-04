/*
 *   Copyright (C) Wed Jun 26 18:49:56 2013 XiaoWei
 *
 *                       handsomexiaowei@gmail.com
 */

#ifndef _VMCODEBUFFER_H_
#define _VMCODEBUFFER_H_

#include "PCode.hpp"
#include "CombosVMCode.hpp"
#include "VirtualMachine.h"

class VMCodeBufferManage
{
public:

  explicit VMCodeBufferManage( VirtualMachine *vm )
    :var_combos(&var_pcode)
  {
    if ( !vm )
      throw;
     var_pcode.init_handle_table(vm->get_vm_handle_table());
     var_pcode.init_sign( vm->get_vm_handle().sign );
  }
  
  VMCodeBufferManage( VirtualMachine *vm,
                      unsigned long key)
    :var_combos(&var_pcode)
  {
    if ( !vm )
      throw;
     var_pcode.init_handle_table(vm->get_vm_handle_table());
     var_pcode.init_sign( vm->get_vm_handle().sign );
     var_pcode.set_key(key);
  }

  virtual ~VMCodeBufferManage() {}
  
  VCombosVMCode var_combos;
  //
  VCombosVMCode & get_generator(  )
  {
    return var_combos;
  }

  PCode var_pcode;
  //
  PCode & get_pcode()
  {
    return var_pcode;
  }

  int var_label;
  //
  void set_vmcode_label(int _label)
  {
     var_label = _label;
  }
};

#endif /* _VMCODEBUFFER_H_ */
