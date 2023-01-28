/*
MIT License

Copyright (c) 2023 izal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <fstream>
#include <iostream>
using std::cout;
using std::endl;

#include <json/json.h>
using namespace Json;

int main(int argc, char* argv[]) {
    Reader Baca;
    Value Akar;
    std::ifstream ifs;
    if(argv[1]) {
        ifs.open(argv[1]);
    } else {
        ifs.open("data.json");
    }

    Baca.parse(ifs, Akar); // reader can also read strings

    cout << "Book: " << Akar["Kopi Careh"].asUInt() << endl;

    const Value& buku = Akar["Daftar_isi"]; // array of characters
    for (int i = 0; i < buku.size(); i++){
        cout << "   Buku    : " << buku[i]["Buku"].asUInt()<< endl;
        cout << "   Nama    : " << buku[i]["nama"].asString()<< endl;
        cout << "   Tahun   : " << buku[i]["tahun"].asString()<< endl;
        cout << endl;
    }

    std::cout << "Patch Data: " << std::endl;
    std::cout << Akar << std::endl;

    return EXIT_SUCCESS;
}
