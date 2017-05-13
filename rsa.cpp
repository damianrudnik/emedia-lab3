#include <iostream>
#include <stdio.h>
#include <fstream>
//#include <iterator>
#include <sstream>
//#include <cctype>
//#include <string>
#include <string>
#include <map>
#include <ctime>
#include <cmath>
//#include <sstream>
#include <gmpxx.h> // duze liczby
#include <limits.h>
#include <vector>

double t;// czas wykonywania algorytmu 
static gmp_randclass r (gmp_randinit_default);
using namespace std;

typedef mpz_class bi;
//void encrypt(bi&,const bi&,const bi&,const bi&);
void generateKey(int, mpz_class&, mpz_class&, mpz_class&);
bi cryptodecrypt(bi&, bi&, const bi&);
bi string2bi(string, unsigned int);
bi char2bi(char *);
char bi2char(bi);
string bi2string_raw(bi&, unsigned int);
string bi2string(bi&, unsigned int, unsigned int);
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


void bi2v(bi&, unsigned int, vector<uint8_t>&);
void wys_w(vector<uint8_t>& v){
    for (size_t i=0;i<v.size();i++)
        cout<<v[i];
    cout<<endl;
}

void wys_wbi(vector<bi> v) {
    cout << "Wektor ma rozmiar: " << v.size() << ", a miesci w sobie: >>";
    while (!v.empty()){
        cout<<bi2char(v.back());
        v.pop_back();
    }
    cout << "<< koniec"<< endl;
}

void menu(){
    cout << " \nMenu:\n";
    cout << " g. Generate keys\n";
    cout << " 0. Write phrase to the binary file\n";
    cout << " 1. Read binary files\n";
    cout << " 2. Encrypt binary file\n";
    cout << " 3. Decrypt binary file\n";
    cout << " x - EXIT\n--- \n";
}

int main(){
    // INITS
    char ccase=0;
    int iKeySize;
    string phrase, line, filename,key="\0",keypub="\0", test;
    unsigned int msg_size;
    bi bipublickey, biprivatekey, bimessage, bicrypted, bidecrypted, bin;
    vector<bi> v_bimsg, v_bicrypted, v_bidecrypted; // wektor, w ktorym wiadomosc jest podzielona na bajty, ale takie zlozone z bi
                
    // main loop
    do{
        menu();
        cin >> ccase;
        switch (ccase){
            case 'g': {
                cout << "Size of the keys to generate: ";
                cin >> iKeySize;
                intime();
                    generateKey(iKeySize, bin, bipublickey, biprivatekey);
                showtime();
                break;
            }
            case '0': {
                cout << "The message: "; 
                getline(cin,phrase);
                WriteToBinFile("dane.in",phrase);
                bi testbi = string2bi(phrase,phrase.length());
                cout << "<To check> Message as number before write: \t" << testbi << endl;
                vector<uint8_t> w;
                bi2v(testbi,phrase.length(),w);
                cout << "<To check> Number converted to string: ";
                wys_w(w);
                w.clear();
                cout << endl;
                break;
            }
            case '1': {
                //read phrase from binary file
                phrase = ReadFile("dane.in");
                cout << "Read phrase: \t" << phrase << endl;
                msg_size = phrase.length();
                bimessage = string2bi(phrase,msg_size);
                cout << "The message as number: " << bimessage << endl << endl;
                cout << "Private key: " << biprivatekey << endl << endl;
                cout << "Pubic key: " << bipublickey << endl << endl;
                break;
            }
            case '2': {
                // szyfrowanie cryptodecrypt(bi base, bi exponent, const bi &modulo)
                vector<uint8_t> v_8msg; // wektor w ktorym wiadomosc jest podzielona na bajty
                bi2v(bimessage,msg_size,v_8msg); // o tutaj dziele to na mniejsze kawaleczki
                mpz_clear(bimessage.get_mpz_t());// usuniecie wiadomosci w formie liczbowej
                char czmienna;

                while (!v_8msg.empty()){ // tutaj zamieniam kazdy bajt z wektora v_8msg na liczbe do v_bimsg
                    czmienna = char(v_8msg.back());
                    v_bimsg.emplace_back(char2bi(&czmienna));
                    v_8msg.pop_back();
                }

                bi xored, tempbi;
                bool firstrun = true;

                while (!v_bimsg.empty()){// i tutaj kazda z liczb wektora v_bimsg szyfruje
                    tempbi = cryptodecrypt(v_bimsg.back(),bipublickey,bin);
                    if (!firstrun){
                        mpz_xor(xored.get_mpz_t(),tempbi.get_mpz_t(),v_bicrypted.back().get_mpz_t());
                    }else{ // bo ten znak to znak zakonczenia stringa
                        //xored = tempbi;
                        firstrun = false;
                    }

                    v_bicrypted.emplace_back(xored);
                    v_bimsg.pop_back();
                }
                // czyszczenie niuzywanych zmiennych
                v_8msg.clear();
                v_bimsg.clear();
                //zapis do pliku
                //WriteToFile("out.txt",crypted);
                cout << "crypt done" << endl;

                //cout << "## Zaszyfrowana wiadomosc: ";
                //wys_wbi(v_bicrypted);
                break;
            }
            case '3':{
                //deszyfrowanie
                bi xored2, tempbi2, tempbi3, bisciagniety;

                while (!v_bicrypted.empty()){// sciagam po kolei elementy wektora zaszyfrowanego - najpierw xor z poprzednim, nastepnie deszyfracja
                    tempbi2 = v_bicrypted.back();
                    v_bicrypted.pop_back();
                    if (!v_bicrypted.empty()){
                        bisciagniety = v_bicrypted.back();
                        mpz_xor(xored2.get_mpz_t(),tempbi2.get_mpz_t(),bisciagniety.get_mpz_t());
                    }else{
                        xored2 = tempbi2;
                        break;
                    }
                    tempbi3 = cryptodecrypt(xored2,biprivatekey,bin);
                    v_bidecrypted.emplace_back(tempbi3);
                }
                cout << "## Zdeszyfrowana wiadomosc: ";
                wys_wbi(v_bidecrypted);
                break;
            }
            case 'x': 

                break;
            default :
                cout << "Wrong letter." << endl;
        }
    }while(ccase != 'x');

    return 0;
}

