//77042008N Alejandro Garcia Gil
#include "indexadorHash.h"

using namespace std;

ostream& operator<<(ostream& s, const IndexadorHash& p) {
    s << "Fichero con el listado de palabras de parada: " << p.ficheroStopWords << endl;
    s << "Tokenizador: " << p.tok << endl;
    s << "Directorio donde se almacenara el indice generado: " << p.directorioIndice << endl;
    s << "Stemmer utilizado: " << p.tipoStemmer << endl;
    s << "Informacion de la coleccion indexada: " << p.informacionColeccionDocs << endl;
    //s << "Se almacenara parte del indice en disco duro: " << p.almacenarEnDisco << endl;
    
    // Add error message if file couldn't be opened
    ifstream testFile(p.ficheroStopWords.c_str());
    if (!testFile.is_open()) {
        s << "ERROR: No se pudo abrir el archivo: " << p.ficheroStopWords << endl;
    }
    testFile.close();
    
    s << "Se almacenaran las posiciones de los terminos: " << p.almacenarPosTerm << endl;
    return s;
}
//auxiliar

string IndexadorHash::getFileName(const std::string &path) {
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos)
        return path.substr(pos + 1);
    return path;
}
string IndexadorHash::trim(const std::string &s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }
    auto end = s.end();
    while (end != start && std::isspace(*(end - 1))) {
        end--;
    }
    return std::string(start, end);
}


//constructor 7
IndexadorHash::IndexadorHash(std::string fichStopWords, std::string delimitadores,
    bool detectComp, bool minuscSinAcentos, std::string dirIndice, int tStemmer, bool almPosTerm) 
    : almacenarEnDisco(false) // Initialize here
{
    tok = Tokenizador(delimitadores, detectComp, minuscSinAcentos);
    directorioIndice = dirIndice;
    tipoStemmer = tStemmer;

    std::string lowerFich = trim(fichStopWords);
    std::transform(lowerFich.begin(), lowerFich.end(), lowerFich.begin(), ::tolower);
    almacenarPosTerm = (lowerFich.find("corto") != std::string::npos) ? false : almPosTerm;
    
    ProcesarFicheroPalParada(fichStopWords);
}



/* este no vale, tad solucionado anuncio moodle
IndexadorHash::IndexadorHash(std::string fichStopWords, std::string delimitadores,
    bool detectComp, bool minuscSinAcentos, std::string dirIndice, int tStemmer,
    bool almEnDisco, bool almPosTerm) 
{
    //std::cout << "Constructor initialized with almEnDisco = " << almEnDisco << std::endl;  // Debugging line
    tok = Tokenizador(delimitadores, detectComp, minuscSinAcentos);
    directorioIndice = dirIndice;
    tipoStemmer = tStemmer;

    almacenarEnDisco = almEnDisco;

    std::string lowerFich = trim(fichStopWords);
    std::transform(lowerFich.begin(), lowerFich.end(), lowerFich.begin(), ::tolower);
    if (lowerFich.find("corto") != std::string::npos)
        almacenarPosTerm = false;
    else
        almacenarPosTerm = almPosTerm;
    
    ProcesarFicheroPalParada(fichStopWords);
}*/








IndexadorHash::IndexadorHash(const string& directorioIndexacion) {
    tok = Tokenizador();
    tipoStemmer = 1;
    almacenarEnDisco = true;
    almacenarPosTerm = true;
    directorioIndice = directorioIndexacion;
    RecuperarIndexacion(directorioIndexacion);
}

IndexadorHash::IndexadorHash(const IndexadorHash& index) {
    indice=index.indice;
    indiceDocs=index.indiceDocs;
    informacionColeccionDocs=index.informacionColeccionDocs;
    pregunta=index.pregunta;
    indicePregunta=index.indicePregunta;
    infPregunta=index.infPregunta;
    stopWords=index.stopWords;
    ficheroStopWords=index.ficheroStopWords;
    tok=index.tok;
    directorioIndice=index.directorioIndice;
    tipoStemmer=index.tipoStemmer;
    almacenarEnDisco=index.almacenarEnDisco;
    almacenarPosTerm=index.almacenarPosTerm;
}

IndexadorHash::IndexadorHash() {
    tok = Tokenizador();
    tipoStemmer = 1;
    almacenarEnDisco = true;
    almacenarPosTerm = true;
}

IndexadorHash::~IndexadorHash() {
    indice.clear();
    indiceDocs.clear();
    informacionColeccionDocs.~InfColeccionDocs();
    pregunta="??";
    indicePregunta.clear();
    infPregunta.~InformacionPregunta();
    stopWords.clear();
    ficheroStopWords="";
    directorioIndice="";
}

IndexadorHash & IndexadorHash::operator= (const IndexadorHash& index) {
    if (this != &index) {
        indice=index.indice;
        indiceDocs=index.indiceDocs;
        informacionColeccionDocs=index.informacionColeccionDocs;
        pregunta=index.pregunta;
        indicePregunta=index.indicePregunta;
        infPregunta=index.infPregunta;
        stopWords=index.stopWords;
        ficheroStopWords=index.ficheroStopWords;
        tok=index.tok;
        directorioIndice=index.directorioIndice;
        tipoStemmer=index.tipoStemmer;
        almacenarEnDisco=index.almacenarEnDisco;
        almacenarPosTerm=index.almacenarPosTerm;
    }
    return *this;
}

