/*
 *   Copyrigth (C) Thu May 23 00:32:50 2013 XiaoWei
 *
 *                       handsomexiaowei@gmail.com
 */

#ifndef _VIRTUALMACHINEMANAGE_H_
#define _VIRTUALMACHINEMANAGE_H_

#include <list>
#include "VirtualMachine.h"
#include "PCode.hpp"

class VirtualMachineManage
{
public:
    list <VirtualMachine *> virtual_machine_list;
    int virtual_machine_count;
    //
    VirtualMachineManage();
    virtual ~VirtualMachineManage();
    VirtualMachine *  add_virtual_machine(long base,bool sign = false);
    VirtualMachine * rand_virtual_machine();
    void moc(VirtualMachine *pvm,PCode *code);
};

#endif /* _VIRTUALMACHINEMANAGE_H_ */
