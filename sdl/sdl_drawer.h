#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftimage.h>
#include <codecvt>
#include <locale>
#include <vector>
#include <iostream>
#include "shape_drawer.h"
#include "sdl_drawer_interface.h"
#include <unistd.h>

using namespace std;
namespace simple_browser_sdldrawer {

struct ResourcesStruct {
    SDL_Window *window;
    SDL_Renderer *render;
    SDL_Event event;
    SDL_Surface* white_surface;
    SDL_Texture* white_texture;
    SDL_Rect white_rect;

    FT_Library ft_lib;
    FT_Bitmap ft_target;
    FT_Face ft_face;
    SDL_Color font_colors[256];
    SDL_Rect font_rect;
    int font_size;
    struct font_baseline_position {
        int x;
        int y;
    } font_baseline_position;

    ResourcesStruct() {
        clear();
    }
    void clear() {
        memset(this, 0, sizeof(struct ResourcesStruct));
    }

};

class SdlDrawer {
private:
    struct ResourcesStruct res;
    int window_width;
    int window_height;

public:
    SdlDrawer(int window_width, int window_height):
    window_width(window_width), window_height(window_height) {
        SDL_Init(SDL_INIT_VIDEO);
    }
    ~SdlDrawer() {
        release_sdldrawer();
        SDL_Quit();
    }

    bool init_sdldrawer() {
        string message;

        if (!(res.window = SDL_CreateWindow("Test", 
            SDL_WINDOWPOS_CENTERED, 0,
            window_width, window_height, SDL_WINDOW_SHOWN))) {
            message = "window";
            goto out;
        }
        if (!(res.render = SDL_CreateRenderer(res.window, -1, SDL_RENDERER_ACCELERATED))) {
            message = "render";
            goto out;
        }
        if (!(res.white_surface = IMG_Load("/home/liut/my/browser/sdl/white.bmp"))) {
            message = "white surface";
            goto out;
        }
        if (!(res.white_texture = SDL_CreateTextureFromSurface(res.render, res.white_surface))) {
            message = "white texture";
            goto out;
        }
        SDL_RenderClear(res.render);
        SDL_SetRenderDrawBlendMode(res.render, SDL_BLENDMODE_BLEND);

        FT_Init_FreeType(&res.ft_lib);
        FT_New_Face(res.ft_lib, "/home/liut/my/browser/sdl/wqy-zenhei.ttc", 0, &res.ft_face);
        for( int i = 0; i < 256; i++)
        {
            res.font_colors[i].r = res.font_colors[i].g = res.font_colors[i].b = i;
        }

        res.font_baseline_position.x = 0;
        res.font_baseline_position.y = 0;

        res.white_rect.x = 0;
        res.white_rect.y = 0;
        res.white_rect.w = window_width;
        res.white_rect.h = window_height;
        return true;
    out:
        printf("Initial sdl %s error!\n", message.c_str());
        return false;
    }

    void release_sdldrawer() {
        if (res.render) {
            SDL_DestroyRenderer(res.render);
        }
        if (res.window) {
            SDL_DestroyWindow(res.window);
        }
        if (res.white_surface) {
            SDL_FreeSurface(res.white_surface);
        }
        if (res.white_texture) {
            SDL_DestroyTexture(res.white_texture);
        }

        FT_Done_Face(res.ft_face);
        FT_Done_FreeType(res.ft_lib);
        res.clear();
    }

    void run_sdldrawer(const SdlDrawerInterface& drawer_interface) {
        int wheel_offset = 0;
        bool quit = false;
    
        SdlRectDrawer drawer(res.render, res.white_texture);

        while(!quit) {
            while (SDL_PollEvent(&res.event)) {
                if (res.event.type == SDL_QUIT) {
                    quit = true;
                }
            }
            SDL_Delay(100);
            SDL_RenderCopy(res.render, res.white_texture, nullptr, &res.white_rect);
            for(int i = 0; i < drawer_interface.rect_list.size(); ++i) {
                drawer.draw(drawer_interface.rect_list[i]);
                // cout << "fff" << endl;
                // sleep(1);
                // SDL_RenderPresent(res.render);
            }
            
            SDL_RenderPresent(res.render);
        }
    }


};

};