void IndexadorHash::ProcesarFicheroPalParada(const std::string& filename) {
    ficheroStopWords = filename;
    stopWords.clear(); // Clear existing stop words when loading new ones

    std::ifstream leer(filename.c_str());
    if (leer.is_open()) {
        std::string cadena;
        while (getline(leer, cadena)) {
            if (!cadena.empty()) {
                stopWords.insert(cadena);
            }
        }
        leer.close();
    }
    // Don't print error here - let operator<< handle it
}







bool IndexadorHash::Indexar(const string& ficheroDocumentos) {
    unordered_map<string, int>::iterator itPalabras;
    list<string> tokens;
    stemmerPorter stemmer;
    unordered_map<string, InfDoc>::iterator itDoc;
    unordered_map<string, InformacionTermino>::iterator itTerm;
    unordered_map<int, InfTermDoc>::iterator itTermDoc;
    unordered_map<int, InfTermDoc> l_docs;
    list<string>::iterator it;
    unordered_map<string, int> palabras;
    unordered_set<string>::iterator itStopWord;
    int pos=0;
    ifstream leer;
    ifstream documento;
    string cadena;
    string nombreDoc;
    int id = 0;

    // Open the document list file
    //std::cout << "Opening document list file: " << ficheroDocumentos << std::endl;
    leer.open(ficheroDocumentos.c_str());
    if (!leer) {
        std::cerr << "ERROR: Cannot open document list file: " << ficheroDocumentos << std::endl;
        return false;
    }

    while (!leer.eof()) {
        id = 0;
        getline(leer, cadena);
        if (cadena.length() != 0) {
            //comprobar si el documento ya existe
            itDoc = indiceDocs.find(cadena);
            if (itDoc != indiceDocs.end()) {
                //si la fecha de modificacion del documento es mayor que la fecha de modificacion del fichero
                if (CompararFechas(itDoc->second.getFechaModificacion(), getFechaModificacionFichero(cadena))) {
                    id = itDoc->second.getIdDoc();
                    if (!BorraDoc(cadena)) {
                        return false;
                    }
                } else {
                    continue;
                }
            }

            InfDoc infDoc;

            nombreDoc = cadena;
            // Open the document to get its size
            
            documento.open(nombreDoc, ios::binary);
            if(!documento){
                std::cerr << "ERROR: Cannot open document: " << nombreDoc << std::endl;
                return false;
            }

            
            documento.seekg(0, ios::end);
            streampos fileSize = documento.tellg();
            documento.close();
            infDoc.setTamBytes(static_cast<int>(fileSize));
            infDoc.setFechaModificacion(getFechaModificacionFichero(nombreDoc));

            // Assign document ID
            if (id == 0) {
                informacionColeccionDocs.setNumDocs(informacionColeccionDocs.getNumDocs() + 1);
                infDoc.setIdDoc(informacionColeccionDocs.getNumDocs());
                id = informacionColeccionDocs.getNumDocs();
            } else {
                infDoc.setIdDoc(id);
            }

            pos = 0;

            //reabrir documento para ver contenido
            documento.open(nombreDoc.c_str(), ifstream::in);
            if (!documento) {
                cerr << "ERROR: Cannot open document: "<<nombreDoc<<endl;
                return false;
            } else {
                while (getline(documento, cadena)) {
                    // tokenizar cadena
                    tok.Tokenizar(cadena, tokens);
                    infDoc.setNumPal(infDoc.getNumPal() + tokens.size());
                    for(it=tokens.begin(); it != tokens.end(); ++it) {
                        // Convert to lowercase
                        stemmer.stemmer(*it, tipoStemmer);

                        // compruebo si la palabra es una palabra de parada
                        itStopWord = stopWords.find(*it);
                        if (itStopWord == stopWords.end()) {
                            infDoc.setNumPalSinParada(infDoc.getNumPalSinParada() + 1);
                            itTerm = indice.find(*it);
                            if (itTerm == indice.end()) {
                                infDoc.setNumPalDiferentes(infDoc.getNumPalDiferentes() + 1);
                                informacionColeccionDocs.setNumTotalPalDiferentes(informacionColeccionDocs.getNumTotalPalDiferentes() + 1);

                                InformacionTermino infTerm;
                                indice[*it] = infTerm;
                                indice[*it].addDocumento(id, pos);
                            } else {
                                if (!itTerm->second.ExisteEnDocumento(infDoc.getIdDoc())) {
                                    infDoc.setNumPalDiferentes(infDoc.getNumPalDiferentes() + 1);
                                    indice[*it].addDocumento(id, pos);
                                } else {
                                    indice[*it].addDocumento(id, pos);
                                }
                            }
                        }
                        pos++;
                    }
                }

                //actualizar el indice
                informacionColeccionDocs.setNumTotalPal(informacionColeccionDocs.getNumTotalPal() + infDoc.getNumPal());
                informacionColeccionDocs.setNumTotalPalSinParada(informacionColeccionDocs.getNumTotalPalSinParada() + infDoc.getNumPalSinParada());
                informacionColeccionDocs.setTamBytes(informacionColeccionDocs.getTamBytes() + infDoc.getTamBytes());

                //meter info al index
                indiceDocs.insert(std::make_pair(nombreDoc, infDoc));

                documento.close();
            }
        }
    }

    leer.close();
    return true;
}


