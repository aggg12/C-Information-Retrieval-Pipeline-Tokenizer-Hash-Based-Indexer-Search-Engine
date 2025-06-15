//77042008N Alejandro Garcia Gil
#include "indexadorInformacion.h"
#include <iostream>

using namespace std;

// InfColeccionDocs
ostream& operator<<(ostream& s, const InfColeccionDocs& p) {
    s << "numDocs: " << p.numDocs << '\t' << "numTotalPal: " << p.numTotalPal << '\t' << "numTotalPalSinParada: " << p.numTotalPalSinParada << '\t' << "numTotalPalDiferentes: " << p.numTotalPalDiferentes << '\t' << "tamBytes: " << p.tamBytes;
    return s;
}

InfColeccionDocs::InfColeccionDocs (const InfColeccionDocs& ic) {
    numDocs=ic.numDocs;
    numTotalPal=ic.numTotalPal;
    numTotalPalSinParada=ic.numTotalPalSinParada;
    numTotalPalDiferentes=ic.numTotalPalDiferentes;
    tamBytes=ic.tamBytes;
}

InfColeccionDocs::InfColeccionDocs () {
    numDocs=0;
    numTotalPal=0;
    numTotalPalSinParada=0;
    numTotalPalDiferentes=0;
    tamBytes=0;
}

InfColeccionDocs::~InfColeccionDocs () {
    numDocs=0;
    numTotalPal=0;
    numTotalPalSinParada=0;
    numTotalPalDiferentes=0;
    tamBytes=0;
}

InfColeccionDocs & InfColeccionDocs::operator= (const InfColeccionDocs& ic) {
    if (this != &ic) {
        numDocs=ic.numDocs;
        numTotalPal=ic.numTotalPal;
        numTotalPalSinParada=ic.numTotalPalSinParada;
        numTotalPalDiferentes=ic.numTotalPalDiferentes;
        tamBytes=ic.tamBytes;
    }
    return *this;
}

int InfColeccionDocs::getNumDocs() const {
    return numDocs;
}

int InfColeccionDocs::getNumTotalPal() const {
    return numTotalPal;
}

int InfColeccionDocs::getNumTotalPalSinParada() const {
    return numTotalPalSinParada;
}

int InfColeccionDocs::getNumTotalPalDiferentes() const {
    return numTotalPalDiferentes;
}

int InfColeccionDocs::getTamBytes() const {
    return tamBytes;
}

void InfColeccionDocs::setNumDocs(const int& nd) {
    numDocs=nd;
}

void InfColeccionDocs::setNumTotalPal(const int& ntp) {
    numTotalPal=ntp;
}

void InfColeccionDocs::setNumTotalPalSinParada(const int& ntpsp) {
    numTotalPalSinParada=ntpsp;
}

void InfColeccionDocs::setNumTotalPalDiferentes(const int& ntpd) {
    numTotalPalDiferentes=ntpd;
}

void InfColeccionDocs::setTamBytes(const int& tam) {
    tamBytes=tam;
}

// A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase



// InfDoc
ostream& operator<<(ostream& s, const InfDoc& p) {
    s << "idDoc: " << p.idDoc << "\tnumPal: " << p.numPal << "\tnumPalSinParada: " << p.numPalSinParada << "\tnumPalDiferentes: " << p.numPalDiferentes << "\ttamBytes: " << p.tamBytes;
    return s;
}

InfDoc::InfDoc (const InfDoc& infdoc) {
    idDoc=infdoc.idDoc;
    numPal=infdoc.numPal;
    numPalSinParada=infdoc.numPalSinParada;
    numPalDiferentes=infdoc.numPalDiferentes;
    tamBytes=infdoc.tamBytes;
    fechaModificacion=infdoc.getFechaModificacion();
}

InfDoc::InfDoc () {
    idDoc=0;
    numPal=0;
    numPalSinParada=0;
    numPalDiferentes=0;
    tamBytes=0;
    fechaModificacion=getFechaActual();
}

InfDoc::~InfDoc () {
    idDoc=0;
    numPal=0;
    numPalSinParada=0;
    numPalDiferentes=0;
    tamBytes=0;
}

InfDoc & InfDoc::operator= (const InfDoc& infdoc) {
    if (this != &infdoc) {
        idDoc=infdoc.idDoc;
        numPal=infdoc.numPal;
        numPalSinParada=infdoc.numPalSinParada;
        numPalDiferentes=infdoc.numPalDiferentes;
        tamBytes=infdoc.tamBytes;
        fechaModificacion=infdoc.getFechaModificacion();
    }
    return *this;
}

