## Configuration:

- install mysql-server and gcc in your computer
- install libmysqlclient-dev or check here to choise a lib Integrated with your system : ==> http://rosindex.github.io/d/libmysqlclient-dev/
- don't forget to include path mysql.h in this program


## How to use:

- $> gcc adminMySQL.c -o adminMySQL -l mysqlclient
- $> ./adminMySQL -u root -p *****


## more instructions:
- if you don't have password in your SQL #adminMySQL ignore -p option or use directly

       ./adminMysql -u root

- all query has been saved in new file named history_MySQL.txt

# adminMySQL:
![adminMySQL](https://raw.githubusercontent.com/ayoubridouani/adminMySQL/master/screenshot.png "adminMySQL")


to contact me : ridouaniayoub@gmail.com

good luck
