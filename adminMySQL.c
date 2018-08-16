#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

//options of connection to Database
#define host "127.0.0.1"
#define user "root"
#define password ""
#define database "mysql"
#define port 0

#define maxLengthOfQuery 1024

//all functions used in program adminMySQL
void login(int , char **);
void connection(MYSQL **);
void administration(MYSQL *);
char* mysql_get_db(MYSQL *);
void showHelp();
void clearBuffer();

//principal function
int main(int argc, char **argv){
	login(argc, argv);
}

//function to check identity a user
void login(int argc, char **argv){
	if(argc == 5 || argc == 3){
		if(strcmp(argv[2],user) == 0 && (strcmp(argv[4],password) == 0 || strcmp(password,"") == 0)){
			MYSQL *con;
			connection(&con);
			printf("\n");
			printf("Welcome to the AdminMySQL version 1\n");
			printf("Client info : %s\n",mysql_get_client_info());
			//printf("Client version : %lu\n",mysql_get_client_version());
			printf("Server Info : %s\n",mysql_get_server_info(con));
			//printf("Server version : %lu\n",mysql_get_server_version(con));
			printf("Host info : %s\n",mysql_get_host_info(con));
			printf("the version of the protocol used by the current connection : %d\n",mysql_get_proto_info(con));
			printf("\n");
			administration(con);
		}else{
			printf("Access denied for user %s@localhost\n",argv[2]);
			exit(0);
		}
	}else{
		printf("Access denied, Please insert a valid information like a :\n");
		printf("adminMySQL -u username -p password\n");
		exit(0);
	}
}

void connection(MYSQL **con){
	if((*con = mysql_init(NULL)) == NULL){
		printf("Error %s (Line : %d) : %s\n\n",mysql_sqlstate(*con),__LINE__,mysql_error(*con));
		exit(0);
	}

	mysql_options(*con,MYSQL_READ_DEFAULT_GROUP,"options");

	if(mysql_real_connect(*con,host,user,password,database,port,0,0) == NULL){
		printf("Error %s (Line : %d) : %s\n\n",mysql_sqlstate(*con),__LINE__,mysql_error(*con));
		exit(0);
	}
}

