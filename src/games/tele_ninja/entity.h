#pragma once

#include <stdbool.h>

/// Represents an object in the world.
/// Members are used selectively depending on object requirements.
typedef struct Entity {
        bool active;
} Entity;

/// Returns a pointer to an unused entity. NULL if all entities
/// are currently in use.
Entity* Entity_new(void);

/// Marks the entity at the specified index as unused.
void Entity_free(Entity*);