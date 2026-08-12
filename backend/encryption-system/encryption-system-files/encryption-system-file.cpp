#include<iostream>
#include<fstream>
#include<string>
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
    cout << "Input file name:";
    getline(cin, filename);
    cout << "Input password:";
    getline(cin, password);
    string outputFilename = filename + ".enc";

    bool readSuccess;
    string fileContent = FileHandler::readFile(filename, readSuccess);
    if (!readSuccess) {
        cout << "ERROR: Could not open input file!" << endl;
        return 1;
    }

    XorCipher cipher(password);
    string encryptedContent = cipher.encrypt(fileContent);

    if (!FileHandler::writeFile(outputFilename, encryptedContent)) {
        cout << "ERROR: Could not open output file!" << endl;
        return 1;
    }

    cout << "File encrypted successfully! Saved as " << outputFilename << endl;
    return 0;
}