
#include "PEFile.h"

CPEReloc::CPEReloc()
{

}

CPEReloc::~CPEReloc()
{

}

void CPEReloc::operator=(CPEFile &PeFile)
{
   SetInfo(PeFile.GetInfo());
}

DWORD CPEReloc::GetBaseReloc() 
{
   PIMAGE_OPTIONAL_HEADER32 NtOptionHead = GetNtOptionalHeader();
   IMAGE_DATA_DIRECTORY Roc = NtOptionHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
       return Roc.VirtualAddress;
}

void CPEReloc::DeleteReloc()
{
    PIMAGE_OPTIONAL_HEADER32 NtOptionHead = GetNtOptionalHeader();
    IMAGE_DATA_DIRECTORY DataReloc = NtOptionHead->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
    if (DataReloc.VirtualAddress == 0)
        return;
    PIMAGE_BASE_RELOCATION pLoc = (PIMAGE_BASE_RELOCATION)
        VaToPtr(NtOptionHead->ImageBase + DataReloc.VirtualAddress);
    pLoc->VirtualAddress = 0;
    pLoc->SizeOfBlock = 0;
}

