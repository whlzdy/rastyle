//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: sqlite.h
//	Author:	whl
//	Description: create rastyle sqlite head file
//	Others:
//	History: 2015-8-13 create by whl
//*****************************************************************************
#ifndef SQLITE_HH
#define SQLITE_HH


//==================================================sqlite interface begin============================================
typedef int (select_callback)(void * args, int col_count, char ** col_values, char ** col_Name);


void sqlite_get_record_data(char * dbname,char * sqldata,select_callback callback,void * args);

//==================================================sqlite interface end============================================

//acs sqlite exec sql
void acs_sqlite_exec_sql(char * dbname,char * sSQL);

//==================================================create sql tables begin============================================

//create acs plan task table
void create_acs_plan_task_table(char * dbname);

//create acs climate table
void create_acs_climate_table(char * dbname);

//create acs user table
void create_acs_user_table(char * dbname);

//create acs abnormal table
void create_acs_abnormal_table(char * dbname);
//==================================================create sql tables end=============================================








#endif
