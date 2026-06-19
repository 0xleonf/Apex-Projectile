#include <iostream>
using namespace std;

//  WARNA ANSI 
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"
#define GOLD    "\033[38;5;220m"
#define PURPLE  "\033[38;5;129m"

// Garis pembatas
void printDivider() {
    cout << "================================================================\n";
}

void howToPlay() {
    system("cls");   

    //bagian 1: OBJECTIVE 
    cout<< BOLD << GOLD;
    cout << "================================================================\n";
    cout << "|                      H O W   T O   P L A Y                   |\n";
    cout << "================================================================\n\n";

    //Objek
    cout << BOLD << BLUE << "OBJECTIVE" << RESET << endl;
    printDivider();
    cout << "Hancurkan seluruh target menggunakan bola yang tersedia.\n";
    cout << "Selesaikan setiap level dengan jumlah tembakan sesedikit\n";
    cout << "mungkin untuk memperoleh skor terbaik.\n\n";
    cout << "Tekan Enter untuk melanjutkan........";
    cin.get();
    system("cls");

    // bagian 2: LANGKAH BERMAIN 
    cout << BOLD << GREEN << "LANGKAH BERMAIN" << RESET << endl;
    printDivider();

    cout << BOLD << YELLOW << "1. Mulai Permainan" << RESET << endl;
    cout << "   Pilih menu " << BOLD << "START" << RESET << " pada Main Menu untuk\n";
    cout << "   memulai permainan.\n\n";

    cout << BOLD << YELLOW << "2. Atur Sudut Peluncuran" << RESET << endl;
    cout << "   Masukkan sudut (Angle) peluncuran bola.\n";
    cout << "   Sudut yang berbeda akan menghasilkan\n";
    cout << "   lintasan yang berbeda pula.\n\n";

    cout << BOLD << YELLOW << "3. Atur Kekuatan Tembakan" << RESET << endl;
    cout << "   Masukkan nilai kekuatan (Power) untuk\n";
    cout << "   menentukan seberapa jauh bola akan\n";
    cout << "   diluncurkan menuju target.\n\n";

    cout << BOLD << YELLOW << "4. Luncurkan Bola" << RESET << endl;
    cout << "   Setelah sudut dan kekuatan ditentukan,\n";
    cout << "   bola akan bergerak mengikuti gravitasi.\n";
    cout << "   Perhatikan arah lintasannya agar\n";
    cout << "   mengenai target.\n\n";
    cout << "Tekan Enter untuk melanjutkan........";
    cin.get();
    system("cls");

    //bagian 3 : TARGET, BOLA, LEVEL
    //Target
    cout << BOLD << RED << "HANCURKAN TARGET" << RESET << endl;
    printDivider();
    cout << "Bidik seluruh target dengan tepat.\n";
    cout << "Setiap target yang berhasil dihancurkan\n";
    cout << "akan menambah skor dan membawamu lebih\n";
    cout << "dekat menuju kemenangan.\n\n";    

    //Bola
    cout << BOLD << CYAN << "BOLA TERBATAS" << RESET << endl;
    printDivider();
    cout << "Jumlah bola pada setiap permainan terbatas.\n";
    cout << "Gunakan setiap bola secara efektif karena\n";
    cout << "setiap tembakan sangat berharga.\n";
    cout << "Semakin sedikit bola yang digunakan,\n";
    cout << "semakin tinggi skor yang diperoleh.\n\n";

    //Level
    cout << BOLD << PURPLE << "DYNAMIC LEVEL" << RESET << endl;
    printDivider();
    cout << "Setiap kali permainan dimulai, susunan\n";
    cout << "target dan rintangan akan diacak.\n";
    cout << "Hal ini membuat setiap permainan memiliki\n";
    cout << "tantangan yang berbeda sehingga pemain\n";
    cout << "harus menyesuaikan strategi, sudut,\n";
    cout << "dan kekuatan tembakan.\n\n";
    cout << "Tekan Enter untuk melanjutkan........";
    cin.get();
    system("cls");

    //bagian 4: MENANG/KALAH 
    cout << BOLD << GREEN << "MENANG" << RESET << endl;
    printDivider();
    cout << "Hancurkan seluruh target untuk\n";
    cout << "menyelesaikan level.\n\n";

    cout << BOLD << RED << "KALAH" << RESET << endl;
    printDivider();
    cout << "Permainan berakhir apabila seluruh bola\n";
    cout << "habis sebelum semua target berhasil\n";
    cout << "dihancurkan.\n\n";

    cout << "Tekan ENTER untuk kembali ke Main Menu......";
    cin.get();
    cin.ignore(1000, '\n');
}

int main() {
    howToPlay();
    return 0;
}