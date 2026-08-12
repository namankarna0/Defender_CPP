#include<iostream>
#include<string>
#include "base64.h"
using namespace std;

class XorCipher {
private:
    string key;

public:
    XorCipher(string password) {
        key = password;
    }

    string encrypt(string message) {
        string result = message;
        for (int i = 0; i < message.length(); i++) {
            result[i] = message[i] ^ key[i % key.length()];
        }
        return result;
    }

    string decrypt(string cipherText) {
        return encrypt(cipherText);
    }
};

int main(){
    string encrypted, password;
    cout << "Enter the message:";
    getline(cin, encrypted);
    cout << "Enter the password:";
    getline(cin, password);

    XorCipher cipher(password);
    string decrypted = cipher.decrypt(base64_decode(encrypted));

    cout << "Decrypted text: " << decrypted;
    return 0;
}