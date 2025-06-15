//77042008N Alejandro Garcia Gil
#ifndef _INDEXADORHASH_
#define _INDEXADORHASH_

#include "indexadorInformacion.h"
#include "tokenizador.h"
#include "stemmer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

class IndexadorHash {
    friend ostream& operator<<(ostream& s, const IndexadorHash& p);

    public:
    //auxiliar
    string getFileName(const std::string &path);
    string trim(const std::string &s);

    IndexadorHash(std::string fichStopWords, std::string delimitadores, bool detectComp, bool minuscSinAcentos, std::string dirIndice, int tStemmer, bool almPosTerm);

    //IndexadorHash(std::string fichStopWords, std::string delimitadores, bool detectComp, bool minuscSinAcentos, std::string dirIndice, int tStemmer,bool almEnDisco, bool almPosTerm);
    IndexadorHash(const string& directorioIndexacion);
    IndexadorHash(const IndexadorHash&);
    ~IndexadorHash();
    IndexadorHash& operator= (const IndexadorHash&);
    void ProcesarFicheroPalParada(const string& filename);
    bool Indexar(const string& ficheroDocumentos);
    bool IndexarDirectorio(const string& dirAIndexar);
    bool GuardarIndexacion() const;
    bool RecuperarIndexacion (const string& directorioIndexacion);
    void ImprimirIndexacion() const;
    bool IndexarPregunta(const string& preg);
    bool DevuelvePregunta(string& preg) const;
    bool DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf) const;
    bool DevuelvePregunta(InformacionPregunta& inf) const;
    void ImprimirIndexacionPregunta(); // {
    void ImprimirPregunta(); // {
    bool Devuelve(const string& word, InformacionTermino& inf) const;
    bool Devuelve(const string& word, const string& nomDoc, InfTermDoc& infDoc) const;
    bool Existe(const string& word) const;
    bool Borra(const string& word);
    bool BorraDoc(const string& nomDoc);
    void VaciarIndiceDocs();
    void VaciarIndicePreg();
    bool Actualiza(const string& word, const InformacionTermino& inf);
    bool Inserta(const string& word, const InformacionTermino& inf);
    int NumPalIndexadas() const;
    string DevolverFichPalParada () const;
    void ListarPalParada() const;
    int NumPalParada() const;
    string DevolverDelimitadores () const;
    bool DevolverCasosEspeciales () const;
    bool DevolverPasarAminuscSinAcentos () const;
    bool DevolverAlmacenarPosTerm () const;
    string DevolverDirIndice () const;
    int DevolverTipoStemming () const;
    bool DevolverAlmEnDisco () const;
    void ListarInfColeccDocs() const;
    void ListarTerminos() const;
    bool ListarTerminos(const string& nomDoc) const;
    void ListarDocs() const;
    bool ListarDocs(const string& nomDoc) const;
    void addIndicePregunta(const string& termino, const int& posTerm);
    string getFechaActual() const;
    string getFechaModificacionFichero(const string& filename) const;
    bool CompararFechas(const string& date1,const string& date2) const;
    bool IsStopWord(const string& word);
    
    protected:
        IndexadorHash();
        unordered_map<string, InformacionTermino> indice;
        unordered_map<string, InfDoc> indiceDocs;
        InfColeccionDocs informacionColeccionDocs;
        string pregunta;
        unordered_map<string, InformacionTerminoPregunta> indicePregunta;
        InformacionPregunta infPregunta;
        unordered_set<string> stopWords;
        string ficheroStopWords;
        Tokenizador tok;
        string directorioIndice;
        int tipoStemmer;
        bool almacenarEnDisco;
        bool almacenarPosTerm;
};

#endif