/* Ring buffer design
 * Copyright (C) 2020 Lazar Razvan
 */

#include "threads.h"

/*
 * This function will be called by all readers threads.
 */
static void *readers_function(void *data)
{
	int w_number;
	struct struct_t w_struct;

	w_number = *(int *)data;
	for (;;) {
			pthread_mutex_lock(&readers_mutex);
			if (read_messages == w_number * NUM_WRITES) {
				pthread_mutex_unlock(&readers_mutex);
				break;
			}
			++read_messages;
			pthread_mutex_unlock(&readers_mutex);
			while (ring_buffer_get(r_buf, &w_struct)) {
			}
	}

	return NULL;
}

/*
 * This function will be called by all writers threads.
 */
static void *writers_function(void *data)
{
	int i;
	struct struct_t w_struct;

	w_struct.thread_id = pthread_self();
	memcpy(w_struct.msg, MSG, MSG_SIZE);

	for (i = 0; i < NUM_WRITES; i++) {
		while (ring_buffer_put(r_buf, &w_struct)) {
		}
	}

	return NULL;
}


int main(int argc, char **argv)
{
	int i, r_number, w_number, err = 0;
	pthread_t *writers, *readers;

	r_buf = ring_buffer_init(sizeof(int), 100);
	if (!r_buf)
		return -1;

	/* Get readers/writers number */
	if (argc < 3) {
		fprintf(stderr, "Specify readers & writers number.Ex:\n%s\n",
			"./threads <readers_nr> <writers_nr>");
		err = -1;
		goto out_err;
	}

	r_number = strtol(argv[1], NULL, 10);
	w_number = strtol(argv[2], NULL, 10);

	/* Create threads */
	readers = (pthread_t *) malloc(r_number * sizeof(pthread_t));
	if (!readers) {
		err = -1;
		goto out_err;
	}
	writers = (pthread_t *) malloc(w_number * sizeof(pthread_t));
	if (!writers) {
		err = -1;
		goto out_err_1;
	}

	/* Start threads */
	for (i = 0; i < w_number; i++) {
		if (pthread_create(&writers[i], NULL, &writers_function, NULL)) {
			ON_ERR(errno);
			goto out_err_2;
			/* TODO: Join rest of the threads */
		}
	}
	for (i = 0; i < r_number; i++) {
		if (pthread_create(&readers[i], NULL, &readers_function, &w_number)) {
			ON_ERR(errno);
			goto out_err_2;
			/* TODO: Join rest of the threads */
		}
	}

	/* Join all threads */
	for (i = 0; i < w_number; i++) {
		if (pthread_join(writers[i], NULL)) {
			ON_ERR(errno);
			goto out_err_2;
			/* TODO: Join rest of the threads */
		}
	}
	for (i = 0; i < r_number; i++) {
		if (pthread_join(readers[i], NULL)) {
			ON_ERR(errno);
			goto out_err_2;
			/* TODO: Join rest of the threads */
		}
	}

out_err_2:
	free(writers);
out_err_1:
	free(readers);
out_err:
	ring_buffer_free(r_buf);
	return err;
}
