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

bool stringContemTexto(string referencia, string texto) {
	return referencia.find(texto) != std::string::npos;
}

enum TipoFiltro {
	DATA,
	HORA,
	INT,
	STRING
};

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

		Data getDataInicial() { this->dataInicial; }
		void setDataInicial(Data dataInicial) { this->dataInicial = dataInicial; }
		Data getDataFinal() { this->dataFinal; }
		void setDataFinal(Data dataFinal) { this->dataFinal = dataFinal; }
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

		Hora getHoraInicial() { this->horaInicial; }
		void setHoraInicial(Hora horaInicial) { this->horaInicial = horaInicial; }
		Hora getHoraFinal() { this->horaFinal; }
		void setHoraFinal(Hora horaFinal) { this->horaFinal = horaFinal; }
};

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

		int getAnoAtual() {
			time_t t = time(NULL);
			struct tm *tm = localtime(&t);

			int ano = tm->tm_year + 1900; 
			return ano;
		}

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

		friend std::ostream& operator<<(ostream& out, Data& d) {
            out << d.ano << "-" << std::setfill('0') << std::setw(2) << d.mes << "-" << std::setfill('0') << std::setw(2) << d.dia;
			return out;
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

		friend std::ostream& operator<<(ostream& out, Hora& h) {
            out << std::setfill('0') << std::setw(2) << h.hora << ":" << std::setfill('0') << std::setw(2) << h.minuto << ":" << std::setfill('0') << std::setw(2) << h.segundo;
			return out;
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

		DataHora getDataHoraAtual() {
			time_t t = time(NULL);
			struct tm *tm = localtime(&t);

			DataHora dataHora(Data(tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900), Hora(tm->tm_hour, tm->tm_min, tm->tm_sec));
			return dataHora;
		}
};

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
		Registro(string linha) {
			DataHora dataHora = this->leDataHora(linha.substr(0, 15));

			string codigo = this->separarValores(linha, ' ')[5];
			codigo = codigo.substr(1, codigo.find(']') - 1);

			string mensagem = linha.substr(linha.find(']') + 2);

			string labelClassificacao = " [Classification: ";
			string labelPrioridade = " [Priority: ";

			bool possuiClassificacao = stringContemTexto(mensagem, labelClassificacao);
			bool possuiPrioridade = stringContemTexto(mensagem, labelPrioridade);

			string classificacao;
			string prioridade;

			int posicaoFinalMensagem = 0, posicaoFinalClassificacao = 0, posicaoFinalPrioridade = 0;
			int posicaoInicialMensagem = 0, posicaoInicialClassificacao = 0; int posicaoInicialPrioridade;
			
				
			if (possuiClassificacao) {
				posicaoFinalMensagem = mensagem.find(labelClassificacao);
				posicaoInicialClassificacao = posicaoFinalMensagem + labelClassificacao.size();
				classificacao = mensagem.substr(posicaoInicialClassificacao);
				posicaoInicialClassificacao = 0;
				mensagem = mensagem.substr(posicaoInicialMensagem, posicaoFinalMensagem);
				if (possuiPrioridade) {
					posicaoFinalClassificacao = classificacao.find(labelPrioridade) - 1;
					posicaoInicialPrioridade = classificacao.find(labelPrioridade) + labelPrioridade.size();
					posicaoFinalPrioridade = 1;
					prioridade = classificacao.substr(posicaoInicialPrioridade, posicaoFinalPrioridade);
					classificacao = classificacao.substr(posicaoInicialClassificacao, posicaoFinalClassificacao);
				}
				else {
					posicaoFinalClassificacao = classificacao.find('{') - 2;
					classificacao = classificacao.substr(posicaoInicialClassificacao,posicaoFinalClassificacao);
				}
			}
			else if (possuiPrioridade) {
				posicaoInicialPrioridade = mensagem.find(labelPrioridade) + labelPrioridade.size();
				posicaoFinalPrioridade = 1;
				prioridade = mensagem.substr(posicaoInicialPrioridade, posicaoFinalPrioridade);
				posicaoFinalMensagem = mensagem.find(labelPrioridade);
				mensagem = mensagem.substr(posicaoInicialMensagem, posicaoFinalMensagem);
			}
			else {
				posicaoFinalMensagem = mensagem.find('{') - 2;
				mensagem = mensagem.substr(posicaoInicialMensagem, posicaoFinalMensagem);
			} 
	
			string protocolo = linha.substr(linha.find('{') + 1);
			protocolo = protocolo.substr(0, protocolo.find('}'));


			string ipPortaOrigem = linha.substr(linha.find('}') + 2);
			ipPortaOrigem = ipPortaOrigem.substr(0, ipPortaOrigem.find(" -> "));

			string origemIP;
			string origemPorta;
			bool possuiPortaOrigem = stringContemTexto(ipPortaOrigem, ":");

			if (possuiPortaOrigem) {
				origemIP = this->separarValores(ipPortaOrigem, ':')[0];
				origemPorta = this->separarValores(ipPortaOrigem, ':')[1];
			} else origemIP = ipPortaOrigem;

			string ipPortaDestino = linha.substr(linha.find(" -> ") + 4);
			
			string destinoIP;
			string destinoPorta;
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
			this->destinoIP = destinoIP;
			if (!destinoPorta.empty()) this->destinoPorta = this->stringParaInt(origemPorta);
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

		DataHora leDataHora(string dataHoraStr) {
			vector<string> valoresDataHora = separarValores(dataHoraStr, ' ');
			// Caso o dia tenha apenas um dígito, ajusta as posições do vector
			if (valoresDataHora[1].empty()) {
				valoresDataHora[1] = valoresDataHora[2];
				valoresDataHora[2] = valoresDataHora[3];
			}
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

		vector<string> separarValores(string linha, char separador) {
            vector<string> valores;
            string valor;
            istringstream registro_stream(linha);
            while (getline(registro_stream, valor, separador)) {
                valores.push_back(valor);
            }
            return valores;
        }

		int stringParaInt(string original) {
            int resultado;
            stringstream string_stream(original);
            string_stream >> resultado;
            return resultado;
        }
};

class Sistema {
	private:
		vector<Registro*> logs;
		vector<Filtro*> filtros;
	public:
		Sistema(string pathArquivo) {
			this->populaLogs(pathArquivo);
		}
		~Sistema() {}

		void populaLogs(string pathArquivo) {
			system("cls");
			string linha;
            ifstream arquivo (pathArquivo.c_str());
            if (arquivo.is_open()) {
				cout << "lendo arquivo..." << endl;
                while (arquivo.good()) {
                    getline (arquivo, linha);
					Registro registro(linha);
                    this->logs.push_back(&registro);
                }
            }
            arquivo.close();
			system("cls");
		}		
};

bool confirmaOperacao() {
	char opcao = 'a';
	while (opcao != 'S' && opcao != 's' && opcao != 'N' && opcao != 'n') {
		cout << "\nTem certeza? (S/N): ";
		cin >> opcao;
	}
	return opcao == 'S' || opcao == 's';
}

void exibeOpcoesMenuAdicionarFiltroso() {
	system("cls");
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
		exibeOpcoesMenuAdicionarFiltroso();
		if (opcao < 0 || opcao > 11) cout << "\nOpcao invalida! Informe uma opcao valida: ";
		else cout << "\nOpcao selecionada: ";
		cin >> opcao;
	} while (opcao < 0 || opcao > 11 || opcao == 0 && !confirmaOperacao());

	return opcao;
}

Filtro* criarFiltroString(int opcao) {
	string teste = "djndlknd";
	AtributoRegistroFiltro atributo;
	Filtro *filtroString = new FiltroString(teste, atributo);
	return filtroString;
}

Filtro* criarFiltroInt(int opcao) {
	int valorInicial = 1;
	int valorFinal = 1;
	AtributoRegistroFiltro atributo;
	Filtro *filtroString = new FiltroInt(valorInicial, valorFinal, atributo);
	return filtroString;
}

void adicionarFiltro(vector<Filtro*> &filtros) {
	int opcao = menuAdicionarFiltros();
	if (opcao == 0) return;
	Filtro *novoFiltro;
	if (opcao == 3 || opcao == 4 || opcao == 5 || opcao == 7 || opcao == 8 || opcao == 10) novoFiltro = criarFiltroString(opcao);
	if (opcao == 6, opcao == 9, opcao == 11) novoFiltro = criarFiltroInt(opcao);
	if (confirmaOperacao()) filtros.push_back(novoFiltro);
}

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

int main(void) {
	
	setlocale(LC_ALL, "Portuguese");
	string arquivo = "snortsyslog";

	// Sistema *sistema = new Sistema(arquivo);
	vector<Filtro*> filtros;
	filtros.push_back(new FiltroInt(5, 10, PRIORIDADE));
	
	int opcao = menu();
	switch (opcao) {
		case 1:
			adicionarFiltro(filtros);
		break;
	}

	// delete sistema;
	return 0;
}
