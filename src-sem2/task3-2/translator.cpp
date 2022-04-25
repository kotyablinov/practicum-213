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
    LEX_INT, LEX_STRING, LEX_REAL, LEX_BOOL, /* 9*/
    //"int", "string", "real", "bool",
    LEX_AND, LEX_NOT, LEX_OR, LEX_FALSE, LEX_TRUE, /* 14*/
    //"and", "not", "or", "false", "true"
    LEX_PROGRAM, LEX_READ, LEX_WRITE, LEX_CONST, /*18*/
    //"program", "read", "write", "const"
    LEX_FIN, /*19*/ 
    //"@"                                                                            
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, LEX_GTR, /*28*/ 
    //";", ",", ":", "=", "(", ")", "==", "<", ">"
    LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_UNARPLUS, LEX_UNARMINUS, /*37*/
    //"+", "-", "*", "/", "<=", "!=", ">=", "++", "--"   
    LEX_LFG, LEX_RFG, LEX_LQUOT, LEX_RQUOT, /*41*/
    //"{", "}", "\"", "\""
    LEX_STR, LEX_ID, LEX_NUMINT, LEX_NUMREAL,/*45*/
    POLIZ_LABEL, /*46*/ 
    POLIZ_ADDRESS, /*47*/
    POLIZ_GO, /*48*/
    POLIZ_FGO /*49*/
};

class Lex {
    type_of_lex t_lex;
    string v_lex;

   public:
    Lex(type_of_lex t = LEX_NULL, string v = "") : t_lex(t), v_lex(v) {}
    type_of_lex get_type() const {
        return t_lex;
    }
    string get_value() const {
        return v_lex;
    }
    friend ostream &operator<<(ostream &s, Lex l);
};

class Ident {
    string name;
    bool declare;
    type_of_lex type;
    bool assign;
    string value;

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
    string get_value() const {
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
    Scanner::TW[] = {"", "if", "else", "while", "do", "goto", "int", "string", "real",  "bool",
                     "and", "not", "or", "false", "true", "program", "read", "write", "const", NULL};

const char *
    Scanner::TD[] = {"@", ";", ",", ":", "=", "(", ")", "==", "<", ">", "+", "-", "*", "/",
                     "<=", "!=", ">=", "++", "--", "{", "}", "\"", "\"", NULL};

Lex Scanner::get_lex() {
    enum state { H, IDENT, NUMB, REAL, STR, ALE, NEQ };
    int d, j, powOfN;
    float fractionalN;
    string buf, currString;
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
                    currString = "";
                    CS = STR;
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
                        return Lex((type_of_lex)(j + (int)LEX_FIN), to_string(j));
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
                        return Lex((type_of_lex)j, to_string(j));
                    } else {
                        j = put(buf);
                        return Lex(LEX_ID, to_string(j));
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
                    return Lex ( LEX_NUMREAL, to_string(fractionalN));
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
                    return Lex(LEX_NUMINT, to_string(d));
                }
                break;
            case STR:
                if (c == '"') {
                    return Lex(LEX_STR, currString);
                } else if (c == '@')
                    throw c;
                else
                    currString.push_back(c);
                break;
            case ALE:
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), to_string(j));
                } else {
                    ungetc(c, fp);
                    j = look(buf, TD);
                    return Lex((type_of_lex)(j + (int)LEX_FIN), to_string(j));
                }
                break;
            case NEQ:
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex(LEX_NEQ, to_string(j));
                } else
                    throw '!';
                break;
        }
    } while (true);
}

////////////////////////////////////////////

template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
    i = st.top(); st.pop();
}
 
class Parser {
    Lex curr_lex;
    type_of_lex c_type;
    string c_val;
    Scanner scan;
    stack < string > st_string;
    stack < type_of_lex >   st_lex;
    void  P();
    void  B1();
    void  D();
    void  B();
    void  S();
    void  E();
    void  E1();
    void  T();
    void  F();
    void  dec ( type_of_lex type);
    void  check_id ();
    void  check_op ();
    void  check_not ();
    void  eq_type ();
    void  eq_bool ();
    void  check_id_in_read ();
    void  gl () {
        curr_lex  = scan.get_lex ();
        c_type    = curr_lex.get_type ();
        c_val     = curr_lex.get_value ();
    }
public:
    vector <Lex> poliz;
    Parser ( const char *program ) : scan (program) { }
    void  analyze();
};
 
