%{
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <list>
#include <stack>
#include <algorithm>

#define YYSTYPE atributos
#define INT "0"
#define FLUT32 "1"
#define BOOL "2"
#define CHAR "3"
#define CHARS "4"
#define SIZE_STR 10
#define FUNCAO "5"
#define INT_VETOR "6"
#define FLUT32_VETOR "7"
#define CHARS_VETOR "8"
#define INT_MATRIZ "9"
#define FLUT32_MATRIZ "10"
#define CHARS_MATRIZ "11"

using namespace std;

struct atributos
{
	string label;
	string traducao;
	string tipo;
	string coluna;
};

typedef struct 
{
	string id;
	string tipo;	
} parametrosFuncao;

typedef struct
{
	string id;
	string tipo;
	string tamanho;
	bool isParametro;
	struct 
	{
		string retorno;
		string traducao;
		list <parametrosFuncao> parametros;
	} funcao;
	string coluna;
} variavelTemporaria;

typedef struct 
{
	string id;
	string tipo;
	string temporario;
} variavelDeclarada;

typedef map<string, variavelTemporaria> mapT;
typedef map<string, variavelDeclarada> mapV;

typedef struct 
{
	bool quebravel;
	mapV mapaVariaveis;
	string rotuloInicio;
	string rotuloFim;
	bool funcional;
	bool isGlobal;
} contextoBloco;

static stack<contextoBloco> pilhaContexto;

static mapT mapaTemporario;
static mapT mapaTemporarioCopia;
static mapV mapaDeclarado;
static list<parametrosFuncao> parametrosAuxiliar;
typedef list<atributos> valoresV;
valoresV valoresVetorAuxiliar;

static stack<valoresV> pilhaVetores;

mapV controiMapaVariaveis () {
	mapV m;
	return m;
}

string proximaVariavelTemporaria () {
	static int n = 0;
	n++;
	return to_string(n);
}

string proximoRotulo () {
	static int m = 0;
	m++;
	return "rotulo_" + to_string(m);
}

string decideTipo (string tipo) {
	if (tipo == INT) return "int ";
	else if (tipo == FLUT32) return "float ";
	else if (tipo == BOOL) return "int ";
	else if (tipo == CHAR) return "char ";
	else if (tipo == CHARS) return "char *";
	else if (tipo == INT_VETOR) return "int *";
	else if (tipo == FLUT32_VETOR) return "float *";
	else if (tipo == CHARS_VETOR) return "char**";
	else if (tipo == INT_MATRIZ) return "int*";
	else if (tipo == FLUT32_MATRIZ) return "float*";
	else if (tipo == CHARS_MATRIZ) return "char**";
}

string decideOperadorRelacional (string op) {
	if (op == "e") return " && ";
	else if (op == "ou") return " || ";
	else if (op == "~=") return " != ";
	else if (op == "~") return " ! ";
	else return op;
}

string decideValorBooleano (string b) {
	if (b == "falso") return "0";
	else if (b == "verdadeiro") return "1";
}

string declaraVariaveisTemporarias () {
	string s = "";
	for (mapT::iterator it = mapaTemporario.begin(); it!=mapaTemporario.end(); ++it) { 
		if (it->second.tipo != FUNCAO) 
    		s += '\t' + decideTipo(it->second.tipo) + ' ' + it->second.id + ";\n";
	}
    return s;
}

string declaraVariaveisTemporariasFuncao () {
	mapT mapa;

	set_difference(mapaTemporario.begin(), mapaTemporario.end(),
                      mapaTemporarioCopia.begin(), mapaTemporarioCopia.end(),
                      insert_iterator<mapT>(mapa, mapa.end()),
              [](const mapT::value_type & a, const mapT::value_type & b)
              { return a.first < b.first; }
                     );

	string s = "";
	for (mapT::iterator it = mapa.begin(); it!=mapa.end(); ++it) {
		if (!it->second.isParametro)
    		s += '\t' + decideTipo(it->second.tipo) + ' ' + it->second.id + ";\n";
	}
	mapa.clear();
    return s;
}

string declaraVariaveisGlobais () {
	stack<contextoBloco> p = pilhaContexto;
	contextoBloco cb;
	string s = "\n";
	mapT mapa, mapAux;
	do {
		cb = p.top();
		p.pop();
	} while (not p.empty());
	for (mapV::iterator it = cb.mapaVariaveis.begin(); it!=cb.mapaVariaveis.end(); ++it) {
		if (it->second.tipo != FUNCAO) {
    		s += decideTipo(mapaTemporario[it->second.temporario].tipo) + ' ' + mapaTemporario[it->second.temporario].id + ";\n";
			mapa[it->second.temporario] = mapaTemporario[it->second.temporario];
		}
	}
	set_difference(mapaTemporario.begin(), mapaTemporario.end(),
                      mapa.begin(), mapa.end(),
                      insert_iterator<mapT>(mapAux, mapAux.end()),
              [](const mapT::value_type & a, const mapT::value_type & b)
              { return a.first < b.first; }
                     );
	mapaTemporario = mapAux;

	return s;
}

int yylex(void);
void yyerror(string);

void verificaVariavelNaoDeclarada (string s) {
	stack<contextoBloco> p = pilhaContexto;
	bool v = true;
	while (not p.empty()){
		if (p.top().mapaVariaveis.find(s) != p.top().mapaVariaveis.end()) {
			v = false;
			break;
		}
	    p.pop( );
	}
	if (v) {
		yyerror("A variável "+ s + " não foi declarada.");					
	}
}

mapV buscaMapa (string s) {
	stack<contextoBloco> p = pilhaContexto;
	while (not p.empty()){
		if (p.top().mapaVariaveis.find(s) != p.top().mapaVariaveis.end()) {
			return p.top().mapaVariaveis;
		}
	    p.pop();
	}
	yyerror("A variável "+ s + " não foi declarada.");						
}

contextoBloco controlarLaco () {
	stack<contextoBloco> p = pilhaContexto;
	while (not p.empty()){
		if (p.top().quebravel) {
			return p.top();
		}
	    p.pop();
	}
	yyerror("Este bloco não permite controle de laços.");							
}

contextoBloco controlarRetorno () {
	stack<contextoBloco> p = pilhaContexto;
	while (not p.empty()){
		if (p.top().funcional) {
			return p.top();
		}
	    p.pop();
	}
	yyerror("Este bloco não permite retorno de funções.");							
}

void verificaVariavelJaDeclarada (string s) {
	if (pilhaContexto.top().mapaVariaveis.find(s) != pilhaContexto.top().mapaVariaveis.end()) {
		yyerror("A variável "+ s + " já foi declarada.");					
	}
}

void defineTiposCompativeis (string s1, string s2) {
	bool v = false;
	if (s1 == BOOL && s2 != BOOL) v = true;
	if (s1 == CHAR && s2 != CHAR) v = true;
	if (s1 == INT && s2 != INT && s2 != FLUT32) v = true;
	if (s1 == FLUT32 && s2 != INT && s2 != FLUT32) v = true;
	if (s1 == CHARS && s2 != CHARS) v = true;
	if (s1 == INT_VETOR && s2 != INT_VETOR) v = true;
	if (s1 == FLUT32_VETOR && s2 != FLUT32_VETOR) v = true;
	if (s1 == CHARS_VETOR && s2 != CHARS_VETOR) v = true;
	if (v) yyerror("As variáveis " + s1 + " e " + s2 + " não são de tipos compatíveis.");
}

string liberarStrings () {
	string s = "";
	for (mapT::iterator it = mapaTemporario.begin(); it!=mapaTemporario.end(); ++it) {
		if (it->second.tipo == CHARS) 
    		s += "\tif (" + it->second.tamanho + " > 0) free(" + it->second.id + ");\n";
	}
    return s;
}

string traduzirFuncoes () {
	string s = "";
	for (mapT::iterator it = mapaTemporario.begin(); it!=mapaTemporario.end(); ++it) {
		if (it->second.tipo == FUNCAO)
    		s += it->second.funcao.traducao;
	}
    return s;
}

void processaArgumentoFuncao(string label, string tipo) {
	verificaVariavelJaDeclarada(label);
	string var = "tmp" + proximaVariavelTemporaria();
	pilhaContexto.top().mapaVariaveis[label] = { .id = label, .tipo = tipo, var};
	mapaTemporario[var] = {.id = var, .tipo = tipo, .tamanho = "", .isParametro = true};
	if (tipo == CHARS) {
		string tamanhoString = "tmp" + proximaVariavelTemporaria();
		mapaTemporario[var].tamanho = tamanhoString;
		mapaTemporario[tamanhoString] = {.id = tamanhoString, .tipo = INT};
	}
	string nomeFuncao = pilhaContexto.top().rotuloInicio;
	mapaTemporario[nomeFuncao].funcao.parametros.push_front({ .id = var, .tipo = tipo });
}

struct atributos processaOpAritmetica (struct atributos $1, struct atributos $2, struct atributos $3) {
	defineTiposCompativeis($1.tipo, $3.tipo);
	struct atributos $$;
	$$.label = "tmp" + proximaVariavelTemporaria();
	$$.traducao = $1.traducao + $3.traducao;
	if ($1.tipo != $3.tipo) {
		string varCast = "tmp" + proximaVariavelTemporaria();
		$$.tipo = FLUT32;
		mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
		mapaTemporario[$$.label] = { .id = $$.label, .tipo = FLUT32 };
		if ($1.tipo == INT && $3.tipo == FLUT32) {
			$$.traducao += '\t' + varCast + " = (float) " + $1.label + ";\n" +
			'\t' + $$.label + " = " + varCast + $2.traducao + $3.label + ";\n";
		} else if ($1.tipo == FLUT32 && $3.tipo == INT) {
			$$.traducao += '\t' + varCast + " = (float) " + $3.label + ";\n" +
			'\t' + $$.label + " = " + varCast + $2.traducao + $1.label + ";\n";
		}
	} else {
		$$.tipo = $1.tipo;
		mapaTemporario[$$.label] = { .id = $$.label, .tipo = $1.tipo };
		$$.traducao += '\t' + $$.label + " = " + $1.label + $2.traducao + $3.label + ";\n";
	}
	return $$;
}

string vetorParaElemento (string tipo) {
	if (tipo == INT_VETOR) return INT;
	else if (tipo == FLUT32_VETOR) return FLUT32;
	else if (tipo == CHARS_VETOR) return CHARS;
	else if (tipo == INT_MATRIZ) return INT;
	else if (tipo == FLUT32_MATRIZ) return FLUT32;
	else if (tipo == CHARS_MATRIZ) return CHARS;
}

struct atributos traducaoVetorNumerico (struct atributos $1, struct atributos $3) {
	struct atributos $$ = $1;
	list<atributos> valoresVetor = pilhaVetores.top();
	pilhaVetores.pop();
	string tamanhoVetor = "tmp" + proximaVariavelTemporaria();
	mapaTemporario[tamanhoVetor] = {.id = tamanhoVetor, .tipo = INT};
	mapaTemporario[$$.label].tamanho = tamanhoVetor;
	$$.traducao = $3.traducao + '\t' + tamanhoVetor + " = " + 
		(valoresVetor.size() == 0 ? $3.label : to_string(valoresVetor.size())) + ";\n" +
		'\t' + $$.label + " = (" +  decideTipo($3.tipo) + ") malloc(sizeof(" + decideTipo(vetorParaElemento($3.tipo))
			+ ")*" + tamanhoVetor + ");\n";
	int i = 0;
	for (list<atributos>::iterator it = valoresVetor.begin(); it != valoresVetor.end(); ++it) {
		defineTiposCompativeis(it->tipo, vetorParaElemento($3.tipo));
		if (it->tipo == FLUT32 && $3.tipo == INT_VETOR) {
			string varCast = "tmp" + proximaVariavelTemporaria();
			mapaTemporario[varCast] = {.id = varCast, .tipo = INT };
			$$.traducao += it->traducao + '\t' + varCast + " = (int) " + it->label + ";\n" +
				'\t' + $$.label + '[' + to_string(i) + "] = " + varCast + ";\n";
		} else if (it->tipo == INT && $3.tipo == FLUT32_VETOR) { 
			string varCast = "tmp" + proximaVariavelTemporaria();
			mapaTemporario[varCast] = {.id = varCast, .tipo = FLUT32 };
			$$.traducao += it->traducao + '\t' + varCast + " = (float) " + it->label + ";\n" +
				'\t' + $$.label + '[' + to_string(i) + "] = " + varCast + ";\n";
		} else if ($3.tipo == CHARS_VETOR) { 
			$$.traducao += it->traducao + '\t' + $$.label + '[' + to_string(i) + ']' + " = " + it->label + ";\n";
		} else {
			$$.traducao += it->traducao + '\t' + $$.label + '[' + to_string(i) 
				+ "] = " + it->label + ";\n";
		}
		i++;
	}
	valoresVetor.clear();
	return $$;
}

struct atributos traducaoMatrizNumerico (struct atributos $1, struct atributos $3) {
	struct atributos $$ = $1;
	if (pilhaVetores.empty()) {
		mapaTemporario[$$.label].tamanho = $3.label;
		mapaTemporario[$$.label].coluna = $3.coluna;
		string tamanhoMatriz = "tmp" + proximaVariavelTemporaria();
		mapaTemporario[tamanhoMatriz] = {.id = tamanhoMatriz, .tipo = INT};
		$$.traducao = $3.traducao + '\t' + tamanhoMatriz + " = " + $3.label + " * " + $3.coluna + ";\n" +
			'\t' + $$.label + " = (" + decideTipo($$.tipo) + ") malloc(sizeof(" + decideTipo(vetorParaElemento($$.tipo)) +
			")*" + tamanhoMatriz + ");\n";
		return $$;
	} else {
		/*string linha = "tmp" + proximaVariavelTemporaria();
		string coluna = "tmp" + proximaVariavelTemporaria();
		mapaTemporario[linha] = {.id = linha, .tiṕo = INT};
		mapaTemporario[coluna] = {.id = coluna, .tipo = INT};
		$$.traducao = '\t' + linha + " = " + pilhaVetores.size() + ';\n' +
			'\t' + coluna + " = " + pilhaVetores.top().size() + ';\n';
		int cols = pilhaVetores.top().size();
		if ($$.tipo == INT_MATRIZ) {
			int i = 0;
			while (!pilhaVetores.empty()) {
				list<atributos> colunas = pilhaVetores.top();
				pilhaVetores.pop();
				if (colunas != cols) yyerror("Apenas matrizes quadradas podem ser c")
			}
		}*/
	}
	/*list<atributos> valoresVetor = pilhaVetores.top();
	pilhaVetores.pop();
	string tamanhoVetor = "tmp" + proximaVariavelTemporaria();
	mapaTemporario[tamanhoVetor] = {.id = tamanhoVetor, .tipo = INT};
	mapaTemporario[$$.label].tamanho = tamanhoVetor;
	$$.traducao = $3.traducao + '\t' + tamanhoVetor + " = " + 
		(valoresVetor.size() == 0 ? $3.label : to_string(valoresVetor.size())) + ";\n" +
		'\t' + $$.label + " = (" +  decideTipo($3.tipo) + ") malloc(sizeof(" + decideTipo(vetorParaElemento($3.tipo))
			+ ")*" + tamanhoVetor + ");\n";
	int i = 0;
	for (list<atributos>::iterator it = valoresVetor.begin(); it != valoresVetor.end(); ++it) {
		defineTiposCompativeis(it->tipo, vetorParaElemento($3.tipo));
		if (it->tipo == FLUT32 && $3.tipo == INT_VETOR) {
			string varCast = "tmp" + proximaVariavelTemporaria();
			mapaTemporario[varCast] = {.id = varCast, .tipo = INT };
			$$.traducao += it->traducao + '\t' + varCast + " = (int) " + it->label + ";\n" +
				'\t' + $$.label + '[' + to_string(i) + "] = " + varCast + ";\n";
		} else if (it->tipo == INT && $3.tipo == FLUT32_VETOR) { 
			string varCast = "tmp" + proximaVariavelTemporaria();
			mapaTemporario[varCast] = {.id = varCast, .tipo = FLUT32 };
			$$.traducao += it->traducao + '\t' + varCast + " = (float) " + it->label + ";\n" +
				'\t' + $$.label + '[' + to_string(i) + "] = " + varCast + ";\n";
		} else if ($3.tipo == CHARS_VETOR) { 
			$$.traducao += it->traducao + '\t' + $$.label + '[' + to_string(i) + ']' + " = " + it->label + ";\n";
		} else {
			$$.traducao += it->traducao + '\t' + $$.label + '[' + to_string(i) 
				+ "] = " + it->label + ";\n";
		}
		i++;
	}
	valoresVetor.clear();*/
	return $$;
}

%}

