
#include "PEFile.h"
#pragma warning(disable:4312)

CPEFile::CPEFile()
{
	m_pFile = new MAP_FILE_STRUCT;
	memset(m_pFile,0,sizeof(MAP_FILE_STRUCT));
}

PMAP_FILE_STRUCT CPEFile::GetInfo()
{
	return m_pFile;
}

bool CPEFile::SetInfo(PMAP_FILE_STRUCT pMapFile)
{
	this->m_pFile = pMapFile;
	return true;
}

bool CPEFile::LoadPEFile(char *file_name)
{
	if (m_pFile->ImageBase)
    {
        delete [] m_pFile->ImageBase;
        m_pFile->ImageBase = NULL;
        m_pFile->Size = 0;
    }
	FILE *pFile;
    fopen_s( &pFile, file_name, "rb" );
	if (!pFile)
    {
        return false;
    }
	fseek(pFile,0,SEEK_END);
	int len = ftell(pFile);
	m_pFile->ImageBase = new unsigned char[len];
	rewind(pFile);
	fread(m_pFile->ImageBase,1,len,pFile);
	fclose(pFile);
	m_pFile->Size = len;
	return true;
}

bool CPEFile::LoadPEFile(void *file_buf,unsigned long size)
{
	if (m_pFile->ImageBase)
		{
			delete [] m_pFile->ImageBase;
			m_pFile->ImageBase = NULL;
			m_pFile->Size = 0;
		}
	m_pFile->ImageBase = new unsigned char[size+1];
	memcpy(m_pFile->ImageBase,file_buf,size);
	return true;
}

void CPEFile::Free()
{
	if (m_pFile->ImageBase)
		{
			delete [] m_pFile->ImageBase;
			m_pFile->ImageBase = NULL;
			m_pFile->Size = 0;
		}
	if (m_pFile)
		{
			delete [] m_pFile;
			m_pFile = NULL;
		}
}

void CPEFile::VacantBuf(unsigned long size)
{
	if (m_pFile->ImageBase)
		{
			delete [] m_pFile->ImageBase;
			m_pFile->ImageBase = NULL;
			m_pFile->Size = 0;
		}
	m_pFile->ImageBase = new unsigned char[size];
	memset(m_pFile->ImageBase,0,size);
	m_pFile->Size = size;
}

void *CPEFile::GetImage()  // done!
{
	return m_pFile->ImageBase;
}

PIMAGE_DOS_HEADER CPEFile::GetDosHeader()  // done!
{
	return (PIMAGE_DOS_HEADER)GetImage();
}

PIMAGE_NT_HEADERS32 CPEFile::GetNtHeader()  // done!
{
	PIMAGE_DOS_HEADER pDH = GetDosHeader();
	if (pDH)
		return (PIMAGE_NT_HEADERS32)((DWORD)GetImage()+pDH->e_lfanew);

	return NULL;
}

PIMAGE_OPTIONAL_HEADER32 CPEFile::GetNtOptionalHeader()  // done!
{
	PIMAGE_NT_HEADERS32 pNth=GetNtHeader();
	if (pNth)
		return (PIMAGE_OPTIONAL_HEADER32)&pNth->OptionalHeader;

	return NULL;
}

PIMAGE_DATA_DIRECTORY CPEFile::GetDataDirectory()
{
    PIMAGE_OPTIONAL_HEADER32 pOptionalHeader = GetNtOptionalHeader();
    if (pOptionalHeader)
        return (PIMAGE_DATA_DIRECTORY) pOptionalHeader->DataDirectory;

    return NULL;
}

PIMAGE_FILE_HEADER CPEFile::GetNtFileHeader()  // done!
{
	PIMAGE_NT_HEADERS32 pNth = GetNtHeader();
	if (pNth)
		return (PIMAGE_FILE_HEADER)&pNth->FileHeader;

	return NULL;
}

PIMAGE_DATA_DIRECTORY CPEFile::GetDataDirectory(int index)
{
	if (index>=0 && index<16)
	{
			PIMAGE_DATA_DIRECTORY pDataDir = GetDataDirectory();
			if(pDataDir)
			{
					return &pDataDir[index];
			}
	}
	return NULL;
}

int CPEFile::GetSectionCount()
{
	PIMAGE_FILE_HEADER pNtFileHeader = GetNtFileHeader();
	if (pNtFileHeader)
		{
			return pNtFileHeader->NumberOfSections;
		}
	return 0;
}

