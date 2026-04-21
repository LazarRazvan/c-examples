/**
 * POC implementation of bounded buffer problem (threads only).
 * Copyright (C) 2026 Lazar Razvan.
 */

#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>


/*****************************************************************************/

#define SIZE								64

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
static unsigned int _buf_read_idx = 0;
static unsigned int _buf_write_idx = 0;

// shared buffer mutex
pthread_mutex_t mutex;

// put and get semaphores
static sem_t s_put;
static sem_t s_get;

// reader/writer threads
static pthread_t writers[WRITERS_NO];
static pthread_t readers[READERS_NO];

// reader/writer threads idx
static atomic_int writers_idx = 0;
static atomic_int readers_idx = 0;


/*****************************************************************************/

static void* __writer_thread_fn(void *arg)
{
	int id;

	(void)arg;

	// assig thread id (order of execution, not creation)
	id = atomic_fetch_add(&writers_idx, 1);

	for (int i = 0; i < ITERATIONS_NO; i++) {
		// wait for available space
		if (sem_wait(&s_put))
			break;

		// critical section
		pthread_mutex_lock(&mutex);
		_buf[_buf_write_idx++ & (SIZE - 1)] = id;
		pthread_mutex_unlock(&mutex);

		// notify available data
		if (sem_post(&s_get))
			break;
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
		// wait for available data
		if (sem_wait(&s_get))
			break;

		pthread_mutex_lock(&mutex);
		value = _buf[_buf_read_idx++ & (SIZE - 1)];
		pthread_mutex_unlock(&mutex);

		//
		printf("reader %d value %d\n", id, value);

		// notify available space
		if (sem_post(&s_put))
			break;
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

	// put semaphores
	if (sem_init(&s_put, 0, SIZE)) {
		rv = -2; goto error_mutex;
	}

	// get semaphore
	if (sem_init(&s_get, 0, 0)) {
		rv = -3; goto error_sem_put;
	}

	// writer threads
	for (writer_idx = 0; writer_idx < WRITERS_NO; writer_idx++) {
		if (pthread_create(&writers[writer_idx], NULL, __writer_thread_fn, NULL)) {
			rv = -4; goto error_writer_threads;
		}
	}

	// reader threads
	for (reader_idx = 0; reader_idx < READERS_NO; reader_idx++) {
		if (pthread_create(&readers[reader_idx], NULL, __reader_thread_fn, NULL)) {
			rv = -5; goto error_reader_threads;
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
//error_sem_get:
	sem_destroy(&s_get);
error_sem_put:
	sem_destroy(&s_put);
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

	// destroy semaphores
	sem_destroy(&s_get);
	sem_destroy(&s_put);

	// destroy mutex
	pthread_mutex_destroy(&mutex);

	return 0;
}
