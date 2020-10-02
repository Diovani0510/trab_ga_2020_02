// Nome: Bruno
// Nome: Diovani
// Nome: Gustavo

//#######################################################################################################################
//#################################################  INCLUDES  ##########################################################
//#######################################################################################################################

#include <stdlib.h> 
#include <locale.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//#######################################################################################################################
//#######################################################################################################################
//#######################################################################################################################

//#######################################################################################################################
//############################################  FUNÇÕES GENÉRICAS  ######################################################
//#######################################################################################################################

// Função genérica utilizada em alguns métodos e construtores
bool stringContemTexto(string referencia, string texto) {
	return referencia.find(texto) != std::string::npos;
}

// Função fenérica de confirmação de operação
bool confirmaOperacao() {
	char opcao = 'a';
	// Enquanto opcao não for S nem N
	while (opcao != 'S' && opcao != 's' && opcao != 'N' && opcao != 'n') {
		cout << "\nTem certeza? (S/N): ";
		cin >> opcao;
	}
	return opcao == 'S' || opcao == 's';
}

//#######################################################################################################################
//#######################################################################################################################
//#######################################################################################################################

//#######################################################################################################################
//#####################################################  ENUMS  #########################################################
//#######################################################################################################################

// Enum para tipo de filtro
enum TipoFiltro {
	DATA,
	HORA,
	INT,
	STRING
};

// Enum para atributo sobre o qual o filtro será aplicado
enum AtributoRegistroFiltro {
	DATA_HORA,
	CODIGO,
	MENSAGEM,
	CLASSIFICACAO,
	PRIORIDADE,
	PROTOCOLO,
	ORIGEM_IP,
	ORIGEM_PORTA,
	DESTINO_IP,
	DESTINO_PORTA
};

//#######################################################################################################################
//#######################################################################################################################
//#######################################################################################################################

//#######################################################################################################################
//###################################################  CLASSES  #########################################################
//#######################################################################################################################

//--------------------------------------------------  DATA E HORA  ------------------------------------------------------

class Data {
	private:
		int dia, mes, ano;

	public:
		Data() {}
		Data(int dia, int mes, int ano) {
			this->dia = dia;
			this->mes = mes;
			this->ano = ano;
		}
		~Data() {}

		int getDia() { return this->dia; }
		int getMes() { return this->mes; }
		int getAno() { return this->ano; }
		void setDia(int dia) { this->dia = dia; }
		void setMes(int mes) { this->mes = mes; }
		void setAno(int ano) { this->ano = ano; }

		// Busca o ano atual utilizando a biblioteca time.h
		int getAnoAtual() {
			time_t t = time(NULL);
			struct tm *tm = localtime(&t);

			int ano = tm->tm_year + 1900; 
			return ano;
		}

		// Converte o mes lido no arquivo de logs de string para int
		int mesStrParaMesInt(string mes) {
			if (mes == "Jan") return 1;
			else if (mes == "Feb") return 2;
			else if (mes == "Mar") return 3;
			else if (mes == "Apr") return 4;
			else if (mes == "May") return 5;
			else if (mes == "Jun") return 6;
			else if (mes == "Jul") return 7;
			else if (mes == "Aug") return 8;
			else if (mes == "Sep") return 9;
			else if (mes == "Oct") return 10;
			else if (mes == "Nov") return 11;
			else if (mes == "Dec") return 12;
			else return 0;
		}

		bool anoBissexto(int ano) {
			return (((ano % 4 == 0) && (ano % 100 != 0)) || (ano % 400 == 0));  
		}

		bool dataValida() {
			if (this->ano > 9999 || this->ano < 1800)  return false; 
			if (this->mes < 1 || this->mes > 12) return false; 
			if (this->dia < 1 || this->dia > 31)  return false; 
			if (this->mes == 2) {
				if (anoBissexto(this->ano)) return this->dia <= 29;
				else return this->dia <= 28;
			}
			if (this->mes == 4 || this->mes == 6 || this->mes == 9 || this->mes == 11) return this->dia <= 30;

			return true;
		}

		friend std::ostream& operator<<(ostream& out, Data& d) {
            out << d.ano << "-" << std::setfill('0') << std::setw(2) << d.mes << "-" << std::setfill('0') << std::setw(2) << d.dia;
			return out;
		}