%token TK_NUM TK_BOOL TK_CHAR TK_STRING
%token TK_MAIN TK_ID TK_TIPO_INT TK_TIPO_FLUT32 TK_TIPO_FLUT64  TK_TIPO_BOOL TK_TIPO_STRING
%token TK_TIPO_CHAR TK_MAIS_MENOS TK_MULTI_DIV TK_CONCATENACAO
%token TK_BIN TK_HEX TK_OCT TK_MODULO TK_CONCAT_IGUAL
%token TK_RELACIONAL TK_LOGICO TK_SEJA
%token TK_FIM TK_ERROR
%token TK_IF TK_WHILE TK_BREAK TK_CONTINUE TK_DO TK_FOR
%token TK_OP_ABREV TK_OP_1 TK_ELSE TK_SWITCH TK_CASE TK_DEFAULT
%token TK_PRINT TK_SCAN TK_TIPO_FUNCAO TK_RETORNA

%start S

%left TK_CONCATENACAO
%left TK_MAIS_MENOS
%left TK_MULTI_DIV
%left TK_MODULO
%left TK_LOGICO TK_RELACIONAL
%left TK_CAST

%%

S 			: PILHA_GLOBAL COMANDOS
			{
				cout << "/*Compilador FAEN*/" << endl;
				cout << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\n#include<stdlib.h>\n#define SIZE_STR 10\nusing namespace std;" << endl;
				cout << declaraVariaveisGlobais() << endl;
				cout << traduzirFuncoes() << endl;
				cout << "int main(void)\n{\n" << declaraVariaveisTemporarias() + $2.traducao + liberarStrings() << "\treturn 0;\n}" << endl; 
			}
			;

