#include <iostream>
#include <chrono>
using namespace std;

void tempatkursor(int x, int y) {                //1. y : nomor baris         2. H = kek pindahin kursor ke sini
    cout << "\033[" << y << ";" << x << "H";    //3. x : nomor kolom          4. ; = pemisah antara baris dan kolom
}//nanti jadi \033[y;xH ,jadi kek perintah pindahin kursor ke baris y kolom x

void ketapel() {
    tempatkursor(1, 6);  cout << "|   |";
    tempatkursor(1, 7);  cout << "|";
    tempatkursor(5, 7);  cout << "|";
    tempatkursor(1, 8);  cout << "|||||";
    tempatkursor(1, 9);  cout << " |||";
    tempatkursor(1, 10); cout << " |||";
    tempatkursor(1, 11); cout << " |||";
}

void hapus() {
    cout << "\033[2J";
}//buat hapus layar

void hidecursor() { cout << "\033[?25l"; } //biar kalo dijalanin ga ada kursornya 
void showcursor() { cout << "\033[?25h"; } //buat nampilin kursornya lagi

void sleep_ms(int ms) {
    auto start = chrono::steady_clock::now(); 
    //1. auto itu type data yg otomatis disesuaikan sama compiler
    //2. chrono::steady_clock::now(); buat ambil waktu sekarang yaitu sejak komputermu pertama kali dinyalakan udah berapa detik gitu
    
    while (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count() < ms);
    //1. hitung selisih waktu sekarang sama yg awal, terus diubah jadi milisecond
    //2. pake count buat ngambil hasil abis diubah jadi milisecond, karna kan hasil dari diubahnya itu bukan angka biasa jadi gabisa dibandingin sama ms yg meruakan int
    //3. <ms buat ngecek apkh belum sampe durasi yg diminta (80)
    //4. pake while ini biar dia terus ngecek selama 80ms buat lanjut ke gerakan bola selanjutnya ngerti ga
}

int main(){
    const int WIDTH = 70;
    const int Y = 7;

    int ulang=0;

    hidecursor();
    hapus();
    ketapel();
    tempatkursor(3,7);cout<<"O";
    tempatkursor(1,12);cout<<"pencet enter";
    cin.get();
    
    
    for (int x = 3; x <= WIDTH; x++) {
        hapus();
        ketapel();
        tempatkursor(x, Y);
        cout << "O";

        cout.flush(); //buat animasinya ga patah patah

        sleep_ms(70);
    }
    
    showcursor();

    cout << "\n\n\n\nSelesai!\n";

    system("pause");
    return 0;
    //ngerti ga
}