#include <chipmunk/chipmunk.h>

const cpFloat PI = 3.141592654;

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP    = 32;

const int TARGET_FPS    = 60;
const cpFloat TARGET_SEC_PER_FRAME = 0.0166667;

const cpLayers L_PLAYER = 1 | 2;
const cpLayers L_SWITCH = 2;
const cpLayers L_GROUND = 1;