PILHA_GLOBAL: 
			{
				pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
					.rotuloInicio = proximoRotulo(), .rotuloFim = proximoRotulo(), .funcional = false, .isGlobal = true});
			}

BLOCO		: '{' COMANDOS '}'
			{
				$$.traducao = $2.traducao;
			}
			;

COMANDOS	: COMANDO COMANDOS
			{
				$$.traducao = $1.traducao + $2.traducao;
			}
			| 
			{
				$$.traducao = "";
			}
			;

COMANDO 	: DECLARACAO ';'
			| ATRIBUICAO ';'
			| SE
			| ENQUANTO
			| LOOP_CONTROLE ';'
			| REPITA ';'
			| PARA
			| ESCOLHA
			| PRINT ';'
			| SCAN ';'
			| DECLARA_FUNCAO
			| CALL_FUNCTION ';'
			| RETORNA ';'
			| DEC_DIN ';'
			;

DECLARACAO  : TK_TIPO_FLUT32 TK_ID DECLARACAO_VF32 DECLARACAO_F32
			{
				if ($3.tipo != "" && $3.tipo != FLUT32_VETOR && $3.tipo != FLUT32_MATRIZ) defineTiposCompativeis($1.tipo, $3.tipo);
				verificaVariavelJaDeclarada($2.label);
				$$.label = "tmp" + proximaVariavelTemporaria();
	        	$$.tipo = $3.tipo == FLUT32_VETOR || $3.tipo == FLUT32_MATRIZ ? $3.tipo : FLUT32;
	  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				if ($3.tipo != "") {
					if ($3.tipo == INT) {
						string varCast = "tmp" + proximaVariavelTemporaria();
						mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
						$$.traducao = $3.traducao + '\t' + varCast + " = (float) " + $3.label + ";\n" +
						'\t' + $$.label + " = " + varCast + ";\n" + $4.traducao;
					}  else if ($3.tipo == FLUT32_VETOR) {
						$$ = traducaoVetorNumerico($$, $3);
						$$.traducao += $4.traducao;
					} else if ($3.tipo == FLUT32_MATRIZ) {
						$$ = traducaoMatrizNumerico($$, $3);
						$$.traducao += $4.traducao;
					} else {
						$$.traducao = $3.traducao + '\t' + $$.label + " = " + $3.label + ";\n" + $4.traducao;
					}
				} else {
					$$.traducao = $4.traducao;
				}
			}
			| TK_TIPO_BOOL TK_ID DECLARACAO_VBOOL DECLARACAO_BOOL
			{
				verificaVariavelJaDeclarada($2.label);
				$$.label = "tmp" + proximaVariavelTemporaria();
	        	$$.tipo = BOOL;
	  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				if ($3.tipo != "") {
					$$.traducao = $3.traducao + '\t' + $$.label + " = " + $3.label + ";\n" + $4.traducao;
				} else {
					$$.traducao = $4.traducao;	
				}
			}
			| TK_TIPO_CHAR TK_ID DECLARACAO_VCHAR DECLARACAO_CHAR
      		{
      			verificaVariavelJaDeclarada($2.label);
				$$.label = "tmp" + proximaVariavelTemporaria();
	        	$$.tipo = CHAR;
	  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
	  			if ($3.tipo != "") {
					$$.traducao = $3.traducao + '\t' + $$.label + " = " + $3.label + ";\n" + $4.traducao;
				} else {
					$$.traducao = $4.traducao;	
				}
      		}
      		| TK_TIPO_STRING TK_ID DECLARACAO_VSTRING DECLARACAO_STRING
      		{
      			if ($3.tipo != "" && $3.tipo != CHARS_VETOR && $3.tipo != CHARS_MATRIZ) defineTiposCompativeis($1.tipo, $3.tipo);
      			verificaVariavelJaDeclarada($2.label);
        		$$.label = "tmp" + proximaVariavelTemporaria();
	        	$$.tipo = $3.tipo == CHARS_VETOR || $3.tipo == CHARS_MATRIZ ? $3.tipo : $1.tipo;
	        	pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo};

	        	if ($$.tipo == CHARS_VETOR) {
	        		$$ = traducaoVetorNumerico($$, $3);
					$$.traducao += $4.traducao;
	        	} else if ($$.tipo == CHARS_MATRIZ) {
					$$ = traducaoMatrizNumerico($$, $3);
					$$.traducao += $4.traducao;
	        	} else {
					string tamanhoString = "tmp" + proximaVariavelTemporaria();
					mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };
					mapaTemporario[$$.label].tamanho = tamanhoString;
					if ($3.tipo != "") {
						$$.traducao = $3.traducao + 
							'\t' + tamanhoString + " = " + mapaTemporario[$3.label].tamanho + ";\n" +
							'\t' + $$.label + " = (char*) malloc(" + tamanhoString + ");\n" + 
							"\tstrcat(" + $$.label + ", " + $3.label + ");\n" + $4.traducao;						
					} else {
						$$.traducao = '\t' + tamanhoString + " = 0;\n" + $4.traducao;
					}
				}
      		}
			| TK_TIPO_INT TK_ID DECLARACAO_VINT DECLARACAO_INT
      		{
      			if ($3.tipo != "" && $3.tipo != INT_VETOR && $3.tipo != INT_MATRIZ) defineTiposCompativeis($1.tipo, $3.tipo);
      			verificaVariavelJaDeclarada($2.label);
        		$$.label = "tmp" + proximaVariavelTemporaria();
	        	$$.tipo = $3.tipo == INT_VETOR || $3.tipo == INT_MATRIZ ? $3.tipo : $1.tipo;
	  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				if ($3.tipo != "") {
					if ($3.tipo == FLUT32) {
						string varCast = "tmp" + proximaVariavelTemporaria();
						mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
						$$.traducao = $3.traducao + '\t' + varCast + " = (int) " + $3.label + ";\n" +
						'\t' + $$.label + " = " + varCast + ";\n" + $4.traducao;
					} else if ($3.tipo == INT_VETOR) {
						$$ = traducaoVetorNumerico($$, $3);
						$$.traducao += $4.traducao;
					} else if ($3.tipo == INT_MATRIZ) {
						$$ = traducaoMatrizNumerico($$, $3);
						$$.traducao += $4.traducao;
					} else {
						$$.traducao = $3.traducao + '\t' + $$.label + " = " + $3.label + ";\n" + $4.traducao;
					}
				} else {
					$$.traducao = $4.traducao;	
				}
      		}
			;

DECLARACAO_VF32: '=' E
				 {
				 	$$ = $2;
				 }
				 | DECLARACAO_VETOR
				 {
				 	$$ = $1;
				 	$$.tipo = FLUT32_VETOR;
				 }
				 | DECLARACAO_MATRIZ
				 {
				 	$$ = $1;
				 	$$.tipo = FLUT32_MATRIZ;
				 }
				 |
				 { $$.traducao = ""; $$.tipo = ""; $$.label = ""; }
				 ;

DECLARACAO_F32 : ',' TK_ID DECLARACAO_VF32 DECLARACAO_F32
				{
					if ($3.tipo != "" && $3.tipo != FLUT32_VETOR && $3.tipo != FLUT32_MATRIZ) defineTiposCompativeis($1.tipo, $3.tipo);
					verificaVariavelJaDeclarada($2.label);
					$$.label = "tmp" + proximaVariavelTemporaria();
		        	$$.tipo = $3.tipo == FLUT32_VETOR || $3.tipo == FLUT32_MATRIZ ? $3.tipo : FLUT32;
		  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
		  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
					if ($3.tipo != "") {
						if ($3.tipo == INT) {
							string varCast = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
							$$.traducao = $3.traducao + '\t' + varCast + " = (float) " + $3.label + ";\n" +
							'\t' + $$.label + " = " + varCast + ";\n" + $4.traducao;
						} else if ($3.tipo == FLUT32_VETOR) {
							$$ = traducaoVetorNumerico($$, $3);
							$$.traducao += $4.traducao;
						} else if ($3.tipo == FLUT32_MATRIZ) {
							$$ = traducaoMatrizNumerico($$, $3);
							$$.traducao += $4.traducao;
						} else {
							$$.traducao = $3.traducao + '\t' + $$.label + " = " + $3.label + ";\n" + $4.traducao;
						}
					} else {
						$$.traducao = $4.traducao;
					}
				}
				|
				{	$$.traducao = "";	}
				;

