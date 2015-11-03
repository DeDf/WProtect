
#pragma once
#include <stdio.h>
#include <windows.h>
#pragma warning(disable:4311)

typedef struct _MAP_FILE_STRUCT
{
	void *ImageBase;
	unsigned long Size;
}MAP_FILE_STRUCT,*PMAP_FILE_STRUCT;

class CPEFile
{
public:
	CPEFile();
	bool LoadPEFile(char * file_name);
	bool LoadPEFile(void * file_buf,unsigned long size);
	bool IsPEFile();
	void Free();
	void VacantBuf(unsigned long size);
	MAP_FILE_STRUCT * m_pFile;
	void * GetImage();
	PIMAGE_DOS_HEADER GetDosHeader();
	PIMAGE_NT_HEADERS32 GetNtHeader();
	PIMAGE_FILE_HEADER GetNtFileHeader();
    PIMAGE_OPTIONAL_HEADER32 GetNtOptionalHeader();
	PIMAGE_DATA_DIRECTORY GetDataDirectory();
	PIMAGE_DATA_DIRECTORY GetDataDirectory(int index);
	int GetSectionCount();
	PIMAGE_SECTION_HEADER GetFirstSectionHeader();
    bool SavePEFile(char *save_file_name);
	PMAP_FILE_STRUCT GetInfo();
	bool SetInfo(PMAP_FILE_STRUCT pMapFile);
	PIMAGE_SECTION_HEADER GetSection(int index);
	void * GetDirectoryEntryToData(unsigned short DirectoryEntry);
	PIMAGE_SECTION_HEADER
    ImageRvaToSection(
    PIMAGE_NT_HEADERS32 NtHeader,
    void * BaseAddress,
    ULONG Rva);
	void * ImageRvaToVa(PIMAGE_NT_HEADERS32 NtHeader,
					void * BaseAddress,
					ULONG Rva,
					PIMAGE_SECTION_HEADER *SectionHeader);
	void * RvaToPtr(DWORD dwRVA);
	void * VaToPtr(DWORD dwVA);
};

class CPESection : public CPEFile
{
public:
    CPESection();
public:
    void operator=(CPEFile &PeFile);
    int RvaToSectionIndex(DWORD dwRva);
    int OffsetToSectionIndex(DWORD dwOffset);
    DWORD GetVRk(int nSectionIndex);
    DWORD RvaToOffset(DWORD dwRva);
    DWORD OffsetToRva(DWORD dwOffset);
    DWORD RvaToVa(DWORD dwRva);
    BOOL AddSection(char *pSectionName, DWORD dwSectionSize, DWORD dwSectionCharacteristics);
    BYTE* GetSectionData(int nIndex, DWORD *rdwSize);
    BOOL WriteSectionData(int nIndex, DWORD dwOffset, BYTE *lpBuffer, DWORD dwSize);
    DWORD VaToOffset(DWORD dwVa);
    DWORD GetSectionVa(int nIndex, DWORD dwOffset);
    DWORD GetSectionPtr(int nIndex, DWORD dwOffset);

    DWORD GetNewSectionBase();              // 创建一个新节并返回其起始VA
    DWORD GetSectionMaxAddress();
    DWORD GetSectionMinAddress();
    BOOL CheckAddressValidity(DWORD Addr);
    DWORD GetCharacteristics(int i);        // 获取该节节头中的Characteristics
};

class CPEImport : public CPEFile
{
public:
    CPEImport(void);
    ~CPEImport(void);
public:
    void operator =(CPEFile& lPeFile);
    PIMAGE_IMPORT_DESCRIPTOR GetFirstImportDesc();
    int GetImportDesCount();
    PIMAGE_IMPORT_DESCRIPTOR GetImportDesc(int index);
    PIMAGE_IMPORT_BY_NAME GetImportFucByName(DWORD RavThunk);
    DWORD GetImportThunk(PIMAGE_IMPORT_DESCRIPTOR pImportDesc);
    BOOL ReLocalImport(DWORD dwRVA);
    PIMAGE_IMPORT_DESCRIPTOR ForceGetImportDesc(int index);
};

class CPEExport : public CPEFile
{
public:
    CPEExport(void);
    ~CPEExport(void);
public:
    void operator =(CPEFile& PeFile);
    PIMAGE_EXPORT_DIRECTORY GetExportTab();
    int GetFuncCount();
    int GetNameCount();
    DWORD GetFirstFunc();
    DWORD GetFunc(int nIndex);
    DWORD GetFuncValue(int nIndex);
    DWORD GetFirstName();
    DWORD GetName(int nIndex);
    char* GetNameValue(int nIndex);
    DWORD GetFirstNameOrd();
    DWORD GetNameOrd(int nIndex);
    WORD GetNameOrdValue(int nIndex);
};

class CPEReloc : public CPEFile
{
public:
    CPEReloc();
    ~CPEReloc();
public:
    void operator=(CPEFile &PeFile);
    DWORD GetBaseReloc();
    void DeleteReloc();
};