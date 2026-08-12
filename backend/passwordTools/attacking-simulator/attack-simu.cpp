#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

class PasswordAnalyzer {
private:
    string password;

    bool hasLowercase = false;
    bool hasUppercase = false;
    bool hasDigit = false;
    bool hasSymbol = false;

    int characterPool = 0;

public:

    PasswordAnalyzer(string pass) {
        password = pass;
        analyzePassword();
    }

    void analyzePassword() {

        for (char ch : password) {

            if (ch >= 'a' && ch <= 'z')
                hasLowercase = true;

            else if (ch >= 'A' && ch <= 'Z')
                hasUppercase = true;

            else if (ch >= '0' && ch <= '9')
                hasDigit = true;

            else
                hasSymbol = true;
        }

        if (hasLowercase)
            characterPool += 26;

        if (hasUppercase)
            characterPool += 26;

        if (hasDigit)
            characterPool += 10;

        if (hasSymbol)
            characterPool += 10;
    }


    void displayInformation() {

        cout << "\n========== PASSWORD ANALYSIS ==========\n";

        cout << "Password Length : "
             << password.length() << endl;

        cout << "Lowercase       : "
             << (hasLowercase ? "Yes" : "No") << endl;

        cout << "Uppercase       : "
             << (hasUppercase ? "Yes" : "No") << endl;

        cout << "Numbers         : "
             << (hasDigit ? "Yes" : "No") << endl;

        cout << "Symbols         : "
             << (hasSymbol ? "Yes" : "No") << endl;

        cout << "Character Pool  : "
             << characterPool << endl;
    }


    long double calculateLogCombinations() {

        int length = password.length();

        return length * log10((long double)characterPool);
    }

    void calculateAttackTime(long double attemptsPerSecond) {

        long double logCombinations =
            calculateLogCombinations();

        long double logSeconds =
            logCombinations - log10(attemptsPerSecond);

        cout << "\n========== ATTACK SIMULATION ==========\n";

        cout << fixed << setprecision(2);

        cout << "Attack Speed : "
             << attemptsPerSecond
             << " attempts/second\n";


        if (logSeconds < 6) {

            long double combinations =
                pow(10, logCombinations);

            long double seconds =
                combinations / attemptsPerSecond;

            cout << "Possible Combinations : "
                 << combinations << endl;

            cout << "Estimated Time : "
                 << seconds << " seconds\n";
        }

        else {

            cout << "Possible Combinations : 10^"
                 << logCombinations << endl;

            cout << "Estimated Time : 10^"
                 << logSeconds
                 << " seconds\n";

            cout << "(Extremely large search space)\n";
        }
    }

    void showStrength() {

        int length = password.length();
        int types = 0;

        if (hasLowercase) types++;
        if (hasUppercase) types++;
        if (hasDigit) types++;
        if (hasSymbol) types++;

        cout << "\n========== PASSWORD STRENGTH ==========\n";

        if (length < 6) {

            cout << "Strength : WEAK\n";
        }

        else if (length < 8 || types < 3) {

            cout << "Strength : MODERATE\n";
        }

        else if (length >= 12 && types == 4) {

            cout << "Strength : VERY STRONG\n";
        }

        else {

            cout << "Strength : STRONG\n";
        }
    }
};


int main() {

    string password;

    long double attackSpeed;

    cout << "============================================\n";
    cout << "       PASSWORD ATTACK SIMULATOR\n";
    cout << "============================================\n";

    cout << "\nEnter a test password: ";
    cin >> password;

    cout << "\nEnter hypothetical attack speed";
    cout << "\n(attempts per second): ";

    cin >> attackSpeed;

    if (attackSpeed <= 0) {

        cout << "\nInvalid attack speed.\n";
        return 0;
    }

    PasswordAnalyzer analyzer(password);

    analyzer.displayInformation();

    analyzer.calculateAttackTime(attackSpeed);

    analyzer.showStrength();

    return 0;
}