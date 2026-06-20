#include <iostream>
#include <conio.h>

using namespace std;

void tampilMenu(int pilihan);
void startGame();
void petunjuk();
void tentang();

void tampilMenu(int pilihan) {
    system("cls");

    string menu[4] = {
        "Start",
        "How To Play",
        "Backroumd Story",
        "Exit"
    };
cout << "o===========================================================================================================o\n" << endl;
cout << "           _    ____  _______  __    ____  ____   ___      _ _____ ____ _____ ___ _     _____\n";          
cout << "          / \\  |  _ \\| ____\\ \\/ /   |  _ \\|  _ \\ / _ \\    | | ____/ ___|_   _|_ _| |   | ____|\n";
cout << "         / _ \\ | |_) |  _|  \\  /    | |_) | |_) | | | |   | |  _|| |     | |  | || |   |  _|\n";
cout << "        / ___ \\|  __/| |___ /  \\    |  __/|  _ <| |_| |___| | |__| |___  | |  | || |___| |__\n";
cout << "       /_/   \\_\\_|   |_____/_/\\_\\   |_|   |_| \\_\\\\___/|_____|_____\\\\____| |_| |___|_____|____|\n"<< endl;
cout << "o===========================================================================================================o\n";
cout << "\n";

cout << "\n";


    for (int i = 0; i < 4; i++) {
        if (i == pilihan)
            cout << "-> " << menu[i] << endl;
       else
           cout << "   " << menu[i] << endl;
    }
}

void startGame() {
    system("cls");

    cout << "=== START GAME ===\n";
    cout << "Game dimulai...\n";

    getch();
}

void petunjuk() {
    system("cls");

    cout << "=== PETUNJUK ===\n";
    cout << "Gunakan tombol panah untuk navigasi.\n";
    cout << "Tekan ENTER untuk memilih menu.\n";

    getch();
}

void tentang() {
    system("cls");

    cout << "=== TENTANG ===\n";
    cout << "Program dibuat menggunakan C++.\n";

    getch();
}
    
    

int main() {
    int pilihan = 0;
    char tombol;

    while (true) {
        tampilMenu(pilihan);

        tombol = getch();

        if (tombol == -32) {
            tombol = getch();

            if (tombol == 72) { // Panah Atas 
                pilihan--;

                if (pilihan < 0)
                    pilihan = 3;
            }
            else if (tombol == 80) { // Panah Bawah
                pilihan++;

                if (pilihan > 3)
                    pilihan = 0;
            }
        }

        else if (tombol == 13) { // Enter

            switch (pilihan) {
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
                    cout << "\nTerima kasih telah menggunakan program ini.\n";
                    return 0;
            }
        }
    }

    return 0;
}

