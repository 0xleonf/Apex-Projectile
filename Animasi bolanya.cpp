#include <iostream>
#include <chrono>
using namespace std;

void tempatkursor(int x, int y) {                //1. y : nomor baris         2. H = kek pindahin kursor ke sini
    cout << "\033[" << y << ";" << x << "H";    //3. x : nomor kolom          4. ; = pemisah antara baris dan kolom
}//nanti jadi \033[y;xH ,jadi kek perintah pindahin kursor ke baris y kolom x

void ketapel() {
    tempatkursor(1, 16);  cout << R"(      \)";
    tempatkursor(1, 17);  cout << R"(       \|)";
    tempatkursor(1, 18);  cout << R"(        |)";
    tempatkursor(1, 19); cout <<  R"(        |)";
    tempatkursor(1, 20); cout <<  R"(_______/\_________________________________________________________________________________________)";    
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
    const int HEIGHT = 20;
    const float gravitasi = 0.25;
    int sudut,kekuatan;

    do{
    cout<<"Masukkan sudut lemparan (30,45,60,75,90): ";
    cin>>sudut;

    if(sudut!=30 && sudut!=45 && sudut!=60 && sudut!=75 && sudut!=90 || cin.fail()){
        cin.clear();cin.ignore(100,'\n');
        cout<<"\033[31m"<<"[!] Input tidak valid\n"<<"\033[0m";

    }}while(sudut!=30 && sudut!=45 && sudut!=60 && sudut!=75 && sudut!=90 || cin.fail());
    
    cout<<"Masukkan kekuatan lemparan (1-10): ";
    cin>>kekuatan;  
    while((kekuatan < 1 || kekuatan > 10) || cin.fail()){
        cin.clear(); cin.ignore(100,'\n');
        cout<<"\033[31m"<<"[!] Input tidak valid\n"<<"\033[0m";
        cout<<"Masukkan kekuatan lemparan (1-5): ";
        cin>>kekuatan;
    }
    float(kekuatan/10);
 
//  =================== buat ngatur kecepatan bolanya ===================================

    float vx, vy;
    switch(sudut){
        case 30: vx=0.5*kekuatan;  vy=-0.1*kekuatan;  break;
        case 45: vx=0.5*kekuatan;    vy=-0.2*kekuatan;  break;
        case 60: vx=0.5*kekuatan;    vy=-0.3*kekuatan;  break;
        case 75: vx=0.5*kekuatan;    vy=-0.4*kekuatan;  break;
        case 90: vx=0;             vy=-0.5*kekuatan;  break;
    }
    

//  =============================DI BAWAH MULAI JALANIN BOLANYA======================================
    hidecursor();
    
    //posisi awal bola
    float x=9;
    float y=16;

    while(!(y >= HEIGHT && vy > 0)){
        hapus();
        ketapel();

        // hanya print kalau bola masih dalam layar
        if(y >= 1 && y < 20 && x >= 1 && x <= WIDTH){
            tempatkursor((int)x, (int)y);
            cout << "O";
        }

        cout.flush(); //buat animasinya ga patah patah
        sleep_ms(100);

        x+=vx;
        vy+=gravitasi;
        y+=vy;
    }

    showcursor();
    tempatkursor(1,HEIGHT+2);

    cout << "Selesai!\n";

    system("pause");
    return 0;
    //ngerti ga
}