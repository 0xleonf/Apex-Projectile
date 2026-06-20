#include "vector.cpp"
#include <cstdlib>
#include <ctime>

struct Star {
    Vector2D position;
    bool isCollected;
    char symbol;
};

// Inisialisasi star dengan simbol bintang
void initStar(Star *s) {
    s->isCollected = false;
    s->symbol = '*';
}

// Random posisi bintang di sisi kanan layar
// WIDTH = 70, HEIGHT = 20, area ketapel di kiri (x < ~15)
void randomStarPosition(Star *s) {
    srand(time(0));  // seed random
    
    int minX = 20;   // jauh dari area ketapel (kiri)
    int maxX = 68;   // batas kanan layar (WIDTH - 2)
    int minY = 1;
    int maxY = 18;   // batas bawah layar (HEIGHT - 2)

    s->position.x = minX + (rand() % (maxX - minX + 1));
    s->position.y = minY + (rand() % (maxY - minY + 1));
}