bi string2bi(string phrase, unsigned int size){
    mpz_t z;
    const void * s = phrase.c_str();
    mpz_init(z);
    mpz_import(z, size, 1, sizeof(char), 0, 0,s);
    bi r = bi(z);
    mpz_clear(z);
    return r;
}

bi char2bi(char *phrase){
    mpz_t z;
    const void * s = phrase;
    mpz_init(z);
    mpz_import(z, 1, 1, sizeof(char), 0, 0,s);
    bi r = bi(z);
    mpz_clear(z);
    return r;
}

char bi2char(bi binumber){
    char answer;
    string str = bi2string_raw(binumber,1);
    uint liczba = atoi( str.c_str() );
    answer = '\0'+liczba;
    return answer;
}

string bi2string_raw(bi& binumber, unsigned int size) {
    char * answer = new char[size];
    mpz_get_str(answer,10,binumber.get_mpz_t());
    return answer;
}

void bi2v(bi& binum, unsigned int size, vector<uint8_t>& v) {
    v.resize(size);
    size_t roman = size_t(size);
    size_t *wskroman = &roman;
    mpz_export(&v[0], wskroman, 1, sizeof(uint8_t), 0, 0, binum.get_mpz_t());
}

void generateKey(int size, mpz_class& n, mpz_class& d, mpz_class& e){
	bool prime = false;
	bool divisor = false;
	mpz_class primenum, result, q, p, phiN;
	mpz_class one = mpz_class(1);

	// Calculation of the p & q
	for(int i = 0; i < 2; ++i){
		while(!prime){
			primenum = r.get_z_bits(size);
			int isPrime = 1;
			isPrime = mpz_probab_prime_p(primenum.get_mpz_t(), 100);
			if(isPrime != 0) break;
		}
		if(i == 0) p = primenum;
		if(i == 1) q = primenum;
	}
	n = p * q;
	phiN = (p - 1) * (q - 1);

	while(!divisor){
		d = r.get_z_bits(size * 2);
		mpz_gcd(result.get_mpz_t(), d.get_mpz_t(), phiN.get_mpz_t());
		if((d < phiN) && (result == one)) break;
	}

	// Calculation of the e:
	mpz_invert(e.get_mpz_t(), d.get_mpz_t(), phiN.get_mpz_t());
}

string ReadFile(string filename){
    int length;
    string phrase;
    ifstream file (filename, ios::in|ios::binary|ios::ate);
    if (file.is_open()){
        //file.seekg(0,file.end);
        length = file.tellg();
        length = length - 4; //przesuniecie o 4 znaki: "^C^@^@^@", ktore sa wpisywane na poczatku pliku binarnego
        file.seekg (4,file.beg);
        char * buffer = new char [length];
        cout << "Zczytano " << length << " znakow.\n";
        file.read(buffer,length);
        phrase.assign(buffer,length);
        file.close();
        return phrase;
    }else {
        cerr << "Nie moge otworzyc pliku " << filename << endl; 
        return getErrorMessage(10); 
    }
}

string getErrorMessage(int errorCode){
    static map<int, string> codes;
    static bool initialized = false;
    if (!initialized) {
        codes[0]    = "No error.";
        codes[1]    = "Create error.";
        codes[10]   = "Read error.";
        codes[40]   = "Network or protocol error.";
        initialized = true;
    }
    if (codes.count(errorCode) > 0)
        return codes[errorCode];
    return "Unknown error.";
}

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
    ofstream encodefile (filename, ios::trunc);
    if (encodefile.is_open()){
        char * buffer = new char [length];
        strcpy(buffer,phrase.c_str());
        encodefile.write(buffer,length);
        delete[] buffer;
        encodefile.close();
        cout << "Zapisano do pliku " << filename<<endl<<endl;
        return true;
    }else {
        cerr << "Nie moge otworzyc pliku " << filename << endl; 
        return false;
    }
}


bool WriteToBinFile(string filename, string phrase){
    int length = phrase.length();
    cout << endl << "Rozmiar: " << length << endl;
    ofstream encodefile (filename, ios::out | ios::trunc | ios::binary);
    if (encodefile.is_open()){
        encodefile.write(reinterpret_cast<char *>(&length),sizeof(int));
        encodefile.write(phrase.c_str(),length);
        encodefile.flush();
        encodefile.close();
        cout << "Zapisano do pliku " << filename<<endl;
        return true;
    }else {
        cerr << "Nie moge otworzyc pliku " << filename << endl; 
        return false;
    }
}

bi cryptodecrypt(bi &base, bi &exponent, const bi &modulo){
   bi result;
    mpz_powm (result.get_mpz_t(), base.get_mpz_t(), exponent.get_mpz_t(), modulo.get_mpz_t());
    return result;
}