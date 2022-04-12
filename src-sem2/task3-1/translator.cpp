#include <ctype.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

enum type_of_lex {
    LEX_NULL, /* 0*/
    LEX_IF, LEX_ELSE, LEX_WHILE, LEX_DO, LEX_GOTO, /* 5*/
    //"if", "else", "while", "do", "goto"
    LEX_INT, LEX_STRING, LEX_REAL, /* 8*/
    //"int", "string", "real", 
    LEX_AND, LEX_NOT, LEX_OR, LEX_FALSE, LEX_TRUE, /* 13*/
    //"and", "not", "or", "false", "true"
    LEX_PROGRAM, LEX_READ, LEX_WRITE, LEX_CONST, /*17*/
    //"program", "read", "write", "const"
    LEX_FIN, /*18*/ 
    //"@"                                                                            
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, LEX_GTR, /*27*/ 
    //";", ",", ":", "=", "(", ")", "==", "<", ">"
    LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_UNARPLUS, LEX_UNARMINUS, /*36*/
    //"+", "-", "*", "/", "<=", "!=", ">=", "++", "--"   
    LEX_LFG, LEX_RFG, LEX_LQUOT, LEX_RQUOT,
    //"{", "}", "\"", "\""
    LEX_STR, LEX_ID, LEX_NUMINT, LEX_NUMREAL,/*40*/
    POLIZ_LABEL, /*41*/ 
    POLIZ_ADDRESS, /*42*/
    POLIZ_GO, /*43*/
    POLIZ_FGO /*44*/
};

class Lex {
    type_of_lex t_lex;
    int v_lex;

   public:
    Lex(type_of_lex t = LEX_NULL, int v = 0) : t_lex(t), v_lex(v) {}
    type_of_lex get_type() const {
        return t_lex;
    }
    int get_value() const {
        return v_lex;
    }
    friend ostream &operator<<(ostream &s, Lex l);
};

class Ident {
    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;

   public:
    Ident() {
        declare = false;
        assign = false;
    }
    bool operator==(const string &s) const {
        return name == s;
    }
    Ident(const string n) {
        name = n;
        declare = false;
        assign = false;
    }
    string get_name() const {
        return name;
    }
    bool get_declare() const {
        return declare;
    }
    void put_declare() {
        declare = true;
    }
    type_of_lex get_type() const {
        return type;
    }
    void put_type(type_of_lex t) {
        type = t;
    }
    bool get_assign() const {
        return assign;
    }
    void put_assign() {
        assign = true;
    }
    int get_value() const {
        return value;
    }
    void put_value(int v) {
        value = v;
    }
};

vector<Ident> TID;
int put(const string &buf) {
    vector<Ident>::iterator k;
    if ((k = find(TID.begin(), TID.end(), buf)) != TID.end())
        return k - TID.begin();
    TID.push_back(Ident(buf));
    return TID.size() - 1;
}

class Scanner {
    FILE *fp;
    char c;
    int look(const string buf, const char **list) {
        int i = 0;
        while (list[i]) {
            if (buf == list[i])
                return i;
            ++i;
        }
        return 0;
    }
    void gc() {
        c = fgetc(fp);
    }

   public:
    static const char *TW[], *TD[];
    Scanner(const char *program) {
        if (!(fp = fopen(program, "r")))
            throw "can’t open file";
    }
    Lex get_lex();
};

const char *
    Scanner::TW[] = {"", "if", "else", "while", "do", "goto", "int", "string", "real",
                     "and", "not", "or", "false", "true", "program", "read", "write", "const", NULL};

const char *
    Scanner::TD[] = {"@", ";", ",", ":", "=", "(", ")", "==", "<", ">", "+", "-", "*", "/",
                     "<=", "!=", ">=", "++", "--", "{", "}", "\"", "\"", NULL};

Lex Scanner::get_lex() {
    enum state { H, IDENT, NUMB, REAL, STR, UNARPLUS, UNARMINUS, ALE, NEQ };
    int d, j, powOfN;
    float fractionalN;
    string buf;
    state CS = H;
    do {
        gc();
        switch (CS) {
            case H:
                if (c == ' ' || c == '\n' || c == '\r' || c == '\t');
                else if (isalpha(c)) {
                    buf.push_back(c);
                    CS = IDENT;
                } else if (isdigit(c)) {
                    d = c - '0';
                    fractionalN = 0;
                    powOfN = 1;
                    CS = NUMB;
                } else if (c == '"') {
                    buf.push_back(c);
                    CS = STR;
                } else if (c == '+') {
                    buf.push_back(c);
                    CS = UNARPLUS;
                } else if (c == '-') {
                    buf.push_back(c);
                    CS = UNARMINUS;
                } else if (c == '=' || c == '<' || c == '>') {
                    buf.push_back(c);
                    CS = ALE;
                } else if (c == '@')
                    return Lex(LEX_FIN);
                else if (c == '!') {
                    buf.push_back(c);
                    CS = NEQ;
                } else {
                    buf.push_back(c);
                    if ((j = look(buf, TD))) {
                        return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                    } else
                        throw c;
                }
                break;
            case IDENT:
                if (isalpha(c) || isdigit(c)) {
                    buf.push_back(c);
                } else {
                    ungetc(c, fp);
                    if ((j = look(buf, TW))) {
                        return Lex((type_of_lex)j, j);
                    } else {
                        j = put(buf);
                        return Lex(LEX_ID, j);
                    }
                }
                break;
            case REAL:
                if ( isdigit (c) ) {
                    fractionalN = fractionalN + ( c - '0' )*pow(0.1,powOfN);
                    powOfN++;
                }
                else {
                    ungetc ( c, fp );
                    return Lex ( LEX_NUMREAL, fractionalN );
                }
                break;
            case NUMB:
                if (isdigit(c)) {
                    d = d * 10 + (c - '0');
                } else if (c == '.') {
                    fractionalN = d;
                    CS = REAL;
                } else {
                    ungetc(c, fp);
                    return Lex(LEX_NUMINT, d);
                }
                break;
            case STR:
                if (c == '"') {
                    return Lex(LEX_STR, 0);
                } else if (c == '@')
                    throw c;
                break;
            case UNARPLUS:
                if (c == '+') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                } else {
                    ungetc(c, fp);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                }
                break;
            case UNARMINUS:
                if (c == '-') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                } else {
                    ungetc(c, fp);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                }
                break;
            case ALE:
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                } else {
                    ungetc(c, fp);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), j);
                }
                break;
            case NEQ:
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex(LEX_NEQ, j);
                } else
                    throw '!';
                break;
        }
    } while (true);
}

ostream & operator<< ( ostream &out, Lex l ){
    int type = l.get_type();
    if (type < LEX_FIN)
        out << Scanner::TW[type];
    else if (type < LEX_STR)
        out << Scanner::TD[type - LEX_FIN];
    if (type == LEX_STR)
        out << "STRING";
    else if (type == LEX_NUMINT)
        out << "INTEGER";
    else if (type == LEX_NUMREAL)
        out << "REAL";
    else if (type == LEX_ID)
        out << "PID";
    out << " == " << l.get_value();
    return out;
}

int main() {
    try {
        Scanner currentFile("input.txt");
        Lex temp;
        while (temp.get_type() != LEX_FIN) {
            temp = currentFile.get_lex();
            cout << temp << endl;
        }
        return 0;
    } catch (char c) {
        cout << "unexpected symbol " << c << endl;
        return 1;
    } catch (Lex l) {
        cout << "unexpected lexeme " << l << endl;
        return 1;
    } catch (const char *source) {
        cout << source << endl;
        return 1;
    }
}