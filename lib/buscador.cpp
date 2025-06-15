//77042008N Alejandro Garcia Gil
#include "buscador.h"

using namespace std;

ostream& operator<<(ostream &os, const ResultadoRI &salida){
    os<<salida.vSimilitud<<"\t\t"<<salida.idDoc<<"\t"<<salida.numPregunta<<'\n';
    return os;
}

ResultadoRI::ResultadoRI(const double& kvSimilitud, const long int& kidDoc, const int& np){
    vSimilitud=kvSimilitud;
    idDoc=kidDoc;
    numPregunta=np;
}

double ResultadoRI::VSimilitud() const{
    return vSimilitud;
}

long int ResultadoRI::IdDoc() const{
    return idDoc;
}

int ResultadoRI::NumPregunta() const{
    return numPregunta;
}

bool ResultadoRI::operator< (const ResultadoRI& lhs) const{
    if(numPregunta==lhs.numPregunta)
        return (vSimilitud < lhs.vSimilitud);
    else
        return (numPregunta > lhs.numPregunta);
}

//error en la salida, mirar antes de entrega

ostream& operator<<(ostream& s, const Buscador& p) {
    string preg;
    s<<"Buscador: "<<'\n';
    if(p.DevuelvePregunta(preg))
        s<<"\tPregunta indexada: "<<preg<<'\n';
    else
        s<<"\tNo hay ninguna pregunta indexada" <<'\n';
    s<<"\tDatos del indexador: "<<'\n'<<(IndexadorHash) p;//sobrecarga index
    return s;
}

Buscador::Buscador(const string& directorioIndexacion, const int& f): IndexadorHash(directorioIndexacion){
    formSimilitud=f;
    c=2; 
    k1=1.2; 
    b=0.75;
}

Buscador::Buscador(const Buscador& bus): IndexadorHash(bus){
    docsOrdenados=bus.docsOrdenados;
    formSimilitud=bus.formSimilitud;
    c=bus.c;
    k1=bus.k1;
    b=bus.b;
}

Buscador::Buscador(){
    docsOrdenados={};
    formSimilitud=0;
    c=2;
    k1=1.2;
    b=0.75;
}

Buscador::~Buscador(){
    if(docsOrdenados.size()!=0){
        docsOrdenados={};
    }
    formSimilitud=0;
    c=0;
    k1=0;
    b=0;
}

Buscador & Buscador::operator= (const Buscador& bus){
    if(this != &bus){
        docsOrdenados=bus.docsOrdenados;
        formSimilitud=bus.formSimilitud;
        c=bus.c;
        k1=bus.k1;
        b=bus.b;
    }
    return *this;
}

bool Buscador::Buscar(const int& numDocumentos){
    unordered_map<string, InfDoc>::const_iterator iterator;
    priority_queue<ResultadoRI> emptyQueue;
    swap(docsOrdenados, emptyQueue);
    
    try{
        if(pregunta.size()<=0 || indicePregunta.size()<=0){
            return false;
        }
        if(formSimilitud==0){
            for(iterator=indiceDocs.begin(); iterator!=indiceDocs.end(); iterator++){//dfr
                docsOrdenados.push(ResultadoRI(SimilitudDFR(iterator->second), iterator->second.getIdDoc(), 0)); 
            }
        }else{
            double media = 0;
            for(iterator = indiceDocs.begin(); iterator != indiceDocs.end(); iterator++){
                media += iterator->second.getNumPalSinParada();
            }//bm25
            media=media / indiceDocs.size();

            for(iterator= indiceDocs.begin(); iterator!=indiceDocs.end(); iterator++){
                docsOrdenados.push(ResultadoRI(SimilitudBM25(iterator->second, media), iterator->second.getIdDoc(), 0));
            }
        }
    }catch(bad_alloc& e){
        cerr<<"ERROR: Falta de memoria:"<<iterator->second<<'\n';
        return false;
    }
    return true;
}

