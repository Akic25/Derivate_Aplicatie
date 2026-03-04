#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <graphics.h>
#include <winbgim.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

ifstream in("rezolvare.txt");
ofstream out("afisare_derivata.txt");
int i;
struct nod
{
    char info;
    nod* urmator;
};

struct stiva
{
    nod* varf;
    int nrElemente;
};
struct rezultat_salvat
{
    string functia;
    string derivata_I;
    string derivata_IIa;
} rez;


bool esteVida(stiva S)
{
    return S.nrElemente == 0;
}

void Initializeaza(stiva& S)
{
    S.varf = NULL;
    S.nrElemente = 0;
}

struct arbore_nod
{
    string info;
    arbore_nod* st, * dr;
    int nivel, coloana;
};

struct nod_p
{
    arbore_nod* info;
    nod_p* urmator;
};

struct stiva_p
{
    nod_p* varf;
};

stiva_p S, S2;
arbore_nod* a, * a2;

int popStiva(stiva& S)
{
    if (!esteVida(S))
    {
        char element;
        element = S.varf->info;
        nod* varf_nou;
        varf_nou = new nod;
        varf_nou = S.varf->urmator;
        delete S.varf;
        S.varf = varf_nou;
        S.nrElemente--;
        return element;
    }
    else
    {
        cout << "Stiva este goala";
        return 0;
    }
}

void Goleste(stiva& S)
{
    while (!esteVida(S))
        popStiva(S);
}

void pushStiva(stiva& S, char element)
{
    if (esteVida(S))
    {
        nod* nod_nou;
        nod_nou = new nod;
        nod_nou->info = element;
        nod_nou->urmator = NULL;
        S.varf = nod_nou;
        S.nrElemente = 1;
    }
    else
    {
        nod* nod_nou;
        nod_nou = new nod;
        nod_nou->info = element;
        nod_nou->urmator = S.varf;
        S.varf = nod_nou;
        S.nrElemente++;

    }
}

int topStiva(stiva S)
{
    if (!esteVida(S))
        return S.varf->info;
}

bool suntOperatori(char c)
{
    if (c == '^' || c == '+' || c == '*' || c == '-' || c == '/')
        return 1;
    return 0;
}

bool esteOperand(char c)
{
    if (c >= 'a' && c <= 'z')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 1;
    if (c >= '0' && c <= '9')
        return 1;
    if (c == '_' || c == ',' || c == '.')
        return 1;
    return 0;
}

int esteRadical(string postfixat)
{
    if (postfixat == "sqrt")
        return 9;
    if (postfixat == "_sqrt")
        return 19;
    return 0;
}

int esteLogaritm(string postfixat)
{
    if (postfixat == "log")
        return 10;
    if (postfixat == "_log")
        return 20;
    return 0;
}

int esteTrigonometrie(string postfixat)
{
    if (postfixat == "sin")
        return 1;
    if (postfixat == "_sin")
        return 11;
    if (postfixat == "cos")
        return 2;
    if (postfixat == "_cos")
        return 12;
    if (postfixat == "tg")
        return 3;
    if (postfixat == "_tg")
        return 13;
    if (postfixat == "ctg")
        return 4;
    if (postfixat == "_ctg")
        return 14;
    if (postfixat == "arcsin")
        return 5;
    if (postfixat == "_arcsin")
        return 15;
    if (postfixat == "arccos")
        return 6;
    if (postfixat == "_arccos")
        return 16;
    if (postfixat == "arctg")
        return 7;
    if (postfixat == "_arctg")
        return 17;
    if (postfixat == "arcctg")
        return 8;
    if (postfixat == "_arcctg")
        return 18;
    return 0;
}
bool esteOperator_aritmetic(string c)
{

    if (c == "^" || c == "+" || c == "*" || c == "-" || c == "/")
        return true;
    if (esteRadical(c) || esteTrigonometrie(c) || esteLogaritm(c))
        return true;
    return false;
}

int prioritateOperator(char c)
{
    if (c == '^')
        return 3;
    if (c == '*' || c == '/')
        return 2;
    if (c == '+' || c == '-')
        return 1;
    return 0;
}
void transformaExpresie(string infixat, string& sir)
{
    stiva S;
    string postfixat;
    char ch;
    infixat.insert(infixat.begin(), '(');
    infixat.insert(infixat.end(), ')');
    string numar;
    for (string::iterator t = infixat.begin(); t != infixat.end(); t++)
    {
        if (esteOperand(*t))
        {
            numar.insert(numar.end(), *t);
            continue;
        }
        if (!esteOperand(*t++))
        {
            if (esteTrigonometrie(numar) != 0)
                pushStiva(S, esteTrigonometrie(numar));
            else if (esteLogaritm(numar) != 0)
                pushStiva(S, esteLogaritm(numar));
            else if (esteRadical(numar) != 0)
                pushStiva(S, esteRadical(numar));
            else
                for (string::iterator i = numar.begin(); i != numar.end(); i++)
                {
                    postfixat.insert(postfixat.end(), *i);
                }
            postfixat.insert(postfixat.end(), ' ');
            numar.erase();
            t--;
        }
        if (suntOperatori(*t))
        {

            int prioritate = prioritateOperator(*t);
            while (!esteVida(S))
            {
                ch = topStiva(S);
                if (suntOperatori(ch) && prioritateOperator(ch) >= prioritate)
                {

                    postfixat.insert(postfixat.end(), ch);
                    postfixat.insert(postfixat.end(), ' ');
                    popStiva(S);
                }
                else
                    break;
            }
            pushStiva(S, *t);
            continue;
        }
        if (*t == '(')
        {
            pushStiva(S, *t);
            continue;
        }
        if (*t == ')')
        {
            while ((ch = topStiva(S)) != '(')
            {
                postfixat.insert(postfixat.end(), ch);
                postfixat.insert(postfixat.end(), ' ');
                popStiva(S);
            }
            popStiva(S);
            ch = topStiva(S);
            if (ch == 1 or ch == 2 or ch == 3 or ch == 4 or ch == 5 or ch == 6 or ch == 7 or ch == 8 or ch == 11 or ch == 12 or ch == 13 or ch == 14 or ch == 15 or ch == 16 or ch == 17 or ch == 18)
            {
                string valoare;
                if (ch == 1)    valoare = "sin";
                if (ch == 2)    valoare = "cos";
                if (ch == 3)    valoare = "tg";
                if (ch == 4)    valoare = "ctg";
                if (ch == 5)    valoare = "arcsin";
                if (ch == 6)    valoare = "arccos";
                if (ch == 7)    valoare = "arctg";
                if (ch == 8)    valoare = "arcctg";
                if (ch == 11)    valoare = "_sin";
                if (ch == 12)    valoare = "_cos";
                if (ch == 13)    valoare = "_tg";
                if (ch == 14)    valoare = "_ctg";
                if (ch == 15)    valoare = "_arcsin";
                if (ch == 16)    valoare = "_arccos";
                if (ch == 17)    valoare = "_arctg";
                if (ch == 18)    valoare = "_arcctg";
                postfixat.append(valoare);
                popStiva(S);
            }
            if ((ch = topStiva(S)) == 10)
            {
                string valoare;
                valoare = "log";
                postfixat.append(valoare);
                popStiva(S);
            }
            if ((ch = topStiva(S)) == 20)
            {
                string valoare;
                valoare = "_log";
                postfixat.append(valoare);
                popStiva(S);
            }
            if ((ch = topStiva(S)) == 9)
            {
                string valoare;
                valoare = "sqrt";
                postfixat.append(valoare);
                popStiva(S);
            }
            if ((ch = topStiva(S)) == 19)
            {
                string valoare;
                valoare = "_sqrt";
                postfixat.append(valoare);
                popStiva(S);
            }
            continue;
        }
        if (isspace(*t))
            continue;
    }
    sir = postfixat;
}


