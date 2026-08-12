#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
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
        for (int i = 0; i < result.length(); i++) {
            result[i] = message[i] ^ key[i % key.length()];
        }
        return result;
    }

    string decrypt(string cipherText) {
        return encrypt(cipherText);
    }
};

class FileHandler {
public:
    static string readFile(string filename, bool &success) {
        ifstream inFILE(filename, ios::binary);
        if (!inFILE.is_open()) {
            success = false;
            return "";
        }
        ostringstream oss;
        oss << inFILE.rdbuf();
        inFILE.close();
        success = true;
        return oss.str();
    }

    static bool writeFile(string filename, string content) {
        ofstream outFILE(filename, ios::binary);
        if (!outFILE.is_open()) {
            return false;
        }
        outFILE << content;
        outFILE.close();
        return true;
    }
};

int main(){
    string filename, password;
    cout << "Input filename:";
    getline(cin, filename);
    cout << "Enter password:";
    getline(cin, password);
    string outputFilename = filename.substr(0, filename.length() - 4);

    bool readSuccess;
    string fileContent = FileHandler::readFile(filename, readSuccess);
    if (!readSuccess) {
        cout << "ERROR: Couldn't open the file";
        return 1;
    }

    XorCipher cipher(password);
    string decryptedContent = cipher.decrypt(fileContent);

    if (!FileHandler::writeFile(outputFilename, decryptedContent)) {
        cout << "ERROR: Couldn't open the file";
        return 1;
    }

    cout << "File decrypted successfully! Saved as " << outputFilename << endl;
    return 0;
}