void Parser::analyze () {
    gl ();
    P ();
    if (c_type != LEX_FIN)
        throw curr_lex;
    for ( Lex l : poliz ) 
        cout << l;
    cout << endl << "Yes!!!" << endl;
}

void Parser::P () {
    if ( c_type == LEX_PROGRAM ) {
        gl ();
    }
    else 
        throw curr_lex; 
    if ( c_type == LEX_LFG ) { 
        gl ();
        D();
        B1();
        if ( c_type == LEX_RFG ) {
            gl ();
        }
        else {
            throw curr_lex;
        }
    }
    else
        throw curr_lex;
}

void Parser::D () {
    type_of_lex line_type;
    while ( c_type == LEX_REAL || c_type == LEX_INT || c_type == LEX_STRING ) {
        if (c_type == LEX_REAL){
            line_type = LEX_REAL;
            gl();
        } else if (c_type == LEX_INT){
            line_type = LEX_INT;
            gl();
        } else {
            line_type = LEX_STRING;
            gl();
        }
        if ( c_type != LEX_ID )
            throw curr_lex;
        else {
            st_string.push ( c_val );

            dec (line_type);
            check_id ();
            poliz.push_back (Lex ( POLIZ_ADDRESS, c_val ) );
            gl ();
            if ( c_type == LEX_ASSIGN ) {
                gl ();
                E ();
                eq_type ();
                poliz.push_back ( Lex ( LEX_ASSIGN ) );
            } else {
                st_lex.top();
                poliz.pop_back();
            }

            while ( c_type == LEX_COMMA) {
                gl ();
                if ( c_type != LEX_ID)
                    throw curr_lex;
                else {
                    st_string.push ( c_val );
                    dec (line_type);
                    check_id ();
                    poliz.push_back (Lex ( POLIZ_ADDRESS, c_val ) );
                    gl ();
                    if ( c_type == LEX_ASSIGN ) {
                        gl ();
                        E ();
                        eq_type ();
                        poliz.push_back ( Lex ( LEX_ASSIGN ) );
                    } else {
                        st_lex.top();
                        poliz.pop_back();
                    }
                }
            }
            if ( c_type != LEX_SEMICOLON )
                throw curr_lex;
            else {
                dec (line_type);
                gl ();
            } 
        }
    }
}

void Parser::B1 () {
    S ();
    while ( c_type == LEX_SEMICOLON ) {
        gl ();
        if (c_type == LEX_RFG) break;
        S ();
    } 
}

void Parser::B () {
    if ( c_type == LEX_LFG ) {
        gl ();
        S ();
        while ( c_type == LEX_SEMICOLON ) {
            gl ();
            if (c_type == LEX_RFG) break;
            S ();
        }
        if ( c_type == LEX_RFG ) {
            gl ();
        }
        else {
            throw curr_lex;
        }
    }
    else
        throw curr_lex;
}

