// game logic
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <string>
#include <thread>
#include <conio.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define ORANGE  "\033[38;5;208m"
#define YELLOW  "\033[33m"
#define WHITE   "\033[37m"
#define GRAY    "\033[90m"

using namespace std;

struct GameConfig {
  const float pi = 3.14159265f;
  const float scale = 0.25f;
  static const int width_chars = 60;
  static const int height_chars = 20;
  const int padding_left = 75;
  static const int max_particles = 50;
  static const int max_obstacles = 30;
  const int max_attempts = 3;

  const string reset = "\033[0m";
  const string bold = "\033[1m";
  const string red = "\033[31m";
  const string green = "\033[32m";
  const string yellow = "\033[33m";
  const string cyan = "\033[36m";
  const string gray = "\033[90m";
  const string magenta = "\033[35m";
};

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

struct GameContext {
  GameConfig config;

  Obstacle *tower;
  Debris particles[GameConfig::max_particles];
  char screen[GameConfig::height_chars][GameConfig::width_chars];

  int activeObstacleCount = 0;
  int activeParticleCount = 0;

  float targetX = 0.0f;
  float targetY = 0.0f;
  float targetVy = 0.0f;

  bool targetHit = false;
  bool targetFallen = false;
  bool gameWon = false;

  float g = 9.8f;
  float dt = 0.05f;

  float lantaiMeter = (float)(GameConfig::height_chars - 2) * config.scale;
};

bool checkCollision(Ball *b, Obstacle *obs, const GameConfig &cfg) {
  if (obs->isBroken)
    return false;
  return (abs(b->x - obs->x) <= 1.2f * cfg.scale &&
          abs(b->y - obs->y) <= 1.0f * cfg.scale);
}

void resetScreen(
    char screen[GameConfig::height_chars][GameConfig::width_chars]) {
  for (int y = 0; y < GameConfig::height_chars - 1; y++) {
    for (int x = 0; x < GameConfig::width_chars; x++)
      screen[y][x] = ' ';
  }
  for (int x = 0; x < GameConfig::width_chars; x++) {
    screen[GameConfig::height_chars - 1][x] = '=';
  }
}

void drawToScreen(
    char screen[GameConfig::height_chars][GameConfig::width_chars],
    float worldX, float worldY, char symbol, const GameContext &ctx) {
    int screenX = (int)round(worldX / ctx.config.scale);
  int screenY = (int)round(worldY / ctx.config.scale);

  if (screenY >= GameConfig::height_chars - 1) {
    screenY = GameConfig::height_chars - 2;
  }

  if (screenX >= 0 && screenX < GameConfig::width_chars && screenY >= 0 &&
      screenY < GameConfig::height_chars - 1) {
    screen[screenY][screenX] = symbol;
  }
}

void printWithPadding(string text, int padding) {
  cout << string(padding, ' ') << text;
}

void renderGameInterface(int attempt, const GameContext &ctx) {
  cout << "\033[1;1H\n\n";
  printWithPadding("+" + string(GameConfig::width_chars, '-') + "+\n",
                   ctx.config.padding_left);
  for (int y = 0; y < GameConfig::height_chars - 1; y++) {
    cout << string(ctx.config.padding_left, ' ') << "|";
    for (int x = 0; x < GameConfig::width_chars; x++) {
      char ch = ctx.screen[y][x];
      if (ch == 'o')
        cout << ctx.config.bold << ctx.config.red << ch << ctx.config.reset;
      else if (ch == '@')
        cout << ctx.config.bold << ctx.config.green << ch << ctx.config.reset;
      else if (ch == '*')
        cout << ctx.config.bold << ctx.config.magenta << ch << ctx.config.reset;
      else if (ch == '[' || ch == ']')
        cout << ctx.config.yellow << ch << ctx.config.reset;
      else if (ch == 'x' || ch == '+')
        cout << ctx.config.gray << ch << ctx.config.reset;
      else
        cout << ch;
    }
    cout << "|\n";
  }

  cout << string(ctx.config.padding_left, ' ') << "+" << ctx.config.cyan;
  for (int x = 0; x < GameConfig::width_chars; x++) {
    cout << "=";
  }
  cout << ctx.config.reset
       << "+ " + ctx.config.cyan + "(LANTAI)" + ctx.config.reset + "\n\n";

  printWithPadding(ctx.config.bold +
                       "=== ANGRY BIRDS CLI: PERFECT LAYOUT ===\n" +
                       ctx.config.reset,
                   ctx.config.padding_left);
  printWithPadding("KESEMPATAN KE: " + to_string(attempt) + " dari " +
                       to_string(ctx.config.max_attempts) + "\n",
                   ctx.config.padding_left);
}

