#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

using namespace std;
typedef struct RectEdge {
    int t;
    int r;
    int b;
    int l;
} Rect_Edge;

typedef struct HtmlRect {
    SDL_Rect rect;
    Rect_Edge edge;
    SDL_Color color;
} Html_Rect;

class SdlShapeDrawer {
protected:
    SDL_Renderer *render = nullptr;
    SDL_Texture *white_texture = nullptr;
public:
    SdlShapeDrawer(SDL_Renderer *render, SDL_Texture *white_texture):
        render(render), white_texture(white_texture) {}
};

class SdlRectDrawer: SdlShapeDrawer {
public:

    SdlRectDrawer(SDL_Renderer *render, SDL_Texture *white_texture):
        SdlShapeDrawer(render, white_texture) {}

    void draw(Html_Rect html_rect) {
        SDL_Color origin_color;
        SDL_GetRenderDrawColor(render, 
            &origin_color.r, &origin_color.g, &origin_color.b, &origin_color.a);

        SDL_Rect rect_t, rect_l, rect_r, rect_b;
        html_rect.edge.t = html_rect.edge.t > 0 ? html_rect.edge.t : 0;
        html_rect.edge.r = html_rect.edge.r > 0 ? html_rect.edge.r : 0;
        html_rect.edge.b = html_rect.edge.b > 0 ? html_rect.edge.b : 0;
        html_rect.edge.l = html_rect.edge.l > 0 ? html_rect.edge.l : 0;

        rect_t.x = html_rect.rect.x;
        rect_t.y = html_rect.rect.y;
        rect_t.w = html_rect.rect.w;
        rect_t.h = html_rect.edge.t;

        rect_l.x = html_rect.rect.x;
        rect_l.y = html_rect.rect.y;
        rect_l.w = html_rect.edge.l;
        rect_l.h = html_rect.rect.h;

        rect_b.x = html_rect.rect.x + html_rect.rect.w;
        rect_b.y = html_rect.rect.y + html_rect.rect.h;
        rect_b.w = -html_rect.rect.w;
        rect_b.h = -html_rect.edge.b;

        rect_r.x = html_rect.rect.x + html_rect.rect.w;
        rect_r.y = html_rect.rect.y + html_rect.rect.h;
        rect_r.w = -html_rect.edge.r;
        rect_r.h = -html_rect.rect.h;

        SDL_SetRenderDrawColor(render, html_rect.color.r, 
            html_rect.color.g, html_rect.color.b, html_rect.color.a);

        SDL_RenderFillRect(render, &rect_l);
        SDL_RenderFillRect(render, &rect_t);
        SDL_RenderFillRect(render, &rect_r);
        SDL_RenderFillRect(render, &rect_b);
        SDL_SetRenderDrawColor(render, 
            origin_color.r, origin_color.g, origin_color.b, origin_color.a);
    }
};