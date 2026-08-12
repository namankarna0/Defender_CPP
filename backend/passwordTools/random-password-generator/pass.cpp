#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class PasswordGenerator
{
private:
    int length;
    bool useLower;
    bool useUpper;
    bool useDigits;
    bool useSymbols;
    string password;

public:
    PasswordGenerator(int len, bool lower, bool upper, bool digits, bool symbols)
    {
        length = len;
        useLower = lower;
        useUpper = upper;
        useDigits = digits;
        useSymbols = symbols;
        password = "";
    }
    void generatePassword()
    {
        string characters = "";
        if (useLower)
            characters += "abcdefghijklmnopqrstuvwxyz";
        if (useUpper)
            characters += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        if (useDigits)
            characters += "0123456789";
        if (useSymbols)
            characters += "!@#$%^&*()-_=+[]{}<>?/|~";
        if (characters.empty())
        {
            cout << "No character types selected. Cannot generate password." << endl;
            return;
        }
        password = "";

        for (int i = 0; i < length; i++)
        {
            int index = rand() % characters.length();
            password += characters[index];
        }
    }
    void displayPassword()
    {
        cout << "\nGenerated Password: " << password << endl;
    }
};

int main()
{
    srand(time(0));
    int length;
    char choice;
    bool lower, upper, digits, symbols; 
    cout << "================RANDOM PASSWORD GENERATOR================" << endl;
    cout << "\nEnter the desired length of the password: ";
    cin >> length;
    if (length <= 0)
    {
        cout << "Password length must be a positive integer." << endl;
        return 1;
    }
    cout << "Do you want to include lowercase letters? (y/n): ";
    cin >> choice;
    lower = (choice == 'y' || choice == 'Y');
    cout << "Do you want to include uppercase letters? (y/n): ";
    cin >> choice;
    upper = (choice == 'y' || choice == 'Y');
    cout << "Do you want to include digits? (y/n): ";
    cin >> choice;
    digits = (choice == 'y' || choice == 'Y');
    cout << "Do you want to include symbols? (y/n): ";
    cin >> choice;
    symbols = (choice == 'y' || choice == 'Y');

    PasswordGenerator generator(length, lower, upper, digits, symbols);

    generator.generatePassword();
    generator.displayPassword();
    return 0;
}