DECLARACAO_VBOOL: '=' E
				 {
				 	$$ = $2;
				 }
				 |
				 '=' L
				 {
				 	$$ = $2;
				 }
				 |
				 { $$.traducao = ""; $$.tipo = ""; $$.label = ""; }
				 ;
				 
DECLARACAO_BOOL : ',' TK_ID DECLARACAO_VBOOL DECLARACAO_BOOL
				{
					verificaVariavelJaDeclarada($2.label);
					$$.label = "tmp" + proximaVariavelTemporaria();
		        	$$.tipo = BOOL;
		  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
		  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
					if ($3.tipo != "") {
						$$.traducao = $3.traducao + '\t' + $$.label + " = " + $3.label + ";\n" + $4.traducao;
					} else {
						$$.traducao = $4.traducao;	
					}
				}
				|
				{	$$.traducao = "";	}
				;

DECLARACAO_VCHAR: '=' E
				 {
				 	$$ = $2;
				 }
				 |
				 { $$.traducao = ""; $$.tipo = ""; $$.label = ""; }
				 ;
				 
DECLARACAO_CHAR : ',' TK_ID DECLARACAO_VCHAR DECLARACAO_CHAR
				{
					verificaVariavelJaDeclarada($2.label);
					$$.label = "tmp" + proximaVariavelTemporaria();
		        	$$.tipo = CHAR;
		  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
		  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
					if ($3.tipo != "") {
						$$.traducao = $3.traducao + '\t' + $$.label + " = " + $3.label + ";\n" + $4.traducao;
					} else {
						$$.traducao = $4.traducao;	
					}
				}
				|
				{	$$.traducao = "";	}
				;

DECLARACAO_VSTRING : '=' E
					{
						$$ = $2;
					}
					| DECLARACAO_VETOR
					{
					 	$$ = $1;
					 	$$.tipo = CHARS_VETOR;
					}
					| DECLARACAO_MATRIZ
					{
					 	$$ = $1;
					 	$$.tipo = CHARS_MATRIZ;
					}
					|
					{	$$.traducao = ""; $$.tipo = ""; $$.label = "";	}

DECLARACAO_STRING  : ',' TK_ID DECLARACAO_VSTRING DECLARACAO_STRING
					{
						if ($3.tipo != "" && $3.tipo != CHARS_VETOR && $3.tipo != CHARS_MATRIZ) defineTiposCompativeis(CHARS, $3.tipo);
		      			verificaVariavelJaDeclarada($2.label);
		        		$$.label = "tmp" + proximaVariavelTemporaria();
			        	$$.tipo = $3.tipo == CHARS_VETOR || $3.tipo == CHARS_MATRIZ ? $3.tipo : CHARS;
			        	pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
						mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };

			        	if ($$.tipo == CHARS_VETOR) {
			        		$$ = traducaoVetorNumerico($$, $3);
							$$.traducao += $4.traducao;
			        	} else if ($$.tipo == CHARS_MATRIZ) {
			        		$$ = traducaoMatrizNumerico($$, $3);
							$$.traducao += $4.traducao;
			        	} else {
							string tamanhoString = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };
							mapaTemporario[$$.label].tamanho = tamanhoString;
							if ($3.tipo != "") {
								$$.traducao = $3.traducao + '\t' + tamanhoString + " = " + mapaTemporario[$3.label].tamanho + ";\n" +
									'\t' + $$.label + " = (char*) malloc(" + tamanhoString + ");\n" + 
									"\tstrcat(" + $$.label + ", " + $3.label + ");\n" + $4.traducao;						
							} else {
								$$.traducao = '\t' + tamanhoString + " = 0;\n" + $4.traducao;
							}
						}
					}
					|
					{	$$.traducao = "";	}
					;

DECLARACAO_VINT: '=' E
				 {
				 	$$ = $2;
				 }
				 | DECLARACAO_VETOR
				 {
				 	$$ = $1;
				 	$$.tipo = INT_VETOR;
				 }
				 | DECLARACAO_MATRIZ
				 {
				 	$$ = $1;
				 	$$.tipo = INT_MATRIZ;
				 }
				 |
				 { $$.traducao = ""; $$.tipo = ""; $$.label = ""; }
				 ;
				 
DECLARACAO_INT : ',' TK_ID DECLARACAO_VINT DECLARACAO_INT
				{
					if ($3.tipo != "" && $3.tipo != INT_VETOR && $3.tipo != INT_MATRIZ) defineTiposCompativeis($1.tipo, $3.tipo);
					verificaVariavelJaDeclarada($2.label);
					$$.label = "tmp" + proximaVariavelTemporaria();
		        	$$.tipo = $3.tipo == INT_VETOR || $3.tipo == INT_MATRIZ ? $3.tipo : INT;
		  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
		  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
					if ($3.tipo != "") {
						if ($3.tipo == FLUT32) {
							string varCast = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
							$$.traducao = $3.traducao + '\t' + varCast + " = (int) " + $3.label + ";\n" +
							'\t' + $$.label + " = " + varCast + ";\n" + $4.traducao;
						} else if ($3.tipo == INT_VETOR) {
							$$ = traducaoVetorNumerico($$, $3);
							$$.traducao += $4.traducao;
						} else if ($3.tipo == INT_MATRIZ) {
							$$ = traducaoMatrizNumerico($$, $3);
							$$.traducao += $4.traducao;
						} else {
							$$.traducao = $3.traducao + '\t' + $$.label + " = " + $3.label + ";\n" + $4.traducao;
						}
					} else {
						$$.traducao = $4.traducao;
					}
				}
				|
				{	$$.traducao = "";	}
				;

DECLARACAO_VETOR: '[' E ']'
				{
					pilhaVetores.push(list<atributos>());
					defineTiposCompativeis($2.tipo, INT);
					$$ = $2;
				}
				| '=' '[' E DCL_VECTOR_AUX
				{
					valoresVetorAuxiliar.push_front($3);
					pilhaVetores.push(valoresVetorAuxiliar);
					valoresVetorAuxiliar.clear();
				}
				;

DCL_VECTOR_AUX	: ',' E DCL_VECTOR_AUX
				{
					valoresVetorAuxiliar.push_front($2);
				}
				| ']'
				{
					$$.traducao = ""; $$.label = ""; $$.tipo = "";
				}
				;

DECLARACAO_MATRIZ:	'[' E ']' '[' E ']'
				{
					defineTiposCompativeis($2.tipo, INT);
					defineTiposCompativeis($5.tipo, INT);
					$$ = $2;
					$$.traducao += $5.traducao;
					$$.coluna = $5.label;
				}
				| '[' '[' E DCL_MATRIZ_AUX
				{
					valoresVetorAuxiliar.push_front($3);
					pilhaVetores.push(valoresVetorAuxiliar);
					valoresVetorAuxiliar.clear();
				}
				;

DCL_MATRIZ_AUX	: ',' E DCL_MATRIZ_AUX
				{
					valoresVetorAuxiliar.push_front($2);
				} 
				| ']' ',' '[' DCL_MATRIZ_AUX
				{
					pilhaVetores.push(valoresVetorAuxiliar);
					valoresVetorAuxiliar.clear();	
				}
				| ']' ']'
				{
					$$.traducao = ""; $$.label = ""; $$.tipo = "";	
				}
				;
 
