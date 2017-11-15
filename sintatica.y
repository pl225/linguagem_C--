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

using namespace std;

struct atributos
{
	string label;
	string traducao;
	string tipo;
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
} variavelTemporaria;

typedef struct 
{
	string id;
	string tipo;
	string temporario;
} variavelDeclarada;

typedef map<string, variavelTemporaria> mapT;
typedef map<string, variavelDeclarada> mapV;
//typedef map<string, funcaoDeclarada> mapF;

typedef struct 
{
	bool quebravel;
	mapV mapaVariaveis;
	string rotuloInicio;
	string rotuloFim;
} contextoBloco;

static stack<contextoBloco> pilhaContexto;

static mapT mapaTemporario;
static mapT mapaTemporarioCopia;
static mapV mapaDeclarado;
//static mapF mapaFuncao;

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

void verificaVariavelJaDeclarada (string s) {
	if (pilhaContexto.top().mapaVariaveis.find(s) != pilhaContexto.top().mapaVariaveis.end()) {
		yyerror("A variável "+ s + " já foi declarada.");					
	}
}

void defineTiposCompativeis (string s1, string s2) {
	bool v = false;
	if (s1 == BOOL && s2 != BOOL)
		v = true;
	if (s1 == CHAR && s2 != CHAR)
		v = true;
	if (s1 == INT && s2 != INT && s2 != FLUT32)
		v = true;
	if (s1 == FLUT32 && s2 != INT && s2 != FLUT32)
		v = true;
	if (s1 == CHARS && s2 != CHARS)
		v = true;
	if (v)
		yyerror("As variáveis " + s1 + " e " + s2 + " não são de tipos compatíveis.");
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
	string var = "tmp" + proximaVariavelTemporaria();
	pilhaContexto.top().mapaVariaveis[label] = { .id = label, .tipo = tipo, var};
	mapaTemporario[var] = {.id = var, .tipo = tipo, .tamanho = "", .isParametro = true};
	string nomeFuncao = pilhaContexto.top().rotuloInicio;
	mapaTemporario[nomeFuncao].funcao.parametros.push_front({ .id = var, .tipo = tipo }); // testar antes se ja nao existe
}

%}

%token TK_NUM TK_BOOL TK_CHAR TK_STRING
%token TK_MAIN TK_ID TK_TIPO_INT TK_TIPO_FLUT32 TK_TIPO_FLUT64  TK_TIPO_BOOL TK_TIPO_STRING
%token TK_TIPO_CHAR TK_MAIS_MENOS TK_MULTI_DIV TK_CONCATENACAO
%token TK_BIN TK_HEX TK_OCT
%token TK_RELACIONAL TK_LOGICO
%token TK_FIM TK_ERROR
%token TK_IF TK_WHILE TK_BREAK TK_CONTINUE TK_DO TK_FOR
%token TK_OP_ABREV TK_OP_1 TK_ELSE TK_SWITCH TK_CASE TK_DEFAULT
%token TK_PRINT TK_SCAN TK_TIPO_FUNCAO

%start S

%left TK_CONCATENACAO
%left TK_MAIS_MENOS
%left TK_MULTI_DIV
%left TK_LOGICO TK_RELACIONAL
%left TK_CAST

%%

S 			: PILHA_GLOBAL COMANDOS
			{
				cout << "/*Compilador FAEN*/" << endl;
				cout << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\n#include<stdlib.h>\n#define SIZE_STR 10\nusing namespace std;" << endl;
				cout << traduzirFuncoes() << endl;
				cout << "int main(void)\n{\n" << declaraVariaveisTemporarias() + $2.traducao + liberarStrings() << "\treturn 0;\n}" << endl; 
			}
			;

PILHA_GLOBAL: 
			{
				pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
					.rotuloInicio = proximoRotulo(), .rotuloFim = proximoRotulo()});
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
			;

