# Named pipes

This is mechanism for inter-process communication.

The application is designed close to a client-server approach. Server creates a pipe and wait for new connection (writes to pipe).
Since the pipe is blocking (O_NONBLOCK not specified) the server will be blocked on "read" unit a client writes to the pipe and
the communication will continue.

Client has two choices :
```
	'y' : Inform server that this is the last message
	'n' : Regular message for server
```

The api used to read/write on pipes is exported as a shared library.

To run the application :
```
$ make
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
$ ./server
$ ./client  (different terminal)
```
