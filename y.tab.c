/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "sintatica.y" /* yacc.c:339  */

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

static int linha = 1;

void contaLinhas(){
	linha++;
}

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

contextoBloco controlarLacoALL () {
	stack<contextoBloco> p = pilhaContexto;
	contextoBloco base;
	while (not p.empty()){
		if (p.top().quebravel) {
			base = p.top();
		}
	    p.pop();
	}
	return base;
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
	else if (tipo == CHARS) return CHAR;
	else
		yyerror("A variável não é vetor, matriz nem string");
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
	} else {

		string linha = "tmp" + proximaVariavelTemporaria();
		string coluna = "tmp" + proximaVariavelTemporaria();
		mapaTemporario[linha] = {.id = linha, .tipo = INT };
		mapaTemporario[coluna] = {.id = coluna, .tipo = INT };
		mapaTemporario[$$.label].tamanho = linha;
		mapaTemporario[$$.label].coluna = coluna;
		string tamanhoMatriz = "tmp" + proximaVariavelTemporaria();
		mapaTemporario[tamanhoMatriz] = {.id = tamanhoMatriz, .tipo = INT};
		$$.traducao = '\t' + linha + " = " + to_string(pilhaVetores.size()) + ";\n" +
				'\t' + coluna + " = " + to_string(pilhaVetores.top().size()) + ";\n" +
				'\t' + tamanhoMatriz + " = " + linha + " * " + coluna + ";\n" +
				'\t' + $$.label + " = (" + decideTipo($$.tipo) + ") malloc(sizeof(" + decideTipo(vetorParaElemento($$.tipo)) +
				")*" + tamanhoMatriz + ");\n";

		int cols = pilhaVetores.top().size();
		int i = 0;

		while (!pilhaVetores.empty()) {
			list<atributos> colunas = pilhaVetores.top();
			pilhaVetores.pop();
			if (colunas.size() != cols) yyerror("Apenas matrizes regulares podem ser declaradas.");
			for (list<atributos>::iterator it = colunas.begin(); it != colunas.end(); ++it) {
				defineTiposCompativeis(it->tipo, vetorParaElemento($$.tipo));
				if (it->tipo == FLUT32 && $$.tipo == INT_MATRIZ) {
					string varCast = "tmp" + proximaVariavelTemporaria();
					mapaTemporario[varCast] = {.id = varCast, .tipo = INT };
					$$.traducao += it->traducao + '\t' + varCast + " = (int) " + it->label + ";\n" +
						'\t' + $$.label + '[' + to_string(i) + "] = " + varCast + ";\n";
				} else if (it->tipo == INT && $$.tipo == FLUT32_MATRIZ) { 
					string varCast = "tmp" + proximaVariavelTemporaria();
					mapaTemporario[varCast] = {.id = varCast, .tipo = FLUT32 };
					$$.traducao += it->traducao + '\t' + varCast + " = (float) " + it->label + ";\n" +
						'\t' + $$.label + '[' + to_string(i) + "] = " + varCast + ";\n";
				} else {
					$$.traducao += it->traducao + '\t' + $$.label + '[' + to_string(i) +
						"] = " + it->label + ";\n";
				}
				i += 1;
			}
		}
	}
	return $$;
}


#line 502 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TK_NUM = 258,
    TK_BOOL = 259,
    TK_CHAR = 260,
    TK_STRING = 261,
    TK_MAIN = 262,
    TK_ID = 263,
    TK_TIPO_INT = 264,
    TK_TIPO_FLUT32 = 265,
    TK_TIPO_FLUT64 = 266,
    TK_TIPO_BOOL = 267,
    TK_TIPO_STRING = 268,
    TK_TIPO_CHAR = 269,
    TK_MAIS_MENOS = 270,
    TK_MULTI_DIV = 271,
    TK_CONCATENACAO = 272,
    TK_BIN = 273,
    TK_HEX = 274,
    TK_OCT = 275,
    TK_MODULO = 276,
    TK_CONCAT_IGUAL = 277,
    TK_RELACIONAL = 278,
    TK_LOGICO = 279,
    TK_SEJA = 280,
    TK_FIM = 281,
    TK_ERROR = 282,
    TK_IF = 283,
    TK_WHILE = 284,
    TK_BREAK = 285,
    TK_CONTINUE = 286,
    TK_DO = 287,
    TK_FOR = 288,
    TK_BREAK_ALL = 289,
    TK_OP_ABREV = 290,
    TK_OP_1 = 291,
    TK_ELSE = 292,
    TK_SWITCH = 293,
    TK_CASE = 294,
    TK_DEFAULT = 295,
    TK_SLICE = 296,
    TK_PRINT = 297,
    TK_SCAN = 298,
    TK_TIPO_FUNCAO = 299,
    TK_RETORNA = 300,
    TK_CAST = 301
  };
#endif
/* Tokens.  */
#define TK_NUM 258
#define TK_BOOL 259
#define TK_CHAR 260
#define TK_STRING 261
#define TK_MAIN 262
#define TK_ID 263
#define TK_TIPO_INT 264
#define TK_TIPO_FLUT32 265
#define TK_TIPO_FLUT64 266
#define TK_TIPO_BOOL 267
#define TK_TIPO_STRING 268
#define TK_TIPO_CHAR 269
#define TK_MAIS_MENOS 270
#define TK_MULTI_DIV 271
#define TK_CONCATENACAO 272
#define TK_BIN 273
#define TK_HEX 274
#define TK_OCT 275
#define TK_MODULO 276
#define TK_CONCAT_IGUAL 277
#define TK_RELACIONAL 278
#define TK_LOGICO 279
#define TK_SEJA 280
#define TK_FIM 281
#define TK_ERROR 282
#define TK_IF 283
#define TK_WHILE 284
#define TK_BREAK 285
#define TK_CONTINUE 286
#define TK_DO 287
#define TK_FOR 288
#define TK_BREAK_ALL 289
#define TK_OP_ABREV 290
#define TK_OP_1 291
#define TK_ELSE 292
#define TK_SWITCH 293
#define TK_CASE 294
#define TK_DEFAULT 295
#define TK_SLICE 296
#define TK_PRINT 297
#define TK_SCAN 298
#define TK_TIPO_FUNCAO 299
#define TK_RETORNA 300
#define TK_CAST 301

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 645 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   345

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  136
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  288

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      54,    55,     2,     2,    51,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    56,    49,
       2,    50,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    52,     2,    53,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    47,     2,    48,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   459,   459,   470,   475,   481,   486,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   507,   542,   555,   568,   605,   642,   646,   651,   657,
     660,   696,   699,   704,   709,   712,   726,   729,   734,   737,
     751,   754,   758,   763,   769,   771,   808,   811,   815,   820,
     826,   829,   865,   868,   874,   882,   886,   892,   900,   908,
     912,   918,   924,   962,   970,   979,  1001,  1013,  1037,  1070,
    1075,  1080,  1086,  1100,  1112,  1120,  1128,  1135,  1143,  1150,
    1158,  1165,  1169,  1191,  1213,  1245,  1260,  1270,  1279,  1287,
    1290,  1300,  1312,  1326,  1332,  1339,  1351,  1363,  1376,  1381,
    1386,  1393,  1404,  1409,  1413,  1420,  1429,  1437,  1445,  1455,
    1461,  1467,  1505,  1510,  1516,  1521,  1527,  1527,  1527,  1527,
    1527,  1529,  1535,  1538,  1544,  1547,  1568,  1574,  1589,  1594,
    1597,  1602,  1605,  1657,  1673,  1693,  1711
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_NUM", "TK_BOOL", "TK_CHAR",
  "TK_STRING", "TK_MAIN", "TK_ID", "TK_TIPO_INT", "TK_TIPO_FLUT32",
  "TK_TIPO_FLUT64", "TK_TIPO_BOOL", "TK_TIPO_STRING", "TK_TIPO_CHAR",
  "TK_MAIS_MENOS", "TK_MULTI_DIV", "TK_CONCATENACAO", "TK_BIN", "TK_HEX",
  "TK_OCT", "TK_MODULO", "TK_CONCAT_IGUAL", "TK_RELACIONAL", "TK_LOGICO",
  "TK_SEJA", "TK_FIM", "TK_ERROR", "TK_IF", "TK_WHILE", "TK_BREAK",
  "TK_CONTINUE", "TK_DO", "TK_FOR", "TK_BREAK_ALL", "TK_OP_ABREV",
  "TK_OP_1", "TK_ELSE", "TK_SWITCH", "TK_CASE", "TK_DEFAULT", "TK_SLICE",
  "TK_PRINT", "TK_SCAN", "TK_TIPO_FUNCAO", "TK_RETORNA", "TK_CAST", "'{'",
  "'}'", "';'", "'='", "','", "'['", "']'", "'('", "')'", "':'", "$accept",
  "S", "PILHA_GLOBAL", "BLOCO", "COMANDOS", "COMANDO", "DECLARACAO",
  "DECLARACAO_VF32", "DECLARACAO_F32", "DECLARACAO_VBOOL",
  "DECLARACAO_BOOL", "DECLARACAO_VCHAR", "DECLARACAO_CHAR",
  "DECLARACAO_VSTRING", "DECLARACAO_STRING", "DECLARACAO_VINT",
  "DECLARACAO_INT", "DECLARACAO_VETOR", "DCL_VECTOR_AUX",
  "DECLARACAO_MATRIZ", "DCL_MATRIZ_AUX", "ATRIBUICAO", "E", "R", "L", "SE",
  "BLOCO_IF", "BLOCO_ELSE", "ENQUANTO", "REPITA", "PARA",
  "PARA_ATRIBUICAO", "PARA_INCREMENTO", "ESCOLHA", "BLOCO_CASE", "CASOS",
  "CASO", "VALOR_CASE", "DEFAULT", "PRINT", "SCAN", "BLOCO_ITERACAO",
  "LOOP_CONTROLE", "TIPO_DADO", "ARGUMENTOS_FUNCAO",
  "ARGUMENTOS_FUNCAO_AUX", "DECLARA_FUNCAO", "TIPO_DADO_FUNC",
  "BLOCO_FUNCAO", "ARG_FUNC_CALL", "ARG_FUNC_CALL_AUX", "CALL_FUNCTION",
  "RETORNA", "DEC_DIN", "DEC_DIN_AUX", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   123,   125,    59,
      61,    44,    91,    93,    40,    41,    58
};
# endif