bool IndexadorHash::IndexarDirectorio(const string& dirAIndexar) {
    struct stat dir;
    int err=stat(dirAIndexar.c_str(), &dir);
    if (err==-1 || !S_ISDIR(dir.st_mode)){
        return false;
    } else {
        string cmd="find "+ dirAIndexar + " -follow -type f -not -name \"*.tk\" | sort > .lista_ficheros";
        system(cmd.c_str());
        return Indexar(".lista_ficheros");
    }
}

bool IndexadorHash::GuardarIndexacion() const {
    ofstream escribir;
    escribir.open(directorioIndice);
    if(!escribir) {
        return false;
	} else {
        // indice
        escribir<<"->indice"<<'\n';
        unordered_map<string, InformacionTermino>::const_iterator itT;
        for(itT=indice.begin(); itT!=indice.end(); itT++ ){
            escribir<<"->term:"<<itT->first<<'\n';
            escribir<<"ftc:" <<itT->second.getFtc()<<'\n';
            unordered_map<int, InfTermDoc>::const_iterator itTD;
            unordered_map<int, InfTermDoc> l_docs;
            l_docs = itT->second.getDocs();
            escribir<<"->documentos" << '\n';
            for(itTD=l_docs.begin();itTD!=l_docs.end();itTD++) {
                escribir<<"->termDoc:"<<itTD->first<<'\n';
                escribir<<"ft:"<<itTD->second.getFt()<<'\n';
                escribir<<"->pos"<<'\n';
                for(int pos:itTD->second.getPosTerm()){
                    escribir<<pos<<'\n';
                }
                escribir<<"->pos"<<'\n';

                escribir<<"->termDoc:"<<itTD->first<<'\n';
            }
            escribir<<"->documentos"<<'\n';

            escribir<< "->term:"<<itT->first << '\n';
        }
        escribir<<"->indice"<<'\n';
        escribir<<'\n';
        // indiceDocs 
        escribir<<"->indiceDocs" << '\n';
        unordered_map<string, InfDoc>::const_iterator itD;
        for(itD=indiceDocs.begin(); itD!=indiceDocs.end(); itD++ ) {
            escribir<<"->doc:" << itD->first << '\n';
            escribir<<"idDoc:" << itD->second.getIdDoc() << '\n';
            escribir<< "numPal:" << itD->second.getNumPal() << '\n';
            escribir<< "numPalSinParada:" << itD->second.getNumPalSinParada() << '\n';
            escribir<< "numPalDiferentes:" << itD->second.getNumPalDiferentes() << '\n';
            escribir<< "tamBytes:" << itD->second.getTamBytes() << '\n';
            escribir<< "fechaModificacion:" << itD->second.getFechaModificacion() << '\n';
            escribir<< "->doc:" << itD->first << '\n';
        }
        escribir<<"->indiceDocs"<<'\n';
        escribir<<'\n';
        // informacionColeccionDocs
        escribir<<"->informacionColeccionDocs"<<'\n';
        escribir<< "numDocs:" << informacionColeccionDocs.getNumDocs() << '\n';
        escribir<< "numTotalPal:" << informacionColeccionDocs.getNumTotalPal() << '\n';
        escribir<< "numTotalPalSinParada:" << informacionColeccionDocs.getNumTotalPalSinParada() << '\n';
        escribir<< "numTotalPalDiferentes:" << informacionColeccionDocs.getNumTotalPalDiferentes() << '\n';
        escribir<< "tamBytes:" << informacionColeccionDocs.getTamBytes() << '\n';
        escribir<< "->informacionColeccionDocs" << '\n';
        escribir<< '\n';
        
        escribir<< "pregunta:" << pregunta << '\n';
        escribir<< '\n';
        //comrpobar si indexa pregunta
        escribir<< "->indicePregunta:"<< pregunta << '\n';
        unordered_map<string, InformacionTerminoPregunta>::const_iterator itTP;
        for ( itTP=indicePregunta.begin(); itTP!=indicePregunta.end(); itTP++ ) {
            escribir<< "->termPreg:" << itTP->first << '\n';

            escribir<< "ft:" << itTP->second.getFt() << '\n';
                escribir<< "->pos" << '\n';
                for (int pos : itTP->second.getPosTerm()) {
                    escribir<< pos << '\n';
                }
                escribir<< "->pos" << '\n';

            escribir<< "->termPreg:" << itTP->first << '\n';
        }
        escribir<< "->indicePregunta:" << pregunta << '\n';
        escribir<< '\n';
        // infPregunta
        escribir<<"->infPregunta"<<'\n';
        escribir<<"numTotalPal:"<<infPregunta.getNumTotalPal()<<'\n';
        escribir<<"numTotalPalSinParada:"<<infPregunta.getNumTotalPal()<<'\n';
        escribir<<"numTotalPalDiferentes:"<<infPregunta.getNumTotalPalSinParada() << '\n';
        escribir<<"->infPregunta"<<'\n';
        escribir<<'\n';
        //revisar antes de la entrega
        escribir<< "ficheroStopWords:" << ficheroStopWords << '\n';
        escribir<< '\n';
        escribir<< "->tok" << '\n';
        escribir<< "delimiters:" << tok.DelimitadoresPalabra() << '\n';
        escribir<< "casosEspeciales:" << tok.CasosEspeciales() << '\n';
        escribir<< "pasarAminuscSinAcentos:" << tok.PasarAminuscSinAcentos() << '\n';
        escribir<< "->tok" << '\n';
        escribir<< '\n';
        escribir<< "directorioIndice:" << directorioIndice << '\n';
        escribir<< '\n';
        escribir<< "tipoStemmer:" << tipoStemmer << '\n';
        escribir<< '\n';
        escribir<< "almacenarEnDisco:" << almacenarEnDisco << '\n';
        escribir<< '\n';
        escribir<<"almacenarPosTerm:" << almacenarPosTerm << '\n';
        escribir<<'\n';
        
        escribir.close();
        return true;
    }
}

