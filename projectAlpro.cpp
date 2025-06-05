#include <iostream>
#include <fstream>
#include <iomanip>
#define max 100
using namespace std;

int jumlahBarang = 0;
int tempIdBarang[max];
string konfirmasi, nama_db = "daftarbarang.txt", fileStruk = "struk.txt";

int cariIndexBarang(int idBarang);
void menu();
void bacaDariFile();
void cetakDataBarang(int idbarang, string namabarang, int jumlahbarang, float hargabarang);
void inputBarang();
void daftarBarang();
void sortBarang(int opsiSort, bool ascending);
void penjualan();
void editBarang();
void hapusBarang();
void cetakStruk(string namabarang, int jumlah, float hargasatuan, float total);
void cetakFooterStruk(float totalbayar);
bool idSudahAda(int id, int countBaru);
float totalHarga(int hargaSatuan, int jumlahBarangDibeli);
float cariHarga(int idBarang);

struct Barang
{
    string namaBarang;
    int jumlahBarang, idBarang;
    float hargaBarang, daftarBarang;
};
Barang barang[max];

int main()
{
    string usn, pass;
    int batas = 0;
    system("cls");

    bacaDariFile();
    while (batas != 3)
    {
        cout << "====================================================================" << endl;
        cout << "|                               LOGIN                              |" << endl;
        cout << "====================================================================" << endl;
        cout << "Username       : ";
        cin >> usn;
        cout << "Password       : ";
        cin >> pass;

        if (usn == "admin" && pass == "12345")
        {
            menu();
        }
        else
        {
            cout << "Username atau Password Salah!" << endl
                 << endl;
            system("pause");

            system("cls");
            cout << "Kesempatan anda sisa " << 2 - batas << " kali percobaan" << endl;
            batas++;
        }
    }
}

float totalHarga(int hargaSatuan, int jumlahBarangDibeli)
{
    if (jumlahBarangDibeli <= 1)
    {
        return hargaSatuan;
    }
    else
    {
        return hargaSatuan + totalHarga(hargaSatuan, jumlahBarangDibeli - 1);
    }
}

void bacaDariFile()
{
    ifstream file(nama_db);
    string line;

    jumlahBarang = 0;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string idStr, nama, jumlahStr, hargaStr;

        getline(ss, idStr, '|');
        getline(ss, nama, '|');
        getline(ss, jumlahStr, '|');
        getline(ss, hargaStr, '|');

        // Cek apakah semua bagian tersedia atau tidak ada yang kosong, kalo kosong tinggalin aja
        if (idStr.empty() || nama.empty() || jumlahStr.empty() || hargaStr.empty())
            continue;

        try
        {
            barang[jumlahBarang].idBarang = stoi(idStr);
            barang[jumlahBarang].namaBarang = nama;
            barang[jumlahBarang].jumlahBarang = stoi(jumlahStr);
            barang[jumlahBarang].hargaBarang = stof(hargaStr);
            jumlahBarang++;
        }
        catch (invalid_argument &e)
        {
            // Lewati baris rusak
            continue;
        }
    }
    file.close();
}

void cetakDataBarang(int idbarang, string namabarang, int jumlahbarang, float hargabarang)
{
    ofstream file(nama_db, ios::app);
    if (file.is_open())
    {
        file << idbarang << "|" << namabarang << "|" << jumlahbarang << "|" << hargabarang << "\n";
        file.close();
    }
    else
    {
        cout << "Gagal menyimpan ke file!" << endl;
    }
}

float cariHarga(int idBarang)
{
    bool found = false;
    int awal, tengah, akhir;

    awal = 0;
    akhir = jumlahBarang;

    while ((!found) && (awal <= akhir))
    {
        tengah = (awal + akhir) / 2;

        if (idBarang == barang[tengah].idBarang)
        {
            found = true;
        }
        else
        {
            if (idBarang < barang[tengah].idBarang)
            {
                akhir = tengah - 1;
            }
            else
            {
                awal = tengah + 1;
            }
        }
    }

    if (found)
    {
        return barang[tengah].hargaBarang;
    }
    else
    {
        return 0;
    }
}

