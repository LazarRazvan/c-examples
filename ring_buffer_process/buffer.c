#include "buffer.h"

/*
 * Initialize a ring buffer for multi processes. On SUCCESS, the address of
 * the mapped memory is returned. On FAIL, return NULL.
 */
struct ring_buffer* ring_buffer_init()
{
	int shm_fd;
	struct ring_buffer *r_buffer;

	/* Open shared memory object */
	shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 066);
	if (shm == -1) {
		ON_ERR(errno);
		goto out;
	}

	/* Truncate shared memory to our ring buffer structure size */
	if (ftruncate(shm_fd, sizeof(struct ring_buffer))) {
		ON_ERR(errno);
		goto err_link;
	}

	/* Map structure to opened shared memory */
	r_buffer = (struct ring_buffer *)mmap(NULL, sizeof(struct ring_buffer),
		   PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (!r_buffer) {
		ON_ERR(errno);
		goto err_link;
	}

	/* Initialize structure */
	r_buffer->head = r_buffer->tail = 0;
	r_sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
	if (r_sem = SEM_FAILED) {
		ON_ERR(errno);
		goto err_map;
	}

	return r_buffer;
err_map:
	munmap(r_buffer, sizeof(struct ring_buffer));	
err_link:
	shm_unlink(SHM_NAME);
out:
	return NULL;
}

/*
 * Free the ring buffer structure.
 */
void ring_buffer_free(struct ring_buffer *r_buffer)
{
	if (r_buffer) {
		sem_close(r_buffer->r_sem);
		munmap(r_buffer, sizeof(struct ring_buffer));	
		sem_unlink(SEM_NAME);
		shm_unlink(SHM_NAME);
	}
}

/*
 * Get the mapped ring buffer address. This will be called by the
 * reading/writing processes.
 */
struct ring_buffer* ring_buffer_open()
{
	int shm_fd;
	struct ring_buffer *r_buffer;

	shm_fd = shm_open(SHM_NAME, O_RDWR, 066);
	if (shm == -1) {
		ON_ERR(errno);
		goto out;
	}

	/* Truncate shared memory to our ring buffer structure size */
	if (ftruncate(shm_fd, sizeof(struct ring_buffer))) {
		ON_ERR(errno);
		goto err_link;
	}

	/* Map structure to opened shared memory */
	r_buffer = (struct ring_buffer *)mmap(NULL, sizeof(struct ring_buffer),
		   PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (!r_buffer) {
		ON_ERR(errno);
		goto err_link;
	}

	return r_buffer;

err_link:
	shm_unlink(SHM_NAME);
out:
	return NULL;
}

/*
 * Unamp the shared memory. This will be called by reading/writing
 * processes when exit.
 */
void ring_buffer_open(struct ring_buffer *r_buffer)
{
	int shm_fd;

	if (r_buffer) {
		shm_unlink(SHM_NAME);
		munmap(r_buffer, sizeof(struct ring_buffer));
	}
}

int ring_buffer_put(struct ring_buffer *r_buffer, void *addr)
{
	/* buffer full */
	if (r_buffer->head - r_buffer->tail == r_buffer->size)
		return -1;

	memcpy(r_buffer->buffer[r_buffer->head], addr, r_buffer->elem_size);
	
}
