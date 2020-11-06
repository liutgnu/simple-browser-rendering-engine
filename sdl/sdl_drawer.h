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

    ResourcesStruct() {
        clear();
    }
    void clear() {
        memset(this, 0, sizeof(struct ResourcesStruct));
    }

};

class SdlDrawer {
private:
    int window_width;
    int window_height;

public:
    struct ResourcesStruct res;
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

        if (!(res.window = SDL_CreateWindow("My Awesome Browser", 
            SDL_WINDOWPOS_CENTERED, 0,
            window_width, window_height, SDL_WINDOW_SHOWN))) {
            message = "window";
            goto out;
        }
        if (!(res.render = SDL_CreateRenderer(res.window, -1, SDL_RENDERER_ACCELERATED))) {
            message = "render";
            goto out;
        }
        if (!(res.white_surface = IMG_Load("./white.bmp"))) {
            message = "white surface";
            goto out;
        }
        if (!(res.white_texture = SDL_CreateTextureFromSurface(res.render, res.white_surface))) {
            message = "white texture";
            goto out;
        }
        SDL_RenderClear(res.render);
        SDL_SetRenderDrawBlendMode(res.render, SDL_BLENDMODE_BLEND);

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
            }

            for (int i = 0; i < drawer_interface.font_list.size(); ++i) {
                SDL_SetRenderDrawColor(res.render, 
                    drawer_interface.font_list[i].background_color.r,
                    drawer_interface.font_list[i].background_color.g,
                    drawer_interface.font_list[i].background_color.b,
                    drawer_interface.font_list[i].background_color.a
                    );
                SDL_RenderFillRect(res.render, &drawer_interface.font_list[i].background_rect);
                SDL_RenderCopy(res.render, drawer_interface.font_list[i].font_texture,
                    nullptr, &drawer_interface.font_list[i].font_rect);
            }
            
            SDL_RenderPresent(res.render);
        }
    }
};

};