		friend istream& operator>>(istream& in, Data& d) {
			char sep;
			char sep2;
			in >> d.dia >> sep >> d.mes >> sep2 >> d.ano;
			return in;
		}
};

class Hora {
	private:
		int hora, minuto, segundo;

	public:
		Hora() {}
		Hora(int hora, int minuto, int segundo) {
			this->hora = hora;
			this->minuto = minuto;
			this->segundo = segundo;
		}
		~Hora() {}

		int getHora() { return this->hora; }
		int getMinuto() { return this->minuto; }
		int getSegundo() { return this->segundo; }
		void setHora(int hora) { this->hora = hora; }
		void setMinuto(int minuto) { this->minuto = minuto; }
		void setSegundo(int segundo) { this->segundo = segundo; }

		bool horaValida() {
			if (this->hora < 0 || this->hora > 23) return false;
			if (this->minuto < 0 || this->minuto > 60) return false;
			if (this->segundo < 0 || this->segundo > 60) return false;
			return true;
		}

		friend std::ostream& operator<<(ostream& out, Hora& h) {
            out << std::setfill('0') << std::setw(2) << h.hora << ":" << std::setfill('0') << std::setw(2) << h.minuto << ":" << std::setfill('0') << std::setw(2) << h.segundo;
			return out;
		}

		friend istream& operator>>(istream& in, Hora& h) {
			char sep;
			char sep2;
			in >> h.hora >> sep >> h.minuto >> sep2 >> h.segundo;
			return in;
		}
};

class DataHora {
	private:
		Data data;
		Hora hora;

	public:
		DataHora() {}
		DataHora(Data data, Hora hora) {
			this->data = data;
			this->hora = hora;
		}
		~DataHora() {}

		Data getData() { return this->data; }
		Hora getHora() { return this->hora; }
		void setData(Data data) { this->data = data; }
		void setHora(Hora hora) { this->hora = hora; }

		friend std::ostream& operator<<(ostream& out, DataHora& dH) {
			out << dH.data << "T" << std::setfill('0') << std::setw(2) << dH.hora;
			return out;
		}
};

//-----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------  FILTROS  --------------------------------------------------------

class Filtro {
	private:
		TipoFiltro tipo;
		AtributoRegistroFiltro atributo;

	public:
		Filtro() {}
		~Filtro() {}
		TipoFiltro getTipoFiltro() { return this->tipo; }
		AtributoRegistroFiltro getAtributo() { return this->atributo; }
		void setTipoFiltro(TipoFiltro tipo) { this->tipo = tipo; }
		void setAtributo(AtributoRegistroFiltro atributo) { this->atributo = atributo; }

		// Converte o Enum de atributo para String
		string atributoEnumParaString(AtributoRegistroFiltro atributoEnum) {
			switch (atributoEnum) {
				case DATA_HORA:
					return "Data";
				break;

				case CODIGO:
					return "Código";
				break;

				case MENSAGEM:
					return "Mensagem";
				break;

				case CLASSIFICACAO:
					return "Classificacao";
				break;

				case PRIORIDADE:
					return "Prioridade";
				break;

				case PROTOCOLO:
					return "Protocolo";
				break;

				case ORIGEM_IP:
					return "Origem IP";
				break;

				case ORIGEM_PORTA:
					return "Origem Porta";
				break;

				case DESTINO_IP:
					return "Destino IP";
				break;

				case DESTINO_PORTA:
					return "Destino Porta";
				break;
				
				default:
					return "Atributo invalido";
					break;
			}
		}
};

class FiltroData:public Filtro {
	private:
		Data dataInicial;
		Data dataFinal;
	
	public:
		FiltroData(Data dataInicial, Data dataFinal) { 
			this->setTipoFiltro(DATA);
			this->setAtributo(DATA_HORA);
			this->dataInicial = dataInicial; 
			this->dataFinal = dataFinal;
		}
		~FiltroData() {}

		Data getDataInicial() { return this->dataInicial; }
		void setDataInicial(Data dataInicial) { this->dataInicial = dataInicial; }
		Data getDataFinal() { return this->dataFinal; }
		void setDataFinal(Data dataFinal) { this->dataFinal = dataFinal; }