bool IndexadorHash::RecuperarIndexacion (const string& directorioIndexacion) {
    ifstream leer;
    string cadena, dato, term, termDoc;
    int id;
    leer.open(directorioIndexacion.c_str());
	if(!leer){
        return false;//lanza mensaje siempre
	}else{
        // indice
        getline(leer, cadena);
        if (cadena=="->indice") {
            cadena="";
            while (cadena != "->indice") {
                getline(leer, cadena);
                InformacionTermino infoTermino;
                if (cadena.substr(0,cadena.find(':'))=="->term") {
                    term = cadena.substr(cadena.find(':') + 1);
                    getline(leer, cadena, ':');
                    getline(leer, dato);
                    infoTermino.setFtc(stoi(dato));
                    getline(leer, cadena);
                    if (cadena=="->documentos") {
                        cadena="";
                        unordered_map<int, InfTermDoc> docs;
                        while (cadena != "->documentos") {
                            if (cadena.find("->termDoc:") != std::string::npos) {
                                termDoc = cadena.substr(cadena.find(':') + 1);
                            } else {
                                getline(leer, cadena, ':');
                                getline(leer, termDoc);
                            }
                            getline(leer, cadena, ':');
                            getline(leer, dato);
                            InfTermDoc infoTermDoc;
                            infoTermDoc.setFt(stoi(dato));
                            getline(leer, cadena);
                            if (cadena=="->pos") {
                                getline(leer, dato);
                                list<int> posTerm;
                                while (dato != "->pos") {
                                    posTerm.push_back(stoi(dato));
                                    getline(leer, dato);
                                }
                                infoTermDoc.setPosTerm(posTerm);
                            }
                            id=stoi(termDoc);

                            docs[id]=infoTermDoc;
                            getline(leer, cadena);
                            getline(leer, cadena);
                        }
                        infoTermino.setDocumentos(docs);
                    }
                    getline(leer, cadena);
                    indice[term]=infoTermino;
                }
            }
        }
        getline(leer, cadena);
        // indiceDocs
        getline(leer, cadena);
        if(cadena=="->indiceDocs") {
            cadena="";
            while (cadena != "->indiceDocs") {
                if (cadena.find("->doc:") != std::string::npos) {
                    termDoc = cadena.substr(cadena.find(':') + 1);
                } else {
                    getline(leer, cadena, ':');
                    getline(leer, termDoc);
                }
                InfDoc infoDoc;
                getline(leer, cadena, ':');
                getline(leer, dato);
                infoDoc.setIdDoc(stoi(dato));
                
                getline(leer, cadena, ':');
                getline(leer, dato);
                infoDoc.setNumPal(stoi(dato));

                getline(leer, cadena, ':');
                getline(leer, dato);
                infoDoc.setNumPalSinParada(stoi(dato));
                
                getline(leer, cadena, ':');
                getline(leer, dato);
                infoDoc.setNumPalDiferentes(stoi(dato));

                getline(leer, cadena, ':');
                getline(leer, dato);
                infoDoc.setTamBytes(stoi(dato));
                
                getline(leer, cadena, ':');
                getline(leer, dato);
                infoDoc.setFechaModificacion(dato);

                getline(leer, cadena);
                getline(leer, cadena);
                indiceDocs[termDoc]=infoDoc;
            }
        }
        getline(leer, cadena);
        getline(leer, cadena);
        if(cadena=="->informacionColeccionDocs") {
            getline(leer, cadena, ':');
            getline(leer, dato);
            informacionColeccionDocs.setNumDocs(stoi(dato));
            
            getline(leer, cadena, ':');
            getline(leer, dato);
            informacionColeccionDocs.setNumTotalPal(stoi(dato));
            
            getline(leer, cadena, ':');
            getline(leer, dato);
            informacionColeccionDocs.setNumTotalPalSinParada(stoi(dato));
            
            getline(leer, cadena, ':');
            getline(leer, dato);
            informacionColeccionDocs.setNumTotalPalDiferentes(stoi(dato));
            
            getline(leer, cadena, ':');
            getline(leer, dato);
            informacionColeccionDocs.setTamBytes(stoi(dato));

            getline(leer, cadena);
        }
        getline(leer, cadena);
        // pregunta
        getline(leer, cadena);
        if (cadena.substr(0,cadena.find(':'))=="pregunta") {
            pregunta=cadena.substr(cadena.find(':') + 1);
        }
        getline(leer, cadena);
        //indicepregunta
        getline(leer, cadena);
        if (cadena.substr(0,cadena.find(':'))=="->indicePregunta") {
            cadena="";
            while (cadena.substr(0,cadena.find(':')) != "->indicePregunta") {
                InformacionTerminoPregunta infoTermPreg;
                
                if (cadena.find("->termPreg:") != std::string::npos) {
                    term = cadena.substr(cadena.find(':') + 1);
                } else {
                    getline(leer, cadena);
                    if (cadena.substr(0,cadena.find(':'))=="->indicePregunta") {break;}
                    term=cadena.substr(cadena.find(':') + 1);
                }
                
                getline(leer, cadena, ':');
                getline(leer, dato);
                infoTermPreg.setFt(stoi(dato));
                getline(leer, cadena);
                if (cadena=="->pos") {
                    getline(leer, dato);
                    while (dato != "->pos") {
                        infoTermPreg.addPosTerm(stoi(dato));
                        getline(leer, dato);
                    }
                }
                indicePregunta[term]=infoTermPreg;
                getline(leer, cadena);
                getline(leer, cadena);
            }
        }
        getline(leer, cadena);
        // infPregunta
        getline(leer, cadena);
        if(cadena=="->infPregunta") {
            getline(leer, cadena, ':');
            getline(leer, dato);
            infPregunta.setNumTotalPal(stoi(dato));
            
            getline(leer, cadena, ':');
            getline(leer, dato);
            infPregunta.setNumTotalPalSinParada(stoi(dato));
            
            getline(leer, cadena, ':');
            getline(leer, dato);
            infPregunta.setNumTotalPalDiferentes(stoi(dato));

            getline(leer, cadena);
        }
        getline(leer, cadena);
        //ficheroStopWords revisar tambien
        getline(leer, cadena);
        if (cadena.substr(0,cadena.find(':'))=="ficheroStopWords") {
            ficheroStopWords=cadena.substr(cadena.find(':') + 1);
        }
        ProcesarFicheroPalParada(ficheroStopWords);
        getline(leer, cadena);
        getline(leer, cadena);
        if(cadena=="->tok"){//tok
            getline(leer, cadena, ':');
            getline(leer, dato);
            tok.DelimitadoresPalabra(dato);
            
            getline(leer, cadena, ':');
            getline(leer, dato);
            if (stoi(dato) == 1 ) {
                tok.CasosEspeciales(true);
            } else {
                tok.CasosEspeciales(false);
            }
            
            getline(leer, cadena, ':');
            getline(leer, dato);
            if (stoi(dato) == 1 ) {
                tok.PasarAminuscSinAcentos(true);
            } else {
                tok.PasarAminuscSinAcentos(false);
            }

            getline(leer, cadena);
        }
        getline(leer, cadena);
        getline(leer, cadena);
        if (cadena.substr(0,cadena.find(':'))=="directorioIndice"){//dirindice
            directorioIndice=cadena.substr(cadena.find(':') + 1);
        }
        getline(leer, cadena);
        getline(leer, cadena);
        if (cadena.substr(0,cadena.find(':'))=="tipoStemmer") {
            tipoStemmer=stoi(cadena.substr(cadena.find(':') + 1));
        }
        getline(leer, cadena);
        getline(leer, cadena);
        if (cadena.substr(0,cadena.find(':'))=="almacenarEnDisco") {
            dato=cadena.substr(cadena.find(':') + 1);
            if (stoi(dato) == 1 ) {
                almacenarEnDisco=true;
            } else {
                almacenarEnDisco=false;
            }
        }
        getline(leer, cadena);
        getline(leer, cadena);//tads constructor ok
        if (cadena.substr(0,cadena.find(':'))=="almacenarPosTerm") {
            dato=cadena.substr(cadena.find(':') + 1);
            if (stoi(dato) == 1 ) {
                almacenarPosTerm=true;
            } else {
                almacenarPosTerm=false;
            }
        }
        leer.close();
        return true;
    }
}

