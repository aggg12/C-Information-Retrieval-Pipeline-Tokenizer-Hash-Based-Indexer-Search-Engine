//tokenizador.cpp
//alejandro garcia gil 77042008N
#include "tokenizador.h"
#include <algorithm>// para std::remove
#include <cctype>// para isalpha
#include <string>
#include <signal.h>
using namespace std;

Tokenizador::Tokenizador(const string& delimitadoresPalabra, const bool& kcasosEspeciales, const bool& minuscSinAcentos){
    string ss="";
    // Si ya está, no añadir
    for(unsigned int i=0; i<delimitadoresPalabra.length(); i++){
        if(ss.find(delimitadoresPalabra[i])==string::npos){
            ss.push_back(delimitadoresPalabra[i]);
        }
    }
    this->pasarAminuscSinAcentos=minuscSinAcentos;
    this->casosEspeciales=kcasosEspeciales;
    this->delimiters=ss;
}
void Tokenizador::DelimitadoresPalabra(const string& nuevoDelimiters){
    string cadena="";
    for(size_t i=0; i<nuevoDelimiters.length(); i++){
        //delim a 0, evit repetidos
        if(cadena.find(nuevoDelimiters[i])==string::npos){
            cadena.push_back(nuevoDelimiters[i]);
        }
    }
    this->delimiters=cadena;
}

void Tokenizador::AnyadirDelimitadoresPalabra(const string& nuevoDelimiters){
    for(size_t i=0; i<nuevoDelimiters.length(); i++){
        if(delimiters.find(nuevoDelimiters[i])==string::npos){
            //expandiendo delimitadores poniendole caract. que no están
            delimiters.push_back(nuevoDelimiters[i]);
        }
    }
}

string Tokenizador::PasarAminuscSinAcentos(const string& cadena) const {
    string aux = "";
    unsigned char c;
    for(unsigned int i = 0; i < cadena.length(); i++){
        c = static_cast<unsigned char>(cadena[i]);
        if(c >= 'A' && c <= 'Z'){
            c = c + ('a' - 'A');
        }
        switch(c){
            case 192: case 193: case 194: case 195:
            case 196: case 197: case 224: case 225:
            case 226: case 227: case 228: case 229:
                c = 'a';
                break;
            case 200: case 201: case 202: case 203:
            case 232: case 233: case 234: case 235:
                c = 'e';
                break;
            case 204: case 205: case 206: case 207:
            case 236: case 237: case 238: case 239:
                c = 'i';
                break;
            case 210: case 211: case 212: case 213: 
            case 214: case 216: case 242: case 243:
            case 244: case 245: case 246: case 248:
                c = 'o';
                break;
            case 217: case 218: case 219: case 220:
            case 249: case 250: case 251: case 252:
                c = 'u';
                break;
            case 199: case 231:
                c = 'c';
                break;
            case 209: case 241:
                c = 'n';
                break;
            case 221: case 253: case 255:
                c = 'y';
                break;
        }
        aux += c;
    }
    return aux;
}

Tokenizador::Tokenizador(const Tokenizador &t){
    this->casosEspeciales = t.casosEspeciales;
    this->pasarAminuscSinAcentos = t.pasarAminuscSinAcentos;
    this->delimiters = t.delimiters;
}

Tokenizador::Tokenizador(){
    this->casosEspeciales = false;
    this->pasarAminuscSinAcentos = false;
    this->delimiters=",;:.-/+*\\ '\"{}[]()<>¡!¿?&#=\t@"; //inicializar
}

Tokenizador::~Tokenizador(){
    this->casosEspeciales = false;
    this->pasarAminuscSinAcentos = false;
    this->delimiters = "";
}

Tokenizador & Tokenizador::operator=(const Tokenizador &t){
    if(this!=&t){
        this->casosEspeciales=t.casosEspeciales;
        this->pasarAminuscSinAcentos=t.pasarAminuscSinAcentos;
        this->delimiters=t.delimiters;
    }
    return *this;
}