		friend std::ostream& operator<<(ostream& out, FiltroData& filtroData) {
			string atributo = filtroData.atributoEnumParaString(filtroData.getAtributo());
			
			out << "Filtro do tipo Data.\tAtributo: " << atributo << "\tData Inicial: " << filtroData.dataInicial << "\tHora Final: " << filtroData.dataFinal;
            return out;
        }
};

class FiltroHora:public Filtro {
	private:
		Hora horaInicial;
		Hora horaFinal;
	
	public:
		FiltroHora(Hora horaInicial, Hora horaFinal) { 
			this->setTipoFiltro(HORA);
			this->setAtributo(DATA_HORA);
			this->horaInicial = horaInicial; 
			this->horaFinal = horaFinal;
		}
		~FiltroHora() {}

		Hora getHoraInicial() { return this->horaInicial; }
		void setHoraInicial(Hora horaInicial) { this->horaInicial = horaInicial; }
		Hora getHoraFinal() { return this->horaFinal; }
		void setHoraFinal(Hora horaFinal) { this->horaFinal = horaFinal; }

		friend std::ostream& operator<<(ostream& out, FiltroHora& filtroHora) {
			string atributo = filtroHora.atributoEnumParaString(filtroHora.getAtributo());
			
			out << "Filtro do tipo Hora.\tAtributo: " << atributo << "\tHora Inicial: " << filtroHora.horaInicial << "\tHora Final: " << filtroHora.horaFinal;
            return out;
        }
};

class FiltroInt:public Filtro {
	private:
		int valorInicial;
		int valorFinal;

	public:
		FiltroInt() {}
		FiltroInt(int valorInicial, int valorFinal, AtributoRegistroFiltro atributo) { 
			this->valorInicial = valorInicial;
			this->valorFinal = valorFinal;
		}
		~FiltroInt() {}
		int getValorInicial() { return this->valorInicial; }
		int getValorFinal() { return this->valorFinal; }
		void setValorInicial(int valorInicial) { this->valorInicial = valorInicial; }
		void setValorFinal(int valorFinal) { this->valorFinal = valorFinal; }

		friend std::ostream& operator<<(ostream& out, FiltroInt& filtroInt) {
			string atributo = filtroInt.atributoEnumParaString(filtroInt.getAtributo());
			
			if (filtroInt.valorInicial == filtroInt.valorFinal)  out << "Filtro do tipo Numerico.\tAtributo: " << atributo << "\tValor: " << filtroInt.valorInicial;
			else out << "Filtro do tipo Numerico.\tAtributo: " << atributo << "\tValor Inicial: " << filtroInt.valorInicial << "\tValor Final: " << filtroInt.valorFinal;
            return out;
        }
};

class FiltroString:public Filtro {
	private:
		string valor;
	
	public:
		FiltroString(string valor, AtributoRegistroFiltro atributo) { 
			this->setTipoFiltro(STRING);
			this->setAtributo(atributo);
			this->valor = valor; 
		}
		~FiltroString() {}

		string getValor() { return this->valor; }
		void setValor(string valor) { this->valor = valor; }

		friend std::ostream& operator<<(ostream& out, FiltroString& filtroString) {
			string atributo = filtroString.atributoEnumParaString(filtroString.getAtributo());
			
            out << "Filtro do tipo String.\tAtributo: " << atributo << "\tValor: " << filtroString.valor;
            return out;
        }
};

class FiltrosSistema {
	private:
		vector<FiltroData> filtrosData;
		vector<FiltroHora> filtrosHora;
		vector<FiltroInt> filtrosInt;
		vector<FiltroString> filtrosString;
	
	public:
		FiltrosSistema() {}
		~FiltrosSistema() {}

		vector<FiltroData> getFiltrosData() { return this->filtrosData; }
		vector<FiltroHora> getFiltrosHora() { return this->filtrosHora; }
		vector<FiltroInt> getFiltrosInt() { return this->filtrosInt; }
		vector<FiltroString> getFiltrosString() { return this->filtrosString; }
		void setFiltrosData(vector<FiltroData> filtrosData) { this->filtrosData = filtrosData; }
		void setFiltrosHora(vector<FiltroHora> filtrosHora) { this->filtrosHora = filtrosHora; }
		void setFiltrosInt(vector<FiltroInt> filtrosInt) { this->filtrosInt = filtrosInt; }
		void setFiltrosString(vector<FiltroString> filtrosString) { this->filtrosString = filtrosString; }

