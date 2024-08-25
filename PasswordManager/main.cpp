#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <cstdlib>
#include <iomanip>
#include <chrono>

struct Password {
    std::string name;
    std::string password;
    std::string category;
    std::string website;
    std::string login;
};

std::vector<Password> passwords;
std::string sourceFile = "passwords.txt";


void savePasswords() {
    std::ofstream outputFile(sourceFile);
    if (outputFile.is_open()) {
        for (const auto& password : passwords) {
            outputFile << password.name << " " << password.password << " " << password.category << " " << password.website << " " << password.login << std::endl;
        }
        outputFile.close();
    }
    else {
        std::cout << "Failed to open the file." << std::endl;
    }
}

void loadPasswords() {
    std::ifstream inputFile(sourceFile);
    if (inputFile.is_open()) {
        Password password;
        while (inputFile >> password.name >> password.password >> password.category >> password.website >> password.login) {
            passwords.push_back(password);
        }
        inputFile.close();
    }
    else {
        std::cout << "Failed to open the file." << std::endl;
    }
}


void displayPasswords(const std::vector<Password>& passwordList) {
    std::cout << "=== Passwords ===" << std::endl;
    for (const auto& password : passwordList) {
        std::cout << "Name: " << password.name << std::endl;
        std::cout << "Password: " << password.password << std::endl;
        std::cout << "Category: " << password.category << std::endl;
        std::cout << "Website: " << password.website << std::endl;
        std::cout << "Login: " << password.login << std::endl;
        std::cout << "-----------------------" << std::endl;
    }
}

void searchPasswords(const std::string& parameter) {
    std::vector<Password> searchResults;

    for (const auto& password : passwords) {
        if (password.name.find(parameter) != std::string::npos ||
            password.category.find(parameter) != std::string::npos ||
            password.website.find(parameter) != std::string::npos ||
            password.login.find(parameter) != std::string::npos) {
            searchResults.push_back(password);
        }
    }

    displayPasswords(searchResults);
}

bool comparePasswordsByName(const Password& a, const Password& b) {
    return a.name < b.name;
}

bool comparePasswordsByCategory(const Password& a, const Password& b) {
    return a.category < b.category;
}

void sortPasswords(const std::string& parameter) {
    std::vector<Password> sortedPasswords = passwords;

    if (parameter == "name") {
        std::sort(sortedPasswords.begin(), sortedPasswords.end(), comparePasswordsByName);
    }
    else if (parameter == "category") {
        std::sort(sortedPasswords.begin(), sortedPasswords.end(), comparePasswordsByCategory);
    }

    displayPasswords(sortedPasswords);
}
std::string generateRandomPassword(int length) {
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*";
    std::string password;

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < length; i++) {
        int randomIndex = rand() % characters.length();
        password += characters[randomIndex];
    }

    return password;
}
bool isPasswordStrong(const std::string& password) {

    if (password.length() < 8) {
        return false; // Password must be at least 8 character
    }
    bool hasUppercase = false;
    bool hasLowercase = false;
    bool hasDigit = false;
    bool hasSpecialChar = false;
    for (char ch : password) {
        if (std::isupper(ch)) {
            hasUppercase = true;
        }
        else if (std::islower(ch)) {
            hasLowercase = true;
        }
        else if (std::isdigit(ch)) {
            hasDigit = true;
        }
        else if (std::ispunct(ch) || std::isspace(ch)) {
            hasSpecialChar = true;
        }
    }
    return hasUppercase && hasLowercase && hasDigit && hasSpecialChar;
}

void addPassword() {
    Password newPassword;

    std::cout << "Enter the name of password: ";
    std::cin >> newPassword.name;

    std::cout << "Do you want to generate a secure password? (Y/N): ";
    std::string generateOption;
    std::cin >> generateOption;
    if (generateOption == "Y" || generateOption == "y") {

        std::string generatedPassword = generateRandomPassword(15);
        newPassword.password = generatedPassword;

        std::cout << "Generated Password: ";
        for (size_t i = 0; i < generatedPassword.length(); ++i) {
            std::cout << '*';
        }
        std::cout << std::endl;
    }
    else {

        std::cout << "Enter the password (min 8 digit): ";
        std::string password;
        char ch;
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {  // Backspace
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b";
                }
            }
            else if (ch == 127) {  // Delete
                password.clear();
                std::cout << "\r";
                std::cout << std::string(newPassword.password.size(), ' ');
                std::cout << "\r";
            }
            else {
                password += ch;
                std::cout << '*';
            }
        }
        std::cout << std::endl;
        newPassword.password = password;
    }
    bool isStrong = isPasswordStrong(newPassword.password);
    if (isStrong) {
        std::cout << "Password is strong." << std::endl;
    }
    else {
        std::cout << "Password is weak." << std::endl;
    }

    std::cout << "Enter the category: ";
    std::cin >> newPassword.category;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter the website: ";
    std::getline(std::cin, newPassword.website);

    std::cout << "Enter the login: ";
    std::getline(std::cin, newPassword.login);



    passwords.push_back(newPassword);
    savePasswords();
    std::cout << "Password added successfully." << std::endl;
}


