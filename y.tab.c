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
#include <stack>

#define YYSTYPE atributos
#define INT "0"
#define FLUT32 "1"
#define BOOL "2"
#define CHAR "3"
#define CHARS "4"
#define SIZE_STR 10

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
	string tamanho;
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
} contextoBloco;

static stack<contextoBloco> pilhaContexto;

static mapT mapaTemporario;
static mapV mapaDeclarado;

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
    	s += '\t' + decideTipo(it->second.tipo) + ' ' + it->second.id + ";\n";
	}
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


#line 239 "y.tab.c" /* yacc.c:339  */

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
    TK_RELACIONAL = 276,
    TK_LOGICO = 277,
    TK_FIM = 278,
    TK_ERROR = 279,
    TK_IF = 280,
    TK_WHILE = 281,
    TK_BREAK = 282,
    TK_CONTINUE = 283,
    TK_DO = 284,
    TK_FOR = 285,
    TK_OP_ABREV = 286,
    TK_OP_1 = 287,
    TK_ELSE = 288,
    TK_SWITCH = 289,
    TK_CASE = 290,
    TK_DEFAULT = 291,
    TK_PRINT = 292,
    TK_SCAN = 293,
    TK_CAST = 294
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
#define TK_RELACIONAL 276
#define TK_LOGICO 277
#define TK_FIM 278
#define TK_ERROR 279
#define TK_IF 280
#define TK_WHILE 281
#define TK_BREAK 282
#define TK_CONTINUE 283
#define TK_DO 284
#define TK_FOR 285
#define TK_OP_ABREV 286
#define TK_OP_1 287
#define TK_ELSE 288
#define TK_SWITCH 289
#define TK_CASE 290
#define TK_DEFAULT 291
#define TK_PRINT 292
#define TK_SCAN 293
#define TK_CAST 294

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

#line 368 "y.tab.c" /* yacc.c:358  */

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
#define YYLAST   185

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  39
/* YYNRULES -- Number of rules.  */
#define YYNRULES  90
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  184

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

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
      45,    46,     2,     2,    44,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    47,    42,
       2,    43,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    40,     2,    41,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   194,   194,   203,   208,   214,   219,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   236,   257,   280,
     297,   317,   340,   345,   348,   370,   373,   378,   383,   388,
     391,   415,   418,   423,   428,   431,   449,   452,   457,   459,
     479,   482,   487,   490,   512,   515,   521,   527,   555,   563,
     574,   598,   622,   636,   648,   656,   664,   671,   680,   690,
     705,   715,   724,   732,   735,   745,   757,   771,   777,   784,
     796,   808,   821,   826,   831,   838,   849,   854,   858,   865,
     874,   882,   890,   900,   906,   911,   919,   929,   967,   972,
     978
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
  "TK_OCT", "TK_RELACIONAL", "TK_LOGICO", "TK_FIM", "TK_ERROR", "TK_IF",
  "TK_WHILE", "TK_BREAK", "TK_CONTINUE", "TK_DO", "TK_FOR", "TK_OP_ABREV",
  "TK_OP_1", "TK_ELSE", "TK_SWITCH", "TK_CASE", "TK_DEFAULT", "TK_PRINT",
  "TK_SCAN", "TK_CAST", "'{'", "'}'", "';'", "'='", "','", "'('", "')'",
  "':'", "$accept", "S", "PILHA_GLOBAL", "BLOCO", "COMANDOS", "COMANDO",
  "DECLARACAO", "DECLARACAO_VF32", "DECLARACAO_F32", "DECLARACAO_VBOOL",
  "DECLARACAO_BOOL", "DECLARACAO_VCHAR", "DECLARACAO_CHAR",
  "DECLARACAO_VSTRING", "DECLARACAO_STRING", "DECLARACAO_VINT",
  "DECLARACAO_INT", "ATRIBUICAO", "E", "R", "L", "SE", "BLOCO_IF",
  "BLOCO_ELSE", "ENQUANTO", "REPITA", "PARA", "PARA_ATRIBUICAO",
  "PARA_INCREMENTO", "ESCOLHA", "BLOCO_CASE", "CASOS", "CASO",
  "VALOR_CASE", "DEFAULT", "PRINT", "SCAN", "BLOCO_ITERACAO",
  "LOOP_CONTROLE", YY_NULLPTR
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
     123,   125,    59,    61,    44,    40,    41,    58
};
# endif

#define YYPACT_NINF -77

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-77)))

