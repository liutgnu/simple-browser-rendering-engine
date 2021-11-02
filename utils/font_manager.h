#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftimage.h>
#include <tuple>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#define DEFAULT_FONT_SIZE 16

typedef struct Font_Color {
    SDL_Color font_color;
    SDL_Color background_color;
} Font_Color;

typedef struct Font_Position {
    int x;
    int y;
} Font_Position;

typedef struct Font_Draw_Info {
    SDL_Rect font_rect;
    SDL_Texture *font_texture;
    SDL_Rect background_rect;
    SDL_Color background_color;
} Font_Draw_Info;

using namespace std;
class FontManager {
public:
    FT_Library ft_lib;
    FT_Bitmap ft_target;
    FT_Face ft_face;
    int font_size;

    FontManager();
    ~FontManager();
    void set_font_size(int size);
    void restore_default_font_size();
    tuple<int, int> get_string_width_length(wstring str);
    void draw_string(wstring str, Font_Color color, Font_Position start_position, 
        vector<Font_Draw_Info>& font_list, SDL_Renderer *render);
    wstring convert_to_wstring(string str);
private:
    void draw_font(wchar_t ch, Font_Color color, Font_Position& position, 
        vector<Font_Draw_Info>& font_list, SDL_Renderer *render);
};