		void adicionarFiltroData(FiltroData filtroData) { this->filtrosData.push_back(filtroData); }
		void adicionarFiltroHora(FiltroHora filtroHora) { this->filtrosHora.push_back(filtroHora); }
		void adicionarFiltroInt(FiltroInt filtroInt) { this->filtrosInt.push_back(filtroInt); }
		void adicionarFiltroString(FiltroString filtroString) { this->filtrosString.push_back(filtroString); }

		// Retorna flag que indica se existe algum filtro cadastrado
		bool possuiFiltros() { return this->getFiltrosData().size() > 0 || this->getFiltrosHora().size() > 0 || this->getFiltrosInt().size() > 0 || this->getFiltrosString().size() > 0; }

		// Retorna flag que indica se existe algum filtro do tipo Data cadastrado
		bool possuiFIltrosData() { return this->getFiltrosData().size() > 0; }
		// Retorna flag que indica se existe algum filtro do tipo Hora cadastrado
		bool possuiFIltrosHora() { return this->getFiltrosHora().size() > 0; }
		// Retorna flag que indica se existe algum filtro do tipo int cadastrado
		bool possuiFIltrosInt() { return this->getFiltrosInt().size() > 0; }
		// Retorna flag que indica se existe algum filtro do tipo string cadastrado
		bool possuiFIltrosString() { return this->getFiltrosString().size() > 0; }

		// Limpa os filtros
		void limparFiltros() {
			if (this->possuiFiltros()) {
				if (this->possuiFIltrosData()) this->filtrosData.clear();
				if (this->possuiFIltrosHora()) this->filtrosHora.clear();
				if (this->possuiFIltrosInt()) this->filtrosInt.clear();
				if (this->possuiFIltrosString()) this->filtrosString.clear();
			}
		}
};

//-----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------  REGISTRO  -------------------------------------------------------

class Registro {
	private:
		DataHora dataHora;
		string codigo;
		string mensagem;
		string classificacao;
		int prioridade;
		string protocolo;
		string origemIP;
		int origemPorta;
		string destinoIP;
		int destinoPorta;