#define YYTABLE_NINF -28

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -77,    26,   -18,   -77,    99,   -77,    21,    49,    61,    70,
      76,    84,    52,    53,   -77,   -77,   -77,   -77,    57,    58,
      69,    74,    99,    68,    75,   -77,   -77,    77,   -77,   -77,
      78,    81,    88,   -77,     5,    73,    89,    92,    95,    96,
      34,    34,   -18,    86,   110,    22,   113,   -77,   -77,   -77,
     -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,
      48,    48,    34,    65,   101,   119,    48,    98,    48,   100,
      13,   102,    48,   103,    10,   104,    65,   -17,   -10,   117,
     137,   -77,   105,   106,   107,   108,   109,    48,   134,   -77,
      55,    -8,    48,    48,    48,    48,   153,    34,    79,   150,
     -77,    79,   151,   -77,   -77,    47,   119,   152,   -77,    79,
     154,   -77,   -77,   -77,   155,   -77,   -77,   -77,   116,   -77,
     122,   125,   -77,   -77,   -77,   -77,    31,   -77,   -77,   134,
     -77,    46,    79,   -77,   -77,    73,    89,    92,    95,    96,
     -18,   -18,    34,    34,    39,    98,   100,   102,   103,   104,
     133,   -77,     9,     1,    80,   120,   127,    39,   -77,   -77,
     -77,   -77,   -77,   -77,   144,   -77,   137,   -77,   -77,   -77,
     123,    99,   -77,   -77,   -77,   -18,   -77,   126,    99,   -77,
     -77,   -18,   -77,   -77
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     1,     6,     2,     0,     0,     0,     0,
       0,     0,     0,     0,    89,    90,    88,    88,     0,     0,
       0,     0,     6,     0,     0,     9,    10,     0,    13,    14,
       0,     0,     0,    49,     0,    42,    23,    29,    38,    34,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     7,
       8,    12,    15,    16,    11,    54,    45,    46,    53,    57,
       0,     0,     0,    47,    63,    48,     0,    44,     0,    25,
       0,    31,     0,    40,     0,    36,     0,     0,     0,     0,
       0,    75,     0,     0,     0,     0,     0,     0,    55,    58,
       0,     0,     0,     0,     0,     0,     0,     0,    41,     0,
      21,    22,     0,    17,    26,    57,    28,     0,    18,    37,
       0,    20,    32,    33,     0,    19,    67,    88,     0,    72,
       0,     0,    85,    86,    84,    87,     0,    56,    62,    50,
      51,    52,    59,    60,    61,    42,    23,    29,    38,    34,
       0,     0,     0,     0,     0,    44,    25,    31,    40,    36,
      64,    69,     0,     0,     0,     0,     0,    78,    77,    43,
      24,    30,    39,    35,    68,    70,     0,    80,    81,    82,
       0,     6,    74,    76,    65,     0,    73,     0,     6,    83,
      66,     0,    79,    71
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -77,   -77,   -77,   -41,   -22,   -77,   -77,    35,    27,    37,
      28,    38,    29,    41,    32,    50,    36,   -76,   -27,   -77,
     -38,    12,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,
     -77,    25,   -77,   -77,   -77,   -77,   -77,   -11,   -77
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     5,    21,    22,    23,    69,   103,    71,
     108,    75,   115,    73,   111,    67,   100,    24,    76,    64,
      65,    25,   140,   175,    26,    27,    28,   120,   177,    29,
      82,   156,   157,   170,   158,    30,    31,    42,    32
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,    79,    77,    78,   119,    97,    43,    63,    55,    56,
      57,    58,    97,    59,    97,   112,    55,   104,   113,    58,
      60,   105,     4,    97,    91,    83,     3,    84,    60,   116,
      85,    97,   106,    88,    89,    90,   117,    55,   128,    98,
      58,   101,    59,   166,    61,   109,    92,    93,    94,    60,
      62,    55,    61,    33,    58,   165,    59,    35,    62,   134,
     126,    92,    93,    60,    34,   129,   130,   131,   132,    36,
      92,    93,    94,    61,   154,   155,    95,   127,    37,    62,
      92,    93,    94,   167,    38,   168,    95,    61,   169,   -27,
     176,   -27,    39,    87,    92,    93,    94,    40,    41,   150,
     151,   127,    44,    45,   152,   153,   141,     6,     7,     8,
      49,     9,    10,    11,    46,    47,    66,    50,    81,    51,
      52,    86,    96,    53,    12,    13,    14,    15,    16,    17,
      54,    80,    68,    18,   180,    70,    19,    20,    72,    74,
     183,    97,    99,   118,   102,     6,   107,   110,   114,   179,
      93,   121,   122,   123,   124,   125,   182,   133,   135,   136,
     137,   142,   138,   139,   143,   144,   164,   171,   172,    12,
     178,   146,   181,   160,   147,   161,   174,   149,   163,   148,
     162,   159,   173,     0,     0,   145
};

