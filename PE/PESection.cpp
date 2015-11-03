
#include "PEFile.h"

#define  ZALIGN(x,a)(((x-1)/a+1)*a)

CPESection::CPESection(void)
{
}

void CPESection::operator=(CPEFile& PeFile)
{
	SetInfo(PeFile.GetInfo());
}

int CPESection::RvaToSectionIndex(DWORD dwRva)
{
	int iSectionIndex=-1;
	int iSectionCount=GetSectionCount();
	for (int i=0;i<iSectionCount;i++)
	{
		PIMAGE_SECTION_HEADER pTempSectionHeader=GetSection(i);
		if (pTempSectionHeader->VirtualAddress<=dwRva)
		{
			if (dwRva<=pTempSectionHeader->Misc.VirtualSize+pTempSectionHeader->VirtualAddress)
			{
				iSectionIndex=i;
				break;
			}
		}
	}
	return iSectionIndex;
}

int CPESection::OffsetToSectionIndex(DWORD dwOffset)
{
	int iSectionIndex=0;
	int iSectionCount=GetSectionCount();
    for (int i=0;i<iSectionCount;i++)
	{
		PIMAGE_SECTION_HEADER pTempSectionHeader=GetSection(i);
		if (pTempSectionHeader->PointerToRawData<dwOffset)
		{
			if (dwOffset<=pTempSectionHeader->PointerToRawData+pTempSectionHeader->SizeOfRawData)
			{
				iSectionIndex=i;
				break;
			}
		}
	}
	return iSectionIndex;
}

DWORD CPESection::GetVRk(int nSeciotnIndex)
{
	PIMAGE_SECTION_HEADER pTempSectionHeader=GetSection(nSeciotnIndex);
	DWORD dwVRk=pTempSectionHeader->VirtualAddress-pTempSectionHeader->PointerToRawData;
	return dwVRk;
}

DWORD CPESection::RvaToOffset(DWORD dwRva)
{
	int nSectionIdex=RvaToSectionIndex(dwRva);
	DWORD dwVRk=GetVRk(nSectionIdex);
	return dwRva-dwVRk;
}

DWORD CPESection::OffsetToRva(DWORD dwOffset)
{
	int nSectionIndex=OffsetToSectionIndex(dwOffset);
	DWORD dwVRk=GetVRk(nSectionIndex);
	return dwVRk+dwOffset;
}

DWORD CPESection::VaToOffset(DWORD dwVa)
{
	return RvaToOffset(dwVa-GetNtOptionalHeader()->ImageBase);
}

DWORD CPESection::RvaToVa(DWORD dwRva)
{
	return dwRva+GetNtOptionalHeader()->ImageBase;
}

BOOL CPESection::AddSection(char *pSectionName, DWORD dwSectionSize, DWORD dwSectionCharacteristics)
{
	LPVOID lPointer = NULL;
	PBYTE  pData    = NULL;

	if (!IsPEFile())
	{
		return false;
	}

	PIMAGE_NT_HEADERS32 pNTHdr=GetNtHeader();
	if ((pNTHdr->FileHeader.NumberOfSections+1)*sizeof(IMAGE_SECTION_HEADER) > pNTHdr->OptionalHeader.SizeOfHeaders)
	{
		return FALSE;
	}

	DWORD uCodeDelta=ZALIGN(dwSectionSize,pNTHdr->OptionalHeader.SectionAlignment);
	DWORD uFileDelta=ZALIGN(dwSectionSize,pNTHdr->OptionalHeader.FileAlignment);

	PIMAGE_SECTION_HEADER pNewSec =(PIMAGE_SECTION_HEADER)(pNTHdr+1)+pNTHdr->FileHeader.NumberOfSections;
	PIMAGE_SECTION_HEADER pLaseSec=pNewSec-1;

	strcpy_s((char*)pNewSec->Name, IMAGE_SIZEOF_SHORT_NAME, pSectionName);
	pNewSec->VirtualAddress   = pLaseSec->VirtualAddress+ZALIGN(pLaseSec->Misc.VirtualSize, pNTHdr->OptionalHeader.SectionAlignment);
	pNewSec->PointerToRawData = pLaseSec->PointerToRawData+pLaseSec->SizeOfRawData;
	pNewSec->Misc.VirtualSize = dwSectionSize;
	pNewSec->SizeOfRawData    = uFileDelta;
	pNewSec->Characteristics  = dwSectionCharacteristics;

	pNTHdr->FileHeader.NumberOfSections++;
	pNTHdr->OptionalHeader.SizeOfCode  += uFileDelta;
	pNTHdr->OptionalHeader.SizeOfImage += uCodeDelta;

	pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size=0;
	pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress=0;

	//žÄ±ä×ÔÉíÊýŸÝ
	DWORD dwOldSize = m_pFile->Size;
	m_pFile->Size += uFileDelta;
	
	BYTE*lpNewImageBase=new BYTE[m_pFile->Size];
	memset(lpNewImageBase,0,m_pFile->Size);
	memcpy(lpNewImageBase,m_pFile->ImageBase,dwOldSize);
	delete [] m_pFile->ImageBase;
	m_pFile->ImageBase=lpNewImageBase;

	return TRUE;
}

