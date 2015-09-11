//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: acs_climate_table.h
//	Author:	whl
//	Description: create rastyle sqlite head file
//	Others:
//	History: 2015-8-20 create by whl
//*****************************************************************************


#ifndef ACS_CLIMATE_TABLE_HH
#define ACS_CLIMATE_TABLE_HH

#include "sqlite.h"


//typedef int (select_callback)(void * data, int col_count, char ** col_values, char ** col_Name);

void create_acs_climate_table(char * dbname);

void acs_climate_table_exec_sql(char * dbname,char * sSQL);

void sqlite_get_record_data(char * dbname,char * sqldata,select_callback callback);



#endif
