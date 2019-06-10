//#include "efi_compat.h"
#include "Ctype.h"
#include "StdLib.h"
#include "String.h"

typedef struct {
	UINTN size;
	void *data[];
} malloc_t;

static void __realloc_copy(malloc_t *new, void *old) {
	malloc_t *old_head = container_of(old, malloc_t, data);
	size_t len = MIN(new->size, old_head->size);
	//LogDebug("copying %d bytes", len);
	memcpy(new->data, old_head->data, len);
}

void free(void *ptr) {
	//LogDebug("free(0x%x)", ptr);
	if(ptr) {
		malloc_t *head = container_of(ptr, malloc_t, data);
		//gBS->FreePool(head);
		CoreFree(head);
	}
}

void *realloc(void *ptr, size_t size) {
	//LogDebug("realloc(0x%x, %d)", ptr, size);
	malloc_t* new_head = (malloc_t*)CoreMalloc(size + sizeof(malloc_t));
	new_head->size = size;
	
	if(ptr) {
		__realloc_copy(new_head, ptr);
		free(ptr);
	}
	
	//LogDebug("realloc(0x%x, %d) -> 0x%x", ptr, size, new_head->data);
	return new_head->data;
}

void *malloc(size_t size) {
	return realloc(NULL, size);
}

void *calloc(size_t count, size_t size) {
	size_t total = count * size;
	void *p = realloc(NULL, total);
	if (p) {
		bzero(p, total);
	}
	return p;
}