void administration(MYSQL *con){
	char *query;
	query = (char*)malloc(sizeof(char) * maxLengthOfQuery);

	unsigned int num_fields;
	unsigned int num_rows;

	FILE *history;
	history=fopen("history_adminMySQL.txt","a");

	MYSQL_RES *result;
	MYSQL_FIELD *fields;
	MYSQL_ROW rows;

	printf("AdminMySQL [%s]> ",mysql_get_db(con));
	scanf("%[^\n]s",query);
	clearBuffer();

	fprintf(history,"%s\n",query);

	fclose(history);

	if(strcmp(query,"help") == 0 || strcmp(query,"?") == 0){
		showHelp();
		administration(con);
	}
	else if(strcmp(query,"clear") == 0){
		system("clear");
		administration(con);
	}
	else if(strcmp(query,"connect") == 0){
		mysql_reload(con);
		administration(con);
	}
	else if(strcmp(query,"exit") == 0 || strcmp(query,"quit") == 0){
		exit(0);
	}
	else if(strcmp(query,"status") == 0){
		printf("%s\n\n",mysql_stat(con));
		administration(con);
	}
	else{
		if(mysql_query(con,query) != 0){
			printf("Error %s (Line : %d) : %s\n\n",mysql_sqlstate(con),__LINE__,mysql_error(con));
			query = NULL;
			free(query);
			administration(con);
		}

		//get string length for larger string in rows + fields
		//just for best design when viewing a result
		if((result = mysql_store_result(con)) != NULL){
			num_fields = mysql_num_fields(result);
			num_rows = mysql_num_rows(result);

			//check if table is empty
			if(num_rows == 0){
				printf("Empty set\n\n");
				administration(con);
			}

			//array to store maxlength to any rows or fields for each column
			int max_string_length[num_fields];
			for(int i=0;i<num_fields;i++) max_string_length[i]=0;

			int k = 0;
			fields = mysql_fetch_fields(result);

			//loop to store maxlength for each rows or fields in array : max_string_length
			while((rows = mysql_fetch_row(result)) != NULL){
				for(int i=0;i<num_fields;i++){
					if(strlen(fields[i].name)>max_string_length[i]){
						max_string_length[i] = strlen(fields[i].name);
					}
				}
				for(int i=0;i<num_fields;i++){
					if(rows[i] == NULL) rows[i] = "NULL";
					if((int)strlen(rows[i])>max_string_length[i]){
						max_string_length[i] = strlen(rows[i]);
					}
				}
				++k;
			}

			//reset result to offset=0
			mysql_data_seek(result,0);

			//to display (open)
			//example : +--------------------------+-------+------+-------+--------+------------+
			for(int i=0;i<num_fields;i++){
				printf("+");
				for(int j=0;j<max_string_length[i]+5;j++)	printf("-");
			}
			printf("+");
			printf("\n");

			//to display fields
			printf("|");
			for(int i=0;i<num_fields;i++){
				printf("%s",fields[i].name);
				for(int k=0;k<max_string_length[i]-strlen(fields[i].name)+5;k++) printf(" ");
				printf("|");
			}
			printf("\n");

			//to display (close)
			//example : +--------------------------+-------+------+-------+--------+------------+
			for(int i=0;i<num_fields;i++){
				printf("+");
				for(int j=0;j<max_string_length[i]+5;j++)	printf("-");
			}
			printf("+");
			printf("\n");

			//to display rows
			while((rows = mysql_fetch_row(result)) != NULL){
				printf("|");
				for(int i=0;i<num_fields;i++){
					if(rows[i]==NULL)  rows[i]="NULL";
					printf("%s",rows[i]);
					for(int k=0;k<max_string_length[i]-strlen(rows[i])+5;k++) printf(" ");
					printf("|");
				}
				printf("\n");
			}

			//to display (close)
			//example : +--------------------------+-------+------+-------+--------+------------+
			for(int i=0;i<num_fields;i++){
				printf("+");
				for(int j=0;j<max_string_length[i]+5;j++)	printf("-");
			}
			printf("+\n");

			printf("%d rows in set\n\n",num_rows);

			query = NULL;
			free(query);
			mysql_free_result(result);
		}else{
			//printf("Error %s (Line : %d) : %s\n\n",mysql_sqlstate(con),__LINE__,mysql_error(con));
		}

		if(result == NULL){
			printf("Query OK, %llu row affected\n\n",mysql_affected_rows(con));
		}

		administration(con);
	}
}

char* mysql_get_db(MYSQL *con){
	MYSQL_ROW rows;
	MYSQL_RES *result;

	char *DB;
	DB = (char*)malloc(50);

	if(mysql_query(con,"select database()") != 0){
		printf("Error %s (Line : %d) : %s\n\n",mysql_sqlstate(con),__LINE__,mysql_error(con));
		administration(con);
	}
	if((result=mysql_use_result(con)) != NULL){
		rows = mysql_fetch_row(result);
	}
	if(rows[0] == NULL){
		strcpy(DB,"none");
	}else{
		strcpy(DB,rows[0]);
	}

	mysql_free_result(result);

	return DB;
}

void showHelp(){
	printf("\
	List of all MySQL commands:\n\
	Note that all text commands must be first on line and end with ';'\n\
	?         (\\?) Synonym for `help'.\n\
	clear     (\\c) Clear the current input statement.\n\
	connect   (\\r) Reconnect to the server. Optional arguments are db and host.\n\
	exit      (\\q) Exit mysql. Same as quit.\n\
	help      (\\h) Display this help.\n\
	quit      (\\q) Quit mysql.\n\
	status    (\\s) Get status information from the server.\n\
	use       (\\u) Use another database. Takes database name as argument.\n\
	\n");
}

void clearBuffer(){
	char clear;
	while((clear = getchar()) != '\n' && clear != EOF);
}