#define YYPACT_NINF -222

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-222)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -222,    27,   251,  -222,    60,    35,    57,    69,    93,    94,
     107,    -1,    71,  -222,  -222,  -222,  -222,  -222,    83,    84,
      90,   137,   128,  -222,   251,   103,   113,  -222,  -222,   121,
    -222,  -222,   127,   134,   142,  -222,   143,   153,   159,   128,
     128,  -222,   160,   128,   128,   -24,     4,   106,    36,   136,
     162,   160,   160,   163,   161,   208,   128,   213,  -222,   168,
    -222,  -222,  -222,  -222,    74,   128,   128,   128,   296,  -222,
    -222,  -222,  -222,  -222,  -222,  -222,  -222,  -222,  -222,  -222,
     296,   296,   160,   287,   202,   204,   164,   236,   172,    26,
     128,   178,  -222,  -222,    26,   181,  -222,  -222,   160,   184,
      26,   196,  -222,  -222,   128,   198,   128,   287,    11,    18,
     251,   205,   223,  -222,   199,    43,   200,   254,   128,    46,
    -222,   138,   128,   128,   128,   128,    34,    45,   128,   262,
     160,    89,   128,  -222,  -222,   115,   296,   173,   259,  -222,
     296,   261,  -222,   287,   204,   266,  -222,   296,   270,  -222,
     296,   280,  -222,   256,  -222,  -222,   242,   216,   264,  -222,
     243,   244,  -222,  -222,   241,   246,    76,  -222,    46,   277,
      -6,  -222,  -222,   296,  -222,  -222,   128,   128,   236,   128,
      23,   253,   -24,     4,   106,    36,   136,   298,  -222,   163,
     163,  -222,   160,   160,   -21,   232,   263,   128,   257,   296,
     180,  -222,   156,   128,  -222,  -222,   128,   178,   181,   184,
     196,   198,   265,   281,  -222,    87,   -16,   119,   267,   272,
     -21,  -222,  -222,  -222,  -222,  -222,  -222,   271,   232,   183,
     128,   274,   128,   152,  -222,    23,   203,  -222,  -222,  -222,
    -222,  -222,   128,   293,  -222,   223,  -222,  -222,  -222,   269,
     251,  -222,  -222,   318,  -222,  -222,   163,  -222,   222,   128,
     156,   275,  -222,  -222,  -222,   256,  -222,   163,  -222,   273,
     251,  -222,   276,  -222,  -222,   296,  -222,   128,  -222,  -222,
     163,  -222,   232,   156,  -222,   271,  -222,  -222
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     6,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   113,   114,   112,   112,   115,     0,     0,
       0,     0,     0,     2,     6,     0,     0,     9,    10,     0,
      13,    14,     0,     0,     0,    17,     0,     0,     0,     0,
       0,    64,     0,     0,   129,    50,    29,    34,    44,    38,
       0,     0,     0,     0,     0,     0,     0,     0,   127,     0,
      74,    79,    78,    73,    77,     0,     0,     0,   133,    81,
       5,     7,     8,    12,    15,    16,    11,    18,    19,    20,
      66,    65,     0,    62,    89,    63,     0,   131,     0,     0,
       0,    52,    48,    49,     0,    31,    27,    28,     0,    36,
       0,    46,    42,    43,     0,    40,     0,     0,     0,     0,
       6,     0,     0,   101,     0,     0,     0,   122,     0,    75,
      80,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   128,   132,     0,    47,     0,     0,    25,
      26,     0,    21,    32,    33,     0,    22,    41,     0,    24,
      37,     0,    23,   136,    93,   112,     0,     0,     0,    98,
       0,     0,   110,   111,     0,     0,     0,    76,    69,    70,
      72,    71,    88,    85,    86,    87,     0,     0,   131,     0,
       0,    53,    50,    29,    34,    44,    38,     0,   134,     0,
       0,     4,     0,     0,     0,     0,     0,     0,    82,    67,
       0,   130,     0,     0,    56,    54,     0,    52,    31,    36,
      46,    40,     0,    90,    95,     0,     0,     0,     0,     0,
     104,   103,   117,   120,   119,   116,   118,   124,     0,     0,
       0,     0,     0,     0,    58,     0,     0,    51,    30,    35,
      45,    39,     0,    94,    96,     0,   106,   107,   108,     0,
       6,   100,   102,     0,   121,   126,     0,    83,     0,     0,
       0,     0,    61,    55,    57,   136,    91,     0,    99,     0,
       6,   109,     0,   125,    84,    68,    59,     0,   135,    92,
       0,   105,     0,     0,    97,   124,    60,   123
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -222,  -222,  -222,  -173,   -20,  -222,  -222,   146,   122,   147,
     124,   148,   125,   150,   129,   155,   131,   -35,   105,   -34,
    -215,  -110,   -19,  -222,   -46,    98,  -222,  -222,  -222,  -222,
    -222,  -222,  -222,  -222,  -222,   123,  -222,  -222,  -222,  -222,
    -222,   -15,  -222,  -221,  -222,    59,  -222,  -222,  -222,  -222,
     167,    -2,  -222,  -222,    77
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   111,    23,    24,    25,    95,   142,    99,
     146,   105,   152,   101,   149,    91,   139,    92,   205,    93,
     234,    26,   107,    84,    85,    27,   189,   267,    28,    29,
      30,   160,   269,    31,   114,   219,   220,   249,   221,    32,
      33,    53,    34,   227,   165,   254,    35,   256,    59,    88,
     133,    69,    37,    38,   188
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      36,    54,   159,    68,    70,   108,   109,   255,   130,   122,
     123,    96,    97,   102,   103,   125,   213,   214,   217,   218,
      80,    81,    36,    83,    86,    87,    89,     3,    90,    60,
      61,    62,    63,   245,    64,   130,   127,   115,   122,   123,
     124,    65,   130,    45,   125,   276,   119,   120,   121,   122,
     123,   124,   144,    51,    94,   125,    90,   128,   122,   123,
     124,   285,   123,   126,   125,    46,   154,   125,   286,   130,
     136,   137,    66,   155,   203,   140,   204,    47,   135,   143,
      67,   147,    39,   273,   175,   150,   100,   153,    90,   167,
     156,   122,   123,   124,   279,    40,    41,   125,   162,   166,
     172,    48,    49,   168,   169,   170,   171,   284,    36,   173,
      42,   130,    43,   178,    44,    50,   180,   197,    60,    61,
      62,    63,   246,    64,   247,    52,   118,   248,    44,   198,
      65,    60,    61,    62,    63,   268,    64,    55,    56,   176,
     190,   177,   244,    65,    57,    58,   215,   216,    96,    97,
     102,   103,    71,   122,   123,   124,    98,   199,   200,   125,
     202,    66,    72,    60,    61,    62,    63,   179,    64,    67,
      73,   122,   123,   124,    66,    65,    74,   125,   229,   122,
     123,   124,    67,    75,   235,   125,   104,   236,   122,   123,
     124,    76,    77,   167,   125,   122,   123,   124,   122,   123,
     124,   125,    78,   261,   125,   262,    66,   232,    79,   233,
     110,   258,   106,   260,    82,   112,   113,   131,   122,   123,
     124,   116,   117,   265,   125,   129,   181,   134,   130,   138,
     271,   158,   141,   231,   157,   145,   257,   122,   123,   124,
     275,   222,   223,   125,   224,   225,   226,   148,    36,   151,
     281,   122,   123,   124,   161,   163,   264,   125,   283,     4,
       5,     6,   164,     7,     8,     9,   174,   182,    36,   183,
     192,   122,   123,   124,   184,   274,    10,   125,   185,    11,
      12,    13,    14,    15,    16,    17,    39,   132,   186,    18,
     191,   194,   193,    19,    20,    21,    22,   195,   125,    40,
      41,   196,   122,   123,   124,   206,   212,   187,   125,   230,
     128,   122,   123,   124,    42,   242,    43,   125,   243,   228,
     251,    11,   253,   250,   259,   270,   272,   277,   280,   208,
     238,   209,   282,   239,   211,   210,   241,   207,   237,   240,
     263,   266,   278,   252,   287,   201
};