ATRIBUICAO	:TK_ID '=' E
			{
				mapV mapa = buscaMapa($1.label);
				$1.tipo = mapaTemporario[mapa[$1.label].temporario].tipo;
				defineTiposCompativeis($1.tipo, $3.tipo);
				if ($1.tipo != $3.tipo) {
					string varCast = "tmp" + proximaVariavelTemporaria();

					if ($1.tipo == INT) {
						$$.tipo = INT;
						mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
						$$.traducao = $3.traducao + '\t' + varCast + " = (int) " + $3.label + ";\n" +
						'\t' + mapaTemporario[mapa[$1.label].temporario].id + " = " + varCast + ";\n";
					} else if ($1.tipo == FLUT32){
						$$.tipo = FLUT32;
						mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
						$$.traducao = $3.traducao + '\t' + varCast + " = (float) " + $3.label + ";\n" +
						'\t' + mapaTemporario[mapa[$1.label].temporario].id + " = " + varCast + ";\n";
					}
				} else if ($1.tipo == CHARS) {
					string temporarioId = mapa[$1.label].temporario;
					string tamanhoString = mapaTemporario[temporarioId].tamanho;
					$$.traducao = $3.traducao + "\tif(" + tamanhoString + " > 0) free(" + temporarioId + ");\n" +
						'\t' + tamanhoString + " = " + mapaTemporario[$3.label].tamanho + ";\n" +
				 		'\t' + temporarioId + " = (char*) malloc(" + tamanhoString + ");\n" + 
						"\tstrcat(" + temporarioId + ", " + $3.label + ");\n";
				} else if ($1.tipo == INT_VETOR || $1.tipo == FLUT32_VETOR || $1.tipo == CHARS_VETOR) {
					string tamanhoVetor = "tmp" + proximaVariavelTemporaria();
					mapaTemporario[tamanhoVetor] = {.id = tamanhoVetor, .tipo = INT};
					mapaTemporario[mapa[$1.label].temporario].tamanho = tamanhoVetor;
					$$.traducao = $3.traducao + '\t' + tamanhoVetor + " = " + mapaTemporario[$3.label].tamanho + ";\n" +
						'\t' + mapaTemporario[mapa[$1.label].temporario].id + " = " + $3.label + ";\n";
				} else {
					$$.traducao = $3.traducao + '\t' + mapaTemporario[mapa[$1.label].temporario].id + 
					" = " + $3.label + ";\n";
				}
			}
			|
			TK_ID '=' L
			{
				mapV mapa = buscaMapa($1.label);
				defineTiposCompativeis($1.tipo, $3.tipo);
				$$.traducao = $3.traducao + '\t' + mapaTemporario[mapa[$1.label].temporario].id + 
				" = " + $3.label + ";\n";	
			}
			|
			TK_ID TK_OP_1
			{
				mapV mapa = buscaMapa($1.label);
				defineTiposCompativeis(mapa[$1.label].tipo, INT);
				string mais1 = mapaTemporario[mapa[$1.label].temporario].tipo == INT ? " 1;\n" : " 1.0;\n";
				string op = $2.traducao == "++" ? " +" : " -";
				$$.traducao = '\t' + mapaTemporario[mapa[$1.label].temporario].id +
				" = " + mapaTemporario[mapa[$1.label].temporario].id + op + mais1;
			}
			| TK_ID TK_OP_ABREV E
			{
				mapV mapa = buscaMapa($1.label);
        		string var = mapa[$1.label].temporario;
        		$1.label = mapaTemporario[var].id;
        		$1.tipo = mapaTemporario[var].tipo;
        		$1.traducao = "";
     		 	struct atributos op;
    		    if ($2.traducao == "+=") { 
      		  		op.traducao = " + ";
      		  	} else if ($2.traducao == "-=") {
        			op.traducao = " - "; 
		        } else if ($2.traducao == "*=") {
		        	op.traducao = " * ";
       			} else if ($2.traducao == "%=") {
       				op.traducao = " % ";
       			} else {
        			op.traducao = " / ";
				}
				$$ = processaOpAritmetica($1, op, $3);
				$$.traducao += '\t' + $1.label + " = " + $$.label + ";\n";
			}
			| TK_ID TK_CONCAT_IGUAL E
			{
				defineTiposCompativeis(CHARS, $3.tipo);
				mapV mapa = buscaMapa($1.label);
				defineTiposCompativeis(CHARS, mapa[$1.label].tipo);
        		$$.traducao = $3.traducao + '\t' + mapaTemporario[mapa[$1.label].temporario].tamanho +
        						" = " + mapaTemporario[mapa[$1.label].temporario].tamanho + " + " 
        						+ mapaTemporario[$3.label].tamanho + ";\n" + '\t' + mapa[$1.label].temporario + 
        						" = (char*) realloc(" + mapa[$1.label].temporario + 
        						", " + mapaTemporario[$3.label].tamanho + ");\n" +
        						"\tstrcat(" + mapa[$1.label].temporario + ", " + $3.label + ");\n";
			}
			| TK_ID '[' E ']' '=' E
			{
				defineTiposCompativeis($3.tipo, INT);
				mapV mapa = buscaMapa($1.label);
				defineTiposCompativeis(vetorParaElemento(mapa[$1.label].tipo), $6.tipo);
        		string var = mapa[$1.label].temporario;
        		string tamanhoVetor = mapaTemporario[var].tamanho;
        		if ($6.tipo == CHARS) {
	        		$$.traducao = $3.traducao + $6.traducao + "\tif(" + $3.label + " < 0 || " + $3.label + " >= " + tamanhoVetor + ") exit(1);\n" +
						'\t' + var + '[' + $3.label + "] = (char*) realloc(" + var + '[' + $3.label + "], " + mapaTemporario[$6.label].tamanho + ");\n" + 
						"\tstrcpy(" + var + '[' + $3.label + ']' + ", " + $6.label + ");\n";
        		} else {
        			if (vetorParaElemento(mapa[$1.label].tipo) != $6.tipo) {
        				string varCast = "tmp" + proximaVariavelTemporaria();
        				mapaTemporario[varCast] = { .id = varCast, .tipo = $6.tipo == INT ? FLUT32 : INT };
        				$$.traducao = $3.traducao + $6.traducao + "\tif (" + $3.label + " < 0 || " + $3.label + " >= " + tamanhoVetor + ") exit(1);\n" +
        					'\t' + varCast + " = (" + decideTipo(mapaTemporario[varCast].tipo) + ") " + $6.label + ";\n" +
	        				'\t' + var + '[' + $3.label + "] = " + varCast + ";\n";
        			} else {
        				$$.traducao = $3.traducao + $6.traducao + "\tif (" + $3.label + " < 0 || " + $3.label + " >= " + tamanhoVetor + ") exit(1);\n" +
	        			'\t' + var + '[' + $3.label + "] = " + $6.label + ";\n";
        			}
        		}
			}
			| TK_ID '[' E ']' '[' E ']' '=' E
			{
				defineTiposCompativeis($3.tipo, INT);
				defineTiposCompativeis($6.tipo, INT);
				mapV mapa = buscaMapa($1.label);
				defineTiposCompativeis(vetorParaElemento(mapa[$1.label].tipo), $9.tipo);
				string var = mapa[$1.label].temporario;
        		string linhas = mapaTemporario[var].tamanho;
        		string colunas = mapaTemporario[var].coluna;
        		string posicao = "tmp" + proximaVariavelTemporaria();
        		mapaTemporario[posicao] = {.id = posicao, .tipo = INT};
        		$$.traducao = $3.traducao + $6.traducao + "\tif(" + linhas + " < 0 || " + $3.label + " >= " + linhas + ") exit(1);\n" +
						"\tif(" + colunas + " < 0 || " + $6.label + " >= " + colunas + ") exit(1);\n" +
						'\t' + posicao + " = " + $3.label + " * " + colunas + ";\n" +
						'\t' + posicao + " = " + posicao + " + " + $6.label + ";\n" + $9.traducao;
        		if (mapa[$1.label].tipo == FLUT32_MATRIZ || mapa[$1.label].tipo == INT_MATRIZ) {
					if (vetorParaElemento(mapa[$1.label].tipo) != $9.tipo) {
						string varCast = "tmp" + proximaVariavelTemporaria();
        				mapaTemporario[varCast] = { .id = varCast, .tipo = $6.tipo == INT ? FLUT32 : INT };
        				$$.traducao += '\t' + varCast + " = (" + decideTipo(mapaTemporario[varCast].tipo) + ") " + $9.label + ";\n" +
        					'\t' + var + '[' + posicao + "] = " + varCast + ";\n";
					} else {
						$$.traducao += '\t' + var + '[' + posicao + "] = " + $9.label + ";\n";
					}
				} else {
					string tamanhoString = "tmp" + proximaVariavelTemporaria();
					mapaTemporario[tamanhoString] = {.id = tamanhoString, .tipo = INT};
					string rotulo = proximoRotulo();
					$$.traducao += '\t' + var + '[' + posicao + "] = " + $9.label + ";\n";
				}
			}
			;

