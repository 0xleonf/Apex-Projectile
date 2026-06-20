#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

struct Debris {
  float x, y;
  float vx, vy; // pergeseran objek dari tabrakan
  char symbol;
  // bool active = true;
};

struct Obstacle {
  float x, y;
  float vx, vy;
  bool isBroken = false;
};

int main() {
  srand(time(0));

  const int WIDTH = 50;
  const int HEIGHT = 15;
  float g = 9.8f;
  float dt = 0.1f;

  // Inisialisasi Obstacle di tengah layar, awalnya diam (vx=0, vy=0)
  Obstacle obs = {25.0f, 10.0f, 0.0f, 0.0f, false};
  vector<Debris> particles;

  int frameCount = 0;
  cout << "\033[?25l"; // Sembunyikan kursor

  while (true) {
    frameCount++;

    // TRIGGER: Di frame 15, diasumsikan senjatamu/burung MENABRAK obstacle
    if (frameCount == 15 && !obs.isBroken) {
      // Beri hantaman/impulse ke kanan (vx positif) dan sedikit ke atas (vy
      // negatif)
      obs.vx = 8.0f;  // Terlempar ke kanan
      obs.vy = -4.0f; // Terlempar sedikit ke atas
    }

    // ==========================================
    // UPDATE PHYSICS
    // ==========================================

    // 1. Jika obstacle MASIH UTUH tapi SUDAH BERGERAK
    if (!obs.isBroken && (obs.vx != 0 || obs.vy != 0)) {
      obs.vy += g * dt; // Pengaruh gravitasi saat terbang
      obs.x += obs.vx * dt;
      obs.y += obs.vy * dt;

      // Cek Tabrakan Fisika: Jika menabrak lantai saat terbang cepat -> PECAH!
      if (obs.y >= HEIGHT - 1 || obs.x >= WIDTH - 2 || obs.x <= 0) {
        obs.isBroken = true;

        // Tentukan posisi ledakan pecahan di tempat benturan terakhir
        float crashX = min(max(obs.x, 0.0f), (float)WIDTH - 2);
        float crashY = min(obs.y, (float)HEIGHT - 1);

        // Lahirkan debris mencar dari titik tabrakan
        for (int i = 0; i < 6; i++) {
          Debris d;
          d.symbol = (i % 2 == 0) ? '[' : ']';
          d.x = crashX;
          d.y = crashY;
          // Kecepatan serpihan setelah benturan (mencar acak)
          d.vx = obs.vx * 0.5f + ((rand() % 10) - 5) * 0.5f;
          d.vy = -((rand() % 10) + 2) * 0.4f; // Memantul ke atas
          particles.push_back(d);
        }
      }
    }

    // 2. Jika obstacle SUDAH PECAH (Update partikel-partikel debris)
    if (obs.isBroken) {
      for (auto &p : particles) {
        p.vy += g * dt;
        p.x += p.vx * dt;
        p.y += p.vy * dt;

        // Debris menyentuh lantai
        if (p.y >= HEIGHT - 1) {
          p.y = HEIGHT - 1;
          p.vx *= 0.4f;        // Gesekan lantai
          p.vy = -p.vy * 0.2f; // Pantulan kecil

          if (abs(p.vy) < 0.2f && abs(p.vx) < 0.2f) {
            p.vx = 0;
            p.vy = 0;
          }
        }
        // Batas dinding untuk debris
        if (p.x < 0) {
          p.x = 0;
          p.vx *= -0.4f;
        }
        if (p.x >= WIDTH) {
          p.x = WIDTH - 1;
          p.vx *= -0.4f;
        }
      }
    }

    // ==========================================
    // RENDER (ANSI Escape)
    // ==========================================
    cout << "\033[H\033[J"; // Clear screen

    vector<string> screen(HEIGHT, string(WIDTH, ' '));

    // Gambar berdasarkan status
    if (!obs.isBroken) {
      int ox = (int)obs.x;
      int oy = (int)obs.y;
      if (oy >= 0 && oy < HEIGHT && ox >= 0 && ox < WIDTH - 1) {
        screen[oy][ox] = '[';
        screen[oy][ox + 1] = ']';
      }
    } else {
      for (const auto &p : particles) {
        int px = (int)p.x;
        int py = (int)p.y;
        if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
          screen[py][px] = p.symbol;
        }
      }
    }

    // Cetak Buffer ke Layar
    for (int y = 0; y < HEIGHT; y++) {
      cout << screen[y] << "\n";
    }
    cout << string(WIDTH, '=') << " (LANTAI)\n";

    if (frameCount < 15) {
      cout << "Status: Obstacle diam menunggu target ditembak...\n";
    } else if (!obs.isBroken) {
      cout << "Status: KENA! Obstacle terpental berpindah tempat!\n";
    } else {
      cout << "Status: BENTURAN! Obstacle hancur menjadi serpihan debris!\n";
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  cout << "\033[?25h";
  return 0;
}
