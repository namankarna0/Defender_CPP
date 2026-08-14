#include <iostream>

#include "password.h"

using namespace std;

int main()
{
    cout << "Password Strength Checker\n";

    PasswordDictionary dictionary("passDictionary.txt");

    StrengthAnalyzer analyzer(dictionary);

    string value;
    cout << "Enter a password: ";
    getline(cin, value);
    analyzer.analyze(Password(value));
    cout << "Strength: " << analyzer.getStrength() << '\n';
    cout << "Common password: " << (analyzer.isCommonPassword() ? "Yes" : "No") << '\n';

    return 0;
}