E 			: E TK_MAIS_MENOS E
			{
				$$ = processaOpAritmetica($1, $2, $3);
			}
			|
			E TK_MULTI_DIV E
			{
				$$ = processaOpAritmetica($1, $2, $3);
			}
			|
			E TK_MODULO E
			{
				defineTiposCompativeis($1.tipo, $3.tipo);
				$$ = processaOpAritmetica($1, $2, $3);
			}
			|
			E TK_CONCATENACAO E
			{
				defineTiposCompativeis($1.tipo, $3.tipo);
				$$.tipo = $1.tipo;
				$$.label = "tmp" + proximaVariavelTemporaria();
				string tamanhoString = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo, .tamanho = tamanhoString };
				mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };
				$$.traducao = $1.traducao + $3.traducao + 
					'\t' + tamanhoString + " = " + mapaTemporario[$1.label].tamanho + " + " +
				 	mapaTemporario[$3.label].tamanho + ";\n" + '\t' + tamanhoString + " = " + tamanhoString + " + 1;\n" +
					'\t' + $$.label + " = (char*) malloc(" + tamanhoString + ");\n" + 
					"\tstrcat(" + $$.label + ", " + $1.label + ");\n" + "\tstrcat(" + $$.label + ", " + $3.label + ");\n";
			}
			| TK_STRING
			{
				$$.tipo = $1.tipo;
				$$.label = "tmp" + proximaVariavelTemporaria();
				string tamanhoString = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo, .tamanho = tamanhoString };
				mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };
				$$.traducao = '\t' + tamanhoString + " = " + to_string($1.traducao.length() - 2) + " + 1;\n" +
					'\t' + $$.label + " = (char*) malloc(" + tamanhoString + ");\n" + 
					"\tstrcat(" + $$.label + ", " + $1.traducao + ");\n";
			}
			| 
			TK_NUM
			{
				$$.tipo = $1.tipo;
				$$.label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = '\t' + $$.label + " = " + $1.traducao + ";\n";
			}
			|
			TK_MAIS_MENOS E 
			{
				$$.tipo = $2.tipo;
				$$.label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $2.traducao + '\t' + $$.label + " = -" + $2.label + ";\n";
			}
			|
			'(' E ')'
			{
				$$.tipo = $2.tipo;
				$$.label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $2.traducao + '\t' + $$.label + " = " + $2.label + ";\n";
			}
			| TK_ID 
			{
				mapV mapa = buscaMapa($1.label);
        		string var = mapa[$1.label].temporario;
        		$$.label = mapaTemporario[var].id;
        		$$.tipo = mapaTemporario[var].tipo;
        		$$.traducao = "";
			}
			| TK_CHAR 
			{
				$$.tipo = $1.tipo;
				$$.label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = '\t' + $$.label + " = " + $1.traducao + ";\n";	
			}
			| TK_BOOL
			{
				$$.tipo = $1.tipo;
				$$.label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = '\t' + $$.label + " = " + decideValorBooleano($1.traducao) + ";\n";	
			}
			|
			TK_CAST E
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = $1.tipo;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $2.traducao + '\t' + $$.label + " = ( " + decideTipo($1.tipo) + " ) " + $2.label + ";\n";
			}
			| CALL_FUNCTION 
			{
				$$ = $1;
			}
			| TK_ID '[' E ']'
			{
				defineTiposCompativeis($3.tipo, INT);
				mapV mapa = buscaMapa($1.label);
        		string var = mapa[$1.label].temporario;
        		string tamanhoVetor = mapaTemporario[var].tamanho;
        		string varElemento = "tmp" + proximaVariavelTemporaria();
        		mapaTemporario[varElemento] = {.id = varElemento, .tipo = vetorParaElemento(mapaTemporario[var].tipo)};
        		$$.label = varElemento;
        		$$.tipo = mapaTemporario[varElemento].tipo;
        		if ($$.tipo == CHARS) {
	        		string tamanhoString = "tmp" + proximaVariavelTemporaria();
	        		mapaTemporario[tamanhoString] = {.id = tamanhoString, .tipo = INT};
	        		mapaTemporario[varElemento].tamanho = tamanhoString;
	        		$$.traducao = $3.traducao + '\t' + tamanhoString + " = strlen(" + var + '[' + $3.label + ']' + ") + 1;\n" +
						'\t' + $$.label + " = (char*) malloc(" + tamanhoString + ");\n" + 
						"\tstrcat(" + $$.label + ", " + var + '[' + $3.label + ']' +");\n";
        		} else {
        			$$.traducao = $3.traducao + "\tif (" + $3.label + " < 0 || " + $3.label + " >= " + tamanhoVetor + ") exit(1);\n" +
	        			'\t' + varElemento + " = " + var + '[' + $3.label + "];\n";
        		}
			} 
			| TK_ID '[' E ']' '[' E ']'
			{
				defineTiposCompativeis($3.tipo, INT);
				defineTiposCompativeis($6.tipo, INT);
				mapV mapa = buscaMapa($1.label);
				string var = mapa[$1.label].temporario;
        		string linhas = mapaTemporario[var].tamanho;
        		string colunas = mapaTemporario[var].coluna;
        		string posicao = "tmp" + proximaVariavelTemporaria();
        		mapaTemporario[posicao] = {.id = posicao, .tipo = INT};
        		string varElemento = "tmp" + proximaVariavelTemporaria();
        		mapaTemporario[varElemento] = {.id = varElemento, .tipo = vetorParaElemento(mapaTemporario[var].tipo)};
        		$$.label = varElemento;
        		$$.tipo = mapaTemporario[varElemento].tipo;
        		$$.traducao = $3.traducao + $6.traducao + "\tif(" + linhas + " < 0 || " + $3.label + " >= " + linhas + ") exit(1);\n" +
						"\tif(" + colunas + " < 0 || " + $6.label + " >= " + colunas + ") exit(1);\n" +
						'\t' + posicao + " = " + $3.label + " * " + colunas + ";\n" +
						'\t' + posicao + " = " + posicao + " + " + $6.label + ";\n";
				if ($$.tipo == CHARS) {
					string tamanhoString = "tmp" + proximaVariavelTemporaria();
	        		mapaTemporario[tamanhoString] = {.id = tamanhoString, .tipo = INT};
	        		mapaTemporario[varElemento].tamanho = tamanhoString;
	        		$$.traducao += '\t' + tamanhoString + " = strlen(" + var + '[' + posicao + ']' + ") + 1;\n" +
						'\t' + $$.label + " = (char*) malloc(" + tamanhoString + ");\n" + 
						"\tstrcat(" + $$.label + ", " + var + '[' + posicao + ']' +");\n";
				} else {
					$$.traducao += '\t' + varElemento + " = " + var + '[' + posicao + "];\n";
				}
			}
			;


R			: E TK_RELACIONAL E
			{
				defineTiposCompativeis($1.tipo, $3.tipo);
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = BOOL;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				if ($1.tipo != CHARS) {
					$$.traducao = $1.traducao + $3.traducao + '\t' +
						$$.label + " = " + $1.label + decideOperadorRelacional($2.traducao) + $3.label + ";\n";
				} else if ($1.tipo == CHARS) {
					$$.traducao = $1.traducao + $3.traducao + '\t' +
						$$.label + " = strcmp(" + $1.label + ", " + $3.label + ");\n" +
						'\t' + $$.label + " = " + $$.label + decideOperadorRelacional($2.traducao) + " 0;\n";
				}
			}
			| R TK_RELACIONAL TK_BOOL 
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = BOOL;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $1.traducao + '\t' + $$.label + " = " + $1.label + 
				"==" + decideValorBooleano($3.traducao) + ";\n";
			}
			;

L 			: L TK_LOGICO L
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = BOOL;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $1.traducao + $3.traducao + '\t' + $$.label + " = " + $1.label + 
				decideOperadorRelacional($2.traducao) + $3.label + ";\n";				
			}
			|
			'(' L ')'
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = BOOL;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $2.traducao + '\t' + $$.label + " = " + $2.label + ";\n";
			}
			|
			R
			;

SE  		: TK_IF '(' L ')' BLOCO_IF BLOCO
			{         
 				string varIf = "tmp" + proximaVariavelTemporaria();
 				string rotuloFim = proximoRotulo();
				mapaTemporario[varIf] = { .id = varIf, .tipo = BOOL };
				$$.traducao = $3.traducao +	"\t" + varIf + " = !" + $3.label + ";\n" +
				"\tif (" + varIf + ") goto " + rotuloFim + ";\n" +
				$6.traducao + '\t' + rotuloFim + ":\n";
				pilhaContexto.pop();
			} 
			| TK_IF '(' L ')' BLOCO_IF BLOCO TK_ELSE SE
			{
				string varIf = "tmp" + proximaVariavelTemporaria();
    		    string rotuloFimIf = proximoRotulo();
    		    string rotuloFimIfElse = proximoRotulo();
				mapaTemporario[varIf] = { .id = varIf, .tipo = BOOL };
				$$.traducao = $3.traducao +	"\t" + varIf + " = !" + $3.label + ";\n" +
				"\tif (" + varIf + ") goto " + rotuloFimIf + ";\n" + 
				$6.traducao + "\tgoto " + rotuloFimIfElse + ";\n" +
				'\t' + rotuloFimIf + ":\n" + $8.traducao + '\t' + rotuloFimIfElse + ":\n";
				pilhaContexto.pop();
			}
    	    | TK_IF '(' L ')' BLOCO_IF BLOCO TK_ELSE BLOCO_ELSE BLOCO
   			{
    		    string varIf = "tmp" + proximaVariavelTemporaria();
    		    string rotuloFimIf = proximoRotulo();
    		    string rotuloFimIfElse = proximoRotulo();
				mapaTemporario[varIf] = { .id = varIf, .tipo = BOOL };
				$$.traducao = $3.traducao +	"\t" + varIf + " = !" + $3.label + ";\n" +
				"\tif (" + varIf + ") goto " + rotuloFimIf + ";\n" + 
				$6.traducao + "\tgoto " + rotuloFimIfElse + ";\n" +
				'\t' + rotuloFimIf + ":\n" + $9.traducao + '\t' + rotuloFimIfElse + ":\n";
				pilhaContexto.pop();
			}

BLOCO_IF	: 
			{
       			pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
      			.rotuloInicio = "", .rotuloFim = ""});
      		}

BLOCO_ELSE	:
			{
				pilhaContexto.pop();
				pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
      				.rotuloInicio = "", .rotuloFim = ""});	
			}