	public:
		// Recebe a linha lida do arquivo de logs
		Registro(string linha) {
			DataHora dataHora = this->leDataHora(linha.substr(0, 15));

			string codigo = this->separarValores(linha, ' ')[5];
			codigo = codigo.substr(1, codigo.find(']') - 1);

			string mensagem = linha.substr(linha.find(']') + 2);

			string labelClassificacao = " [Classification: ";
			string labelPrioridade = " [Priority: ";

			// Verifica se a linha possui o atributo de classificação baseado na label de classificação (o atributo classificação é opcional)
			bool possuiClassificacao = stringContemTexto(mensagem, labelClassificacao);
			// Verifica se a linha possui o atributo de prioridade baseado na label de prioridade (o atributo prioridade é opcional)
			bool possuiPrioridade = stringContemTexto(mensagem, labelPrioridade);

			string classificacao;
			string prioridade;

			int posicaoFinalMensagem = 0, posicaoFinalClassificacao = 0, posicaoFinalPrioridade = 0;
			int posicaoInicialMensagem = 0, posicaoInicialClassificacao = 0; int posicaoInicialPrioridade;
			
			// Se linha possui classificacao
			if (possuiClassificacao) {
				posicaoFinalMensagem = mensagem.find(labelClassificacao);
				posicaoInicialClassificacao = posicaoFinalMensagem + labelClassificacao.size();
				classificacao = mensagem.substr(posicaoInicialClassificacao);
				posicaoInicialClassificacao = 0;
				mensagem = mensagem.substr(posicaoInicialMensagem, posicaoFinalMensagem);
				// Se linha possui prioridade
				if (possuiPrioridade) {
					posicaoFinalClassificacao = classificacao.find(labelPrioridade) - 1;
					posicaoInicialPrioridade = classificacao.find(labelPrioridade) + labelPrioridade.size();
					posicaoFinalPrioridade = 1;
					prioridade = classificacao.substr(posicaoInicialPrioridade, posicaoFinalPrioridade);
					classificacao = classificacao.substr(posicaoInicialClassificacao, posicaoFinalClassificacao);
				// Se linha não possui prioridade
				} else {
					posicaoFinalClassificacao = classificacao.find('{') - 2;
					classificacao = classificacao.substr(posicaoInicialClassificacao,posicaoFinalClassificacao);
				}
			// Se linha possui prioridade e não possui classificação
			} else if (possuiPrioridade) {
				posicaoInicialPrioridade = mensagem.find(labelPrioridade) + labelPrioridade.size();
				posicaoFinalPrioridade = 1;
				prioridade = mensagem.substr(posicaoInicialPrioridade, posicaoFinalPrioridade);
				posicaoFinalMensagem = mensagem.find(labelPrioridade);
				mensagem = mensagem.substr(posicaoInicialMensagem, posicaoFinalMensagem);
			// Se linha não possui prioridade e nem classificação
			} else {
				posicaoFinalMensagem = mensagem.find('{') - 2;
				mensagem = mensagem.substr(posicaoInicialMensagem, posicaoFinalMensagem);
			} 
	
			string protocolo = linha.substr(linha.find('{') + 1);
			protocolo = protocolo.substr(0, protocolo.find('}'));


			string ipPortaOrigem = linha.substr(linha.find('}') + 2);
			ipPortaOrigem = ipPortaOrigem.substr(0, ipPortaOrigem.find(" -> "));

			string origemIP;
			string origemPorta;
			// Verifica se linha possui porta de origem baseado no caractere ':'
			bool possuiPortaOrigem = stringContemTexto(ipPortaOrigem, ":");

			if (possuiPortaOrigem) {
				origemIP = this->separarValores(ipPortaOrigem, ':')[0];
				origemPorta = this->separarValores(ipPortaOrigem, ':')[1];
			} else origemIP = ipPortaOrigem;

			string ipPortaDestino = linha.substr(linha.find(" -> ") + 4);
			
			string destinoIP;
			string destinoPorta;
			// Verifica se linha possui porta baseado no caractere ':'
			bool possuiPortaDestino = stringContemTexto(ipPortaDestino, ":");

			if (possuiPortaDestino) {
				destinoIP = this->separarValores(ipPortaDestino, ':')[0];
				destinoPorta = this->separarValores(ipPortaDestino, ':')[1];
			} else destinoIP = ipPortaOrigem;

			this->dataHora = dataHora;
			this->codigo = codigo;
			this->mensagem = mensagem;
			if (!classificacao.empty()) this->classificacao = classificacao;
			if (!prioridade.empty()) this->prioridade = this->stringParaInt(prioridade);
			this->protocolo = protocolo;
			this->origemIP = origemIP;
			if (!origemPorta.empty()) this->origemPorta = this->stringParaInt(origemPorta);
			else this->origemPorta = -1;
			this->destinoIP = destinoIP;
			if (!destinoPorta.empty()) this->destinoPorta = this->stringParaInt(origemPorta);
			else this->origemPorta = -1;
		}

		~Registro() {}

		DataHora getDataHora() { return this->dataHora; }
		string getCodigo() { return this->codigo; }
		string getMensagem() { return this->mensagem; }
		string getClassificacao() { return this->classificacao; }
		int getPrioridade() { return this->prioridade; }
		string getProtocolo() { return this->protocolo; }
		string getOrigemIP() { return this->origemIP; }
		int getOrigemPorta() { return this->origemPorta; }
		string getDestinoIp() { return this->destinoIP; }
		int getDestinoPorta() { return this->origemPorta; }

		void setDataHora(DataHora dataHora) { this->dataHora = dataHora; }
		void setCodigo(string codigo) { this->codigo = codigo; }
		void setMensagem(string mensagem) { this->mensagem = mensagem; }
		void setClassificacao(string classificacao) { this->classificacao = classificacao; }
		void setPrioridade(int prioridade) { this->prioridade = prioridade; }
		void setProtocolo(string protocolo) { this->protocolo = protocolo; }
		void setOrigemIP(string origemIp) { this->origemIP = origemIP; }
		void setOrigemPorta(int origemPorta) { this->origemPorta = origemPorta; }
		void setDestinoIp(string destinoIp) { this->destinoIP = destinoIp; }
		void setDestinoPorta(int origemPorta) { this->origemPorta = origemPorta; }

