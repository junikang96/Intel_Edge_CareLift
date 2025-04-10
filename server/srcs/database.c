#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <../incs/main.h>

#define  DB_HOST  "localhost"
#define  DB_USER  "server"
#define  DB_PASS  "123456"
#define  DB_NAME  "db_info_disadvantages"


int checkid_db(char* id)
{
   MYSQL* conn;
   MYSQL_RES* res;
   MYSQL_ROW row;

   char query[256];
   int disadvantaged = 0;

   conn = mysql_init(NULL); //conn init
   if (conn == NULL)
   {
      fprintf(stderr, "mysql_init() failed\n");
      return -1;
   }

   if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL)
   {
      fprintf(stderr, "mysql_real_connect() failed : %s\n", mysql_error(conn));
      mysql_close(conn);
      return -1;
   }

   //select db
   sprintf(query, "select disadvantaged from tbl_disadvantaged where ID = '%s'", id);
   if (mysql_query(conn, query))
   {
      fprintf(stderr, "select query failed : %s\n", mysql_error(conn));
      mysql_close(conn);
      return -1;
   }

   //read db
   res = mysql_store_result(conn);
   if (res == NULL)
   {
      fprintf(stderr, "mysql_store_resule() failed\n");
      mysql_close(conn);
      return -1;
   }

   //charge money
   row = mysql_fetch_row(res);
   if (row)
   {
      disadvantaged = atoi(row[0]);
   }
   else
   {
      printf("'%s' is invalid id\n", id);
   }

   mysql_free_result(res);
   mysql_close(conn);
   return disadvantaged;
}

t_database *getDatabaseInfo(char** id)
{
   int i = 0, j = 0;
   int ret;
   char id_dst[20];
   bool flag = true;
   
   t_database *database = (t_database *)malloc(sizeof(t_database));
   if (database == NULL)
      printf("malloc error\n");

   database->disabled = 0;
   database->notDisabled = 0;
   memset(database->notDisabled_RFID, 0, BUFSIZE);
   
   while (id[i])
   {
      int k = 0;
      for (j = 0; id[i][j] != '\0'; j++)
      {
         if (id[i][j] != ' ')
         {
            id_dst[k++] = id[i][j];
         }
      }
      id_dst[k] = '\0';

      printf("checking id %s\n", id_dst);
      ret = checkid_db(id_dst);
      printf("ret is:%d\n", ret);

      if (ret == 1) // disadvantaged == 1
         (database->disabled)++;
      else if (ret == 0)
      {
         (database->notDisabled)++;
         if (flag)
         {
            strncpy(database->notDisabled_RFID, id_dst, strlen(id_dst));
            flag = false;
         }
      }
      else if (ret < 0) // reading database failed
      {
         printf("reading database failed");
         return (NULL);
      }
      i++;
   }
   return (database);
}

