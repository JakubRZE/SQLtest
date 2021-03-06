// SQLtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>


void showSQLError(unsigned int handleType, const SQLHANDLE& handle)
{
	SQLCHAR SQLState[1024];
	SQLCHAR message[1024];
	if (SQL_SUCCESS == SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, message, 1024, NULL))
	{
		std::cout << "SQL message: " << message << "\nSQL state: " << SQLState << std::endl;
	}
}

int main()
{
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL; 
	SQLRETURN retCode = 0;
	char SQLQuery[] = "SELECT [id], [name], [amount] FROM[TESTdatabase].[dbo].[Order]";

	do{
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvHandle))
			break;

		if (SQL_SUCCESS != SQLSetEnvAttr(SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
			break;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SQLEnvHandle, &SQLConnectionHandle))
			break;

		if (SQL_SUCCESS != SQLSetConnectAttr(SQLConnectionHandle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0))
			break;

		SQLCHAR retConString[1024];
		switch (SQLDriverConnect(SQLConnectionHandle, NULL, (SQLCHAR*)
			"DRIVER={SQL Server}; SERVER=DESTRUKTOR\\SQLEXPRESS; DATABASE=TESTdatabase; Trusted_Connection=Yes;", SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT))
		{
		case SQL_SUCCESS:
			break;

		case SQL_SUCCESS_WITH_INFO:
			break;

		case SQL_NO_DATA_FOUND:
			showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			retCode = -1;
			break;
		case SQL_INVALID_HANDLE:
			showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			retCode = -1;
			break;
		case SQL_ERROR:
			showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			retCode = -1;
			break;
		default:
			break;
		}

		if (retCode == -1)
			break;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle))
			break;

		if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLCHAR*)SQLQuery, SQL_NTS))
		{
			showSQLError(SQL_HANDLE_STMT, SQLStatementHandle);
			break;
		}
		else
		{

			int id;
			char name[50];
			int amount;

			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS)
			{
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &id, sizeof(id), NULL);
				SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &name, sizeof(name), NULL);
				SQLGetData(SQLStatementHandle, 3, SQL_C_DEFAULT, &amount, sizeof(amount), NULL);
				std::cout << id << " " << name <<" "<< amount <<'\n';
			}
		}

	} while (false);

	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);

	getchar();

    return 0;
}

