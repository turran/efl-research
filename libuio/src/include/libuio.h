#ifndef LIBUIO_H_
#define LIBUIO_H_

/* for now place hal info here */
/* maybe place the uio stuff in another header ? */
#define MAX_NAME_SIZE 64
#define MAX_MAPS 5

typedef struct _Uio_Map
{
	unsigned long addr; /* the uio memory address */
	int size; /* the size of the memory */
	void *ptr; /* the mmaped address */
} Uio_Map;

typedef struct _Uio_Device
{
	int fd;
	char name[MAX_NAME_SIZE];
	char version[MAX_NAME_SIZE];
	int number;
	Uio_Map maps[MAX_MAPS];
	int maps_num;
} Uio_Device;

Uio_Device * uio_open(const char *name);
void * uio_map(Uio_Device *d, int map);
void uio_unmap(Uio_Device *d, int map);
void uio_close(Uio_Device *d);
void uio_dump(Uio_Device *d);

#endif /*LIBUIO_H_*/