void editPassword() {
    std::string passwordName;
    std::cout << "Enter the name of the password to edit: ";
    std::cin >> passwordName;

    bool found = false;
    for (auto& password : passwords) {
        if (password.name == passwordName) {
            std::cout << "Enter the new name: ";
            std::cin >> password.name;

            std::cout << "Do you want to generate a secure password? (Y/N): ";
            std::string generateOption;
            std::cin >> generateOption;
            std::string newPassword;

            if (generateOption == "Y" || generateOption == "y") {
                std::string generatedPassword = generateRandomPassword(15);
                newPassword = generatedPassword;

                std::cout << "Generated Password: ";
                for (size_t i = 0; i < generatedPassword.length(); ++i) {
                    std::cout << '*';
                }
                std::cout << std::endl;
            }
            else {
                std::cout << "Enter the new password: ";
                char ch;
                while ((ch = _getch()) != '\r') {
                    if (ch == '\b') {
                        if (!newPassword.empty()) {
                            newPassword.pop_back();
                            std::cout << "\b \b";
                        }
                    } else {
                        newPassword += ch;
                        std::cout << '*';
                    }
                }
            }
            password.password = newPassword;
            bool isStrong = isPasswordStrong(newPassword);
            if (isStrong) {
                std::cout << "Password is strong." << std::endl;
            }
            else {
                std::cout << "Password is weak." << std::endl;
            }
            std::cout << "Enter the new category: ";
            std::cin >> password.category;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Enter the website: ";
            std::getline(std::cin, password.website);

            std::cout << "Enter the login: ";
            std::getline(std::cin, password.login);

            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Password not found." << std::endl;
    }
    else {
        std::cout << "Password edited successfully." << std::endl;
    }
}

void deletePassword() {
    std::string passwordName;
    std::cout << "Enter the name of the password to delete: ";
    std::cin >> passwordName;

    auto it = std::remove_if(passwords.begin(), passwords.end(), [&](const Password& password) {
        return password.name == passwordName;
        });

    if (it != passwords.end()) {
        passwords.erase(it, passwords.end());
        savePasswords();
        std::cout << "Password deleted successfully." << std::endl;
    }
    else {
        std::cout << "Password not found." << std::endl;
    }
}

void addCategory() {
    std::string newCategory;
    std::cout << "Enter the name of the new category: ";
    std::cin >> newCategory;


    savePasswords();
    std::cout << "Category added successfully." << std::endl;
}
void deleteCategory() {
    std::string categoryToDelete;
    std::cout << "Enter the name of the category to delete: ";
    std::cin >> categoryToDelete;



    savePasswords();
    std::cout << "Category deleted successfully." << std::endl;
}


void showMenu() {
    std::cout << "=== Password Manager ===" << std::endl;
    std::cout << "1. Search passwords" << std::endl;
    std::cout << "2. Sort passwords" << std::endl;
    std::cout << "3. Add password" << std::endl;
    std::cout << "4. Edit password" << std::endl;
    std::cout << "5. Delete password" << std::endl;
    std::cout << "6. Add category" << std::endl;
    std::cout << "7. Delete category" << std::endl;
    std::cout << "0. Exit" << std::endl;
}


std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

std::string encryptData(const std::string& data, const std::string& key) {
    std::string encryptedData = data;
    for (size_t i = 0; i < data.length(); ++i) {
        encryptedData[i] = data[i] ^ key[i % key.length()];
    }
    return encryptedData;
}

std::string decryptData(const std::string& encryptedData, const std::string& key) {
    std::string decryptedData = encryptedData;
    for (size_t i = 0; i < encryptedData.length(); ++i) {
        decryptedData[i] = encryptedData[i] ^ key[i % key.length()];
    }
    return decryptedData;
}

void encryptDataFile(const std::string& inputFile, const std::string& outputFile, const std::string& key) {
    std::ifstream fin(inputFile);
    std::ofstream fout(outputFile);

    if (!fin || !fout) {
        std::cout << "Failed to open file." << std::endl;
        return;
    }

    std::string line;
    while (fin >> line) {
        std::string timestamp = getCurrentTimestamp();
        std::string encryptedLine = encryptData(timestamp + " " + line, key);
        fout << encryptedLine << std::endl;
    }

    fin.close();
    fout.close();

    std::cout << "The data has been successfully encrypted." << std::endl;
}

void decryptDataFile(const std::string& inputFile, const std::string& outputFile, const std::string& key) {
    std::ifstream fin(inputFile);
    std::ofstream fout(outputFile);

    if (!fin || !fout) {
        std::cout << "Failed to open file." << std::endl;
        return;
    }

    std::string line;
    while (fin >> line) {
        std::string decryptedLine = decryptData(line, key);
        size_t spacePos = decryptedLine.find(' ');
        if (spacePos != std::string::npos) {
            std::string timestamp = decryptedLine.substr(0, spacePos);
            std::string data = decryptedLine.substr(spacePos + 1);
            fout << data << std::endl;
        }
    }

    fin.close();
    fout.close();

    std::cout << "The data has been successfully decrypted." << std::endl;
}





int main() {


    std::string key = "my_key";
    encryptDataFile(sourceFile, "encrypted.txt", key);
    decryptDataFile("encrypted.txt", "decrypted.txt", key);




    loadPasswords();
    int choice;
    do {
        showMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string parameter;
            std::cout << "Enter the search parameter: ";
            std::cin >> parameter;
            searchPasswords(parameter);
            break;
        }
        case 2: {
            std::string parameter;
            std::cout << "Enter the sort parameter (write name/category): ";
            std::cin >> parameter;
            sortPasswords(parameter);
            break;
        }
        case 3:
            addPassword();
            break;
        case 4:
            editPassword();
            break;
        case 5:
            deletePassword();
            break;
        case 6:
            addCategory();
            break;
        case 7:
            deleteCategory();
            break;
        case 0:
            std::cout << "Exiting the program." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please enter a valid menu option." << std::endl;
            break;
        }
    } while (choice != 0);

    return 0;
}
