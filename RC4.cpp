#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

unsigned char* passphrase = (unsigned char*)"your_passphrase", * encrypted_text, * decrypted_text;

void RC4(unsigned char* input, unsigned char* passphrase, unsigned char*& output)
{
    unsigned char* temporary;
    int i, t, j = 0, tmp, tmp2, s[256], k[256];

    for (tmp = 0; tmp < 256; tmp++) {
        s[tmp] = tmp;
        k[tmp] = passphrase[(tmp % strlen((char*)passphrase))];
    }
    for (i = 0; i < 256; i++) {
        j = (j + s[i] + k[i]) % 256;
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }

    temporary = new unsigned char[(int)strlen((char*)input) + 1];
    i = j = 0;
    for (tmp = 0; tmp < (int)strlen((char*)input); tmp++) {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;
        tmp2 = s[i];
        s[i] = s[j];
        s[j] = tmp2;
        t = (s[i] + s[j]) % 256;

        if (s[t] == input[tmp]) {
            temporary[tmp] = input[tmp];
        }
        else {
            temporary[tmp] = s[t] ^ input[tmp];
        }
    }

    temporary[tmp] = '\0';
    output = temporary;
}

void textToASCII(const char* text, uint8_t* output)
{
    for (int i = 0; i < strlen(text); i++)
        output[i] = int(text[i]);
}

void writeTextToFile(unsigned char* data, int length)
{
    fstream file;
    file.open("encryptedText.txt", ios::out);
    for (size_t i = 0; i < length; i++)
        file << data[i];

    file.close();
}

void getTextFromFile(char*& data)
{
    fstream file;
    file.open("plainText.txt", ios::in);
    while (!file.eof())
        file.getline(data, 100000);

    file.close();
}

int main()
{
    char* plain_text = new char[100000];
    getTextFromFile(plain_text);

    // convert plainText to ASCII
    int length = strlen(plain_text);
    uint8_t* plain_text_ASCII = new uint8_t[length];
    textToASCII(plain_text, plain_text_ASCII);

    // plainText encryption
    cout << "Your passphrase: " << passphrase << endl;
    cout << "\nPlain text: " << plain_text << endl;
    RC4(plain_text_ASCII, passphrase, encrypted_text);
    cout << "\nEncrypted text: ";
    for (int i = 0; i < length; i++)
        cout << encrypted_text[i];
    cout << endl;
    writeTextToFile(encrypted_text, length);

    // decryption
    RC4(encrypted_text, passphrase, decrypted_text);
    cout << "\nDecrypted text: ";
    for (int i = 0; i < length; i++)
        cout << decrypted_text[i];
    cout << endl;

    delete[] plain_text;
    delete[] plain_text_ASCII;

    cout << "\nProgram runtime: " << (double)clock() / CLOCKS_PER_SEC <<  " seconds." << endl;
    return 0;
}
