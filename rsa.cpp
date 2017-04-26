#include <iostream>
#include <stdio.h>
#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>
#include <cctype>
#include <string>
#include <cstring>
#include <map>
#include <ctime>
#include <cmath>
#include <sstream>
#include <gmpxx.h> // duze liczby

double t;// czas wykonywania algorytmu 
using namespace std;
typedef mpz_class bi;

bool isPrime(int);
bi string2bi(string, unsigned int);
string getErrorMessage(int);
string ReadFile(string);    //read whole file
string ReadFileSW(string);  //single word
string RSAencode(string,string);
bool WriteToFile(string, string);
bool WriteToBinFile(string, string);
void intime(){ 
  t=clock();
}
void showtime(){ 
  t=clock()-t; 
  cout <<"\nWszystkie operacje zajely: " << ((double)t)/CLOCKS_PER_SEC << " s.\n\n";
}


int main(){
    char ccase=0;
    unsigned char bajt;
    string phrase, line, filename, crypted, decrypted, key="\0",keypub="\0";
    int **Tab = new int*[95];
    for(int i = 0; i < 95; ++i) {
        Tab[i] = new int[95];
    }
    
    do{
        cout << " \nMenu:\n";
        cout << " 0. Wpisywanie do pliku binarnego\n";
        cout << " 1. Odczyt plikow binarnych\n";
        cout << " 2. Szyfrowanie pliku tekstowego\n";
        cout << " 3. Deszyfrowanie pliku\n";
        cout << " x - wyjscie\n--- \n";
        cin >> ccase;
        switch (ccase){
            case '0': {
                cout << "Wprowadz zdania do zapisania: \n"; 
                //cin >> phrase;
                phrase = "Ala ma kota, kot ma Ale.";
                WriteToBinFile("dane.in",phrase);

                cout << "Wprowadz klucz prywatny do zapisania: \n";
                //cin >> key;
                key = "mmHmfSjZ40TB77o9ff8AQTPabxdJkcyI4TcUhH348Cq6R/dlbWpA8+8Icq5YJWkB8IMKXcWflFno2OZ80NroPAMmlsn+fo1S+R+U1cC/FvQxUYrruGIOl/pKAOIKP4HtvGMnGttPUEjCAy3FmzekAUVgOnciJm/JfWg7kQkrzje0NOkv+GyqQbENRSJAW4QJ/z2YOEtxl4W0SeJBwsbi/36e07/uVQquOtcAkuUx7v1IX0xD1HpgEonOnlgn+FHdO4Bek6AuNasAwI/LoSTasUOxWgJU/iqaribwFyQ9i/A/23r2Ai08m7Go0m13P/cKRrNBY0WOR57dEaBpi6CQXHJjzr4SZdkberQn+cDHmj9Z36vWI2XnKlPyBD1bCn5VCW83ZCf35+c10pO5/cQpX+mnPCBKoYmERoE9VaBPcn79dX8YStoRBl7cve1pUefrl/HvLSKwdIXFTxT3ek+9IDdN+0hwqyo9jWZux8XG+MbWTUsBPFNyC8B5cor2bLykssTYwEFuv2h31bBuxJN3Izooyvd2Qq/93MzhW4MWci9DC8qTkYT6Z1/nJCa3TJfbpVRtp8kotAqZDTUS6iBBXFUGKgV3fVqzmOSMBPVhJQewYD8pao1M5c7eINTOdVYkE3/FLFTpbfcU6yipalaUE3Jthfix1DSDm8QhkQJzf/crXEJJLsAVMj+9tJrh6jXBgXsQARQ3F0u3NYFlICE8bAvqINfQRx478XBdqbYbsdrjAx4FUvmDZct5bo8IEexlkWlrRzGLTaEEdowzIo93CiG7aQJGZWY33Bck5Eb1ghiN67koEDKrpnvGhgsxA/qA";
                WriteToBinFile("key.dat",key);

                cout <<  "Wprowadz klucz publiczny do zapisania: \n";
                //cin >> key;
                keypub = "AAAAB3NzaC1yc2EAAAADAQABAAAAgQDS0ckeE+A2AMS9hKi7lBaKIuJgGfXyOF3l9XM83vVyhtXoNz5jyfJFihfgZdssD4V6rOGYlJITBOnoRYlHovHi2q2kqVZGvbo7iN2PSM9VB5RauiKWa9YBJ77wiYpYrLMcFZ4BLl0Ea6vjpcSwFDb6US9qbpN64cmRTADHlrU3JQ==";
                WriteToBinFile("keypub.dat",keypub);
                break;
            }
            case '1': {
                /*int k=0, sum =0, modulo=0;
                for (int i = 0; i<95;i++){
                    for (int j = 0; j<95;j++){
                        sum = 32 + j + k;
                        modulo = sum%127;
                        if (modulo < 33) {
                            Tab[i][j] = modulo + 32;
                        }else{
                            Tab[i][j] = modulo;
                        }
                        cout<< char(Tab[i][j]) << " ";
                    }
                    k++;
                    cout << endl;
                }*/

                //odczyt frazy
                phrase = ReadFile("dane.in");
                cout << "Odczytana fraza: \t" << phrase << endl;
                key = ReadFile("key.dat");
                cout << "Odczytany klucz prwatny: \t" << key << endl << endl;
                keypub = ReadFile("keypub.dat");
                cout << "Odczytany klucz publiczny: \t" << keypub << endl << "koniec"<< endl;
                
                
                
                //cout << "inicjalizacja\t:";
                bi z;
                // cout << "zapis klucza do inta\t:";
                z = string2bi(key,204);
                cout << "\nKlucz jako mpz_t: \t: " << z <<"\n\n";
                //gmp_printf ("Klucz jako liczba %Zd\n", &z);
                break;
            }
            case '2': {
                //odczyt frazy
                //filename = "in.dat";
                //phrase = ReadFile(filename);
                //odczyt klucza
                filename = "key.dat";
                string key2;
                key = ReadFileSW(filename);
                std::stringstream ss;
                long long number;
                ss >> key;
                ss << number;
                cout << endl << key << endl;
                cout << number << endl;
                key2=to_string(number);
                cout << endl << key2 << endl;
                //zapis do pliku
                //filename = "out.txt";
                //WriteToFile(filename,crypted);
                break;
            }
            case '3':{
                //odczyt frazy
                filename = "out.txt";
                phrase = ReadFile(filename);
                //odczyta klucza
                filename = "key.txt";
                key = ReadFileSW(filename);
                cout << endl << crypted << endl;
                
                //zapis do pliku
                filename = "out2.txt";
                WriteToFile(filename,crypted);
                break;
            }
            case 'x': break;
            default :
                cout << "Niewlasciwy znak" <<endl;
        }
    }while(ccase != 'x');

    // zwalnianie pamieci

    return 0;
}