		// Monta um objeto do tipo DataHora utilizando a substring da linha do arquivo correspondente a estes valores
		DataHora leDataHora(string dataHoraStr) {
			// Separa a substring em mês, dia e horário
			vector<string> valoresDataHora = separarValores(dataHoraStr, ' ');

			// Caso o dia tenha apenas um dígito, ajusta as posições do vector
			if (valoresDataHora[1].empty()) {
				valoresDataHora[1] = valoresDataHora[2];
				valoresDataHora[2] = valoresDataHora[3];
			}

			// Separa o horário em hora, minuto e segundo
			vector<string> valoresHora = separarValores(valoresDataHora[2], ':');

			
			Data data;
			data.setAno(data.getAnoAtual());
			data.setMes(data.mesStrParaMesInt(valoresDataHora[0]));
			data.setDia(this->stringParaInt(valoresDataHora[1]));
			Hora hora;
			hora.setHora(this->stringParaInt(valoresHora[0]));
			hora.setMinuto(this->stringParaInt(valoresHora[1]));
			hora.setSegundo(this->stringParaInt(valoresHora[2]));
			DataHora dataHora;
			dataHora.setData(data);
			dataHora.setHora(hora);
			return dataHora;
		}

		// Separa os valores de uma string de acordo com o caractere separador
		vector<string> separarValores(string linha, char separador) {
            vector<string> valores;
            string valor;
            istringstream registro_stream(linha);
            while (getline(registro_stream, valor, separador)) {
                valores.push_back(valor);
            }
            return valores;
        }

		// Converte um valor string para int
		int stringParaInt(string original) {
            int resultado;
            stringstream string_stream(original);
            string_stream >> resultado;
            return resultado;
        }
};

//-----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------  REGISTRO  -------------------------------------------------------

class Sistema {
	private:
		vector<Registro*> logs;
	public:
		Sistema(string pathArquivo) {
			this->populaLogs(pathArquivo);
		}
		~Sistema() {}

		vector<Registro*> getLogs() { return this->logs; }
		void setLogs(vector<Registro*> logs) { this->logs = logs; }

		// Popula o vector de logs percorrendo o arquivo
		void populaLogs(string pathArquivo) {
			string linha;
            ifstream arquivo (pathArquivo.c_str());
            if (arquivo.is_open()) {
				cout << "lendo arquivo..." << endl;
                while (arquivo.good()) {
                    getline (arquivo, linha);
					Registro registro = Registro(linha);
                    this->logs.push_back(&registro);
                }
            }
            arquivo.close();
		}		
};

//-----------------------------------------------------------------------------------------------------------------------

//#######################################################################################################################
//#######################################################################################################################
//#######################################################################################################################

//#######################################################################################################################
//###############################################  ADICIONAR FILTRO  ####################################################
//#######################################################################################################################


FiltroData criarFiltroData() {

	Data dataInicial, dataFinal;
	bool dataInicialValida = true, dataFinalValida = true;

	bool confirmou = false;

	while (!confirmou) {
		do {
			system("cls");
			if (!dataInicialValida) cout << "Data inicial invalida!" << endl;
			cout << "Data inicial (DD/MM/YYYY): " << endl;
			cin >> dataInicial;
			dataInicialValida = dataInicial.dataValida();
		} while (!dataInicialValida);

		do {
			system("cls");
			cout << "Data inicial: " << dataInicial << endl;
			if (!dataFinalValida) cout << "Data final invalida!" << endl;
			cout << "Data final (DD/MM/YYYY): " << endl;
			cin >> dataFinal;
			dataFinalValida = dataFinal.dataValida();
		} while (!dataFinalValida);

		system("cls");
		cout << "Data inicial: " << dataInicial << endl;
		cout << "Data final: " << dataFinal << endl;

		confirmou = confirmaOperacao();
	}

	return FiltroData(dataInicial, dataFinal);
}