void push(stiva_p& S, arbore_nod* a)
{
    nod_p* p;
    if (S.varf == NULL)
    {
        S.varf = new nod_p;
        S.varf->info = a;
        S.varf->urmator = NULL;
    }
    else
    {
        p = new nod_p;
        p->info = a;
        p->urmator = S.varf;
        S.varf = p;
    }
}

void pop(stiva_p& S, arbore_nod*& a)
{
    if (S.varf != NULL)
    {
        nod_p* varf_nou;
        varf_nou = S.varf->urmator;
        a = S.varf->info;
        delete S.varf;
        S.varf = varf_nou;
    }
    else
        cout << "Expresie invalida" << endl;
}
int lungime(string c)
{
    return c.length();
}
int numar_nivel(arbore_nod* a)
{
    if (a == NULL)
        return 0;
    else
    {
        int n1 = numar_nivel(a->st);
        int n2 = numar_nivel(a->dr);
        return 1 + max(n1, n2);
    }
}
int numar_coloane(arbore_nod* a)
{
    if (a == NULL)return 0;
    else
    {
        int n1 = numar_coloane(a->st);
        int n2 = numar_coloane(a->dr);
        return 1 + n1 + n2;
    }
}

void deseneaza_nodul(const std::string& element, int xc, int yc, int raza)
{
    // Converteste sirul in array de caractere
    char arr[50];
    strcpy(arr, element.c_str());

    // Seteaza culoarea si umplerea pentru cerc
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, WHITE);

    // Deseneaza cercul umplut
    fillellipse(xc, yc, raza, raza);

    // Seteaza stilul și proprietățile textului
    settextstyle(BOLD_FONT, HORIZ_DIR, 2); // Font mai gros și clar
    setbkcolor(WHITE);
    setcolor(BLACK);
    settextjustify(CENTER_TEXT, CENTER_TEXT);

    // Deseneaza textul în interiorul cercului
    outtextxy(xc, yc, arr);
}

void deseneaza_arbore(arbore_nod* a, int niv, int stanga, int latime, int inaltime, int raza, bool vertical)
{
    if (a != NULL)
    {
        int numarcoloane_stanga = numar_coloane(a->st);
        int xc, yc;

        if (vertical)
        {
            // Pentru afișare verticală (rădăcina sus, copiii jos)
            xc = stanga + latime / 2;
            yc = niv * inaltime - inaltime / 2;
        }
        else
        {
            // Afișare orizontală (implicită)
            xc = stanga + latime * numarcoloane_stanga + latime / 2;
            yc = niv * inaltime - inaltime / 2;
        }

        // Desenează linia către fiul stâng, dacă există
        if (a->st != NULL)
        {
            int xcs, ycs;
            if (vertical)
            {
                xcs = xc;
                ycs = yc + inaltime;
            }
            else
            {
                xcs = stanga + latime * numar_coloane(a->st->st) + latime / 2;
                ycs = yc + inaltime;
            }
            setcolor(WHITE);
            setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
            line(xc, yc, xcs, ycs);
        }

        // Desenează linia către fiul drept, dacă există
        if (a->dr != NULL)
        {
            int xcd, ycd;
            if (vertical)
            {
                xcd = xc;
                ycd = yc + inaltime;
            }
            else
            {
                xcd = stanga + latime * (numarcoloane_stanga + 1) + latime * numar_coloane(a->dr->st) + latime / 2;
                ycd = yc + inaltime;
            }
            setcolor(WHITE);
            setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
            line(xc, yc, xcd, ycd);
        }

        // Verifică dacă nodul curent este funcție trigonometrică și ajustează afișarea nodurilor
        if (vertical && (a->info == "sin" || a->info == "cos" || a->info == "arctg" || a->info == "arccos"))
        {
            deseneaza_nodul(a->info, xc, yc, raza);

            if (a->st != NULL)
            {
                int sub_ycs = yc + inaltime;
                setcolor(WHITE);
                setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
                line(xc, yc, xc, sub_ycs);
                deseneaza_arbore(a->st, niv + 1, stanga, latime, inaltime, raza, vertical);
            }
        }
        else
        {
            // Desenează nodul curent
            deseneaza_nodul(a->info, xc, yc, raza);

            // Apelează recursiv pentru fiii stâng și drept
            if (vertical)
            {
                deseneaza_arbore(a->st, niv + 1, stanga, latime, inaltime, raza, vertical);
                deseneaza_arbore(a->dr, niv + 1, stanga, latime, inaltime, raza, vertical);
            }
            else
            {
                deseneaza_arbore(a->st, niv + 1, stanga, latime, inaltime, raza, vertical);
                deseneaza_arbore(a->dr, niv + 1, stanga + latime * (numarcoloane_stanga + 1), latime, inaltime, raza, vertical);
            }
        }
    }
}

void deseneaza(arbore_nod* radacina, bool vertical = false)
{
    int height = 700, width = 1000;
    initwindow(width, height, "Arborele Binar", 150, 150, false, false);
    setcolor(WHITE);
    setbkcolor(BLACK);
    cleardevice();

    rectangle(1, 1, width - 1, height - 1);

    int latime = vertical ? height / numar_nivel(radacina) : width / numar_coloane(radacina);
    int inaltime = vertical ? width / numar_coloane(radacina) : height / numar_nivel(radacina);
    int raza = min(latime, inaltime) / 4;

    deseneaza_arbore(radacina, 1, 0, latime, inaltime, raza, vertical);

    getch();
    closegraph(CURRENT_WINDOW);
}