ENQUANTO	: TK_WHILE '(' L ')' BLOCO_ITERACAO BLOCO
			{
				string varWhile = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[varWhile] = { .id = varWhile, .tipo = BOOL };
				$$.traducao = '\t' + pilhaContexto.top().rotuloInicio + ":\n" +	$3.traducao +
				"\t" + varWhile + " = !" + $3.label + ";\n" +
				"\tif (" + varWhile + ") goto " + pilhaContexto.top().rotuloFim + ";\n" +
				$6.traducao + "\tgoto " + pilhaContexto.top().rotuloInicio + ";\n" +
				'\t' + pilhaContexto.top().rotuloFim + ":\n";
				pilhaContexto.pop();
			}

REPITA		: TK_DO BLOCO_ITERACAO BLOCO TK_WHILE '(' L ')'
			{
				string varDoWhile = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[varDoWhile] = { .id = varDoWhile, .tipo = BOOL };
				$$.traducao = '\t' + pilhaContexto.top().rotuloInicio + ":\n" + $3.traducao +
				$6.traducao + '\t' + varDoWhile + " = !" + $6.label + ";\n" + 
				"\tif (" + varDoWhile + ") goto " + pilhaContexto.top().rotuloFim + ";\n" +
				"\tgoto " + pilhaContexto.top().rotuloInicio + ";\n" +
				'\t' + pilhaContexto.top().rotuloFim + ":\n";
				pilhaContexto.pop();
			}

PARA		: TK_FOR BLOCO_ITERACAO '(' PARA_ATRIBUICAO ';' L ';' PARA_INCREMENTO ')' BLOCO
			{
				string varFor = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[varFor] = { .id = varFor, .tipo = BOOL };
				$$.traducao = $4.traducao + '\t' + pilhaContexto.top().rotuloInicio + ":\n" +
				$6.traducao + '\t' + varFor + " = !" + $6.label + ";\n" +
				"\tif (" + varFor + ") goto " + pilhaContexto.top().rotuloFim + ";\n" +
				$10.traducao + $8.traducao + "\tgoto " + pilhaContexto.top().rotuloInicio + ";\n"+
				'\t' + pilhaContexto.top().rotuloFim + ":\n";
				pilhaContexto.pop();
			}


PARA_ATRIBUICAO: ATRIBUICAO
				{
					$$.traducao = $1.traducao;
				}

PARA_INCREMENTO: ATRIBUICAO
				{
					$$.traducao = $1.traducao;
				}

ESCOLHA		: TK_SWITCH '(' BLOCO_CASE ')' '{' CASOS '}'
			{
  				$$.traducao = $3.traducao + $6.traducao + '\t' + pilhaContexto.top().rotuloFim + ":\n";
  				pilhaContexto.pop();
			}
			;

BLOCO_CASE	: TK_ID
			{
				mapV mapa = buscaMapa($1.label); 
				string var = "tmp" + proximaVariavelTemporaria();
  				mapaTemporario[var] = {.id = var, .tipo = mapa[$1.label].tipo};
  				$$.traducao = '\t' + var + " = " + mapa[$1.label].temporario + ";\n";
				pilhaContexto.push({ .quebravel = true, .mapaVariaveis = controiMapaVariaveis(),
            	    .rotuloInicio = var, .rotuloFim = proximoRotulo()}); // rotulo inicio esta sendo igual a variavel temporaria que se refere a variavel de entrada do switch
			}
			;

CASOS 		: CASO CASOS
			{
				$$.traducao = $1.traducao + $2.traducao;
			}	
			| 
        	DEFAULT
			{
				$$.traducao = $1.traducao;
			}
			| CASO
      		{
        		$$.traducao = $1.traducao;
      		}
			;


CASO 		: TK_CASE VALOR_CASE ':' COMANDOS
			{  			
				defineTiposCompativeis($2.tipo, mapaTemporario[pilhaContexto.top().rotuloInicio].tipo);
  				string rotulo = proximoRotulo();
  				$$.traducao = $2.traducao + "\tif (" + $2.label + " != " + pilhaContexto.top().rotuloInicio + ") goto " + rotulo + ";\n"
            	+ $4.traducao + '\t' + rotulo + ":\n";  
			} 
			;

VALOR_CASE	: TK_NUM
			{
                $$.tipo = $1.tipo;
				$$.label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = '\t' + $$.label + " = " + $1.traducao + ";\n";
			}
			|
            TK_CHAR
            {
              	$$.tipo = $1.tipo;
                $$.label = "tmp" + proximaVariavelTemporaria();
                mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
                $$.traducao = '\t' + $$.label + " = " + $1.traducao + ";\n";
            }
			|
            TK_ID
            {
                mapV mapa = buscaMapa($1.label);
        		string var = mapa[$1.label].temporario;
        		$$.label = mapaTemporario[var].id;
        		$$.tipo = mapaTemporario[var].tipo;
        		$$.traducao = "";
            }
			;

DEFAULT 	: TK_DEFAULT ':' COMANDOS
			{
				$$.traducao = $3.traducao;
			}
			;

PRINT 		: TK_PRINT '(' E ')'
			{
				$$.traducao = $3.traducao + "\tcout << " + $3.label + " << endl;\n";
			} 
			;

SCAN 		: TK_SCAN '(' TK_ID ')'
			{
				mapV mapa = buscaMapa($3.label);
				if (mapa[$3.label].tipo == CHARS) {
					string rotuloInicio = proximoRotulo();
					string rotuloFim = proximoRotulo();
					string tamanho = mapaTemporario[mapa[$3.label].temporario].tamanho;
					string id = mapaTemporario[mapa[$3.label].temporario].id;
					string tmpChar = "tmp" + proximaVariavelTemporaria();
					mapaTemporario[tmpChar] = { .id = tmpChar, .tipo = CHAR };

					$$.traducao = "\tif (" + tamanho + " > 0)" + " free(" + id + ");\n" +
						'\t' + id + " = (char*) malloc(SIZE_STR);\n" +
						'\t' + tamanho + " = 0;\n" + '\t' + rotuloInicio + ":\n" +
						'\t' + tmpChar + " = getchar();\n" + 
						"\tif(" + tmpChar + " == \'\\0\' || " + tmpChar + " == \'\\n\') goto " + rotuloFim + ";\n" +
						'\t' + id + '[' + tamanho + "] = " + tmpChar + ";\n" +
						'\t' + tamanho + " = " + tamanho + " + 1;\n" +
						"\tif(" + tamanho + " < SIZE_STR) goto " + rotuloInicio + ";\n" +
						'\t' + tamanho + " = " + tamanho + " + SIZE_STR;\n" +
						'\t' + id + " = (char*) realloc(" + id + ", " + tamanho + ");\n" + 
						"\tgoto " + rotuloInicio + ";\n" + '\t' + rotuloFim + ":\n" +
						'\t' + tamanho + " = " + tamanho + " + 1;\n" +
						'\t' + id + '[' + tamanho + "] = \'\\0\';\n";

				} else if(mapa[$3.label].tipo == CHAR) {
					$$.traducao = "\tscanf(\"%c\", &" + mapa[$3.label].temporario + ");\n";
				} else if(mapa[$3.label].tipo == FLUT32) {
					$$.traducao = "\tscanf(\"%f\", &" + mapa[$3.label].temporario + ");\n";
				} else if(mapa[$3.label].tipo == INT) {
					$$.traducao = "\tscanf(\"%d\", &" + mapa[$3.label].temporario + ");\n";
				}

				$$.traducao += "\tsetbuf(stdin, NULL);\n";
			}
			;

BLOCO_ITERACAO:
			{
				pilhaContexto.push({ .quebravel = true, .mapaVariaveis = controiMapaVariaveis(),
					.rotuloInicio = proximoRotulo(), .rotuloFim = proximoRotulo()});
			}

LOOP_CONTROLE: TK_BREAK 
			{
				contextoBloco cb = controlarLaco();
				$$.traducao = "\tgoto " + cb.rotuloFim + ";\n";
			}
			|
			TK_CONTINUE
			{
				contextoBloco cb = controlarLaco();
				$$.traducao = "\tgoto " + cb.rotuloInicio + ";\n";
			}

TIPO_DADO:	TK_TIPO_STRING | TK_TIPO_INT | TK_TIPO_CHAR | TK_TIPO_BOOL | TK_TIPO_FLUT32;

ARGUMENTOS_FUNCAO: TK_ID ':' TIPO_DADO ARGUMENTOS_FUNCAO_AUX
				{
					processaArgumentoFuncao($1.label, $3.tipo);
					mapaTemporario[pilhaContexto.top().rotuloInicio].funcao.traducao = decideTipo($3.tipo) + 
						pilhaContexto.top().mapaVariaveis[$1.label].temporario + $4.traducao;
				}
				|{ $$.traducao = ""; }
				;

