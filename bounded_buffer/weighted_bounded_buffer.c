/**
 * POC implementation of weighted bounded buffer problem (threads only).
 * Copyright (C) 2026 Lazar Razvan.
 */

#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>


/*****************************************************************************/

#define SIZE								64
#define WEIGHT								32

//
#define ITERATIONS_NO						1024

//
#define IS_POWER_OF_TWO(x)					((x) > 0 && !((x) & ((x) - 1)))

//
#define WRITERS_NO							16
#define READERS_NO							16


/*****************************************************************************/

// shared buffer
static int _buf[SIZE];
static unsigned long _buf_weight = 0;
static unsigned int _buf_count = 0;
static unsigned int _buf_read_idx = 0;
static unsigned int _buf_write_idx = 0;

// shared buffer mutex
pthread_mutex_t mutex;

// put and get condition variable
pthread_cond_t c_put;
pthread_cond_t c_get;

// reader/writer threads
static pthread_t writers[WRITERS_NO];
static pthread_t readers[READERS_NO];

// reader/writer threads idx
static atomic_int writers_idx = 1;
static atomic_int readers_idx = 1;


/*****************************************************************************/

/**
 * WARNING: functions must be called whithin a critical section.
 */

static void __put(int value)
{
	_buf[_buf_write_idx++ & (SIZE - 1)] = value;
	//
	_buf_weight += value;
	_buf_count++;
}

static int __get(void)
{
	int value;

	value = _buf[_buf_read_idx++ & (SIZE - 1)];
	//
	_buf_weight -= value;
	_buf_count--;

	return value;
}


/*****************************************************************************/

static void* __writer_thread_fn(void *arg)
{
	int id;

	(void)arg;

	// assig thread id (order of execution, not creation)
	id = atomic_fetch_add(&writers_idx, 1);

	for (int i = 0; i < ITERATIONS_NO; i++) {

		// condition loop for available space
		pthread_mutex_lock(&mutex);

		//
		while (_buf_count == SIZE || _buf_weight + id > WEIGHT)
			pthread_cond_wait(&c_put, &mutex);

		// put
		__put(id);

		// signal readers
		pthread_cond_signal(&c_get);

		// release lock
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

static void* __reader_thread_fn(void *arg)
{
	int value, id;

	(void)arg;

	// assig thread id (order of execution, not creation)
	id = atomic_fetch_add(&readers_idx, 1);

	for (int i = 0; i < ITERATIONS_NO; i++) {

		// condition loop for available data
		pthread_mutex_lock(&mutex);

		// condition loop for available data
		while (_buf_count == 0)
			pthread_cond_wait(&c_get, &mutex);

		// get
		value = __get();

		// signal writers
		pthread_cond_broadcast(&c_put);

		// release lock
		pthread_mutex_unlock(&mutex);

		printf("reader %d value %d\n", id, value);
	}

	return NULL;
}


/*****************************************************************************/

static int __init(void)
{
	int rv, writer_idx, reader_idx;

	// size has to be power of 2
	assert(IS_POWER_OF_TWO(SIZE));

	// mutex
	if (pthread_mutex_init(&mutex, NULL)) {
		rv = -1; goto error;
	}

	// put
	if (pthread_cond_init(&c_put, NULL)) {
		rv = -2; goto error_mutex;
	}

	// get
	if (pthread_cond_init(&c_get, NULL)) {
		rv = -4; goto error_cond_put;
	}

	// writer threads
	for (writer_idx = 0; writer_idx < WRITERS_NO; writer_idx++) {
		if (pthread_create(&writers[writer_idx], NULL, __writer_thread_fn, NULL)) {
			rv = -6; goto error_writer_threads;
		}
	}

	// reader threads
	for (reader_idx = 0; reader_idx < READERS_NO; reader_idx++) {
		if (pthread_create(&readers[reader_idx], NULL, __reader_thread_fn, NULL)) {
			rv = -7; goto error_reader_threads;
		}
	}

// success:
	return 0;

	/**
	 * On threads error, some may be still be blocked, waiting for semaphore
	 * but this is a proof of concept implementation.
	 */
error_reader_threads:
	for (--reader_idx; reader_idx >= 0; reader_idx--)
		pthread_join(readers[reader_idx], NULL);
error_writer_threads:
	for (--writer_idx; writer_idx >= 0; writer_idx--)
		pthread_join(writers[writer_idx], NULL);
//error_cond_get:
	pthread_cond_destroy(&c_get);
error_cond_put:
	pthread_cond_destroy(&c_put);
error_mutex:
	pthread_mutex_destroy(&mutex);
error:
	fprintf(stderr, "%s error %d\n", __func__, rv);
	return rv;
}


/*****************************************************************************/


int main()
{
	// init
	if (__init())
		return -1;

	// join threads
	for (int i = 0; i < WRITERS_NO; i++)
		pthread_join(writers[i], NULL);
	//
	for (int i = 0; i < READERS_NO; i++)
		pthread_join(readers[i], NULL);

	// destroy put/get
	pthread_cond_destroy(&c_get);
	pthread_cond_destroy(&c_put);

	// destroy mutex
	pthread_mutex_destroy(&mutex);

	return 0;
}
