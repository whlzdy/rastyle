//*****************************************************************************
//  Copyright (C) 2015 rastyle¡£com
//	File name: acs_climate_table.c
//	Author:	whl
//	Description: create rastyle sqlite head file
//	Others:
//	History: 2015-8-20 create by whl
//*****************************************************************************

#include "acs_climate_table.h"
#include "../systemconfig.h"



/*
* create acs climate table
*/
void create_acs_climate_table(char * dbname)
{
	const char * sSQL1 = "create table acs_climate_data_table (Planname varchar(20) PRIMARY KEY,Cycle varchar(20),STATE varchar(20),beginningtime datetime ,Stoptime datetime);";
	sqlite3 *db = NULL;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(dbname, &db);
	if( rc != SQLITE_OK )
	{
		fprintf(stderr, "Can¡¯t open database: %s£Ün",sqlite3_errmsg(db));
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
	// ¹Ø±ÕÊý¾Ý¿â
	sqlite3_close(db);
	db = 0;
}