ARGUMENTOS_FUNCAO_AUX:	',' TK_ID ':' TIPO_DADO ARGUMENTOS_FUNCAO_AUX
				{
					processaArgumentoFuncao($2.label, $4.tipo);
					$$.traducao = ", " + decideTipo($4.tipo) + 
						pilhaContexto.top().mapaVariaveis[$2.label].temporario + $5.traducao;
				}
				|{ $$.traducao = ""; }
				;

DECLARA_FUNCAO:	TK_TIPO_FUNCAO BLOCO_FUNCAO '(' ARGUMENTOS_FUNCAO ')' ':' TIPO_DADO_FUNC BLOCO 
				{
					string nomeFuncao = pilhaContexto.top().rotuloInicio;
					string tamanhoStringParametros = "";
					list<parametrosFuncao> a = mapaTemporario[nomeFuncao].funcao.parametros;
					for (list<parametrosFuncao>::iterator it = a.begin(); it != a.end(); ++it) {
						if (it->tipo == CHARS) {
							tamanhoStringParametros += '\t' + mapaTemporario[it->id].tamanho + " = strlen(" + it->id + ");\n"; 
						}
					}
					mapaTemporario[nomeFuncao].funcao.traducao = decideTipo($7.tipo) + nomeFuncao + 
						'(' + mapaTemporario[nomeFuncao].funcao.traducao + ')' +
						"{\n" + declaraVariaveisTemporariasFuncao() + tamanhoStringParametros + $8.traducao + "}\n";
					mapaTemporarioCopia[nomeFuncao] = mapaTemporario[nomeFuncao];
					mapaTemporario = mapaTemporarioCopia;
					mapaTemporarioCopia.clear();
					pilhaContexto.pop();
					$$.traducao = "";
				} 
				;

TIPO_DADO_FUNC	: TIPO_DADO
				{
					mapaTemporario[pilhaContexto.top().rotuloInicio].funcao.retorno = $1.tipo;
				}
				;

BLOCO_FUNCAO 	: TK_ID
				{
					if (!pilhaContexto.top().isGlobal) yyerror("Uma função só pode ser declarada no escopo global.");
					verificaVariavelJaDeclarada($1.label);
					string var = "tmp" + proximaVariavelTemporaria();
					pilhaContexto.top().mapaVariaveis[$1.label] = { .id = $1.label, .tipo = FUNCAO, var};
					mapaTemporario[var] = { .id = var, .tipo = FUNCAO, .tamanho = "", .isParametro = false,
						.funcao = {.retorno = "", .traducao = "", .parametros = list<parametrosFuncao>()}
					};
					mapaTemporarioCopia = mapaTemporario;
					pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
						.rotuloInicio = var, .rotuloFim = "", .funcional = true});
				}
				;

ARG_FUNC_CALL	: E ARG_FUNC_CALL_AUX
				{
					$$.traducao = $1.traducao + $2.traducao;
					parametrosAuxiliar.push_front({.id = $1.label, .tipo = $1.tipo});
				}
				| { $$.traducao = ""; }
				;

ARG_FUNC_CALL_AUX : ',' E ARG_FUNC_CALL_AUX
				{
					$$.traducao = $2.traducao + $3.traducao;
					parametrosAuxiliar.push_front({.id = $2.label, .tipo = $2.tipo});
				}
				| { $$.traducao = ""; }
				;

CALL_FUNCTION	: TK_ID '(' ARG_FUNC_CALL ')'
				{
					mapV mapa = buscaMapa($1.label);
					if (mapa[$1.label].tipo == FUNCAO) {
						string traducao = "", traducaoCast;
						string temporario = mapa[$1.label].temporario;
						list<parametrosFuncao>::iterator real = mapaTemporario[temporario].funcao.parametros.begin(),
							aux = parametrosAuxiliar.begin();

						while (real != mapaTemporario[temporario].funcao.parametros.end() && aux != parametrosAuxiliar.end()) {
							defineTiposCompativeis(real->tipo, aux->tipo);
							if (real->tipo != aux->tipo) {
								string var = "tmp" + proximaVariavelTemporaria();
								mapaTemporario[var] = {.id = var, .tipo = real->tipo};
								traducaoCast += '\t' + var + " = (" + decideTipo(real->tipo) + ") " + aux->id + ";\n";
								traducao += var + ',';
							} else if (aux->tipo == CHARS) {
								string varString = "tmp" + proximaVariavelTemporaria();
								string tamanhoString = "tmp" + proximaVariavelTemporaria();
								mapaTemporario[varString] = {.id = varString, .tipo = CHARS, .tamanho = tamanhoString};
								mapaTemporario[tamanhoString] = {.id = tamanhoString, .tipo = INT};
								traducaoCast += '\t' + tamanhoString + " = " + mapaTemporario[aux->id].tamanho + ";\n" +
												'\t' + varString + " = (char*) malloc(" + tamanhoString + ");\n" +
												"\tstrcat(" + varString + ", " + aux->id + ");\n";
								traducao += varString + ',';
							} else {
								traducao += aux->id + ',';
							}
							real++; aux++;
						}
						if (real != mapaTemporario[temporario].funcao.parametros.end() || aux != parametrosAuxiliar.end()) 
							yyerror("Número errado de parâmetros enviado à função.");
						if (traducao.length() > 0) traducao.pop_back();
						$$.label = "tmp" + proximaVariavelTemporaria();
						$$.tipo = mapaTemporario[temporario].funcao.retorno;
						mapaTemporario[$$.label] = {.id = $$.label, .tipo = $$.tipo};
						if ($$.tipo == CHARS) {
							string tamanhoString = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[tamanhoString] = {.id = tamanhoString, .tipo = INT};
							mapaTemporario[$$.label].tamanho = tamanhoString;
							$$.traducao = $3.traducao + traducaoCast + '\t' + $$.label + " = " + temporario + '(' + traducao + ");\n" +
											'\t' + tamanhoString + " = strlen(" + $$.label + ");\n";
						} else {
							$$.traducao = $3.traducao + traducaoCast + '\t' + $$.label + " = " + temporario + '(' + traducao + ");\n";
						}
						parametrosAuxiliar.clear();
					} else {
						yyerror("A variável " + $1.label + " não é uma função.");
					} 	
				}
				;

RETORNA 		: TK_RETORNA E
				{
					contextoBloco cb = controlarRetorno();		
					string retorno = mapaTemporario[cb.rotuloInicio].funcao.retorno;
					defineTiposCompativeis(retorno, $2.tipo);
					if (retorno != $2.tipo) {
						string var = "tmp" + proximaVariavelTemporaria();
						mapaTemporario[var] = {.id = var, .tipo = retorno};
						$$.traducao = $2.traducao + '\t' + var + " = (" + decideTipo(retorno) + ") " + $2.label + ";\n"
							"\treturn " + var + ";\n";
					} else {
						$$.traducao = $2.traducao + "\treturn " + $2.label + ";\n";
					}
				}
				;

DEC_DIN			: TK_SEJA TK_ID '=' E DEC_DIN_AUX
				{
					verificaVariavelJaDeclarada($2.label);
					$$.label = "tmp" + proximaVariavelTemporaria();
	        		$$.tipo = $4.tipo;
	  				pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
	  				if ($4.tipo == CHARS) {
	  					string tamanhoString = "tmp" + proximaVariavelTemporaria();
	  					mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };
	  					mapaTemporario[$$.label].tamanho = tamanhoString;
	  					$$.traducao = $4.traducao + '\t' + tamanhoString + " = " + mapaTemporario[$4.label].tamanho + ";\n" +
	  								'\t' + $$.label + " = (char*) malloc(" + tamanhoString + ");\n" +
	  								"\tstrcat(" + $$.label + ", " + $4.label + ");\n"; 
	  				} else {
	  					$$.traducao = $4.traducao + '\t' + $$.label + " = " + $4.label + ";\n" + $5.traducao;
	  				}
				}
				;

DEC_DIN_AUX		: ',' TK_ID '=' E DEC_DIN_AUX
				{
					verificaVariavelJaDeclarada($2.label);
					$$.label = "tmp" + proximaVariavelTemporaria();
	        		$$.tipo = $4.tipo;
	  				pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
					if ($4.tipo == CHARS) {
	  					string tamanhoString = "tmp" + proximaVariavelTemporaria();
	  					mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };
	  					mapaTemporario[$$.label].tamanho = tamanhoString;
	  					$$.traducao = $4.traducao + '\t' + tamanhoString + " = " + mapaTemporario[$4.label].tamanho + ";\n" +
	  								'\t' + $$.label + " = (char*) malloc(" + tamanhoString + ");\n" +
	  								"\tstrcat(" + $$.label + ", " + $4.label + ");\n"; 
	  				} else {
	  					$$.traducao = $4.traducao + '\t' + $$.label + " = " + $4.label + ";\n" + $5.traducao;
	  				}
				}
				| { $$.traducao = ""; $$.tipo = ""; $$.label = ""; }

%%

#include "lex.yy.c"

int yyparse();

int main( int argc, char* argv[] )
{
	yyparse();

	return 0;
}

void yyerror( string MSG )
{
	cout << MSG << endl;
	exit (0);
}				
