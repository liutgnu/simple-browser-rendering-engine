#include "font_manager.h"
#include <codecvt>                                                                                                                                                          
#include <locale>

using namespace std;
FontManager::FontManager() {
    FT_Init_FreeType(&ft_lib);
    FT_New_Face(ft_lib, "../utils/wqy-zenhei.ttc", 0, &ft_face);
    restore_default_font_size();
}

FontManager::~FontManager() {
    FT_Done_Face(ft_face);
    FT_Done_FreeType(ft_lib);
    memset(this, sizeof(FontManager), 0);
}

void FontManager::set_font_size(int size) {
    font_size = size;
    FT_Set_Pixel_Sizes(ft_face, font_size, font_size);
}

void FontManager::restore_default_font_size() {
    set_font_size(DEFAULT_FONT_SIZE);
}

tuple<int, int> FontManager::get_string_width_length(wstring str) {
    if (str.size() > 0) {
        int x = 0;
        for (auto it = str.cbegin(); it != str.cend(); ++it) {
            if (*it == ' ' || *it == '\r' || *it == '\n') {
                x += (font_size >> 2);
                continue;
            }
            FT_Load_Char(ft_face, *it, FT_LOAD_RENDER);
            x += ft_face->glyph->metrics.horiAdvance >> 6;
        }
        return make_tuple(x, (ft_face->size->metrics.ascender - ft_face->size->metrics.descender) >> 6);
    } else {
        return make_tuple(0, 0);
    }
}

void FontManager::draw_font(wchar_t ch, Font_Color color, Font_Position& position, 
    vector<Font_Draw_Info>& font_list, SDL_Renderer *render) {
    Font_Draw_Info info;
    SDL_Surface *font_surface = nullptr;

    memset(&info, sizeof(info), 0);
    FT_Load_Char(ft_face, ch, FT_LOAD_RENDER);

    vector<unsigned char> font_buf(
        ft_face->glyph->bitmap.width * ft_face->glyph->bitmap.rows * 4);
    for(int y = 0; y < ft_face->glyph->bitmap.rows; ++y) {
        for(int x = 0; x < ft_face->glyph->bitmap.width; ++x) {
            unsigned char val = ft_face->glyph->bitmap.buffer[
                (ft_face->glyph->bitmap.pitch * y) + x];
            int base = ((ft_face->glyph->bitmap.width * y) + x) * 4;
            font_buf[base] = color.font_color.r;
            font_buf[base + 1] = color.font_color.g;
            font_buf[base + 2] = color.font_color.b;
            font_buf[base + 3] = val;
        }
    }

    if (!(font_surface = SDL_CreateRGBSurfaceFrom(
        &font_buf[0], 
        ft_face->glyph->bitmap.width, 
        ft_face->glyph->bitmap.rows, 
        32, 
        ft_face->glyph->bitmap.pitch * 4,
        0x000000ff,
        0x0000ff00,
        0x00ff0000,
        0xff000000
        )))
        goto out;
    // SDL_SetSurfaceBlendMode(font_surface, SDL_BlendMode::SDL_BLENDMODE_BLEND);
    if (!(info.font_texture = SDL_CreateTextureFromSurface(render, font_surface))) {
        goto out;
    }
    SDL_QueryTexture(info.font_texture, nullptr, nullptr, &info.font_rect.w, &info.font_rect.h);
    info.font_rect.x = position.x + ft_face->glyph->bitmap_left;
    info.font_rect.y = position.y - (ft_face->glyph->metrics.horiBearingY >> 6);

    info.background_rect.x = position.x;
    info.background_rect.y = position.y - (ft_face->size->metrics.ascender >> 6);
    info.background_rect.h = (ft_face->size->metrics.ascender - ft_face->size->metrics.descender) >> 6;
    info.background_rect.w = ft_face->glyph->metrics.horiAdvance >> 6;
    info.background_color = color.background_color;

    position.x += ft_face->glyph->metrics.horiAdvance >> 6;
    font_list.push_back(info);
out:
    if (font_surface) {
        SDL_FreeSurface(font_surface);;
    }
}

void FontManager::draw_string(wstring str, Font_Color color, Font_Position start_position, 
    vector<Font_Draw_Info>& font_list, SDL_Renderer *render)
{
    Font_Position position = start_position;
    for (auto it = str.begin(); it != str.end(); ++it) {
        draw_font(*it, color, position, font_list, render);
    }
}

wstring FontManager::convert_to_wstring(string str) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}