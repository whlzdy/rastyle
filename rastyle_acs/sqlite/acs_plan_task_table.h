//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: acs_plan_task.h
//	Author:	whl
//	Description: create rastyle sqlite head file
//	Others:
//	History: 2015-8-17 create by whl
//*****************************************************************************


#ifndef ACS_PLAN_TASK_HH
#define ACS_PLAN_TASK_HH


typedef int (select_callback)(void * data, int col_count, char ** col_values, char ** col_Name);

void create_acs_plan_task_table(char * dbname);

void acs_plan_task_table_exec_sql(char * dbname,char * sSQL);

void sqlite_get_record_data(char * dbname,char * sqldata,select_callback callback);



#endif