void creeaza_arbore(stiva_p& s, string postfixat)
{
    s.varf = NULL;
    int i;
    string token;
    for (i = 0; postfixat[i] != NULL; i++)
    {
        token.erase();
        while (postfixat[i] != ' ' && postfixat[i] != NULL)
            token += postfixat[i], i++;
        if (esteTrigonometrie(token) && token[0] != '_')
        {
            arbore_nod* operatie, * operand;
            operatie = new arbore_nod;
            operatie->info = token;
            pop(s, operand);
            operatie->st = operand;
            operatie->dr = NULL;
            push(s, operatie);
        }
        else if (esteTrigonometrie(token) && token[0] == '_')
        {
            arbore_nod* operatie, * operand, * inmultire, * semn;
            operatie = new arbore_nod;
            inmultire = new arbore_nod;
            semn = new arbore_nod;
            token.erase(0, 1);
            operatie->info = token;
            pop(s, operand);
            operatie->st = operand;
            operatie->dr = NULL;
            inmultire->info = "*";
            inmultire->dr = operatie;
            inmultire->st = semn;
            semn->info = "_1";
            semn->dr = NULL;
            semn->st = NULL;
            push(s, inmultire);
        }
        else if (esteOperator_aritmetic(token) && token[0] != '_')
        {
            arbore_nod* operatie, * a, * b;
            operatie = new arbore_nod;
            operatie->info = token;
            pop(s, a);
            pop(s, b);
            operatie->st = b;
            operatie->dr = a;
            push(s, operatie);
        }
        else if (esteOperator_aritmetic(token) && token[0] == '_')
        {
            arbore_nod* operatie, * a, * b, * inmultire, * semn;
            operatie = new arbore_nod;
            inmultire = new arbore_nod;
            semn = new arbore_nod;
            token.erase(0, 1);
            operatie->info = token;
            pop(s, a);
            pop(s, b);
            operatie->st = b;
            operatie->dr = a;
            inmultire->info = "*";
            inmultire->dr = operatie;
            inmultire->st = semn;
            semn->info = "-1";
            semn->dr = NULL;
            semn->st = NULL;
            push(s, inmultire);
        }
        else if (esteOperand(token[0]))
        {
            arbore_nod* operand;
            operand = new arbore_nod;
            operand->info = token;
            operand->st = NULL;
            operand->dr = NULL;
            push(s, operand);
        }
    }
}

void inordine(arbore_nod* a, string& infixat, int& h)
{
    if (a != NULL)
    {
        if (lungime(a->info) > 1 && (esteTrigonometrie(a->info) > 0))
        {
            if (a->info[0] == '-')
            {
                infixat.insert(infixat.end(), '(');
            }
            infixat.append(a->info);
            infixat.insert(infixat.end(), '(');
            inordine(a->st, infixat, h);
            infixat.insert(infixat.end(), ')');
            if (a->info[0] == '-')
            {
                infixat.insert(infixat.end(), ')');
            }
        }
        else if (lungime(a->info) > 1 && (esteRadical(a->info) > 0))
        {
            infixat.insert(infixat.end(), '(');
            inordine(a->st, infixat, h);
            infixat.insert(infixat.end(), ')');
            infixat.append(a->info);
            infixat.insert(infixat.end(), '(');
            inordine(a->dr, infixat, h);
            infixat.insert(infixat.end(), ')');
        }
        else if (lungime(a->info) > 1 && (esteLogaritm(a->info) > 0))
        {
            infixat.append(a->info);
            infixat.insert(infixat.end(), '(');
            infixat.insert(infixat.end(), '(');
            inordine(a->st, infixat, h);
            infixat.insert(infixat.end(), ')');
            infixat.insert(infixat.end(), '(');
            inordine(a->dr, infixat, h);
            infixat.insert(infixat.end(), ')');
            infixat.insert(infixat.end(), ')');
        }
        else
        {
            if (lungime(a->info) == 1 && esteOperator_aritmetic(a->info))
            {
                if (prioritateOperator(a->info[0]) == 2)
                {
                    if (esteOperand(a->st->info[0]) && esteOperand(a->dr->info[0]))
                    {
                        int ok;
                    }
                    else
                    {
                        infixat.insert(infixat.end(), '(');
                    }
                }
                else
                    infixat.insert(infixat.end(), '(');

            }
            inordine(a->st, infixat, h);
            if (a->info[0] == '_')
            {
                infixat.insert(infixat.end(), '(');
                infixat.append(a->info);
                infixat.insert(infixat.end(), ')');
            }
            else
                infixat.append(a->info);
            inordine(a->dr, infixat, h);
            if (lungime(a->info) == 1 && esteOperator_aritmetic(a->info))
            {
                if (prioritateOperator(a->info[0]) == 2)
                {
                    if (esteOperand(a->st->info[0]) && esteOperand(a->dr->info[0]))
                    {
                        int ok;
                    }
                    else
                    {
                        infixat.insert(infixat.end(), ')');
                    }
                }
                else
                    infixat.insert(infixat.end(), ')');
            }
        }
    }
}
void inordine_final(arbore_nod* a, string& infixat)
{
    int h = 0;
    inordine(a, infixat, h);
}

void copiaza_arbore(arbore_nod* a, arbore_nod*& b)
{
    if (a != NULL)
    {
        b->info = a->info;
        if (a->st != NULL)
        {
            arbore_nod* x;
            x = new arbore_nod;
            b->st = x;
            copiaza_arbore(a->st, x);
        }
        else
            b->st = NULL;
        if (a->dr != NULL)
        {
            arbore_nod* x;
            x = new arbore_nod;
            b->dr = x;
            copiaza_arbore(a->dr, x);
        }
        else
            b->dr = NULL;
    }
}

