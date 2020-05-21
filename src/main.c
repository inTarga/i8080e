#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "cpu.h"
#include "display.h"
#include "input.h"

typedef struct {
    Cpu_state *state;
    Display *display;
    Input *input;
} Arcade_system;

unsigned char *initalise_memory(char *rom_filename) {
    FILE *f = fopen(rom_filename, "rb");
    if (!f) {
        printf("Could not open %s\n", rom_filename);
        exit(1);
    }

    fseek(f, 0, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *memory = malloc(sizeof(unsigned char) * 16000);
    memset(memory, 0, 16000);

    fread(memory, 1, fsize, f);
    fclose(f);

    return memory;
}

int initalise_state(Cpu_state *state, char *rom_filename) {
    state->pc = 0;
    state->sp = 0;
    state->int_enable = 0;
    state->cc.ac = 0;
    state->cc.cy = 0;
    state->cc.z = 0;
    state->cc.s = 0;
    state->cc.p = 0;
    for (int i = 0; i < 7; i++)
        state->regs[i] = 0;
    state->memory = initalise_memory(rom_filename);
    return 0;
}

Arcade_system initialise_system() {
    Arcade_system system;

    system.state = malloc(sizeof(Cpu_state));
    /*
    Cpu_state state;
    initalise_state(&state, "rom/invaders");
    system.state = &state;
    */

    //system.display = Display display;
    system.display = malloc(sizeof(Display));
    //system.display = &(Display) {};

    return system;
}

int main() {
    Arcade_system system = initialise_system();
    //memset(&cabinet, 0, sizeof(Cabinet));


    initialise_SDL(system.display);

    //atexit(cleanup);

    bool done = false;
    while (!done) {
        prepareScene(system.display, system.state->memory);

        handleInput(system.input);

        done = emulate_op(system.state);

        presentScene(system.display);

        SDL_Delay(16);
    }

    //TODO free structs

    return 0;
}