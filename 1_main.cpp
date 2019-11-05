
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#pragma warning(disable:4311)
#include "AsmJit/AsmJit.h"
#define _STDINT_H
#include "Libudis86/udis86.h"
#include "PE/PEFile.h"
//
#include "algorithms.hpp"
#include "Analysis.hpp"
#include "VirtualMachine.h"
#include "VMHandle.h"
#include "CombosVMCode.hpp"
#include "PCode.hpp"
#include "SeniorVMHandle.hpp"
#include "BuildCode.hpp"
#include "VMAddressTable.hpp"
#include "BuildVMByteCode.hpp"

void add_jmp_addr(CPEFile pe,long base,long jmp_address)
{
    char * c = (char*)pe.VaToPtr(base);
    *(char*)c = (char)0xe9;
    *(long*)(c + 1) = jmp_address - base - 5;
}

void get_wprotect_sdk_address(CPESection & section,
                              BuildExeInfo & build_info,
                              char *sz_sdk_begin_name,
                              char *sz_sdk_end_name)
{
    int sdk_begin_count       = 0;
    int sdk_end_count         = 0;
    int protect_begin_address = 0;
    int protect_end_address   = 0;
    int section_count         = section.GetSectionCount();
    int sdk_begin_str_size    = (int)strlen(sz_sdk_begin_name) + 1;
    int sdk_end_str_size      = (int)strlen(sz_sdk_end_name)   + 1;

    for (int i = 0; i < section_count; i++)  // 每个区段
    {
        DWORD section_size;
        BYTE * p = section.GetSectionData(i, &section_size);

        for (DWORD offset = 0; offset+1 < section_size; offset++)
        {
            if ( !(section.GetCharacteristics(i) & 0x20000000) )  // IMAGE_SCN_MEM_EXECUTE
                continue;

            if (p[offset] == 0xeb)
            {
                if (p[offset+1] == sdk_begin_str_size &&
                    offset+2 + sdk_begin_str_size < section_size)
                {
                    if (!strcmp(sz_sdk_begin_name, (char*)p+offset+2))
                    {
                        sdk_begin_count++;
                        if (sdk_begin_count - sdk_end_count == 1)
                        {
                            memset(p+offset, 0x90, 2+sdk_begin_str_size);
                            protect_begin_address = (ULONG)(p+offset);
                        }
                        else
                        {
                            printf("sdk_begin_count Error!\n");
                        }
                    }
                }
                else if (p[offset+1] == sdk_end_str_size &&
                    offset+2 + sdk_end_str_size < section_size)
                {
                    if (!strcmp(sz_sdk_end_name, (char*)p+offset+2))
                    {
                        sdk_end_count++;
                        if (sdk_begin_count == sdk_end_count)
                        {
                            protect_end_address = (ULONG)(p+offset);
                            memset(p+offset, 0x90, 2+sdk_end_str_size);

                            Code_Piece piece;
                            piece.CodeStartAddr =
                                section.GetSectionVa(i, protect_begin_address - (ULONG)p);

                            piece.CodeSize = protect_end_address - protect_begin_address;
                            build_info.push_back(piece);

                            printf("保护地址%x - %x\n",
                                piece.CodeStartAddr,
                                piece.CodeStartAddr+piece.CodeSize);
                        }
                        else
                        {
                            printf("sdk_end_count Error!\n");
                        }
                    }
                }
            }
        }
    }
    if (sdk_begin_count != sdk_end_count)
    {
        printf("need last sdk_end_count!\n");
        exit(-1);
    }
}

void buildvm_test(BuildExeInfo & build_info)
{
    srand( (unsigned int)time( NULL ) );

    char * build_exec_name = build_info.get_filename();
    printf("待处理文件：%s\n", build_exec_name);

    CPEFile file;
    if (!file.LoadPEFile(build_exec_name))
    {
        printf("file is not find!\n");
        return;
    }

    CPEReloc reloc;
    reloc = file;
    reloc.DeleteReloc();

    CPESection section;
    section = file;
    get_wprotect_sdk_address(section,build_info,"WProtect Begin","WProtect End");
    printf("\n");

    ULONG NewSectionBase = section.GetNewSectionBase();
    printf("NewSectionBase : %x\n", NewSectionBase);

    VMAddressTable table( NewSectionBase, 1024, false );
    bool t_sign = table.get_sign();
    table.set_sign(true);
    long vm_address = table.assign_address(4096);
    table.set_sign(t_sign);

    VirtualMachine *pvm = new VirtualMachine(vm_address, false);
    //
    printf("vm_address : %x\n", vm_address);

    table.copy(vm_address, pvm->vm_info.buf, pvm->vm_info.size);

    CodeBufferInfo Code;
    for (BuildExeInfo::iterator iter = build_info.begin();
        iter != build_info.end();
        iter++)
    {
        long CodeStartAddr = iter->CodeStartAddr;

        Code.buf  = file.VaToPtr(CodeStartAddr);
        Code.addr = CodeStartAddr;
        Code.size = iter->CodeSize;
        if (Code.size < 5)
        {
            printf("编译内容不能小于5Byte,容不下一个跳转\n");
            return;
        }

        Analysis analysis;
        std::vector<long> addr_table;
        //
        analysis.analysis_address_table(&Code,
            addr_table,
            section.GetSectionMinAddress(),
            section.GetSectionMaxAddress());

        BuildVMByteCode build(pvm, &Code, &table);
        memset(Code.buf, 0, Code.size);                // 旧代码置零
        add_jmp_addr(file, CodeStartAddr, Code.addr);  // 旧代码处修改为jmp Code.addr
    }

<<<<<<< HEAD
=======
#ifdef _DEBUG
    FILE *pfile;
    fopen_s( &pfile, "virtualmachine", "wb" );
    fwrite( vm->vm_info.buf, 1, vm->vm_info.size, pfile );
    fclose( pfile );
#endif

>>>>>>> 90d34fde52f92c4126714c7704493f0c90f707e2
    unsigned long section_size = (unsigned long)(table.buffer_size);
    section.AddSection(".WPro", section_size, 0xE0000020);
    section.WriteSectionData(file.GetSectionCount()-1,
        0,
        (unsigned char*)table.buffer,
        (unsigned long )table.buffer_size);

    char new_file_name[256];
    memcpy(new_file_name, build_exec_name, strlen(build_exec_name)-3); 
    strcat_s(new_file_name,256,"wp.exe");
    printf("Out File:%s\n", new_file_name);
    file.SavePEFile(new_file_name);
}

int main()
{
    BuildExeInfo build_pe("movzx_test.exe");
    buildvm_test(build_pe);
    getchar();
    return 0;
}