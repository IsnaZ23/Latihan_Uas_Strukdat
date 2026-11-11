#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct NodeAntrian{
    string pelanggan;
    NodeAntrian *next;
};

struct Antrian{
    NodeAntrian *front;
    NodeAntrian *rear;
};

struct Menu{
    string nama;
    string kategori;
    int harga;
    bool tersedia;
    Antrian antrian;
};





int main() {
    
    return 0;
}