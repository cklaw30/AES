#include <bits/stdc++.h>
#include "AES.cpp"
#include <sstream>
using namespace std;

class KeyGen{
    vector<vector<vector<int>>> keys;

    vector<int> rotWord(vector<int>& a) {
        int temp = a[0];
        for(int i=0; i<3; i++) {
            a[i] = a[i+1];
        }
        a[3] = temp;

        return a;
    }

    vector<int> subWord(vector<int>& a) {
        for(int i=0; i<4; i++) {
            int cur_bytes = a[i];
            int r = cur_bytes >> 4;
            int c = cur_bytes & 0x0F;

            a[i] = subBytes(r, c);
        }
        return a;
    }

    int getRCon(int round) {
        map<int, int> rCon = {
            {1, 0x01},
            {2, 0x02},
            {3, 0x04},
            {4, 0x08},
            {5, 0x10},
            {6, 0x20},
            {7, 0x40},
            {8, 0x80},
            {9, 0x1B},
            {10, 0x36}
        };

        return rCon[round];
    }

    vector<int> rCon(vector<int>& a, int round) {
        a[0] = a[0]^getRCon(round);

        return a;
    }

public:
    KeyGen() {
        keys.resize(11, vector<vector<int>>(4, vector<int>(4)));
    }

    void generateSubKey() {
        for (int i = 4; i < 44; i++) {
            int round = i / 4;
            int word_in_round = i % 4;

            if (word_in_round != 0) {
                vector<int> prev_word = keys[round][word_in_round - 1];
                vector<int> word_i_minus_4 = keys[round - 1][word_in_round];

                for (int j = 0; j < 4; j++) {
                    int b1 = prev_word[j];
                    int b2 = word_i_minus_4[j];
                    keys[round][word_in_round][j] = b1^b2;
                }
            } else {
                vector<int> t = keys[round - 1][3];
                t = rotWord(t);
                t = subWord(t);
                t = rCon(t, round);

                vector<int> word_i_minus_4 = keys[round - 1][0];
                for (int j = 0; j < 4; j++) {
                    keys[round][0][j] = t[j]^word_i_minus_4[j];
                }
            }
        }
    }

    void getSecretKey(string key) {
        for(int i=0; i<16; i++) {
            char let = key[i];
            keys[0][i/4][i%4] = int(let);
        }
        
        // vector<vector<int>> z{
        //     {0x0F, 0x15, 0x71, 0xC9},
        //     {0x47, 0xD9, 0xE8, 0x59},
        //     {0x0C, 0xB7, 0xAD, 0xD6},
        //     {0xAF, 0x7F, 0x67, 0x98}
        // };
        // keys[0] = z;

        generateSubKey();
    }

    vector<vector<int>> getKey(int n) {
        return keys[n];
    }

    void printKeys() {
        for (int i=0; i<11; i++) {
            for(int r=0; r<4; r++) {
                for(int c=0; c<4; c++) {
                    cout << setw(2) << setfill('0') << hex << uppercase << keys[i][c][r] << ' ';
                }
                cout << '\n';
            }
            cout << '\n';
        }
    }

    void printSelectedKey(int n) {
        vector<vector<int>> key = keys[n];        
        for(int r=0; r<4; r++) {
            for(int c=0; c<4; c++) {
                cout << setw(2) << setfill('0') << hex << uppercase << key[c][r] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }

    void printKey() {
        vector<vector<int>> key = keys[0];
        for(int r=0; r<4; r++) {
            for(int c=0; c<4; c++) {
                cout << setw(2) << setfill('0') << hex << uppercase << key[c][r] << ' ';
            }
        }
    }
};