FiltroHora criarFiltroHora() {
	Hora horaInicial, horaFinal;
	bool horaInicialValida = true, horaFinalValida = true;

	bool confirmou = false;

	while (!confirmou) {
		do {
			system("cls");
			if (!horaInicialValida) cout << "Hora inicial invalida!" << endl;
			cout << "Hora inicial (HH:MM:SS): " << endl;
			cin >> horaInicial;
			horaInicialValida = horaInicial.horaValida();
		} while (!horaInicialValida);

		do {
			system("cls");
			cout << "Hora inicial: " << horaInicial << endl;
			if (!horaFinalValida) cout << "Hora final invalida!" << endl;
			cout << "Hora final (HH:MM:SS): " << endl;
			cin >> horaFinal;
			horaFinalValida = horaFinal.horaValida();
		} while (!horaFinalValida);

		system("cls");
		cout << "Hora inicial: " << horaInicial << endl;
		cout << "Hora final: " << horaFinal << endl;

		confirmou = confirmaOperacao();
	}

	return FiltroHora(horaInicial, horaFinal);
}


//---------------------------------------------------  Filtro string  ---------------------------------------------------

FiltroString criarFiltroString(int opcao) {
	system("cls");
	string valorFiltro;
	cout << "Infome o valor do filtro: ";
	cin >> valorFiltro;

	AtributoRegistroFiltro atributo;
	switch (opcao) {
		case 3:
			atributo = CODIGO;
		break;

		case 4:
			atributo = MENSAGEM;
		break;

		case 5:
			atributo = CLASSIFICACAO;
		break;

		case 7:
			atributo = PROTOCOLO;
		break;

		case 8:
			atributo = ORIGEM_IP;
		break;

		case 10:
			atributo = DESTINO_IP;
		break;
		
		default:
			break;
	}

	return FiltroString(valorFiltro, atributo);
}

//-----------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------  Filtro int  ------------------------------------------------------

void exibeOpcoesFiltroInt() {
	cout << "Filtrar valores por:" << endl;
	cout << "1 - Intervalo de valores" << endl;
	cout << "2 - Valor exato" << endl;
}

int menuOpcoesFiltroInt() {
	int opcao = 1;
	do {
		system("cls");
		exibeOpcoesFiltroInt();
		if (opcao < 1 || opcao > 2) cout << "\nOpcao invalida! Informe uma opcao valida: ";
		else cout << "\nOpcao selecionada: ";
		cin >> opcao;
	} while (opcao < 1 || opcao > 2);

	return opcao;
}

FiltroInt criarFiltroInt(int opcao) {
	int opcaoFiltro = 0;
	opcaoFiltro = menuOpcoesFiltroInt();
	int valorInicial;
	int valorFinal;

	system("cls");
	if (opcaoFiltro == 1) {
		cout << "Digite o valor inicial: ";
		cin >> valorInicial;
		cout << "\nDigite o valor final: ";
		cin >> valorFinal;
	} else {
		cout << "Digite o valor exato a ser buscado: ";
		cin >> valorInicial;
		valorFinal = valorInicial;
	}

	AtributoRegistroFiltro atributo;

	switch (opcao) {
		case 6:
			atributo = PRIORIDADE;
		break;

		case 9:
			atributo = ORIGEM_PORTA;
		break;

		case 11:
			atributo = DESTINO_PORTA;
		break;

		default:
			break;
	}
	
	return FiltroInt(valorInicial, valorFinal, atributo);
	
}

//-----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------  Menu adicionar filtro  ------------------------------------------------

void exibeOpcoesMenuAdicionarFiltro() {
	cout << "Adicionar Filtro:" << endl << endl;
	cout << "1- Filtrar por data" <<endl;
	cout << "2- Filtrar por hora" <<endl;
	cout << "3- Filtrar por codigo" <<endl;
	cout << "4- Filtrar por mensagem" <<endl;
	cout << "5- Filtrar por classificacao" <<endl;
	cout << "6- Filtrar por prioridade" <<endl;
	cout << "7- Filtrar por protocolo" <<endl;
	cout << "8- Filtrar por origemIP" <<endl;
	cout << "9- Filtrar por origemPorta" <<endl;
	cout << "10- Filtrar por destinoIP" <<endl;
	cout << "11- Filtrar por destinoPorta" <<endl;
	cout << "0 - Voltar" << endl;
}

int menuAdicionarFiltros() {
	int opcao = 0;
	do {
		system("cls");
		exibeOpcoesMenuAdicionarFiltro();
		if (opcao < 0 || opcao > 11) cout << "\nOpcao invalida! Informe uma opcao valida: ";
		else cout << "\nOpcao selecionada: ";
		cin >> opcao;
	} while (opcao < 0 || opcao > 11 || opcao == 0 && !confirmaOperacao());

	return opcao;
}

