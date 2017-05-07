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
    while (!v.empty()){
        cout<<bi2char(v.back());
        v.pop_back();
    }
    cout << endl;
}

void menu(){
    cout << " \nMenu:\n";
    cout << " g. Generuj klucze\n";
    cout << " 0. Wpisywanie do pliku binarnego\n";
    cout << " 1. Odczyt plikow binarnych\n";
    cout << " 2. Szyfrowanie pliku tekstowego\n";
    cout << " 3. Deszyfrowanie pliku\n";
    cout << " x - wyjscie\n--- \n";
}

int main(){
    // INITS
    char ccase=0;
    int iKeySize;
    string phrase, line, filename,key="\0",keypub="\0", test;
    unsigned int /*uipublickey_size, uiprivatekey_size,*/ msg_size;
    bi bipublickey, biprivatekey, bimessage, bicrypted, bidecrypted, bin;
    vector<bi> v_bimsg, v_bicrypted; // wektor, w ktorym wiadomosc jest podzielona na bajty, ale takie zlozone z bi
                
    // main loop
    do{
        menu();
        cin >> ccase;
        switch (ccase){
            case 't': {
                cin >> test;
                bin = string2bi(test,test.length());
                cout << "num: "<< bin << ", size: " << test.length() << endl;
                cout<<bi2char(bin);
                WriteToFile("test.txt",bi2string_raw(bin,test.length()));
                vector<uint8_t> v;
                bi2v(bin,test.length(),v);
                wys_w(v);
                v.clear();
                break;
            }
            case 'g': {
                cout << "Size of the keys to generate: ";
                cin >> iKeySize;
                generateKey(iKeySize, bin, bipublickey, biprivatekey);
                cout << "Private key: " << biprivatekey << endl;
                cout << "Pubic key: " << bipublickey << endl;
                cout << "Modulo key: " << bin << endl;
                break;
            }
            case '0': {
                cout << "The message: "; 
                //cin >> noskipws >> phrase;
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
                
                /*int prime = mpz_probab_prime_p(bipublickey.get_mpz_t(), 25);
                cout << "Check if the key is prime: \t" << prime;
                if ( prime == 0){
                    cout << "\tKey is not prime. \n";
                }else if (prime == 1){
                    cout << "\tKey is probably prime. \n";
                }else{
                    cout << "\tKey is prime. \n";
                }*/
                break;
            }
            /*case '2': {
                // szyfrowanie cryptodecrypt(bi base, bi exponent, const bi &modulo)
                bicrypted = cryptodecrypt(bimessage,bipublickey,bin);
                cout << "\nCrypted message:\t";
                vector<uint8_t> c;
                bi2v(bicrypted,msg_size,c);
                wys_w(c);
                c.clear();
                cout << endl;
                //zapis do pliku
                //WriteToFile("out.txt",crypted);
                break;
            }*/
            case '2': {
                // szyfrowanie cryptodecrypt(bi base, bi exponent, const bi &modulo)
                vector<uint8_t> v_8msg; // wektor w ktorym wiadomosc jest podzielona na bajty
                bi2v(bimessage,msg_size,v_8msg); // o tutaj dziele to na mniejsze kawaleczki
                mpz_clear(bimessage.get_mpz_t());// usuniecie wiadomosci w formie liczbowej
                //v_bimsg.resize(msg_size);
                char czmienna;
                while (!v_8msg.empty()){ // tutaj zamieniam kazdy bajt z wektora v_8msg na liczbe do v_bimsg
                    //cout << "while.." << endl;
                    czmienna = char(v_8msg.back());
                    //cout << "po cascie.." << endl;
                    //cout << char2bi(&czmienna) << endl;
                    v_bimsg.emplace_back(char2bi(&czmienna));
                    //cout<<"."<<v_bimsg.back()<<endl;
                    v_8msg.pop_back();
                }
                //cout << "rozmiar wektora: " << int(v_bimsg.size())<<endl;
                while (!v_bimsg.empty()){// i tutaj kazda z liczb wektora v_bimsg szyfruje
                    v_bicrypted.emplace_back(cryptodecrypt(v_bimsg.back(),bipublickey,bin));
                    v_bimsg.pop_back();
                }
                //cout << "rozmiar wektora: " << int(v_bicrypted.size())<<endl;
                // czyszczenie niuzywanych zmiennych
                v_8msg.clear();
                v_bimsg.clear();
                //cout << "wyswietlanie.. " << endl;
                //wys_wbi(v_bicrypted);
                //zapis do pliku
                //WriteToFile("out.txt",crypted);
                cout << "crypt done" << endl;
                break;
            }
            case '3':{
                vector<bi> v_bidecrypted;
                //deszyfrowanie

                while (!v_bicrypted.empty()){// i tutaj kazda z liczb wektora v_bimsg szyfruje
                    v_bidecrypted.push_back(cryptodecrypt(v_bicrypted.back(),biprivatekey,bin));
                    v_bicrypted.pop_back();
                }
                v_bicrypted.clear();
                wys_wbi(v_bidecrypted);

                /*
                bidecrypted = cryptodecrypt(bicrypted,biprivatekey,bin);
                cout << "\nDecrypted message:\t";
                vector<uint8_t> d;
                bi2v(bidecrypted,msg_size,d);
                wys_w(d);
                d.clear();
                cout << endl;*/
                break;
            }
            case 'x': break;
            default :
                cout << "Wrong letter." << endl;
        }
    }while(ccase != 'x');

    return 0;
}