void Parser::S () {
    int pl0, pl1, pl2, pl3, pl4, pl5;
    vector <Lex> name_ptr;
    vector <int> addr_ptr;
    Lex point;
    if ( c_type == LEX_IF ) {
        //условие
        gl ();
        E ();
        eq_bool ();
        pl2 = poliz.size();
        poliz.push_back ( Lex() );
        poliz.push_back ( Lex(POLIZ_FGO) );
        //то
        S ();
        pl3 = poliz.size ();
        poliz.push_back ( Lex () );
        poliz.push_back ( Lex ( POLIZ_GO ) );
        poliz[pl2] = Lex ( POLIZ_LABEL, to_string(poliz.size()));
        //иначе
        if ( c_type == LEX_ELSE ) {
            gl ();
            S ();
            poliz[pl3] = Lex ( POLIZ_LABEL, to_string(poliz.size()) );
        }
        else
            throw curr_lex;
    }//end if
    else if (c_type == LEX_DO) {
        pl0 = poliz.size ();
        pl1 = poliz.size ();
        poliz.push_back ( Lex ( POLIZ_GO) );
        gl();
        S();
        poliz.push_back ( Lex ( POLIZ_LABEL, to_string(pl0 )) );
        poliz[pl1] = Lex ( POLIZ_LABEL, to_string(poliz.size()) );
        if (c_type == LEX_WHILE) {
            pl0 = poliz.size ();
            gl ();
            E ();
            eq_bool ();
            pl1 = poliz.size (); 
            poliz.push_back ( Lex () );
            poliz.push_back ( Lex (POLIZ_FGO) );
        }
    else
        throw curr_lex;
    }//end do while
    else if ( c_type == LEX_WHILE ) {
        pl0 = poliz.size ();
        gl ();
        E ();
        eq_bool ();
        pl1 = poliz.size (); 
        poliz.push_back ( Lex () );
        poliz.push_back ( Lex (POLIZ_FGO) );
        
        S();
        poliz.push_back ( Lex ( POLIZ_LABEL, to_string(pl0 ) ) );
        poliz.push_back ( Lex ( POLIZ_GO) );
        poliz[pl1] = Lex ( POLIZ_LABEL, to_string(poliz.size()) );
    }//end while
    else if ( c_type == LEX_READ ) {
        gl ();
        if ( c_type == LEX_LPAREN ) {
            gl ();
            if ( c_type == LEX_ID ) {
                check_id_in_read ();
                poliz.push_back ( Lex( POLIZ_ADDRESS, c_val) );
                gl();
            }
            else
                throw curr_lex;
            if ( c_type == LEX_RPAREN ) {
                gl ();
                poliz.push_back ( Lex (LEX_READ) );
            }    
        else
            throw curr_lex;
        }
    else  
            throw curr_lex;
    }//end read
    else if ( c_type == LEX_WRITE ) {
        gl ();
        if ( c_type == LEX_LPAREN ) {
            gl ();
            E ();
            while (c_type == LEX_COMMA) {
                gl ();
                E ();
            }
            if ( c_type == LEX_RPAREN ) {
                gl ();
                poliz.push_back ( Lex ( LEX_WRITE ) );
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;
    }//end write
    else if ( c_type == LEX_ID ) { 
        try{
        check_id (); }
        catch (string){
            point = curr_lex;
            pl5 = poliz.size ();
            gl ();
            if ( c_type == LEX_COLON ) {
                gl();
                name_ptr.push_back(point);
                addr_ptr.push_back(pl5);
                S();
            }
            else
                throw "not declared";
        }

        poliz.push_back (Lex ( POLIZ_ADDRESS, c_val ) );
        gl();
        if ( c_type == LEX_ASSIGN ) {
            gl ();
            E ();
            eq_type ();
            poliz.push_back ( Lex ( LEX_ASSIGN ) );
        }
        else
            throw curr_lex;
    }//assign-end
    else if ( c_type == LEX_GOTO ){
        pl4 = poliz.size (); 
        poliz.push_back ( Lex () );
        poliz.push_back ( Lex ( POLIZ_GO ) );
        gl ();
    }
    else
        B();
}

void Parser::E () {
    E1 ();
    if ( c_type == LEX_EQ  || c_type == LEX_LSS || c_type == LEX_GTR ||
         c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ ) {
        st_lex.push ( c_type );
        gl (); 
        E1 (); 
        check_op ();
    }
}
 
void Parser::E1 () {
    T ();
    while ( c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
        st_lex.push ( c_type );
        gl ();
        T ();
        check_op ();
    }
}

void Parser::T () {
    F ();
    while ( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND) {
        st_lex.push ( c_type );
        gl ();
        F ();
        check_op ();
    }
}

void Parser::F () {
    if ( c_type == LEX_ID ) {
        check_id ();
        poliz.push_back ( Lex ( LEX_ID, c_val ) );
        gl ();
    }
    else if (c_type == LEX_NUMINT) {
        st_lex.push ( LEX_INT );
        poliz.push_back ( curr_lex );
        gl ();
    }
    else if (c_type == LEX_NUMREAL) {
        st_lex.push (LEX_REAL);
        poliz.push_back ( curr_lex );
        gl ();
    }
    else if ( c_type == LEX_STR ) {
        st_lex.push ( LEX_STRING );
        poliz.push_back ( curr_lex );
        gl ();
    }
    else if ( c_type == LEX_TRUE ) {
        st_lex.push ( LEX_BOOL );
        poliz.push_back ( Lex (LEX_TRUE, "1") );
        gl ();
    }
    else if ( c_type == LEX_FALSE) {
        st_lex.push ( LEX_BOOL );
        poliz.push_back ( Lex (LEX_FALSE, "0") );
        gl ();
    }
    else if ( c_type == LEX_NOT ) {
        gl (); 
        F (); 
        check_not ();
    }
    else if ( c_type == LEX_MINUS ) {
        st_lex.push ( LEX_INT );
        poliz.push_back ( Lex (LEX_INT, "0") );
        st_lex.push ( c_type );
        gl ();
        F ();
    }
    else if ( c_type == LEX_PLUS ) {
        gl (); 
        F ();
    }
    else if ( c_type == LEX_LPAREN ) {
        gl (); 
        E ();
        if ( c_type == LEX_RPAREN)
            gl ();
        else 
            throw curr_lex;
    }
    else 
        throw curr_lex;
}
 
////////////////////////////////////////////////////////////////
 
void Parser::dec ( type_of_lex type ) {
    string i;
    while ( !st_string.empty () ) {
        from_st ( st_string, i );
        if ( TID[stoi(i)].get_declare () ) 
            throw "twice";
        else {
            TID[stoi(i)].put_declare ();
            TID[stoi(i)].put_type ( type );
        }
    }
}
 
void Parser::check_id () {
    if ( TID[stoi(c_val)].get_declare() )
        st_lex.push ( TID[stoi(c_val)].get_type () );
    else 
        throw "not declared";
}
 
void Parser::check_op () {
    type_of_lex t1, t2, op, s = LEX_INT, t = LEX_REAL, r = LEX_BOOL;
 
    from_st ( st_lex, t2 );
    from_st ( st_lex, op );
    from_st ( st_lex, t1 );
 
    if ( op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH )
        r = t2;
    if ( op == LEX_OR || op == LEX_AND ){
        t = LEX_BOOL;
        s = LEX_BOOL;
    }
    if ( t1 == t2  &&  (t1 == t||t1 == s) ) 
        st_lex.push (r);
    else
        throw "wrong types are in operation";
    poliz.push_back (Lex (op) );
}
 
void Parser::check_not () {
    if (st_lex.top() != LEX_BOOL)
        throw "wrong type is in not";
    else  
        poliz.push_back ( Lex (LEX_NOT) );
}
 
void Parser::eq_type () {
    type_of_lex t;
    from_st ( st_lex, t );
    if ( t != st_lex.top () )
        throw "wrong types are in =";
    st_lex.pop();
}

void Parser::eq_bool () {
    if ( st_lex.top () != LEX_BOOL )
        throw "expression is not boolean";
    st_lex.pop ();
  }
 
void Parser::check_id_in_read () {
    if ( !TID[stoi(c_val)].get_declare() )
        throw "not declared";
}

///////////////////////////////////////////
ostream & operator<< ( ostream &out, Lex l ){
    int type = l.get_type();
    if (type < LEX_FIN)
        out << Scanner::TW[type]<< ' ';
    else if (type < LEX_STR)
        out << Scanner::TD[type - LEX_FIN]<< ' ';
    if (type == LEX_STR)
        out << l.get_value() << ' ';
    else if (type == LEX_NUMINT)
        out << l.get_value() << ' ';
    else if (type == LEX_NUMREAL)
        out << l.get_value() << ' ';
    else if (type == LEX_ID)
        out << "PID ";
    return out;
}

int main () {
    try {
      Parser pars("input1.txt");
      pars.analyze();
      return 0;
    }
    catch ( char c ) {
        cout << "unexpected char " << c << endl;
        return 1;
    }
    catch ( Lex l ) {
        cout << "unexpected lexeme " << l << endl;
        return 1;
    }
    catch ( const char *source ) {
        cout << source << endl;
        return 1;
    }
}