void adicionarFiltro(FiltrosSistema &filtros) {
	int opcao = menuAdicionarFiltros();
	if (opcao == 0) return;
	if (opcao == 1) {
		FiltroData filtroData = criarFiltroData();
		filtros.adicionarFiltroData(filtroData);
	}
	if (opcao == 2) {
		FiltroHora filtroHora = criarFiltroHora();
		filtros.adicionarFiltroHora(filtroHora);
	}
	if (opcao == 3 || opcao == 4 || opcao == 5 || opcao == 7 || opcao == 8 || opcao == 10) {
		FiltroString filtroString = criarFiltroString(opcao);
		if (confirmaOperacao()) filtros.adicionarFiltroString(filtroString);
	} else if (opcao == 6 || opcao == 9 || opcao == 11) {
		FiltroInt filtroInt = criarFiltroInt(opcao);
		if (confirmaOperacao()) filtros.adicionarFiltroInt(filtroInt);
	} 
	
}

//-----------------------------------------------------------------------------------------------------------------------

//#######################################################################################################################
//#######################################################################################################################
//#######################################################################################################################

//#######################################################################################################################
//###############################################  VISUALIZAR FILTROS  ##################################################
//#######################################################################################################################

void visualizarFiltros(FiltrosSistema &filtros) {
	system("cls");
	if (filtros.possuiFiltros()) {
		if (filtros.possuiFIltrosData()) {
			for (int i = 0; i < filtros.getFiltrosData().size(); i++) {
				cout << filtros.getFiltrosData().at(i) << endl;
			}
		}
		if (filtros.possuiFIltrosHora()) {
			for (int i = 0; i < filtros.getFiltrosHora().size(); i++) {
				cout << filtros.getFiltrosHora().at(i) << endl;
			}
		}
		if (filtros.possuiFIltrosInt()) {
			for (int i = 0; i < filtros.getFiltrosInt().size(); i++) {
				cout << filtros.getFiltrosInt().at(i) << endl;
			}
		}
		if (filtros.possuiFIltrosString()) {
			for (int i = 0; i < filtros.getFiltrosString().size(); i++) {
				cout << filtros.getFiltrosString().at(i) << endl;
			}
		}
	} else cout << "Ainda não existem filtros adicionados.";
	cout << endl;
	char voltar = 'a';
	cout << "\n\nDigite 'S' para voltar: ";
	while (voltar != 'S' && voltar != 's') cin >> voltar;
}

//#######################################################################################################################
//#######################################################################################################################
//#######################################################################################################################


//#######################################################################################################################
//#################################################  MENU PRINCIPAL  ####################################################
//#######################################################################################################################

void exibeOpcoesMenu() {
	cout << "1- Adicionar filtro" <<endl;
	cout << "2 - Limpar filtros" << endl;
	cout << "3 - Visualizar filtros" << endl;
	cout << "4 - Visualizar dados" << endl;
	cout << "5 - Exportar dados" << endl;
	cout << "0 - Sair" << endl;
}

int menu() {
	int opcao = 0;
	do {
		system("cls");
		exibeOpcoesMenu();
		if (opcao < 0 || opcao > 5) cout << "\nOpcao invalida! Informe uma opcao valida: ";
		else cout << "\nOpcao selecionada: ";
		cin >> opcao;
	} while (opcao < 0 || opcao > 5 || opcao == 0 && !confirmaOperacao());

	return opcao;
}

//#######################################################################################################################
//#######################################################################################################################
//#######################################################################################################################

//#######################################################################################################################
//###################################################  EXECUÇÃO  ########################################################
//#######################################################################################################################

int main(void) {
	
	setlocale(LC_ALL, "Portuguese");
	string arquivo = "snortsyslog";

	Sistema *sistema = new Sistema(arquivo);

	FiltrosSistema filtros;
	
	int opcao;

	while (1) {
		opcao = menu();
		if (opcao == 0) break;
		switch (opcao) {
			case 1:
				adicionarFiltro(filtros);
			break;

			case 2:
				filtros.limparFiltros();
			break;
				
			case 3:
				visualizarFiltros(filtros);
			break;
		}
	}

	return 0;
}

//#######################################################################################################################
//#######################################################################################################################
//#######################################################################################################################