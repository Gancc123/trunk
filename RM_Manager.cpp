#include "stdafx.h"
#include "RM_Manager.h"
#include "str.h"
#include <string>


RC OpenScan(RM_FileScan *rmFileScan,RM_FileHandle *fileHandle,int conNum,Con *conditions)//初始化扫描
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

//获取指定文件中标识符为rid的记录内容到rec指向的记录结构中。
RC GetRec (RM_FileHandle *fileHandle,RID *rid, RM_Record *rec) 
{
	if (rid->pageNum > fileHandle->PFhandle.pFileSubHeader->pageCount)//大于尾页页号
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

RC RM_CreateFile (char *fileName, int recordSize)//创建记录文件
{
	if ( recordSize <= 0 || recordSize > PF_PAGE_SIZE) return RM_INVALIDRECSIZE;//记录大小非法
	RC ret = CreateFile(fileName);//先创建分页文件
	if ( ret != SUCCESS) return ret;

	int fd = open(fileName, _O_RDWR);
	Page page;
	memset(&page,0,PF_PAGESIZE);
	page.pageNum = 1;

	RM_FileSubHeader * pFileSubHeader;
	char * pBitmap;
	pBitmap = page.pData + (int)sizeof(RM_FileSubHeader);
	pFileSubHeader = (RM_FileSubHeader *)page.pData;
	pFileSubHeader->recordSize = recordSize;//每条记录大小
	int recordCount = (double(PF_PAGE_SIZE))/(double(recordSize) + 1.0/8);//计算每页记录数
	pFileSubHeader->recordsPerPage = recordCount;//每个页面能容纳的记录数
	pFileSubHeader->firstRecordOffset = PF_PAGE_SIZE - recordCount * recordSize;//第一条记录的在数据区的偏移量
	pBitmap[0] |= 0x03;//设置记录文件页面位图

	if ( _lseek(fd, 0, SEEK_END) == -1) return PF_FILEERR;//将文件指针移动到文件尾
	if(_write(fd,(char *)&page,sizeof(Page)) != sizeof(Page)){//将记录信息控制页写入文件
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

	const RC ret0 = openFile(fileName, &(fileHandle->PFhandle));//取得分页文件句柄（第0页）
	if (ret0 != SUCCESS)
		return ret0;
	fileHandle->bOpen = true;
	const RC ret1 = GetThisPage(&fileHandle->PFhandle, 1, &pageHandle);//读取记录信息控制页（第一页）
	if (ret1 != SUCCESS)
		return ret1;

	fileHandle->pHdrFrame = pageHandle.pFrame;//设置记录控制页帧指针
	fileHandle->pHdrPage = &(fileHandle->pHdrFrame->page);//设置记录控制页指针
	fileHandle->pBitmap = fileHandle->pHdrPage->pData + sizeof(RM_FileSubHeader);//设置记录信息控制页位图指针
	fileHandle->pFileSubHeader = (RM_FileSubHeader *)fileHandle->pHdrPage->pData;

	return SUCCESS;
}

RC RM_CloseFile(RM_FileHandle *fileHandle)
{
	return CloseFile(&(fileHandle->PFhandle));
}