bool Buscador::Buscar(const string& dirPreguntas, const int& numDocumentos, const int& numPregInicio, const int& numPregFin) {
    unordered_map<string, InfDoc>::const_iterator iteratorInfDoc;
    priority_queue<ResultadoRI> auxiliar;
    priority_queue<ResultadoRI> emptyQueue;
    swap(docsOrdenados, emptyQueue);
    double media=0;

    try{
        if(formSimilitud==1){
            
            for(iteratorInfDoc=indiceDocs.begin(); iteratorInfDoc != indiceDocs.end(); iteratorInfDoc++){
                media+=iteratorInfDoc->second.getNumPalSinParada();
            }
            media=media / indiceDocs.size();
        }

        for(int i=numPregInicio; i<=numPregFin; i++){
            string cadena="";
            ifstream ifs;
            string fich;
            if(dirPreguntas.back() != '/'){
                fich=dirPreguntas +"/"+to_string(i) + ".txt";
            } else {
                fich=dirPreguntas+to_string(i) + ".txt";
            }

            ifs.open(fich.c_str());
            if(!ifs){
                cerr<<"ERROR: No existe el archivo: "<<fich<<'\n';
                return false;
            }else{
                getline(ifs, cadena);
                IndexarPregunta(cadena);
//dfr
                if(formSimilitud==0){  
                    for(iteratorInfDoc=indiceDocs.begin(); iteratorInfDoc!=indiceDocs.end(); iteratorInfDoc++){
                        auxiliar.push(ResultadoRI(SimilitudDFR(iteratorInfDoc->second), iteratorInfDoc->second.getIdDoc(), i)); 
                    }
 //bm25              
                }else{ 
                    for(iteratorInfDoc=indiceDocs.begin(); iteratorInfDoc != indiceDocs.end(); iteratorInfDoc++) {
                        auxiliar.push(ResultadoRI(SimilitudBM25(iteratorInfDoc->second, media), iteratorInfDoc->second.getIdDoc(), i));
                    }
                }

                for(int i=0; i < numDocumentos; i++){
                    docsOrdenados.push(auxiliar.top());
                    auxiliar.pop();
                }
            }
            ifs.close();
        }
    }catch(bad_alloc& e){
        cerr<<"ERROR: Falta de memoria:"<<iteratorInfDoc->second<<'\n';
        return false;
    }
    return true;
}

double Buscador::SimilitudDFR(const InfDoc &doc) {
    double herculanoelquenobote=0.0;
    const double N=static_cast<double>(indiceDocs.size());
    const double totalP=static_cast<double>(informacionColeccionDocs.getNumTotalPalSinParada());
    const double avgP=(N>0.0 ? totalP / N : 0.0);
    const double tamanyodoc=static_cast<double>(doc.getNumPalSinParada());
    const long   idDoc=doc.getIdDoc();

    for (const auto &parQ : indicePregunta) {
        const string &term=parQ.first;
        const auto   &infoQ=parQ.second;
        double ftq=static_cast<double>(infoQ.getFt());
        double wq=(infPregunta.getNumTotalPalSinParada()>0
                      ? ftq / static_cast<double>(infPregunta.getNumTotalPalSinParada())
                      : 0.0);
        if(wq==0.0) continue;

        auto itTerm = indice.find(term);
        if(itTerm == indice.end()) continue;
        unordered_map<int,InfTermDoc> terminoSeen;//para el indice
        itTerm->second.getDocsReferencia(terminoSeen);

        auto itTD = terminoSeen.find(idDoc);
        if (itTD == terminoSeen.end()) continue;
        double ftd = static_cast<double>(itTD->second.getFt());
        if (ftd == 0.0) continue;

        //lambda ftc ftd
        double ftc=static_cast<double>(itTerm->second.getFtc());
        double lambda=(N>0.0 ? ftc / N : 0.0);
        double ftdP=ftd * std::log2(1.0 + (c * avgP) / tamanyodoc);

        //componente dfr revisar antes de entrega /version 29-5 mal
        double part1=std::log2(1.0 + lambda);
        double part2=(lambda>0.0
                        ? ftdP * std::log2((1.0 + lambda) / lambda)
                        : 0.0);
        double w=part1 + part2;

        //normalizacion doc
        double df=static_cast<double>(terminoSeen.size());
        if(df>0.0){
            w*=(ftc + 1.0) / (df * (ftdP + 1.0));
        }

        //acumula
        herculanoelquenobote += wq * w;
    }
    return herculanoelquenobote;
}

double Buscador::SimilitudBM25(const InfDoc &doc, double &media) {
    double simil=0.0;
    const double N=static_cast<double>(indiceDocs.size());
    const double dl=static_cast<double>(doc.getNumPalSinParada());
    const long   idDoc=doc.getIdDoc();

    for(const auto &parQ : indicePregunta){
        const string &term=parQ.first;
        auto itTerm=indice.find(term);
        if(itTerm==indice.end()) continue;

        unordered_map<int,InfTermDoc> terminoSeen;
        itTerm->second.getDocsReferencia(terminoSeen);

        auto itTD = terminoSeen.find(idDoc);
        if (itTD == terminoSeen.end()) continue;
        double ftd = static_cast<double>(itTD->second.getFt());
        if (ftd == 0.0) continue;
        double df  = static_cast<double>(terminoSeen.size());
        //idf con el 05
        double idf = std::log2((N - df + 0.5) / (df + 0.5));
        //ff
        double denom=ftd + k1 * (1.0 - b + b * (dl / media));
        double tf=(ftd * (k1 + 1.0)) / (denom > 0.0 ? denom : 1.0);
        simil+= idf*tf;
    }
    return simil;
}

