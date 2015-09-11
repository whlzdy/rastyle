//*****************************************************************************
//  Copyright (C) 2015 rastyle。com
//	File name: sqlite.c
//	Author:	whl
//	Description: create rastyle sqlite.c file
//	Others:
//	History: 2015-8-13 create by whl
//*****************************************************************************


#include <stdio.h>
#include <stdlib.h>
#include "sqlite.h"
#include <sqlite3.h>


#include "../systemconfig.h"


void create_acs_plan_task_table(char * dbname)
{
	const char * sSQL1 = "create table acs_plan_task_table (Planname varchar(20) PRIMARY KEY,Cycle varchar(20),STATE varchar(20),beginningtime datetime ,Stoptime datetime);";
	sqlite3 *db = NULL;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(dbname, &db);
	if( rc != SQLITE_OK )
	{
		fprintf(stderr, "Can’t open database: %s＼n",sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
	else
	{
		rc = sqlite3_exec( db, sSQL1, 0, 0, &zErrMsg );
		if( rc != SQLITE_OK ){
		    fprintf(stderr, "SQL error: %s\n", zErrMsg);
		    sqlite3_free(zErrMsg);
		 }
	}
	// 关闭数据库
	sqlite3_close(db);
	db = 0;
}



void acs_plan_task_table_exec_sql(char * dbname,char * sSQL)
{
	sqlite3 *db = NULL;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(dbname, &db);
	if( rc != SQLITE_OK )
	{
		fprintf(stderr, "Can’t open database: %s＼n",sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
	rc = sqlite3_exec(db, sSQL, 0, 0, &zErrMsg);
	if(rc == SQLITE_OK){
		//printf("插入数据成功\n");
	 }
	sqlite3_close(db);
	db = 0;
}


void sqlite_get_record_data(char * dbname,char * sqldata,select_callback callback)
{
	sqlite3 *db = NULL;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(dbname, &db);
	if( rc != SQLITE_OK )
	{
		fprintf(stderr, "Can’t open database: %s＼n",sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
	sqlite3_exec( db, sqldata, callback, 0, &zErrMsg);
	sqlite3_close(db);
	db = 0;
}