void Tokenizador::Tokenizar(const string& str, list<string>& token)const{
    //delim activo
    char actual;
    //delimitadores
	const char guiones='-';
	const char puntos='.';
	const string url="_:/.?&-=#@";
	const string email=".-_@";
	const string puntocoma=",.";
	const string porcentajedolar="%$";
    //comprobarcadenas
    string texto=str;
	string copiaTexto=texto;
	string auxiliar="";
	string cadenaManejo="";
	string guardaDelimitadores="";
    //control
	bool emailtok=false;
	bool emailerror=false;
	bool numerosespeciales=false;
	bool numeroerror=false;
	string::size_type auxposicion=0;

	if(casosEspeciales){
        //comprobar espacio/saltolinea
		if(static_cast<int>(delimiters.find(" "))==-1){
			guardaDelimitadores.append(" ");
		}
		if(static_cast<int>(delimiters.find("\n"))==-1){
			guardaDelimitadores.append("\n");
		}
	}
	guardaDelimitadores.append(delimiters);
	string noGuion=guardaDelimitadores;
	string noPunto=guardaDelimitadores;
    //primer NO delimitador
	string::size_type ultimaposicion=texto.find_first_not_of(guardaDelimitadores,0);
	string::size_type pos=texto.find_first_of(guardaDelimitadores,ultimaposicion);
    //paso el token para que salga en minus y sin acentos
	if(pasarAminuscSinAcentos){
		texto=PasarAminuscSinAcentos(texto);
		copiaTexto=texto;
	}
	token.clear();

	while(string::npos!=ultimaposicion || string::npos!=pos)
	{
		auxiliar=texto.substr(ultimaposicion, pos-ultimaposicion);
		//caso especial guardando delim detectado
		if(casosEspeciales){ 
			actual=texto[pos];
			 if(pos!=std::string::npos){//si no hay posiciones, no se almacena token+delim
				cadenaManejo=auxiliar+actual; // String hasta el delimitador (incluyendo delimitador)
			} else {
				cadenaManejo=auxiliar;
			}
			//url
			if(cadenaManejo.substr(0,4)=="ftp:" || cadenaManejo.substr(0,5)=="http:" || cadenaManejo.substr(0,6)=="https:") {
				if(static_cast<int>(pos!=std::string::npos && guardaDelimitadores.find(actual))==-1){
                    //miro que no sea delim 
					pos = texto.find_first_of(guardaDelimitadores, pos+1);
				//miro si no es delim de url
				}else if(static_cast<int>(url.find(actual))!=-1){
					if(pos!=texto.length()-1){
						if(static_cast<int>(guardaDelimitadores.find(texto[pos+1]))==-1 || (static_cast<int>(guardaDelimitadores.find(texto[pos+1]))!=-1 && static_cast<int>(url.find(texto[pos+1]))!=-1)) {
							pos=texto.find_first_of(guardaDelimitadores, pos+1);
							continue;
						}
					} 
					if ((cadenaManejo!="ftp:" && cadenaManejo!="http:" && cadenaManejo!="https:")){
						pos+=1;
					} 
				}
				auxiliar=texto.substr(ultimaposicion, pos-ultimaposicion);
			}
            //delim con pnto o coma
			else if((static_cast<int>(puntocoma.find(actual))!=-1 || static_cast<int>(puntocoma.find(texto[ultimaposicion-1]))!=-1 || cadenaManejo.find_first_of(porcentajedolar, 0)!=std::string::npos) && Check(texto[ultimaposicion-1], auxiliar, puntocoma, porcentajedolar) && !numerosespeciales && !numeroerror) {
				if(texto[ultimaposicion-1]==',' || texto[ultimaposicion-1]=='.'){
                    //se pone 0 al principio de posible num que sin num a la izzq del punto o coma
                    ultimaposicion-=1;
					//quita comas y puntos de antes(1 vez por numero)
					for(int i=ultimaposicion-1; i>=0; i--){
						if(texto[i]==','||texto[i]=='.'){
							texto.erase(i,1);
							ultimaposicion-=1;
						}else{
							break;
						}
					}
					copiaTexto=texto;
					texto.insert(ultimaposicion,"0");
					pos=texto.find_first_of(guardaDelimitadores, ultimaposicion+2);
					continue;
				}
				size_t especial=texto.find_first_of(porcentajedolar, ultimaposicion);
				//agranda token siempre y cuando este rodeado de numeros, y si es "%" o "$" se parar� y pondra estos caracteres en el siguiente token 
				if(actual!='$' && actual!='%' && isdigit(texto[pos-1]) && isdigit(texto[pos+1])){
					if(texto[especial+1]==' ' && especial<=pos && especial>=ultimaposicion){
						pos=especial;
						numerosespeciales=true;
						continue;
					}
					auxposicion=texto.find_first_of(guardaDelimitadores,pos+1);
					// Comprueba si el token extendido es un numero. Si no, determina que el posible token no es un numero con puntos y comas
					if(Check(texto[ultimaposicion-1], texto.substr(ultimaposicion, auxposicion - ultimaposicion), puntocoma, porcentajedolar)) {
						pos=texto.find_first_of(guardaDelimitadores, pos+1);
						continue;
					}else{
						numeroerror=true;
						texto=copiaTexto;
						if(!isdigit(texto[ultimaposicion])){
							ultimaposicion+=1;
						}
						pos=texto.find_first_of(guardaDelimitadores, ultimaposicion);
						continue;
					}
				}else if(especial>=ultimaposicion && especial<=pos && texto[especial+1]==' ' && auxiliar.find_first_of(puntocoma,0)!=std::string::npos) {
					pos=especial;
					numerosespeciales=true;
					continue;
				}else if(!isdigit(texto[pos+1]) && pos!=texto.length()-1){
					numeroerror=true;
					texto=copiaTexto;
					if(!isdigit(texto[ultimaposicion])){
						ultimaposicion+=1;
					}
					pos=texto.find_first_of(guardaDelimitadores, ultimaposicion);
					continue;
				}else if(especial!=std::string::npos && texto[especial+1]!=' '){
					numeroerror=true;
					texto=copiaTexto;
					if(!isdigit(texto[ultimaposicion])){
						ultimaposicion+=1;
					}
					pos=texto.find_first_of(guardaDelimitadores, ultimaposicion);
					continue;
				}
			}else if(numerosespeciales){
				if(texto[pos+1]==' '){//solo token es numero con ./, si siguiente a %/$ es espacioblanco
					token.push_back(auxiliar);
					ultimaposicion=pos;
					pos+=1;
					auxiliar=texto.substr(ultimaposicion, pos-ultimaposicion);
					numerosespeciales=false;
					copiaTexto=texto;
				}else{
					pos=texto.find_first_of(guardaDelimitadores, ultimaposicion);
					auxiliar=texto.substr(ultimaposicion, pos-ultimaposicion);
					token.push_back(auxiliar);
					ultimaposicion=texto.find_first_not_of(guardaDelimitadores, pos);
					pos=texto.find_first_of(guardaDelimitadores, ultimaposicion);//corregir antes de tads
					numerosespeciales=false;
					numeroerror=true;
					copiaTexto=texto;
					continue;
				}
			}
            //mails, comprobar todas restricciones de @
			else if(static_cast<int>(cadenaManejo.find("@"))!=-1 && static_cast<int>(email.find(actual))!=-1 && static_cast<int>(guardaDelimitadores.find(texto[pos-1]))==-1 && static_cast<int>(guardaDelimitadores.find(texto[pos+1]))==-1 && !emailerror && pos!=texto.length()-1) {
				if (!emailtok && auxiliar.find_first_of(guardaDelimitadores,0)!=std::string::npos){
                //compruebo que no tenga delimitador antes de @ porque no se tomaría como mail
					emailerror=true;
				}else{
                //solo un @
					if(actual=='@' && emailtok){
						emailerror=true;
						pos=texto.find_first_of(guardaDelimitadores, ultimaposicion);
					}else{
						emailtok=true;
						pos=texto.find_first_of(guardaDelimitadores, pos+1);
					}
				}
				continue;
			}else if(actual=='@' && emailerror){
				emailerror=false;
			}//acronimos
			else if(actual==puntos && static_cast<int>(guardaDelimitadores.find(texto[pos-1]))==-1 && static_cast<int>(guardaDelimitadores.find(texto[pos+1]))==-1 && pos!=texto.length()-1){
                //delim de acronimos, no rodeado de mas delim (tads)
				if(noPunto.find_first_of(".",0)!=std::string::npos){
					noPunto.erase(std::remove(noPunto.begin(), noPunto.end(), '.'),noPunto.end());
                    //borro en caso de que sí
				}
				if(auxiliar.find_first_of(noPunto,0)==std::string::npos){
					pos=texto.find_first_of(guardaDelimitadores,pos+1);
					continue;
				}
			}
			//guion
			else if (actual==guiones && static_cast<int>(guardaDelimitadores.find(texto[pos-1]))==-1 && static_cast<int>(guardaDelimitadores.find(texto[pos+1]))==-1 && pos!=texto.length()-1){
                //que antes/despues no haya delim (tad)
				if(noGuion.find_first_of("-",0)!=std::string::npos){
					noGuion.erase(std::remove(noGuion.begin(), noGuion.end(), '-'),noGuion.end());
				}
				if(auxiliar.find_first_of(noGuion,0)==string::npos){
					pos=texto.find_first_of(guardaDelimitadores, pos+1);
					continue;
				}
			}
		}
        //reseteo flags y almacenamiento tokens
		emailtok=false;
		numeroerror=false;
		copiaTexto=texto;
		token.push_back(auxiliar);
		ultimaposicion=texto.find_first_not_of(guardaDelimitadores,pos);
		pos=texto.find_first_of(guardaDelimitadores,ultimaposicion);
	}
}




