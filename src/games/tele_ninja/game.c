#include "game.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <seed/assets.h>
#include <seed/atlas.h>
#include <seed/camera.h>
#include <seed/log.h>
#include <seed/render.h>
#include <seed/sprite.h>
#include <seed/stretchy_buffer.h>
#include <seed/texture.h>

#include "entity.h"
#include "fps.h"
#include "random.h"

#include "tilemap.h"

static tilemap s_tile_map;
static atlas s_dirt_atlas;

static sprite s_cowboy_sprite;

static sprite s_jurassic_background_sprite;
static renderer* s_renderer;

bool game_init(GLFWwindow* window,
               uint32_t virtual_width, uint32_t virtual_height)
{
        assert(window);

        // Todo: Load shaders path from config file.
        s_renderer = render_create(window,
                                   virtual_width, virtual_height,
                                   "data/shaders/vertex.glsl",
                                   "data/shaders/fragment.glsl");
        if (!s_renderer) {
                LOGERR("%s", "Failed to initialize renderer");
                return false;
        }

        assets_init();
        Fps_init();


        s_jurassic_background_sprite.scale = 1.0f;
        s_jurassic_background_sprite.depth = CHAR_MAX;
        s_jurassic_background_sprite.tex = assets_get_texture("data/maps/jurassic/background.png");
        s_jurassic_background_sprite.tex = assets_get_texture("data/maps/jurassic/background.png");

        s_cowboy_sprite.scale = 0.15f;
        s_cowboy_sprite.depth = 1;
        s_cowboy_sprite.x_pos = 100;
        s_cowboy_sprite.y_pos = 95;
        s_cowboy_sprite.tex = assets_get_texture("data/characters/cowboy/cowboy.png");
        //atlas_init(&s_atlas, &s_player_texture, "data/anims/walk_cycle.txt");
        //atlas_sprite_name(&s_atlas, &s_player_sprite1, "walk_cycle_1.png", 50, 50, 0, 0, 1.0f, 0);
        //atlas_sprite_name(&s_atlas, &s_player_sprite2, "walk_cycle_0.png", 250, 100, 0, 0, 1.0f, 0);
        //s_player_sprite1.depth = 10;
        //s_player_sprite2.depth = 10;

        
        atlas_init(&s_dirt_atlas, 
                   assets_get_texture("data/maps/jurassic/jurassic_atlas.png"), 
                   "data/maps/jurassic/jurassic_atlas.txt");
        tilemap_init(&s_tile_map, &s_dirt_atlas, "data/maps/jurassic/jurassic_map.json");

        return true;
}

void game_update(double dt)
{
        Fps_log();

        render_add_sprite(s_renderer, &s_jurassic_background_sprite);
        render_add_sprite(s_renderer, &s_cowboy_sprite);
        render_add_sprites(s_renderer, s_tile_map.sprite_sb, sb_count(s_tile_map.sprite_sb));

        render_submit(s_renderer);
}

void game_cleanup(void)
{
        assets_reset(s_renderer);
        render_free(s_renderer);
}

void game_mouse_moved(double x_pos, double y_pos)
{}

void game_mouse_pressed()
{
}

void game_key_pressed(int key)
{
}

void game_key_released(int key)
{

}