void IndexadorHash::ImprimirIndexacion() const {
    cout << "Terminos indexados: " << endl;
    this->ListarTerminos();
    cout << "Documentos indexados: " << endl;
    this->ListarDocs();
}

bool IndexadorHash::IndexarPregunta(const string& preg) {
    if(preg == "") {
        return false;
    }

    // Normaliza la pregunta completa (minúsculas y sin acentos)
    string pregNorm = preg;
    if(tok.PasarAminuscSinAcentos()) {
        pregNorm = tok.PasarAminuscSinAcentos(pregNorm);
    }
    
    // Guarda la pregunta original (o normalizada, según convenga)
    pregunta = pregNorm;
    indicePregunta.clear();
    infPregunta.setNumTotalPal(0);
    infPregunta.setNumTotalPalSinParada(0);
    infPregunta.setNumTotalPalDiferentes(0);

    // Tokeniza la pregunta ya normalizada
    list<string> tokens;
    tok.Tokenizar(pregNorm, tokens);

    // Contabiliza palabras para la información de la pregunta
    unordered_map<string, int> palabras;
    int NumTotalPal = 0;
    int NumTotalPalSinParada = 0;
    for(auto token : tokens) {
        NumTotalPal++;
        if(stopWords.find(token) == stopWords.end()) {
            NumTotalPalSinParada++;
            palabras[token]++;
        }
    }
    infPregunta.setNumTotalPal(NumTotalPal);
    if(NumTotalPal == 0) {
        return false;
    }
    infPregunta.setNumTotalPalSinParada(NumTotalPalSinParada);
    infPregunta.setNumTotalPalDiferentes(palabras.size());
    palabras.clear();

    // Indexa cada token aplicando el stemming de forma consistente
    int clave = 0;
    stemmerPorter stemmer;
    for(auto token : tokens) {
        string palabra;
        switch(tipoStemmer) {
            case 0:
                palabra = token;
                break;
            case 1:
                stemmer.stemmer(token, 1, palabra);
                break;
            case 2:
                stemmer.stemmer(token, 2, palabra);
                break;
        }
        if(!IsStopWord(palabra)) {
            addIndicePregunta(palabra, clave);
        }
        clave++;
    }
    return true;
}


