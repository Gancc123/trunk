#include "stdafx.h"
#include "EditArea.h"
#include "SYS_Manager.h"
#include "QU_Manager.h"
#include "RM_Manager.h"
#include <string>
#include <iostream>
#include <Windows.h>
#include "RC.h"
#include <vector>
//创建一个vector用来保存句柄*filehandle
std::vector<RM_FileHandle *> vec;
/*
vec.push_back(E e);//插入
for(std::vector<RM_FileHandle *>::size_type i = 0; i < vec.size(); i++){//遍历
	RM_CloseFile(vec[i]);
}
*/
void ExecuteAndMessage(char * sql,CEditArea* editArea){//根据执行的语句类型在界面上显示执行结果。此函数需修改
	std::string s_sql = sql;
	if(s_sql.find("select") == 0){
		SelResult res;
		Init_Result(&res);
		//rc = Query(sql,&res);
		//将查询结果处理一下，整理成下面这种形式
		//调用editArea->ShowSelResult(col_num,row_num,fields,rows);
		int col_num = 5;
		int row_num = 3;
		char ** fields = new char *[5];
		for(int i = 0;i<col_num;i++){
			fields[i] = new char[20];
			memset(fields[i],0,20);
			fields[i][0] = 'f';
			fields[i][1] = i+'0';
		}
		char *** rows = new char**[row_num];
		for(int i = 0;i<row_num;i++){
			rows[i] = new char*[col_num];
			for(int j = 0;j<col_num;j++){
				rows[i][j] = new char[20];
				memset(rows[i][j],0,20);
				rows[i][j][0] = 'r';
				rows[i][j][1] = i + '0';
				rows[i][j][2] = '+';
				rows[i][j][3] = j + '0';
			}
		}
		editArea->ShowSelResult(col_num,row_num,fields,rows);
		for(int i = 0;i<5;i++){
			delete[] fields[i];
		}
		delete[] fields;
		Destory_Result(&res);
		return;
	}
	RC rc = execute(sql);
	int row_num = 0;
	char**messages;
	switch(rc){
	case SUCCESS:
		row_num = 1;
		messages = new char*[row_num];
		messages[0] = "操作成功";
		editArea->ShowMessage(row_num,messages);
		delete[] messages;
		break;
	case SQL_SYNTAX:
		row_num = 1;
		messages = new char*[row_num];
		messages[0] = "有语法错误";
		editArea->ShowMessage(row_num,messages);
		delete[] messages;
		break;
	default:
		row_num = 1;
		messages = new char*[row_num];
		messages[0] = "功能未实现";
		editArea->ShowMessage(row_num,messages);
	delete[] messages;
		break;
	}
}

RC execute(char * sql){
	sqlstr *sql_str = NULL;
	RC rc;
	sql_str = get_sqlstr();
  	rc = parse(sql, sql_str);//只有两种返回结果SUCCESS和SQL_SYNTAX
	
	if (rc == SUCCESS)
	{
		int i = 0;
		switch (sql_str->flag)
		{
			case 1:
			////判断SQL语句为select语句

			break;

			case 2:
			//判断SQL语句为insert语句

			break;

			case 3:	
			//判断SQL语句为update语句
			break;

			case 4:					
			//判断SQL语句为delete语句
			break;

			case 5:
			//判断SQL语句为createTable语句
			break;

			case 6:	
			//判断SQL语句为dropTable语句
			break;

			case 7:
			//判断SQL语句为createIndex语句
			break;
	
			case 8:	
			//判断SQL语句为dropIndex语句
			break;
			
			case 9:
			//判断为help语句，可以给出帮助提示
			break;
		
			case 10: 
			//判断为exit语句，可以由此进行退出操作
			break;

		}
	}
	else{
		AfxMessageBox(sql_str->sstr.errors);//弹出警告框，sql语句词法解析错误信息
		return rc;
	}
	return rc;
}

