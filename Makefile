
dbsv.out: Main.c Network.c Parameter.c Database.c Config.h
	gcc -Wall -o dbsv.out Main.c Network.c Parameter.c Database.c -lmysqlclient -L/usr/lib64/mysql/