void Tokenizador::CasosEspeciales(const bool& nuevo){
    this->casosEspeciales = nuevo;
}

bool Tokenizador::CasosEspeciales() const{
    return casosEspeciales;
}

bool Tokenizador::Check(const char& anterior, const string& check, const string& delimitadores, const string& especialesdelimitadores) const {
    int cont = 0;
    string d=delimitadores+especialesdelimitadores+" ";
    string t=check;

    for(unsigned int i=0; i<t.length(); i++){
        bool esNumero = isdigit(t[i]);
        bool esDelimitador=(d.find(t[i]) < d.length());
        if(!esNumero && !esDelimitador){
            return false;
        } else if(delimitadores.find(t[i]) < delimitadores.length()){
            cont++;
        }
    }
    return true;
}

bool Tokenizador::Tokenizar(const string& i, const string& f) const {
    ifstream leer(i);
    ofstream escribir(f);
    string cadena;
    list<string> auxiliar;
    if(!leer){
        cerr<< "ERROR: No existe el archivo: " <<i<< '\n';
        return false;
    }
    if(!escribir){
        cerr<< "ERROR: No se pudo abrir el archivo: " <<f<< '\n';
        return false;
    }
    while(getline(leer, cadena)){
        if(!cadena.empty()){
            list<string> token;
            Tokenizar(cadena, token);
            auxiliar.insert(auxiliar.end(), token.begin(), token.end());
        }
    }
    for (size_t i = 0; i < auxiliar.size(); i++) {
        escribir << *next(auxiliar.begin(), i) << '\n';
    }
    return true;
}