static const yytype_int16 yycheck[] =
{
      22,    42,    40,    41,    80,    22,    17,    34,     3,     4,
       5,     6,    22,     8,    22,     5,     3,     4,     8,     6,
      15,     8,    40,    22,    62,     3,     0,     5,    15,    46,
       8,    22,    70,    60,    61,    62,    46,     3,    46,    66,
       6,    68,     8,    42,    39,    72,    15,    16,    17,    15,
      45,     3,    39,    32,     6,    46,     8,     8,    45,    97,
      87,    15,    16,    15,    43,    92,    93,    94,    95,     8,
      15,    16,    17,    39,    35,    36,    21,    46,     8,    45,
      15,    16,    17,     3,     8,     5,    21,    39,     8,    42,
     166,    44,     8,    45,    15,    16,    17,    45,    45,   140,
     141,    46,    45,    45,   142,   143,   117,     8,     9,    10,
      42,    12,    13,    14,    45,    41,    43,    42,     8,    42,
      42,     8,    21,    42,    25,    26,    27,    28,    29,    30,
      42,    45,    43,    34,   175,    43,    37,    38,    43,    43,
     181,    22,    44,    26,    44,     8,    44,    44,    44,   171,
      16,    46,    46,    46,    46,    46,   178,     4,     8,     8,
       8,    45,     8,     8,    42,    40,    33,    47,    41,    25,
      47,   136,    46,   146,   137,   147,   164,   139,   149,   138,
     148,   145,   157,    -1,    -1,   135
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    49,    50,     0,    40,    51,     8,     9,    10,    12,
      13,    14,    25,    26,    27,    28,    29,    30,    34,    37,
      38,    52,    53,    54,    65,    69,    72,    73,    74,    77,
      83,    84,    86,    32,    43,     8,     8,     8,     8,     8,
      45,    45,    85,    85,    45,    45,    45,    41,    52,    42,
      42,    42,    42,    42,    42,     3,     4,     5,     6,     8,
      15,    39,    45,    66,    67,    68,    43,    63,    43,    55,
      43,    57,    43,    61,    43,    59,    66,    68,    68,    51,
      45,     8,    78,     3,     5,     8,     8,    45,    66,    66,
      66,    68,    15,    16,    17,    21,    21,    22,    66,    44,
      64,    66,    44,    56,     4,     8,    68,    44,    58,    66,
      44,    62,     5,     8,    44,    60,    46,    46,    26,    65,
      75,    46,    46,    46,    46,    46,    66,    46,    46,    66,
      66,    66,    66,     4,    68,     8,     8,     8,     8,     8,
      70,    85,    45,    42,    40,    63,    55,    57,    61,    59,
      51,    51,    68,    68,    35,    36,    79,    80,    82,    64,
      56,    58,    62,    60,    33,    46,    42,     3,     5,     8,
      81,    47,    41,    79,    69,    71,    65,    76,    47,    52,
      51,    46,    52,    51
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    50,    51,    52,    52,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    54,    54,    54,
      54,    54,    55,    55,    56,    56,    57,    57,    57,    57,
      58,    58,    59,    59,    59,    60,    60,    61,    61,    62,
      62,    63,    63,    64,    64,    65,    65,    65,    65,    65,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    67,
      67,    68,    68,    68,    69,    69,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    79,    79,    80,
      81,    81,    81,    82,    83,    83,    83,    84,    85,    86,
      86
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     3,     2,     0,     2,     2,     1,
       1,     2,     2,     1,     1,     2,     2,     4,     4,     4,
       4,     4,     2,     0,     4,     0,     2,     2,     2,     0,
       4,     0,     2,     2,     0,     4,     0,     2,     0,     4,
       0,     2,     0,     4,     0,     3,     3,     3,     3,     2,
       3,     3,     3,     1,     1,     2,     3,     1,     2,     3,
       3,     3,     3,     1,     6,     8,     9,     0,     0,     6,
       7,    10,     1,     1,     7,     1,     2,     1,     1,     4,
       1,     1,     1,     3,     4,     4,     4,     4,     0,     1,
       1
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
#line 195 "sintatica.y" /* yacc.c:1646  */
    {
				cout << "/*Compilador FAEN*/" << endl;
				cout << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\n#include<stdlib.h>\n#define SIZE_STR 10\nusing namespace std;" << endl;
				cout << "int main(void)\n{\n" << declaraVariaveisTemporarias() + (yyvsp[0]).traducao + liberarStrings() << "\treturn 0;\n}" << endl; 
			}
#line 1583 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 203 "sintatica.y" /* yacc.c:1646  */
    {
				pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
					.rotuloInicio = proximoRotulo(), .rotuloFim = proximoRotulo()});
			}
#line 1592 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 209 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao;
			}