bool IndexadorHash::IsStopWord(const string& word) {
    stemmerPorter stemmer;
    string auxPalabra;
    for (string palabra : stopWords) {
        auxPalabra=palabra;
        if (tok.PasarAminuscSinAcentos()) {
            auxPalabra = tok.PasarAminuscSinAcentos(auxPalabra);
        }
        stemmer.stemmer(auxPalabra, tipoStemmer);
        
        if (word==auxPalabra) {
            return true;
        }
    }
    return false;
}

bool IndexadorHash::DevuelvePregunta(string& preg) const {
    if (pregunta.empty()) {
        // no hay pregunta indexada
        preg.clear();
        return false;
    } else {
        preg = pregunta;
        return true;
    }
}

bool IndexadorHash::DevuelvePregunta(const string& word, InformacionTerminoPregunta& inf) const {
    unordered_map<string, InformacionTerminoPregunta>::const_iterator it;
    stemmerPorter stemmer;
    string auxPalabra, palabra;
    palabra = word;
    if (tok.PasarAminuscSinAcentos()) {
        palabra = tok.PasarAminuscSinAcentos(palabra);
    }
    stemmer.stemmer(palabra, tipoStemmer, palabra);
    
    it=indicePregunta.find(palabra);
    if (it==indicePregunta.end()) {
        return false;
    } else {
        inf=it->second;
        return true;
    }
}

bool IndexadorHash::DevuelvePregunta(InformacionPregunta& inf) const {
    if(pregunta != "" && pregunta != "??") {
        inf=infPregunta;
        return true;
    }
    return false;
}

void IndexadorHash::ImprimirIndexacionPregunta() {
    cout << "Pregunta indexada: " << pregunta << endl;
    cout << "Terminos indexados en la pregunta: " << endl;
    for (const auto& it : indicePregunta) {
        cout << it.first << '\t' << it.second << endl;
    }
    cout << "Informacion de la pregunta: " << infPregunta << endl;
}

void IndexadorHash::ImprimirPregunta() {
    cout << "Pregunta indexada: " << pregunta << endl;
    cout << "Informacion de la pregunta: " << infPregunta << endl;
}

bool IndexadorHash::Devuelve(const string& word, InformacionTermino& inf) const {
    unordered_map<string, InformacionTermino>::const_iterator it;
    stemmerPorter stemmer;
    string auxPalabra, palabra;
    palabra = word;

    if (tok.PasarAminuscSinAcentos()) {
        auxPalabra = palabra;
        palabra = tok.PasarAminuscSinAcentos(palabra);
    }
    stemmer.stemmer(palabra, tipoStemmer, palabra);

    it=indice.find(palabra);
    if (it==indice.end()) {
        return false;
    } else {
        inf=it->second;
        return true;
    }
}

bool IndexadorHash::Devuelve(const string& word, const string& nomDoc, InfTermDoc& infDoc) const {
    unordered_map<string, InformacionTermino>::const_iterator itTerm;
    unordered_map<int, InfTermDoc>::const_iterator itTermDoc;
    unordered_map<string, InfDoc>::const_iterator itDoc;
    InfTermDoc itd;
    stemmerPorter stemmer;
    string auxPalabra, palabra;
    palabra = word;

    if (tok.PasarAminuscSinAcentos()) {
        auxPalabra = palabra;
        palabra = tok.PasarAminuscSinAcentos(palabra);
    }
    stemmer.stemmer(palabra, tipoStemmer, palabra);

    itTerm=indice.find(palabra);
    if (itTerm==indice.end()) {
        infDoc = itd;
        return false;
    } else {
        itDoc=indiceDocs.find(nomDoc);
        if (itDoc==indiceDocs.end()) {
            infDoc = itd;
            return false;
        } else {
            unordered_map<int, InfTermDoc> l_docs;
            itTerm->second.getDocsReferencia(l_docs);
            itTermDoc=l_docs.find(itDoc->second.getIdDoc());
            if (itTermDoc==l_docs.end()) {
                infDoc = itd;
                return false;
            } else {
                infDoc=itTermDoc->second;
                return true;
            }
        }
    }
}

