#include <iostream>
#include <string>
using namespace std;

//waiting list

struct Pelanggan {
    string nama;
    Pelanggan* next;
};

struct Queue {
    Pelanggan* front;
    Pelanggan* rear;
};

void initQueue(Queue &q) {
    q.front = nullptr;
    q.rear = nullptr;
}


void enqueue(Queue &q, string nama) {
    Pelanggan* baru = new Pelanggan;
    baru->nama = nama;
    baru->next = nullptr;

    if (q.rear == nullptr) {
        q.front = baru;
        q.rear = baru;
    } else {
        q.rear->next = baru;
        q.rear = baru;
    }
}


void enqueueFront(Queue &q, string nama) {
    Pelanggan* baru = new Pelanggan;
    baru->nama = nama;
    baru->next = q.front;

    q.front = baru;

    if (q.rear == nullptr) {
        q.rear = baru;
    }
}


string dequeue(Queue &q) {
    if (q.front == nullptr) {
        return "";
    }

    Pelanggan* hapus = q.front;
    string nama = hapus->nama;

    q.front = q.front->next;

    if (q.front == nullptr) {
        q.rear = nullptr;
    }

    delete hapus;
    return nama;
}

void hapusBelakang(Queue &q) {
    if (q.front == nullptr) {
        return;
    }

    if (q.front == q.rear) {
        delete q.front;
        q.front = nullptr;
        q.rear = nullptr;
        return;
    }

    Pelanggan* bantu = q.front;

    while (bantu->next != q.rear) {
        bantu = bantu->next;
    }

    delete q.rear;
    q.rear = bantu;
    q.rear->next = nullptr;
}

void tampilQueue(Queue q) {
    if (q.front == nullptr) {
        cout << "Antrian kosong\n";
        return;
    }

    Pelanggan* bantu = q.front;

    while (bantu != nullptr) {
        cout << bantu->nama << " menunggu menu\n";
        bantu = bantu->next;
    }
}

//bst

struct Node {
    string nama;
    string kategori;
    int harga;
    bool tersedia;

    Queue antrian;

    Node* left;
    Node* right;
};

Node* buatNode(string nama, string kategori, int harga) {
    Node* baru = new Node;

    baru->nama = nama;
    baru->kategori = kategori;
    baru->harga = harga;
    baru->tersedia = true;

    initQueue(baru->antrian);

    baru->left = nullptr;
    baru->right = nullptr;

    return baru;
}

Node* tambahMenu(Node* root, string nama,
                 string kategori, int harga) {

    if (root == nullptr) {
        return buatNode(nama, kategori, harga);
    }

    if (nama < root->nama) {
        root->left = tambahMenu(
            root->left, nama, kategori, harga);
    }
    else if (nama > root->nama) {
        root->right = tambahMenu(
            root->right, nama, kategori, harga);
    }
    else {
        cout << "Menu sudah ada!\n";
    }

    return root;
}

Node* cariMenu(Node* root, string nama) {
    if (root == nullptr || root->nama == nama) {
        return root;
    }

    if (nama < root->nama) {
        return cariMenu(root->left, nama);
    }

    return cariMenu(root->right, nama);
}

void tampilMenu(Node* root) {
    if (root == nullptr) {
        return;
    }

    tampilMenu(root->left);

    cout << "\nNama     : " << root->nama;
    cout << "\nKategori : " << root->kategori;
    cout << "\nHarga    : Rp" << root->harga;
    cout << "\nStatus   : "
         << (root->tersedia ? "Tersedia" : "Habis");
    cout << "\n-------------------------\n";

    tampilMenu(root->right);
}


void hapusQueue(Queue &q) {
    while (q.front != nullptr) {
        dequeue(q);
    }
}