void generateLevel(GameContext &ctx) {
  int lantaiYChar = GameConfig::height_chars - 2;
  ctx.activeObstacleCount = 0;

  int targetXChar = 42 + (rand() % 8);
  int towerHeight = 2 + (rand() % 3);
  int targetFloor = rand() % towerHeight;

  int targetYChar = lantaiYChar - targetFloor;

  ctx.targetX = targetXChar * ctx.config.scale;
  ctx.targetY = targetYChar * ctx.config.scale;

  for (int tingkat = 0; tingkat < towerHeight; tingkat++) {
    int currentYChar = lantaiYChar - tingkat;

    if (rand() % 2 == 0 &&
        ctx.activeObstacleCount < GameConfig::max_obstacles) {
      Obstacle *obs = ctx.tower + ctx.activeObstacleCount;
      obs->x = (targetXChar - 2) * ctx.config.scale;
      obs->y = currentYChar * ctx.config.scale;
      obs->vx = 0.0f;
      obs->vy = 0.0f;
      obs->isBroken = false;
      obs->isTowerComponent = true;
      ctx.activeObstacleCount++;
    }
    if (tingkat != targetFloor) {
      if ((tingkat < targetFloor || rand() % 2 == 0) &&
          ctx.activeObstacleCount < GameConfig::max_obstacles) {
        Obstacle *obs = ctx.tower + ctx.activeObstacleCount;
        obs->x = (targetXChar - 1) * ctx.config.scale;
        obs->y = currentYChar * ctx.config.scale;
        obs->vx = 0.0f;
        obs->vy = 0.0f;
        obs->isBroken = false;
        obs->isTowerComponent = true;
        ctx.activeObstacleCount++;
      }
    }
    if (rand() % 2 == 0 &&
        ctx.activeObstacleCount < GameConfig::max_obstacles) {
      Obstacle *obs = ctx.tower + ctx.activeObstacleCount;
      obs->x = (targetXChar + 2) * ctx.config.scale;
      obs->y = currentYChar * ctx.config.scale;
      obs->vx = 0.0f;
      obs->vy = 0.0f;
      obs->isBroken = false;
      obs->isTowerComponent = true;
      ctx.activeObstacleCount++;
    }
  }

  int shieldXChar = targetXChar - 7 - (rand() % 3);
  int shieldHeight = 1 + (rand() % 3);
  for (int i = 0; i < shieldHeight; i++) {
    if (ctx.activeObstacleCount < GameConfig::max_obstacles) {
      Obstacle *obs = ctx.tower + ctx.activeObstacleCount;
      obs->x = (shieldXChar - 1) * ctx.config.scale;
      obs->y = (lantaiYChar - i) * ctx.config.scale;
      obs->vx = 0.0f;
      obs->vy = 0.0f;
      obs->isBroken = false;
      obs->isTowerComponent = false;
      ctx.activeObstacleCount++;
    }
  }
}

