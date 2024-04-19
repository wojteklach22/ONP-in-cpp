//POP_2021_01_13_projekt_2_Lach_Wojciech_EIT_3_185759 ,CodeBlocks 20.03 GNU GCC Compiler
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>

using namespace std;

void menu();
void readFile(fstream& file, string path);
string makeOnp(string line);
string calculateFromOnr(string eqOnr);
void copyFiles(string path);

int main()
{
    menu();
    return 0;
}


void menu() {
    while (1) {
        string ans;
        cout << "Wpisz sciezke pliku bez rozszerzenia albo \"wyjscie\" aby wyjsc z programu:" << endl;
        cin >> ans;
        if (ans == "wyjscie") break;

        system("cls");

        ans += ".txt";
        fstream equationsFile;
        equationsFile.open(ans, ios::in);

        if (equationsFile.good()) {
            readFile(equationsFile, ans);
        }
        else {
            cout << "Plik nie istnieje! wpisz jeszcze raz." << endl;
        }
    }
}


void readFile(fstream& file, string path) {
    string lineBuffer;
    string lineOnrBuffer;
    string ans;
    fstream bufFile;
    bufFile.open("eqBufferLines.txt", ios::out);
    while (!file.eof()) {
        getline(file, lineBuffer);
        if (lineBuffer != "") {
            lineOnrBuffer = makeOnp(lineBuffer);
            ans = calculateFromOnr(lineOnrBuffer);
            cout << lineBuffer << " | ONP: " << lineOnrBuffer << " | wynik: " << ans << endl;
            bufFile << lineBuffer << " " << ans << endl;
        }

    }
    file.close();
    bufFile.close();
    copyFiles(path);
}


string makeOnp(string line) {
    if (line.at(0) != '#') return { 'e', 'r', 'r' };

    char *equationOnp = new char[line.length() * 2];
    int equationPionter = 0;
    char *stackBuf = new char[line.length() * 2];
    int pointer = 0;

    for (int i = 0; i < line.length() * 2; i++) {
        equationOnp[i] = ' ';
        stackBuf[i] = ' ';
    }

    bool equalsFlag = false;
    for (int i = 1; i < line.length(); i++) {
        char buf = line.at(i);
        if (buf == ' ') continue;
        else if (buf == '(') {
            stackBuf[pointer] = buf;
            pointer++;
        }
        else if (buf == ')') {
            for (; stackBuf[pointer - 1] != '('; pointer--) {
                equationOnp[equationPionter++] = stackBuf[pointer - 1];
                if (pointer <= 1) return { 'e', 'r', 'r' };
            }
            pointer--;
        }
        else if (buf == '+' || buf == '-') {
            while (pointer > 0 && stackBuf[pointer - 1] != '(' && stackBuf[pointer - 1] != '+' && stackBuf[pointer - 1] != '-') {
                equationOnp[equationPionter++] = stackBuf[--pointer];
            }
            stackBuf[pointer++] = buf;
        }
        else if (buf == '*' || buf == '/') {
            while (pointer > 0 && ((stackBuf[pointer - 1] == '^' || stackBuf[pointer - 1] == 's' || stackBuf[pointer - 1] == 'q' || stackBuf[pointer - 1] == 'c'))) {
                equationOnp[equationPionter++] = stackBuf[--pointer];
            }
            stackBuf[pointer++] = buf;
        }
        else if (buf == '^') {
            stackBuf[pointer++] = buf;
        }
        else if (buf == 'c' || buf == 's') {
            if (i + 3 >= line.length()) break;
            if ((buf == 'c' && line.at(i + 1) == 'o' && line.at(i + 2) == 's') || (buf == 's' && line.at(i + 1) == 'i' && line.at(i + 2) == 'n')) {
                stackBuf[pointer++] = buf;
                i += 2;
            }
            else if (buf == 's' && line.at(i + 1) == 'q' && line.at(i + 2) == 'r' && line.at(i + 3) == 't') {
                stackBuf[pointer++] = 'q';
                i += 3;
            }
            else return { 'e', 'r', 'r' };
        }
        else if (buf >= '0' && buf <= '9') {
            equationOnp[equationPionter++] = buf;
            if (i + 1 >= line.length()) break;
            buf = line.at(i + 1);
            while ((buf >= '0' && buf <= '9') || buf == ',' || buf == '.') {
                i++;
                if (buf == ',') buf = '.';
                equationOnp[equationPionter++] = buf;
                buf = line.at(i + 1);
            }
            equationOnp[equationPionter++] = ' ';
        }
        else if (buf == '=') {
            equalsFlag = true;
            break;
        }
        else {
            return { 'e', 'r', 'r' };
        }
    }

    if (!equalsFlag) return { 'e', 'r', 'r' };

    while (pointer >= 0) {

        equationOnp[equationPionter++] = stackBuf[--pointer];
    }
    equationOnp[equationPionter] = '=';
    string returnString = "";
    int length = sizeof(equationOnp) / sizeof(char);
    for (int i = 0; i < length; i++) {
        if (equationOnp[i] == '=') break;
        else returnString += equationOnp[i];
    }
    return returnString;
}


