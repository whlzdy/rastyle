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
#include <sqlite3.h>
#include <pthread.h>

#include "../systemconfig.h"
#include "sqlite.h"

static pthread_mutex_t sqlite_lock = PTHREAD_MUTEX_INITIALIZER;


int select_print_callback(void * data, int col_count, char ** col_values, char ** col_Name)
{
  // 每条记录回调一次该函数,有多少条就回调多少次
  int i;
  for( i=0; i < col_count; i++){
    printf( "%s = %s\n", col_Name[i], col_values[i] == 0 ? "NULL" : col_values[i] );
  }
  return 0;
}


/*
 * sqlite get record data
 */
void sqlite_get_record_data(char * dbname,char * sqldata,select_callback callback,void * args)
{
	sqlite3 *db = NULL;
	char *zErrMsg = 0;
	int rc,retV;
	if((retV=pthread_mutex_lock(&sqlite_lock)) != 0)
	{
			printf("sqlite_get_record_data pthread_mutex_trylock(&fifo_lock) != 0, =%d \n", retV);
			return ;   /* failed to get lock */
	}
	rc = sqlite3_open(dbname, &db);
	if( rc != SQLITE_OK )
	{
		fprintf(stderr, "Can’t open database: %s＼n",sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
	sqlite3_exec( db, sqldata, callback, args, &zErrMsg);
	sqlite3_close(db);
	pthread_mutex_unlock(&sqlite_lock);
	db = 0;
}


/*
* acs sqlite exec sql
*/
void acs_sqlite_exec_sql(char * dbname,char * sSQL)
{
	sqlite3 *db = NULL;
	char *zErrMsg = 0;
	int rc,retV;
	if((retV=pthread_mutex_lock(&sqlite_lock)) != 0)
	{
			printf("acs_sqlite_exec_sql pthread_mutex_trylock(&fifo_lock) != 0, =%d \n", retV);
			return ;   /* failed to get lock */
	}
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
	pthread_mutex_unlock(&sqlite_lock);
	db = 0;
}


/*
* create acs plan task table
*/
void create_acs_plan_task_table(char * dbname)
{
	char * sSQL1 = "create table acs_plan_task_table (Planname varchar(20) PRIMARY KEY,Objectadd1 integer,volume1 integer,"\
			"Objectadd2 integer,volume2 integer,Objectadd3 integer,volume3 integer ,SMCSTATE varchar(20),Climatename varchar(20),"\
			" Cycle varchar(20),STATE varchar(20),beginningtime datetime ,Stoptime datetime);";
	sqlite3 *db = NULL;
	char *zErrMsg = 0;
	int rc,retV;
	if((retV=pthread_mutex_lock(&sqlite_lock)) != 0)
	{
			printf("create_acs_plan_task_table pthread_mutex_trylock(&fifo_lock) != 0, =%d \n", retV);
			return ;   /* failed to get lock */
	}
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
	pthread_mutex_unlock(&sqlite_lock);
	db = 0;
}


/*
* create acs climate table
*/
void create_acs_climate_table(char * dbname)
{
    char * sSQL1 = "create table acs_climate_data_table (Climatename varchar(20) PRIMARY KEY,CO2Concentration varchar(20),Temperature varchar(20),Humidity varchar(20));";
	sqlite3 *db = NULL;
	char *zErrMsg = 0;
	int rc,retV;
	if((retV=pthread_mutex_lock(&sqlite_lock)) != 0)
	{
			printf("create_acs_climate_table pthread_mutex_trylock(&fifo_lock) != 0, =%d \n", retV);
			return ;   /* failed to get lock */
	}
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
	pthread_mutex_unlock(&sqlite_lock);
	db = 0;
}

//create acs user table
void create_acs_user_table(char * dbname)
{
	char * sSQL1 = "create table acs_user_table (UserID integer PRIMARY KEY,Username varchar(20),PWD varchar(20),authorization varchar(20),isop varchar(20),Publickey varchar(128));";
	sqlite3 *db = NULL;
	char *zErrMsg = 0;
	int rc,retV;
	if((retV=pthread_mutex_lock(&sqlite_lock)) != 0)
	{
			printf("create_acs_user_table pthread_mutex_trylock(&fifo_lock) != 0, =%d \n", retV);
			return ;   /* failed to get lock */
	}
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
	pthread_mutex_unlock(&sqlite_lock);
	db = 0;
}

//create acs abnormal table
void create_acs_abnormal_table(char * dbname)
{
	//do nothing
}