void getPlayerInput(float &angleDeg, float &speedMS, const GameContext &ctx) {
  cout << "\033[?25h";
  cin.clear();

  printWithPadding("Masukkan sudut tembakan (0 - 90 derajat): ",
                   ctx.config.padding_left);
  while (!(cin >> angleDeg) || angleDeg < 0 || angleDeg > 90) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\033[A\033[K";
    printWithPadding("Masukkan sudut tembakan (0 - 90 derajat): ",
                     ctx.config.padding_left);
  }

  printWithPadding("Masukkan kekuatan tembak m/s (Rekomendasi: 12 - 18): ",
                   ctx.config.padding_left);
  while (!(cin >> speedMS) || speedMS <= 0) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\033[A\033[K";
    printWithPadding("Masukkan kekuatan tembak m/s (Rekomendasi: 12 - 18): ",
                     ctx.config.padding_left);
  }
  cout << "\033[?25l";
}

void updatePhysics(Ball &ball, GameContext &ctx) {
  if (ball.active) {
    ball.vy += ctx.g * ctx.dt;
    ball.x += ball.vx * ctx.dt;
    ball.y += ball.vy * ctx.dt;

    if (ball.x / ctx.config.scale >= GameConfig::width_chars || ball.x < 0 ||
        ball.y >= ctx.lantaiMeter + 0.1f) {
      ball.active = false;
    }

    if (!ctx.targetHit &&
        abs(ball.x - ctx.targetX) <= 1.0f * ctx.config.scale &&
        abs(ball.y - ctx.targetY) <= 1.0f * ctx.config.scale) {
      ctx.targetHit = true;
      ctx.targetFallen = true;
      ball.active = false;
    }

    for (int i = 0; i < ctx.activeObstacleCount; i++) {
      Obstacle *obs = ctx.tower + i;
      if (checkCollision(&ball, obs, ctx.config)) {
        obs->vx = ball.vx * 0.5f;
        obs->vy = ball.vy * 0.4f - 1.5f;
        ball.active = false;
        ctx.targetFallen = true;
        break;
      }
    }
  }

  for (int i = 0; i < ctx.activeObstacleCount; i++) {
    Obstacle *obs = ctx.tower + i;
    if (!obs->isBroken && (obs->vx != 0.0f || obs->vy != 0.0f)) {
      obs->vy += ctx.g * ctx.dt;
      obs->x += obs->vx * ctx.dt;
      obs->y += obs->vy * ctx.dt;
      if (!ctx.targetHit &&
          abs(obs->x - ctx.targetX) <= 1.2f * ctx.config.scale &&
          abs(obs->y - ctx.targetY) <= 1.0f * ctx.config.scale) {
        ctx.targetHit = true;
        ctx.targetFallen = true;
      }
      if (obs->y >= ctx.lantaiMeter ||
          obs->x / ctx.config.scale >= GameConfig::width_chars - 2 ||
          obs->x < 0) {
        obs->isBroken = true;
        obs->y = ctx.lantaiMeter;
        for (int j = 0; j < 4; j++) {
          if (ctx.activeParticleCount < GameConfig::max_particles) {
            Debris *d = &ctx.particles[ctx.activeParticleCount];
            d->symbol = (j % 2 == 0) ? 'x' : '+';
            d->x = obs->x;
            d->y = ctx.lantaiMeter;
            d->vx = obs->vx * 0.4f + ((rand() % 10) - 5) * 0.5f;
            d->vy = -((rand() % 10) + 3) * 0.3f;
            d->active = true;
            ctx.activeParticleCount++;
          }
        }
        obs->vx = 0.0f;
        obs->vy = 0.0f;
      }
    }
  }

  if (ctx.targetFallen && !ctx.targetHit) {
    bool supportExists = false;
    for (int i = 0; i < ctx.activeObstacleCount; i++) {
      Obstacle *obs = ctx.tower + i;
      if (!obs->isBroken && obs->vx == 0.0f && obs->vy == 0.0f) {
        if (abs(obs->x - ctx.targetX) < 0.5f &&
            abs(obs->y - (ctx.targetY + ctx.config.scale)) <= 0.1f) {
          supportExists = true;
          break;
        }
      }
    }
    if (!supportExists) {
      ctx.targetVy += ctx.g * ctx.dt;
      ctx.targetY += ctx.targetVy * ctx.dt;
      if (ctx.targetY >= ctx.lantaiMeter) {
        ctx.targetY = ctx.lantaiMeter;
        ctx.targetHit = true;
      }
    } else {
      ctx.targetVy = 0.0f;
    }
  }

  for (int i = 0; i < GameConfig::max_particles; i++) {
    Debris *p = &ctx.particles[i];
    if (p->active) {
      p->vy += ctx.g * ctx.dt;
      p->x += p->vx * ctx.dt;
      p->y += p->vy * ctx.dt;
      if (p->y >= ctx.lantaiMeter) {
        p->y = ctx.lantaiMeter;
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
}

void updateScreenBuffer(Ball &ball, GameContext &ctx) {
  resetScreen(ctx.screen);

  drawToScreen(ctx.screen, ctx.targetX, ctx.targetY,
               (!ctx.targetHit) ? '@' : '*', ctx);

  for (int i = 0; i < ctx.activeObstacleCount; i++) {
    Obstacle *obs = ctx.tower + i;
    if (!obs->isBroken) {
      drawToScreen(ctx.screen, obs->x, obs->y, '[', ctx);
      drawToScreen(ctx.screen, obs->x + ctx.config.scale, obs->y, ']', ctx);
    }
  }

  if (ball.active)
    drawToScreen(ctx.screen, ball.x, ball.y, 'o', ctx);
  for (int i = 0; i < GameConfig::max_particles; i++) {
    if (ctx.particles[i].active)
      drawToScreen(ctx.screen, ctx.particles[i].x, ctx.particles[i].y,
                   ctx.particles[i].symbol, ctx);
  }
}

bool runRoundSimulation(int attempt, GameContext &ctx) {
  Ball ball;

  ball.x = 2.0f * ctx.config.scale;
  ball.y = ctx.lantaiMeter - ctx.config.scale;
  ball.active = false;

  for (int i = 0; i < GameConfig::max_particles; i++)
    ctx.particles[i].active = false;
  ctx.activeParticleCount = 0;

  updateScreenBuffer(ball, ctx);
  renderGameInterface(attempt, ctx);

  printWithPadding(string(60, ' ') + "\n", ctx.config.padding_left);
  printWithPadding(string(60, ' ') + "\n", ctx.config.padding_left);
  cout << "\033[2A";

  float angleDeg = 0, speedMS = 0;
  getPlayerInput(angleDeg, speedMS, ctx);

  float angleRad = angleDeg * ctx.config.pi / 180.0f;
  ball.vx = speedMS * cos(angleRad);
  ball.vy = -speedMS * sin(angleRad);
  ball.active = true;

  bool shotRunning = true;
  while (shotRunning) {
    bool anyObjectMoving = false;
    if (ball.active)
      anyObjectMoving = true;
    for (int i = 0; i < ctx.activeObstacleCount; i++) {
      if (!(ctx.tower + i)->isBroken &&
          ((ctx.tower + i)->vx != 0.0f || (ctx.tower + i)->vy != 0.0f))
        anyObjectMoving = true;
    }
    for (int i = 0; i < GameConfig::max_particles; i++) {
      if (ctx.particles[i].active)
        anyObjectMoving = true;
    }

    if (!anyObjectMoving) {
      shotRunning = false;
      break;
    }

    updatePhysics(ball, ctx);
    updateScreenBuffer(ball, ctx);
    renderGameInterface(attempt, ctx);

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  }

  if (ctx.targetHit) {
    return true;
  } else {
    printWithPadding(
        ctx.config.red +
            "Meleset! Target '@' aman. Bersiap untuk ronde berikutnya...\n" +
            ctx.config.reset,
        ctx.config.padding_left);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    cout << "\033[A\033[K";
    cin.clear();
    return false;
  }
}

void displayGameResult(const GameContext &ctx) {
  cout << "\033[2J\033[1;1H\n\n";
  if (ctx.gameWon) {
    printWithPadding(ctx.config.green +
                         "====================================\n" +
                         ctx.config.reset,
                     ctx.config.padding_left);
    printWithPadding(ctx.config.bold + ctx.config.green +
                         " SUCCESS: TARGET '@' HANCUR KAMU MENANG!\n" +
                         ctx.config.reset,
                     ctx.config.padding_left);
    printWithPadding(ctx.config.green +
                         "====================================\n" +
                         ctx.config.reset,
                     ctx.config.padding_left);
  } else {
    printWithPadding(ctx.config.red + "====================================\n" +
                         ctx.config.reset,
                     ctx.config.padding_left);
    printWithPadding(ctx.config.bold + ctx.config.red +
                         " GAME OVER: SISA KESEMPATAN HABIS (0/3).\n" +
                         ctx.config.reset,
                     ctx.config.padding_left);
    printWithPadding(ctx.config.red + "====================================\n" +
                         ctx.config.reset,
                     ctx.config.padding_left);
  }
  cout << "\n";
}



//Petunjuk
// WARNA ANSI 
#define RESET    "\033[0m"
#define RED      "\033[31m"
#define GREEN    "\033[32m"
#define YELLOW   "\033[33m"
#define BLUE     "\033[34m"
#define CYAN     "\033[36m"
#define BOLD     "\033[1m"
#define GOLD     "\033[38;5;220m"
#define PURPLE   "\033[38;5;129m"
#define MARGIN  30

void printCentered(string text) {
    cout << string(MARGIN, ' ') << text << endl;
}

// Garis pembatas yang sudah terpusat
void printDivider() {
    cout << string(MARGIN, ' ') << "================================================================" << endl;
}

void howToPlay() {
    system("cls"); 

    // BAGIAN 1: OBJECTIVE
    cout << BOLD << GOLD;
    cout << string(MARGIN, ' ') << "================================================================" << endl;
    cout << string(MARGIN, ' ') << "|                     H O W   T O   P L A Y                    |" << endl;
    cout << string(MARGIN, ' ') << "================================================================" << endl << endl;

    cout << BOLD << BLUE;
    printCentered("OBJECTIVE");
    cout << RESET;
    printDivider();
    printCentered("Hancurkan seluruh target menggunakan bola yang tersedia.");
    printCentered("Selesaikan setiap level dengan jumlah tembakan sesedikit");
    
    printCentered("Tekan Enter untuk melanjutkan........");
    cin.get();
    system("cls");

    // BAGIAN 2: LANGKAH BERMAIN
    cout << BOLD << GREEN;
    printCentered("LANGKAH BERMAIN");
    cout << RESET;
    printDivider();

    cout << BOLD << YELLOW << string(MARGIN, ' ') << "1. Mulai Permainan" << RESET << endl;
    printCentered("Pilih menu START pada Main Menu untuk memulai permainan.\n");

    cout << BOLD << YELLOW << string(MARGIN, ' ') << "2. Atur Sudut Peluncuran" << RESET << endl;
    printCentered("Masukkan sudut (derajat) untuk peluncuran bola.");
    printCentered("Sudut yang berbeda akan menghasilkan lintasan yang berbeda.\n");

    cout << BOLD << YELLOW << string(MARGIN, ' ') << "3. Atur Kekuatan Tembakan" << RESET << endl;
    printCentered("Masukkan nilai kecepatan tembakan (m/s) untuk menentukan");
    printCentered("seberapa jauh bola akan diluncurkan menuju target.\n");

    cout << BOLD << YELLOW << string(MARGIN, ' ') << "4. Luncurkan Bola" << RESET << endl;
    printCentered("Setelah sudut dan kekuatan ditentukan, bola akan");
    printCentered("bergerak mengikuti gravitasi.\n");
    
    printCentered("Tekan Enter untuk melanjutkan........");
    cin.get();
    system("cls");

    // BAGIAN 3: TARGET, BOLA, LEVEL
    cout << BOLD << RED;
    printCentered("HANCURKAN TARGET");
    cout << RESET;
    printDivider();
    printCentered("Bidik seluruh target dengan tepat untuk menambah skor.\n");

    cout << BOLD << CYAN;
    printCentered("BOLA TERBATAS");
    cout << RESET;
    printDivider();
    printCentered("Jumlah kesempatan terbatas. Gunakan seefektif mungkin.\n");

    cout << BOLD << PURPLE;
    printCentered("DYNAMIC LEVEL");
    cout << RESET;
    printDivider();
    printCentered("Setiap level memiliki susunan target yang diacak.");
    printCentered("Sesuaikan strategi, sudut, dan kekuatan setiap saat.\n");
    
    printCentered("Tekan Enter untuk melanjutkan........");
    cin.get();
    system("cls");

    // BAGIAN 4: MENANG/KALAH
    cout << BOLD << GREEN;
    printCentered("MENANG");
    cout << RESET;
    printDivider();
    printCentered("Hancurkan seluruh target untuk menyelesaikan level.\n");

    cout << BOLD << RED;
    printCentered("KALAH");
    cout << RESET;
    printDivider();
    printCentered("Permainan berakhir jika bola habis sebelum target hancur.\n");

    printCentered("Tekan ENTER untuk kembali ke Main Menu......");
    cin.get();
    cin.ignore(1000, '\n');
}



// backgroundstory
void hapus (){
    cout << "\033[2J\033[H";
}

void delay (int MS) {
    this_thread::sleep_for(chrono::milliseconds(MS));
}

void ketik (string teks, int kecMS = 10){
    for (char c : teks){
        cout << c;
        cout.flush();
        delay(kecMS);
    }
}

void BGStory (){
    hapus ();

    ketik ("Tahun 2389. Kehancuran melanda bumi...");
    delay (1000);

    ketik ("\n\nTeknologi lumpuh.");
    delay(500);
    ketik ("\nHarapan hampir sirna.");
    delay (1500);

    ketik ("\n\nKamu adalah Prof.George,");
    delay (400);
    ketik ("\nSatu-satunya yang masih berani melawan.");
    delay (1500);

    ketik ("\n\nHanya tersisa satu hal...");
    delay (1500);

    ketik ("\n\nSenjata yang tak pernah kehabisan amunisi");
    delay (1500);

    ketik ("\n\nIni adalah pertempuran terakhirmu..");
    delay (1000);

    ketik ("\n\n\"Fisika adalah senjata terkuat.\"");
    delay (500);
    ketik ("\n\t-Prof.George\n\n");

    cout << "[Tekan ENTER untuk kembali ke menu...]";
    cin.get();

}

// menu
void tampilMenu(int pilihan);
void startGame();
void petunjuk();
void tentang();

void hideKursor() { cout << "\033[?25l" << flush; }
void showKursor() { cout << "\033[?25h" << flush; }
void goHome()      { cout << "\033[H\033[J"; }
void clearScreen()  { cout << "\033[2J\033[H"; }

void tampilMenu(int pilihan)
{
    hideKursor();
    goHome();

    string menu[4] = {
        "START",
        "HOW TO PLAY",
        "BACKGROUND STORY",
        "EXIT"
    };

    cout << "\to====================================================================================================o\n" << endl;
    cout << RED;
    cout << "\t           _    ____  _______  __    ____  ____   ___      _ _____ ____ _____ ___ _     _____\n";
    cout << "\t          / \\  |  _ \\| ____\\ \\/ /   |  _ \\|  _ \\ / _ \\    | | ____/ ___|_   _|_ _| |   | ____|\n";
    cout << "\t         / _ \\ | |_) |  _|  \\  /    | |_) | |_) | | | |   | |  _|| |     | |  | || |   |  _|\n";
    cout << "\t        / ___ \\|  __/| |___ /  \\    |  __/|  _ <| |_| |___| | |__| |___  | |  | || |___| |__\n";
    cout << "\t       /_/   \\_\\_|   |_____/_/\\_\\   |_|   |_| \\_\\\\___/|_____|_____\\\\____| |_| |___|_____|____|\n" << endl;
    cout << RESET;
    cout << "\to====================================================================================================o\n";
    cout << "\n\n";

    for (int i = 0; i < 4; i++)
    {
        if (i == pilihan)
            cout << ORANGE;
        else
            cout << WHITE;

        cout << "\t\t\t\t\t\t+----------------------+\n";

        string isi = (i == pilihan) ? "> " + menu[i] : "  " + menu[i];

        cout << "\t\t\t\t\t\t| " << isi;

        int sisa = 20 - (int)isi.length();
        for (int j = 0; j < sisa; j++)
            cout << " ";

        cout << " |\n";
        cout << "\t\t\t\t\t\t+----------------------+\n\n";
    }

    cout << GRAY;
    cout << "\n";
    cout << "\t\t\t\t\tGunakan PANAH ATAS/BAWAH untuk memilih\n";
    cout << "\t\t\t\t\t     Tekan ENTER untuk melanjutkan\n";
    cout << RESET;

    showKursor();
}

void startGame()
{
    clearScreen();

    cout << "================ START GAME ================\n\n";
    cout << "Game dimulai...\n\n";

    srand(time(0));

    GameContext ctx;
    ctx.tower = new Obstacle[GameConfig::max_obstacles];

    generateLevel(ctx);
    ctx.targetFallen = (ctx.targetY < ctx.lantaiMeter);

    cout << "\033[2J\033[?25l";

    for (int attempt = 1; attempt <= ctx.config.max_attempts; attempt++) {
        ctx.gameWon = runRoundSimulation(attempt, ctx);
        if (ctx.gameWon)
        break;
    }

    displayGameResult(ctx);

    delete[] ctx.tower;
    cout << "\033[?25h";

    getch();
}

void petunjuk()
{
    clearScreen();
    cout << "================ HOW TO PLAY ================\n\n";
    howToPlay();
    getch();
}

void tentang()
{
    clearScreen();

    cout << "============= BACKGROUND STORY =============\n\n";
    BGStory();
    getch();
}



int main() {
     int pilihan = 0;
    char tombol;

    cout << "\033[?1049h"; // hilangkan scrollback
    clearScreen();

    while (true)
    {
        tampilMenu(pilihan);

        tombol = getch();

        if (tombol == -32 || tombol == 0)
        {
            tombol = getch();

            if (tombol == 72) // Panah Atas
            {
                pilihan--;
                if (pilihan < 0)
                    pilihan = 3;
            }
            else if (tombol == 80) // Panah Bawah
            {
                pilihan++;
                if (pilihan > 3)
                    pilihan = 0;
            }
        }
        else if (tombol == 13) // Enter
        {
            switch (pilihan)
            {
                case 0:
                    startGame();
                    break;

                case 1:
                    petunjuk();
                    break;

                case 2:
                    tentang();
                    break;

                case 3:
                    clearScreen();
                    cout << YELLOW;
                    cout << "\n\n";
                    cout << "             Terima kasih telah memainkan APEX PROJECTILE!\n\n";
                    cout << RESET;

                    cout << "\033[?1049l"; // keluar alternate screen buffer
                    return 0;
            }
        }
    }

    cout << "\033[?1049l";
}