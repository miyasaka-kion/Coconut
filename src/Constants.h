#include <box2d/box2d.h>

const int MET2PIX = 10; // 640 / 80 = 8

const int WIDTH = 1000;
const int HEIGHT = 700;

const int SCALED_WIDTH = WIDTH / MET2PIX; // 4 | 3
const int SCALED_HEIGHT = HEIGHT / MET2PIX;

//1 rad × 180/π = 57,296°
const float RAD2DEG = 180 / M_PI;