void derivare(arbore_nod*& a)
{
    if (a == NULL)
        return;
    if (suntOperatori(a->info[0]))
    {
        switch (a->info[0])
        {
        case '-':
            derivare(a->st);
            derivare(a->dr);
            break;
        case '+':
            derivare(a->st);
            derivare(a->dr);
            break;
        case '*':
            if (isdigit(a->st->info[0]))
                derivare(a->dr);
            else if (isdigit(a->dr->info[0]))
                derivare(a->st);
            else
            {
                arbore_nod* op1, * op2, * fiul1, * fiul2, * fiul3, * fiul4;
                op1 = new arbore_nod;
                op2 = new arbore_nod;
                fiul1 = new arbore_nod;
                fiul2 = new arbore_nod;
                fiul3 = new arbore_nod;
                fiul4 = new arbore_nod;
                op1->info = '*';
                op2->info = '*';
                op1->st = fiul1;
                op1->dr = fiul2;
                op2->st = fiul3;
                op2->dr = fiul4;
                copiaza_arbore(a->st, fiul1);
                copiaza_arbore(a->dr, fiul2);
                copiaza_arbore(a->st, fiul3);
                copiaza_arbore(a->dr, fiul4);
                a->info = "+";
                a->st = op1;
                a->dr = op2;
                derivare(fiul1);
                derivare(fiul4);
            }
            break;
        case '/':
            if (1 == 1)
            {
                arbore_nod* sub, * pwr, * mul1, * mul2;
                arbore_nod* fiu1, * fiu2, * fiu3, * fiu4, * fiu5, * fiu6;
                sub = new arbore_nod;
                pwr = new arbore_nod;
                mul1 = new arbore_nod;
                mul2 = new arbore_nod;
                fiu1 = new arbore_nod;
                fiu2 = new arbore_nod;
                fiu3 = new arbore_nod;
                fiu4 = new arbore_nod;
                fiu5 = new arbore_nod;
                fiu6 = new arbore_nod;
                sub->info = "-";
                pwr->info = "^";
                mul1->info = "*";
                mul2->info = "*";
                sub->st = mul1;
                sub->dr = mul2;
                mul1->st = fiu1;
                mul1->dr = fiu2;
                mul2->st = fiu3;
                mul2->dr = fiu4;
                pwr->st = fiu5;
                pwr->dr = fiu6;
                copiaza_arbore(a->st, fiu1);
                copiaza_arbore(a->dr, fiu2);
                copiaza_arbore(a->st, fiu3);
                copiaza_arbore(a->dr, fiu4);
                copiaza_arbore(a->dr, fiu5);
                fiu6->info = "2";
                fiu6->st = NULL;
                fiu6->dr = NULL;
                a->st = sub;
                a->dr = pwr;
                derivare(mul1->st);
                derivare(mul2->dr);
            }
            break;
        case '^':
            if (1 == 1)
            {
                if ((a->st->info[0] == 'e' or isdigit(a->st->info[0])))
                {
                    arbore_nod* mul1, * mul2, * pwr, * loga, * f1, * f2, * f3, * f4, * f5;
                    mul1 = new arbore_nod;
                    mul2 = new arbore_nod;
                    pwr = new arbore_nod;
                    loga = new arbore_nod;
                    f1 = new arbore_nod;
                    f2 = new arbore_nod;
                    f3 = new arbore_nod;
                    f4 = new arbore_nod;
                    f5 = new arbore_nod;
                    mul1->info = "*";
                    mul2->info = "*";
                    pwr->info = "^";
                    loga->info = "log";
                    mul1->st = mul2;
                    mul1->dr = f4;
                    mul2->st = pwr;
                    mul2->dr = loga;
                    pwr->st = f1;
                    pwr->dr = f2;
                    loga->st = f5;
                    loga->dr = f3;
                    f5->info = "e";
                    f5->st = NULL;
                    f5->dr = NULL;
                    copiaza_arbore(a->st, f1);
                    copiaza_arbore(a->dr, f2);
                    copiaza_arbore(a->st, f3);
                    copiaza_arbore(a->dr, f4);
                    *a = *mul1;
                    derivare(f4);
                }
                else if (a->st->info[0] != 'e' and !isdigit(a->st->info[0]))
                {
                    arbore_nod* f1, * f2, * f3, * f4, * pwr1, * mul, * scad;
                    f1 = new arbore_nod;
                    f2 = new arbore_nod;
                    f3 = new arbore_nod;
                    f4 = new arbore_nod;
                    pwr1 = new arbore_nod;
                    mul = new arbore_nod;
                    scad = new arbore_nod;
                    mul->info = '*';
                    mul->st = f1;
                    mul->dr = pwr1;
                    pwr1->info = '^';
                    pwr1->st = f2;
                    pwr1->dr = scad;
                    scad->info = "-";
                    scad->st = f3;
                    scad->dr = f4;
                    copiaza_arbore(a->dr, f1);
                    copiaza_arbore(a->st, f2);
                    copiaza_arbore(a->dr, f3);
                    f4->info = "1";
                    f4->dr = NULL;
                    f4->st = NULL;
                    *a = *mul;
                    if (esteOperator_aritmetic(pwr1->st->info))
                    {
                        arbore_nod* m1, * com1;
                        com1 = new arbore_nod;
                        m1 = new arbore_nod;
                        m1->info = '*';
                        m1->st = mul;
                        m1->dr = com1;
                        copiaza_arbore(pwr1->st, com1);
                        *a = *m1;
                        derivare(com1);
                    }
                }
            }
            break;
        }
    }
    else if (esteTrigonometrie(a->info))
    {

        int trig = esteTrigonometrie(a->info);
        switch (trig)
        {
        case 1:   //sin
            if (1 == 1)
            {
                a->info = "cos";
                if (esteOperator_aritmetic(a->st->info))
                {
                    arbore_nod* m1, * op1, * op2;
                    m1 = new arbore_nod;
                    op1 = new arbore_nod;
                    op2 = new arbore_nod;

                    m1->info = "*";
                    m1->st = op1;
                    m1->dr = op2;

                    copiaza_arbore(a, op1);
                    copiaza_arbore(a->st, op2);

                    *a = *m1;
                    derivare(op2);
                }
                break;
            }
        case 2:     //cos
            if (1 == 1)
            {
                arbore_nod* mul, * semn, * f1;
                mul = new arbore_nod;
                semn = new arbore_nod;
                f1 = new arbore_nod;
                mul->info = "*";
                semn->info = "-1";
                semn->st = NULL;
                semn->dr = NULL;
                mul->st = semn;
                mul->dr = f1;
                copiaza_arbore(a, f1);
                *a = *mul;
                a = a->dr;
                a->info = "sin";
                if (esteOperator_aritmetic(a->st->info))
                {
                    arbore_nod* m1, * op1, * op2;
                    m1 = new arbore_nod;
                    op1 = new arbore_nod;
                    op2 = new arbore_nod;

                    m1->info = "*";
                    m1->st = op1;
                    m1->dr = op2;

                    copiaza_arbore(a, op1);
                    copiaza_arbore(a->st, op2);

                    *a = *m1;
                    derivare(op2);
                }

                break;
            }
        case 3:     //tg
            if (1 == 1)
            {
                arbore_nod* impart, * f1, * f2, * f3, * pwr;
                impart = new arbore_nod;
                f1 = new arbore_nod;
                f2 = new arbore_nod;
                f3 = new arbore_nod;
                pwr = new arbore_nod;
                impart->info = "/";
                impart->st = f1;
                impart->dr = pwr;

                pwr->info = "^";
                pwr->st = f2;
                pwr->dr = f3;

                copiaza_arbore(a->st, f1);
                a->info = "cos";
                copiaza_arbore(a, f2);

                f3->info = "2";
                f3->st = NULL;
                f3->dr = NULL;
                *a = *impart;
                derivare(f1);
            }
            break;
        case 4:     //ctg
            if (1 == 1)
            {
                arbore_nod* impart, * f1, * f2, * f3, * pwr, * munu, * mul;
                impart = new arbore_nod;
                f1 = new arbore_nod;
                f2 = new arbore_nod;
                f3 = new arbore_nod;
                munu = new arbore_nod;
                pwr = new arbore_nod;
                mul = new arbore_nod;

                impart->info = "/";
                impart->st = f1;
                impart->dr = pwr;

                pwr->info = "^";
                pwr->st = f2;
                pwr->dr = f3;

                copiaza_arbore(a->st, f1);
                a->info = "sin";
                copiaza_arbore(a, f2);

                f3->info = "2";
                f3->st = NULL;
                f3->dr = NULL;
                *a = *impart;

                mul->info = "*";
                mul->dr = munu;
                mul->st = f1;
                munu->info = "_1";
                munu->st = NULL;
                munu->dr = NULL;
                a->st = mul;

                derivare(f1);
            }
            break;
        case 5:     //arcsin
            if (1 == 1)
            {
                arbore_nod* impart, * f1, * f2, * f3, * f4, * f5, * pwr, * scad, * sqr;
                impart = new arbore_nod;
                f1 = new arbore_nod;
                f2 = new arbore_nod;
                f3 = new arbore_nod;
                f4 = new arbore_nod;
                f5 = new arbore_nod;
                pwr = new arbore_nod;
                sqr = new arbore_nod;
                scad = new arbore_nod;

                impart->info = "/";
                sqr->info = "sqrt";
                scad->info = "-";
                pwr->info = "^";
                f5->info = "2";

                impart->st = f1;
                impart->dr = sqr;
                sqr->st = f5;
                sqr->dr = scad;
                scad->st = f2;
                f5->st = NULL;
                f5->dr = NULL;
                scad->dr = pwr;
                pwr->st = f3;
                pwr->dr = f4;

                copiaza_arbore(a->st, f1);
                f2->info = "1";
                f2->st = NULL;
                f2->dr = NULL;
                copiaza_arbore(a->st, f3);
                f4->info = "2";
                f4->st = NULL;
                f4->dr = NULL;

                *a = *impart;

                derivare(f1);

            }
            break;
        case 6:     //arccos
            if (1 == 1)
            {
                arbore_nod* impart, * f1, * f2, * f3, * f4, * f5, * pwr, * scad, * sqr, * munu, * mul;
                impart = new arbore_nod;
                f1 = new arbore_nod;
                f2 = new arbore_nod;
                f3 = new arbore_nod;
                f4 = new arbore_nod;
                f5 = new arbore_nod;
                pwr = new arbore_nod;
                sqr = new arbore_nod;
                scad = new arbore_nod;
                munu = new arbore_nod;
                mul = new arbore_nod;

                impart->info = "/";
                sqr->info = "sqrt";
                scad->info = "-";
                pwr->info = "^";
                f5->info = "2";

                impart->st = f1;
                impart->dr = sqr;
                sqr->st = f5;
                sqr->dr = scad;
                f5->st = NULL;
                f5->dr = NULL;
                scad->st = f2;
                scad->dr = pwr;
                pwr->st = f3;
                pwr->dr = f4;

                copiaza_arbore(a->st, f1);
                f2->info = "1";
                f2->st = NULL;
                f2->dr = NULL;
                copiaza_arbore(a->st, f3);
                f4->info = "2";
                f4->st = NULL;
                f4->dr = NULL;

                *a = *impart;

                mul->info = "*";
                mul->dr = munu;
                mul->st = f1;
                munu->info = "_1";
                munu->st = NULL;
                munu->dr = NULL;
                a->st = mul;
                derivare(f1);

            }
            break;
        case 7:     //arctg
            if (1 == 1)
            {
                arbore_nod* impart, * f1, * f2, * f3, * f4, * pwr, * adun;
                impart = new arbore_nod;
                f1 = new arbore_nod;
                f2 = new arbore_nod;
                f3 = new arbore_nod;
                f4 = new arbore_nod;
                pwr = new arbore_nod;
                adun = new arbore_nod;

                impart->info = "/";
                adun->info = "+";
                pwr->info = "^";

                impart->st = f1;
                impart->dr = adun;
                adun->st = pwr;
                adun->dr = f4;
                pwr->st = f2;
                pwr->dr = f3;

                copiaza_arbore(a->st, f1);
                copiaza_arbore(a->st, f2);
                f3->info = "2";
                f3->st = NULL;
                f3->dr = NULL;
                f4->info = "1";
                f4->st = NULL;
                f4->dr = NULL;

                *a = *impart;

                derivare(f1);

            }
            break;
        case 8:     //arcctg
            if (1 == 1)
            {
                arbore_nod* impart, * f1, * f2, * f3, * f4, * pwr, * adun, * munu, * mul;
                impart = new arbore_nod;
                f1 = new arbore_nod;
                f2 = new arbore_nod;
                f3 = new arbore_nod;
                f4 = new arbore_nod;
                pwr = new arbore_nod;
                adun = new arbore_nod;
                munu = new arbore_nod;
                mul = new arbore_nod;

                impart->info = "/";
                adun->info = "+";
                pwr->info = "^";
                impart->st = f1;
                impart->dr = adun;
                adun->st = pwr;
                adun->dr = f4;
                pwr->st = f2;
                pwr->dr = f3;

                copiaza_arbore(a->st, f1);
                copiaza_arbore(a->st, f2);
                f3->info = "2";
                f3->st = NULL;
                f3->dr = NULL;
                f4->info = "1";
                f4->st = NULL;
                f4->dr = NULL;

                *a = *impart;

                mul->info = "*";
                mul->dr = munu;
                mul->st = f1;
                munu->info = "_1";
                munu->st = NULL;
                munu->dr = NULL;
                a->st = mul;
                derivare(f1);

            }
            break;
        }
    }
    else if (esteRadical(a->info))
    {
        if (1 == 1)
        {
            arbore_nod* impart, * f1, * f2, * f3, * f4, * f5, * f6, * pwr, * scad, * mul, * rad;
            impart = new arbore_nod;
            f1 = new arbore_nod;
            f2 = new arbore_nod;
            f3 = new arbore_nod;
            f4 = new arbore_nod;
            f5 = new arbore_nod;
            f6 = new arbore_nod;
            pwr = new arbore_nod;
            scad = new arbore_nod;
            mul = new arbore_nod;
            rad = new arbore_nod;

            impart->info = "/";
            scad->info = "-";
            pwr->info = "^";
            mul->info = "*";
            rad->info = "sqrt";

            impart->st = f1;
            impart->dr = mul;
            mul->st = f2;
            mul->dr = rad;
            rad->st = f3;
            rad->dr = pwr;
            pwr->st = f4;
            pwr->dr = scad;
            scad->st = f5;
            scad->dr = f6;

            copiaza_arbore(a->dr, f1);
            copiaza_arbore(a->st, f2);
            copiaza_arbore(a->st, f3);
            copiaza_arbore(a->dr, f4);
            copiaza_arbore(a->st, f5);

            f6->info = "1";
            f6->st = NULL;
            f6->dr = NULL;

            *a = *impart;

            derivare(f1);
        }
    }
    else if (esteLogaritm(a->info))
    {
        if (1 == 1)
        {
            arbore_nod* impart, * f1, * f2, * f3, * f4, * mul, * log_e;
            impart = new arbore_nod;
            f1 = new arbore_nod;
            f2 = new arbore_nod;
            f3 = new arbore_nod;
            f4 = new arbore_nod;
            mul = new arbore_nod;
            log_e = new arbore_nod;

            impart->info = "/";
            mul->info = "*";
            log_e->info = "log";
            f3->info = "e";

            f3->st = NULL;
            f3->dr = NULL;
            impart->st = f1;
            impart->dr = mul;
            mul->st = f2;
            mul->dr = log_e;
            log_e->st = f3;
            log_e->dr = f4;

            copiaza_arbore(a->dr, f1);
            copiaza_arbore(a->dr, f2);
            copiaza_arbore(a->st, f4);

            *a = *impart;
            derivare(f1);
        }
    }
    else if (esteOperand(a->info[0]))
    {
        if (isdigit(a->info[0]) or (a->info[0] == '_' && isdigit(a->info[1])) or a->info[0] != 'x')
            a->info = "0";
        else if (a->info[0] == '_')
        {
            a->info = "-1";
        }
        else
            a->info = "1";
    }
}

