#include <iostream>
#include <chrono>
#include <thread>
#include <string>
using namespace std;

void hapus (){
    cout << "\033[2J\033[H";
}

void delay (int MS) {
    this_thread::sleep_for(chrono::milliseconds(MS));
}

void ketik (string teks, int kecMS = 50){
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

    cout << "[Tekan ENTER untuk memulai...]";
    cin.get();

}
