#include <bits/stdc++.h>
#include "Table.cpp"
#include <sstream>
using namespace std;

class AES {
    vector<vector<vector<int>>> states;

public:
    AES(int n, string message, bool encrypt) {
        states.resize(n, vector<vector<int>>(4, vector<int>(4)));
        int m_length = message.length();

        if(encrypt) {
            for(int s=0; s<n; s++) {
                for(int r=0; r<4; r++) {
                    for(int c=0; c<4; c++) {
                        int index = r*4 + c + (s*16);
                        if(index < m_length) {
                            char let = message[index];
                            states[s][c][r] = int(let);
                        } else {
                            states[s][c][r] = int('Z');
                        }
                        
                    }
                }
            }
        } else {
            for(int s=0; s<n; s++) {
                string block = message.substr(s*32, 32);
                for(int i=0; i<32; i+=2) {
                    stringstream stream;
                    int y;
                    string s1 = block.substr(i, 2);
                    stream << s1 << ' ';
                    stream >> hex >> y;

                    states[s][(i%8)/2][i/8] = y;
                }
            }
        }
    }

    void addRoundKey(vector<vector<int>> key) {
        for(int s=0; s<getStateSize(); s++) {
            for(int i=0; i<4; i++) {
                for(int j=0; j<4; j++) {
                    states[s][i][j] = (states[s][i][j])^(key[j][i]);
                }
            }
        }
    }

    void callSubBytes() {
        for(int s=0; s<getStateSize(); s++) {
            for(int i=0; i<4; i++) {
                for(int j=0; j<4; j++) {
                    int cur_bytes = states[s][i][j];
                    int r = cur_bytes >> 4;
                    int c = cur_bytes & 0x0F;

                    states[s][i][j] = subBytes(r, c);
                }
            }
        }
    }

    void shiftRows() {
        for(int s=0; s<getStateSize(); s++) {
            for(int i=0; i<4; i++) {
                if(i==1) {
                    int temp = states[s][i][0];
                    for(int j=0; j<3; j++) {
                        states[s][i][j] = states[s][i][j+1];
                    }
                    states[s][i][3] = temp;
                }

                else if(i==2) {
                    int temp1 = states[s][i][0];
                    int temp2 = states[s][i][1];
                    for(int j=0; j<2; j++) {
                        states[s][i][j] = states[s][i][j+2];
                    }
                    states[s][i][2] = temp1;
                    states[s][i][3] = temp2;
                }

                else if(i==3) {
                    int temp = states[s][i][3];
                    for(int j=3; j>0; j--) {
                        states[s][i][j] = states[s][i][j-1];
                    }
                    states[s][i][0] = temp;
                }
            }
        }
    }

    void invShiftRows() {
        for(int s=0; s<getStateSize(); s++) {
            for(int i=0; i<4; i++) {
                if(i==1) {
                    int temp = states[s][i][3];
                    for(int j=3; j>0; j--) {
                        states[s][i][j] = states[s][i][j-1];
                    }
                    states[s][i][0] = temp;
                }

                else if(i==2) {
                    int temp1 = states[s][i][2];
                    int temp2 = states[s][i][3];
                    for(int j=2; j<4; j++) {
                        states[s][i][j] = states[s][i][j-2];
                    }
                    states[s][i][0] = temp1;
                    states[s][i][1] = temp2;
                }

                else if(i==3) {
                    int temp = states[s][i][0];
                    for(int j=0; j<3; j++) {
                        states[s][i][j] = states[s][i][j+1];
                    }
                    states[s][i][3] = temp;
                }
            }
        }
    }

    void callInvSubBytes() {
        for(int s=0; s<getStateSize(); s++) {
            for(int i=0; i<4; i++) {
                for(int j=0; j<4; j++) {
                    int cur_bytes = states[s][i][j];
                    int r = cur_bytes >> 4;
                    int c = cur_bytes & 0x0F;

                    states[s][i][j] = invSubBytes(r, c);
                }
            }
        }
    }

    int xtime(int a) {
        if (a & 0x80) {
            return ((a << 1) ^ 0x1b) & 0xff;
        }
        return a << 1;
    }

