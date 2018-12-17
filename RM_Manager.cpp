#include "stdafx.h"
#include "RM_Manager.h"
#include "str.h"
#include <string>


RC OpenScan(RM_FileScan *rmFileScan,RM_FileHandle *fileHandle,int conNum,Con *conditions)//��ʼ��ɨ��
{
	rmFileScan->bOpen = true;
	rmFileScan->conNum = conNum;
	rmFileScan->bOpen = conditions;
	rmFileScan->pRMFileHandle = fileHandle;
	return SUCCESS;
}

RC GetNextRec(RM_FileScan *rmFileScan,RM_Record *rec)
{
	return SUCCESS;
}

//��ȡָ���ļ��б�ʶ��Ϊrid�ļ�¼���ݵ�recָ��ļ�¼�ṹ�С�
RC GetRec (RM_FileHandle *fileHandle,RID *rid, RM_Record *rec) 
{
	if (rid->pageNum > fileHandle->PFhandle.pFileSubHeader->pageCount)//����βҳҳ��
		return PF_INVALIDPAGENUM;
	if((fileHandle->pBitmap[rid->pageNum/8]&(1<<(rid->pageNum%8)))==0)
		return PF_INVALIDPAGENUM;
	if (rid->slotNum > fileHandle->pFileSubHeader->recordsPerPage)
		return RM_INVALIDRID;

	PF_PageHandle pageHandle;
	GetThisPage(&(fileHandle->PFhandle), rid->pageNum, &pageHandle);
	return SUCCESS;
}

RC InsertRec (RM_FileHandle *fileHandle,char *pData, RID *rid)
{
	return SUCCESS;
}

RC DeleteRec (RM_FileHandle *fileHandle,const RID *rid)
{
	return SUCCESS;
}

RC UpdateRec (RM_FileHandle *fileHandle,const RM_Record *rec)
{
	return SUCCESS;
}

RC RM_CreateFile (char *fileName, int recordSize)//������¼�ļ�
{
	if ( recordSize <= 0 || recordSize > PF_PAGE_SIZE) return RM_INVALIDRECSIZE;//��¼��С�Ƿ�
	RC ret = CreateFile(fileName);//�ȴ�����ҳ�ļ�
	if ( ret != SUCCESS) return ret;

	int fd = open(fileName, _O_RDWR);
	Page page;
	memset(&page,0,PF_PAGESIZE);
	page.pageNum = 1;

	RM_FileSubHeader * pFileSubHeader;
	char * pBitmap;
	pBitmap = page.pData + (int)sizeof(RM_FileSubHeader);
	pFileSubHeader = (RM_FileSubHeader *)page.pData;
	pFileSubHeader->recordSize = recordSize;//ÿ����¼��С
	int recordCount = (double(PF_PAGE_SIZE))/(double(recordSize) + 1.0/8);//����ÿҳ��¼��
	pFileSubHeader->recordsPerPage = recordCount;//ÿ��ҳ�������ɵļ�¼��
	pFileSubHeader->firstRecordOffset = PF_PAGE_SIZE - recordCount * recordSize;//��һ����¼������������ƫ����
	pBitmap[0] |= 0x03;//���ü�¼�ļ�ҳ��λͼ

	if ( _lseek(fd, 0, SEEK_END) == -1) return PF_FILEERR;//���ļ�ָ���ƶ����ļ�β
	if(_write(fd,(char *)&page,sizeof(Page)) != sizeof(Page)){//����¼��Ϣ����ҳд���ļ�
		_close(fd);
		return PF_FILEERR;
	}

	Page ppage;
	memset(&ppage,0,PF_PAGESIZE);

	char * bitmap;
	PF_FileSubHeader * fileSubHeader;
	
	bitmap=ppage.pData+(int)PF_FILESUBHDR_SIZE;
	fileSubHeader=(PF_FileSubHeader *)ppage.pData;
	fileSubHeader->pageCount = 1;
	fileSubHeader->nAllocatedPages = 2;
	bitmap[0]|=0x03;
	if(_lseek(fd,0,SEEK_SET)==-1) 
		return PF_FILEERR;
	if(_write(fd,(char *)&ppage,sizeof(Page))!=sizeof(Page)){
		_close(fd);
		return PF_FILEERR;
	}
	if(_close(fd)<0)
		return PF_FILEERR;

	return SUCCESS;
}

RC RM_OpenFile(char *fileName, RM_FileHandle *fileHandle)
{
	PF_PageHandle pageHandle;

	const RC ret0 = openFile(fileName, &(fileHandle->PFhandle));//ȡ�÷�ҳ�ļ��������0ҳ��
	if (ret0 != SUCCESS)
		return ret0;
	fileHandle->bOpen = true;
	const RC ret1 = GetThisPage(&fileHandle->PFhandle, 1, &pageHandle);//��ȡ��¼��Ϣ����ҳ����һҳ��
	if (ret1 != SUCCESS)
		return ret1;

	fileHandle->pHdrFrame = pageHandle.pFrame;//���ü�¼����ҳָ֡��
	fileHandle->pHdrPage = &(fileHandle->pHdrFrame->page);//���ü�¼����ҳָ��
	fileHandle->pBitmap = fileHandle->pHdrPage->pData + sizeof(RM_FileSubHeader);//���ü�¼��Ϣ����ҳλͼָ��
	fileHandle->pFileSubHeader = (RM_FileSubHeader *)fileHandle->pHdrPage->pData;

	return SUCCESS;
}

RC RM_CloseFile(RM_FileHandle *fileHandle)
{
	return CloseFile(&(fileHandle->PFhandle));
}
