#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <limits> 

using namespace std;

const float PI = 3.14159265f;
const float SCALE = 0.25f; 

const int WIDTH_CHARS = 60;
const int HEIGHT_CHARS = 20;
const int PADDING_LEFT = 40; 

const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m"; 
const string CYAN    = "\033[36m";
const string GRAY    = "\033[90m";
const string MAGENTA = "\033[35m";

struct Debris {
    float x, y;
    float vx, vy;
    char symbol;
    bool active = false;
};

struct Obstacle {
    float x, y;
    float vx, vy;
    bool isBroken = false;
    bool isTowerComponent = false; 
};

struct Ball {
    float x, y;
    float vx, vy;
    bool active = false;
};

bool checkCollision(Ball* b, Obstacle* obs) {
    if (obs->isBroken) return false;
    if (abs(b->x - obs->x) <= 1.2f * SCALE && abs(b->y - obs->y) <= 1.0f * SCALE) {
        return true;
    }
    return false;
}

void resetScreen(char screen[HEIGHT_CHARS][WIDTH_CHARS]) {
    for (int y = 0; y < HEIGHT_CHARS; y++) {
        for (int x = 0; x < WIDTH_CHARS; x++) {
            screen[y][x] = ' ';
        }
    }
}

void drawToScreen(char screen[HEIGHT_CHARS][WIDTH_CHARS], float worldX, float worldY, char symbol) {
    int screenX = (int)(worldX / SCALE);
    int screenY = (int)(worldY / SCALE);
    if (screenX >= 0 && screenX < WIDTH_CHARS && screenY >= 0 && screenY < HEIGHT_CHARS) {
        screen[screenY][screenX] = symbol;
    }
}

void printWithPadding(string text) {
    cout << string(PADDING_LEFT, ' ') << text;
}

void renderGameInterface(char screen[HEIGHT_CHARS][WIDTH_CHARS], int attempt, int maxAttempts) {
    cout << "\033[1;1H"; 
    cout << "\n\n"; 
    
    cout << string(PADDING_LEFT, ' ') << "+" << string(WIDTH_CHARS, '-') << "+\n";

    for (int y = 0; y < HEIGHT_CHARS; y++) {
        cout << string(PADDING_LEFT, ' ') << "|"; 
        for (int x = 0; x < WIDTH_CHARS; x++) {
            char ch = screen[y][x];
            if (ch == 'o') {
                cout << BOLD << RED << ch << RESET;      
            } else if (ch == '@') {
                cout << BOLD << GREEN << ch << RESET;    
            } else if (ch == '*') {
                cout << BOLD << MAGENTA << ch << RESET;  
            } else if (ch == '[' || ch == ']') {
                cout << YELLOW << ch << RESET;           
            } else if (ch == 'x' || ch == '+') {
                cout << GRAY << ch << RESET;             
            } else {
                cout << ch;                              
            }
        }
        cout << "|\n"; 
    }

    cout << string(PADDING_LEFT, ' ') << "+" << CYAN << string(WIDTH_CHARS, '=') << RESET << "+ " << CYAN << "(LANTAI)" << RESET << "\n";
    cout << "\n";
    printWithPadding(BOLD + "=== ANGRY BIRDS CLI: COLORED LAYOUT ===\n" + RESET);
    printWithPadding("KESEMPATAN KE: " + to_string(attempt) + " dari " + to_string(maxAttempts) + "\n");
}

