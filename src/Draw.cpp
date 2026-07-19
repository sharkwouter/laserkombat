#include "Draw.h"

#include "utils.h"
#include "constants.h"

Draw::Draw(SDL_Renderer * renderer) : renderer(renderer) {
    animation = 0;
    font_custom = TTF_OpenFont(getAssetPath("fonts", "FreeSans.ttf").c_str(), 14);
    font_text = TTF_OpenFont(getAssetPath("fonts", "FreeSerifItalic.ttf").c_str(), 24);
    font_title = TTF_OpenFont(getAssetPath("fonts", "FreeSerifItalic.ttf").c_str(), 28);
}

Draw::~Draw() {
    TTF_CloseFont(font_custom);
    TTF_CloseFont(font_text);
    TTF_CloseFont(font_title);
}

void Draw::BlitSquare(SDL_Texture * texture, int x, int y, int dx, int dy) {
    x *= BLOCK_SIZE;
    y *= BLOCK_SIZE;
    dx *= BLOCK_SIZE;
    dy *= BLOCK_SIZE;
    SDL_Rect sr={x,y,BLOCK_SIZE, BLOCK_SIZE};
    SDL_Rect dr={dx,dy,BLOCK_SIZE, BLOCK_SIZE};
    SDL_RenderCopy(renderer, texture, &sr, &dr);
}

void Draw::BlitWater(SDL_Texture * texture, int rotation, int dx, int dy) {
    // Slow down the water by half for small block sizes
    if (BLOCK_SIZE < 20) {
        rotation /= 2;
    }
    rotation = rotation % BLOCK_SIZE;
    dx *= BLOCK_SIZE;
    dy *= BLOCK_SIZE;
    SDL_Rect dr={dx + rotation, dy, BLOCK_SIZE - rotation, BLOCK_SIZE};
    SDL_Rect sr={0, 0, BLOCK_SIZE - rotation, BLOCK_SIZE};
    SDL_RenderCopy(renderer, texture, &sr, &dr);
    if (rotation > 0) {
        SDL_Rect dr2={dx, dy, rotation, BLOCK_SIZE};
        SDL_Rect sr2={BLOCK_SIZE - rotation, 0, rotation, BLOCK_SIZE};
        SDL_RenderCopy(renderer, texture, &sr2, &dr2);
    }
}

void Draw::BlitOther(SDL_Texture * texture, int x, int y, int dx, int dy, int w, int h) {
    SDL_Rect sr={x,y,w,h};
    SDL_Rect dr={dx,dy,w, h};
    SDL_RenderCopy(renderer, texture, &sr, &dr);
}

void Draw::BlackSquare(int x, int y) {
    x *= BLOCK_SIZE;
    y *= BLOCK_SIZE;
    SDL_Rect r={x,y,BLOCK_SIZE,BLOCK_SIZE};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &r);
}

void Draw::BlitBeam(int rotation, int x, int y) {
    x *= BLOCK_SIZE;
    y *= BLOCK_SIZE;
    SDL_Rect r={0, 0, 0, 0};
    switch(rotation) {
        case 0:
            r.x = x;
            r.y = y + (BLOCK_SIZE / 2) - 1;
            r.w = BLOCK_SIZE / 2 ;
            r.h = 2;
            break;
        case 1:
            r.x = x + (BLOCK_SIZE / 2) - 1;
            r.y = y;
            r.w = 2;
            r.h = BLOCK_SIZE / 2;
            break;
        case 2:
            r.x = x + BLOCK_SIZE / 2;
            r.y = y + (BLOCK_SIZE / 2) - 1;
            r.w = BLOCK_SIZE / 2;
            r.h = 2;
            break;
        case 3:
            r.x = x + (BLOCK_SIZE / 2) - 1;
            r.y = y + (BLOCK_SIZE / 2);
            r.w = 2;
            r.h = BLOCK_SIZE;
            break;
        default:
            return;
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &r);
}