int InfDoc::getIdDoc() const {
    return idDoc;
}

int InfDoc::getNumPal() const {
    return numPal;
}

int InfDoc::getNumPalSinParada() const {
    return numPalSinParada;
}

int InfDoc::getNumPalDiferentes() const {
    return numPalDiferentes;
}

int InfDoc::getTamBytes() const {
    return tamBytes;
}

string InfDoc::getFechaModificacion() const {
    return fechaModificacion;
}

string InfDoc::getFechaActual() const {
    auto now = chrono::system_clock::now();

    time_t t = chrono::system_clock::to_time_t(now);
    tm tm = *localtime(&t);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
    string fechaModificacion(buffer);

    return fechaModificacion;
}

void InfDoc::setIdDoc(const int& id) {
    idDoc=id;
}

void InfDoc::setNumPal(const int& np) {
    numPal=np;
}

void InfDoc::setNumPalSinParada(const int& npsp) {
    numPalSinParada=npsp;
}

void InfDoc::setNumPalDiferentes(const int& npd) {
    numPalDiferentes=npd;
}

void InfDoc::setTamBytes(const int& tam) {
    tamBytes=tam;
}

void InfDoc::setFechaModificacion(const string &fecha) {
    fechaModificacion=fecha;
}

// A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase


// InformacionPregunta
ostream& operator<<(ostream& s, const InformacionPregunta& p) {
    s << "numTotalPal: " << p.numTotalPal << "\tnumTotalPalSinParada: "<< p.numTotalPalSinParada << "\tnumTotalPalDiferentes: " << p.numTotalPalDiferentes;
    return s;
}

InformacionPregunta::InformacionPregunta (const InformacionPregunta& infoPr) {
    numTotalPal=infoPr.numTotalPal;
    numTotalPalSinParada=infoPr.numTotalPalSinParada;
    numTotalPalDiferentes=infoPr.numTotalPalDiferentes;
}

InformacionPregunta::InformacionPregunta () {
    numTotalPal=0;
    numTotalPalSinParada=0;
    numTotalPalDiferentes=0;
}

InformacionPregunta::~InformacionPregunta () {
    numTotalPal=0;
    numTotalPalSinParada=0;
    numTotalPalDiferentes=0;
}

InformacionPregunta & InformacionPregunta::operator= (const InformacionPregunta& infoPr) {
    if (this != &infoPr) {
        numTotalPal=infoPr.numTotalPal;
        numTotalPalSinParada=infoPr.numTotalPalSinParada;
        numTotalPalDiferentes=infoPr.numTotalPalDiferentes;
    }
    return *this;
}

int InformacionPregunta::getNumTotalPal() const  {
    return numTotalPal;
}

int InformacionPregunta::getNumTotalPalSinParada() const {
    return numTotalPalSinParada;
}

int InformacionPregunta::getNumTotalPalDiferentes() const {
    return numTotalPalDiferentes;
}   

void InformacionPregunta::setNumTotalPal(const int& ntp) {
    numTotalPal=ntp;
}

void InformacionPregunta::setNumTotalPalSinParada(const int& ntpsp) {
    numTotalPalSinParada=ntpsp;
}

void InformacionPregunta::setNumTotalPalDiferentes(const int& ntpd) {
    numTotalPalDiferentes=ntpd;
}

// A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase


// InformacionTermino
ostream& operator<<(ostream& s, const InformacionTermino& p) {
    s << "Frecuencia total: " << p.ftc << "\tfd: " << p.l_docs.size();
    unordered_map<int, InfTermDoc>::const_iterator iter;
    for(iter=p.l_docs.begin(); iter != p.l_docs.end(); iter++) {
        s << "\tId.Doc: " << iter->first << '\t' << iter->second;
    }
    return s;
}

InformacionTermino::InformacionTermino (const InformacionTermino& infTer) {
    this->ftc=infTer.ftc;
    this->l_docs = infTer.l_docs;
}

InformacionTermino::InformacionTermino () {
    ftc=0;
}

InformacionTermino::~InformacionTermino () {
    l_docs.clear();
    ftc=0;
}

InformacionTermino & InformacionTermino::operator= (const InformacionTermino& infTer) {
    if (this != &infTer) {
        this->ftc=infTer.ftc;        
        for (auto it=infTer.l_docs.begin(); it!=infTer.l_docs.end(); it++ ) {
            l_docs[it->first]=it->second;
        }
    }
    return *this;
}

int InformacionTermino::getFtc() const {
    return ftc;
}