void simplificare(arbore_nod*& a, arbore_nod* parinte)
{
    if (a != NULL)
    {
        if (a->info[0] == '+' or (a->info[0] == '-' and a->info[1] == NULL))
        {
            if (a->st->info[0] == '0')
            {
                copiaza_arbore(a->dr, a);
                simplificare(parinte, parinte);
            }
            else if (a->dr->info[0] == '0')
            {
                copiaza_arbore(a->st, a);
                simplificare(parinte, parinte);
            }
            else if (a->dr->info[0] == '1' and isdigit(a->st->info[0]))
            {
                int x;
                x = (int)(a->st->info[0]);
                switch (a->info[0])
                {
                case '+':
                    x++;
                    break;
                case '-':
                    x--;
                    break;
                }
                a->info[0] = (char)x;
                a->st = NULL;
                a->dr = NULL;
                simplificare(parinte, parinte);
            }
            else
            {
                simplificare(a->st, a);
                simplificare(a->dr, a);
            }
        }
        else if (a->info[0] == '*')
        {
            if (a->st->info[0] == '0')
            {
                copiaza_arbore(a->st, a);
                simplificare(parinte, parinte);
            }
            else if (a->dr->info[0] == '0')
            {
                copiaza_arbore(a->dr, a);
                simplificare(parinte, parinte);
            }
            else if (a->st->info[0] == '1')
            {
                copiaza_arbore(a->dr, a);
                simplificare(parinte, parinte);
            }
            else if (a->dr->info[0] == '1')
            {
                copiaza_arbore(a->st, a);
                simplificare(parinte, parinte);
            }
            else if (a->st->info[0] == '_' and a->st->info[1] == '1' and a->dr->info[0] == '*' and a->dr->st->info[0] == '_' and a->dr->st->info[1] == '1')
            {
                copiaza_arbore(a->dr->dr, a);
                simplificare(parinte, parinte);
            }
            else
            {
                simplificare(a->st, a);
                simplificare(a->dr, a);
            }
        }
        else if (a->info[0] == '^')
        {
            if (a->dr->info[0] == '0' or a->st->info[0] == '1')
            {
                a->info = "1";
                a->st = NULL;
                a->dr = NULL;
                simplificare(parinte, parinte);
            }
            else if (a->st->info[0] == '0')
            {
                a->info = "0";
                a->st = NULL;
                a->dr = NULL;
                simplificare(parinte, parinte);
            }
            else if (a->dr->info[0] == '1')
            {
                copiaza_arbore(a->st, a);
                simplificare(parinte, parinte);
            }
            else
            {
                simplificare(a->st, a);
                simplificare(a->dr, a);
            }
        }
        else if (a->info[0] == '/')
        {
            if (a->st->info[0] == '0')
            {
                a->info = "0";
                a->dr = NULL;
                a->st = NULL;
                simplificare(parinte, parinte);
            }
            else if (a->dr->info[0] == '1')
            {
                copiaza_arbore(a->dr, a);
                simplificare(parinte, parinte);
            }
            else
            {
                simplificare(a->st, a);
                simplificare(a->dr, a);
            }
        }
        else if (esteLogaritm(a->info))
        {
            if (a->st->info[0] == 'e' and a->dr->info[0] == 'e')
            {
                a->info = "1";
                a->st = NULL;
                a->dr = NULL;
                simplificare(parinte, parinte);
            }
            else
            {
                simplificare(a->st, a);
                simplificare(a->dr, a);
            }
        }
        else
        {
            simplificare(a->st, a);
            simplificare(a->dr, a);
        }
    }
}