static const yytype_uint16 yycheck[] =
{
       2,    16,   112,    22,    24,    51,    52,   228,    24,    15,
      16,    46,    46,    48,    48,    21,   189,   190,    39,    40,
      39,    40,    24,    42,    43,    44,    50,     0,    52,     3,
       4,     5,     6,    49,     8,    24,    82,    56,    15,    16,
      17,    15,    24,     8,    21,   260,    65,    66,    67,    15,
      16,    17,    98,    54,    50,    21,    52,    23,    15,    16,
      17,   282,    16,    82,    21,     8,    55,    21,   283,    24,
      89,    90,    46,    55,    51,    94,    53,     8,    52,    98,
      54,   100,    22,   256,   130,   104,    50,   106,    52,    55,
     110,    15,    16,    17,   267,    35,    36,    21,    55,   118,
      55,     8,     8,   122,   123,   124,   125,   280,   110,   128,
      50,    24,    52,   132,    54,     8,   135,    41,     3,     4,
       5,     6,     3,     8,     5,    54,    52,     8,    54,    53,
      15,     3,     4,     5,     6,   245,     8,    54,    54,    50,
     155,    52,    55,    15,    54,     8,   192,   193,   183,   183,
     185,   185,    49,    15,    16,    17,    50,   176,   177,    21,
     179,    46,    49,     3,     4,     5,     6,    52,     8,    54,
      49,    15,    16,    17,    46,    15,    49,    21,   197,    15,
      16,    17,    54,    49,   203,    21,    50,   206,    15,    16,
      17,    49,    49,    55,    21,    15,    16,    17,    15,    16,
      17,    21,    49,    51,    21,    53,    46,    51,    49,    53,
      47,   230,    50,   232,    54,    54,     8,    53,    15,    16,
      17,     8,    54,   242,    21,    23,    53,    55,    24,    51,
     250,     8,    51,    53,    29,    51,    53,    15,    16,    17,
     259,     9,    10,    21,    12,    13,    14,    51,   250,    51,
     270,    15,    16,    17,    55,    55,    53,    21,   277,     8,
       9,    10,     8,    12,    13,    14,     4,     8,   270,     8,
      54,    15,    16,    17,     8,    53,    25,    21,     8,    28,
      29,    30,    31,    32,    33,    34,    22,    51,     8,    38,
      48,    47,    49,    42,    43,    44,    45,    56,    21,    35,
      36,    55,    15,    16,    17,    52,     8,    51,    21,    52,
      23,    15,    16,    17,    50,    50,    52,    21,    37,    56,
      48,    28,    51,    56,    50,    56,     8,    52,    55,   183,
     208,   184,    56,   209,   186,   185,   211,   182,   207,   210,
     235,   243,   265,   220,   285,   178
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    58,    59,     0,     8,     9,    10,    12,    13,    14,
      25,    28,    29,    30,    31,    32,    33,    34,    38,    42,
      43,    44,    45,    61,    62,    63,    78,    82,    85,    86,
      87,    90,    96,    97,    99,   103,   108,   109,   110,    22,
      35,    36,    50,    52,    54,     8,     8,     8,     8,     8,
       8,    54,    54,    98,    98,    54,    54,    54,     8,   105,
       3,     4,     5,     6,     8,    15,    46,    54,    79,   108,
      61,    49,    49,    49,    49,    49,    49,    49,    49,    49,
      79,    79,    54,    79,    80,    81,    79,    79,   106,    50,
      52,    72,    74,    76,    50,    64,    74,    76,    50,    66,
      50,    70,    74,    76,    50,    68,    50,    79,    81,    81,
      47,    60,    54,     8,    91,    79,     8,    54,    52,    79,
      79,    79,    15,    16,    17,    21,    79,    81,    23,    23,
      24,    53,    51,   107,    55,    52,    79,    79,    51,    73,
      79,    51,    65,    79,    81,    51,    67,    79,    51,    71,
      79,    51,    69,    79,    55,    55,    61,    29,     8,    78,
      88,    55,    55,    55,     8,   101,    79,    55,    79,    79,
      79,    79,    55,    79,     4,    81,    50,    52,    79,    52,
      79,    53,     8,     8,     8,     8,     8,    51,   111,    83,
      98,    48,    54,    49,    47,    56,    55,    41,    53,    79,
      79,   107,    79,    51,    53,    75,    52,    72,    64,    66,
      70,    68,     8,    60,    60,    81,    81,    39,    40,    92,
      93,    95,     9,    10,    12,    13,    14,   100,    56,    79,
      52,    53,    51,    53,    77,    79,    79,    73,    65,    67,
      71,    69,    50,    37,    55,    49,     3,     5,     8,    94,
      56,    48,    92,    51,   102,   100,   104,    53,    79,    50,
      79,    51,    53,    75,    53,    79,    82,    84,    78,    89,
      56,    61,     8,    60,    53,    79,    77,    52,   111,    60,
      55,    61,    56,    79,    60,   100,    77,   102
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    57,    58,    59,    60,    61,    61,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    63,    63,    63,    63,    63,    64,    64,    64,    64,
      65,    65,    66,    66,    66,    67,    67,    68,    68,    69,
      69,    70,    70,    70,    70,    71,    71,    72,    72,    72,
      72,    73,    73,    74,    74,    75,    75,    76,    76,    77,
      77,    77,    78,    78,    78,    78,    78,    78,    78,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    80,    80,    81,    81,    81,
      82,    82,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    92,    92,    93,    94,    94,    94,    95,
      96,    97,    98,    99,    99,    99,   100,   100,   100,   100,
     100,   101,   101,   102,   102,   103,   104,   105,   106,   106,
     107,   107,   108,   109,   110,   111,   111
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     3,     2,     0,     2,     2,     1,
       1,     2,     2,     1,     1,     2,     2,     1,     2,     2,
       2,     4,     4,     4,     4,     4,     2,     1,     1,     0,
       4,     0,     2,     2,     0,     4,     0,     2,     0,     4,
       0,     2,     1,     1,     0,     4,     0,     2,     1,     1,
       0,     4,     0,     3,     4,     3,     1,     6,     5,     3,
       5,     2,     3,     3,     2,     3,     3,     6,     9,     3,
       3,     3,     3,     1,     1,     2,     3,     1,     1,     1,
       2,     1,     4,     6,     7,     3,     3,     3,     3,     1,
       6,     8,     9,     0,     0,     6,     7,    10,     1,     1,
       7,     1,     2,     1,     1,     4,     1,     1,     1,     3,
       4,     4,     0,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     0,     5,     0,     8,     1,     1,     2,     0,
       3,     0,     4,     2,     5,     5,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 460 "sintatica.y" /* yacc.c:1646  */
    {
				cout << "/*Compilador FAEN*/" << endl;
				cout << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\n#include<stdlib.h>\n#define SIZE_STR 10\nusing namespace std;" << endl;
				cout << declaraVariaveisGlobais() << endl;
				cout << traduzirFuncoes() << endl;
				cout << "int main(void)\n{\n" << declaraVariaveisTemporarias() + (yyvsp[0]).traducao + liberarStrings() << "\treturn 0;\n}" << endl; 
			}
#line 1948 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 470 "sintatica.y" /* yacc.c:1646  */
    {
				pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
					.rotuloInicio = proximoRotulo(), .rotuloFim = proximoRotulo(), .funcional = false, .isGlobal = true});
			}
#line 1957 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 476 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao;
			}
#line 1965 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 482 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 1973 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 486 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 1981 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 508 "sintatica.y" /* yacc.c:1646  */
    {
				if ((yyvsp[-1]).tipo != "" && (yyvsp[-1]).tipo != FLUT32_VETOR && (yyvsp[-1]).tipo != FLUT32_MATRIZ) defineTiposCompativeis((yyvsp[-3]).tipo, (yyvsp[-1]).tipo);
				verificaVariavelJaDeclarada((yyvsp[-2]).label);
				(yyval).label = "tmp" + proximaVariavelTemporaria();
	        	(yyval).tipo = (yyvsp[-1]).tipo == FLUT32_VETOR || (yyvsp[-1]).tipo == FLUT32_MATRIZ ? (yyvsp[-1]).tipo : FLUT32;
	  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
	  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				if ((yyvsp[-1]).tipo != "") {
					if ((yyvsp[-1]).tipo == INT) {
						string varCast = "tmp" + proximaVariavelTemporaria();
						mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
						(yyval).traducao = (yyvsp[-1]).traducao + '\t' + varCast + " = (float) " + (yyvsp[-1]).label + ";\n" +
						'\t' + (yyval).label + " = " + varCast + ";\n" + (yyvsp[0]).traducao;
					}  else if ((yyvsp[-1]).tipo == FLUT32_VETOR) {
						if (pilhaVetores.size() > 0) {
							(yyval) = traducaoVetorNumerico((yyval), (yyvsp[-1]));
							(yyval).traducao += (yyvsp[0]).traducao;
						} else {
							string tamanhoVetor = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[tamanhoVetor] = {.id = tamanhoVetor, .tipo = INT};
							mapaTemporario[(yyval).label].tamanho = tamanhoVetor;
							(yyval).traducao = (yyvsp[-1]).traducao + '\t' + tamanhoVetor + " = " + mapaTemporario[(yyvsp[-1]).label].tamanho + ";\n" +
								'\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
						}
					} else if ((yyvsp[-1]).tipo == FLUT32_MATRIZ) {
						(yyval) = traducaoMatrizNumerico((yyval), (yyvsp[-1]));
						(yyval).traducao += (yyvsp[0]).traducao;
					} else {
						(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
					}
				} else {
					(yyval).traducao = (yyvsp[0]).traducao;
				}
			}
#line 2020 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 543 "sintatica.y" /* yacc.c:1646  */
    {
				verificaVariavelJaDeclarada((yyvsp[-2]).label);
				(yyval).label = "tmp" + proximaVariavelTemporaria();
	        	(yyval).tipo = BOOL;
	  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
	  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				if ((yyvsp[-1]).tipo != "") {
					(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
				} else {
					(yyval).traducao = (yyvsp[0]).traducao;	
				}
			}
#line 2037 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 556 "sintatica.y" /* yacc.c:1646  */
    {
      			verificaVariavelJaDeclarada((yyvsp[-2]).label);
				(yyval).label = "tmp" + proximaVariavelTemporaria();
	        	(yyval).tipo = CHAR;
	  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
	  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
	  			if ((yyvsp[-1]).tipo != "") {
					(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
				} else {
					(yyval).traducao = (yyvsp[0]).traducao;	
				}
      		}
#line 2054 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 569 "sintatica.y" /* yacc.c:1646  */
    {
      			if ((yyvsp[-1]).tipo != "" && (yyvsp[-1]).tipo != CHARS_VETOR && (yyvsp[-1]).tipo != CHARS_MATRIZ) defineTiposCompativeis((yyvsp[-3]).tipo, (yyvsp[-1]).tipo);
      			verificaVariavelJaDeclarada((yyvsp[-2]).label);
        		(yyval).label = "tmp" + proximaVariavelTemporaria();
	        	(yyval).tipo = (yyvsp[-1]).tipo == CHARS_VETOR || (yyvsp[-1]).tipo == CHARS_MATRIZ ? (yyvsp[-1]).tipo : (yyvsp[-3]).tipo;
	        	pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo};

	        	if ((yyval).tipo == CHARS_VETOR) {
	        		if (pilhaVetores.size() > 0) {
							(yyval) = traducaoVetorNumerico((yyval), (yyvsp[-1]));
							(yyval).traducao += (yyvsp[0]).traducao;
					} else {
						string tamanhoVetor = "tmp" + proximaVariavelTemporaria();
						mapaTemporario[tamanhoVetor] = {.id = tamanhoVetor, .tipo = INT};
						mapaTemporario[(yyval).label].tamanho = tamanhoVetor;
						(yyval).traducao = (yyvsp[-1]).traducao + '\t' + tamanhoVetor + " = " + mapaTemporario[(yyvsp[-1]).label].tamanho + ";\n" +
							'\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
					}
	        	} else if ((yyval).tipo == CHARS_MATRIZ) {
					(yyval) = traducaoMatrizNumerico((yyval), (yyvsp[-1]));
					(yyval).traducao += (yyvsp[0]).traducao;
	        	} else {
					string tamanhoString = "tmp" + proximaVariavelTemporaria();
					mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };
					mapaTemporario[(yyval).label].tamanho = tamanhoString;
					if ((yyvsp[-1]).tipo != "") {
						(yyval).traducao = (yyvsp[-1]).traducao + 
							'\t' + tamanhoString + " = " + mapaTemporario[(yyvsp[-1]).label].tamanho + ";\n" +
							'\t' + (yyval).label + " = (char*) malloc(" + tamanhoString + ");\n" + 
							"\tstrcat(" + (yyval).label + ", " + (yyvsp[-1]).label + ");\n" + (yyvsp[0]).traducao;						
					} else {
						(yyval).traducao = '\t' + tamanhoString + " = 0;\n" + (yyvsp[0]).traducao;
					}
				}
      		}
#line 2095 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 606 "sintatica.y" /* yacc.c:1646  */
    {
      			if ((yyvsp[-1]).tipo != "" && (yyvsp[-1]).tipo != INT_VETOR && (yyvsp[-1]).tipo != INT_MATRIZ) defineTiposCompativeis((yyvsp[-3]).tipo, (yyvsp[-1]).tipo);
      			verificaVariavelJaDeclarada((yyvsp[-2]).label);
        		(yyval).label = "tmp" + proximaVariavelTemporaria();
	        	(yyval).tipo = (yyvsp[-1]).tipo == INT_VETOR || (yyvsp[-1]).tipo == INT_MATRIZ ? (yyvsp[-1]).tipo : (yyvsp[-3]).tipo;
	  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
	  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				if ((yyvsp[-1]).tipo != "") {
					if ((yyvsp[-1]).tipo == FLUT32) {
						string varCast = "tmp" + proximaVariavelTemporaria();
						mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
						(yyval).traducao = (yyvsp[-1]).traducao + '\t' + varCast + " = (int) " + (yyvsp[-1]).label + ";\n" +
						'\t' + (yyval).label + " = " + varCast + ";\n" + (yyvsp[0]).traducao;
					} else if ((yyvsp[-1]).tipo == INT_VETOR) {
						if (pilhaVetores.size() > 0) {
							(yyval) = traducaoVetorNumerico((yyval), (yyvsp[-1]));
							(yyval).traducao += (yyvsp[0]).traducao;
						} else {
							string tamanhoVetor = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[tamanhoVetor] = {.id = tamanhoVetor, .tipo = INT};
							mapaTemporario[(yyval).label].tamanho = tamanhoVetor;
							(yyval).traducao = (yyvsp[-1]).traducao + '\t' + tamanhoVetor + " = " + mapaTemporario[(yyvsp[-1]).label].tamanho + ";\n" +
								'\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
						}
					} else if ((yyvsp[-1]).tipo == INT_MATRIZ) {
						(yyval) = traducaoMatrizNumerico((yyval), (yyvsp[-1]));
						(yyval).traducao += (yyvsp[0]).traducao;
					} else {
						(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
					}
				} else {
					(yyval).traducao = (yyvsp[0]).traducao;	
				}
      		}
#line 2134 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 643 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 }
#line 2142 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 647 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 	(yyval).tipo = FLUT32_VETOR;
				 }
#line 2151 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 652 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 	(yyval).tipo = FLUT32_MATRIZ;
				 }