int main() {
    srand(time(0));

    float g = 9.8f;   
    float dt = 0.05f; 

    int lantaiYChar = HEIGHT_CHARS - 2;
    int maxObstacles = 30; 
    Obstacle* tower = new Obstacle[maxObstacles]; 
    int activeObstacleCount = 0;

    int targetXChar = 42 + (rand() % 8); 
    int targetYChar = lantaiYChar; 
    
    int towerHeight = 2 + (rand() % 3); 
    int targetFloor = rand() % towerHeight;
    targetYChar = lantaiYChar - targetFloor;

    for (int tingkat = 0; tingkat < towerHeight; tingkat++) {
        if (rand() % 2 == 0 && activeObstacleCount < maxObstacles) {
            Obstacle* obs = tower + activeObstacleCount;
            obs->x = (targetXChar - 2) * SCALE;
            obs->y = (lantaiYChar - tingkat) * SCALE;
            obs->vx = 0.0f; obs->vy = 0.0f; obs->isBroken = false;
            obs->isTowerComponent = true;
            activeObstacleCount++;
        }
        if (tingkat != targetFloor) {
            if ((tingkat < targetFloor || rand() % 2 == 0) && activeObstacleCount < maxObstacles) {
                Obstacle* obs = tower + activeObstacleCount;
                obs->x = (targetXChar - 1) * SCALE; 
                obs->y = (lantaiYChar - tingkat) * SCALE;
                obs->vx = 0.0f; obs->vy = 0.0f; obs->isBroken = false;
                obs->isTowerComponent = true;
                activeObstacleCount++;
            }
        }
        if (rand() % 2 == 0 && activeObstacleCount < maxObstacles) {
            Obstacle* obs = tower + activeObstacleCount;
            obs->x = (targetXChar + 2) * SCALE;
            obs->y = (lantaiYChar - tingkat) * SCALE;
            obs->vx = 0.0f; obs->vy = 0.0f; obs->isBroken = false;
            obs->isTowerComponent = true;
            activeObstacleCount++;
        }
    }

    float targetX = targetXChar * SCALE;
    float targetY = targetYChar * SCALE;
    bool targetHit = false;
    bool targetFallen = (targetFloor > 0); 
    float targetVy = 0.0f;

    int shieldXChar = targetXChar - 7 - (rand() % 3); 
    int shieldHeight = 1 + (rand() % 3); 
    
    for (int i = 0; i < shieldHeight; i++) {
        if (activeObstacleCount < maxObstacles) {
            Obstacle* obs = tower + activeObstacleCount;
            obs->x = (shieldXChar - 1) * SCALE;
            obs->y = (lantaiYChar - i) * SCALE; 
            obs->vx = 0.0f; obs->vy = 0.0f; obs->isBroken = false;
            obs->isTowerComponent = false;
            activeObstacleCount++;
        }
    }

    const int MAX_PARTICLES = 50;
    Debris particles[MAX_PARTICLES];
    int activeParticleCount = 0;

    char screen[HEIGHT_CHARS][WIDTH_CHARS];
    const int MAX_ATTEMPTS = 3;
    bool gameWon = false;

    // Bersihkan layar penuh sekali di awal game
    cout << "\033[2J\033[?25l";

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
        Ball ball;
        ball.x = 2.0f * SCALE;
        ball.y = (float)(HEIGHT_CHARS - 2) * SCALE;
        ball.active = false;

        for (int i = 0; i < MAX_PARTICLES; i++) particles[i].active = false;
        activeParticleCount = 0;

        resetScreen(screen);
        drawToScreen(screen, ball.x, ball.y, 'o');
        
        int currentTargetXChar = (int)(targetX / SCALE);
        int currentTargetYChar = (int)(targetY / SCALE);
        if (currentTargetYChar >= 0 && currentTargetYChar < HEIGHT_CHARS && currentTargetXChar >= 0 && currentTargetXChar < WIDTH_CHARS) {
            screen[currentTargetYChar][currentTargetXChar] = (!targetHit) ? '@' : '*';
        }

        for (int i = 0; i < activeObstacleCount; i++) {
            Obstacle* obs = tower + i;
            if (!obs->isBroken) {
                drawToScreen(screen, obs->x, obs->y, '[');
                drawToScreen(screen, obs->x + SCALE, obs->y, ']');
            }
        }

        renderGameInterface(screen, attempt, MAX_ATTEMPTS);

        printWithPadding(string(60, ' ') + "\n");
        printWithPadding(string(60, ' ') + "\n");
        cout << "\033[2A"; 

        cout << "\033[?25h"; 
        float angleDeg = -1, speedMS = -1;
        
        // Pastikan buffer bersih dari sisa stream
        cin.clear();
        
        printWithPadding("Masukkan sudut tembakan (0 - 90 derajat): ");
        while (!(cin >> angleDeg) || angleDeg < 0 || angleDeg > 90) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\033[A\033[K"; 
            printWithPadding("Masukkan sudut tembakan (0 - 90 derajat): ");
        }

        printWithPadding("Masukkan kekuatan tembak m/s (Rekomendasi: 12 - 18): ");
        while (!(cin >> speedMS) || speedMS <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\033[A\033[K";
            printWithPadding("Masukkan kekuatan tembak m/s (Rekomendasi: 12 - 18): ");
        }

        cout << "\033[?25l"; 

        float angleRad = angleDeg * PI / 180.0f;
        ball.vx = speedMS * cos(angleRad);
        ball.vy = -speedMS * sin(angleRad); 
        ball.active = true;

        float lantaiMeter = (HEIGHT_CHARS - 1) * SCALE;
        bool shotRunning = true;

        while (shotRunning) {
            bool anyObjectMoving = false;

            if (ball.active) anyObjectMoving = true;
            
            for (int i = 0; i < activeObstacleCount; i++) {
                if (!(tower + i)->isBroken && ((tower + i)->vx != 0.0f || (tower + i)->vy != 0.0f)) {
                    anyObjectMoving = true;
                }
            }
            for (int i = 0; i < MAX_PARTICLES; i++) {
                if (particles[i].active) anyObjectMoving = true;
            }
            
            // PERBAIKAN UTAMA: Hilangkan penguncian state targetFallen yang bikin loop macet/langsung patah
            
            if (!anyObjectMoving) {
                shotRunning = false;
                break; // Keluar dari loop animasi dengan aman
            }

            if (ball.active) {
                ball.vy += g * dt; ball.x += ball.vx * dt; ball.y += ball.vy * dt;
                if (ball.x / SCALE >= WIDTH_CHARS || ball.x < 0 || ball.y >= lantaiMeter) ball.active = false;
                
                if (!targetHit && abs(ball.x - targetX) <= 1.0f * SCALE && abs(ball.y - targetY) <= 1.0f * SCALE) {
                    targetHit = true; targetFallen = true; ball.active = false;
                }
                
                for (int i = 0; i < activeObstacleCount; i++) {
                    Obstacle* obs = tower + i;
                    if (checkCollision(&ball, obs)) { 
                        obs->vx = ball.vx * 0.5f; obs->vy = ball.vy * 0.4f - 1.5f; 
                        ball.active = false; 
                        targetFallen = true; 
                        break;
                    }
                }
            }

            for (int i = 0; i < activeObstacleCount; i++) {
                Obstacle* obs = tower + i;
                if (!obs->isBroken && (obs->vx != 0.0f || obs->vy != 0.0f)) {
                    obs->vy += g * dt; obs->x += obs->vx * dt; obs->y += obs->vy * dt;
                    if (!targetHit && abs(obs->x - targetX) <= 1.2f * SCALE && abs(obs->y - targetY) <= 1.0f * SCALE) {
                        targetHit = true; targetFallen = true;
                    }
                    if (obs->y >= lantaiMeter || obs->x / SCALE >= WIDTH_CHARS - 2 || obs->x < 0) {
                        obs->isBroken = true;
                        for (int j = 0; j < 4; j++) {
                            if (activeParticleCount < MAX_PARTICLES) {
                                Debris* d = &particles[activeParticleCount];
                                d->symbol = (j % 2 == 0) ? 'x' : '+'; 
                                d->x = obs->x; d->y = lantaiMeter - 0.1f;
                                d->vx = obs->vx * 0.4f + ((rand() % 10) - 5) * 0.5f; d->vy = -((rand() % 10) + 3) * 0.3f;
                                d->active = true; activeParticleCount++;
                            }
                        }
                        obs->vx = 0.0f; obs->vy = 0.0f;
                    }
                }
            }

            if (targetFallen && !targetHit) {
                bool supportExists = false;
                for (int i = 0; i < activeObstacleCount; i++) {
                    Obstacle* obs = tower + i;
                    if (!obs->isBroken && obs->vx == 0.0f && obs->vy == 0.0f) {
                        if (abs(obs->x - targetX) < 0.5f && abs(obs->y - (targetY + SCALE)) <= 0.1f) { supportExists = true; break; }
                    }
                }
                if (!supportExists) {
                    targetVy += g * dt; targetY += targetVy * dt;
                    if (targetY >= lantaiMeter) { targetY = lantaiMeter; targetHit = true; }
                } else { targetVy = 0.0f; }
            }

            for (int i = 0; i < MAX_PARTICLES; i++) {
                Debris* p = &particles[i];
                if (p->active) {
                    p->vy += g * dt; p->x += p->vx * dt; p->y += p->vy * dt;
                    if (p->y >= lantaiMeter) {
                        p->y = lantaiMeter; p->vx *= 0.4f; p->vy = -p->vy * 0.2f;
                        if (abs(p->vy) < 0.2f && abs(p->vx) < 0.2f) { p->vx = 0.0f; p->vy = 0.0f; p->active = false; }
                    }
                }
            }

            resetScreen(screen);
            int targetXRender = (int)(targetX / SCALE);
            int targetYRender = (int)(targetY / SCALE);
            if (targetYRender >= 0 && targetYRender < HEIGHT_CHARS && targetXRender >= 0 && targetXRender < WIDTH_CHARS) {
                screen[targetYRender][targetXRender] = (!targetHit) ? '@' : '*';
            }

            for (int i = 0; i < activeObstacleCount; i++) {
                Obstacle* obs = tower + i;
                if (!obs->isBroken) {
                    drawToScreen(screen, obs->x, obs->y, '[');
                    drawToScreen(screen, obs->x + SCALE, obs->y, ']');
                }
            }

            if (ball.active) drawToScreen(screen, ball.x, ball.y, 'o');

            for (int i = 0; i < MAX_PARTICLES; i++) {
                if (particles[i].active) drawToScreen(screen, particles[i].x, particles[i].y, particles[i].symbol);
            }

            renderGameInterface(screen, attempt, MAX_ATTEMPTS);
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }

        if (targetHit) {
            gameWon = true;
            break; 
        } else {
            printWithPadding(RED + "Meleset! Target '@' aman. Bersiap untuk ronde berikutnya...\n" + RESET);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            
            // Hapus sisa teks info meleset agar interface tetap konsisten bersih
            cout << "\033[A\033[K";
            
            // Bersihkan stream state sebelum ronde baru meminta input kembali
            cin.clear();
        }
    }

    cout << "\033[2J\033[1;1H\n\n";
    if (gameWon) {
        printWithPadding(GREEN + "====================================\n" + RESET);
        printWithPadding(BOLD + GREEN + " SUCCESS: TARGET '@' HANCUR! LU MENANG!\n" + RESET);
        printWithPadding(GREEN + "====================================\n" + RESET);
    } else {
        printWithPadding(RED + "====================================\n" + RESET);
        printWithPadding(BOLD + RED + " GAME OVER: SISA NYAWA HABIS (0/3).\n" + RESET);
        printWithPadding(RED + "====================================\n" + RESET);
    }
    cout << "\n";

    delete[] tower; 
    cout << "\033[?25h"; 
    return 0;
}