bool Tokenizador::Tokenizar(const string& i) const {
    list<string> auxiliar;
    string ficherolectura = i;
    string ficheroescritura = i + ".tk";
    list<string> token;
    
    ifstream lectura(ficherolectura, ios::binary | ios::ate);
    if(!lectura.is_open()){
        cerr << "ERROR: No existe el archivo: " << ficherolectura << '\n';
        return false;
    }

    streamsize tamlectura = lectura.tellg();
    lectura.seekg(0, ios::beg);

    char* input_data = new char[tamlectura];
    if (!lectura.read(input_data, tamlectura)) {
        cerr << "ERROR: No se pudo acceder" << '\n';
        delete[] input_data;
        return false;
    }
    lectura.close();

    char* inicio = input_data;
    for (std::streamsize i = 0; i < tamlectura; i++) {
        if (input_data[i] == '\n') {
            Tokenizar(string(inicio, input_data + i - inicio), token);
            auxiliar.insert(auxiliar.end(), token.begin(), token.end());
            inicio = input_data + i + 1;
        }
    }
    if(inicio < input_data + tamlectura) {
        Tokenizar(string(inicio, input_data + tamlectura - inicio), token);
        auxiliar.insert(auxiliar.end(), token.begin(), token.end());
    }

    ostringstream output_stringstream;
    size_t tamAuxiliar = auxiliar.size();
    for (size_t i = 0; i < tamAuxiliar; i++){
        output_stringstream << *next(auxiliar.begin(), i) << '\n';
    }

    ofstream escritura(ficheroescritura, ios::binary);
    if(!escritura.is_open()){
        cerr << "ERROR: No se pudo abrir el archivo: " << ficheroescritura << '\n';
        delete[] input_data;
        return false;
    }
    
    const string& cadenaEscritura = output_stringstream.str();
    escritura.write(cadenaEscritura.data(), cadenaEscritura.size());
    escritura.close();
    delete[] input_data;
    return true;
}

bool Tokenizador::TokenizarListaFicheros(const string& i) const { 
    ifstream lectura(i);
    if(!lectura){
        cerr << "ERROR: No existe el archivo: " << i << '\n';
        return false;
    }
    string cadena;
    bool tokenizacion = true;
    while(getline(lectura, cadena)){
        if(!Tokenizar(cadena) && !cadena.empty()){
            tokenizacion = false;
        }
    }
    lectura.close();
    return tokenizacion;
}

bool Tokenizador::TokenizarDirectorio(const string& i) const {
    struct stat directorio;
    //verificar ruta
    if(stat(i.c_str(), &directorio) == -1 || !S_ISDIR(directorio.st_mode)) {
        return false;
    }
    string comando="find " +i+ " -follow | sort > .lista_fich";
    int resultado=system(comando.c_str());
    //lista ordenada de fichs del directorio, que se utiliza para tokenizar
    if(resultado!=0){
        return false;
    }
    return this->TokenizarListaFicheros(".lista_fich");
}


string Tokenizador::DelimitadoresPalabra() const {
    return delimiters;
}


void Tokenizador::PasarAminuscSinAcentos(const bool& nuevoPasarAminuscSinAcentos) {
    this->pasarAminuscSinAcentos=nuevoPasarAminuscSinAcentos;
}

bool Tokenizador::PasarAminuscSinAcentos() const {
    return pasarAminuscSinAcentos;
}

ostream& operator<<(ostream &os, const Tokenizador &t){
    os << "DELIMITADORES: " << t.delimiters << " TRATA CASOS ESPECIALES: " << t.casosEspeciales 
       << " PASAR A MINUSCULAS Y SIN ACENTOS: " << t.pasarAminuscSinAcentos;
    return os;
};