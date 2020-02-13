# Ring buffer

Implement a circular buffer that supports multithreading.

## libring_buffer.so

Run the following command to get the shared library and the unittest:
```
	$ make
```
The shared library expose the API to be used for the ring buffer.

[Ring buffer implementation] (https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem)

The API contains the following functions:
```
	-ring_buffer_init:	Create a ring buffer
	-ring_buffer_free:	Free the ring buffer
	-ring_buffer_put:	Add new element to ring buffer
	-ring_buffer_get:	Extract an element from ring buffer
```

The shared library supports both single-threaded and multi-threading implementation. By default, it is created
with support for multi-threading. If you only want to use it for single-threaded examples (one writer/one reader)
you can delete the ```-DMULTI_THREADING``` from ```CFLAGS``` inside Makefile.

## threads

The purpose of this is to test the behavior of the ring buffer. When running, you need to specify the number of
writing and reading threads.

By default ```(NUM_WRITES)``` each writer will add 10 messages to ring buffer. You can change this value inside
```threads.h```, alongisde with ```RING_SIZE(ring buffer size)```.

Remove ```PRINT``` content in ```Makefile``` if you don't want to see the messages. (reduce the performance)

## Synchronization

For synchronization there are 2 mutexes used:
```
	- readers_mutex
	- r_mutex
```

```readers_mutex``` is used to synchronize all readers. Since we know the total number of messages that will be
written to the ring buffer ```NUM_WRITES * writers```, ```read_messages``` will be incremented by each reders
once a message is read need protection (atomic context).

```r_mutex``` is used for synchronization at ring buffer level. Only one thread can access at once the buffer.

To run the application :
```
$ make
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
$ ./threads <readers_number> <writers_number>
```
