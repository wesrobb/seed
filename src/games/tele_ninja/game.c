#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <log/log.h>

#include <render/camera.h>
#include <render/render.h>
#include <render/sprite.h>
#include <render/spritebatch.h>
#include <render/texture.h>

#include "entity.h"
#include "fps.h"
#include "random.h"

#include "tilemap.h"

static TileMap s_tile_map;
static SpriteBatch s_player;
static Sprite* s_player_sprite;
static int walk_cycle_index = 0;

bool Game_init(uint32_t screen_width, uint32_t screen_height,
               uint32_t virtual_width, uint32_t virtual_height)
{
        // Todo: Load shaders path from config file.
        if (!Render_init(screen_width, screen_height,
                         virtual_width, virtual_height,
                         "data/shaders/vertex.glsl",
                         "data/shaders/fragment.glsl")) {
                LOGERR("%s", "Failed to initialize renderer");
                return false;
        }

        Fps_init();
        
        TileMap_init(&s_tile_map, "data/maps/DirtMap.txt",
                                  "data/maps/DirtSprites.txt",
                                  "data/maps/DirtSprites.png");
                          
        SpriteBatch_init(&s_player, "data/anims/walk_cycle.txt",
                                    "data/anims/walk_cycle.png");
        s_player_sprite = SpriteBatch_addSpriteID(&s_player, walk_cycle_index, 
                                                  300, 150, 0, 0, 20, 40, 0);
        

        return true;
}

void Game_update(double dt)
{
}

void Game_render(double interpolation)
{
        interpolation = 0;
        Render_begin();
        Fps_log();
        TileMap_draw(&s_tile_map);
        SpriteBatch_draw(&s_player);
        Render_end();
}

void Game_cleanup(void)
{
        Render_free();
}

void Game_mouseMoved(double x_pos, double y_pos)
{
}

void Game_mousePressed()
{
        walk_cycle_index = ++walk_cycle_index % 2;
        SpriteBatch_removeSprite(&s_player, &s_player_sprite);
        s_player_sprite = SpriteBatch_addSpriteID(&s_player, walk_cycle_index,
                                                  300, 150, 0, 0, 20, 40, 0);
}

