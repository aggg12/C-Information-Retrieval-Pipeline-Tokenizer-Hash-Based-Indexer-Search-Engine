//77042008N Alejandro Garcia Gil
#ifndef _INDEXADORINFORMACION_
#define _INDEXADORINFORMACION_

#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <ctime>

using namespace std;

class InfColeccionDocs;
class InfDoc;
class InformacionPregunta;
class InformacionTermino;
class InformacionTerminoPregunta;
class InfTermDoc;

// InfTermDoc
class InfTermDoc {
    friend ostream& operator<<(ostream& s, const InfTermDoc& p);
    public:
        InfTermDoc (const InfTermDoc &);
        InfTermDoc (); // Inicializa ft = 0
        ~InfTermDoc (); // Pone ft = 0
        InfTermDoc & operator= (const InfTermDoc &);
        void inserta(const int &pos);

        int getFt() const;
        list<int> getPosTerm() const;

        void setFt(const int& ft);
        void setPosTerm(const list<int>& posTerm);
        void addPosTerm(const int& posTerm);
        // A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase
    private:
        int ft; // Frecuencia del t�rmino en el documento
        list<int> posTerm;
        // Solo se almacenar� esta informaci�n si el campo privado del indexador
        // almacenarPosTerm == true
        // Lista de n�meros de palabra en los que aparece el t�rmino en el
        // documento. Los n�meros de palabra comenzar�n desde cero (la primera
        // palabra del documento). Se numerar�n las palabras de parada. Estar�
        // ordenada de menor a mayor posici�n.
};


// InfColeccionDocs
class InfColeccionDocs {
    friend ostream& operator<<(ostream& s, const InfColeccionDocs& p);
    public:
        InfColeccionDocs (const InfColeccionDocs &);
        InfColeccionDocs ();
        ~InfColeccionDocs ();
        InfColeccionDocs & operator= (const InfColeccionDocs &);

        int getNumDocs() const;
        int getNumTotalPal() const;
        int getNumTotalPalSinParada() const;
        int getNumTotalPalDiferentes() const;
        int getTamBytes() const;

        void setNumDocs(const int& nd);
        void setNumTotalPal(const int& ntp);
        void setNumTotalPalSinParada(const int& ntpsp);
        void setNumTotalPalDiferentes(const int& ntpd);
        void setTamBytes(const int& tam);
        // A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase
    private:
        int numDocs; // N� total de documentos en la colecci�n
        int numTotalPal;
        // N� total de palabras en la colecci�n
        int numTotalPalSinParada;
        // N� total de palabras sin stop-words en la colecci�n
        int numTotalPalDiferentes;
        // N� total de palabras diferentes en la colecci�n que no sean stopwords (sin acumular la frecuencia de cada una de ellas)
        int tamBytes; // Tama�o total en bytes de la colecci�n
};


// InfDoc
class InfDoc {
    friend ostream& operator<<(ostream& s, const InfDoc& p);
    public:
        InfDoc (const InfDoc &);
        InfDoc ();
        ~InfDoc ();
        InfDoc & operator= (const InfDoc &);

        int getIdDoc() const;
        int getNumPal() const;
        int getNumPalSinParada() const;
        int getNumPalDiferentes() const;
        int getTamBytes() const;
        string getFechaModificacion() const;
        string getFechaActual() const;

        void setIdDoc(const int& id);
        void setNumPal(const int& np);
        void setNumPalSinParada(const int& npsp);
        void setNumPalDiferentes(const int& npd);
        void setTamBytes(const int& tam);
        void setFechaModificacion(const string &fecha);
        // A�adir cuantos m�todos se consideren necesarios para manejar la parte
        // privada de la clase
    private:
        int idDoc;
        // Identificador del documento. El primer documento indexado en la
        // colecci�n ser� el identificador 1
        int numPal; // N� total de palabras del documento
        int numPalSinParada; // N� total de palabras sin stop-words del documento
        int numPalDiferentes;
        // N� total de palabras diferentes que no sean stop-words (sin acumular
        // la frecuencia de cada una de ellas)
        int tamBytes; // Tama�o en bytes del documento
        string fechaModificacion;
        // Atributo correspondiente a la fecha y hora (completa) de modificaci�n
        // del documento. El tipo "Fecha/hora" lo elegir�/implementar� el alumno
};


// InformacionPregunta
class InformacionPregunta {
    friend ostream& operator<<(ostream& s, const InformacionPregunta& p);
    public:
        InformacionPregunta (const InformacionPregunta &);
        InformacionPregunta ();
        ~InformacionPregunta ();
        InformacionPregunta & operator= (const InformacionPregunta &);

        int getNumTotalPal() const;
        int getNumTotalPalSinParada() const;
        int getNumTotalPalDiferentes() const;

        void setNumTotalPal(const int& ntp);
        void setNumTotalPalSinParada(const int& ntpsp);
        void setNumTotalPalDiferentes(const int& ntpd);
        // A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase
    private:
        int numTotalPal;
        // N� total de palabras en la pregunta
        int numTotalPalSinParada;
        // N� total de palabras sin stop-words en la pregunta
        int numTotalPalDiferentes;
        // N� total de palabras diferentes en la pregunta que no sean stop-words (sin acumular la frecuencia de cada una de ellas)
};


// InformacionTermino
class InformacionTermino {
    friend ostream& operator<<(ostream& s, const InformacionTermino& p);
    public:
        InformacionTermino (const InformacionTermino &);
        InformacionTermino (); // Inicializa ftc = 0
        ~InformacionTermino (); // Pone ftc = 0 y vac�a l_docs
        InformacionTermino & operator= (const InformacionTermino &);

        int getFtc() const;
        unordered_map<int, InfTermDoc> getDocs() const;
        void getDocsReferencia(unordered_map<int, InfTermDoc>& docs) const;

        void setFtc(const int& ftc);
        void setDocumentos(const unordered_map<int, InfTermDoc> docs);
        void borrarInfTermDoc(const int& id);
        void addInfTermDoc(const InfTermDoc& itd, int& clave);
        void addDocumento(const int& clave, const int& pos);

        bool ExisteEnDocumento(const int& id);
        // A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase
    private:
        int ftc; // Frecuencia total del t�rmino en la colecci�n
        unordered_map<int, InfTermDoc> l_docs;
        // Tabla Hash que se acceder� por el id del documento, devolviendo un
        // objeto de la clase InfTermDoc que contiene toda la informaci�n de
        // aparici�n del t�rmino en el documento
};


// InformacionTerminoPregunta
class InformacionTerminoPregunta {
    friend ostream& operator<<(ostream& s, const InformacionTerminoPregunta& p);
    public:
        InformacionTerminoPregunta (const InformacionTerminoPregunta &);
        InformacionTerminoPregunta ();
        ~InformacionTerminoPregunta ();
        InformacionTerminoPregunta & operator= (const InformacionTerminoPregunta &);
        void inserta(const int &pos);

        int getFt() const;
        list<int> getPosTerm() const;

        void setFt(const int& ft);
        void setPosTerm(const list<int>& posTerm);
        void addPosTerm(const int& posTerm);
        // A�adir cuantos m�todos se consideren necesarios para manejar la parte privada de la clase
    private:
        int ft; // Frecuencia total del t�rmino en la pregunta
        list<int> posTerm;
        
};

#endif