PIMAGE_SECTION_HEADER CPEFile::GetFirstSectionHeader()  // done!
{
	PIMAGE_NT_HEADERS32 pNth = GetNtHeader();
	if (pNth)
			return IMAGE_FIRST_SECTION(pNth);

	return NULL;
}

PIMAGE_SECTION_HEADER CPEFile::GetSection(int index)  // done!
{
	int nCount = GetSectionCount();
	if (index >= nCount || index < 0)
		return NULL;

	PIMAGE_SECTION_HEADER pFirstSection = GetFirstSectionHeader();
	for (int i = 0; i < nCount; i++,pFirstSection++ )
		if (i == index)
			return pFirstSection;
	return NULL;
}

bool CPEFile::IsPEFile()  // done!
{
	PIMAGE_DOS_HEADER pDosHeader = GetDosHeader();
	if (pDosHeader->e_magic == IMAGE_DOS_SIGNATURE)
    {
        PIMAGE_NT_HEADERS32 pNtHeader = GetNtHeader();
        if (pNtHeader->Signature == IMAGE_NT_SIGNATURE)
            return true;
    }
	return false;
}

void * CPEFile::GetDirectoryEntryToData(unsigned short DirectoryEntry)
{
	DWORD dwDataStartRVA;
	void * pDirData = NULL;
	PIMAGE_NT_HEADERS32 pNth = NULL;
	PIMAGE_OPTIONAL_HEADER32 pOh = NULL;

	pNth = GetNtHeader();
	if (!pNth)
		return NULL;
	dwDataStartRVA = GetDataDirectory(DirectoryEntry)->VirtualAddress;
	if (!dwDataStartRVA)
		return NULL;

	pDirData = RvaToPtr(dwDataStartRVA);
	if (!pDirData)
		return NULL;
	return pDirData;
}

PIMAGE_SECTION_HEADER
CPEFile::ImageRvaToSection(
    PIMAGE_NT_HEADERS32 NtHeader,
    void *  BaseAddress,
    ULONG Rva)
{
    PIMAGE_SECTION_HEADER Section;
    ULONG Va;
    ULONG Count;
	

    Count = NtHeader->FileHeader.NumberOfSections;
    Section = IMAGE_FIRST_SECTION(NtHeader);

    while (Count--)
    {
        Va = Section->VirtualAddress;
        if ( (Va <= Rva) &&
             (Rva < Va + Section->Misc.VirtualSize) )
            return Section;
        Section++;
    }
    return NULL;
}

void * CPEFile::ImageRvaToVa(PIMAGE_NT_HEADERS32 NtHeader,
					void * BaseAddress,
					ULONG Rva,
					PIMAGE_SECTION_HEADER *SectionHeader)
{
    PIMAGE_SECTION_HEADER Section = NULL;

    if (SectionHeader)
        Section = *SectionHeader;

    if (Section == NULL ||
            Rva <  Section->VirtualAddress ||
            Rva >= Section->VirtualAddress + Section->Misc.VirtualSize)
    {
        Section = ImageRvaToSection (NtHeader, BaseAddress, Rva);
        if (Section == NULL)
            return 0;

        if (SectionHeader)
            *SectionHeader = Section;
    }

    return (void *)((ULONG_PTR)BaseAddress + Rva +
                   Section->PointerToRawData - (ULONG_PTR)Section->VirtualAddress);
}

void * CPEFile::RvaToPtr(DWORD dwRVA)
{
	PIMAGE_NT_HEADERS32 pNth = GetNtHeader();
	return ImageRvaToVa(pNth,m_pFile->ImageBase,dwRVA,NULL);
}

void * CPEFile::VaToPtr(DWORD dwVA)
{
	PIMAGE_NT_HEADERS32 pNth = GetNtHeader();
	DWORD dwRVA = dwVA-GetNtOptionalHeader()->ImageBase;
	return ImageRvaToVa(pNth,m_pFile->ImageBase,dwRVA,NULL);
}

bool CPEFile::SavePEFile(char *save_file_name)
{
	FILE * pFile;
    fopen_s( &pFile, save_file_name, "wb" );
	if (pFile)
		{
			fwrite(m_pFile->ImageBase,m_pFile->Size,1,pFile);
			fclose(pFile);
			return true;
		}
	return false;
}