DECLARACAO  : TK_TIPO_FLUT32 TK_ID DECLARACAO_VF32 DECLARACAO_F32
			{
				if ($3.tipo != "") defineTiposCompativeis($1.tipo, $3.tipo);
				verificaVariavelJaDeclarada($2.label);
				$$.label = "tmp" + proximaVariavelTemporaria();
	        	$$.tipo = $1.tipo;
	  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				if ($3.tipo != "") {
					if ($3.tipo == INT) {
						string varCast = "tmp" + proximaVariavelTemporaria();
						mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
						$$.traducao = $3.traducao + '\t' + varCast + " = (float) " + $3.label + ";\n" +
						'\t' + $$.label + " = " + varCast + ";\n" + $4.traducao;
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
				if ($3.tipo != "" && $3.label == "") {
					$$.traducao = $4.traducao + '\t' + $$.label + " = " + decideValorBooleano($3.traducao) + ";\n";
				} 
				else if ($3.label != "" && $3.tipo == "") {
					mapV mapa = buscaMapa($3.label);
					defineTiposCompativeis(BOOL, pilhaContexto.top().mapaVariaveis[$3.label].tipo);
					$$.traducao = $4.traducao + '\t' + $$.label + " = " + mapaTemporario[mapa[$3.label].temporario].id + ";\n";
				}
				else if ($3.label != "" && $3.tipo != ""){
					$$.traducao = $4.traducao + $3.traducao + 
					'\t' + $$.label + " = " + $3.label + ";\n";
				}
				else {
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
				if ($3.tipo != "") { // caracter puro com tipo
					$$.traducao = $4.traducao + '\t' + $$.label + " = " + $3.traducao + ";\n";
				} else if ($3.label != "") { // variavel
					mapV mapa = buscaMapa($3.label);
					defineTiposCompativeis(CHAR, mapa[$3.label].tipo);
					$$.traducao = $4.traducao + '\t' + $$.label + " = " + mapaTemporario[mapa[$3.label].temporario].id + ";\n";
				} else {
					$$.traducao = $4.traducao;
				}
      		}
      		| TK_TIPO_STRING TK_ID DECLARACAO_VSTRING DECLARACAO_STRING
      		{
      			if ($3.tipo != "") defineTiposCompativeis($1.tipo, $3.tipo);
      			verificaVariavelJaDeclarada($2.label);
				$$.label = "tmp" + proximaVariavelTemporaria();
				string tamanhoString = "tmp" + proximaVariavelTemporaria();
				$$.tipo = $1.tipo;
				pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo, .tamanho = tamanhoString };
				mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };

				if ($3.tipo != "") {
					$$.traducao = $3.traducao + 
						'\t' + tamanhoString + " = " + mapaTemporario[$3.label].tamanho + ";\n" +
						'\t' + $$.label + " = (char*) malloc(" + tamanhoString + ");\n" + 
						"\tstrcat(" + $$.label + ", " + $3.label + ");\n" + $4.traducao;						
				} else {
					$$.traducao = '\t' + tamanhoString + " = 0;\n" + $4.traducao;
				}
      		}
			| TK_TIPO_INT TK_ID DECLARACAO_VINT DECLARACAO_INT
      		{
      			if ($3.tipo != "") defineTiposCompativeis($1.tipo, $3.tipo);
      			verificaVariavelJaDeclarada($2.label);
        		$$.label = "tmp" + proximaVariavelTemporaria();
	        	$$.tipo = $1.tipo;
	  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
	  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				if ($3.tipo != "") {
					if ($3.tipo == FLUT32) {
						string varCast = "tmp" + proximaVariavelTemporaria();
						mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
						$$.traducao = $3.traducao + '\t' + varCast + " = (int) " + $3.label + ";\n" +
						'\t' + $$.label + " = " + varCast + ";\n" + $4.traducao;
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
				 |
				 { $$.traducao = ""; $$.tipo = ""; $$.label = ""; }
				 ;

DECLARACAO_F32 : ',' TK_ID DECLARACAO_VF32 DECLARACAO_F32
				{
					if ($3.tipo != "") defineTiposCompativeis(FLUT32, $3.tipo);
					verificaVariavelJaDeclarada($2.label);
					$$.label = "tmp" + proximaVariavelTemporaria();
		        	$$.tipo = FLUT32;
		  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
		  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
					if ($3.tipo != "") {
						if ($3.tipo == INT) {
							string varCast = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
							$$.traducao = $3.traducao + '\t' + varCast + " = (float) " + $3.label + ";\n" +
							'\t' + $$.label + " = " + varCast + ";\n" + $4.traducao;
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

DECLARACAO_VBOOL: '=' TK_BOOL
				 {
				 	$$ = $2;
				 }
				 |
				 '=' TK_ID 
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
					if ($3.tipo != "" && $3.label == "") {
						$$.traducao = $4.traducao + '\t' + $$.label + " = " + decideValorBooleano($3.traducao) + ";\n";
					} 
					else if ($3.label != "" && $3.tipo == ""){
						mapV mapa = buscaMapa($3.label);
						defineTiposCompativeis(BOOL,mapa[$3.label].tipo);
						$$.traducao = $4.traducao + '\t' + $$.label + " = " + mapaTemporario[mapa[$3.label].temporario].id + ";\n";
					}
					else if ($3.label != "" && $3.tipo != ""){
						$$.traducao = $4.traducao + $3.traducao + 
						'\t' + $$.label + " = " + $3.label + ";\n";
					}
					else {
						$$.traducao = $4.traducao;
					}
				}
				|
				{	$$.traducao = "";	}
				;

DECLARACAO_VCHAR: '=' TK_CHAR
				 {
				 	$$ = $2;
				 }
				 |
				 '=' TK_ID 
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
					if ($3.tipo != "") { // caracter puro com tipo
						$$.traducao = $4.traducao + '\t' + $$.label + " = " + $3.traducao + ";\n";
					} else if ($3.label != "") { // variavel
						mapV mapa = buscaMapa($3.label);
						defineTiposCompativeis(CHAR, mapa[$3.label].tipo);
						$$.traducao = $4.traducao + '\t' + $$.label + " = " + mapaTemporario[mapa[$3.label].temporario].id + ";\n";
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
					|
					{	$$.traducao = ""; $$.tipo = ""; $$.label = "";	}

DECLARACAO_STRING  : ',' TK_ID DECLARACAO_VSTRING DECLARACAO_STRING
					{
						if ($3.tipo != "") defineTiposCompativeis(CHARS, $3.tipo);
		      			verificaVariavelJaDeclarada($2.label);
						$$.label = "tmp" + proximaVariavelTemporaria();
						string tamanhoString = "tmp" + proximaVariavelTemporaria();
						$$.tipo = CHARS;
						pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
						mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo, .tamanho = tamanhoString };
						mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };

						if ($3.tipo != "") {
							$$.traducao = $3.traducao + '\t' + tamanhoString + " = " + mapaTemporario[$3.label].tamanho + ";\n" +
								'\t' + $$.label + " = (char*) malloc(" + tamanhoString + ");\n" + 
								"\tstrcat(" + $$.label + ", " + $3.label + ");\n" + $4.traducao;						
						} else {
							$$.traducao = '\t' + tamanhoString + " = 0;\n" + $4.traducao;
						}
					}
					|
					{	$$.traducao = "";	}
					;

DECLARACAO_VINT: '=' E
				 {
				 	$$ = $2;
				 }
				 |
				 { $$.traducao = ""; $$.tipo = ""; $$.label = ""; }
				 ;
				 
DECLARACAO_INT : ',' TK_ID DECLARACAO_VINT DECLARACAO_INT
				{
					if ($3.tipo != "") defineTiposCompativeis(INT, $3.tipo);
					verificaVariavelJaDeclarada($2.label);
					$$.label = "tmp" + proximaVariavelTemporaria();
		        	$$.tipo = INT;
		  			pilhaContexto.top().mapaVariaveis[$2.label] = { .id = $2.label, .tipo = $$.tipo, $$.label };
		  			mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
					if ($3.tipo != "") {
						if ($3.tipo == FLUT32) {
							string varCast = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
							$$.traducao = $3.traducao + '\t' + varCast + " = (int) " + $3.label + ";\n" +
							'\t' + $$.label + " = " + varCast + ";\n" + $4.traducao;
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

ATRIBUICAO	: TK_ID '=' TK_BOOL
			{
				mapV mapa = buscaMapa($1.label);
				$$.traducao = '\t' + mapaTemporario[mapa[$1.label].temporario].id + " = " + $3.traducao + ";\n";
			}
			|
			 TK_ID '=' TK_CHAR
			{
				mapV mapa = buscaMapa($1.label);
				$$.traducao = '\t' + mapaTemporario[mapa[$1.label].temporario].id + " = " + $3.traducao + ";\n";
			} 
			|
			TK_ID '=' E
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
			;

E 			: E TK_MAIS_MENOS E
			{
				defineTiposCompativeis($1.tipo, $3.tipo);
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
			}
			|
			E TK_MULTI_DIV E
			{
				defineTiposCompativeis($1.tipo, $3.tipo);
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
			|
			TK_CAST E
			{
				$$.label = "tmp" + proximaVariavelTemporaria();
				$$.tipo = $1.tipo;
				mapaTemporario[$$.label] = { .id = $$.label, .tipo = $$.tipo };
				$$.traducao = $2.traducao + '\t' + $$.label + " = ( " + decideTipo($1.tipo) + " ) " + $2.label + ";\n";
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

PRINT 		: TK_PRINT '(' TK_ID ')'
			{
				mapV mapa = buscaMapa($3.label);
				$$.traducao = "\tcout << " + mapa[$3.label].temporario + " << endl;\n";
			} 
			| TK_PRINT '(' TK_NUM ')'
			{
				$$.tipo = $3.tipo;
				$$.label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[$$.label] = {.id = $$.label, .tipo = $$.tipo};
				$$.traducao = '\t' + $$.label + " = " + $3.traducao + ";\n" +
					"\tcout << " + $$.label + " << endl;\n";
			}	
			| TK_PRINT '(' TK_CHAR ')'
			{
				$$.tipo = CHAR;
				$$.label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[$$.label] = {.id = $$.label, .tipo = $$.tipo};
				$$.traducao = '\t' + $$.label + " = " + $3.traducao + ";\n" +
					"\tcout << " + $$.label + " << endl;\n";
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

DECLARA_FUNCAO:	TK_TIPO_FUNCAO BLOCO_FUNCAO '(' ARGUMENTOS_FUNCAO ')' ':' TIPO_DADO BLOCO 
				{
					string nomeFuncao = pilhaContexto.top().rotuloInicio;
					mapaTemporario[nomeFuncao].funcao.retorno = $8.tipo;
					mapaTemporario[nomeFuncao].funcao.traducao = decideTipo($8.tipo) + nomeFuncao + 
						'(' + mapaTemporario[nomeFuncao].funcao.traducao + ')' +
						"{\n" + declaraVariaveisTemporariasFuncao() + $8.traducao + "}\n";
					mapaTemporarioCopia[nomeFuncao] = mapaTemporario[nomeFuncao];
					mapaTemporario = mapaTemporarioCopia;
					mapaTemporarioCopia.clear();
					pilhaContexto.pop();
					$$.traducao = "";
				} 
				;

BLOCO_FUNCAO 	: TK_ID
				{
					verificaVariavelJaDeclarada($1.label);
					string var = "tmp" + proximaVariavelTemporaria();
					pilhaContexto.top().mapaVariaveis[$1.label] = { .id = $1.label, .tipo = FUNCAO, var};
					mapaTemporario[var] = { .id = var, .tipo = FUNCAO, .tamanho = "", .isParametro = false,
						.funcao = {.retorno = "", .traducao = "", .parametros = list<parametrosFuncao>()}
					};
					mapaTemporarioCopia = mapaTemporario;
					pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
						.rotuloInicio = var, .rotuloFim = ""});
				}
				;
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
