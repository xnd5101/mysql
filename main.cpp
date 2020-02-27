#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// #include "mysql_connection.h"
// #include <cppconn/driver.h>
// #include <cppconn/exception.h>
// #include <cppconn/resultset.h>
// #include <cppconn/statement.h>
// #include <cppconn/prepared_statement.h>
#include "mysql.h"
using namespace std;

void testMysqlConnect()
{
	// sql::Driver *driver;
 //    	sql::Connection *con;
 //    	sql::Statement *stmt;
 //    	sql::ResultSet *res;
 //    	sql::PreparedStatement *pstmt;
 //    	try {
 //        	driver = get_driver_instance();
 //        	con = driver->connect("tcp://192.168.0.240:3306", "root", "123456");
 //        	con->setSchema("honghan_ibss_sijiqing");
 //    	}
 //    	catch (sql::SQLException &e)
 //    	{
 //        	printf("error \n");
	// 	cout << e.what() << endl;
 //    	}
	// cout << "hello world!" << endl;
	/*	
	stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS test");
        stmt->execute("CREATE TABLE test(id INT, name varchar(32))");
        delete stmt;

        pstmt = con->prepareStatement("INSERT INTO test(id,name) VALUES (?,?)");
        for (int i = 1; i <= 10; i++) {
            	pstmt->setInt(1, i);
		pstmt->setString(2,"xx");
            	pstmt->executeUpdate();
        }
        delete pstmt;

        pstmt = con->prepareStatement("SELECT id FROM test ORDER BY id ASC");
        res = pstmt->executeQuery();
	//方法一	
	while(res->next)
	{
		cout << "id:" << res->getInt("id") << endl;
	}
	//方法二
        res->afterLast();
        while (res->previous())
	{
            cout << "\t... MySQL counts: " << res->getInt("id") << endl;
	}
        delete res;

        delete pstmt;
	*/
	// delete con;
}

void testMysqlClient()
{
	MYSQL conn;
	int res;
	mysql_init(&conn);
	if(mysql_real_connect(&conn,"192.168.0.240","root","123456","new_ibss",0,NULL,CLIENT_FOUND_ROWS)) //"root":数据库管理员 "":root密码 "test":数据库的名字
	{
		printf("connect success!\n");
		res=mysql_query(&conn,"select * from hh_device where device_id = 1;");
		if(res)
		{
			printf("error\n");
			cout << mysql_error(&conn) << endl;
		}
		else
		{
			printf("OK\n");
			MYSQL_FIELD *field;
			MYSQL_ROW row; 
			MYSQL_RES *result = mysql_store_result(&conn);
			int num = mysql_num_fields(result);
			for(int i = 0; i < num; i++)
			{
				field = mysql_fetch_field_direct(result, i);  //返回字段类型
				//cout << field->name << endl;  //输出字段名
			}

			row = mysql_fetch_row(result);
			while(row != NULL)
			{
				cout << row[0] << "******" << endl;
				cout << row[1] << "------" << endl;
				for(int i = 0; i < num; i++)
		 		{
		    		// cout << row[i] << endl;  //输出字段名
		 		}
		 		row = mysql_fetch_row(result);
			}
			mysql_free_result(result);

			MYSQL_STMT * stmt = NULL;
			stmt = mysql_stmt_init(&conn);
			MYSQL_BIND bind[2];
			memset(bind, 0, sizeof(bind));
			
			string sql = "select device_id,name from hh_device where device_id= ? and name = ?;";
			// string sql = "select device_id,name from hh_device where device_id= ?;";
			// string sql = "update hh_device set deleted_at = 1 where device_id= ?;";
			cout << "sql:" << sql << endl;
			mysql_stmt_prepare(stmt, sql.c_str(), sql.length());

			int param_count = mysql_stmt_param_count(stmt);
			cout << "param_count:" << param_count << endl;

			int id = 1;
			bind[0].buffer_type = MYSQL_TYPE_LONG;//MYSQL_TYPE_INT24;
			bind[0].buffer = (char*)&id;
			bind[0].buffer_length = sizeof(id);

			// char str[50] = {0};
			// strcpy(str, "247");
			// bind[1].buffer_type = MYSQL_TYPE_STRING;
			// bind[1].buffer = str;
			// bind[1].buffer_length = sizeof(str);
			
			string str = "247";
			bind[1].buffer_type = MYSQL_TYPE_STRING;
			bind[1].buffer = (char*)str.c_str();
			bind[1].buffer_length = str.length();
			
			
			cout << "bind:" << endl;

			mysql_stmt_bind_param(stmt, bind);
			cout << "bind end:" << endl;
			mysql_stmt_execute(stmt);
			cout << "mysql_stmt_execute:" << endl;
			int affected_rows= mysql_stmt_affected_rows(stmt);
			cout << "affected_rows:" << affected_rows << endl;

			MYSQL_BIND outbind[2];
			memset(outbind, 0, sizeof(outbind));
			int device_id = 0;
			outbind[0].buffer_type = MYSQL_TYPE_LONG;//MYSQL_TYPE_INT24;
			outbind[0].buffer = (char*)&device_id;
			outbind[0].is_null = 0;
			outbind[0].length = 0;

			char str_data[50];
			unsigned long str_length;
			outbind[1].buffer_type = MYSQL_TYPE_STRING;//MYSQL_TYPE_INT24;
			outbind[1].buffer = (char*)str_data;
			outbind[1].buffer_length= 50;
			outbind[1].is_null = 0;
			outbind[1].length = &str_length;

			mysql_stmt_bind_result(stmt, outbind);
			int ret = mysql_stmt_store_result(stmt);

			MYSQL_RES *prepare_meta_result = mysql_stmt_result_metadata(stmt);
			int column_count= mysql_num_fields(prepare_meta_result);
			cout << "column_count:" << column_count << endl;
			int prepare_rows = mysql_stmt_num_rows(stmt);
			cout << "prepare_rows:" << prepare_rows << endl;
			
			while (!mysql_stmt_fetch(stmt))
			{
				cout << device_id << endl;
				cout << str_data << endl;
			}
			// int num1 = mysql_num_fields(result1);
			// cout << "num1:" << num1 << endl;
			mysql_stmt_free_result (stmt);
			mysql_stmt_close (stmt);
		}
		mysql_close(&conn);
	}
	mysql_close(&conn);
}

int main(int argc, char *argv[])
{
	// testMysqlConnect();
	testMysqlClient();

	return 0;
}