BYTE* CPESection::GetSectionData(int nIndex, DWORD *rdwSize)
{
	BYTE* lpBuffer=NULL;
	PIMAGE_SECTION_HEADER pSection=GetSection(nIndex);
	if (pSection->SizeOfRawData>0)
	{
		DWORD dwFileOffset=pSection->PointerToRawData;
		*rdwSize=pSection->SizeOfRawData;
		lpBuffer = (BYTE*)GetImage()+dwFileOffset;
    }

	return lpBuffer;
}

BOOL CPESection::WriteSectionData(int nIndex, DWORD dwOffset, BYTE *lpBuffer, DWORD dwSize)
{
	BOOL bIsOK=false;
	DWORD dwSectionSize;
	BYTE *lpMyBuffer = GetSectionData(nIndex,&dwSectionSize);

	if ( lpMyBuffer && dwSectionSize >= (dwSize+dwOffset) )
	{
		lpMyBuffer=lpMyBuffer+dwOffset;
		memcpy(lpMyBuffer,lpBuffer,dwSize);
		bIsOK=TRUE;
	}
	return bIsOK;
}

DWORD CPESection::GetSectionPtr(int nIndex, DWORD dwOffset)
{
	BOOL bIsOK=false;
	DWORD dwSectionSize;
	BYTE* lpMyBuffer=NULL;
	lpMyBuffer=GetSectionData(nIndex,&dwSectionSize);
	if (lpMyBuffer!=NULL&&dwSectionSize>=dwOffset)
	{
		lpMyBuffer=lpMyBuffer+dwOffset;
		return (DWORD)lpMyBuffer;
	}
	return 0;
}


DWORD CPESection::GetSectionVa(int nIndex, DWORD dwOffset)
{
    BOOL bIsOK=false;
    DWORD dwSectionSize;
    BYTE* p=GetSectionData(nIndex,&dwSectionSize);

    if (p && dwSectionSize>=dwOffset)
    {
        p=p+dwOffset;
        DWORD rva = OffsetToRva( (DWORD)p - (DWORD)GetImage());
        return RvaToVa( rva );
    }
    return 0;
}

// ����һ���½ڲ���������ʼVA
DWORD CPESection::GetNewSectionBase()
{
	PIMAGE_NT_HEADERS32 pNTHdr=GetNtHeader();
	PIMAGE_SECTION_HEADER pNewSec=(PIMAGE_SECTION_HEADER)(pNTHdr+1)+pNTHdr->FileHeader.NumberOfSections;
	PIMAGE_SECTION_HEADER pLastSec=pNewSec-1;
	DWORD newSectionAddr =
        pLastSec->VirtualAddress + ZALIGN(pLastSec->Misc.VirtualSize,pNTHdr->OptionalHeader.SectionAlignment);
	return RvaToVa(newSectionAddr);
}


DWORD CPESection::GetSectionMaxAddress()
{
    int i = GetSectionCount() - 1;
    PIMAGE_SECTION_HEADER psection = GetSection(i);
    //return GetSectionVa(i,psection->SizeOfRawData);
    return RvaToVa(psection->VirtualAddress);
}

DWORD CPESection::GetSectionMinAddress()
{
    PIMAGE_SECTION_HEADER psection = GetSection(0);
    //return GetSectionVa(0,psection->SizeOfRawData);
    return RvaToVa(psection->VirtualAddress);
}

BOOL CPESection::CheckAddressValidity(DWORD Addr)
{
   if (Addr <= GetSectionMaxAddress() && Addr >= GetSectionMinAddress())
   {
       return TRUE;
   }
   return FALSE;
}

// ��ȡ�ýڽ�ͷ�е�Characteristics
DWORD CPESection::GetCharacteristics(int i)
{
    return GetSection(i)->Characteristics;
}
