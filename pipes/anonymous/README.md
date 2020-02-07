# Anonymous pipes

This is mechanism for inter-process communication.

Anonymous pipes can only be used for related process (parent/child) or the same process.

The program contains two examples :
```
	Select '1' : Read/write from the same process
	Select '2' : Read/write from different processes
```

The example shows an anoymous pipe been created which is inherited by the child process. In the next step, the parent and the child
close one end of the pipe(read/write) and send/receive the message.

To run the program :
```
$ make
$ ./pipes
```