#line 1600 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 215 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 1608 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 219 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 1616 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 237 "sintatica.y" /* yacc.c:1646  */
    {
				if ((yyvsp[-1]).tipo != "") defineTiposCompativeis((yyvsp[-3]).tipo, (yyvsp[-1]).tipo);
				verificaVariavelJaDeclarada((yyvsp[-2]).label);
				(yyval).label = "tmp" + proximaVariavelTemporaria();
	        	(yyval).tipo = (yyvsp[-3]).tipo;
	  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
	  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				if ((yyvsp[-1]).tipo != "") {
					if ((yyvsp[-1]).tipo == INT) {
						string varCast = "tmp" + proximaVariavelTemporaria();
						mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
						(yyval).traducao = (yyvsp[-1]).traducao + '\t' + varCast + " = (float) " + (yyvsp[-1]).label + ";\n" +
						'\t' + (yyval).label + " = " + varCast + ";\n" + (yyvsp[0]).traducao;
					} else {
						(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
					}
				} else {
					(yyval).traducao = (yyvsp[0]).traducao;
				}
			}
#line 1641 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 258 "sintatica.y" /* yacc.c:1646  */
    {
				verificaVariavelJaDeclarada((yyvsp[-2]).label);
				(yyval).label = "tmp" + proximaVariavelTemporaria();
	        	(yyval).tipo = BOOL;
	  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
	  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				if ((yyvsp[-1]).tipo != "" && (yyvsp[-1]).label == "") {
					(yyval).traducao = (yyvsp[0]).traducao + '\t' + (yyval).label + " = " + decideValorBooleano((yyvsp[-1]).traducao) + ";\n";
				} 
				else if ((yyvsp[-1]).label != "" && (yyvsp[-1]).tipo == "") {
					mapV mapa = buscaMapa((yyvsp[-1]).label);
					defineTiposCompativeis(BOOL, pilhaContexto.top().mapaVariaveis[(yyvsp[-1]).label].tipo);
					(yyval).traducao = (yyvsp[0]).traducao + '\t' + (yyval).label + " = " + mapaTemporario[mapa[(yyvsp[-1]).label].temporario].id + ";\n";
				}
				else if ((yyvsp[-1]).label != "" && (yyvsp[-1]).tipo != ""){
					(yyval).traducao = (yyvsp[0]).traducao + (yyvsp[-1]).traducao + 
					'\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n";
				}
				else {
					(yyval).traducao = (yyvsp[0]).traducao;
				}
			}
#line 1668 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 281 "sintatica.y" /* yacc.c:1646  */
    {
      			verificaVariavelJaDeclarada((yyvsp[-2]).label);
				(yyval).label = "tmp" + proximaVariavelTemporaria();
	        	(yyval).tipo = CHAR;
	  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
	  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				if ((yyvsp[-1]).tipo != "") { // caracter puro com tipo
					(yyval).traducao = (yyvsp[0]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).traducao + ";\n";
				} else if ((yyvsp[-1]).label != "") { // variavel
					mapV mapa = buscaMapa((yyvsp[-1]).label);
					defineTiposCompativeis(CHAR, mapa[(yyvsp[-1]).label].tipo);
					(yyval).traducao = (yyvsp[0]).traducao + '\t' + (yyval).label + " = " + mapaTemporario[mapa[(yyvsp[-1]).label].temporario].id + ";\n";
				} else {
					(yyval).traducao = (yyvsp[0]).traducao;
				}
      		}
#line 1689 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 298 "sintatica.y" /* yacc.c:1646  */
    {
      			if ((yyvsp[-1]).tipo != "") defineTiposCompativeis((yyvsp[-3]).tipo, (yyvsp[-1]).tipo);
      			verificaVariavelJaDeclarada((yyvsp[-2]).label);
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				string tamanhoString = "tmp" + proximaVariavelTemporaria();
				(yyval).tipo = (yyvsp[-3]).tipo;
				pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo, .tamanho = tamanhoString };
				mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };

				if ((yyvsp[-1]).tipo != "") {
					(yyval).traducao = (yyvsp[-1]).traducao + 
						'\t' + tamanhoString + " = " + mapaTemporario[(yyvsp[-1]).label].tamanho + ";\n" +
						'\t' + (yyval).label + " = (char*) malloc(" + tamanhoString + ");\n" + 
						"\tstrcat(" + (yyval).label + ", " + (yyvsp[-1]).label + ");\n" + (yyvsp[0]).traducao;						
				} else {
					(yyval).traducao = '\t' + tamanhoString + " = 0;\n" + (yyvsp[0]).traducao;
				}
      		}
