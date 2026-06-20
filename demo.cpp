#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>

using namespace std;

const float PI = 3.14159265f;
const float SCALE = 0.25f; // 1 Karakter di terminal = 0.25 meter (25 cm)

const int WIDTH_CHARS = 60;
const int HEIGHT_CHARS = 20;

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
  bool isTowerComponent = false; // Penanda apakah balok penyangga @
};

struct Ball {
  float x, y;
  float vx, vy;
  bool active = false;
};

bool checkCollision(Ball *b, Obstacle *obs) {
  if (obs->isBroken)
    return false;

  if (abs(b->x - obs->x) <= 1.2f * SCALE &&
      abs(b->y - obs->y) <= 1.0f * SCALE) {
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

void drawToScreen(char screen[HEIGHT_CHARS][WIDTH_CHARS], float worldX,
                  float worldY, char symbol) {
  int screenX = (int)(worldX / SCALE);
  int screenY = (int)(worldY / SCALE);
  if (screenX >= 0 && screenX < WIDTH_CHARS && screenY >= 0 &&
      screenY < HEIGHT_CHARS) {
    screen[screenY][screenX] = symbol;
  }
}

int main() {
  srand(time(0));

  float g = 9.8f; // gravity
  float dt = 0.05f;

  int lantaiYChar = HEIGHT_CHARS - 2;

  int maxObstacles = 15;
  Obstacle *tower = new Obstacle[maxObstacles];
  int activeObstacleCount = 0;

  int targetXChar = 42 + (rand() % 10);
  int targetYChar;

  bool spawnTower = (rand() % 2 == 0);

  if (spawnTower) {
    int numObstacles = 2 + (rand() % 2);
    for (int i = 0; i < numObstacles; i++) {
      Obstacle *obs = tower + activeObstacleCount;
      obs->x = (targetXChar - 1) * SCALE;
      obs->y = (lantaiYChar - i) * SCALE;
      obs->vx = 0.0f;
      obs->vy = 0.0f;
      obs->isBroken = false;
      obs->isTowerComponent = true;
      activeObstacleCount++;
    }
    targetYChar = lantaiYChar - numObstacles;
  } else {
    targetYChar = lantaiYChar;
  }

  float targetX = targetXChar * SCALE;
  float targetY = targetYChar * SCALE;
  bool targetHit = false;
  bool targetFallen = spawnTower;
  float targetVy = 0.0f;

  // Tambah Tameng Penghalang acak di depan menara/target (Menyentuh tanah juga)
  int shieldXChar = targetXChar - 6 - (rand() % 3);
  int shieldHeight = 2 + (rand() % 2);

  for (int i = 0; i < shieldHeight; i++) {
    if (activeObstacleCount < maxObstacles) {
      Obstacle *obs = tower + activeObstacleCount;
      obs->x = (shieldXChar - 1) * SCALE;
      obs->y = (lantaiYChar - i) * SCALE; // Berdiri kokoh di atas lantai
      obs->vx = 0.0f;
      obs->vy = 0.0f;
      obs->isBroken = false;
      obs->isTowerComponent = false;
      activeObstacleCount++;
    }
  }

  // Array Statis C-Style untuk partikel ledakan (Debris)
  const int MAX_PARTICLES = 50;
  Debris particles[MAX_PARTICLES];
  int activeParticleCount = 0;

  // Array 2D Statis untuk buffer visualisasi layar
  char screen[HEIGHT_CHARS][WIDTH_CHARS];

  const int MAX_ATTEMPTS = 3;
  bool gameWon = false;

  for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {

    Ball ball;
    ball.x = 2.0f * SCALE;
    ball.y = (float)(HEIGHT_CHARS - 2) * SCALE;
    ball.active = false;

    for (int i = 0; i < MAX_PARTICLES; i++)
      particles[i].active = false;
    activeParticleCount = 0;

    cout << "\033[H\033[J\033[?25l";
    resetScreen(screen);

    drawToScreen(screen, ball.x, ball.y, 'o');

    int currentTargetXChar = (int)(targetX / SCALE);
    int currentTargetYChar = (int)(targetY / SCALE);
    if (currentTargetYChar >= 0 && currentTargetYChar < HEIGHT_CHARS &&
        currentTargetXChar >= 0 && currentTargetXChar < WIDTH_CHARS) {
      screen[currentTargetYChar][currentTargetXChar] = (!targetHit) ? '@' : '*';
    }

    for (int i = 0; i < activeObstacleCount; i++) {
      Obstacle *obs = tower + i;
      if (!obs->isBroken) {
        drawToScreen(screen, obs->x, obs->y, '[');
        drawToScreen(screen, obs->x + SCALE, obs->y, ']');
      }
    }

    for (int y = 0; y < HEIGHT_CHARS; y++) {
      for (int x = 0; x < WIDTH_CHARS; x++)
        cout << screen[y][x];
      cout << "\n";
    }
    cout << string(WIDTH_CHARS, '=') << " (LANTAI)\n";

    cout << "\033[?25h";
    float angleDeg, speedMS;
    cout << "\n=== APEX PROJECTILE ===\n";
    cout << "KESEMPATAN KE: " << attempt << " dari " << MAX_ATTEMPTS << "\n";
    // if (!targetFallen) cout << "Kondisi: Target '@' sembunyi di TANAH!\n";
    // else cout << "Kondisi: Target '@' berada di atas TOWER!\n";

    cout << "Masukkan sudut tembakan (0 - 90 derajat): ";
    cin >> angleDeg;
    cout << "Masukkan kekuatan tembak m/s (Rekomendasi: 12 - 18 m/s): ";
    cin >> speedMS;

    float angleRad = angleDeg * PI / 180.0f;
    ball.vx = speedMS * cos(angleRad);
    ball.vy = -speedMS * sin(angleRad);
    ball.active = true;

    cout << "\033[?25l";

    float lantaiMeter = (HEIGHT_CHARS - 1) * SCALE;
    bool shotRunning = true;

    while (shotRunning) {

      bool anyObjectMoving = false;

      if (ball.active) {
        anyObjectMoving = true;
      }
      for (int i = 0; i < activeObstacleCount; i++) {
        if (!(tower + i)->isBroken &&
            ((tower + i)->vx != 0.0f || (tower + i)->vy != 0.0f)) {
          anyObjectMoving = true;
        }
      }
      for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) {
          anyObjectMoving = true;
        }
      }
      if (targetFallen && !targetHit) {
        anyObjectMoving = true;
      }

      if (!anyObjectMoving) {
        shotRunning = false;
      }

      if (ball.active) {
        ball.vy += g * dt;
        ball.x += ball.vx * dt;
        ball.y += ball.vy * dt;

        if (ball.x / SCALE >= WIDTH_CHARS || ball.x < 0 ||
            ball.y >= lantaiMeter) {
          ball.active = false;
        }

        if (!targetHit && abs(ball.x - targetX) <= 1.0f * SCALE &&
            abs(ball.y - targetY) <= 1.0f * SCALE) {
          targetHit = true;
          targetFallen = true;
          ball.active = false;
        }

        for (int i = 0; i < activeObstacleCount; i++) {
          Obstacle *obs = tower + i;
          if (checkCollision(&ball, obs)) {
            obs->vx = ball.vx * 0.5f;
            obs->vy = ball.vy * 0.4f - 1.5f;
            ball.active = false;

            if (spawnTower && obs->isTowerComponent) {
              targetFallen = true;
            }
            break;
          }
        }
      }

      // Obstacle: debris system
      for (int i = 0; i < activeObstacleCount; i++) {
        Obstacle *obs = tower + i;
        if (!obs->isBroken && (obs->vx != 0.0f || obs->vy != 0.0f)) {
          obs->vy += g * dt;
          obs->x += obs->vx * dt;
          obs->y += obs->vy * dt;

          if (!targetHit && abs(obs->x - targetX) <= 1.2f * SCALE &&
              abs(obs->y - targetY) <= 1.0f * SCALE) {
            targetHit = true;
            targetFallen = true;
          }

          if (obs->y >= lantaiMeter || obs->x / SCALE >= WIDTH_CHARS - 2 ||
              obs->x < 0) {
            obs->isBroken = true;

            for (int j = 0; j < 4; j++) {
              if (activeParticleCount < MAX_PARTICLES) {
                Debris *d = &particles[activeParticleCount];
                d->symbol = (j % 2 == 0) ? '[' : ']';
                d->x = obs->x;
                d->y = lantaiMeter - 0.1f;
                d->vx = obs->vx * 0.4f + ((rand() % 10) - 5) * 0.5f;
                d->vy = -((rand() % 10) + 3) * 0.3f;
                d->active = true;
                activeParticleCount++;
              }
            }
            obs->vx = 0.0f;
            obs->vy = 0.0f;
          }
        }
      }

      // target fallen down if tower broke
      if (targetFallen && !targetHit) {
        bool supportExists = false;
        for (int i = 0; i < activeObstacleCount; i++) {
          Obstacle *obs = tower + i;
          if (obs->isTowerComponent && !obs->isBroken && obs->vx == 0.0f &&
              obs->vy == 0.0f) {
            if (abs(obs->x - targetX) < 0.5f && obs->y > targetY) {
              supportExists = true;
            }
          }
        }

        if (!supportExists) {
          targetVy += g * dt;
          targetY += targetVy * dt;
          if (targetY >= lantaiMeter) {
            targetY = lantaiMeter;
            targetHit = true;
          }
        }
      }

      // partikel debris
      for (int i = 0; i < MAX_PARTICLES; i++) {
        Debris *p = &particles[i];
        if (p->active) {
          p->vy += g * dt;
          p->x += p->vx * dt;
          p->y += p->vy * dt;
          if (p->y >= lantaiMeter) {
            p->y = lantaiMeter;
            p->vx *= 0.4f;
            p->vy = -p->vy * 0.2f;
            if (abs(p->vy) < 0.2f && abs(p->vx) < 0.2f) {
              p->vx = 0.0f;
              p->vy = 0.0f;
              p->active = false;
            }
          }
        }
      }

      cout << "\033[H\033[J";
      resetScreen(screen);

      int targetXRender = (int)(targetX / SCALE);
      int targetYRender = (int)(targetY / SCALE);
      if (targetYRender >= 0 && targetYRender < HEIGHT_CHARS &&
          targetXRender >= 0 && targetXRender < WIDTH_CHARS) {
        screen[targetYRender][targetXRender] = (!targetHit) ? '@' : '*';
      }

      for (int i = 0; i < activeObstacleCount; i++) {
        Obstacle *obs = tower + i;
        if (!obs->isBroken) {
          drawToScreen(screen, obs->x, obs->y, '[');
          drawToScreen(screen, obs->x + SCALE, obs->y, ']');
        }
      }

      if (ball.active)
        drawToScreen(screen, ball.x, ball.y, 'o');

      for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].active) {
          drawToScreen(screen, particles[i].x, particles[i].y,
                       particles[i].symbol);
        }
      }

      for (int y = 0; y < HEIGHT_CHARS; y++) {
        for (int x = 0; x < WIDTH_CHARS; x++)
          cout << screen[y][x];
        cout << "\n";
      }
      cout << string(WIDTH_CHARS, '=') << " (LANTAI)\n";

      std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    if (targetHit) {
      gameWon = true;
      break;
    } else {
      cout << "\n Obstacle hancur tapi Target '@' selamat! Bersiap tembakan "
              "berikutnya...\n";
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  }

  cout << "\033[H\033[J\n====================================\n";
  if (gameWon)
    cout << " SUCCESS: TARGET '@' HANCUR! KAMU MENANG!\n";
  else
    cout << " GAME OVER: SISA KESEMPATAN HABIS (0/3).\n";
  cout << "====================================\n\n";

  delete[] tower;

  cout << "\033[?25h";
  return 0;
}
