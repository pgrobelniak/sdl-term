#include <stdlib.h>
#include <SDL.h>

#include "scancodes.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

SDL_Window *window;
SDL_Renderer *renderer;

int shift = 0;
int ctrl = 0;

void setup() {
    SDL_Init(SDL_INIT_EVERYTHING);
    if(SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) < 0) {
        fprintf(stderr, "%s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetWindowTitle(window, "Vortex");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
}

void keydown(SDL_Scancode scancode, int repeat) {
    switch(scancode){
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
    switch(scancode){
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
    while(SDL_WaitEvent(&ev) >= 0){
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