#line 1713 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 318 "sintatica.y" /* yacc.c:1646  */
    {
      			if ((yyvsp[-1]).tipo != "") defineTiposCompativeis((yyvsp[-3]).tipo, (yyvsp[-1]).tipo);
      			verificaVariavelJaDeclarada((yyvsp[-2]).label);
        		(yyval).label = "tmp" + proximaVariavelTemporaria();
	        	(yyval).tipo = (yyvsp[-3]).tipo;
	  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
	  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				if ((yyvsp[-1]).tipo != "") {
					if ((yyvsp[-1]).tipo == FLUT32) {
						string varCast = "tmp" + proximaVariavelTemporaria();
						mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
						(yyval).traducao = (yyvsp[-1]).traducao + '\t' + varCast + " = (int) " + (yyvsp[-1]).label + ";\n" +
						'\t' + (yyval).label + " = " + varCast + ";\n" + (yyvsp[0]).traducao;
					} else {
						(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
					}
				} else {
					(yyval).traducao = (yyvsp[0]).traducao;	
				}
      		}
#line 1738 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 341 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 }
#line 1746 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 345 "sintatica.y" /* yacc.c:1646  */
    { (yyval).traducao = ""; (yyval).tipo = ""; (yyval).label = ""; }
#line 1752 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 349 "sintatica.y" /* yacc.c:1646  */
    {
					if ((yyvsp[-1]).tipo != "") defineTiposCompativeis(FLUT32, (yyvsp[-1]).tipo);
					verificaVariavelJaDeclarada((yyvsp[-2]).label);
					(yyval).label = "tmp" + proximaVariavelTemporaria();
		        	(yyval).tipo = FLUT32;
		  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
		  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
					if ((yyvsp[-1]).tipo != "") {
						if ((yyvsp[-1]).tipo == INT) {
							string varCast = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
							(yyval).traducao = (yyvsp[-1]).traducao + '\t' + varCast + " = (float) " + (yyvsp[-1]).label + ";\n" +
							'\t' + (yyval).label + " = " + varCast + ";\n" + (yyvsp[0]).traducao;
						} else {
							(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
						}
					} else {
						(yyval).traducao = (yyvsp[0]).traducao;
					}
				}
#line 1777 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 370 "sintatica.y" /* yacc.c:1646  */
    {	(yyval).traducao = "";	}
#line 1783 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 374 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 }
#line 1791 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 379 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 }
#line 1799 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 384 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 }
#line 1807 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 388 "sintatica.y" /* yacc.c:1646  */
    { (yyval).traducao = ""; (yyval).tipo = ""; (yyval).label = ""; }
#line 1813 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 392 "sintatica.y" /* yacc.c:1646  */
    {
					verificaVariavelJaDeclarada((yyvsp[-2]).label);
					(yyval).label = "tmp" + proximaVariavelTemporaria();
		        	(yyval).tipo = BOOL;
		  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
		  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
					if ((yyvsp[-1]).tipo != "" && (yyvsp[-1]).label == "") {
						(yyval).traducao = (yyvsp[0]).traducao + '\t' + (yyval).label + " = " + decideValorBooleano((yyvsp[-1]).traducao) + ";\n";
					} 
					else if ((yyvsp[-1]).label != "" && (yyvsp[-1]).tipo == ""){
						mapV mapa = buscaMapa((yyvsp[-1]).label);
						defineTiposCompativeis(BOOL,mapa[(yyvsp[-1]).label].tipo);
						(yyval).traducao = (yyvsp[0]).traducao + '\t' + (yyval).label + " = " + mapaTemporario[mapa[(yyvsp[-1]).label].temporario].id + ";\n";
					}
					else if ((yyvsp[-1]).label != "" && (yyvsp[-1]).tipo != ""){
						(yyval).traducao = (yyvsp[0]).traducao + (yyvsp[-1]).traducao + 
						'\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n";
					}
					else {
						(yyval).traducao = (yyvsp[0]).traducao;
					}
				}
#line 1840 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 415 "sintatica.y" /* yacc.c:1646  */
    {	(yyval).traducao = "";	}
#line 1846 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 419 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 }
#line 1854 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 424 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 }
#line 1862 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 428 "sintatica.y" /* yacc.c:1646  */
    { (yyval).traducao = ""; (yyval).tipo = ""; (yyval).label = ""; }
#line 1868 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 432 "sintatica.y" /* yacc.c:1646  */
    {
					verificaVariavelJaDeclarada((yyvsp[-2]).label);
					(yyval).label = "tmp" + proximaVariavelTemporaria();
		        	(yyval).tipo = CHAR;
		  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
		  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
					if ((yyvsp[-1]).tipo != "") { // caracter puro com tipo
						(yyval).traducao = (yyvsp[0]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).traducao + ";\n";
					} else if ((yyvsp[-1]).label != "") { // variavel
						mapV mapa = buscaMapa((yyvsp[-1]).label);
						defineTiposCompativeis(CHAR, mapa[(yyvsp[-1]).label].tipo);
						(yyval).traducao = (yyvsp[0]).traducao + '\t' + (yyval).label + " = " + mapaTemporario[mapa[(yyvsp[-1]).label].temporario].id + ";\n";
					} else {
						(yyval).traducao = (yyvsp[0]).traducao;
					}
				}
#line 1889 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 449 "sintatica.y" /* yacc.c:1646  */
    {	(yyval).traducao = "";	}
#line 1895 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 453 "sintatica.y" /* yacc.c:1646  */
    {
						(yyval) = (yyvsp[0]);
					}
#line 1903 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 457 "sintatica.y" /* yacc.c:1646  */
    {	(yyval).traducao = ""; (yyval).tipo = ""; (yyval).label = "";	}
#line 1909 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 460 "sintatica.y" /* yacc.c:1646  */
    {
						if ((yyvsp[-1]).tipo != "") defineTiposCompativeis(CHARS, (yyvsp[-1]).tipo);
		      			verificaVariavelJaDeclarada((yyvsp[-2]).label);
						(yyval).label = "tmp" + proximaVariavelTemporaria();
						string tamanhoString = "tmp" + proximaVariavelTemporaria();
						(yyval).tipo = CHARS;
						pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
						mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo, .tamanho = tamanhoString };
						mapaTemporario[tamanhoString] = { .id = tamanhoString, .tipo = INT };

						if ((yyvsp[-1]).tipo != "") {
							(yyval).traducao = (yyvsp[-1]).traducao + '\t' + tamanhoString + " = " + mapaTemporario[(yyvsp[-1]).label].tamanho + ";\n" +
								'\t' + (yyval).label + " = (char*) malloc(" + tamanhoString + ");\n" + 
								"\tstrcat(" + (yyval).label + ", " + (yyvsp[-1]).label + ");\n" + (yyvsp[0]).traducao;						
						} else {
							(yyval).traducao = '\t' + tamanhoString + " = 0;\n" + (yyvsp[0]).traducao;
						}
					}
#line 1932 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 479 "sintatica.y" /* yacc.c:1646  */
    {	(yyval).traducao = "";	}
#line 1938 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 483 "sintatica.y" /* yacc.c:1646  */
    {
				 	(yyval) = (yyvsp[0]);
				 }
#line 1946 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 487 "sintatica.y" /* yacc.c:1646  */
    { (yyval).traducao = ""; (yyval).tipo = ""; (yyval).label = ""; }
#line 1952 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 491 "sintatica.y" /* yacc.c:1646  */
    {
					if ((yyvsp[-1]).tipo != "") defineTiposCompativeis(INT, (yyvsp[-1]).tipo);
					verificaVariavelJaDeclarada((yyvsp[-2]).label);
					(yyval).label = "tmp" + proximaVariavelTemporaria();
		        	(yyval).tipo = INT;
		  			pilhaContexto.top().mapaVariaveis[(yyvsp[-2]).label] = { .id = (yyvsp[-2]).label, .tipo = (yyval).tipo, (yyval).label };
		  			mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
					if ((yyvsp[-1]).tipo != "") {
						if ((yyvsp[-1]).tipo == FLUT32) {
							string varCast = "tmp" + proximaVariavelTemporaria();
							mapaTemporario[varCast] = { .id = varCast, .tipo = INT };
							(yyval).traducao = (yyvsp[-1]).traducao + '\t' + varCast + " = (int) " + (yyvsp[-1]).label + ";\n" +
							'\t' + (yyval).label + " = " + varCast + ";\n" + (yyvsp[0]).traducao;
						} else {
							(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n" + (yyvsp[0]).traducao;
						}
					} else {
						(yyval).traducao = (yyvsp[0]).traducao;
					}
				}
#line 1977 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 512 "sintatica.y" /* yacc.c:1646  */
    {	(yyval).traducao = "";	}
#line 1983 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 516 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[-2]).label);
				(yyval).traducao = '\t' + mapaTemporario[mapa[(yyvsp[-2]).label].temporario].id + " = " + (yyvsp[0]).traducao + ";\n";
			}
#line 1992 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 522 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[-2]).label);
				(yyval).traducao = '\t' + mapaTemporario[mapa[(yyvsp[-2]).label].temporario].id + " = " + (yyvsp[0]).traducao + ";\n";
			}