    vector<int> mixCols(vector<int> &col) {
        int c_0 = col[0];
        int all_xor = col[0] ^ col[1] ^ col[2] ^ col[3];
        
        col[0] ^= all_xor ^ xtime(col[0] ^ col[1]);
        col[1] ^= all_xor ^ xtime(col[1] ^ col[2]);
        col[2] ^= all_xor ^ xtime(col[2] ^ col[3]);
        col[3] ^= all_xor ^ xtime(c_0 ^ col[3]);

        return col;
    }

    void callMixCols() {
        for(int s=0; s<getStateSize(); s++) {
            vector<vector<int>> temp(4, vector<int>(4));
            for(int i=0; i<4; i++) {
                for(int j=0; j<4; j++) {
                    temp[i][j] = states[s][j][i];
                }
            }

            for (auto &col : temp) {
                col = mixCols(col);
            }

            for(int i=0; i<4; i++) {
                for(int j=0; j<4; j++) {
                    states[s][i][j] = temp[j][i];
                }
            }

        }        
    }

    int xtimes_0e(int b) {
        return xtime(xtime(xtime(b) ^ b) ^ b);
    }

    int xtimes_0b(int b) {
        return xtime(xtime(xtime(b)) ^ b) ^ b;
    }

    int xtimes_0d(int b) {
        return xtime(xtime(xtime(b) ^ b)) ^ b;
    }

    int xtimes_09(int b) {
        return xtime(xtime(xtime(b))) ^ b;
    }

    vector<int> invMixCols(vector<int>& col) {
        int c_0 = col[0], c_1 = col[1], c_2 = col[2], c_3 = col[3];
        col[0] = xtimes_0e(c_0) ^ xtimes_0b(c_1) ^ xtimes_0d(c_2) ^ xtimes_09(c_3);
        col[1] = xtimes_09(c_0) ^ xtimes_0e(c_1) ^ xtimes_0b(c_2) ^ xtimes_0d(c_3);
        col[2] = xtimes_0d(c_0) ^ xtimes_09(c_1) ^ xtimes_0e(c_2) ^ xtimes_0b(c_3);
        col[3] = xtimes_0b(c_0) ^ xtimes_0d(c_1) ^ xtimes_09(c_2) ^ xtimes_0e(c_3);

        return col;
    }

    void callInvMixCols() {
        for(int s=0; s<getStateSize(); s++) {
            vector<vector<int>> temp(4, vector<int>(4));
            for(int i=0; i<4; i++) {
                for(int j=0; j<4; j++) {
                    temp[i][j] = states[s][j][i];
                }
            }

            for (auto &col : temp) {
                col = invMixCols(col);
            }

            for(int i=0; i<4; i++) {
                for(int j=0; j<4; j++) {
                    states[s][i][j] = temp[j][i];
                }
            }

        }        
    }

    void printStates() {
        for(auto& i: states) {
            for(auto& j: i) {
                for(auto& z: j) {
                    cout << setw(2) << setfill('0') << hex << uppercase << z << ' ';
                }
                cout << '\n';
            }
            cout << '\n';
        }
    }

    void printOneState(int n) {
        vector<vector<int>> state = states[n];
        for(auto& i: state) {
            for(auto& j: i) {
                cout << hex << j << ' ';
            }
            cout << '\n';
        }
    }

    void printCiphertext() {
        for(int s=0; s<getStateSize(); s++) {
            for(int i=0; i<4; i++) {
                for(int j=0; j<4; j++) {
                    cout << setw(2) << setfill('0') << hex << uppercase << states[s][j][i];
                }
            }
        }
        cout << '\n';
    }

    void printPlaintext() {
        stringstream stream;
        int y;
        for(int s=0; s<getStateSize(); s++) {
            for(int i=0; i<4; i++) {
                for(int j=0; j<4; j++) {
                    stream << states[s][j][i] << ' ';
                    stream >> dec >> y;
                    cout << char(y);
                }
            }
        }
        cout << '\n';
    }

    int getStateSize() {
        return states.size();
    }
};