// Encrypt Function
/*void encrypt(	  mpz_class& crypted, // Encrypted Message
			const mpz_class& message, // Message to be encrypted
			const mpz_class& key, // Keys
			const mpz_class& bin)
{
	//encrypt: c = (m ^ e) mod n ||  decrypt: m = (c ^ d) mod n
	mpz_powm(	crypted.get_mpz_t(), 	// Resulting Encrypted Message
				message.get_mpz_t(), 	// Message to be Encrypted
				key.get_mpz_t(), 	// Key Pair (n, e) private
				bin.get_mpz_t());	// Key Pair (n, e) shared
}*/

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
    //const char * cstr = cos.c_str();
    //answer = cstr;
    uint liczba = atoi( str.c_str() );
    //cout << "liczba: " << liczba << endl;
    answer = '\0'+liczba;
    return answer;
}

string bi2string_raw(bi& binumber, unsigned int size) {
    char * answer = new char[size];
    mpz_get_str(answer,10,binumber.get_mpz_t());
    /*//mpz_import(mpz_t rop, size_t count, int order, size_t size, int endian, size_t nails, const void *op)
    //mpz_export(void *rop, size_t *countp, int order, size_t size, int endian, size_t nails, const mpz_t op)
    size_t roman = size_t(size);
    cout << "rozmiar : " << roman << endl;
    size_t *wskroman = &roman;
    mpz_export(answer, wskroman, 1, sizeof(char), 0, 0, binumber.get_mpz_t());*/
    return answer;
}

void bi2v(bi& binum, unsigned int size, vector<uint8_t>& v) {
    v.resize(size);
    size_t roman = size_t(size);
    size_t *wskroman = &roman;
    mpz_export(&v[0], wskroman, 1, sizeof(uint8_t), 0, 0, binum.get_mpz_t());
}


/*int checkfilephraselength(string filename){
    int length;
    ifstream file (filename, ios::in|ios::binary|ios::ate);
    if (file.is_open()){
        length = file.tellg();
        length = length-4;// dodatkowe 4 znaki na poczatku pliku
        file.close();
        return length;
    }else return 0;
}*/

/*string bi2string(bi& binumber, unsigned int size, unsigned int answersize) {
    char * answer = new char[size]; // miejsce na zapis liczby
    mpz_get_str(answer,10,binumber.get_mpz_t());
    
    if (WriteToBinFile("tempbinaryfile.bin",answer)){
        string phrase;
        //int length = checkfilephraselength("tempbinaryfile.bin");
        int i = 0, j = 0;
        unsigned char test[2];

        FILE * pFile;
        fpos_t position;
        pFile = fopen ( "tempbinaryfile.bin" , "rb" );
        if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

        // check file size
        fseek (pFile , 0 , SEEK_END);
        int length = ftell (pFile);
        length = length -4;
        rewind (pFile);
        
        cout << "dlugosc odczytywanego ciagu: " << length << endl;
        char * buffer = new char [length]; // miejsce na odczyt literek z pojedynczych bajtow
        fsetpos (pFile, &position);
        while(fread(test,2,1,pFile) == 1) {
            i++;
            if ( i > 2){
                printf("test first byte 8 bits :%c\n", test[0]);
                printf("test second byte : %c\n",test[1]);
                buffer[j++] = test[0];
                buffer[j++] = test[1];
            }
        }
        fclose (pFile);
        remove("tempbinaryfile.bin");
        phrase.assign(buffer,size); // przypisanie bufora charow do stringa
        return phrase;
    }else{
        return getErrorMessage(1); 
    }
}*/

void generateKey(int size, mpz_class& n, mpz_class& d, mpz_class& e){
	bool prime = false;
	bool divisor = false;
	mpz_class PNUM, result, q, p, phiN;
	mpz_class one = mpz_class(1);

	// Compute p & q
	for(int i = 0; i < 2; ++i){
		while(!prime){
			PNUM = r.get_z_bits(size);
			int isPrime = 1;
			isPrime = mpz_probab_prime_p(PNUM.get_mpz_t(), 100);
			if(isPrime != 0) break;
		}
		if(i == 0) p = PNUM;
		if(i == 1) q = PNUM;
	}
	n = p * q;
	phiN = (p - 1) * (q - 1);

	while(!divisor){
		d = r.get_z_bits(size * 2);
		mpz_gcd(result.get_mpz_t(), d.get_mpz_t(), phiN.get_mpz_t());
		if((d < phiN) && (result == one)) break;
	}

	// Compute e:
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
        //phrase = buffer;
        //delete[] buffer;
        file.close();
        return phrase;
    }else {
        cerr << "Nie moge otworzyc pliku " << filename << endl; 
        return getErrorMessage(10); 
    }
}
/*
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
*/
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
        //char * buffer = new char [length];
        //strcpy(buffer,phrase.c_str());
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
   /* bi result = 1;
    while (exponent > 0){
        if (bi(exponent & 1) == 1)
            result = (base*result)%modulo;
        base = (base * base) % modulo;
        exponent >>= 1;
    }*/
    mpz_powm (result.get_mpz_t(), base.get_mpz_t(), exponent.get_mpz_t(), modulo.get_mpz_t());
    return result;
}