#line 2001 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 528 "sintatica.y" /* yacc.c:1646  */
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
					} else {
						(yyval).traducao = (yyvsp[0]).traducao + '\t' + mapaTemporario[mapa[(yyvsp[-2]).label].temporario].id + 
							" = " + (yyvsp[0]).label + ";\n";	
					}
				} else {
					(yyval).traducao = (yyvsp[0]).traducao + '\t' + mapaTemporario[mapa[(yyvsp[-2]).label].temporario].id + 
					" = " + (yyvsp[0]).label + ";\n";
				}
			}
#line 2032 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 556 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[-2]).label);
				defineTiposCompativeis((yyvsp[-2]).tipo, (yyvsp[0]).tipo);
				(yyval).traducao = (yyvsp[0]).traducao + '\t' + mapaTemporario[mapa[(yyvsp[-2]).label].temporario].id + 
				" = " + (yyvsp[0]).label + ";\n";	
			}
#line 2043 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 564 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[-1]).label);
				defineTiposCompativeis(mapa[(yyvsp[-1]).label].tipo, INT);
				string mais1 = mapaTemporario[mapa[(yyvsp[-1]).label].temporario].tipo == INT ? " 1;\n" : " 1.0;\n";
				string op = (yyvsp[0]).traducao == "++" ? " +" : " -";
				(yyval).traducao = '\t' + mapaTemporario[mapa[(yyvsp[-1]).label].temporario].id +
				" = " + mapaTemporario[mapa[(yyvsp[-1]).label].temporario].id + op + mais1;
			}
#line 2056 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 575 "sintatica.y" /* yacc.c:1646  */
    {
				defineTiposCompativeis((yyvsp[-2]).tipo, (yyvsp[0]).tipo);
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao;
				if ((yyvsp[-2]).tipo != (yyvsp[0]).tipo) {
					string varCast = "tmp" + proximaVariavelTemporaria();
					(yyval).tipo = FLUT32;
					mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
					mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = FLUT32 };
					if ((yyvsp[-2]).tipo == INT && (yyvsp[0]).tipo == FLUT32) {
						(yyval).traducao += '\t' + varCast + " = (float) " + (yyvsp[-2]).label + ";\n" +
						'\t' + (yyval).label + " = " + varCast + (yyvsp[-1]).traducao + (yyvsp[0]).label + ";\n";
					} else if ((yyvsp[-2]).tipo == FLUT32 && (yyvsp[0]).tipo == INT) {
						(yyval).traducao += '\t' + varCast + " = (float) " + (yyvsp[0]).label + ";\n" +
						'\t' + (yyval).label + " = " + varCast + (yyvsp[-1]).traducao + (yyvsp[-2]).label + ";\n";
					}
				} else {
					(yyval).tipo = (yyvsp[-2]).tipo;
					mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyvsp[-2]).tipo };
					(yyval).traducao += '\t' + (yyval).label + " = " + (yyvsp[-2]).label + (yyvsp[-1]).traducao + (yyvsp[0]).label + ";\n";
				}
			}
