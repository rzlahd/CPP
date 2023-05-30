#include <iostream>
using namespace std;

int main()
{
    char c = 'a';
    char∗ p = &c; // p menyimpan alamat c; & adalah address-dari operator
    char c2 = ∗p; // c2 == ’a’; * adalah dereference operator
    
// Input cin
    char name[25];
    cout << "Masukan Nama \n";
    cin >> name;
    cout << "Halo " << name << endl;
// Input getline
    char nama2[25];
    cout << "Masukan Nama \n";
    cin.getline(nama2,25);
    cout << "Halo " << nama2 << endl;

    return 0;
}
