#include <iostream>
#include <conio.h>
using namespace std;

int main() {
    int pilihan = 0;
    string menu[3] = {
        "Start Game",
        "Petunjuk Main",
        "Keluar"
        
    };

    while (true) {  
        system("cls");

        cout << "=====================\n";
        cout << "      GAME SAYA      \n";
        cout << "=====================\n\n";

        for (int i = 0; i < 3; i++) {
            if (i == pilihan)
                cout << " > " << menu[i] << endl;
            else
                cout << "   " << menu[i] << endl;
        }

        char tombol = _getch();

        if (tombol == -32) {
            tombol = _getch();

            if (tombol == 72) { // panah atas
                pilihan--;
                if (pilihan < 0)
                    pilihan = 2;
            }

            if (tombol == 80) { // panah bawah
                pilihan++;
                if (pilihan > 2)
                    pilihan = 0;
            }
        }

        else if (tombol == 13) { // Enter
            system("cls");

            if (pilihan == 0) {
                cout << "=== START GAME ===\n";
                cout << "Game dimulai...\n";
            }
            else if (pilihan == 1) {
                cout << "=== PETUNJUK MAIN ===\n";
                cout << "1. Gunakan panah untuk bergerak.\n";
                cout << "2. Kumpulkan poin sebanyak mungkin.\n";
                cout << "3. Hindari rintangan.\n";
            }
            else if (pilihan == 2) {
                cout << "Terima kasih telah bermain!\n";
                break;
            }

            cout << "\n\nTekan sembarang tombol...";
            _getch();
        }
    }

    return 0;
}