#line 2083 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 599 "sintatica.y" /* yacc.c:1646  */
    {
				defineTiposCompativeis((yyvsp[-2]).tipo, (yyvsp[0]).tipo);
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao;
				if ((yyvsp[-2]).tipo != (yyvsp[0]).tipo) {
					string varCast = "tmp" + proximaVariavelTemporaria();
					(yyval).tipo = FLUT32;
					mapaTemporario[varCast] = { .id = varCast, .tipo = FLUT32 };
					mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = FLUT32 };
					if ((yyvsp[-2]).tipo == INT && (yyvsp[0]).tipo == FLUT32) {
						(yyval).traducao += '\t' + varCast + " = (float) " + (yyvsp[-2]).label + ";\n" +
						'\t' + (yyval).label + " = " + varCast + (yyvsp[-1]).traducao + (yyvsp[0]).label + ";\n";
					} else if ((yyvsp[-2]).tipo == FLUT32 && (yyvsp[0]).tipo == INT) {
						(yyval).traducao += '\t' + varCast + " = (float) " + (yyvsp[0]).label + ";\n" +
						'\t' + (yyval).label + " = " + varCast + (yyvsp[-1]).traducao + (yyvsp[-2]).label + ";\n";
					}
				} else {
					(yyval).tipo = (yyvsp[-2]).tipo;
					mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyvsp[-2]).tipo };
					(yyval).traducao += '\t' + (yyval).label + " = " + (yyvsp[-2]).label + (yyvsp[-1]).traducao + (yyvsp[0]).label + ";\n";
				}
			}
#line 2110 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 623 "sintatica.y" /* yacc.c:1646  */
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
#line 2128 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 637 "sintatica.y" /* yacc.c:1646  */
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
#line 2143 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 649 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = (yyvsp[0]).tipo;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = '\t' + (yyval).label + " = " + (yyvsp[0]).traducao + ";\n";
			}
#line 2154 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 657 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = (yyvsp[0]).tipo;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = (yyvsp[0]).traducao + '\t' + (yyval).label + " = -" + (yyvsp[0]).label + ";\n";
			}
#line 2165 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 665 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = (yyvsp[-1]).tipo;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n";
			}
#line 2176 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 672 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[0]).label);
        		string var = mapa[(yyvsp[0]).label].temporario;
        		(yyval).label = mapaTemporario[var].id;
        		(yyval).tipo = mapaTemporario[var].tipo;
        		(yyval).traducao = "";
			}
#line 2188 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 681 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				(yyval).tipo = (yyvsp[-1]).tipo;
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = (yyvsp[0]).traducao + '\t' + (yyval).label + " = ( " + decideTipo((yyvsp[-1]).tipo) + " ) " + (yyvsp[0]).label + ";\n";
			}
#line 2199 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 691 "sintatica.y" /* yacc.c:1646  */
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
#line 2218 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 706 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				(yyval).tipo = BOOL;
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = (yyvsp[-2]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-2]).label + 
				"==" + decideValorBooleano((yyvsp[0]).traducao) + ";\n";
			}
#line 2230 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 716 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				(yyval).tipo = BOOL;
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-2]).label + 
				decideOperadorRelacional((yyvsp[-1]).traducao) + (yyvsp[0]).label + ";\n";				
			}
#line 2242 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 725 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				(yyval).tipo = BOOL;
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = (yyvsp[-1]).traducao + '\t' + (yyval).label + " = " + (yyvsp[-1]).label + ";\n";
			}
#line 2253 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 736 "sintatica.y" /* yacc.c:1646  */
    {         
 				string varIf = "tmp" + proximaVariavelTemporaria();
 				string rotuloFim = proximoRotulo();
				mapaTemporario[varIf] = { .id = varIf, .tipo = BOOL };
				(yyval).traducao = (yyvsp[-3]).traducao +	"\t" + varIf + " = !" + (yyvsp[-3]).label + ";\n" +
				"\tif (" + varIf + ") goto " + rotuloFim + ";\n" +
				(yyvsp[0]).traducao + '\t' + rotuloFim + ":\n";
				pilhaContexto.pop();
			}
#line 2267 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 746 "sintatica.y" /* yacc.c:1646  */
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
#line 2283 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 758 "sintatica.y" /* yacc.c:1646  */
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
#line 2299 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 771 "sintatica.y" /* yacc.c:1646  */
    {
       			pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
      			.rotuloInicio = "", .rotuloFim = ""});
      		}
#line 2308 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 777 "sintatica.y" /* yacc.c:1646  */
    {
				pilhaContexto.pop();
				pilhaContexto.push({ .quebravel = false, .mapaVariaveis = controiMapaVariaveis(),
      				.rotuloInicio = "", .rotuloFim = ""});	
			}
#line 2318 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 785 "sintatica.y" /* yacc.c:1646  */
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
#line 2333 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 797 "sintatica.y" /* yacc.c:1646  */
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
#line 2348 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 809 "sintatica.y" /* yacc.c:1646  */
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
#line 2363 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 822 "sintatica.y" /* yacc.c:1646  */
    {
					(yyval).traducao = (yyvsp[0]).traducao;
				}
