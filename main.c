#include <stdlib.h>
#include <SDL.h>

#include "scancodes.h"
#include "vt52rom.h"

#define TERM_WIDTH 80
#define TERM_HEIGHT 24

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 20

#define WINDOW_WIDTH TERM_WIDTH*CHAR_WIDTH // 640
#define WINDOW_HEIGHT TERM_HEIGHT*CHAR_HEIGHT // 480

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *fonttex[128];

int shift = 0;
int ctrl = 0;

void createChar(Uint32 *raster, int c) {
    int i, j;
    Uint8 *chr = &vt52rom[c*8];
    memset(raster, 0, CHAR_WIDTH*CHAR_HEIGHT*sizeof(Uint32));
    for(i = 0; i < 8; i++) {
        for(j = 0; j < 7; j++) {
            if(chr[i]&(0100>>j)) {
                raster[(i*2+0)*CHAR_WIDTH + j] = 0xFF;
                raster[(i*2+1)*CHAR_WIDTH + j] = 0xFF;
            }
        }
    }
}

void createFont() {
    int w = CHAR_WIDTH;
    int h = CHAR_HEIGHT;
    Uint32 *raster = malloc(w*h*sizeof(Uint32));
    for(int i = 0; i < 128; i++) {
        createChar(raster, i);
        fonttex[i] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
        SDL_SetTextureBlendMode(fonttex[i], SDL_BLENDMODE_ADD);
        SDL_UpdateTexture(fonttex[i], NULL, raster, w*sizeof(Uint32));
    }
}

void setup() {
    SDL_Init(SDL_INIT_EVERYTHING);
    if(SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) < 0) {
        fprintf(stderr, "%s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetWindowTitle(window, "Vortex");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    createFont();
}

void keydown(SDL_Scancode scancode, int repeat) {
    switch(scancode) {
        case SDL_SCANCODE_LSHIFT:
        case SDL_SCANCODE_RSHIFT: 
            shift = 1; 
            return;
        case SDL_SCANCODE_CAPSLOCK:
        case SDL_SCANCODE_LCTRL:
        case SDL_SCANCODE_RCTRL: 
            ctrl = 1; 
            return;
        default: 
            break;
    }
    char key = scancodemap[scancode][shift];
    if(ctrl) {
        key &= 037;
    }
}

void keyup(SDL_Scancode scancode) {
    switch(scancode) {
        case SDL_SCANCODE_LSHIFT:
        case SDL_SCANCODE_RSHIFT: 
            shift = 0; 
            return;
        case SDL_SCANCODE_CAPSLOCK:
        case SDL_SCANCODE_LCTRL:
        case SDL_SCANCODE_RCTRL: 
            ctrl = 0; 
            return;
        default: 
            break;
    }
}

void loop() {
    SDL_Event ev;
    while(SDL_WaitEvent(&ev) >= 0) {
        switch(ev.type){
            case SDL_QUIT:
                return;
            case SDL_KEYDOWN:
                keydown(ev.key.keysym.scancode, ev.key.repeat);
                break;
            case SDL_KEYUP:
                keyup(ev.key.keysym.scancode);
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    setup();
    loop();
    SDL_Quit();
    return 0;
}
