#include "entity.h"

#include <inttypes.h>
#include <stddef.h>
#include <string.h>

#define MAX_ENTITIES 5096

/// Global var for all code to access all entities.
static Entity _entities[MAX_ENTITIES];
/// ID of the next unused entity. -1 if no inactive entities.
static int32_t _next_entity;

Entity* Entity_new(void)
{
        if (_next_entity == -1) {
                return NULL;
        }
        
        int32_t new_entity = _next_entity;
        
        // Find the new next entity.
        // Search from new_entity to MAX_ENTITIES - 1
        while (++_next_entity < MAX_ENTITIES) {
                if (!_entities[_next_entity].active) {
                        return &_entities[new_entity];
                }
        }
        
        // Didn't find the next yet. Search from 0 to new_entity - 1.
        _next_entity = -1;
        while(++_next_entity < new_entity) {
                if (!_entities[_next_entity].active) {
                        return &_entities[new_entity];
                }
        }
        
        // No next entity :(
        _next_entity = -1;
        return &_entities[new_entity];
}

void Entity_free(Entity* e)
{
        ptrdiff_t i = e - &_entities[0];
        memset(&_entities[i], 0, sizeof(Entity));
        _next_entity = i;
}