#line 2160 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 657 "sintatica.y" /* yacc.c:1646  */
    { (yyval).traducao = ""; (yyval).tipo = ""; (yyval).label = ""; }
#line 2166 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 661 "sintatica.y" /* yacc.c:1646  */
    {
					if ((yyvsp[-1]).tipo != "" && (yyvsp[-1]).tipo != FLUT32_VETOR && (yyvsp[-1]).tipo != FLUT32_MATRIZ) defineTiposCompativeis((yyvsp[-3]).tipo, (yyvsp[-1]).tipo);
					verificaVariavelJaDeclarada((yyvsp[-2]).label);
					(yyval).label = "tmp" + proximaVariavelTemporaria();
		        	(yyval).tipo = (yyvsp[-1]).tipo == FLUT32_VETOR || (yyvsp[-1]).tipo == FLUT32_MATRIZ ? (yyvsp[-1]).tipo : FLUT32;
		  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
		  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
					if ((yyvsp[-1]).tipo != "") {
						if ((yyvsp[-1]).tipo == INT) {
							string varCast = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
							(yyval).traducao = (yyvsp[-1]).traducao + '\t' + varCast + " = (float) " + (yyvsp[-1]).label + ";\n" +
							'\t' + (yyval).label + " = " + varCast + ";\n" + (yyvsp[0]).traducao;
						} else if ((yyvsp[-1]).tipo == FLUT32_VETOR) {
							if (pilhaVetores.size() > 0) {
								(yyval) = traducaoVetorNumerico((yyval), (yyvsp[-1]));
								(yyval).traducao += (yyvsp[0]).traducao;
							} else {
								string tamanhoVetor = "tmp" + proximaVariavelTemporaria();
								mapaTemporario[tamanhoVetor] = {.id = tamanhoVetor, .tipo = INT};
								mapaTemporario[(yyval).label].tamanho = tamanhoVetor;
								(yyval).traducao = (yyvsp[-1]).traducao + '\t' + tamanhoVetor + " = " + mapaTemporario[(yyvsp[-1]).label].tamanho + ";\n" +
									'\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
							}
						} else if ((yyvsp[-1]).tipo == FLUT32_MATRIZ) {
							(yyval) = traducaoMatrizNumerico((yyval), (yyvsp[-1]));
							(yyval).traducao += (yyvsp[0]).traducao;
						} else {
							(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
						}
					} else {
						(yyval).traducao = (yyvsp[0]).traducao;
					}
				}
#line 2205 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 696 "sintatica.y" /* yacc.c:1646  */
    {	(yyval).traducao = "";	}
#line 2211 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 700 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 }
#line 2219 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 705 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 }
#line 2227 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 709 "sintatica.y" /* yacc.c:1646  */
    { (yyval).traducao = ""; (yyval).tipo = ""; (yyval).label = ""; }
#line 2233 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 713 "sintatica.y" /* yacc.c:1646  */
    {
					verificaVariavelJaDeclarada((yyvsp[-2]).label);
					(yyval).label = "tmp" + proximaVariavelTemporaria();
		        	(yyval).tipo = BOOL;
		  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
		  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
					if ((yyvsp[-1]).tipo != "") {
						(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
					} else {
						(yyval).traducao = (yyvsp[0]).traducao;	
					}
				}
#line 2250 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 726 "sintatica.y" /* yacc.c:1646  */
    {	(yyval).traducao = "";	}
#line 2256 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 730 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 }
#line 2264 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 734 "sintatica.y" /* yacc.c:1646  */
    { (yyval).traducao = ""; (yyval).tipo = ""; (yyval).label = ""; }
#line 2270 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 738 "sintatica.y" /* yacc.c:1646  */
    {
					verificaVariavelJaDeclarada((yyvsp[-2]).label);
					(yyval).label = "tmp" + proximaVariavelTemporaria();
		        	(yyval).tipo = CHAR;
		  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
		  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
					if ((yyvsp[-1]).tipo != "") {
						(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
					} else {
						(yyval).traducao = (yyvsp[0]).traducao;	
					}
				}
#line 2287 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 751 "sintatica.y" /* yacc.c:1646  */
    {	(yyval).traducao = "";	}
#line 2293 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 755 "sintatica.y" /* yacc.c:1646  */
    {
						(yyval) = (yyvsp[0]);
					}
#line 2301 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 759 "sintatica.y" /* yacc.c:1646  */
    {
					 	(yyval) = (yyvsp[0]);
					 	(yyval).tipo = CHARS_VETOR;
					}
#line 2310 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 764 "sintatica.y" /* yacc.c:1646  */
    {
					 	(yyval) = (yyvsp[0]);
					 	(yyval).tipo = CHARS_MATRIZ;
					}
#line 2319 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 769 "sintatica.y" /* yacc.c:1646  */
    {	(yyval).traducao = ""; (yyval).tipo = ""; (yyval).label = "";	}
#line 2325 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 772 "sintatica.y" /* yacc.c:1646  */
    {
						if ((yyvsp[-1]).tipo != "" && (yyvsp[-1]).tipo != CHARS_VETOR && (yyvsp[-1]).tipo != CHARS_MATRIZ) defineTiposCompativeis(CHARS, (yyvsp[-1]).tipo);
		      			verificaVariavelJaDeclarada((yyvsp[-2]).label);
		        		(yyval).label = "tmp" + proximaVariavelTemporaria();
			        	(yyval).tipo = (yyvsp[-1]).tipo == CHARS_VETOR || (yyvsp[-1]).tipo == CHARS_MATRIZ ? (yyvsp[-1]).tipo : CHARS;
			        	pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
						mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };

			        	if ((yyval).tipo == CHARS_VETOR) {
			        		if (pilhaVetores.size() > 0) {
								(yyval) = traducaoVetorNumerico((yyval), (yyvsp[-1]));
								(yyval).traducao += (yyvsp[0]).traducao;
							} else {
								string tamanhoVetor = "tmp" + proximaVariavelTemporaria();
								mapaTemporario[tamanhoVetor] = {.id = tamanhoVetor, .tipo = INT};
								mapaTemporario[(yyval).label].tamanho = tamanhoVetor;
								(yyval).traducao = (yyvsp[-1]).traducao + '\t' + tamanhoVetor + " = " + mapaTemporario[(yyvsp[-1]).label].tamanho + ";\n" +
									'\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
							}
			        	} else if ((yyval).tipo == CHARS_MATRIZ) {
			        		(yyval) = traducaoMatrizNumerico((yyval), (yyvsp[-1]));
							(yyval).traducao += (yyvsp[0]).traducao;
			        	} else {
							string tamanhoString = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };
							mapaTemporario[(yyval).label].tamanho = tamanhoString;
							if ((yyvsp[-1]).tipo != "") {
								(yyval).traducao = (yyvsp[-1]).traducao + '\t' + tamanhoString + " = " + mapaTemporario[(yyvsp[-1]).label].tamanho + ";\n" +
									'\t' + (yyval).label + " = (char*) malloc(" + tamanhoString + ");\n" + 
									"\tstrcat(" + (yyval).label + ", " + (yyvsp[-1]).label + ");\n" + (yyvsp[0]).traducao;						
							} else {
								(yyval).traducao = '\t' + tamanhoString + " = 0;\n" + (yyvsp[0]).traducao;
							}
						}
					}
#line 2365 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 808 "sintatica.y" /* yacc.c:1646  */
    {	(yyval).traducao = "";	}
#line 2371 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 812 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 }
#line 2379 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 816 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 	(yyval).tipo = INT_VETOR;
				 }
#line 2388 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 821 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 	(yyval).tipo = INT_MATRIZ;
				 }
#line 2397 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 826 "sintatica.y" /* yacc.c:1646  */
    { (yyval).traducao = ""; (yyval).tipo = ""; (yyval).label = ""; }