void sortBarang(int opsiSort, bool ascending)
{
    for (int i = 0; i < jumlahBarang - 1; i++)
    {
        for (int j = 0; j < jumlahBarang - i - 1; j++)
        {
            bool kondisi = false;
            switch (opsiSort)
            {
            case 1: // Berdasarkan Stok Barang
                kondisi = ascending ? barang[j].jumlahBarang > barang[j + 1].jumlahBarang
                                    : barang[j].jumlahBarang < barang[j + 1].jumlahBarang;
                break;
            case 2: // Berdasarkan Nama Barang
                kondisi = ascending ? barang[j].namaBarang > barang[j + 1].namaBarang
                                    : barang[j].namaBarang < barang[j + 1].namaBarang;
                break;
            case 3: // Berdasarkan Harga Barang
                kondisi = ascending ? barang[j].hargaBarang > barang[j + 1].hargaBarang
                                    : barang[j].hargaBarang < barang[j + 1].hargaBarang;
                break;
            }
            if (kondisi)
            {
                swap(barang[j], barang[j + 1]);
            }
        }
    }
}

void inputBarang()
{
    int jumlahInput, idbarang, jumlahbarang;
    string namabarang;
    float hargabarang;
    bool ulangInput;

    system("cls");
    cout << "====================================================================" << endl;
    cout << "|                            INPUT BARANG                          |" << endl;
    cout << "====================================================================" << endl;
    cout << "Masukkan jumlah barang yang ingin diinput: ";
    cin >> jumlahInput;
    cout << endl;

    if (jumlahInput != 0 && jumlahInput + jumlahBarang <= max)
    {
        int countBaru = 0;
        for (int i = 0; i < jumlahInput; i++)
        {
            cout << "Detail barang ke-" << i + 1 << endl;
            cout << "------------------" << endl;

            do
            {
                ulangInput = false;
                cout << "ID Barang     : ";
                cin >> idbarang;
                if (idSudahAda(idbarang, countBaru))
                {
                    ulangInput = true;
                    cout << "ID barang sudah digunakan. Silahkan gunakan ID yang lain!\n"
                         << endl;
                }

            } while (ulangInput);

            cout << "Nama Barang   : ";
            cin.ignore();
            getline(cin, namabarang);
            cout << "Jumlah Barang : ";
            cin >> jumlahbarang;
            cout << "Harga Barang  : ";
            cin >> hargabarang;
            cout << endl;

            jumlahBarang++;
            tempIdBarang[countBaru] = idbarang;
            cetakDataBarang(idbarang, namabarang, jumlahbarang, hargabarang);
            countBaru++;
        }
    }
    else
    {
        cout << "Jumlah barang melebihi kapasitas maksimal!" << endl;
    }
}

bool idSudahAda(int id, int countBaru)
{
    // Cek ID di barang[] (data dari file)
    for (int i = 0; i < jumlahBarang; i++)
    {
        if (barang[i].idBarang == id)
            return true;
    }
    // Cek ID di tempIdBarang[] (yang baru diinput di sesi saat ini)
    for (int i = 0; i < countBaru; i++)
    {
        if (tempIdBarang[i] == id)
            return true;
    }
    return false;
}

