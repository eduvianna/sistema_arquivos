servidor: server/server.c
	file="servidor"
	if [ -f "$file" ]; then rm -f $file; fi
	gcc -pthread server/server.c -o servidor
	