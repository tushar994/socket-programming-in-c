# Socket programming

## What this is
We have two files here, `client.c` and `server.c`, which contain code corresponding to a client and server respectively. When you run the server in a certain directory, and the client in a certain directory, you can download files in the server directory into the client directory.


## How to run

1) First compile and run the server file in a certain directory
	```shell
	gcc -o  exec_ser server.c
	./exec_ser
	```
2) Next, compile and run the Client file.
	```shell
	gcc -o exec_cli Client.c
	./exec_cli
	```

## Commands you can type into client
- To download files from client, run
	```shell
	client> get <file1> <file2> <file3>...
	```
- To exit
	```shell
	client> exit
	```

## Multiple clients
- The server will process one client at a time, and will start processes next client only if the current client exits.