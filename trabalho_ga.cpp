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

class Filtro {
	private:
		TipoFiltro tipo;

	public:
		Filtro() {}
		~Filtro() {}
		
		TipoFiltro getTipoFiltro() { return this->tipo; }
		void setTipoFiltro(TipoFiltro tipo) { this->tipo = tipo; }
};

class DataHora {
	private:
		int dia, mes, ano;
		int hora, minuto, segundo;
	
	public:
		DataHora() {}
		DataHora(int dia, int mes, int ano, int hora, int minuto, int segundo) {
			this->dia = dia;
			this->mes = mes;
			this->ano = ano;
			this->hora = hora;
			this->minuto = minuto;
			this->segundo = segundo;
		}
		~DataHora() {}

		int getDia() { return this->dia; }
		int getMes() { return this->mes; }
		int getAno() { return this->ano; }
		int getHora() { return this->hora; }
		int getMinuto() { return this->minuto; }
		int getSegundo() { return this->segundo; }
		void setDia(int dia) { this->dia = dia; }
		void setMes(int mes) { this->mes = mes; }
		void setAno(int ano) { this->ano = ano; }
		void setHora(int hora) { this->hora = hora; }
		void setMinuto(int minuto) { this->minuto = minuto; }
		void setSegundo(int segundo) { this->segundo = segundo; }

		friend std::ostream& operator<<(ostream& out, DataHora& dH) {
			out << dH.ano 
				<< "-" << std::setfill('0') << std::setw(2) << dH.mes 
				<< "-" << std::setfill('0') << std::setw(2)  << dH.dia 
				<< "T" << std::setfill('0') << std::setw(2) << dH.hora 
				<< ":" << std::setfill('0') << std::setw(2) << dH.minuto 
				<< ":" << std::setfill('0') << std::setw(2) << dH.segundo;
			return out;
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

		DataHora getDataHoraAtual() {
			time_t t = time(NULL);
			struct tm *tm = localtime(&t);

			DataHora dataHora(tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900, tm->tm_hour, tm->tm_min, tm->tm_sec);
			return dataHora;
		}

		int getAnoAtual() {
			time_t t = time(NULL);
			struct tm *tm = localtime(&t);

			int ano = tm->tm_year + 1900; 
			return ano;
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

			DataHora dataHora;
			dataHora.setAno(dataHora.getAnoAtual());
			dataHora.setMes(dataHora.mesStrParaMesInt(valoresDataHora[0]));
			dataHora.setDia(this->stringParaInt(valoresDataHora[1]));
			dataHora.setHora(this->stringParaInt(valoresHora[0]));
			dataHora.setMinuto(this->stringParaInt(valoresHora[1]));
			dataHora.setSegundo(this->stringParaInt(valoresHora[2]));
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
			cout << "lendo arquivo..." << endl;
			string linha;
            ifstream arquivo (pathArquivo.c_str());
            if (arquivo.is_open()) {
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

void exibeOpcoesMenu() {
	cout << "1- Adicionar filtro" <<endl;
	cout << "2 - Limpar filtros" << endl;
	cout << "3 - Visualizar filtros" << endl;
	cout << "4 - Visualizar dados" << endl;
	cout << "5 - Exportar dados" << endl;
}

int menu() {
	int opcao = 1;

	do {
		system("cls");
		exibeOpcoesMenu();
		if (opcao < 1 || opcao > 5) cout << "\nOpcao invalida! Informe uma opcao valida: ";
		else cout << "\nOpcao selecionada: ";
		cin >> opcao;
	} while (opcao < 1 || opcao > 5);

	return opcao;
}



int main(void) {
	
	setlocale(LC_ALL, "Portuguese");
	string arquivo = "snortsyslog";

	Sistema *sistema = new Sistema(arquivo);

	return 0;
}
