#include <bits/stdc++.h>
#include "KeyGen.cpp"
#include <sstream>
using namespace std;

int main() {
    int option;
    do{
        cout << "\nWelcome to Crypto World!!!" << '\n';
        cout << "AES-128" << '\n';
        cout << "1. Encrypt" << '\n';
        cout << "2. Decrypt" << '\n';
        cout << "3. Exit" << '\n';
        cout << "Option: ";

        cin >> option;
        cin.ignore();

        if(option == 1) {
            string message;

            cout << "Enter the message to encrypt: ";
            getline(cin, message);

            int message_length = message.length();

            int n = message_length/16;
            int r = message_length%16;

            int num_states = (r>0 ? n+1 : n);

            AES aes(num_states, message, true);

            KeyGen kgen;
            string key;
            cout << "Enter you secret key: ";
            cin >> key;

            while(key.length() != 16) {
                cout << "exactly 16 characters is needed in AES-128";
                cout << "\nEnter you secret key: ";
                cin >> key;
            }

            kgen.getSecretKey(key);

            // encryption
            cout << "\n----------------------" << '\n';
            cout << "Round 0" << '\n';
            cout << "States" << '\n';
            aes.printStates();
            cout << "Round Key 0" << '\n';
            kgen.printSelectedKey(0);
            cout << "AddRoundKey" << '\n';            
            aes.addRoundKey(kgen.getKey(0));
            aes.printStates();
            cout << "----------------------" << '\n';

            for(int i=1; i<=10; i++) {
                cout << "----------------------" << '\n';
                cout << "Round " << dec << i << '\n';
                cout << "States" << '\n';
                aes.printStates();

                cout << "SubBytes" << '\n';
                aes.callSubBytes();
                aes.printStates();

                cout << "ShiftRows" << '\n';
                aes.shiftRows();
                aes.printStates();

                if(i < 10) {
                    cout << "MixColumns" << '\n';
                    aes.callMixCols();
                    aes.printStates();
                }

                cout << "Round Key " << dec << i << '\n';
                kgen.printSelectedKey(i);

                cout << "AddRoundKey" << '\n'; 
                aes.addRoundKey(kgen.getKey(i));
                aes.printStates();
                cout << "----------------------" << '\n';
            }

            cout << "Ciphertext: ";
            aes.printCiphertext();
        }

        else if(option == 2) {
            cout << "Enter the ciphertext: ";
            string message;
            cin >> message;

            int message_length = message.length();

            int n = message_length/32;
            int r = message_length%32;

            int num_states = (r>0 ? n+1 : n);

            AES aes(num_states, message, false);

            KeyGen kgen;
            string key;
            cout << "Enter you secret key: ";
            cin >> key;

            while(key.length() != 16) {
                cout << "exactly 16 characters is needed in AES-128";
                cout << "\nEnter you secret key: ";
                cin >> key;
            }

            kgen.getSecretKey(key);

            // decryption
            cout << "\n----------------------" << '\n';
            cout << "Round 0" << '\n';
            cout << "States" << '\n';
            aes.printStates();
            cout << "Round Key 10" << '\n';
            kgen.printSelectedKey(0);
            cout << "AddRoundKey" << '\n';            
            aes.addRoundKey(kgen.getKey(10));
            aes.printStates();
            cout << "----------------------" << '\n';

            for(int i=9; i>=0; i--) {
                cout << "----------------------" << '\n';
                cout << "Round " << dec << 10-i << '\n';
                cout << "States" << '\n';
                aes.printStates();

                cout << "InvShiftRows" << '\n';
                aes.invShiftRows();
                aes.printStates();

                cout << "InvSubBytes" << '\n';
                aes.callInvSubBytes();
                aes.printStates();

                cout << "Round Key " << dec << i << '\n';
                kgen.printSelectedKey(i);

                cout << "AddRoundKey" << '\n';
                aes.addRoundKey(kgen.getKey(i));
                aes.printStates();

                if(i > 0) {
                    cout << "InvMixColumns" << '\n';
                    aes.callInvMixCols();
                    aes.printStates();
                }
                cout << "----------------------" << '\n';
            }

            cout << "\nPlaintext: ";
            aes.printPlaintext();
        }

        else if(option == 3) {
            break;
        }

        else {
            cout << "Invalid input. Please try again \n";
        }

    } while(option != 3);

    return 0;
}