void daftarBarang()
{
    system("cls");
    bacaDariFile();

    cout << "====================================================================" << endl;
    cout << "|                           DAFTAR BARANG                          |" << endl;
    cout << "====================================================================" << endl;

    if (jumlahBarang == 0)
    {
        cout << "Belum ada barang! Silahkan input terlebih dahulu" << endl;
        system("pause");
    }
    else
    {
        int opsi, urut;
        bool ulangInput;
        cout << "Urutkan berdasarkan : " << endl;
        cout << "1. Stok Barang" << endl;
        cout << "2. Nama Barang" << endl;
        cout << "3. Harga Barang" << endl;
        cout << "--------------------" << endl;
        do
        {
            ulangInput = false;
            cout << "Pilihan (1-3): ";
            cin >> opsi;
            if (opsi != 1 && opsi != 2 && opsi != 3)
            {
                ulangInput = true;
                cout << "Pilihan opsi tidak sesuai! Silahkan isi sesuai dengan perintah!" << endl;
            }
        } while (ulangInput);

        system("cls");
        cout << "====================================================================" << endl;
        cout << "|                           DAFTAR BARANG                          |" << endl;
        cout << "====================================================================" << endl;
        cout << "Urutkan dari :" << endl;
        cout << "1. Terkecil" << endl;
        cout << "2. Terbesar" << endl;
        cout << "--------------------" << endl;
        do
        {
            cout << "Pilihan (1-2) : ";
            cin >> urut;
            if (urut != 1 && urut != 2)
            {
                ulangInput = true;
                cout << "Pilihan opsi tidak sesuai! Silahkan isi sesuai dengan perintah!" << endl;
            }
        } while (ulangInput);

        sortBarang(opsi, urut == 1);

        system("cls");
        cout << "====================================================================" << endl;
        cout << "|                           DAFTAR BARANG                          |" << endl;
        cout << "====================================================================" << endl;
        Barang *ptrBarang = barang;
        for (int i = 0; i < jumlahBarang; i++)
        {
            cout << "ID Barang     : " << (ptrBarang + i)->idBarang << endl;
            cout << "Nama Barang   : " << (ptrBarang + i)->namaBarang << endl;
            cout << "Jumlah Barang : " << (ptrBarang + i)->jumlahBarang << endl;
            cout << "Harga Barang  : " << (ptrBarang + i)->hargaBarang << endl
                 << endl;
        }
        system("pause");
    }
}

void editBarang()
{
    system("cls");

    bacaDariFile();
    cout << "====================================================================" << endl;
    cout << "|                            EDIT BARANG                           |" << endl;
    cout << "====================================================================" << endl;

    if (jumlahBarang == 0)
    {
        cout << "Belum ada barang untuk diedit. Silakan input barang terlebih dahulu." << endl;
        system("pause");
        return;
    }

    for (int i = 0; i < jumlahBarang; i++)
    {
        cout << "ID Barang     : " << barang[i].idBarang << endl;
        cout << "Nama Barang   : " << barang[i].namaBarang << endl;
        cout << "Jumlah Barang : " << barang[i].jumlahBarang << endl;
        cout << "Harga Barang  : " << barang[i].hargaBarang << endl
             << endl;
    }
    cout << "----------------------------------------------" << endl;

    do
    {
        int idEdit;
        cout << "Masukkan ID barang yang ingin diedit: ";
        cin >> idEdit;
        int index = cariIndexBarang(idEdit);
        if (index == -1)
        {
            cout << "Barang dengan ID tersebut tidak ditemukan!" << endl;
            cout << "Coba lagi? (y/n): ";
            cin >> konfirmasi;
            if (konfirmasi != "y" && konfirmasi != "Y")
                break;
            continue;
        }

        cout << "Masukkan data baru untuk barang tersebut:" << endl;
        cout << "Jumlah Barang : ";
        cin >> barang[index].jumlahBarang;
        cout << "Harga Barang  : ";
        cin >> barang[index].hargaBarang;

        ofstream file(nama_db);
        if (file.is_open())
        {
            for (int i = 0; i < jumlahBarang; i++)
            {
                file << barang[i].idBarang << "|" << barang[i].namaBarang << "|" << barang[i].jumlahBarang << "|" << barang[i].hargaBarang << "\n";
            }
            file.close();
            cout << "Barang berhasil diperbarui!" << endl;
            system("pause");
            menu();
        }
    } while (konfirmasi == "y" || konfirmasi == "Y");
    system("pause");
    menu();
}

