hello everyone ..

Description : with adminMySQL you can do a total administration on your all databases

Configuration:

- install mysql-server and gcc in your computer

- install libmysqlclient-dev or check here to choise a lib Integrated with your system : ==> http://rosindex.github.io/d/libmysqlclient-dev/

-don't forget to include path mysql.h in this program


How to use:

gcc adminMySQL.c -o adminMySQL -l mysqlclient
./adminMysql -u root -p *****


if you don't have password in your SQL #adminMySQL ignore -p option or use directly
./adminMysql -u root

all query has been saved in new file named history_MySQL.txt

good luck