#line 2403 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 830 "sintatica.y" /* yacc.c:1646  */
    {
					if ((yyvsp[-1]).tipo != "" && (yyvsp[-1]).tipo != INT_VETOR && (yyvsp[-1]).tipo != INT_MATRIZ) defineTiposCompativeis((yyvsp[-3]).tipo, (yyvsp[-1]).tipo);
					verificaVariavelJaDeclarada((yyvsp[-2]).label);
					(yyval).label = "tmp" + proximaVariavelTemporaria();
		        	(yyval).tipo = (yyvsp[-1]).tipo == INT_VETOR || (yyvsp[-1]).tipo == INT_MATRIZ ? (yyvsp[-1]).tipo : INT;
		  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
		  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
					if ((yyvsp[-1]).tipo != "") {
						if ((yyvsp[-1]).tipo == FLUT32) {
							string varCast = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
							(yyval).traducao = (yyvsp[-1]).traducao + '\t' + varCast + " = (int) " + (yyvsp[-1]).label + ";\n" +
							'\t' + (yyval).label + " = " + varCast + ";\n" + (yyvsp[0]).traducao;
						} else if ((yyvsp[-1]).tipo == INT_VETOR) {
							if (pilhaVetores.size() > 0) {
								(yyval) = traducaoVetorNumerico((yyval), (yyvsp[-1]));
								(yyval).traducao += (yyvsp[0]).traducao;
							} else {
								string tamanhoVetor = "tmp" + proximaVariavelTemporaria();
								mapaTemporario[tamanhoVetor] = {.id = tamanhoVetor, .tipo = INT};
								mapaTemporario[(yyval).label].tamanho = tamanhoVetor;
								(yyval).traducao = (yyvsp[-1]).traducao + '\t' + tamanhoVetor + " = " + mapaTemporario[(yyvsp[-1]).label].tamanho + ";\n" +
									'\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
							}
						} else if ((yyvsp[-1]).tipo == INT_MATRIZ) {
							(yyval) = traducaoMatrizNumerico((yyval), (yyvsp[-1]));
							(yyval).traducao += (yyvsp[0]).traducao;
						} else {
							(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
						}
					} else {
						(yyval).traducao = (yyvsp[0]).traducao;
					}
				}
#line 2442 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 865 "sintatica.y" /* yacc.c:1646  */
    {	(yyval).traducao = "";	}
#line 2448 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 869 "sintatica.y" /* yacc.c:1646  */
    {
					pilhaVetores.push(list<atributos>());
					defineTiposCompativeis((yyvsp[-1]).tipo, INT);
					(yyval) = (yyvsp[-1]);
				}
#line 2458 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 875 "sintatica.y" /* yacc.c:1646  */
    {
					valoresVetorAuxiliar.push_front((yyvsp[-1]));
					pilhaVetores.push(valoresVetorAuxiliar);
					valoresVetorAuxiliar.clear();
				}
#line 2468 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 883 "sintatica.y" /* yacc.c:1646  */
    {
					valoresVetorAuxiliar.push_front((yyvsp[-1]));
				}
#line 2476 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 887 "sintatica.y" /* yacc.c:1646  */
    {
					(yyval).traducao = ""; (yyval).label = ""; (yyval).tipo = "";
				}
#line 2484 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 893 "sintatica.y" /* yacc.c:1646  */
    {
					defineTiposCompativeis((yyvsp[-4]).tipo, INT);
					defineTiposCompativeis((yyvsp[-1]).tipo, INT);
					(yyval) = (yyvsp[-4]);
					(yyval).traducao += (yyvsp[-1]).traducao;
					(yyval).coluna = (yyvsp[-1]).label;
				}
#line 2496 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 901 "sintatica.y" /* yacc.c:1646  */
    {
					valoresVetorAuxiliar.push_front((yyvsp[-1]));
					pilhaVetores.push(valoresVetorAuxiliar);
					valoresVetorAuxiliar.clear();
				}
#line 2506 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 909 "sintatica.y" /* yacc.c:1646  */
    {
					valoresVetorAuxiliar.push_front((yyvsp[-1]));
				}
#line 2514 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 913 "sintatica.y" /* yacc.c:1646  */
    {
					valoresVetorAuxiliar.push_front((yyvsp[-1]));
					pilhaVetores.push(valoresVetorAuxiliar);
					valoresVetorAuxiliar.clear();	
				}
#line 2524 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 919 "sintatica.y" /* yacc.c:1646  */
    {
					(yyval).traducao = ""; (yyval).label = ""; (yyval).tipo = "";	
				}
#line 2532 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 925 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[-2]).label);
				(yyvsp[-2]).tipo = mapaTemporario[mapa[(yyvsp[-2]).label].temporario].tipo;
				defineTiposCompativeis((yyvsp[-2]).tipo, (yyvsp[0]).tipo);
				if ((yyvsp[-2]).tipo != (yyvsp[0]).tipo) {
					string varCast = "tmp" + proximaVariavelTemporaria();

					if ((yyvsp[-2]).tipo == INT) {
						(yyval).tipo = INT;
						mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
						(yyval).traducao = (yyvsp[0]).traducao + '\t' + varCast + " = (int) " + (yyvsp[0]).label + ";\n" +
						'\t' + mapaTemporario[mapa[(yyvsp[-2]).label].temporario].id + " = " + varCast + ";\n";
					} else if ((yyvsp[-2]).tipo == FLUT32){
						(yyval).tipo = FLUT32;
						mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
						(yyval).traducao = (yyvsp[0]).traducao + '\t' + varCast + " = (float) " + (yyvsp[0]).label + ";\n" +
						'\t' + mapaTemporario[mapa[(yyvsp[-2]).label].temporario].id + " = " + varCast + ";\n";
					}
				} else if ((yyvsp[-2]).tipo == CHARS) {
					string temporarioId = mapa[(yyvsp[-2]).label].temporario;
					string tamanhoString = mapaTemporario[temporarioId].tamanho;
					(yyval).traducao = (yyvsp[0]).traducao + "\tif(" + tamanhoString + " > 0) free(" + temporarioId + ");\n" +
						'\t' + tamanhoString + " = " + mapaTemporario[(yyvsp[0]).label].tamanho + ";\n" +
				 		'\t' + temporarioId + " = (char*) malloc(" + tamanhoString + ");\n" + 
						"\tstrcat(" + temporarioId + ", " + (yyvsp[0]).label + ");\n";
				} else if ((yyvsp[-2]).tipo == INT_VETOR || (yyvsp[-2]).tipo == FLUT32_VETOR || (yyvsp[-2]).tipo == CHARS_VETOR) {
					string tamanhoVetor = "tmp" + proximaVariavelTemporaria();
					mapaTemporario[tamanhoVetor] = {.id = tamanhoVetor, .tipo = INT};
					mapaTemporario[mapa[(yyvsp[-2]).label].temporario].tamanho = tamanhoVetor;
					(yyval).traducao = (yyvsp[0]).traducao + '\t' + tamanhoVetor + " = " + mapaTemporario[(yyvsp[0]).label].tamanho + ";\n" +
						'\t' + mapaTemporario[mapa[(yyvsp[-2]).label].temporario].id + " = " + (yyvsp[0]).label + ";\n";
				} else {
					(yyval).traducao = (yyvsp[0]).traducao + '\t' + mapaTemporario[mapa[(yyvsp[-2]).label].temporario].id + 
					" = " + (yyvsp[0]).label + ";\n";
				}
			}
#line 2573 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 963 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[-2]).label);
				defineTiposCompativeis((yyvsp[-2]).tipo, (yyvsp[0]).tipo);
				(yyval).traducao = (yyvsp[0]).traducao + '\t' + mapaTemporario[mapa[(yyvsp[-2]).label].temporario].id + 
				" = " + (yyvsp[0]).label + ";\n";	
			}
#line 2584 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 971 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[-1]).label);
				defineTiposCompativeis(mapa[(yyvsp[-1]).label].tipo, INT);
				string mais1 = mapaTemporario[mapa[(yyvsp[-1]).label].temporario].tipo == INT ? " 1;\n" : " 1.0;\n";
				string op = (yyvsp[0]).traducao == "++" ? " +" : " -";
				(yyval).traducao = '\t' + mapaTemporario[mapa[(yyvsp[-1]).label].temporario].id +
				" = " + mapaTemporario[mapa[(yyvsp[-1]).label].temporario].id + op + mais1;
			}
#line 2597 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 980 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[-2]).label);
        		string var = mapa[(yyvsp[-2]).label].temporario;
        		(yyvsp[-2]).label = mapaTemporario[var].id;
        		(yyvsp[-2]).tipo = mapaTemporario[var].tipo;
        		(yyvsp[-2]).traducao = "";
     		 	struct atributos op;
    		    if ((yyvsp[-1]).traducao == "+=") { 
      		  		op.traducao = " + ";
      		  	} else if ((yyvsp[-1]).traducao == "-=") {
        			op.traducao = " - "; 
		        } else if ((yyvsp[-1]).traducao == "*=") {
		        	op.traducao = " * ";
       			} else if ((yyvsp[-1]).traducao == "%=") {
       				op.traducao = " % ";
       			} else {
        			op.traducao = " / ";
				}
				(yyval) = processaOpAritmetica((yyvsp[-2]), op, (yyvsp[0]));
				(yyval).traducao += '\t' + (yyvsp[-2]).label + " = " + (yyval).label + ";\n";
			}
#line 2623 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1002 "sintatica.y" /* yacc.c:1646  */
    {
				defineTiposCompativeis(CHARS, (yyvsp[0]).tipo);
				mapV mapa = buscaMapa((yyvsp[-2]).label);
				defineTiposCompativeis(CHARS, mapa[(yyvsp[-2]).label].tipo);
        		(yyval).traducao = (yyvsp[0]).traducao + '\t' + mapaTemporario[mapa[(yyvsp[-2]).label].temporario].tamanho +
        						" = " + mapaTemporario[mapa[(yyvsp[-2]).label].temporario].tamanho + " + " 
        						+ mapaTemporario[(yyvsp[0]).label].tamanho + ";\n" + '\t' + mapa[(yyvsp[-2]).label].temporario + 
        						" = (char*) realloc(" + mapa[(yyvsp[-2]).label].temporario + 
        						", " + mapaTemporario[(yyvsp[0]).label].tamanho + ");\n" +
        						"\tstrcat(" + mapa[(yyvsp[-2]).label].temporario + ", " + (yyvsp[0]).label + ");\n";
			}
#line 2639 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1014 "sintatica.y" /* yacc.c:1646  */
    {
				defineTiposCompativeis((yyvsp[-3]).tipo, INT);
				mapV mapa = buscaMapa((yyvsp[-5]).label);
				defineTiposCompativeis(vetorParaElemento(mapa[(yyvsp[-5]).label].tipo), (yyvsp[0]).tipo);
        		string var = mapa[(yyvsp[-5]).label].temporario;
        		string tamanhoVetor = mapaTemporario[var].tamanho;
        		if ((yyvsp[0]).tipo == CHARS) {
	        		(yyval).traducao = (yyvsp[-3]).traducao + (yyvsp[0]).traducao + "\tif(" + (yyvsp[-3]).label + " < 0 || " + (yyvsp[-3]).label + " >= " + tamanhoVetor + ") exit(1);\n" +
						'\t' + var + '[' + (yyvsp[-3]).label + "] = (char*) realloc(" + var + '[' + (yyvsp[-3]).label + "], " + mapaTemporario[(yyvsp[0]).label].tamanho + ");\n" + 
						"\tstrcpy(" + var + '[' + (yyvsp[-3]).label + ']' + ", " + (yyvsp[0]).label + ");\n";
        		} else {
        			if (vetorParaElemento(mapa[(yyvsp[-5]).label].tipo) != (yyvsp[0]).tipo) {
        				string varCast = "tmp" + proximaVariavelTemporaria();
        				mapaTemporario[varCast] = { .id = varCast, .tipo = (yyvsp[0]).tipo == INT ? FLUT32 : INT };
        				(yyval).traducao = (yyvsp[-3]).traducao + (yyvsp[0]).traducao + "\tif (" + (yyvsp[-3]).label + " < 0 || " + (yyvsp[-3]).label + " >= " + tamanhoVetor + ") exit(1);\n" +
        					'\t' + varCast + " = (" + decideTipo(mapaTemporario[varCast].tipo) + ") " + (yyvsp[0]).label + ";\n" +
	        				'\t' + var + '[' + (yyvsp[-3]).label + "] = " + varCast + ";\n";
        			} else {
        				(yyval).traducao = (yyvsp[-3]).traducao + (yyvsp[0]).traducao + "\tif (" + (yyvsp[-3]).label + " < 0 || " + (yyvsp[-3]).label + " >= " + tamanhoVetor + ") exit(1);\n" +
	        			'\t' + var + '[' + (yyvsp[-3]).label + "] = " + (yyvsp[0]).label + ";\n";
        			}
        		}
			}