void transf(string infixat1, string& infixat2)
{
    infixat2.clear();
    for (string::iterator i = infixat1.begin(); i != infixat1.end(); i++)
    {
        if (*i == '_')
            infixat2.insert(infixat2.end(), '-');
        else
            infixat2.insert(infixat2.end(), *i);
    }
}

void buton(int x1, int y1, int x2, int y2, char sir[])
{
    setcolor(CYAN);
    rectangle(x1, y1, x2, y2);
    setfillstyle(SOLID_FILL, BLACK);
    floodfill(x1 + 1, y1 + 1, CYAN);
    settextstyle(10, 0, 2);
    setcolor(WHITE);
    outtextxy(x1 + 10, y1 + 10, sir);
}

void buton_apasat(int x1, int y1, int x2, int y2, char sir[])
{
    setcolor(CYAN);
    rectangle(x1, y1, x2, y2);
    setfillstyle(SOLID_FILL, WHITE);
    floodfill(x1 + 1, y1 + 1, CYAN);
    settextstyle(10, 0, 2);
    setcolor(BLACK);
    setbkcolor(WHITE);
    outtextxy(x1 + 10, y1 + 10, sir);
    setbkcolor(BLACK);
}

void butoane(int c)
{
    if (c)
    {
        buton(415, 580, 870, 620, "Deseneaza arborele derivatei I");
        buton(415, 630, 870, 670, "Deseneaza arborele derivatei a-II-a");
        buton(930, 160, 1185, 200, "Introdu o functie");
        buton(930, 205, 1185, 245, "Deriveaza din fisier");
        buton(930, 250, 1185, 290, "Limba");
        buton(1080, 630, 1180, 668, "Iesi");
        buton(1080, 580, 1180, 620, "Info");
        buton(930, 295, 1185, 335, "Model profesor");
        buton(930, 340, 1185, 380, "Model background");
    }
    else
    {
        buton(415, 580, 870, 620, "Draw the derivative tree I");
        buton(415, 630, 870, 670, "Draw the second derivative tree");
        buton(930, 160, 1185, 200, "Input a function");
        buton(930, 205, 1185, 245, "Derive from file");
        buton(930, 250, 1185, 290, "Language");
        buton(1080, 630, 1180, 668, "Exit");
        buton(1080, 580, 1180, 620, "Info");
        buton(930, 295, 1185, 335, "Professor model");
        buton(930, 340, 1185, 380, "Background model");

    }

}