Node* hapusMenu(Node* root, string nama) {
    if (root == nullptr) {
        return nullptr;
    }

    if (nama < root->nama) {
        root->left = hapusMenu(root->left, nama);
    }
    else if (nama > root->nama) {
        root->right = hapusMenu(root->right, nama);
    }
    else {

        if (root->left == nullptr) {
            Node* temp = root->right;
            hapusQueue(root->antrian);
            delete root;
            return temp;
        }

        
        if (root->right == nullptr) {
            Node* temp = root->left;
            hapusQueue(root->antrian);
            delete root;
            return temp;
        }

        
        Node* penerus = root->right;

        while (penerus->left != nullptr) {
            penerus = penerus->left;
        }

        root->nama = penerus->nama;
        root->kategori = penerus->kategori;
        root->harga = penerus->harga;
        root->tersedia = penerus->tersedia;

        hapusQueue(root->antrian);

        while (penerus->antrian.front != nullptr) {
            enqueue(root->antrian,
                    dequeue(penerus->antrian));
        }

        root->right = hapusMenu(
            root->right, penerus->nama);
    }

    return root;
}

void bebaskanTree(Node* &root) {
    if (root == nullptr) {
        return;
    }

    bebaskanTree(root->left);
    bebaskanTree(root->right);

    hapusQueue(root->antrian);
    delete root;
    root = nullptr;
}

//undo

struct Aksi {
    int jenis;
    string namaMenu;
    string namaPelanggan;
    bool masukAntrian;
    string pelangganDilayani;

    Aksi* next;
};

Aksi* top = nullptr;


void push(int jenis, string menu,
          string pelanggan = "",
          bool antrian = false,
          string dilayani = "") {

    Aksi* baru = new Aksi;

    baru->jenis = jenis;
    baru->namaMenu = menu;
    baru->namaPelanggan = pelanggan;
    baru->masukAntrian = antrian;
    baru->pelangganDilayani = dilayani;

    baru->next = top;
    top = baru;
}

void bebaskanStack() {
    while (top != nullptr) {
        Aksi* hapus = top;
        top = top->next;
        delete hapus;
    }
}


bool bacaAngka(string pesan, int &hasil) {
    string input;

    cout << pesan;
    getline(cin, input);

    try {
        hasil = stoi(input);
    } catch (...) {
        return false;
    }

    return true;
}

//==========================================
// 1. Menambahkan menu
void fiturTambahMenu(Node* &root) {
    string nama, kategori;
    int harga;

    cout << "Nama menu     : ";
    getline(cin, nama);

    if (cariMenu(root, nama) != nullptr) {
        cout << "Menu sudah ada!\n";
        return;
    }

    cout << "Kategori      : ";
    getline(cin, kategori);

    if (!bacaAngka("Harga         : ", harga)) {
        cout << "Harga tidak valid\n";
        return;
    }

    root = tambahMenu(root, nama, kategori, harga);

    cout << "Menu berhasil ditambahkan\n";
}

// 2. Melihat menu
void fiturLihatMenu(Node* root) {
    if (root == nullptr) {
        cout << "Belum ada menu\n";
        return;
    }

    tampilMenu(root);
}

// 3. Mencatat pesanan
void fiturPesan(Node* root) {
    string pelanggan, namaMenu;

    cout << "Nama pelanggan : ";
    getline(cin, pelanggan);

    cout << "Nama menu      : ";
    getline(cin, namaMenu);

    Node* menu = cariMenu(root, namaMenu);

    if (menu == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }

    if (menu->tersedia) {
        push(1, namaMenu, pelanggan, false);

        cout << "Pesanan " << pelanggan
             << " berhasil dibuat\n";
    }
    else {
        push(1, namaMenu, pelanggan, true);

        enqueue(menu->antrian, pelanggan);

        cout << "Menu sedang habis, "
             << pelanggan
             << " masuk ke dalam waiting list\n";
    }
}