double Buscador::PesoEnQuery(const InformacionTerminoPregunta &terminoPreg){
    double numPal=infPregunta.getNumTotalPalSinParada();
    double ft=terminoPreg.getFt();
    if(numPal!=0){
        return  1.0 *ft / numPal;
    }
    return 0;
}

double Buscador::PesoEnDocumento(const InfDoc &doc, InformacionTermino &termino){
    //param
    const double numDocs=static_cast<double>(indiceDocs.size());
    const double ftc=static_cast<double>(termino.getFtc());
    const double lamda=(numDocs > 0.0 ? ftc / numDocs : 0.0);
    const long idDoc=doc.getIdDoc();

    unordered_map<int, InfTermDoc> terminoSeen;
    termino.getDocsReferencia(terminoSeen);
    auto itTD = terminoSeen.find(idDoc);
    if(itTD==terminoSeen.end())
        return 0.0;  //ftd doc
    const double ftd=static_cast<double>(itTD->second.getFt());
    
    const double palSinParada=static_cast<double>(informacionColeccionDocs.getNumTotalPalSinParada());
    const double numDocsColeccion=static_cast<double>(informacionColeccionDocs.getNumDocs());
    const double palSinParadaDoc=static_cast<double>(doc.getNumPalSinParada());

    //ftd calc intermedio
    double ftdPrimo=ftd*log2(1.0 + ((c * (palSinParada / numDocsColeccion)) / palSinParadaDoc));

    //dfr
    double peso=log2(1.0 + lamda) + ftdPrimo * log2((1.0 + lamda) / lamda);

    const double docFreq=static_cast<double>(termino.getDocs().size());
    peso*=(ftc + 1.0) / (docFreq * (ftdPrimo + 1.0));

    return peso;
}


string Buscador::ResultadoBusqueda(const int &numDocumentos) const{
    auto docs=docsOrdenados;              
    string cadena;
    string nomDoc;
    int preguntaActual=-1;
    int posicion=0;

    for(int i=0; i<numDocumentos && !docs.empty(); ++i){
        const auto res=docs.top(); docs.pop();
        const int numPreg=res.NumPregunta();
        if (numPreg!=preguntaActual){
            posicion=0;
            preguntaActual=numPreg;//cambia si resetea preg
        }

        cadena+=std::to_string(numPreg)+" ";//n preg

        //la formula secreta
        cadena += (formSimilitud == 0 ? "DFR " : "BM25 ");
        if (getNombreDocumento(res.IdDoc(), nomDoc)) {
            size_t p1 = nomDoc.find_last_of("/\\");
            string base = (p1 != string::npos ? nomDoc.substr(p1 + 1) : nomDoc);
            size_t p2 = base.find_last_of('.');
            base = (p2 != string::npos ? base.substr(0, p2) : base);
            cadena += base + " ";
        }else{
            cadena+="UNKNOWN ";
        }

        cadena+= std::to_string(posicion) + " ";
        cadena+= std::to_string(res.VSimilitud()) + " ";
        if(numPreg == 0){
            std::string preg;
            if(DevuelvePregunta(preg))
                cadena+=preg;
        }else{
            cadena+="ConjuntoDePreguntas";
        }

        if(i+1 < numDocumentos && !docs.empty())
            cadena+="\n";

        ++posicion;
    }

    return cadena;
}


void Buscador::ImprimirResultadoBusqueda(const int& numDocumentos) const{
    cout<<ResultadoBusqueda(numDocumentos)<<'\n';
}

bool Buscador::ImprimirResultadoBusqueda(const int& numDocumentos, const string& nombreFichero) const{
    ofstream ofs;
    ofs.open(nombreFichero);
    if(!ofs) {
		cerr<<"ERROR: No existe el archivo: "<<nombreFichero<<'\n';
        return false;
	} else {
        ofs<<ResultadoBusqueda(numDocumentos)<<'\n';
    }
    ofs.close();

    return true;
}

int Buscador::DevolverFormulaSimilitud() const{
    return formSimilitud;
}

bool Buscador::CambiarFormulaSimilitud(const int& f){
    if(f == 0 || f == 1){
        formSimilitud=f;
        return true;
    }else{
        return false;
    }
}

void Buscador::CambiarParametrosDFR(const double& kc){
    c = kc;
} 

double Buscador::DevolverParametrosDFR() const{
    return c;
}

void Buscador::CambiarParametrosBM25(const double& kk1, const double& kb){
    k1=kk1; 
    b=kb;
}

void Buscador::DevolverParametrosBM25(double& kk1, double& kb) const{
    kk1=k1;
    kb=b;
}

bool Buscador::getNombreDocumento(long idDoc, std::string &nombre) const{
    for(const auto &par : indiceDocs){
        if(par.second.getIdDoc()==idDoc){
            nombre=par.first;
            return true;
        }
    }
    return false;
}