void hapusBarang()
{
    system("cls");

    bacaDariFile();
    cout << "====================================================================" << endl;
    cout << "|                            HAPUS BARANG                          |" << endl;
    cout << "====================================================================" << endl;

    if (jumlahBarang == 0)
    {
        cout << "Belum ada barang untuk dihapus. Silakan input barang terlebih dahulu." << endl;
        system("pause");
        return;
    }

    // Tampilkan daftar barang
    for (int i = 0; i < jumlahBarang; i++)
    {
        cout << "ID Barang     : " << barang[i].idBarang << endl;
        cout << "Nama Barang   : " << barang[i].namaBarang << endl;
        cout << "Jumlah Barang : " << barang[i].jumlahBarang << endl;
        cout << "Harga Barang  : " << barang[i].hargaBarang << endl
             << endl;
    }

    do
    {
        int idHapus;
        cout << "\nMasukkan ID barang yang ingin dihapus: ";
        cin >> idHapus;

        int index = cariIndexBarang(idHapus);
        if (index == -1)
        {
            cout << "Barang dengan ID tersebut tidak ditemukan!" << endl;
            cout << "Coba lagi? (y/n): ";
            cin >> konfirmasi;
            if (konfirmasi != "y" && konfirmasi != "Y")
                break;
            continue;
        }

        cout << "Yakin ingin menghapus barang '" << barang[index].namaBarang << "'? (y/n): ";
        string konfir;
        cin >> konfir;
        if (konfir != "y" && konfir != "Y")
        {
            cout << "Penghapusan dibatalkan." << endl;
            system("pause");
            return;
        }

        for (int i = index; i < jumlahBarang - 1; i++)
        {
            barang[i] = barang[i + 1];
        }
        jumlahBarang--;

        ofstream file(nama_db);
        if (file.is_open())
        {
            for (int i = 0; i < jumlahBarang; i++)
            {
                file << barang[i].idBarang << "|" << barang[i].namaBarang << "|" << barang[i].jumlahBarang << "|" << barang[i].hargaBarang << "\n";
            }
            file.close();
            cout << "Barang berhasil dihapus dan data file diperbarui." << endl;
            system("pause");
        }
    } while (konfirmasi == "y" || konfirmasi == "Y");
}

void penjualan()
{
    system("cls");

    bacaDariFile();

    int idBarangDibeli, jumlahBarangDibeli, stok, index;
    float hargaSatuan, total, totalBayar = 0;
    bool strukBaru = true;

    if (strukBaru)
    {
        ofstream file(fileStruk);
        if (file.is_open())
        {
            file << "===================================================================" << "\n";
            file << "|                         STRUK PEMBELIAN                         |\n";
            file << "===================================================================" << "\n";
            file << left << setw(22) << "Nama Barang" << " "
                 << left << setw(10) << "Jumlah" << " "
                 << left << setw(16) << "Harga" << " "
                 << left << setw(16) << "Total" << "\n";
            file << "-------------------------------------------------------------------" << "\n";
            file.close();
        }
        strukBaru = false;
    }

    if (jumlahBarang == 0)
    {
        cout << "====================================================================" << endl;
        cout << "|                             PENJUALAN                            |" << endl;
        cout << "====================================================================" << endl;
        cout << "Belum ada barang! Silahkan input terlebih dahulu" << endl;
        system("pause");
    }
    else
    {
        cout << "====================================================================" << endl;
        cout << "|                           DAFTAR BARANG                          |" << endl;
        cout << "====================================================================" << endl;
        for (int i = 0; i < jumlahBarang; i++)
        {
            cout << barang[i].idBarang << ". " << barang[i].namaBarang << endl;
        }
        cout << endl;

        cout << "====================================================================" << endl;
        cout << "|                             PENJUALAN                            |" << endl;
        cout << "====================================================================" << endl;
        do
        {
            do
            {
                cout << "ID Barang Dibeli       : ";
                cin >> idBarangDibeli;
                index = cariIndexBarang(idBarangDibeli);
                if (index == -1)
                {
                    cout << "Barang dengan ID tersebut tidak ditemukan!" << endl;
                    cout << "Coba lagi? (y/n): ";
                    cin >> konfirmasi;
                    if (konfirmasi != "y" && konfirmasi != "Y")
                        return;
                    continue;
                }

                stok = barang[index].jumlahBarang;
                if (stok == 0)
                {
                    cout << "Maaf, stok barang habis\n"
                         << endl;
                }
            } while (stok == 0);

            cout << "\nJumlah Barang Dibeli   : ";
            cin >> jumlahBarangDibeli;

            if (jumlahBarangDibeli > stok)
            {
                cout << "\nMohon maaf jumlah barang yang ingin dibeli melebihi stok yang ada. Stok saat ini ialah " << stok << endl;
                cout << "Jumlah Barang Dibeli   : ";
                cin >> jumlahBarangDibeli;
            }
            // searching -- harga barang
            hargaSatuan = cariHarga(idBarangDibeli);
            cout << "Harga Satuan           : Rp." << hargaSatuan << endl;

            // rekursif -- total harga
            total = totalHarga(hargaSatuan, jumlahBarangDibeli);
            totalBayar += total;
            cout << "Harga Total            : Rp." << total << endl;

            cout << "Ingin beli barang lain? (y/n) : ";
            cin >> konfirmasi;
            cout << endl;

            cetakStruk(barang[index].namaBarang, jumlahBarangDibeli, hargaSatuan, total);

            barang[index].jumlahBarang = stok - jumlahBarangDibeli;
        } while (konfirmasi == "y");
        cetakFooterStruk(totalBayar);
        system("pause");

        ofstream file(nama_db);
        if (file.is_open())
        {
            for (int i = 0; i < jumlahBarang; i++)
            {
                file << barang[i].idBarang << "|" << barang[i].namaBarang << "|" << barang[i].jumlahBarang << "|" << barang[i].hargaBarang << "\n";
            }
            file.close();
        }
        else
        {
            cout << "Gagal menyimpan perubahan stok ke file!" << endl;
        }
    }
}