int main()
{
    int c = 1;
    int okpoza = 0, okprofesor = 1;
    int fereastra;
    fereastra = initwindow(1200, 700, "DERIVARE");

    readimagefile("deriva1.jpg", 0, 0, 1200, 700);


    settextstyle(8, 0, 1);
    setcolor(WHITE);

    // Updated to remove the turquoise border and use #1d4230 (RGB(29, 66, 48))
    setfillstyle(SOLID_FILL, COLOR(29, 66, 48));  // Dark green fill color
    bar(142, 117, 600, 250);  // Replacing the rectangle with a filled bar for the background


    butoane(c);


start:
    // Rest of your code logic remains unchanged


    bool ok = true;
    do
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            int x = mousex();
            int y = mousey();

            if (x > 930 && x < 1185 && y > 295 && y < 335)
            {
                if (okprofesor == 1 && okpoza == 0)
                {
                    readimagefile("deriva3.jpg", 0, 0, 1200, 700);
                    okprofesor = 0;
                    butoane(c);
                }
                else
                {
                    if (okprofesor == 1 && okpoza == 1)
                    {
                        readimagefile("deriva4.jpg", 0, 0, 1200, 700);
                        okprofesor = 0;
                        butoane(c);
                    }
                    else
                    {
                        if (okprofesor == 0 && okpoza == 1)
                        {
                            readimagefile("deriva.jpg", 0, 0, 1200, 700);
                            okprofesor = 1;
                            butoane(c);
                        }
                        else
                        {
                            if (okprofesor == 0 && okpoza == 0)
                            {
                                readimagefile("deriva1.jpg", 0, 0, 1200, 700);
                                okprofesor = 1;
                                butoane(c);
                            }
                        }
                    }
                }
            }

            if (x > 930 && x < 1185 && y > 340 && y < 380)
            {
                if (okprofesor == 1 && okpoza == 0)
                {
                    readimagefile("deriva.jpg", 0, 0, 1200, 700);
                    okpoza = 1;
                    butoane(c);
                }
                else
                {
                    if (okprofesor == 1 && okpoza == 1)
                    {
                        readimagefile("deriva1.jpg", 0, 0, 1200, 700);
                        okpoza = 0;
                        butoane(c);
                    }
                    else
                    {
                        if (okprofesor == 0 && okpoza == 1)
                        {
                            readimagefile("deriva4.jpg", 0, 0, 1200, 700);
                            okpoza = 0;
                            butoane(c);
                        }
                        else
                        {
                            if (okprofesor == 0 && okpoza == 0)
                            {
                                readimagefile("deriva3.jpg", 0, 0, 1200, 700);
                                okpoza = 1;
                                butoane(c);
                            }
                        }
                    }
                }
            }
            if (x > 930 && x < 1185 && y > 250 && y < 290) { c = 1 - c; butoane(c); }
            if (x > 930 && x < 1185 && y > 160 && y < 200)
            {
                buton_apasat(930, 160, 1185, 200, "Introdu o functie");
                ok = false;

                // Fill only the input area with #1d4230 (Dark Green) instead of BLACK
                setfillstyle(SOLID_FILL, COLOR(29, 66, 48));
                bar(142, 117, 600, 250); // Replace floodfill with bar to fill only the black rectangle

                setbkcolor(COLOR(29, 66, 48)); // Fundal verde închis
                setcolor(WHITE);               // Culoarea textului alb
                outtextxy(150, 120, "f(x)= "); // Textul


                string infixat, final_infixat, postfixat;
                int i = 0, x1 = 210, y1 = 120, cheie, x, y;
                char ch[2], final_char[256], final_char2[256];
                ch[1] = NULL;
                do
                {
                    ch[0] = (char)getch();
                    cheie = (int)ch[0];
                    if (cheie == 8)
                    {
                        if (lungime(infixat))
                            infixat.pop_back();
                        x1 -= 12;


                    }
                    else if (isalnum(ch[0]) || suntOperatori(ch[0]) || ch[0] == ')' || ch[0] == '(' || ch[0] == '.' || ch[0] == '_' || ch[0] == ' ')
                    {
                        infixat += ch[0];
                        outtextxy(x1, y1, ch);
                        x1 += 12;
                    }
                    if (x1 > 1050)
                        moveto(150, y1 + 20), x1 = 150, y1 = y1 + 20;

                } while (cheie != 13);
                cout << "infixat: " << infixat << " and postfixat: ";
                transformaExpresie(infixat, postfixat);
                cout << postfixat << endl;
                rez.functia = infixat;
                creeaza_arbore(S, postfixat);
                a = S.varf->info;
                derivare(a);
                a = S.varf->info;
                simplificare(a, a);
                inordine_final(a, final_infixat);
                string final_infixat2;
                transf(final_infixat, final_infixat2);
                rez.derivata_I = final_infixat2;
                strcpy(final_char, final_infixat2.c_str());
                x1 = 230, y1 = y1 + 20;
                outtextxy(150, y1, "f'(x)= ");
                for (int i = 0; i < strlen(final_char); i++)
                {
                    char x[2];
                    x[1] = NULL;
                    x[0] = final_char[i];
                    outtextxy(x1, y1, x);
                    x1 = x1 + textwidth(x);
                    if (x1 > 1050)
                        moveto(150, y1 + 20), x1 = 150, y1 = y1 + 20;
                }

                string postfixat2;
                transformaExpresie(final_infixat, postfixat2);
                creeaza_arbore(S2, postfixat2);
                a2 = S2.varf->info;
                derivare(a2);
                a2 = S2.varf->info;
                simplificare(a2, a2);
                string final_ind, final_ind2;
                inordine_final(a2, final_ind);
                transf(final_ind, final_ind2);
                strcpy(final_char2, final_ind2.c_str());
                rez.derivata_IIa = final_ind2;
                x1 = 230, y1 = y1 + 20;

                outtextxy(150, y1, "f''(x)= ");
                for (int i = 0; i < strlen(final_char2); i++)
                {
                    char xx[2];
                    xx[1] = NULL;
                    xx[0] = final_char2[i];
                    outtextxy(x1, y1, xx);
                    x1 = x1 + textwidth(xx);
                    if (x1 > 1050)
                        moveto(150, y1 + 20), x1 = 150, y1 = y1 + 20;
                }
                butoane(c);
                goto start;
            }
            if (x > 930 and x < 1185 and y>205 and y < 245)
            {
                ok = false;
                setfillstyle(SOLID_FILL, COLOR(29, 66, 48));


                setcolor(WHITE);
                outtextxy(150, 120, "f(x)= ");

                string infixat, final_infixat, postfixat;
                int i = 0, x1 = 210, y1 = 120, cheie, x, y;
                char ch[2], final_char[256], final_char2[256], infixat_ch[256];
                getline(in, infixat);
                in.close();
                strcpy(infixat_ch, infixat.c_str());
                for (int i = 0; i < strlen(infixat_ch); i++)
                {
                    char x[2];
                    x[1] = NULL;
                    x[0] = infixat[i];
                    outtextxy(x1, y1, x);
                    x1 = x1 + textwidth(x);
                    if (x1 > 1050)
                        moveto(150, y1 + 20), x1 = 150, y1 = y1 + 20;
                }
                cout << "infixat: " << infixat << " and postfixat: ";
                transformaExpresie(infixat, postfixat);
                cout << postfixat << endl;
                rez.functia = infixat;
                creeaza_arbore(S, postfixat);
                a = S.varf->info;
                derivare(a);
                a = S.varf->info;
                simplificare(a, a);
                inordine_final(a, final_infixat);
                cout << final_infixat;
                string final_infixat2;
                transf(final_infixat, final_infixat2);
                rez.derivata_I = final_infixat2;
                strcpy(final_char, final_infixat2.c_str());
                x1 = 230, y1 = y1 + 20;
                outtextxy(150, y1, "f'(x)= ");
                for (int i = 0; i < strlen(final_char); i++)
                {
                    char x[2];
                    x[1] = NULL;
                    x[0] = final_char[i];
                    outtextxy(x1, y1, x);
                    x1 = x1 + textwidth(x);
                    if (x1 > 1050)
                        moveto(150, y1 + 20), x1 = 150, y1 = y1 + 20;
                }
                ///derivata a doua
                string postfixat2;
                transformaExpresie(final_infixat, postfixat2);
                creeaza_arbore(S2, postfixat2);
                a2 = S2.varf->info;
                derivare(a2);
                a2 = S2.varf->info;
                simplificare(a2, a2);
                string final_ind, final_ind2;
                inordine_final(a2, final_ind);
                rez.derivata_IIa = final_ind;
                transf(final_ind, final_ind2);
                strcpy(final_char2, final_ind2.c_str());
                x1 = 230, y1 = y1 + 20;
                outtextxy(150, y1, "f''(x)= ");
                for (int i = 0; i < strlen(final_char2); i++)
                {
                    char xx[2];
                    xx[1] = NULL;
                    xx[0] = final_char2[i];
                    outtextxy(x1, y1, xx);
                    x1 = x1 + textwidth(xx);
                    if (x1 > 1050)
                        moveto(150, y1 + 20), x1 = 150, y1 = y1 + 20;
                }
                butoane(c);
                goto start;
            }
            if (x > 710 and x < 950 and y>540 and y < 580)
            {
                out << "f(x)=" << rez.functia << endl;
                out << "f'(x)=" << rez.derivata_I << endl;
                out << "f''(x)=" << rez.derivata_IIa << endl;
                out.close();
                butoane(c);
            }
            if (x > 415 and x < 870 and y>580 and y < 620)
            {
                a = S.varf->info;
                deseneaza(a);
                setcurrentwindow(fereastra);

                butoane(c);
            }
            if (x > 415 and x < 870 and y>630 and y < 670)
            {
                a2 = S2.varf->info;
                deseneaza(a2);
                setcurrentwindow(fereastra);
                butoane(c);
            }
            if (x > 1080 && x < 1180 && y > 580 && y < 620)
            {
                buton_apasat(1080, 580, 1180, 620, "Info");
                initwindow(800, 549, "Informatii", 150, 150, false, false);

                // Setăm fontul inițial pentru titlu
                settextstyle(10, 0, 5);
                outtextxy(200, 10, "Despre Aplicatie");

                // Setăm fontul inițial pentru text explicativ
                settextstyle(8, 0, 1);
                outtextxy(30, 50, "- Aplicatia permite calcularea derivatelor unei functii.");
                outtextxy(30, 70, "- Separatorul zecimal acceptat este punctul '.'");
                outtextxy(30, 100, "- Variabila utilizata pentru derivare este 'x'.");
                outtextxy(30, 130, "- Operatii si functii acceptate:");
                outtextxy(30, 150, "  '+', '-', '*', '/', '^', 'sin', 'cos', 'tg', 'ctg',");
                outtextxy(30, 170, "  'arcsin', 'arccos', 'arctg', 'arcctg', '(n)sqrt',");
                outtextxy(30, 190, "  '(baza)log'.");
                outtextxy(30, 220, "- Functia este preluata din fisierul 'rezolvare.txt'.");
                outtextxy(30, 240, "- Rezultatele vor fi salvate in 'afisare_derivata.txt'.");

                // Adăugăm secțiunea STUDENTI
                settextstyle(10, 0, 2);
                outtextxy(25, 300, "STUDENTI:");
                settextstyle(10, 0, 1);
                outtextxy(25, 340, "COSTAN CALIN");
                outtextxy(25, 360, "CANUR ANDREEA");

                // Adăugăm secțiunea PROFESOR
                settextstyle(10, 0, 2);
                outtextxy(235, 300, "PROFESOR:");
                settextstyle(10, 0, 1);
                outtextxy(235, 340, "BOGDAN PATRUT");

                getch();
                closegraph(CURRENT_WINDOW);
                setcurrentwindow(fereastra);
                butoane(c);
            }




            if (x > 1080 and x < 1180 and y>630 and y < 668)
            {
                ok = false;
                closegraph();
            }
        }
    } while (ok);
    return 0;
}