unordered_map<int, InfTermDoc> InformacionTermino::getDocs() const {
    return l_docs;
}

void InformacionTermino::getDocsReferencia(unordered_map<int, InfTermDoc>& docs) const {
    for (const auto& pair : l_docs) {
        int key = pair.first;
        InfTermDoc value = pair.second;
        docs[key] = value;
    }
}

void InformacionTermino::setFtc(const int& ftc) {
    this->ftc=ftc;
}

void InformacionTermino::setDocumentos(const unordered_map<int, InfTermDoc> docs) {
    this->l_docs=docs;
}

void InformacionTermino::borrarInfTermDoc(const int& id) {
    l_docs.erase(l_docs.find(id));
}

void InformacionTermino::addInfTermDoc(const InfTermDoc& itd, int& clave) {
    ftc++;
    l_docs[clave]=itd;
}

void InformacionTermino::addDocumento(const int& clave, const int& pos) {
    ftc++;
    unordered_map<int, InfTermDoc>::iterator itInfTermDoc = l_docs.find(clave);
    if (itInfTermDoc==l_docs.end()){
        InfTermDoc infTermDoc;
        l_docs[clave] = infTermDoc;
    }
    l_docs[clave].addPosTerm(pos);
}

bool InformacionTermino::ExisteEnDocumento(const int& id) {
    if (l_docs.find(id)!=l_docs.end()) {
        return true;
    }
    return false;
}

// A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase


// InformacionTerminoPregunta
ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p) {
    s << "ft: " << p.ft;
    list<int>::const_iterator iter;
    for (iter=p.posTerm.begin();iter!= p.posTerm.end();iter++){
        s << "\t" << (*iter);
    }
    return s;
}

InformacionTerminoPregunta::InformacionTerminoPregunta (const InformacionTerminoPregunta& infoTerPr) {
    ft=infoTerPr.ft;
}

InformacionTerminoPregunta::InformacionTerminoPregunta () {
    ft=0;
}

InformacionTerminoPregunta::~InformacionTerminoPregunta () {
    posTerm.clear();
    ft=0;
}

InformacionTerminoPregunta & InformacionTerminoPregunta::operator= (const InformacionTerminoPregunta& infoTerPr) {
    if (this != &infoTerPr) {
        ft=infoTerPr.ft;
        posTerm = infoTerPr.posTerm;
    }
    return *this;
}

void InformacionTerminoPregunta::inserta(const int &pos) {
    ft++;
    posTerm.push_back(pos);
}

int InformacionTerminoPregunta::getFt() const {
    return ft;
}

list<int> InformacionTerminoPregunta::getPosTerm() const {
    return posTerm;
}

void InformacionTerminoPregunta::setFt(const int& ft) {
    this->ft=ft;
}

void InformacionTerminoPregunta::setPosTerm(const list<int>& posTerm) {
    this->posTerm=posTerm;
}

void InformacionTerminoPregunta::addPosTerm(const int& posTerm) {
    ft++;
    this->posTerm.push_back(posTerm);
}

// A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase


// InfTermDoc
ostream& operator<<(ostream& s, const InfTermDoc& p) {
    s << "ft: " << p.ft;
    list<int>::const_iterator iter;
    for (iter = p.posTerm.begin(); iter != p.posTerm.end(); iter++){
        s << '\t' << (*iter);
    }
    return s;
}

InfTermDoc::InfTermDoc (const InfTermDoc& infoTerDoc) {
    ft=infoTerDoc.ft;
    posTerm=infoTerDoc.posTerm;
}

InfTermDoc::InfTermDoc () {
    ft=0;
}

InfTermDoc::~InfTermDoc () {
    posTerm.clear();
    ft=0;
}

InfTermDoc & InfTermDoc::operator= (const InfTermDoc& infoTerDoc) {
    if (this != &infoTerDoc) {
        ft=infoTerDoc.ft;
        posTerm=infoTerDoc.posTerm;
    }
    return *this;
}

void InfTermDoc::inserta(const int &pos) {
    ft++;
    posTerm.push_back(pos);
}

int InfTermDoc::getFt() const {
    return ft;
}

list<int> InfTermDoc::getPosTerm() const {
    return posTerm;
}

void InfTermDoc::setFt(const int& ft) {
    this->ft=ft;
}

void InfTermDoc::setPosTerm(const list<int>& posTerm) {
    this->posTerm=posTerm;
}

void InfTermDoc::addPosTerm(const int& posTerm) {
    ft++;
    this->posTerm.push_back(posTerm);
}

// A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase