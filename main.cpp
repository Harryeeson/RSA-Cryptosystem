#include <iostream>
#include <math.h>
#include <fstream>

using namespace std;

int GCD(int e, int phi) {                                       // calculates greatest common factor
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

int modInv(int e, int phi) {                                    // calculates modulo inverse
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

int totient(int n) {                                            // finds the totient of n
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

int modPow(int x, unsigned int y, int p) {                      // calculates modulo of a number with an exponent
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

void encrypt(int e, int n, string encryptWord) {                                                    //encrypt function
    int letterNum;
    int eNum;
    ofstream encryptfile("encrypt.txt");
    for(int i = 0; i < encryptWord.size() - 1; i++) {
        letterNum = encryptWord[i];
        if(letterNum == 32) {                                       // if space turn into 28
            letterNum = 28;
        }
        else {
         letterNum = letterNum - 63;                                // ascii - 63 so A starts at 2
        }
        eNum = modPow(letterNum, e, n);                             
        encryptfile << eNum << " ";                                 // write into encrypted file
    }
    letterNum = encryptWord[encryptWord.size() - 1];                // for last letter
    letterNum = letterNum - 63;
    eNum = modPow(letterNum, e, n);
    encryptfile << eNum;
    encryptfile.close();
}

void decrypt(int d, int n) {                                                    // decrypt function
    ifstream decryptfile;
    ofstream decryptedFile("decrypt.txt");
    int enWord;
    int enWord2;
    char convWord;
    decryptfile.open("encrypt.txt");
    if(decryptfile.is_open()) {
        while(decryptfile >> enWord) {
            enWord = modPow(enWord, d, n); 
            if(enWord == 28) {                                      // encrypted space
                convWord = 32;
            }
            else {
            convWord = enWord + 63;                                 // add 63 back to get originall ascii of letters
            }
            decryptedFile << convWord;                              // write into decrypted file
        }
    }
    decryptfile.close();
    decryptedFile.close();
}

int main() {
    int e;
    int d;
    int n;
    int wordLen;
    string choice;
    string encryptWord;
    char wordCont;

    while(1) {
        cout << "Enter a value for n \n";
        cin >> n;
        int phi = totient(n);
        cout << "Enter a value for e.\n"; 
        cin >> e;  
        if(GCD(e, phi) == 1 && e < phi) {                           //check if key is valid
            d = modInv(e, phi);
            while(1) {
                cout << "Do you want to encrypt or decrypt? (Enter e for encrypt or d for decrypt)\n";
                cin >> choice;
                if(choice == "e") {
                    cout << "Enter word for encryption\n";
                    cin.ignore();
                    getline(cin, encryptWord);
                    wordLen = encryptWord.size();
                    encrypt(e, n , encryptWord);
                    cout << "Encryption Successful.\n";
                    cout << "Do you want to continue to decrypt? (Enter y or n)\n";
                    cin >> wordCont;
                    if(wordCont == 'n') {
                        cout << "Terminating program.\n";
                        exit(0);
                    }
                    else if(wordCont == 'y') {
                        decrypt(d, n);
                        cout << "Decryption Successful. Now terminating program.\n";
                        exit(0);
                    }
                }
                else if(choice == "d") {
                    decrypt(d, n);
                    cout << "Decryption Successful. Now terminating program.\n";
                    exit(0);
                }
                else {
                    cout << "Invalid choice, please try again.\n"; 
                }
            }
        }
        else {
            cout << "Invalid key. Would you like to try again? (Enter y for yes or n for no)\n";
            cin >> choice;
            if(choice != "y") {                                     // anything other than y would terminate the program
                break;
            }
        }
    }
    return 0;
}