#line 2667 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1038 "sintatica.y" /* yacc.c:1646  */
    {
				defineTiposCompativeis((yyvsp[-6]).tipo, INT);
				defineTiposCompativeis((yyvsp[-3]).tipo, INT);
				mapV mapa = buscaMapa((yyvsp[-8]).label);
				defineTiposCompativeis(vetorParaElemento(mapa[(yyvsp[-8]).label].tipo), (yyvsp[0]).tipo);
				string var = mapa[(yyvsp[-8]).label].temporario;
        		string linhas = mapaTemporario[var].tamanho;
        		string colunas = mapaTemporario[var].coluna;
        		string posicao = "tmp" + proximaVariavelTemporaria();
        		mapaTemporario[posicao] = {.id = posicao, .tipo = INT};
        		(yyval).traducao = (yyvsp[-6]).traducao + (yyvsp[-3]).traducao + "\tif(" + linhas + " < 0 || " + (yyvsp[-6]).label + " >= " + linhas + ") exit(1);\n" +
						"\tif(" + colunas + " < 0 || " + (yyvsp[-3]).label + " >= " + colunas + ") exit(1);\n" +
						'\t' + posicao + " = " + (yyvsp[-6]).label + " * " + colunas + ";\n" +
						'\t' + posicao + " = " + posicao + " + " + (yyvsp[-3]).label + ";\n" + (yyvsp[0]).traducao;
        		if (mapa[(yyvsp[-8]).label].tipo == FLUT32_MATRIZ || mapa[(yyvsp[-8]).label].tipo == INT_MATRIZ) {
					if (vetorParaElemento(mapa[(yyvsp[-8]).label].tipo) != (yyvsp[0]).tipo) {
						string varCast = "tmp" + proximaVariavelTemporaria();
        				mapaTemporario[varCast] = { .id = varCast, .tipo = (yyvsp[-3]).tipo == INT ? FLUT32 : INT };
        				(yyval).traducao += '\t' + varCast + " = (" + decideTipo(mapaTemporario[varCast].tipo) + ") " + (yyvsp[0]).label + ";\n" +
        					'\t' + var + '[' + posicao + "] = " + varCast + ";\n";
					} else {
						(yyval).traducao += '\t' + var + '[' + posicao + "] = " + (yyvsp[0]).label + ";\n";
					}
				} else {
					string tamanhoString = "tmp" + proximaVariavelTemporaria();
					mapaTemporario[tamanhoString] = {.id = tamanhoString, .tipo = INT};
					string rotulo = proximoRotulo();
					(yyval).traducao += '\t' + var + '[' + posicao + "] = " + (yyvsp[0]).label + ";\n";
				}
			}
#line 2702 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1071 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval) = processaOpAritmetica((yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]));
			}
#line 2710 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1076 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval) = processaOpAritmetica((yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]));
			}
#line 2718 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 1081 "sintatica.y" /* yacc.c:1646  */
    {
				defineTiposCompativeis((yyvsp[-2]).tipo, (yyvsp[0]).tipo);
				(yyval) = processaOpAritmetica((yyvsp[-2]), (yyvsp[-1]), (yyvsp[0]));
			}
#line 2727 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 1087 "sintatica.y" /* yacc.c:1646  */
    {
				defineTiposCompativeis((yyvsp[-2]).tipo, (yyvsp[0]).tipo);
				(yyval).tipo = (yyvsp[-2]).tipo;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				string tamanhoString = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo, .tamanho = tamanhoString };
				mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao + 
					'\t' + tamanhoString + " = " + mapaTemporario[(yyvsp[-2]).label].tamanho + " + " +
				 	mapaTemporario[(yyvsp[0]).label].tamanho + ";\n" + '\t' + tamanhoString + " = " + tamanhoString + " + 1;\n" +
					'\t' + (yyval).label + " = (char*) malloc(" + tamanhoString + ");\n" + 
					"\tstrcat(" + (yyval).label + ", " + (yyvsp[-2]).label + ");\n" + "\tstrcat(" + (yyval).label + ", " + (yyvsp[0]).label + ");\n";
			}
#line 2745 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1101 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = (yyvsp[0]).tipo;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				string tamanhoString = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo, .tamanho = tamanhoString };
				mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };
				(yyval).traducao = '\t' + tamanhoString + " = " + to_string((yyvsp[0]).traducao.length() - 2) + " + 1;\n" +
					'\t' + (yyval).label + " = (char*) malloc(" + tamanhoString + ");\n" + 
					"\tstrcat(" + (yyval).label + ", " + (yyvsp[0]).traducao + ");\n";
			}
#line 2760 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1113 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = (yyvsp[0]).tipo;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = '\t' + (yyval).label + " = " + (yyvsp[0]).traducao + ";\n";
			}
#line 2771 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 1121 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = (yyvsp[0]).tipo;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = (yyvsp[0]).traducao + '\t' + (yyval).label + " = -" + (yyvsp[0]).label + ";\n";
			}
#line 2782 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1129 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = (yyvsp[-1]).tipo;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n";
			}
#line 2793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 1136 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[0]).label);
        		string var = mapa[(yyvsp[0]).label].temporario;
        		(yyval).label = mapaTemporario[var].id;
        		(yyval).tipo = mapaTemporario[var].tipo;
        		(yyval).traducao = "";
			}
#line 2805 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1144 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = (yyvsp[0]).tipo;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = '\t' + (yyval).label + " = " + (yyvsp[0]).traducao + ";\n";	
			}
#line 2816 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1151 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = (yyvsp[0]).tipo;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = '\t' + (yyval).label + " = " + decideValorBooleano((yyvsp[0]).traducao) + ";\n";	
			}
#line 2827 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1159 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				(yyval).tipo = (yyvsp[-1]).tipo;
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = (yyvsp[0]).traducao + '\t' + (yyval).label + " = ( " + decideTipo((yyvsp[-1]).tipo) + " ) " + (yyvsp[0]).label + ";\n";
			}
#line 2838 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1166 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval) = (yyvsp[0]);
			}
#line 2846 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1170 "sintatica.y" /* yacc.c:1646  */
    {
				defineTiposCompativeis((yyvsp[-1]).tipo, INT);
				mapV mapa = buscaMapa((yyvsp[-3]).label);
        		string var = mapa[(yyvsp[-3]).label].temporario;
        		string tamanhoVetor = mapaTemporario[var].tamanho;
        		string varElemento = "tmp" + proximaVariavelTemporaria();
        		mapaTemporario[varElemento] = {.id = varElemento, .tipo = vetorParaElemento(mapaTemporario[var].tipo)};
        		(yyval).label = varElemento;
        		(yyval).tipo = mapaTemporario[varElemento].tipo;
        		if ((yyval).tipo == CHARS) {
	        		string tamanhoString = "tmp" + proximaVariavelTemporaria();
	        		mapaTemporario[tamanhoString] = {.id = tamanhoString, .tipo = INT};
	        		mapaTemporario[varElemento].tamanho = tamanhoString;
	        		(yyval).traducao = (yyvsp[-1]).traducao + '\t' + tamanhoString + " = strlen(" + var + '[' + (yyvsp[-1]).label + ']' + ") + 1;\n" +
						'\t' + (yyval).label + " = (char*) malloc(" + tamanhoString + ");\n" + 
						"\tstrcat(" + (yyval).label + ", " + var + '[' + (yyvsp[-1]).label + ']' +");\n";
        		} else {
        			(yyval).traducao = (yyvsp[-1]).traducao + "\tif (" + (yyvsp[-1]).label + " < 0 || " + (yyvsp[-1]).label + " >= " + tamanhoVetor + ") exit(1);\n" +
	        			'\t' + varElemento + " = " + var + '[' + (yyvsp[-1]).label + "];\n";
        		}
			}
#line 2872 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1192 "sintatica.y" /* yacc.c:1646  */
    {
				defineTiposCompativeis((yyvsp[-3]).tipo, INT);
				defineTiposCompativeis((yyvsp[-1]).tipo, INT);
				mapV mapa = buscaMapa((yyvsp[-5]).label);
				if (mapa[(yyvsp[-5]).label].tipo != INT_VETOR && mapa[(yyvsp[-5]).label].tipo != FLUT32_VETOR 
					&& mapa[(yyvsp[-5]).label].tipo != CHARS_VETOR && mapa[(yyvsp[-5]).label].tipo != CHARS)	yyerror("Slice permitido apenas em vetores");
        		string var = mapa[(yyvsp[-5]).label].temporario;
        		string tamanhoVetor = mapaTemporario[var].tamanho;
        		string slice = "tmp" + proximaVariavelTemporaria();
        		string tamanhoSlice = "tmp" + proximaVariavelTemporaria();
        		mapaTemporario[tamanhoSlice] = { .id = tamanhoSlice, .tipo = INT };
        		mapaTemporario[slice] = { .id = slice, .tipo = mapaTemporario[var].tipo, .tamanho = tamanhoSlice };
        		(yyval).label = slice;
        		(yyval).tipo = mapaTemporario[var].tipo;
        		(yyval).traducao = (yyvsp[-3]).traducao + (yyvsp[-1]).traducao + "\tif(" + (yyvsp[-3]).label + " < 0 || " + (yyvsp[-3]).label + " >= " + tamanhoVetor + ") exit(1);\n" +
        			"\tif(" + (yyvsp[-1]).label + " < 0 || " + (yyvsp[-1]).label + " >= " + tamanhoVetor + ") exit(1);\n" +
        			'\t' + tamanhoSlice + " = " + (yyvsp[-1]).label + " - " + (yyvsp[-3]).label + ";\n" +
        			"\tif(" + tamanhoSlice + " <= 0) exit(1);\n" +
        			'\t' + tamanhoSlice + " = " + tamanhoSlice + " + 1;\n" +
        			'\t' + slice + " = " + var + " + " + (yyvsp[-3]).label + ";\n";
			}
#line 2898 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1214 "sintatica.y" /* yacc.c:1646  */
    {
				defineTiposCompativeis((yyvsp[-4]).tipo, INT);
				defineTiposCompativeis((yyvsp[-1]).tipo, INT);
				mapV mapa = buscaMapa((yyvsp[-6]).label);
				string var = mapa[(yyvsp[-6]).label].temporario;
        		string linhas = mapaTemporario[var].tamanho;
        		string colunas = mapaTemporario[var].coluna;
        		string posicao = "tmp" + proximaVariavelTemporaria();
        		mapaTemporario[posicao] = {.id = posicao, .tipo = INT};
        		string varElemento = "tmp" + proximaVariavelTemporaria();
        		mapaTemporario[varElemento] = {.id = varElemento, .tipo = vetorParaElemento(mapaTemporario[var].tipo)};
        		(yyval).label = varElemento;
        		(yyval).tipo = mapaTemporario[varElemento].tipo;
        		(yyval).traducao = (yyvsp[-4]).traducao + (yyvsp[-1]).traducao + "\tif(" + linhas + " < 0 || " + (yyvsp[-4]).label + " >= " + linhas + ") exit(1);\n" +
						"\tif(" + colunas + " < 0 || " + (yyvsp[-1]).label + " >= " + colunas + ") exit(1);\n" +
						'\t' + posicao + " = " + (yyvsp[-4]).label + " * " + colunas + ";\n" +
						'\t' + posicao + " = " + posicao + " + " + (yyvsp[-1]).label + ";\n";
				if ((yyval).tipo == CHARS) {
					string tamanhoString = "tmp" + proximaVariavelTemporaria();
	        		mapaTemporario[tamanhoString] = {.id = tamanhoString, .tipo = INT};
	        		mapaTemporario[varElemento].tamanho = tamanhoString;
	        		(yyval).traducao += '\t' + tamanhoString + " = strlen(" + var + '[' + posicao + ']' + ") + 1;\n" +
						'\t' + (yyval).label + " = (char*) malloc(" + tamanhoString + ");\n" + 
						"\tstrcat(" + (yyval).label + ", " + var + '[' + posicao + ']' +");\n";
				} else {
					(yyval).traducao += '\t' + varElemento + " = " + var + '[' + posicao + "];\n";
				}
			}
