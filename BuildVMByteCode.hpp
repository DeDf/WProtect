
#ifndef BUILDVMBYTECODE_HPP
#define BUILDVMBYTECODE_HPP

#include "CombosVMCode.hpp"
#include "VMAddressTable.hpp"
#include "VMCodeBuffer.hpp"
#include "Analysis.hpp"
#include <vector>

class BuildVMByteCode {
public:
    enum ProtectType{
        ComplexIns, //Complex Instruction Set Computer
        ReducudIns, //Reduced Instruction Set Computer
                           //��������ָ��û����������
        UntimateReducedIns //Ultimate Reduced Instruction Set Computer
    };

    VirtualMachine * pVM;
    VMAddressTable * ptr_addr_table;
    std::map<ud_type,long> mapped_vm_register;
    int newlabel_count;
    std::map<int,RegisterStore> var_map_label_vmreg_store;
    std::map<int,RegisterStore> var_map_label_vmreg_store_in;  
    std::map<int,RegisterStore> var_map_label_vmreg_store_out;  
    std::map<int,long>  var_map_label_vmkey;          //��ǩ��Ӧ��vmkey
    std::map<int,long>  var_map_label_vmcode_addr;    //��ǩ��Ӧ�ķ����vmcode��ַ 
    std::map<int,long>  var_map_newlabel_vmcode_addr;
    std::map<int,VMCodeBufferManage *> var_map_label_vmbuffer_manage;
    std::map<int,VMCodeBufferManage *> var_map_newlabel_vmbuffer_manage;
    std::vector<long*> var_entry_address;
    std::vector<CodePiece> var_list_code_piece;  
    std::vector<VMCodeBufferManage *> var_vmcode_manager_list; 

    BuildVMByteCode(VirtualMachine *vm,
        pCodeBufferInfo ptr_info,
        VMAddressTable *ptr_address_table,
        std::vector<long *> & entry_address);

    ~BuildVMByteCode();

    long get_newvm_cur_label();
    VMCodeBufferManage * get_newvm_piece();
    VMCodeBufferManage * create_newvm_piece();
    
    void build_pfx(VCombosVMCode & var_combos_vm_code,ud_t &var_ud,CodePiece &var_cur_piece,bool b_allocator);
    void build_fpu(VCombosVMCode & var_combos_vm_code,ud_t &var_ud);
    void build(VCombosVMCode & var_combos_vm_code,ud_t & var_ud);
    long build_vmcode(bool b_allocator);

    long create_vm_entry(int _label);
    long get_vmcode_address(int _label_index);

    void set_lastpiece_register_store(std::vector<CodePiece> &var_list_code_piece,
        std::map<int,RegisterStore> & var_map_label_vmreg_store);
    void set_register_store(std::vector<CodePiece> &var_list_code_piece,
        std::map<int,RegisterStore> & var_map_label_vmreg_store_in,
        std::map<int,RegisterStore> & var_map_label_vmreg_store_out,
        std::map<int,RegisterStore> & var_map_lastpiece_vmreg_store);   
    void full_register_store(std::vector<CodePiece> &var_list_code_piece,
        std::map<int,RegisterStore> & var_map_label_vmreg_store_in,
        std::map<int,RegisterStore> & var_map_label_vmreg_store_out);    
    void set_vmregister_store(std::vector<CodePiece> &var_list_code_piece,
        std::map<int,RegisterStore> & var_map_label_vmreg_store_in,
        std::map<int,RegisterStore> & var_map_label_vmreg_store_out,
        int var_cur_label); 
    void full_vmregister_store(std::vector<CodePiece> &var_list_code_piece,
        std::map<int,RegisterStore> & var_map_label_vmreg_store_in,
        std::map<int,RegisterStore> & var_map_label_vmreg_store_out);

    void vm_operand(VCombosVMCode & var_combos_vm_code,ud_operand & var_operand);
    void write_vm_operand(VCombosVMCode & var_combos_vm_code,ud_operand & var_operand); 
    void read_vm_operand(VCombosVMCode & var_combos_vm_code,ud_operand & var_operand);

    void copy_exec_code_to_stack(VCombosVMCode & var_combos_vm_code,
        const uint8_t * var_code_buffer,
        size_t var_code_size);

    void register_mapped_init();

    long get_vm_register(ud_type _register);
};

#endif /* BUILDVMBYTECODE_HPP */
