#include <iostream>
#include <stdio.h>
#include <fstream>
//#include <iterator>
#include <sstream>
//#include <cctype>
//#include <string>
#include <cstring>
#include <map>
#include <ctime>
#include <cmath>
//#include <sstream>
#include <gmpxx.h> // duze liczby

double t;// czas wykonywania algorytmu 
using namespace std;

typedef mpz_class bi;
void encrypt(bi&,const bi&,const bi&,const bi&);
bi string2bi(string, unsigned int);
string getErrorMessage(int);
string ReadFile(string);    //read whole file
string ReadFileSW(string);  //single word
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
    string phrase, line, filename,key="\0",keypub="\0";
    unsigned int kluczpub_size, kluczpriv_size, msg_size;

    bi bikluczpub, bikluczpriv, bimessage, bicrypted, bin;
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

                kluczpub_size = keypub.length();
                kluczpriv_size = key.length();
                msg_size = phrase.length();
                break;
            }
            case '1': {
                //odczyt frazy
                phrase = ReadFile("dane.in");
                cout << "Odczytana fraza: \t" << phrase << endl;
                key = ReadFile("key.dat");
                cout << "Odczytany klucz prwatny: \t" << key << endl << endl;
                keypub = ReadFile("keypub.dat");
                cout << "Odczytany klucz publiczny: \t" << keypub << endl << "koniec"<< endl;
                
                //cout << "inicjalizacja\t:";
                // cout << "zapis klucza do inta\t:";
                bikluczpriv = string2bi(key,kluczpriv_size);
                bikluczpub = string2bi(keypub,kluczpub_size);
                bimessage = string2bi(phrase,msg_size);
                bin = string2bi("5288447750321988791615322464262168318627237463714249754277190362195246329890490766601513683517722278780729696200186866434111",124);
                //cout << "\nKlucz jako mpz_t: \t: " << bikluczpriv <<"\n\n";
                //gmp_printf ("Klucz jako liczba %Zd\n", &bikluczpriv);
                //mpz_t integ;
                //mpz_init_set_str (integ, "2582249878086908589655919172003011874329705792829223512830659356540647622016841194629645353280137831435903171972747493557", 10);
                int prime = mpz_probab_prime_p(bikluczpriv.get_mpz_t(), 100);
                if ( prime == 1 || prime == 2){
                    cout << "\nKlucz jest liczba pierwsza. \n";
                }else{
                    cout << "\nKlucz nie jest liczba pierwsza. \n";
                }
                break;
            }
            case '2': {
                // szyfrowanie
                encrypt(bicrypted,bimessage,bikluczpub,bin);
                cout << "\nZaszyfrowana wiadomosc:\t" <<bicrypted<<endl<<endl;
                //zapis do pliku
                //filename = "out.txt";
                //WriteToFile(filename,crypted);
                break;
            }
            case '3':{
                //deszyfrowanie
                encrypt(bimessage,bicrypted,bikluczpriv,bin);
                cout << "\nOdszyfowana wiadomosc:\t" <<bimessage<<endl<<endl;
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

// Encrypt Function
void encrypt(	  mpz_class& crypted, // Encrypted Message
			const mpz_class& message, // Message to be encrypted
			const mpz_class& key, // Keys
			const mpz_class& bin)
{
	//encrypt: c = (m ^ e) mod n ||  decrypt: m = (c ^ d) mod n
	mpz_powm(	crypted.get_mpz_t(), 	// Resulting Encrypted Message
				message.get_mpz_t(), 	// Message to be Encrypted
				key.get_mpz_t(), 	// Key Pair (n, e) private
				bin.get_mpz_t());	// Key Pair (n, e) shared
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

/*
bool IsPrime(const bi &number, unsigned int size) {
    if(number == 2)
        return true;
    if(bi(number & 1) == 0 || number < 2)
        return false;
}
*/

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
    if (file.is_open()){
        while (*getline (file,line);// ){
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
        //char * buffer = new char [length];
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