#line 2931 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1246 "sintatica.y" /* yacc.c:1646  */
    {
				defineTiposCompativeis((yyvsp[-2]).tipo, (yyvsp[0]).tipo);
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				(yyval).tipo = BOOL;
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				if ((yyvsp[-2]).tipo != CHARS) {
					(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao + '\t' +
						(yyval).label + " = " + (yyvsp[-2]).label + decideOperadorRelacional((yyvsp[-1]).traducao) + (yyvsp[0]).label + ";\n";
				} else if ((yyvsp[-2]).tipo == CHARS) {
					(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao + '\t' +
						(yyval).label + " = strcmp(" + (yyvsp[-2]).label + ", " + (yyvsp[0]).label + ");\n" +
						'\t' + (yyval).label + " = " + (yyval).label + decideOperadorRelacional((yyvsp[-1]).traducao) + " 0;\n";
				}
			}
#line 2950 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 1261 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				(yyval).tipo = BOOL;
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = (yyvsp[-2]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-2]).label + 
				"==" + decideValorBooleano((yyvsp[0]).traducao) + ";\n";
			}
#line 2962 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1271 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				(yyval).tipo = BOOL;
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-2]).label + 
				decideOperadorRelacional((yyvsp[-1]).traducao) + (yyvsp[0]).label + ";\n";				
			}
#line 2974 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 1280 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				(yyval).tipo = BOOL;
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n";
			}
#line 2985 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1291 "sintatica.y" /* yacc.c:1646  */
    {         
 				string varIf = "tmp" + proximaVariavelTemporaria();
 				string rotuloFim = proximoRotulo();
				mapaTemporario[varIf] = { .id = varIf, .tipo = BOOL };
				(yyval).traducao = (yyvsp[-3]).traducao +	"\t" + varIf + " = !" + (yyvsp[-3]).label + ";\n" +
				"\tif (" + varIf + ") goto " + rotuloFim + ";\n" +
				(yyvsp[0]).traducao + '\t' + rotuloFim + ":\n";
				pilhaContexto.pop();
			}
#line 2999 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1301 "sintatica.y" /* yacc.c:1646  */
    {
				string varIf = "tmp" + proximaVariavelTemporaria();
    		    string rotuloFimIf = proximoRotulo();
    		    string rotuloFimIfElse = proximoRotulo();
				mapaTemporario[varIf] = { .id = varIf, .tipo = BOOL };
				(yyval).traducao = (yyvsp[-5]).traducao +	"\t" + varIf + " = !" + (yyvsp[-5]).label + ";\n" +
				"\tif (" + varIf + ") goto " + rotuloFimIf + ";\n" + 
				(yyvsp[-2]).traducao + "\tgoto " + rotuloFimIfElse + ";\n" +
				'\t' + rotuloFimIf + ":\n" + (yyvsp[0]).traducao + '\t' + rotuloFimIfElse + ":\n";
				pilhaContexto.pop();
			}
#line 3015 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1313 "sintatica.y" /* yacc.c:1646  */
    {
    		    string varIf = "tmp" + proximaVariavelTemporaria();
    		    string rotuloFimIf = proximoRotulo();
    		    string rotuloFimIfElse = proximoRotulo();
				mapaTemporario[varIf] = { .id = varIf, .tipo = BOOL };
				(yyval).traducao = (yyvsp[-6]).traducao +	"\t" + varIf + " = !" + (yyvsp[-6]).label + ";\n" +
				"\tif (" + varIf + ") goto " + rotuloFimIf + ";\n" + 
				(yyvsp[-3]).traducao + "\tgoto " + rotuloFimIfElse + ";\n" +
				'\t' + rotuloFimIf + ":\n" + (yyvsp[0]).traducao + '\t' + rotuloFimIfElse + ":\n";
				pilhaContexto.pop();
			}
#line 3031 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1326 "sintatica.y" /* yacc.c:1646  */
    {
       			pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
      			.rotuloInicio = "", .rotuloFim = ""});
      		}
#line 3040 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1332 "sintatica.y" /* yacc.c:1646  */
    {
				pilhaContexto.pop();
				pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
      				.rotuloInicio = "", .rotuloFim = ""});	
			}
#line 3050 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1340 "sintatica.y" /* yacc.c:1646  */
    {
				string varWhile = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[varWhile] = { .id = varWhile, .tipo = BOOL };
				(yyval).traducao = '\t' + pilhaContexto.top().rotuloInicio + ":\n" +	(yyvsp[-3]).traducao +
				"\t" + varWhile + " = !" + (yyvsp[-3]).label + ";\n" +
				"\tif (" + varWhile + ") goto " + pilhaContexto.top().rotuloFim + ";\n" +
				(yyvsp[0]).traducao + "\tgoto " + pilhaContexto.top().rotuloInicio + ";\n" +
				'\t' + pilhaContexto.top().rotuloFim + ":\n";
				pilhaContexto.pop();
			}
#line 3065 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1352 "sintatica.y" /* yacc.c:1646  */
    {
				string varDoWhile = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[varDoWhile] = { .id = varDoWhile, .tipo = BOOL };
				(yyval).traducao = '\t' + pilhaContexto.top().rotuloInicio + ":\n" + (yyvsp[-4]).traducao +
				(yyvsp[-1]).traducao + '\t' + varDoWhile + " = !" + (yyvsp[-1]).label + ";\n" + 
				"\tif (" + varDoWhile + ") goto " + pilhaContexto.top().rotuloFim + ";\n" +
				"\tgoto " + pilhaContexto.top().rotuloInicio + ";\n" +
				'\t' + pilhaContexto.top().rotuloFim + ":\n";
				pilhaContexto.pop();
			}
#line 3080 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1364 "sintatica.y" /* yacc.c:1646  */
    {
				string varFor = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[varFor] = { .id = varFor, .tipo = BOOL };
				(yyval).traducao = (yyvsp[-6]).traducao + '\t' + pilhaContexto.top().rotuloInicio + ":\n" +
				(yyvsp[-4]).traducao + '\t' + varFor + " = !" + (yyvsp[-4]).label + ";\n" +
				"\tif (" + varFor + ") goto " + pilhaContexto.top().rotuloFim + ";\n" +
				(yyvsp[0]).traducao + (yyvsp[-2]).traducao + "\tgoto " + pilhaContexto.top().rotuloInicio + ";\n"+
				'\t' + pilhaContexto.top().rotuloFim + ":\n";
				pilhaContexto.pop();
			}
#line 3095 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1377 "sintatica.y" /* yacc.c:1646  */
    {
					(yyval).traducao = (yyvsp[0]).traducao;
				}
#line 3103 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1382 "sintatica.y" /* yacc.c:1646  */
    {
					(yyval).traducao = (yyvsp[0]).traducao;
				}
#line 3111 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1387 "sintatica.y" /* yacc.c:1646  */
    {
  				(yyval).traducao = (yyvsp[-4]).traducao + (yyvsp[-1]).traducao + '\t' + pilhaContexto.top().rotuloFim + ":\n";
  				pilhaContexto.pop();
			}
#line 3120 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1394 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[0]).label); 
				string var = "tmp" + proximaVariavelTemporaria();
  				mapaTemporario[var] = {.id = var, .tipo = mapa[(yyvsp[0]).label].tipo};
  				(yyval).traducao = '\t' + var + " = " + mapa[(yyvsp[0]).label].temporario + ";\n";
				pilhaContexto.push({ .quebravel = true, .mapaVariaveis = controiMapaVariaveis(),
            	    .rotuloInicio = var, .rotuloFim = proximoRotulo()}); // rotulo inicio esta sendo igual a variavel temporaria que se refere a variavel de entrada do switch
			}
#line 3133 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1405 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3141 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1410 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 3149 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1414 "sintatica.y" /* yacc.c:1646  */
    {
        		(yyval).traducao = (yyvsp[0]).traducao;
      		}
#line 3157 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1421 "sintatica.y" /* yacc.c:1646  */
    {  			
				defineTiposCompativeis((yyvsp[-2]).tipo, mapaTemporario[pilhaContexto.top().rotuloInicio].tipo);
  				string rotulo = proximoRotulo();
  				(yyval).traducao = (yyvsp[-2]).traducao + "\tif (" + (yyvsp[-2]).label + " != " + pilhaContexto.top().rotuloInicio + ") goto " + rotulo + ";\n"
            	+ (yyvsp[0]).traducao + '\t' + rotulo + ":\n";  
			}
#line 3168 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1430 "sintatica.y" /* yacc.c:1646  */
    {
                (yyval).tipo = (yyvsp[0]).tipo;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = '\t' + (yyval).label + " = " + (yyvsp[0]).traducao + ";\n";
			}
#line 3179 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1438 "sintatica.y" /* yacc.c:1646  */
    {
              	(yyval).tipo = (yyvsp[0]).tipo;
                (yyval).label = "tmp" + proximaVariavelTemporaria();
                mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
                (yyval).traducao = '\t' + (yyval).label + " = " + (yyvsp[0]).traducao + ";\n";
            }
#line 3190 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1446 "sintatica.y" /* yacc.c:1646  */
    {
                mapV mapa = buscaMapa((yyvsp[0]).label);
        		string var = mapa[(yyvsp[0]).label].temporario;
        		(yyval).label = mapaTemporario[var].id;
        		(yyval).tipo = mapaTemporario[var].tipo;
        		(yyval).traducao = "";
            }
#line 3202 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1456 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 3210 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1462 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + "\tcout << " + (yyvsp[-1]).label + " << endl;\n";
			}