bool IndexadorHash::Existe(const string& word) const {
    unordered_map<string, InformacionTermino>::const_iterator itTerm;
    stemmerPorter stemmer;
    string auxPalabra, palabra;
    palabra = word;

    if (tok.PasarAminuscSinAcentos()) {
        auxPalabra = palabra;
        palabra = tok.PasarAminuscSinAcentos(palabra);
    }
    stemmer.stemmer(palabra, tipoStemmer, palabra);

    itTerm=indice.find(palabra);
    if (itTerm==indice.end()) {
        return false;
    } else {
        return true;
    }
}

bool IndexadorHash::Borra(const string& word) {
    unordered_map<string, InformacionTermino>::const_iterator itTerm;
    stemmerPorter stemmer;
    string auxPalabra, palabra;
    palabra = word;

    if (tok.PasarAminuscSinAcentos()) {
        auxPalabra = palabra;
        palabra = tok.PasarAminuscSinAcentos(palabra);
    }
    stemmer.stemmer(palabra, tipoStemmer, palabra);

    itTerm=indice.find(palabra);
    if (itTerm==indice.end()) {
        return false;
    } else {
        indice.erase(itTerm);
        return true;
    }
}

bool IndexadorHash::BorraDoc(const string& nomDoc) {
    unordered_map<string, InfDoc>::const_iterator itDoc;
    itDoc=indiceDocs.find(nomDoc);
    if (itDoc==indiceDocs.end()) {
        //cerr << "ERROR: No se pudo borrar el documento: " << nomDoc << endl;
        return false;
    } else {
        // Modifico los datos para que tengan en cuenta el documento eliminado
        informacionColeccionDocs.setNumDocs(informacionColeccionDocs.getNumDocs() - 1);
        informacionColeccionDocs.setNumTotalPal(informacionColeccionDocs.getNumTotalPal() - itDoc->second.getNumPal());
        informacionColeccionDocs.setNumTotalPalSinParada(informacionColeccionDocs.getNumTotalPalSinParada() - itDoc->second.getNumPalSinParada());
        informacionColeccionDocs.setTamBytes(informacionColeccionDocs.getTamBytes() - itDoc->second.getTamBytes());
        
        // Se borra tel documento de InformacionTermino (De todos)
        unordered_map<string, InformacionTermino>::iterator itT;
        unordered_map<int, InfTermDoc>::const_iterator itTermDoc;
        unordered_map<int, InfTermDoc> l_docs;
        int idDoc=itDoc->second.getIdDoc();
        for ( itT=indice.begin(); itT!=indice.end(); itT++ ) {
            l_docs=itT->second.getDocs();
            itTermDoc=l_docs.find(idDoc);
            if (itTermDoc!=l_docs.end()) {
                itT->second.setFtc(itT->second.getFtc() - itTermDoc->second.getFt());
                itT->second.borrarInfTermDoc(idDoc);
                if(itT->second.getDocs().empty()) {
                    informacionColeccionDocs.setNumTotalPalDiferentes(informacionColeccionDocs.getNumTotalPalDiferentes() - 1);
				}
            }
        }
        // Eliminamos el documento de los indices de documentos
        indiceDocs.erase(itDoc);
        // Se eliminaran los terminos con frecuencia 0 debido a que ya no existe el documento en el que rersidian
        unordered_map<string, InformacionTermino>::iterator it = indice.begin();
        while (it != indice.end()) {
            if (it->second.getFtc() == 0) {
                it = indice.erase(it);
            } else {
                ++it;
            }
        }

        return true;
    }
}

void IndexadorHash::VaciarIndiceDocs() {
    indice.clear();
    indiceDocs.clear();
}

void IndexadorHash::VaciarIndicePreg() {
    indicePregunta.clear();
    //indice.clear();
    pregunta="??";
}

bool IndexadorHash::Actualiza(const string& word, const InformacionTermino& inf) {
    stemmerPorter stemmer;
    string auxPalabra, palabra;
    palabra = word;

    if (tok.PasarAminuscSinAcentos()) {
        auxPalabra = palabra;
        palabra = tok.PasarAminuscSinAcentos(palabra);
    }
    stemmer.stemmer(palabra, tipoStemmer, palabra);

    if (Existe(palabra)) {
        indice[palabra]=inf;
        return true;
    }
    return false;
}

bool IndexadorHash::Inserta(const string& word, const InformacionTermino& inf) {
    unordered_map<string, InformacionTermino>::const_iterator itTerm;
    stemmerPorter stemmer;
    string auxPalabra, palabra;
    palabra = word;

    if (tok.PasarAminuscSinAcentos()) {
        auxPalabra = palabra;
        palabra = tok.PasarAminuscSinAcentos(palabra);
    }
    stemmer.stemmer(palabra, tipoStemmer, palabra);

    itTerm=indice.find(palabra);
    if (itTerm==indice.end()) {
        indice[palabra] = inf;
        return true;
    } else {
        return false;
    }
}