RC CreateDB(char *dbpath,char *dbname){//创建数据库
	RC rc1, rc2, rc3;
	rc1 = SUCCESS;//未完成
	//"D:\DBMS\student","student"
	//char *转 string
	std::string str_dbpath(dbpath);
	std::string str_dbname(dbname);

	//通过字符串连接组合文件路径和文件名
	std::string str1 = str_dbpath + "\\" + str_dbname + ".xx"; 
	std::string str2 = str_dbpath + "\\SYSTABLES.xx";
	std::string str3 = str_dbpath + "\\SYSCOLUMNS.xx";

	//string转const char *
	const char* c_s1 = str1.c_str();
	const char* c_s2 = str2.c_str();
	const char* c_s3 = str3.c_str();

	//const char *转char *
	char * c1 = new char[1000];
	char * c2 = new char[1000];
	char * c3 = new char[1000];
	//strcpy(c1, c_s1);
	strcpy(c2, c_s2);
	strcpy(c3, c_s3);

	//创建文件
	rc2 = RM_CreateFile(c2, 25);
	rc3 = RM_CreateFile(c3, 76);
	// 	FILE *fp=NULL;
	// 	FILE *fp2 = NULL;
	// 	FILE *fp3 = NULL;
	//     fopen_s(&fp,c_s1,"w");
	// 	fopen_s(&fp2,c_s2,"w");
	// 	fopen_s(&fp3,c_s3,"w");
	// 	fclose(fp);
	// 	fclose(fp2);
	// 	fclose(fp3);

	if(rc1 != SUCCESS){
		return rc1;
	}
	else if(rc2 != SUCCESS){
		return rc2;
	}
	else if (rc3 != SUCCESS){
		return rc3;
	}
	else{
		return SUCCESS;
	}
}

RC DropDB(const char * dbpath){
	if(RemoveDir(dbpath) == true)
		return SUCCESS;
	else
		return (RC)32;//FAIL
}

RC OpenDB(const char *dbpath){
	
	if(SetCurrentDirectory(dbpath))//设置指定路径为工作路径
		return SUCCESS;
	else return (RC)32;//FAIL
}


RC CloseDB(){
	for(std::vector<RM_FileHandle *>::size_type i = 0; i < vec.size(); i++){//遍历vector中的句柄关闭每一个打开的记录文件
		if(vec[i] != NULL)
			RM_CloseFile(vec[i]);
	}
	vec.clear();
	return SUCCESS;
}

bool CanButtonClick(){//需要重新实现
	//如果当前有数据库已经打开
	if(!vec.empty())//vector非空
		return true;
	//如果当前没有数据库打开
	else return false;
}

bool RemoveDir(const char* szFileDir)
{
	std::string strDir = szFileDir;
	if (strDir.at(strDir.length()-1) != '\\')
		strDir += '\\';
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile((strDir + "*.*").c_str(),&wfd);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	do
	{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (stricmp(wfd.cFileName,".") != 0 &&
				stricmp(wfd.cFileName,"..") != 0)
				RemoveDir( (strDir + wfd.cFileName).c_str());
		}
		else
		{
			DeleteFile( (strDir + wfd.cFileName).c_str());
		}
	}
	while (FindNextFile(hFind,&wfd));
	FindClose(hFind);
	RemoveDirectory(szFileDir);
	return true;
}

RC CreateTable(char *relName,int attrCount,AttrInfo *attributes){
/*创建一个名为relName的表。参数attrCount表示关系中属性的数量（取值为1到MAXATTRS之间）。
参数attributes是一个长度为attrCount的数组。
对于新关系中第i个属性，attributes数组中的第i个元素包含名称、类型和属性的长度
*/
	//获取当前工作路径
	char buf[1000];
	GetCurrentDirectory(1000,buf);  //使用windows api得到当前工作路径
	CString s;
	s.Format("当前工作路径设置为： %s", buf);
	AfxMessageBox(s);

	//将当前工作路径与表名组合，调用RM_CreateFile()创建记录文件（表）
	//char * 转 string
	std::string strRel(relName);
	std::string strPath = buf;
	std::string strRelPath = strPath + "\\" + strRel + ".xx";
	//string 转 char *，先转为const char *，再转为char *
	char cRelPath[1000];
	strcpy(cRelPath, strRelPath.c_str());
	//计算记录文件每一条记录的长度
	int len = 0;
	int i;
	for(i = 0; i < attrCount; i++){
		len += attributes[i].attrLength;
	}
	//调用RM_CreateFile()创建记录文件（表）
	RM_CreateFile(cRelPath,len);

	//修改SYSTABLES.xx和SYSCOLUMNS.xx文件
	RM_FileHandle *SYSTABLESFileHandle = NULL;
	RM_FileHandle *SYSCOLUMNSFileHandle = NULL;
	//遍历vector，查询是否已经存在该记录文件句柄
	for(std::vector<RM_FileHandle *>::size_type i = 0; i < vec.size(); i++){//遍历vector中的句柄
		if(vec[i] != NULL){
			//if(vec[i]->fileName)
		}
	}
}

RC DropTable(char *relName){
//销毁名为relName的表以及在该表上建立的所有索引。
	//删除表上的每一个索引


	//删除表
}