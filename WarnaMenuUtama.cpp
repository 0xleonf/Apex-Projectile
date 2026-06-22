#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

void tampilMenu(int pilihan);
void startGame();
void petunjuk();
void tentang();

void tampilMenu(int pilihan)
{
    system("cls");

    string menu[4] = {
        "START",
        "HOW TO PLAY",
        "BACKGROUND STORY",
        "EXIT"
    };
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

cout << "\to====================================================================================================o\n" << endl;
SetConsoleTextAttribute(h, 4);
cout << "\t           _    ____  _______  __    ____  ____   ___      _ _____ ____ _____ ___ _     _____\n";          
cout << "\t          / \\  |  _ \\| ____\\ \\/ /   |  _ \\|  _ \\ / _ \\    | | ____/ ___|_   _|_ _| |   | ____|\n";
cout << "\t         / _ \\ | |_) |  _|  \\  /    | |_) | |_) | | | |   | |  _|| |     | |  | || |   |  _|\n";
cout << "\t        / ___ \\|  __/| |___ /  \\    |  __/|  _ <| |_| |___| | |__| |___  | |  | || |___| |__\n";
cout << "\t       /_/   \\_\\_|   |_____/_/\\_\\   |_|   |_| \\_\\\\___/|_____|_____\\\\____| |_| |___|_____|____|\n"<< endl;
SetConsoleTextAttribute(h, 0);
cout << "\to====================================================================================================o\n";
cout << "\n";

cout << "\n";
    SetConsoleTextAttribute(h, 14);

   
    for (int i = 0; i < 4; i++)
    {
        if (i == pilihan)
            SetConsoleTextAttribute(h, 6); // orange
        else
            SetConsoleTextAttribute(h, 7);  // Putih

        cout << "\t\t\t\t\t\t+----------------------+\n";

        string isi;

        if (i == pilihan)
            isi = "> " + menu[i];
        else
            isi = "  " + menu[i];

        cout << "\t\t\t\t\t\t| ";

        cout << isi;

        int sisa = 20 - isi.length();

        for (int j = 0; j < sisa; j++)
            cout << " ";

        cout << " |\n";

        cout << "\t\t\t\t\t\t+----------------------+\n\n";
    }

    SetConsoleTextAttribute(h, 8);

    cout << "\n";
    cout << "\t\t\t\t\tGunakan PANAH ATAS/BAWAH untuk memilih\n";
    cout << "\t\t\t\t\t     Tekan ENTER untuk melanjutkan\n";

    SetConsoleTextAttribute(h, 7);
}
void startGame()
{
    system("cls");

    cout << "================ START GAME ================\n\n";
    cout << "Game dimulai...\n\n";

    cout << "Tekan tombol apa saja untuk kembali...";
    getch();
}

void petunjuk()
{
    system("cls");

    cout << "================ HOW TO PLAY ================\n\n";

    cout << "1. Gunakan tombol PANAH ATAS dan PANAH BAWAH........\n";
    cout << "Tekan tombol apa saja untuk kembali...";
    getch();
}

void tentang()
{
    system("cls");

    cout << "============= BACKGROUND STORY =============\n\n";
    cout << "Tekan tombol apa saja untuk kembali...";
    getch();
}

int main()
{
    int pilihan = 0;
    char tombol;

    while (true)
    {
        tampilMenu(pilihan);

        tombol = getch();

        if (tombol == -32)
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
                    system("cls");

                    SetConsoleTextAttribute(
                        GetStdHandle(STD_OUTPUT_HANDLE), 14);

                    cout << "\n\n";
                    cout << "             Terima kasih telah memainkan APEX PROJECTILE!\n\n";

                    SetConsoleTextAttribute(
                        GetStdHandle(STD_OUTPUT_HANDLE), 7);

                    return 0;
            }
        }
    }

    return 0;
}