string calculateFromOnr(string eqOnr) {
    if (eqOnr == "err") return eqOnr;

    double *stackBuf = new double[eqOnr.length()];
    int pointer = 0;
    char buf;
    bool isFloatingPoint;

    for (int i = 0; i < eqOnr.length(); i++) {
        buf = eqOnr.at(i);
        if (buf >= '0' && buf <= '9') {
            if (i + 1 >= eqOnr.length()) return "err";
            stackBuf[pointer++] = (double)(((int)buf) - 48);
            buf = eqOnr.at(i + 1);
            isFloatingPoint = false;
            double divideFactor = 10.0;
            while ((buf >= '0' && buf <= '9') || buf == '.') {
                if (buf == '.') {
                    isFloatingPoint = true;
                }
                else if (isFloatingPoint) {
                    double doubleBuf = (double)((((int)buf) - 48) / divideFactor);
                    stackBuf[pointer - 1] += doubleBuf;
                    divideFactor *= 10.0;
                }
                else {
                    stackBuf[pointer - 1] *= 10;
                    stackBuf[pointer - 1] += (double)(((int)buf) - 48);
                }
                buf = eqOnr.at(++i + 1);
            }
        }
        else if (buf == '*' || buf == '/' || buf == '+' || buf == '-' || buf == '^') {
            if (pointer <= 1) return "err";
            double a = stackBuf[--pointer];
            double b = stackBuf[--pointer];
            if (buf == '*') stackBuf[pointer++] = a * b;
            else if (buf == '/') stackBuf[pointer++] = b / a;
            else if (buf == '+') stackBuf[pointer++] = b + a;
            else if (buf == '-') stackBuf[pointer++] = b - a;
            else if (buf == '^') stackBuf[pointer++] = pow(b, a);
        }
        else if (buf == 'c' || buf == 's' || buf == 'q') {
            if (pointer < 1) return "err";
            double a = stackBuf[--pointer];
            if (buf == 'c') stackBuf[pointer++] = cos(a);
            else if (buf == 's') stackBuf[pointer++] = sin(a);
            else if (buf == 'q') stackBuf[pointer++] = sqrt(a);
        }
    }
    string returnStatement;
    if (pointer != 1) return "err";
    else returnStatement = to_string(stackBuf[0]);
    return returnStatement;
}


void copyFiles(string path) {
    fstream equationsFile;
    equationsFile.open(path, ios::out);
    fstream bufFile;
    string buf;
    bufFile.open("eqBufferLines.txt", ios::in);
    if (bufFile.good()) {
        while (!bufFile.eof()) {
            getline(bufFile, buf);
            equationsFile << buf << endl;
        }
    }
    equationsFile.close();
    bufFile.close();
    remove("eqBufferLines.txt");

}
