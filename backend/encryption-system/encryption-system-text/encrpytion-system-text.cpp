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
    string message, password;
    cout << "Enter the message:" << endl;
    getline(cin, message);
    cout << "Enter the password:" << endl;
    getline(cin, password);

    XorCipher cipher(password);

    string encrypted = cipher.encrypt(message);

    cout << "Encrypted Text: "<< base64_encode((unsigned char const*)encrypted.c_str(), encrypted.length())<< endl;

    return 0;
}