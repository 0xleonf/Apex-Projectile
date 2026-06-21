# Apex Projectile - Terminal Based Physics Arcade Game

Apex Projectile merupakan permainan arcade berbasis terminal (CLI / Command Line Interface) yang mengimplementasikan simulasi gerak parabola (Projectile Motion) dalam lingkungan permainan interaktif. Pemain mengendalikan sebuah ketapel untuk meluncurkan proyektil dengan mengatur sudut dan kekuatan tembakan guna menghancurkan target serta melewati berbagai rintangan yang dihasilkan secara acak pada setiap permainan.

Proyek ini dikembangkan menggunakan bahasa C++ dengan pendekatan modular sehingga setiap komponen utama permainan dipisahkan ke dalam berkas terpisah untuk memudahkan pengembangan kolaboratif menggunakan GitHub.

## Fitur Utama Permainan

1. **Sistem Gerak Parabola (Projectile Physics)**

   * Simulasi lintasan proyektil menggunakan persamaan gerak parabola.
   * Pemain dapat menentukan sudut peluncuran (0° - 90°).
   * Pemain dapat menentukan kekuatan tembakan secara manual.
   * Pergerakan proyektil divisualisasikan secara real-time pada terminal.

2. **Sistem Target dan Objektif Permainan**

   * Target ditempatkan pada posisi acak setiap permainan.
   * Pemain memiliki jumlah kesempatan tembak yang terbatas.
   * Permainan dinyatakan menang apabila target berhasil dihancurkan.
   * Sistem Game Over ketika seluruh kesempatan telah habis.

3. **Obstacle & Shield Generation**

   * Rintangan (Obstacle) dihasilkan secara acak pada setiap permainan.
   * Menara target dapat terbentuk secara prosedural dengan jumlah blok berbeda.
   * Shield atau penghalang tambahan dihasilkan secara acak untuk meningkatkan tingkat kesulitan.
   * Sistem validasi posisi untuk mencegah konflik koordinat antar objek.

4. **Physics Interaction System**

   * Tumbukan proyektil dengan obstacle menghasilkan efek fisika sederhana.
   * Obstacle dapat terdorong dan jatuh akibat benturan.
   * Target dapat ikut jatuh apabila struktur penyangga runtuh.
   * Simulasi gravitasi diterapkan pada seluruh objek bergerak.

5. **Debris & Destruction Effect**

   * Obstacle yang hancur menghasilkan partikel debris.
   * Debris memiliki kecepatan dan arah gerak acak.
   * Efek pantulan sederhana diterapkan ketika debris menyentuh tanah.
   * Debris akan menghilang secara otomatis ketika energi geraknya habis.

6. **Rendering Terminal Real-Time**

   * Seluruh objek permainan divisualisasikan menggunakan karakter ASCII.
   * Sistem buffer layar digunakan untuk mengurangi flickering.
   * Frame diperbarui secara berkala menggunakan mekanisme refresh terminal.
   * Visualisasi target, proyektil, obstacle, debris, dan area permainan dilakukan secara real-time.

## Struktur Arsitektur Berkas

Proyek menggunakan pendekatan modular agar setiap anggota kelompok dapat mengembangkan fitur secara paralel melalui GitHub.

* `main.cpp` : Entry point program, menu utama, navigasi permainan, dan pengatur alur utama aplikasi.
* `gameplay.cpp` : Sistem gameplay utama, simulasi tembakan, fisika proyektil, target, dan logika kemenangan.
* `obstacle.cpp` : Sistem pembuatan obstacle, shield, menara target, serta manajemen koordinat objek.
* `vector.cpp` : Struktur data Vector2D yang digunakan untuk menyimpan posisi objek dalam permainan.
* `render.cpp` : Sistem rendering terminal, buffer layar, visualisasi objek ASCII, dan pengelolaan frame.
* `collision.cpp` : Sistem deteksi tabrakan antara proyektil, obstacle, target, dan debris.
* `particle.cpp` : Sistem partikel debris dan efek kehancuran objek.
* `menu.cpp` : Antarmuka menu utama, petunjuk permainan, dan informasi pengembang.

## Konsep Pemrograman yang Digunakan

Beberapa konsep yang diterapkan dalam proyek ini antara lain:

* Array dan Array Multidimensi
* Struct dan Pointer
* Dynamic Memory Allocation (`new` dan `delete`)
* Modular Programming
* Random Number Generation
* Physics Simulation
* Collision Detection
* Real-Time Rendering
* Looping dan Conditional Statement
* GitHub Collaborative Development

## Prasyarat Sistem

Untuk mengompilasi dan menjalankan program, pastikan perangkat telah memiliki:

* GCC Compiler (C++11 atau lebih baru)
* Visual Studio Code (Direkomendasikan)
* Git dan GitHub
* Terminal yang mendukung ANSI Escape Sequence
* Sistem Operasi Windows, Linux, atau macOS

## Langkah Kompilasi dan Menjalankan Program

1. Buka terminal pada direktori proyek.
2. Kompilasi seluruh berkas sumber menggunakan GCC:

```bash
g++ main.cpp -o apex_projectile
```

3. Jalankan program:

```bash
./apex_projectile
```

## Cara Bermain

1. Pilih menu **Start Game** pada menu utama.
2. Perhatikan posisi target dan obstacle yang muncul pada area permainan.
3. Masukkan sudut tembakan yang diinginkan.
4. Masukkan kekuatan peluncuran proyektil.
5. Amati lintasan proyektil dan interaksi fisika yang terjadi.
6. Hancurkan target sebelum seluruh kesempatan tembakan habis.
7. Kumpulkan kemenangan dengan strategi sudut dan kekuatan yang tepat.

## Tim Pengembang

Proyek ini dikembangkan sebagai Project Akhir Mata Kuliah Algoritma dan Pemrograman menggunakan bahasa C++ dengan metode pengembangan kolaboratif berbasis GitHub.
