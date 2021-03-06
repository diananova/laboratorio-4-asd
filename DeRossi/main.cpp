/////////////////////////////////////////////////////////////////////
//
// Laboratorio 4: calcolo di semplici espressioni numeriche
//                mediante pile e code
//
// vengono accettati dall'esterno solo numeri positivi.
// Per trattare anche numeri negativi occorre modificare
// l'analizzatore lessicale.
//
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

#include "token.h"
#include "queue.h"
#include "stack.h"

using std::cout;
using std::cin;
using std::string;
using std::istringstream;
using std::ostringstream;

/////////////////////////////////////////////////////////////////////
// Funzioni principali: leggi() e calcola()
/////////////////////////////////////////////////////////////////////

// Estrae uno dopo l'altro i token dalla stringa "str", inserendoli via via nella coda "codaToken"
bool leggi( const string &str, queue::Queue &codaToken) {
    token tk;
    string strm = str;
    int insertToken = 0;
    codaToken = queue::EMPTYQUEUE;
    while(prossimoToken(strm, tk)){
        insertToken++;
        queue::enqueue(tk, codaToken);
    }
    if(tk.k == SCONOSCIUTO){
        return false;
    }
    return insertToken >= 5;
}

// Estrae uno dopo l'altro i token dalla coda, inserendoli via via sullo stack.
// Appena si incontra un token PARENTESI CHIUSA, quello segnala la fine
// di una sottoespressione; allora tiriamo giu' dallo stack gli ultimi cinque token inseriti.
// I token estratti dovrebbero essere esattamente, nell'ordine:
// un ")", un numero, un operatore aritmetico, un altro
// numero, e un "("; se non è così, allora si segnala
// errore sintattico e l'algoritmo termina.
// A questo punto la funzione deve convertire i token di tipo NUMERO in numeri interi,
// eseguire l'operazione aritmetica opportuna, e trasformare il risultato da numero a
// token e inserire quest'ultimo sullo stack.
// Alla fine se non ci sono errori sintattici la funzione deve mettere nella variabile
// risultato il vaore dell'espressione
bool calcola(queue::Queue codaToken, int &risultato) {
    token tk1, tk2;
    stack::Stack s = stack::EMPTYSTACK;
    while (!queue::isEmpty(codaToken)){
        tk1 = queue::dequeue(codaToken);
        if (tk1.k == PARENTESI_CHIUSA){
            tk1 = stack::pop(s);
            if (tk1.k != NUMERO) return 0;
            int a = str2int(tk1.val);
            tk1 = stack::pop(s);
            tk2 = stack::pop(s);
            if (tk2.k != NUMERO){ return 0; }
            switch( tk1.k ){
                case OP_SOMMA:{
                    a = a + str2int(tk2.val);
                    break;
                }
                case OP_SOTTRAZIONE:{
                    a = str2int(tk2.val) - a;
                    break;
                }
                case OP_MOLTIPLICAZIONE:{
                    a = a * str2int(tk2.val);
                    break;
                }
                default: { return 0; }
            }
            if(stack::pop(s).k != PARENTESI_APERTA){ return 0; }
            tk1.val = int2str(a);
            tk1.k = NUMERO;
            stack::push(tk1,s);
        } else { stack::push(tk1, s); }
    }
    tk1 = stack::pop(s);
    risultato = str2int(tk1.val);
    return stack::isEmpty(s);
}



/////////////////////////////////////////////////////////////////////
//                               MAIN
/////////////////////////////////////////////////////////////////////

int main() {

    string s;
    queue::Queue q;
    int r;

    cout << "Inserisci l'espressione che vuoi valutare:" << endl;
    getline(cin, s);

    if (!leggi(s, q)) {
        cout << "errore lessicale\n";
        exit(1);
    }

    if (!calcola(q, r)) {
        cout << "errore sintattico\n";
        exit(1);
    }

    cout << "risultato: " << r << "\n";
    exit(0);
}
