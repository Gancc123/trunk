#include "stdafx.h"
#include "IX_Manager.h"

RC CreateIndex(const char * fileName,AttrType attrType,int attrLength)
{
	RC ret ;
	ret = CreateFile(fileName);
	if(ret != SUCCESS) return ret;
	
	int fd;
	char *bitmap;
	PF_FileSubHeader *fileSubHeader;
	fd=open(fileName,_O_RDWR);
	Page page;
	memset(&page,0,PF_PAGESIZE);
	bitmap=page.pData+(int)PF_FILESUBHDR_SIZE;
	fileSubHeader=(PF_FileSubHeader *)page.pData;
	fileSubHeader->pageCount = 1;
	fileSubHeader->nAllocatedPages=2;
	bitmap[0]|=0x03;
	if(_lseek(fd,0,SEEK_SET)==-1)
		return PF_FILEERR;
	if(_write(fd,(char *)&page,sizeof(Page))!=sizeof(Page)){
		_close(fd);
		return PF_FILEERR;
	}

	char *bitmap1;
	IX_FileHeader *ixfileheader;
	Page page1;
	memset(&page1,0,PF_PAGESIZE);
	page1.pageNum= 1;
	bitmap1=page1.pData+(int)IX_FILEHEADER_SIZE;
	ixfileheader=(IX_FileHeader *)page1.pData;
	ixfileheader->attrLength = attrLength;
	ixfileheader->attrType = attrType;

	if(_lseek(fd,0,SEEK_END)==-1)
		return PF_FILEERR;
	if(_write(fd,(char *)&page1,sizeof(Page))!=sizeof(Page)){
		_close(fd);
		return PF_FILEERR;
	}
	if(_close(fd)<0)
		return PF_FILEERR;
	return SUCCESS;
}

RC OpenIndex(const char *fileName,IX_IndexHandle *indexHandle)
{
	RC ret;
	char * ixfilename = new char[1000];
	strcpy(ixfilename,fileName);
	ret = openFile(ixfilename,&indexHandle->fileHandle);
	delete[] ixfilename;
	if (ret != SUCCESS) 
		return ret;
	return SUCCESS;
}

RC CloseIndex(IX_IndexHandle *indexHandle)
{
	return  CloseFile(&indexHandle->fileHandle);
}

RC OpenIndexScan(IX_IndexScan *indexScan,IX_IndexHandle *indexHandle,CompOp compOp,char *value){
	return SUCCESS;
}

RC IX_GetNextEntry(IX_IndexScan *indexScan,RID * rid){
	return SUCCESS;
}

RC CloseIndexScan(IX_IndexScan *indexScan){
		return SUCCESS;
}

RC GetIndexTree(char *fileName, Tree *index){
		return SUCCESS;
}