int cariIndexBarang(int idBarang)
{
    for (int i = 0; i < jumlahBarang; i++)
    {
        if (barang[i].idBarang == idBarang)
        {
            return i;
        }
    }
    return -1;
}

void cetakStruk(string namabarang, int jumlah, float hargasatuan, float total)
{
    ofstream file(fileStruk, ios::app);
    if (file.is_open())
    {
        file << left << setw(22) << namabarang << " "
             << left << setw(10) << jumlah << " "
             << left << setw(4) << "Rp. " << setw(12) << hargasatuan << " "
             << left << setw(4) << "Rp. " << setw(12) << total << "\n";
        file.close();
    }
    else
    {
        cout << "Gagal menyimpan ke file!" << endl;
    }
}
void cetakFooterStruk(float totalbayar)
{
    ofstream file(fileStruk, ios::app);
    if (file.is_open())
    {
        file << "===================================================================" << "\n";
        file << "TOTAL BAYAR         : Rp." << fixed << setprecision(0) << totalbayar << "\n";
        file << "===================================================================" << "\n";
        file << "                   Terima kasih telah berbelanja!                 " << "\n";
        file << "===================================================================" << "\n";
        file.close();
    }
    else
    {
        cout << "Gagal menyimpan ke file!" << endl;
    }
}

void menu()
{
    system("cls");
    int menu;
    do
    {
        system("cls");
        cout << "====================================================================" << endl;
        cout << "|                             MENU UTAMA                           |" << endl;
        cout << "====================================================================" << endl;
        cout << "|  1. Input Barang                                                 |" << endl;
        cout << "|  2. Tampilkan Barang                                             |" << endl;
        cout << "|  3. Penjualan                                                    |" << endl;
        cout << "|  4. Edit Barang                                                  |" << endl;
        cout << "|  5. Hapus Barang                                                 |" << endl;
        cout << "|  6. Exit                                                         |" << endl;
        cout << "====================================================================" << endl;
        cout << "Menu Yang dipilih : ";
        cin >> menu;
        switch (menu)
        {
        case 1:
            inputBarang();
            break;

        case 2:
            daftarBarang();
            break;

        case 3:
            penjualan();
            break;
        case 4:
            editBarang();
            break;
        case 5:
            hapusBarang();
            break;
        case 6:
            system("cls");
            cout << "Terima kasih telah menggunakan program ini!" << endl;
            exit(0);
            break;
        }
    } while (menu != 4);
}