#line 3218 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1468 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[-1]).label);
				if (mapa[(yyvsp[-1]).label].tipo == CHARS) {
					string rotuloInicio = proximoRotulo();
					string rotuloFim = proximoRotulo();
					string tamanho = mapaTemporario[mapa[(yyvsp[-1]).label].temporario].tamanho;
					string id = mapaTemporario[mapa[(yyvsp[-1]).label].temporario].id;
					string tmpChar = "tmp" + proximaVariavelTemporaria();
					mapaTemporario[tmpChar] = { .id = tmpChar, .tipo = CHAR };

					(yyval).traducao = "\tif (" + tamanho + " > 0)" + " free(" + id + ");\n" +
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

				} else if(mapa[(yyvsp[-1]).label].tipo == CHAR) {
					(yyval).traducao = "\tscanf(\"%c\", &" + mapa[(yyvsp[-1]).label].temporario + ");\n";
				} else if(mapa[(yyvsp[-1]).label].tipo == FLUT32) {
					(yyval).traducao = "\tscanf(\"%f\", &" + mapa[(yyvsp[-1]).label].temporario + ");\n";
				} else if(mapa[(yyvsp[-1]).label].tipo == INT) {
					(yyval).traducao = "\tscanf(\"%d\", &" + mapa[(yyvsp[-1]).label].temporario + ");\n";
				}

				(yyval).traducao += "\tsetbuf(stdin, NULL);\n";
			}
#line 3257 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1505 "sintatica.y" /* yacc.c:1646  */
    {
				pilhaContexto.push({ .quebravel = true, .mapaVariaveis = controiMapaVariaveis(),
					.rotuloInicio = proximoRotulo(), .rotuloFim = proximoRotulo()});
			}
#line 3266 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1511 "sintatica.y" /* yacc.c:1646  */
    {
				contextoBloco cb = controlarLaco();
				(yyval).traducao = "\tgoto " + cb.rotuloFim + ";\n";
			}
#line 3275 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1517 "sintatica.y" /* yacc.c:1646  */
    {
				contextoBloco cb = controlarLaco();
				(yyval).traducao = "\tgoto " + cb.rotuloInicio + ";\n";
			}
#line 3284 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1522 "sintatica.y" /* yacc.c:1646  */
    {
				contextoBloco cb = controlarLacoALL();
				(yyval).traducao = "\tgoto " + cb.rotuloFim + ";\n";
			}
#line 3293 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1530 "sintatica.y" /* yacc.c:1646  */
    {
					processaArgumentoFuncao((yyvsp[-3]).label, (yyvsp[-1]).tipo);
					mapaTemporario[pilhaContexto.top().rotuloInicio].funcao.traducao = decideTipo((yyvsp[-1]).tipo) + 
						pilhaContexto.top().mapaVariaveis[(yyvsp[-3]).label].temporario + (yyvsp[0]).traducao;
				}
#line 3303 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1535 "sintatica.y" /* yacc.c:1646  */
    { (yyval).traducao = ""; }
#line 3309 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1539 "sintatica.y" /* yacc.c:1646  */
    {
					processaArgumentoFuncao((yyvsp[-3]).label, (yyvsp[-1]).tipo);
					(yyval).traducao = ", " + decideTipo((yyvsp[-1]).tipo) + 
						pilhaContexto.top().mapaVariaveis[(yyvsp[-3]).label].temporario + (yyvsp[0]).traducao;
				}
#line 3319 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1544 "sintatica.y" /* yacc.c:1646  */
    { (yyval).traducao = ""; }
#line 3325 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1548 "sintatica.y" /* yacc.c:1646  */
    {
					string nomeFuncao = pilhaContexto.top().rotuloInicio;
					string tamanhoStringParametros = "";
					list<parametrosFuncao> a = mapaTemporario[nomeFuncao].funcao.parametros;
					for (list<parametrosFuncao>::iterator it = a.begin(); it != a.end(); ++it) {
						if (it->tipo == CHARS) {
							tamanhoStringParametros += '\t' + mapaTemporario[it->id].tamanho + " = strlen(" + it->id + ");\n"; 
						}
					}
					mapaTemporario[nomeFuncao].funcao.traducao = decideTipo((yyvsp[-1]).tipo) + nomeFuncao + 
						'(' + mapaTemporario[nomeFuncao].funcao.traducao + ')' +
						"{\n" + declaraVariaveisTemporariasFuncao() + tamanhoStringParametros + (yyvsp[0]).traducao + "}\n";
					mapaTemporarioCopia[nomeFuncao] = mapaTemporario[nomeFuncao];
					mapaTemporario = mapaTemporarioCopia;
					mapaTemporarioCopia.clear();
					pilhaContexto.pop();
					(yyval).traducao = "";
				}
#line 3348 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1569 "sintatica.y" /* yacc.c:1646  */
    {
					mapaTemporario[pilhaContexto.top().rotuloInicio].funcao.retorno = (yyvsp[0]).tipo;
				}
#line 3356 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1575 "sintatica.y" /* yacc.c:1646  */
    {
					if (!pilhaContexto.top().isGlobal) yyerror("Uma função só pode ser declarada no escopo global.");
					verificaVariavelJaDeclarada((yyvsp[0]).label);
					string var = "tmp" + proximaVariavelTemporaria();
					pilhaContexto.top().mapaVariaveis[(yyvsp[0]).label] = { .id = (yyvsp[0]).label, .tipo = FUNCAO, var};
					mapaTemporario[var] = { .id = var, .tipo = FUNCAO, .tamanho = "", .isParametro = false,
						.funcao = {.retorno = "", .traducao = "", .parametros = list<parametrosFuncao>()}
					};
					mapaTemporarioCopia = mapaTemporario;
					pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
						.rotuloInicio = var, .rotuloFim = "", .funcional = true});
				}
#line 3373 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1590 "sintatica.y" /* yacc.c:1646  */
    {
					(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
					parametrosAuxiliar.push_front({.id = (yyvsp[-1]).label, .tipo = (yyvsp[-1]).tipo});
				}
#line 3382 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1594 "sintatica.y" /* yacc.c:1646  */
    { (yyval).traducao = ""; }
#line 3388 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1598 "sintatica.y" /* yacc.c:1646  */
    {
					(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
					parametrosAuxiliar.push_front({.id = (yyvsp[-1]).label, .tipo = (yyvsp[-1]).tipo});
				}
#line 3397 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 1602 "sintatica.y" /* yacc.c:1646  */
    { (yyval).traducao = ""; }
#line 3403 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 1606 "sintatica.y" /* yacc.c:1646  */
    {
					mapV mapa = buscaMapa((yyvsp[-3]).label);
					if (mapa[(yyvsp[-3]).label].tipo == FUNCAO) {
						string traducao = "", traducaoCast;
						string temporario = mapa[(yyvsp[-3]).label].temporario;
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
						(yyval).label = "tmp" + proximaVariavelTemporaria();
						(yyval).tipo = mapaTemporario[temporario].funcao.retorno;
						mapaTemporario[(yyval).label] = {.id = (yyval).label, .tipo = (yyval).tipo};
						if ((yyval).tipo == CHARS) {
							string tamanhoString = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[tamanhoString] = {.id = tamanhoString, .tipo = INT};
							mapaTemporario[(yyval).label].tamanho = tamanhoString;
							(yyval).traducao = (yyvsp[-1]).traducao + traducaoCast + '\t' + (yyval).label + " = " + temporario + '(' + traducao + ");\n" +
											'\t' + tamanhoString + " = strlen(" + (yyval).label + ");\n";
						} else {
							(yyval).traducao = (yyvsp[-1]).traducao + traducaoCast + '\t' + (yyval).label + " = " + temporario + '(' + traducao + ");\n";
						}
						parametrosAuxiliar.clear();
					} else {
						yyerror("A variável " + (yyvsp[-3]).label + " não é uma função.");
					} 	
				}
#line 3457 "y.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 1658 "sintatica.y" /* yacc.c:1646  */
    {
					contextoBloco cb = controlarRetorno();		
					string retorno = mapaTemporario[cb.rotuloInicio].funcao.retorno;
					defineTiposCompativeis(retorno, (yyvsp[0]).tipo);
					if (retorno != (yyvsp[0]).tipo) {
						string var = "tmp" + proximaVariavelTemporaria();
						mapaTemporario[var] = {.id = var, .tipo = retorno};
						(yyval).traducao = (yyvsp[0]).traducao + '\t' + var + " = (" + decideTipo(retorno) + ") " + (yyvsp[0]).label + ";\n"
							"\treturn " + var + ";\n";
					} else {
						(yyval).traducao = (yyvsp[0]).traducao + "\treturn " + (yyvsp[0]).label + ";\n";
					}
				}
#line 3475 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 1674 "sintatica.y" /* yacc.c:1646  */
    {
					verificaVariavelJaDeclarada((yyvsp[-3]).label);
					(yyval).label = "tmp" + proximaVariavelTemporaria();
	        		(yyval).tipo = (yyvsp[-1]).tipo;
	  				pilhaContexto.top().mapaVariaveis[(yyvsp[-3]).label] = { .id = (yyvsp[-3]).label, .tipo = (yyval).tipo, (yyval).label };
	  				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
	  				if ((yyvsp[-1]).tipo == CHARS) {
	  					string tamanhoString = "tmp" + proximaVariavelTemporaria();
	  					mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };
	  					mapaTemporario[(yyval).label].tamanho = tamanhoString;
	  					(yyval).traducao = (yyvsp[-1]).traducao + '\t' + tamanhoString + " = " + mapaTemporario[(yyvsp[-1]).label].tamanho + ";\n" +
	  								'\t' + (yyval).label + " = (char*) malloc(" + tamanhoString + ");\n" +
	  								"\tstrcat(" + (yyval).label + ", " + (yyvsp[-1]).label + ");\n"; 
	  				} else {
	  					(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
	  				}
				}
#line 3497 "y.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 1694 "sintatica.y" /* yacc.c:1646  */
    {
					verificaVariavelJaDeclarada((yyvsp[-3]).label);
					(yyval).label = "tmp" + proximaVariavelTemporaria();
	        		(yyval).tipo = (yyvsp[-1]).tipo;
	  				pilhaContexto.top().mapaVariaveis[(yyvsp[-3]).label] = { .id = (yyvsp[-3]).label, .tipo = (yyval).tipo, (yyval).label };
	  				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
					if ((yyvsp[-1]).tipo == CHARS) {
	  					string tamanhoString = "tmp" + proximaVariavelTemporaria();
	  					mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };
	  					mapaTemporario[(yyval).label].tamanho = tamanhoString;
	  					(yyval).traducao = (yyvsp[-1]).traducao + '\t' + tamanhoString + " = " + mapaTemporario[(yyvsp[-1]).label].tamanho + ";\n" +
	  								'\t' + (yyval).label + " = (char*) malloc(" + tamanhoString + ");\n" +
	  								"\tstrcat(" + (yyval).label + ", " + (yyvsp[-1]).label + ");\n"; 
	  				} else {
	  					(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
	  				}
				}
#line 3519 "y.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 1711 "sintatica.y" /* yacc.c:1646  */
    { (yyval).traducao = ""; (yyval).tipo = ""; (yyval).label = ""; }
#line 3525 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3529 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1713 "sintatica.y" /* yacc.c:1906  */


#include "lex.yy.c"

int yyparse();

int main( int argc, char* argv[] )
{
	yyparse();

	return 0;
}

void yyerror( string MSG )
{
	cout << "Erro na linha " << linha << ": " << MSG << endl;
	exit (0);
}				
