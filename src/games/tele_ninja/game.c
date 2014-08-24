#include "game.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <log/log.h>

#include <render/atlas.h>
#include <render/camera.h>
#include <render/render.h>
#include <render/sprite.h>
#include <render/texture.h>

#include "entity.h"
#include "fps.h"
#include "random.h"

#include "tilemap.h"

//static tilemap s_tile_map;
//static spritebatch s_player;
static atlas s_atlas;
static sprite s_player_sprite;
static texture s_player_texture;
static int walk_cycle_index = 0;
static renderer* s_renderer;

bool Game_init(GLFWwindow* window,
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

        Fps_init();

        texture_init(&s_player_texture, "data/anims/walk_cycle.png");
        atlas_init(&s_atlas, &s_player_texture, "data/anims/walk_cycle.txt");
        atlas_sprite_name(&s_atlas, &s_player_sprite, "walk_cycle_1.png", 50, 50, 0, 0, 50, 50, 0);
        //s_player_sprite.x_pos = 50;
        //s_player_sprite.y_pos = 50;
        //s_player_sprite.width = 100;
        //s_player_sprite.height = 50;
        //s_player_sprite.tex = &s_player_texture;

        /*tilemap_init(&s_tile_map, "data/maps/DirtMap.txt",
                     "data/maps/DirtSprites.txt",
                     "data/maps/DirtSprites.png");*/

        /*spritebatch_init(&s_player, "data/anims/walk_cycle.txt",
                         "data/anims/walk_cycle.png");*/
        /*s_player_sprite = spritebatch_add_sprite_id(&s_player, walk_cycle_index,
                                                    300, 150, 0, 0, 20, 40, 0);*/


        return true;
}

void Game_update(double dt)
{
        render_add_sprite(s_renderer, &s_player_sprite);
        Fps_log();

        //tilemap_draw(&s_tile_map);
        //spritebatch_draw(&s_player);
        render_submit(s_renderer);
}

void Game_render(double interpolation)
{
}

void Game_cleanup(void)
{
        render_free(s_renderer);
}

void Game_mouseMoved(double x_pos, double y_pos)
{}

void Game_mousePressed()
{
        walk_cycle_index = ++walk_cycle_index % 2;
        /*spritebatch_remove_sprite(&s_player, &s_player_sprite);
        s_player_sprite = spritebatch_add_sprite_id(&s_player, walk_cycle_index,
                                                    300, 150, 0, 0, 20, 40, 0);*/
}