#line 2371 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 827 "sintatica.y" /* yacc.c:1646  */
    {
					(yyval).traducao = (yyvsp[0]).traducao;
				}
#line 2379 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 832 "sintatica.y" /* yacc.c:1646  */
    {
  				(yyval).traducao = (yyvsp[-4]).traducao + (yyvsp[-1]).traducao + '\t' + pilhaContexto.top().rotuloFim + ":\n";
  				pilhaContexto.pop();
			}
#line 2388 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 839 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[0]).label); 
				string var = "tmp" + proximaVariavelTemporaria();
  				mapaTemporario[var] = {.id = var, .tipo = mapa[(yyvsp[0]).label].tipo};
  				(yyval).traducao = '\t' + var + " = " + mapa[(yyvsp[0]).label].temporario + ";\n";
				pilhaContexto.push({ .quebravel = true, .mapaVariaveis = controiMapaVariaveis(),
            	    .rotuloInicio = var, .rotuloFim = proximoRotulo()}); // rotulo inicio esta sendo igual a variavel temporaria que se refere a variavel de entrada do switch
			}
#line 2401 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 850 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 2409 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 855 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2417 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 859 "sintatica.y" /* yacc.c:1646  */
    {
        		(yyval).traducao = (yyvsp[0]).traducao;
      		}
#line 2425 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 866 "sintatica.y" /* yacc.c:1646  */
    {  			
				defineTiposCompativeis((yyvsp[-2]).tipo, mapaTemporario[pilhaContexto.top().rotuloInicio].tipo);
  				string rotulo = proximoRotulo();
  				(yyval).traducao = (yyvsp[-2]).traducao + "\tif (" + (yyvsp[-2]).label + " != " + pilhaContexto.top().rotuloInicio + ") goto " + rotulo + ";\n"
            	+ (yyvsp[0]).traducao + '\t' + rotulo + ":\n";  
			}
#line 2436 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 875 "sintatica.y" /* yacc.c:1646  */
    {
                (yyval).tipo = (yyvsp[0]).tipo;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
				(yyval).traducao = '\t' + (yyval).label + " = " + (yyvsp[0]).traducao + ";\n";
			}
#line 2447 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 883 "sintatica.y" /* yacc.c:1646  */
    {
              	(yyval).tipo = (yyvsp[0]).tipo;
                (yyval).label = "tmp" + proximaVariavelTemporaria();
                mapaTemporario[(yyval).label] = { .id = (yyval).label, .tipo = (yyval).tipo };
                (yyval).traducao = '\t' + (yyval).label + " = " + (yyvsp[0]).traducao + ";\n";
            }
#line 2458 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 891 "sintatica.y" /* yacc.c:1646  */
    {
                mapV mapa = buscaMapa((yyvsp[0]).label);
        		string var = mapa[(yyvsp[0]).label].temporario;
        		(yyval).label = mapaTemporario[var].id;
        		(yyval).tipo = mapaTemporario[var].tipo;
        		(yyval).traducao = "";
            }
#line 2470 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 901 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2478 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 907 "sintatica.y" /* yacc.c:1646  */
    {
				mapV mapa = buscaMapa((yyvsp[-1]).label);
				(yyval).traducao = "\tcout << " + mapa[(yyvsp[-1]).label].temporario + " << endl;\n";
			}
#line 2487 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 912 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = (yyvsp[-1]).tipo;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = {.id = (yyval).label, .tipo = (yyval).tipo};
				(yyval).traducao = '\t' + (yyval).label + " = " + (yyvsp[-1]).traducao + ";\n" +
					"\tcout << " + (yyval).label + " << endl;\n";
			}
#line 2499 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 920 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = CHAR;
				(yyval).label = "tmp" + proximaVariavelTemporaria();
				mapaTemporario[(yyval).label] = {.id = (yyval).label, .tipo = (yyval).tipo};
				(yyval).traducao = '\t' + (yyval).label + " = " + (yyvsp[-1]).traducao + ";\n" +
					"\tcout << " + (yyval).label + " << endl;\n";
			}
#line 2511 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 930 "sintatica.y" /* yacc.c:1646  */
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
#line 2550 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 967 "sintatica.y" /* yacc.c:1646  */
    {
				pilhaContexto.push({ .quebravel = true, .mapaVariaveis = controiMapaVariaveis(),
					.rotuloInicio = proximoRotulo(), .rotuloFim = proximoRotulo()});
			}
#line 2559 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 973 "sintatica.y" /* yacc.c:1646  */
    {
				contextoBloco cb = controlarLaco();
				(yyval).traducao = "\tgoto " + cb.rotuloFim + ";\n";
			}
#line 2568 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 979 "sintatica.y" /* yacc.c:1646  */
    {
				contextoBloco cb = controlarLaco();
				(yyval).traducao = "\tgoto " + cb.rotuloInicio + ";\n";
			}
#line 2577 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2581 "y.tab.c" /* yacc.c:1646  */
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
#line 985 "sintatica.y" /* yacc.c:1906  */


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