bi string2bi(string phrase, unsigned int size){
    mpz_t z;
    const void * s = phrase.c_str();
    mpz_init(z);
    mpz_import(z, size, 1, sizeof(phrase[0]), 0, 0,s);
    bi r = bi(z);
    mpz_clear(z);
    return r;
}

bool isPrime(int liczba) {
    bool test = true;
    if (liczba != 2){
        for (int i=2; i<(int)sqrt(liczba) +1; i++){
            if (liczba % i == 0){
                test = false;
                break;
            }
        }
    }
    return test;
}

string ReadFile(string filename){
    int length;
    string phrase;
    ifstream file (filename, ios::ate);
    if (file.is_open()){
        file.seekg(0,file.end);
        length = file.tellg();
        file.seekg (0,file.beg);
        char * buffer = new char [length];
        cout << "Zczytano " << length << " znakow.\n";
        file.read(buffer,length);
        phrase.assign(buffer,length);
        //phrase = buffer;
        //delete[] buffer;
        file.close();
        return phrase;
    }else {
        cerr << "Nie moge otworzyc pliku " << filename << endl; 
        return getErrorMessage(10); 
    }
}

int ReadBin(char* bufor, string filename) {
    int size;
    ifstream file(filename,ios::binary | ios::ate);
    if (file.is_open()){
        size = file.tellg();
        bufor = new char [size];
        file.seekg(0,file.beg);
        file.read(bufor,size);
        file.close();
        cout << "Odczytano " << size << " bytow"<< endl;
    }
    return size;
}

string getErrorMessage(int errorCode){
    static map<int, string> codes;
    static bool initialized = false;
    if (!initialized) {
        codes[0]    = "No error.";
        codes[10]   = "Read error.";
        codes[40]   = "Network or protocol error.";
        initialized = true;
    }
    if (codes.count(errorCode) > 0)
        return codes[errorCode];
    return "Unknown error.";
}

/*vector<char> ReadFileBIN(string filename){
   ifstream file (filename, ios::binary);
    /*if (file.is_open()){
        /*while (*getline (file,line);// ){
            istringstream dane(line);
            dane >> phrase;//}
        file.close();
        return phrase;
    }else {
        return getErrorMessage(10); 
    }
   vector<char> buffer((istreambuf_iterator<char>(file)),(istreambuf_iterator<char>()));
   return buffer;
}*/

string ReadFileSW(string filename){
    string phrase, line;
    ifstream file (filename);
    if (file.is_open()){
        /*while (*/getline (file,line);// ){
            istringstream dane(line);
            dane >> phrase;//}
        file.close();
        return phrase;
    }else {
        return getErrorMessage(10); 
    }
}

bool WriteToFile(string filename, string phrase){
    int length = phrase.length();
    ofstream encodefile (filename);
    if (encodefile.is_open()){
        char * buffer = new char [length];
        strcpy(buffer,phrase.c_str());
        encodefile.write(buffer,length);
        delete[] buffer;
        encodefile.close();
        cout << "Zapisano do pliku " << filename<<endl;
        return true;
    }else {
        cerr << "Nie moge otworzyc pliku " << filename << endl; 
        return false;
    }
}

bool WriteToBinFile(string filename, string phrase){
    int length = phrase.length();
    cout << endl << "Rozmiar: " << length << endl;
    ofstream encodefile (filename, ios::out | ios::binary);
    if (encodefile.is_open()){
        char * buffer = new char [length];
        //strcpy(buffer,phrase.c_str());
        encodefile.write(reinterpret_cast<char *>(&length),sizeof(int));
        encodefile.write(phrase.c_str(),length);
        //delete[] buffer;
        encodefile.flush();
        encodefile.close();
        cout << "Zapisano do pliku " << filename<<endl;
        return true;
    }else {
        cerr << "Nie moge otworzyc pliku " << filename << endl; 
        return false;
    }
}

string RSAencode(string msg,string key){
    string crypted = msg;
    uint msgsize = msg.length();
    cout << "Rozmiar wiadomosci: " << msgsize << endl;
    uint keysize = key.length();
    cout << "Rozmiar klucza: " << keysize << endl;


    return crypted;
}