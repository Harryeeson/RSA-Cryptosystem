#include <iostream>
#include <math.h>
#include <fstream>

using namespace std;

void encrypt();
void decrypt();
string encryptWord;
int e, d, n;

int GCD(int e, int phi) {
    int mod = e % phi;
    while(1) {
        if(mod == 0) {
            return phi;
        }
        e = phi;
        phi = mod;
        mod = e % phi;
    }
}

int modInv(int e, int phi) {
    bool found = true;
    for(int i = 0; found; i++) {
        if(((i * phi) + 1) % e == 0) {
            int d = ((i * phi) + 1) / e;
            found = false;
            return d;
        }
    }
    return -1;
}

int totient(int n ) {
    int p;
    int q;
    for(int i = 2; i < sqrt(n); i++) {
        if(n % i == 0) {
            p = i;
            q = n / p;
            int phi = (p - 1) * (q - 1);  
            return phi;      
            }
    }
    return -1;
}

int power(int x, unsigned int y, int p) { 
    int res = 1; 
    x = x % p;
    while (y > 0) { 
        if (y & 1) {
            res = (res * x) % p; 
        }
        y = y >> 1; 
        x = (x * x) % p;   
    } 
    return res; 
}

int main() {
    int numCont;
    int wordLen;
    string choice;
    char wordCont;

    cout << "Enter n \n";
    cin >> n;
    int phi = totient(n);
    cout << "Enter value for e.\n"; 
    cin >> e;

    if(GCD(e, phi) == 1 && e < phi) { 
        d = modInv(e, phi);
    }

    else {
        cout << "Invalid key.\n";
        exit(0);
    }
 
    numCont = 1;
    while(numCont == 1) {
        cout << "Do you want to encrypt or decrypt? (Enter e for encrypt or d for decrypt)\n";
        cin >> choice;
        if(choice == "e") {
            cout << "Enter word for encryption\n";
            cin.ignore();
            getline(cin, encryptWord);
            wordLen = encryptWord.size();
            encrypt();
            cout << "Do you want to decrypt? (Enter y or n)\n";
            cin >> wordCont;
            if(wordCont == 'n') {
                numCont = 0;
            }
            else if(wordCont == 'y') {
                decrypt();
                numCont = 0;
            }
        }
        else if(choice == "d") {
            decrypt();
            numCont = 0;
        }
        else {
            cout << "Invalid choice, please try again.\n"; 
        }
    }
    return 0;
}

void encrypt() {
    int letterNum;
    int eNum;
    ofstream encryptfile("encrypt.txt");
    for(int i = 0; i < encryptWord.size() - 1; i++) {
        letterNum = encryptWord[i];
        if(letterNum == 32) {
            letterNum = 28;
        }
        else {
         letterNum = letterNum - 63;
        }
        eNum = power(letterNum, e, n);
        encryptfile << eNum << " ";
    }
    letterNum = encryptWord[encryptWord.size() - 1];
    letterNum = letterNum - 63;
    eNum = power(letterNum, e, n);
    encryptfile << eNum;
    encryptfile.close();
}

void decrypt() {
    ifstream decryptfile;
    ofstream decryptedFile("decrypt.txt");
    int enWord;
    int enWord2;
    char convWord;
    decryptfile.open("encrypt.txt");
    if(decryptfile.is_open()) {
        while(decryptfile >> enWord) {
            enWord = power(enWord, d, n);
            if(enWord == 28) {
                convWord = 32;
            }
            else {
            convWord = enWord + 63;
            }
            decryptedFile << convWord;
        }
    }
    decryptfile.close();
    decryptedFile.close();
}
