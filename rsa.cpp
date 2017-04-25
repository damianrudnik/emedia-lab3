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

double t;// czas wykonywania algorytmu 
using namespace std;
bool isPrime(int);
string getErrorMessage(int);
string ReadFile(string);    //read whole file
string ReadFileSW(string);  //single word
//vector<char> ReadFileBIN(string);  //binary file
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
    string phrase, line, filename, crypted, decrypted, key="\0";
    int **Tab = new int*[95];
    for(int i = 0; i < 95; ++i) {
        Tab[i] = new int[95];
    }
    
    do{
        cout << " \nMenu:\n";
        cout << " 0. Wpisywanie do pliku binarnego\n";
        cout << " 1. Szyfrowanie pliku binarnego\n";
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

                cout << "Wprowadz klucz do zapisania: \n";
                //cin >> phrase;
                phrase = "12345678";
                WriteToBinFile("key.dat",phrase);
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
                filename = "dane.in";
                phrase = ReadFile(filename);
                cout << "\tOdczytana fraza: \n" << phrase << endl;
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
    ifstream file (filename);
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