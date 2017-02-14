/*
 * Generic map manipulation functions
 *
 * Originally by Elliot C Back - http://elliottback.com/wp/map-implementation-in-c/
 *
 * Modified by Pete Warden to fix a serious performance problem, support strings as keys
 * and removed thread synchronization - http://petewarden.typepad.com
 */
#ifndef DZN_MAP_H
#define DZN_MAP_H

#define MAP_MISSING -3  /* No such element */
#define MAP_FULL -2 	/* Map is full */
#define MAP_OMEM -1 	/* Out of Memory */
#define MAP_OK 0 	/* OK */

/* We need to keep keys and values */
typedef struct {
	char* key;
	int in_use;
	void* data;
} map_element;

/* A map has some maximum size and current size,
 * as well as the data to hold. */
typedef struct {
	int table_size;
	int size;
	map_element *data;
} map;

typedef int (*map_f)(map_element*, void*);

extern void map_init(map* m);

/*
 * Iteratively call f with argument (item, data) for
 * each element data in the map. The function must
 * return a map status code. If it returns anything other
 * than MAP_OK the traversal is terminated. f must
 * not reenter any map functions, or deadlock may arise.
 */
extern int map_iterate(map* self, map_f f, void* item);

/*
 * Add an element to the map. Return MAP_OK or MAP_OMEM.
 */
extern int map_put(map* self, char* key, void* value);

/*
 * Get an element from the map. Return MAP_OK or MAP_MISSING.
 */
extern int map_get(map* self, char* key, void* *arg);

/*
 * Remove an element from the map. Return MAP_OK or MAP_MISSING.
 */
extern int map_remove(map* self, char* key);

/*
 * Get any element. Return MAP_OK or MAP_MISSING.
 * remove - should the element be removed from the map
 */
extern int map_get_one(map* self, void* *arg, int remove);

/*
 * Free the map
 */
extern void map_free(map* self);

/*
 * Get the current size of a map
 */
extern int map_length(map* self);

#endif /* DZN_MAP_H */
