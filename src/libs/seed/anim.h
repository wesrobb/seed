#pragma once

#include <stdbool.h>

typedef struct anim {
        struct spSkeleton* skeleton;
        struct spSkeletonData* skeleton_data;
        struct spAnimationState* state;
        struct spAtlas* atlas;
        float time_scale;
        struct sprite* sprite_sb;
        bool ownsAnimStateDate;
        float* world_vert_sb;
} anim;


/// Initializes the animation from the specified spin files.
bool anim_init(anim*, const char* spine_atlas_path, const char* spine_json_path);

/// Frees all allocated resources in the animation. Does not free the animation itself.
void anim_reset(anim*);