void Draw::BlitMessage(const char *title, const char **lines, int line_count) {
    int widest_line = 0;
    int text_height = 0;
    SDL_Surface * title_surface = TTF_RenderText_Solid(font_title, title, {0, 0, 0, 255});
    if (title_surface == NULL) {
        SDL_Log("Couldn't create surface for text %s: %s", title, TTF_GetError());
        return;
    }
    text_height += title_surface->h;
    if (title_surface->w > widest_line)
        widest_line = title_surface->w;

    SDL_Texture * title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
    SDL_FreeSurface(title_surface);
    if (!title_texture) {
        SDL_Log("Couldn't create texture for text %s: %s", title, TTF_GetError());
        return;
    }

    // Draw each line of text
    SDL_Texture ** line_textures = (SDL_Texture **) SDL_calloc(line_count, sizeof(SDL_Texture *));
    for (int i = 0; i < line_count; i++) {
        SDL_Surface * surface = TTF_RenderText_Solid(font_text, lines[i], {0, 0, 0, 255});
        if (surface == NULL) {
            SDL_Log("Couldn't create surface for line %s: %s", lines[i], TTF_GetError());
            return;
        }
        text_height += surface->h;
        if (surface->w > widest_line)
            widest_line = surface->w;

        line_textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!line_textures[i]) {
            SDL_Log("Couldn't create texture for line %s: %s", lines[i], TTF_GetError());
            return;
        }
    }

    // Draw outside box
    SDL_Rect box = {(COLUMNS * BLOCK_SIZE / 2) - ((widest_line + BLOCK_SIZE / 2) / 2), (ROWS * BLOCK_SIZE / 2) - ((text_height + BLOCK_SIZE / 2) / 2), widest_line + BLOCK_SIZE / 2, text_height + BLOCK_SIZE / 2};
    BlitMessageBox(&box);

    // Draw text
    SDL_Rect title_rect = {box.x  + box.w / 2, box.y + BLOCK_SIZE / 4, 0, 0};
    SDL_QueryTexture(title_texture, NULL, NULL, &title_rect.w, &title_rect.h);
    title_rect.x -= title_rect.w / 2;
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);

    int current_text_y = title_rect.y + title_rect.h;
    for (int i = 0; i < line_count; i++) {
        SDL_Rect line_rect = {box.x + box.w / 2, current_text_y, 0, 0};
        SDL_QueryTexture(line_textures[i], NULL, NULL, &line_rect.w, &line_rect.h);
        line_rect.x -= line_rect.w / 2;
        current_text_y += line_rect.h;
        SDL_RenderCopy(renderer, line_textures[i], NULL, &line_rect);
    }

    // Clean up
    SDL_DestroyTexture(title_texture);
    for (int i = 0; i < line_count; i++) {
        SDL_DestroyTexture(line_textures[i]);
    }
    free(line_textures);
}

void Draw::BlitMessageBox(SDL_Rect *box) {
    SDL_SetRenderDrawColor(renderer, 87, 247, 249, 255);

    // Top 2 lines
    SDL_RenderDrawLine(renderer, box->x, box->y, box->x + box->w - 1, box->y);
    SDL_RenderDrawLine(renderer, box->x, box->y + 1, box->x + box->w - 2, box->y + 1);

    // Left 2 lines
    SDL_RenderDrawLine(renderer, box->x, box->y + 2, box->x, box->y + box->h - 1);
    SDL_RenderDrawLine(renderer, box->x + 1, box->y + 2, box->x + 1, box->y + box->h - 2);

    SDL_SetRenderDrawColor(renderer, 2, 59, 60, 255);
    // Bottom 2 lines
    SDL_RenderDrawLine(renderer, box->x + 1, box->y + box->h - 1, box->x + box->w, box->y + box->h - 1);
    SDL_RenderDrawLine(renderer, box->x, box->y + box->h, box->x + box->w, box->y + box->h);

    // Right 2 lines
    SDL_RenderDrawLine(renderer, box->x + box->w - 1, box->y + 1, box->x + box->w - 1, box->y + box->h - 2);
    SDL_RenderDrawLine(renderer, box->x + box->w, box->y, box->x + box->w, box->y + box->h - 2);

    SDL_SetRenderDrawColor(renderer, 3, 103, 107, 255);
    for (int i = 0; i < 4; i++){
        SDL_Rect current_line_box = {
            box->x + 2 + i,
            box->y + 2 + i,
            box->w - 3 - (i * 2),
            box->h - 3 - (i * 2),
        };
        SDL_RenderDrawRect(renderer, &current_line_box);
    }

    // Render inside lines
    SDL_SetRenderDrawColor(renderer, 2, 59, 60, 255);
    SDL_RenderDrawLine(renderer, box->x + 6, box->y + 6, box->x + box->w - 6, box->y + 6);
    SDL_RenderDrawLine(renderer, box->x + 6, box->y + 7, box->x + 6, box->y + box->h - 6);

    SDL_SetRenderDrawColor(renderer, 87, 247, 249, 255);
    SDL_RenderDrawLine(renderer, box->x + 7, box->y + box->h - 6, box->x + box->w - 6, box->y + box->h - 6);
    SDL_RenderDrawLine(renderer, box->x + box->w - 6, box->y + 7, box->x + box->w - 6, box->y + box->h - 7);

    // Render background
    SDL_SetRenderDrawColor(renderer, 3, 103, 107, 255);
    SDL_Rect background_rect = {
        box->x + 7,
        box->y + 7,
        box->w - 13,
        box->h - 13
    };
    SDL_RenderFillRect(renderer, &background_rect);
}

void Draw::BlitText(char * text, int x, int y) {
    SDL_Surface * surface = TTF_RenderText_Solid(font_custom, text, {0, 255, 0, 255});
    if (surface == NULL) {
        SDL_Log("Couldn't create surface for text %s: %s", text, TTF_GetError());
        return;
    }

    SDL_Texture * texture = NULL;
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    if (texture) {
        SDL_Rect rect = {x, y, 0, 0};
        SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
        rect.x -= rect.w/2;
        rect.y -= rect.h/2;
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
    }
}



void Draw::Flip() {
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}