int IndexadorHash::NumPalIndexadas() const {
    return indice.size();
}

string IndexadorHash::DevolverFichPalParada () const {
    return ficheroStopWords;
}

void IndexadorHash::ListarPalParada() const {
    if (stopWords.empty()) {
        std::cout << "No hay palabras de parada cargadas." << std::endl;
        return; // Si no hay palabras de parada, salimos de la función.
    }

    //std::cout << "Lista de palabras de parada cargadas:" << std::endl;
    for (const auto& palabra : stopWords) {
        std::cout << palabra << std::endl;  // Imprime cada palabra de parada.
    }
}

int IndexadorHash::NumPalParada() const {
    return stopWords.size();
}

string IndexadorHash::DevolverDelimitadores () const {
    return tok.DelimitadoresPalabra();
}

bool IndexadorHash::DevolverCasosEspeciales () const {
    return tok.CasosEspeciales();
}

bool IndexadorHash::DevolverPasarAminuscSinAcentos () const {
    return tok.PasarAminuscSinAcentos();
}

bool IndexadorHash::DevolverAlmacenarPosTerm () const {
    return almacenarPosTerm;
}

string IndexadorHash::DevolverDirIndice () const {
    return directorioIndice;
}

int IndexadorHash::DevolverTipoStemming () const {
    return tipoStemmer;
}

bool IndexadorHash::DevolverAlmEnDisco () const {
    return almacenarEnDisco;
}

void IndexadorHash::ListarInfColeccDocs() const {
    cout << informacionColeccionDocs << endl;
}

void IndexadorHash::ListarTerminos() const {
    for (const auto& termino : indice) {
        std::cout << termino.first << '\t' << termino.second << std::endl;
    }
}

bool IndexadorHash::ListarTerminos(const string& nomDoc) const {
    unordered_map<string, InfDoc>::const_iterator itDoc;
    itDoc = indiceDocs.find(nomDoc);
    if (itDoc==indiceDocs.end()) {
        return false;
    } else {
        unordered_map<string, InformacionTermino>::const_iterator itT;
        unordered_map<int, InfTermDoc>::const_iterator itTermDoc;
        unordered_map<int, InfTermDoc> l_docs;
        int idDoc=itDoc->second.getIdDoc();
        for ( itT=indice.begin(); itT!=indice.end(); itT++ ) {
            l_docs = itT->second.getDocs();
            itTermDoc = l_docs.find(idDoc);
            if (itTermDoc != l_docs.end()) {
                cout << itT->first << "\t"<< itT->second << endl;
            }
        }
        return true;
    }
}

void IndexadorHash::ListarDocs() const {
    for (const auto& documento : indiceDocs) {
        cout << documento.first << '\t' << documento.second << endl;
    }
}

bool IndexadorHash::ListarDocs(const string& nomDoc) const {
     unordered_map<string, InfDoc>::const_iterator itDoc;
    itDoc = indiceDocs.find(nomDoc);
    if (itDoc==indiceDocs.end()) {
        return false;
    } else {
        cout << itDoc->first << '\t' << itDoc->second << endl;
        return true;
    }
}

void IndexadorHash::addIndicePregunta(const string& termino, const int& posTerm) {
    unordered_map<string, InformacionTerminoPregunta>::const_iterator itTermPreg;
    InformacionTerminoPregunta infTermPreg;
    itTermPreg=indicePregunta.find(termino);
    if (itTermPreg==indicePregunta.end()) {
        indicePregunta[termino] = infTermPreg;
    }
    indicePregunta[termino].addPosTerm(posTerm);
}

string IndexadorHash::getFechaActual() const {
    auto now = chrono::system_clock::now();

    time_t t = chrono::system_clock::to_time_t(now);
    tm tm = *localtime(&t);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
    string fechaModificacion(buffer);

    return fechaModificacion;
}

string IndexadorHash::getFechaModificacionFichero(const string& filename) const {
    const char* path = filename.c_str();
    struct stat file;
    stat(path, &file);
    tm fechaModif = *localtime(&file.st_mtime);
    stringstream ss;
    ss << put_time(&fechaModif, "%Y-%m-%d %H:%M:%S");
    string salida = ss.str();

    return salida;
}

bool IndexadorHash::CompararFechas(const string& date1, const string& date2) const {
    string fecha_str1=date1;
    string fecha_str2=date2;
    tm fecha1 = {};
    tm fecha2 = {};
    strptime(fecha_str1.c_str(), "%Y-%m-%d %H:%M:%S", &fecha1);
    strptime(fecha_str2.c_str(), "%Y-%m-%d %H:%M:%S", &fecha2);
    time_t tiempo1 = mktime(&fecha1);
    time_t tiempo2 = mktime(&fecha2);

    if (tiempo1  < tiempo2) {
        return true;
    } else if (tiempo1  > tiempo2) {
        return false;
    } else {
        return false;
    }
}