// 4. Memperbarui status menu
void fiturUpdateMenu(Node* root) {
    string nama;

    cout << "Nama menu: ";
    getline(cin, nama);

    Node* menu = cariMenu(root, nama);

    if (menu == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }

    string dilayani = "";

    menu->tersedia = true;

    if (menu->antrian.front != nullptr) {
        dilayani = dequeue(menu->antrian);
    }

    push(2, nama, "", false, dilayani);

    cout << "Menu tersedia kembali\n";

    if (dilayani != "") {
        cout << "Pesanan langsung diberikan kepada "
             << dilayani
             << " dari waiting list\n";
    }
}

// 5. Menghapus menu
void fiturHapusMenu(Node* &root) {
    string nama;

    cout << "Nama menu yang dihapus: ";
    getline(cin, nama);

    if (cariMenu(root, nama) == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }

    root = hapusMenu(root, nama);

    cout << "Menu berhasil dihapus\n";
}

// 6. Undo aktivitas
void fiturUndo(Node* root) {
    if (top == nullptr) {
        cout << "Tidak ada aksi untuk di-undo\n";
        return;
    }

    Aksi* hapus = top;
    top = top->next;

    Node* menu = cariMenu(root, hapus->namaMenu);

    if (menu == nullptr) {
        cout << "Menu sudah tidak ditemukan\n";
        delete hapus;
        return;
    }

    if (hapus->jenis == 1) {
        if (hapus->masukAntrian) {
            hapusBelakang(menu->antrian);
        }

        cout << "Undo: Pesanan menu "
             << hapus->namaMenu
             << " dibatalkan\n";
    }
    else if (hapus->jenis == 2) {
        menu->tersedia = false;
        if (hapus->pelangganDilayani != "") {
            enqueueFront(
                menu->antrian,
                hapus->pelangganDilayani
            );
        }

        cout << "Undo: Menu "
             << hapus->namaMenu
             << " kembali habis\n";
    }

    delete hapus;
}

// 7. Melihat waiting list
void fiturWaitingList(Node* root) {
    string nama;

    cout << "Nama menu: ";
    getline(cin, nama);

    Node* menu = cariMenu(root, nama);

    if (menu == nullptr) {
        cout << "Menu tidak ditemukan\n";
        return;
    }

    if (menu->antrian.front == nullptr) {
        cout << "Antrian kosong\n";
        return;
    }

    Pelanggan* bantu = menu->antrian.front;

    while (bantu != nullptr) {
        cout << bantu->nama
             << " menunggu menu "
             << menu->nama << endl;

        bantu = bantu->next;
    }
}

//=======================================================================================================================

int main() {
    Node* root = nullptr;
    int pilihan;

    do {
        cout << "\n================================\n";
        cout << "     SISTEM PEMESANAN RESTORAN\n";
        cout << "================================\n";
        cout << "1. Tambah Menu\n";
        cout << "2. Lihat Daftar Menu\n";
        cout << "3. Pesan Menu\n";
        cout << "4. Menu Tersedia Kembali\n";
        cout << "5. Hapus Menu\n";
        cout << "6. Undo Aktivitas\n";
        cout << "7. Lihat Waiting List\n";
        cout << "8. Keluar\n";
        cout << "================================\n";

        if (!bacaAngka("Pilih menu: ", pilihan)) {
            pilihan = 0;
        }

        switch (pilihan) {
            case 1:
                fiturTambahMenu(root);
                break;

            case 2:
                fiturLihatMenu(root);
                break;

            case 3:
                fiturPesan(root);
                break;

            case 4:
                fiturUpdateMenu(root);
                break;

            case 5:
                fiturHapusMenu(root);
                break;

            case 6:
                fiturUndo(root);
                break;

            case 7:
                fiturWaitingList(root);
                break;

            case 8:
                cout << "Terima kasih!\n";
                break;

            default:
                cout << "Pilihan tidak valid!\n";
        }

    } while (pilihan != 8);


    bebaskanTree(root);
    bebaskanStack();

    return 0;
}