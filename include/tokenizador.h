//tokenizador.h
//alejandro garcia gil 77042008N
#ifndef _TOKENIZADOR_
#define _TOKENIZADOR_


#include <iostream>
#include <cctype>//tolower minus
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <list>
#include <set>
#include <unordered_set> //no se si la llegare a utilizar
#include <vector>
#include <algorithm>
#include <limits>
#include <fcntl.h>
#include <unistd.h>
//opcionales para mapeo y gestion de archivos
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
//para TokenizarDirectorio
#include <dirent.h>
#include <unordered_map>//tabla conversion iso/utf
using namespace std;

class Tokenizador{
    friend ostream& operator<<(ostream&, const Tokenizador&);
    public:
        Tokenizador(const string& delimitadoresPalabra, const bool& kcasosEspeciales,const bool& minuscSinAcentos);
        Tokenizador (const Tokenizador&);
        Tokenizador(); //,;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t@
        ~Tokenizador();
        Tokenizador& operator= (const Tokenizador&);
        void Tokenizar (const string& str, list<string>& tokens) const;
        bool Tokenizar (const string& i, const string& f) const;
        bool Tokenizar (const string & i) const;
        bool TokenizarListaFicheros (const string& i) const;
        bool TokenizarDirectorio (const string& i) const;
        void DelimitadoresPalabra(const string& nuevoDelimiters);
        void AnyadirDelimitadoresPalabra(const string& nuevoDelimiters);
        string DelimitadoresPalabra() const;
        void CasosEspeciales (const bool& nuevoCasosEspeciales);
        bool CasosEspeciales () const;
        string PasarAminuscSinAcentos (const string&) const;
        void PasarAminuscSinAcentos (const bool& nuevoPasarAminuscSinAcentos);
        bool PasarAminuscSinAcentos () const;
        bool Check(const char& preToken, const string& str, const string& delimitadores, const string& delimitadoresEsp) const;
        
        
    private:
        string delimiters;
        bool casosEspeciales;
        bool pasarAminuscSinAcentos;
};

#endif