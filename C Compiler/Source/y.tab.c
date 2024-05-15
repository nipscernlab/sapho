/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "variaveis.h"
#include "labels.h"
#include "t2t.h"

#define OFST 1000000

FILE *f_asm, *yyin;

int yylex(void);

int acc_ok = 0; // 0 -> acc vazio (use LOAD)  , 1 -> acc carregado (use PLD)
int prtype = 0; // 0 -> processador fonto fixo, 1 -> processador ponto flutuante
int p_test;     // testa parametros na chamada de funcoes
int fun_id1;    // guarda id da funcao atual durante declaracao
int fun_id2;    // guarda id da funcao atualmente chamada
int mainok = 0; // status da funcao main: 0 -> nao usada, 1 -> declarada, 2 -> chamada no inicio
int ret_ok = 0; // status de retorno da funcao atual;
int acc_id;     // id da ultima variavel carregada no acumulador

int   load      (int id, int is_const, int dtype, int is_array);
void  load_check(int et, int sinal);
void  yyerror   (char  const *s);
void  func_ret  (int id);
void  var_set   (int id, int et, int is_array, int set_type);
int   negacao   (int et);
int   neg         (int et);
int operacoes   (int et1, int et2, char *iop, char *fop, int *op);
void declar_var (int id1, int id2, int id3);
int  declar_par (int   t, int id );
void declar_fun (int id1, int id2);
void declar_ret (int et);
int  fcall      (int id);
void  vcall      (int id);
int   int_oper    (int t1, int t2, char *op, char *code, int fok);
void  array_check(int id, int et);
int   get_npar    (int par);
void  par_check  (int et);
int   exec_in    (int et);
int   exec_abs   (int et);
void  exec_sign1 (int et);
int   exec_sign2 (int et);
int   get_type   (int et);
void  exec_out1  (int et);
void  exec_out2  (int et);


/* Line 189 of yacc.c  */
#line 124 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     FOR = 258,
     PRNAME = 259,
     DIRNAM = 260,
     DATYPE = 261,
     NUBITS = 262,
     NBMANT = 263,
     NBEXPO = 264,
     NDSTAC = 265,
     SDEPTH = 266,
     NUIOIN = 267,
     NUIOOU = 268,
     NUGAIN = 269,
     TYPE = 270,
     INUM = 271,
     FNUM = 272,
     ID = 273,
     STRING = 274,
     IN = 275,
     OUT = 276,
     ABS = 277,
     SIGN = 278,
     RETURN = 279,
     WHILE = 280,
     IF = 281,
     ELSE = 282,
     SHIFTL = 283,
     SHIFTR = 284,
     SSHIFTR = 285,
     GREQU = 286,
     LESEQ = 287,
     EQU = 288,
     DIF = 289,
     LAND = 290,
     LOR = 291,
     NORM = 292
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 203 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\y.tab.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  32
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   885

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  60
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  119
/* YYNRULES -- Number of states.  */
#define YYNSTATES  249

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   292

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    49,     2,     2,     2,    48,    41,     2,
      54,    55,    46,    44,    38,    45,     2,    47,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    51,
      43,    58,    42,     2,    59,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    52,     2,    53,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    56,    39,    57,    50,     2,     2,     2,
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
      35,    36,    37
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    17,    20,
      23,    26,    29,    32,    35,    38,    41,    44,    47,    51,
      59,    61,    65,    67,    72,    73,    74,    85,    86,    95,
      98,   102,   104,   107,   109,   111,   113,   118,   121,   125,
     127,   129,   131,   133,   135,   137,   139,   141,   142,   149,
     150,   156,   160,   161,   163,   167,   168,   177,   182,   187,
     188,   196,   199,   203,   205,   209,   212,   217,   220,   221,
     222,   232,   235,   238,   239,   245,   247,   253,   259,   264,
     268,   272,   277,   282,   283,   292,   294,   297,   299,   301,
     303,   308,   310,   312,   314,   316,   320,   323,   326,   329,
     332,   336,   340,   344,   348,   352,   356,   360,   364,   368,
     372,   376,   380,   384,   388,   392,   396,   400,   404,   408
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      61,     0,    -1,    62,    -1,    61,    62,    -1,    63,    -1,
      64,    -1,    67,    -1,     4,    18,    -1,     5,    19,    -1,
       6,    16,    -1,     7,    16,    -1,     8,    16,    -1,     9,
      16,    -1,    10,    16,    -1,    11,    16,    -1,    12,    16,
      -1,    13,    16,    -1,    14,    16,    -1,    15,    65,    51,
      -1,    15,    18,    52,    16,    53,    19,    51,    -1,    66,
      -1,    65,    38,    66,    -1,    18,    -1,    18,    52,    16,
      53,    -1,    -1,    -1,    15,    18,    54,    68,    71,    55,
      69,    56,    72,    57,    -1,    -1,    15,    18,    54,    55,
      70,    56,    72,    57,    -1,    15,    18,    -1,    71,    38,
      71,    -1,    73,    -1,    72,    73,    -1,    74,    -1,    87,
      -1,    51,    -1,    18,    44,    44,    51,    -1,    56,    57,
      -1,    56,    72,    57,    -1,    99,    -1,   100,    -1,    91,
      -1,    95,    -1,    88,    -1,    81,    -1,    75,    -1,    79,
      -1,    -1,    18,    54,    76,    80,    55,    51,    -1,    -1,
      18,    54,    78,    80,    55,    -1,    24,   103,    51,    -1,
      -1,   103,    -1,    80,    38,   103,    -1,    -1,    21,    54,
     103,    38,    82,   103,    55,    51,    -1,    20,    54,   103,
      55,    -1,    22,    54,   103,    55,    -1,    -1,    23,    54,
     103,    38,    86,   103,    55,    -1,    90,    73,    -1,    89,
      27,    87,    -1,    96,    -1,    89,    27,    74,    -1,    90,
      74,    -1,    26,    54,   103,    55,    -1,    92,    74,    -1,
      -1,    -1,     3,    54,   100,    93,   103,    94,    51,   100,
      55,    -1,    97,    74,    -1,    97,    87,    -1,    -1,    25,
      98,    54,   103,    55,    -1,    64,    -1,    15,    18,    58,
     103,    51,    -1,    15,    18,    59,   103,    51,    -1,    18,
      58,   103,    51,    -1,    18,    44,    44,    -1,    18,    58,
     103,    -1,    18,    59,   103,    51,    -1,    18,    37,   103,
      51,    -1,    -1,    18,    52,   103,    53,    58,   101,   103,
      51,    -1,    16,    -1,    45,   102,    -1,   102,    -1,    17,
      -1,    18,    -1,    18,    52,   103,    53,    -1,    83,    -1,
      84,    -1,    85,    -1,    77,    -1,    54,   103,    55,    -1,
      44,   103,    -1,    45,   103,    -1,    49,   103,    -1,    50,
     103,    -1,   103,    48,   103,    -1,   103,    28,   103,    -1,
     103,    29,   103,    -1,   103,    30,   103,    -1,   103,    41,
     103,    -1,   103,    39,   103,    -1,   103,    40,   103,    -1,
     103,    35,   103,    -1,   103,    36,   103,    -1,   103,    46,
     103,    -1,   103,    47,   103,    -1,   103,    44,   103,    -1,
     103,    45,   103,    -1,   103,    43,   103,    -1,   103,    42,
     103,    -1,   103,    31,   103,    -1,   103,    32,   103,    -1,
     103,    33,   103,    -1,   103,    34,   103,    -1,    37,   103,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    84,    84,    84,    85,    85,    85,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   103,   104,
     106,   107,   109,   110,   114,   115,   114,   117,   117,   121,
     122,   126,   126,   127,   127,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   144,   144,   146,
     146,   148,   150,   151,   152,   156,   156,   158,   159,   160,
     160,   165,   166,   167,   169,   170,   172,   175,   178,   179,
     177,   185,   186,   188,   188,   195,   196,   197,   204,   205,
     206,   207,   208,   209,   209,   215,   216,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FOR", "PRNAME", "DIRNAM", "DATYPE",
  "NUBITS", "NBMANT", "NBEXPO", "NDSTAC", "SDEPTH", "NUIOIN", "NUIOOU",
  "NUGAIN", "TYPE", "INUM", "FNUM", "ID", "STRING", "IN", "OUT", "ABS",
  "SIGN", "RETURN", "WHILE", "IF", "ELSE", "SHIFTL", "SHIFTR", "SSHIFTR",
  "GREQU", "LESEQ", "EQU", "DIF", "LAND", "LOR", "NORM", "','", "'|'",
  "'^'", "'&'", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'",
  "'~'", "';'", "'['", "']'", "'('", "')'", "'{'", "'}'", "'='", "'@'",
  "$accept", "prog", "prog_elements", "direct", "declar", "id_list", "IID",
  "funcao", "$@1", "$@2", "$@3", "par_list", "stmt_list", "stmt",
  "stmt_full", "void_call", "$@4", "func_call", "$@5", "return_call",
  "exp_list", "std_out", "$@6", "std_in", "std_abs", "std_sign", "$@7",
  "stmt_if", "ifelse_stmt", "if_exp_stmt", "if_exp", "for_stmt", "for_exp",
  "@8", "$@9", "while_stmt", "while_if", "while_exp", "$@10",
  "declar_full", "assignment", "@11", "const", "exp", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,    44,   124,
      94,    38,    62,    60,    43,    45,    42,    47,    37,    33,
     126,    59,    91,    93,    40,    41,   123,   125,    61,    64
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    60,    61,    61,    62,    62,    62,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    64,    64,
      65,    65,    66,    66,    68,    69,    67,    70,    67,    71,
      71,    72,    72,    73,    73,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    76,    75,    78,
      77,    79,    80,    80,    80,    82,    81,    83,    84,    86,
      85,    87,    87,    87,    88,    89,    90,    91,    93,    94,
      92,    95,    96,    98,    97,    99,    99,    99,   100,   100,
     100,   100,   100,   101,   100,   102,   102,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     7,
       1,     3,     1,     4,     0,     0,    10,     0,     8,     2,
       3,     1,     2,     1,     1,     1,     4,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     6,     0,
       5,     3,     0,     1,     3,     0,     8,     4,     4,     0,
       7,     2,     3,     1,     3,     2,     4,     2,     0,     0,
       9,     2,     2,     0,     5,     1,     5,     5,     4,     3,
       3,     4,     4,     0,     8,     1,     2,     1,     1,     1,
       4,     1,     1,     1,     1,     3,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     2,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    22,
       0,    20,     1,     3,     0,    24,     0,    18,     0,    27,
       0,    22,    21,    23,     0,     0,     0,     0,     0,     0,
      29,     0,    25,     0,    19,     0,     0,     0,     0,     0,
      73,     0,    35,     0,    75,     0,    31,    33,    45,    46,
      44,    34,    43,     0,     0,    41,     0,    42,    63,     0,
      39,    40,    30,     0,    23,     0,    22,     0,     0,     0,
      47,     0,     0,     0,    85,    88,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    91,    92,    93,
      87,     0,     0,     0,    37,     0,    28,    32,     0,    61,
      33,    67,     0,     0,     0,    71,    72,     0,     0,    68,
       0,     0,     0,    79,     0,    52,    80,     0,     0,     0,
      49,     0,     0,     0,   119,    96,    86,    97,    98,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,    38,    64,    62,     0,    65,     0,     0,
       0,     0,     0,    82,    36,     0,     0,    53,    78,    81,
      55,     0,    52,     0,     0,     0,    95,   101,   102,   103,
     115,   116,   117,   118,   107,   108,   105,   106,   104,   114,
     113,   111,   112,   109,   110,   100,     0,    66,    26,    79,
      69,    76,    77,    83,     0,     0,     0,    90,     0,    57,
      58,    59,    74,     0,     0,    54,    48,     0,    50,     0,
       0,     0,     0,     0,     0,    84,    56,    60,    70
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    13,    14,    15,    64,    30,    31,    17,    40,    83,
      44,    46,    65,    66,    67,    68,   135,   106,   192,    69,
     186,    70,   226,   107,   108,   109,   239,    71,    72,    73,
      74,    75,    76,   180,   233,    77,    78,    79,   112,    80,
      81,   234,   110,   187
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -85
static const yytype_int16 yypact[] =
{
     870,   -15,   -10,    -1,     3,    15,    24,    33,    35,    37,
      39,    44,    51,   858,   -85,   -85,   -85,   -85,   -85,   -85,
     -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -27,
     -28,   -85,   -85,   -85,    62,    26,    61,   -85,    38,   -85,
      75,    59,   -85,    94,    63,    99,   -34,   106,    73,   196,
     -85,    75,   -85,    72,   -85,    74,   108,   201,    76,   212,
     -85,    77,   -85,   120,   -85,   133,   -85,   -85,   -85,   -85,
     -85,   -85,   -85,   100,   196,   -85,   196,   -85,   -85,   196,
     -85,   -85,    91,    78,   -85,   114,   -23,   212,    89,   212,
     -85,   212,   212,   212,   -85,   -85,    -4,    83,    85,    88,
     212,   212,   212,   212,   212,   212,   -85,   -85,   -85,   -85,
     -85,   487,    95,   212,   -85,   167,   -85,   -85,   196,   -85,
     116,   -85,   123,   196,   196,   -85,   -85,   196,   103,   -85,
     212,   212,   511,   101,   435,   212,   535,   559,   655,   212,
     -85,   212,   212,   212,   -85,    -9,   -85,    -9,   -85,   -85,
     239,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     -85,   212,   267,   -85,   -85,   -85,   196,   -85,   180,   109,
     212,   583,   607,   -85,   -85,   102,   -33,   697,   -85,   -85,
     -85,   461,   212,   295,   323,   676,   -85,    41,    41,    41,
      29,    29,   809,   809,   739,   718,   760,   781,   802,    29,
      29,    -9,    -9,   -85,   -85,   -85,   351,   -85,   -85,   -85,
     697,   -85,   -85,   -85,   212,   112,   212,   -85,   -31,   -85,
     -85,   -85,   -85,   113,   212,   697,   -85,   379,   -85,   212,
     114,   631,   115,   407,   117,   -85,   -85,   -85,   -85
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -85,   -85,   155,   -85,    13,   -85,   137,   -85,   -85,   -85,
     -85,   118,   -57,   -63,   -62,   -85,   -85,   -85,   -85,   -85,
     -18,   -85,   -85,   -85,   -85,   -85,   -85,   -71,   -85,   -60,
     -58,   -85,   -85,   -85,   -85,   -85,   -85,   -56,   -85,   -85,
     -84,   -85,    79,   -59
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -66
static const yytype_int16 yytable[] =
{
     111,   129,   117,    18,    51,   224,   115,   224,   126,    19,
      36,   119,   120,    16,   121,    20,   122,   125,   123,    21,
     124,    52,   225,    37,   238,    34,    16,    35,   132,    34,
     134,    22,   136,   137,   138,   130,   131,   167,   168,   169,
      23,   144,   145,   147,   148,   149,   150,   175,   139,    24,
     140,    25,   117,    26,   172,    27,   174,   151,   152,   153,
      28,   177,   125,   122,   122,   123,   123,   124,   124,    29,
     178,   181,   182,   165,   166,   167,   168,   169,    38,    41,
     191,    39,   193,   194,   195,   165,   166,   167,   168,   169,
      45,    43,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,    47,   216,    48,   174,   117,   122,    50,   123,    49,
     124,   220,    53,    55,    54,    84,    86,   118,    85,    51,
      93,   113,   128,   133,   127,    56,    55,   141,    57,   142,
      87,    58,   143,   -65,    59,    60,    61,   179,    56,   171,
     176,    57,   184,   219,    58,    89,   244,    59,    60,    61,
     223,    91,    92,   236,   240,   235,   246,   237,    33,    82,
      55,    62,   248,    42,   228,   241,    63,   114,     0,     0,
     243,   146,    56,    55,    62,    57,     0,     0,    58,    63,
     116,    59,    60,    61,     0,    56,     0,     0,    57,    55,
       0,    58,     0,     0,    59,    60,    61,     0,     0,     0,
       0,    56,     0,     0,    57,     0,     0,    58,    62,     0,
      59,    60,    61,    63,   173,     0,     0,     0,    94,    95,
      96,    62,    97,     0,    98,    99,    63,   218,    87,     0,
       0,     0,     0,     0,     0,    88,     0,    62,     0,   100,
       0,     0,    63,    89,     0,    90,   101,   102,     0,    91,
      92,   103,   104,     0,     0,     0,   105,   151,   152,   153,
     154,   155,   156,   157,   158,   159,     0,     0,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,     0,     0,
       0,     0,     0,     0,   196,   151,   152,   153,   154,   155,
     156,   157,   158,   159,     0,     0,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,     0,     0,     0,     0,
       0,     0,   217,   151,   152,   153,   154,   155,   156,   157,
     158,   159,     0,     0,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,     0,     0,     0,     0,     0,     0,
     229,   151,   152,   153,   154,   155,   156,   157,   158,   159,
       0,     0,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,     0,     0,     0,     0,     0,     0,   230,   151,
     152,   153,   154,   155,   156,   157,   158,   159,     0,     0,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
       0,     0,     0,     0,     0,     0,   232,   151,   152,   153,
     154,   155,   156,   157,   158,   159,     0,     0,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,     0,     0,
       0,     0,     0,     0,   242,   151,   152,   153,   154,   155,
     156,   157,   158,   159,     0,     0,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,     0,     0,     0,     0,
       0,     0,   247,   151,   152,   153,   154,   155,   156,   157,
     158,   159,     0,     0,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,     0,     0,     0,     0,   185,   151,
     152,   153,   154,   155,   156,   157,   158,   159,     0,     0,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
       0,     0,     0,     0,   227,   151,   152,   153,   154,   155,
     156,   157,   158,   159,     0,     0,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,     0,     0,   170,   151,
     152,   153,   154,   155,   156,   157,   158,   159,     0,     0,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
       0,     0,   183,   151,   152,   153,   154,   155,   156,   157,
     158,   159,     0,     0,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,     0,     0,   188,   151,   152,   153,
     154,   155,   156,   157,   158,   159,     0,     0,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,     0,     0,
     189,   151,   152,   153,   154,   155,   156,   157,   158,   159,
       0,     0,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,     0,     0,   221,   151,   152,   153,   154,   155,
     156,   157,   158,   159,     0,     0,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,     0,     0,   222,   151,
     152,   153,   154,   155,   156,   157,   158,   159,     0,     0,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
       0,     0,   245,   151,   152,   153,   154,   155,   156,   157,
     158,   159,     0,   190,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   151,   152,   153,   154,   155,   156,
     157,   158,   159,     0,   231,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   151,   152,   153,   154,   155,
     156,   157,   158,   159,     0,     0,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   151,   152,   153,   154,
     155,   156,   157,   158,     0,     0,     0,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   151,   152,   153,
     154,   155,   156,   157,     0,     0,     0,     0,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   151,   152,
     153,   154,   155,   156,   157,     0,     0,     0,     0,     0,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   151,
     152,   153,   154,   155,   156,   157,     0,     0,     0,     0,
       0,     0,   162,   163,   164,   165,   166,   167,   168,   169,
     151,   152,   153,   154,   155,   156,   157,   151,   152,   153,
     154,   155,     0,     0,   163,   164,   165,   166,   167,   168,
     169,   163,   164,   165,   166,   167,   168,   169,    32,     0,
       0,     0,     1,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,     1,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12
};

static const yytype_int16 yycheck[] =
{
      59,    85,    65,    18,    38,    38,    63,    38,    79,    19,
      38,    74,    74,     0,    76,    16,    76,    79,    76,    16,
      76,    55,    55,    51,    55,    52,    13,    54,    87,    52,
      89,    16,    91,    92,    93,    58,    59,    46,    47,    48,
      16,   100,   101,   102,   103,   104,   105,   118,    52,    16,
      54,    16,   115,    16,   113,    16,   118,    28,    29,    30,
      16,   123,   124,   123,   124,   123,   124,   123,   124,    18,
     127,   130,   131,    44,    45,    46,    47,    48,    16,    18,
     139,    55,   141,   142,   143,    44,    45,    46,    47,    48,
      15,    53,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    52,   171,    19,   176,   178,   176,    18,   176,    56,
     176,   180,    16,     3,    51,    53,    18,    27,    54,    38,
      54,    54,    18,    44,    56,    15,     3,    54,    18,    54,
      37,    21,    54,    27,    24,    25,    26,    44,    15,    54,
      27,    18,    51,    44,    21,    52,   240,    24,    25,    26,
      58,    58,    59,    51,    51,   224,    51,   226,    13,    51,
       3,    51,    55,    36,   192,   234,    56,    57,    -1,    -1,
     239,   102,    15,     3,    51,    18,    -1,    -1,    21,    56,
      57,    24,    25,    26,    -1,    15,    -1,    -1,    18,     3,
      -1,    21,    -1,    -1,    24,    25,    26,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    18,    -1,    -1,    21,    51,    -1,
      24,    25,    26,    56,    57,    -1,    -1,    -1,    16,    17,
      18,    51,    20,    -1,    22,    23,    56,    57,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    51,    -1,    37,
      -1,    -1,    56,    52,    -1,    54,    44,    45,    -1,    58,
      59,    49,    50,    -1,    -1,    -1,    54,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    55,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    -1,    -1,    -1,    53,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      -1,    -1,    -1,    -1,    53,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    -1,    -1,    51,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      -1,    -1,    51,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    -1,    51,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    -1,    -1,
      51,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    -1,    51,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    -1,    -1,    51,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      -1,    -1,    51,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    28,    29,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      28,    29,    30,    31,    32,    33,    34,    28,    29,    30,
      31,    32,    -1,    -1,    42,    43,    44,    45,    46,    47,
      48,    42,    43,    44,    45,    46,    47,    48,     0,    -1,
      -1,    -1,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    61,    62,    63,    64,    67,    18,    19,
      16,    16,    16,    16,    16,    16,    16,    16,    16,    18,
      65,    66,     0,    62,    52,    54,    38,    51,    16,    55,
      68,    18,    66,    53,    70,    15,    71,    52,    19,    56,
      18,    38,    55,    16,    51,     3,    15,    18,    21,    24,
      25,    26,    51,    56,    64,    72,    73,    74,    75,    79,
      81,    87,    88,    89,    90,    91,    92,    95,    96,    97,
      99,   100,    71,    69,    53,    54,    18,    37,    44,    52,
      54,    58,    59,    54,    16,    17,    18,    20,    22,    23,
      37,    44,    45,    49,    50,    54,    77,    83,    84,    85,
     102,   103,    98,    54,    57,    72,    57,    73,    27,    73,
      74,    74,    89,    90,    97,    74,    87,    56,    18,   100,
      58,    59,   103,    44,   103,    76,   103,   103,   103,    52,
      54,    54,    54,    54,   103,   103,   102,   103,   103,   103,
     103,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      51,    54,   103,    57,    74,    87,    27,    74,    72,    44,
      93,   103,   103,    51,    51,    53,    80,   103,    51,    51,
      38,   103,    78,   103,   103,   103,    55,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,    55,    57,    44,
     103,    51,    51,    58,    38,    55,    82,    53,    80,    55,
      55,    38,    55,    94,   101,   103,    51,   103,    55,    86,
      51,   103,    55,   103,   100,    51,    51,    55,    55
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
  *++yyvsp = yylval;

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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 7:

/* Line 1464 of yacc.c  */
#line 89 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "#PRNAME %s\n", v_name[(yyvsp[(2) - (2)])]);;}
    break;

  case 8:

/* Line 1464 of yacc.c  */
#line 90 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "#DIRNAM %s\n", v_name[(yyvsp[(2) - (2)])]);;}
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 91 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "#DATYPE %s\n", v_name[(yyvsp[(2) - (2)])]); prtype = atoi(v_name[(yyvsp[(2) - (2)])]);;}
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 92 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "#NUBITS %s\n", v_name[(yyvsp[(2) - (2)])]);;}
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 93 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "#NBMANT %s\n", v_name[(yyvsp[(2) - (2)])]); nbmant = atoi(v_name[(yyvsp[(2) - (2)])]);;}
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 94 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "#NBEXPO %s\n", v_name[(yyvsp[(2) - (2)])]); nbexpo = atoi(v_name[(yyvsp[(2) - (2)])]);;}
    break;

  case 13:

/* Line 1464 of yacc.c  */
#line 95 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "#NDSTAC %s\n", v_name[(yyvsp[(2) - (2)])]);;}
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 96 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "#SDEPTH %s\n", v_name[(yyvsp[(2) - (2)])]);;}
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 97 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "#NUIOIN %s\n", v_name[(yyvsp[(2) - (2)])]);;}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 98 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "#NUIOOU %s\n", v_name[(yyvsp[(2) - (2)])]);;}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 99 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "#NUGAIN %s\n", v_name[(yyvsp[(2) - (2)])]);;}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 104 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {declar_var((yyvsp[(2) - (7)]),(yyvsp[(4) - (7)]),(yyvsp[(6) - (7)])); v_asgn[(yyvsp[(2) - (7)])] = 1;;}
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 109 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {declar_var((yyvsp[(1) - (1)]),-1,-1);;}
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 110 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {declar_var((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]),-1); v_asgn[(yyvsp[(1) - (4)])] = 1;;}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 114 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {declar_fun((yyvsp[(1) - (3)]),(yyvsp[(2) - (3)]));;}
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 115 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "SET %s\n", v_name[(yyvsp[(5) - (6)])]); acc_id = (yyvsp[(5) - (6)]);;}
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 116 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {func_ret((yyvsp[(2) - (10)]));;}
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 117 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {declar_fun((yyvsp[(1) - (4)]),(yyvsp[(2) - (4)]));;}
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 118 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {func_ret((yyvsp[(2) - (8)]));;}
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 121 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = declar_par((yyvsp[(1) - (2)]),(yyvsp[(2) - (2)]));;}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 122 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "SETP %s\n", v_name[(yyvsp[(3) - (3)])]);;}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 130 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {var_set((yyvsp[(1) - (4)]),(yyvsp[(1) - (4)])+1,0,0);;}
    break;

  case 47:

/* Line 1464 of yacc.c  */
#line 144 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    { fun_id2 = (yyvsp[(1) - (2)]);;}
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 145 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {     vcall((yyvsp[(1) - (6)]));;}
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 146 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    { fun_id2 = (yyvsp[(1) - (2)]);;}
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 147 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = fcall((yyvsp[(1) - (5)]));;}
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 148 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {declar_ret((yyvsp[(2) - (3)]));;}
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 151 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {p_test = get_type((yyvsp[(1) - (1)]));             par_check((yyvsp[(1) - (1)]));;}
    break;

  case 54:

/* Line 1464 of yacc.c  */
#line 152 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {p_test = p_test*10 + get_type((yyvsp[(3) - (3)])); par_check((yyvsp[(3) - (3)]));;}
    break;

  case 55:

/* Line 1464 of yacc.c  */
#line 156 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {exec_out1((yyvsp[(3) - (4)]));     ;}
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 157 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {exec_out2((yyvsp[(6) - (8)]));     ;}
    break;

  case 57:

/* Line 1464 of yacc.c  */
#line 158 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = exec_in((yyvsp[(3) - (4)]));  ;}
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 159 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = exec_abs((yyvsp[(3) - (4)])); ;}
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 160 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {exec_sign1((yyvsp[(3) - (4)]));     ;}
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 161 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = exec_sign2((yyvsp[(6) - (7)]));;}
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 165 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {acc_id = -1; fprintf(f_asm, "@L%delse ", pop_lab());;}
    break;

  case 62:

/* Line 1464 of yacc.c  */
#line 166 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {acc_id = -1; fprintf(f_asm, "@L%dend " , pop_lab());;}
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 169 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {acc_id = -1; fprintf(f_asm, "@L%dend ", pop_lab());;}
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 170 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {acc_id = -1; fprintf(f_asm, "JMP L%dend\n@L%delse ", get_lab(), get_lab());;}
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 172 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {load_check((yyvsp[(3) - (4)]), 0); fprintf(f_asm, "JZ L%delse\n", push_lab()); acc_ok = 0; acc_id = -1;;}
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 175 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {acc_id = -1; fprintf(f_asm, "JMP L%d\n@L%dend ", pop_lab(), get_lab());;}
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 178 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {acc_id = 0; fprintf(f_asm, "@L%d ", push_lab());    ;}
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 179 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {fprintf(f_asm, "JZ L%dend\n", get_lab());acc_ok = 0;;}
    break;

  case 70:

/* Line 1464 of yacc.c  */
#line 180 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {load_check((yyvsp[(4) - (9)]), 0);                                  ;}
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 185 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {acc_id = -1; fprintf(f_asm, "JMP L%d\n@L%dend ", pop_lab(), get_lab());;}
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 186 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {acc_id = -1; fprintf(f_asm, "JMP L%d\n@L%dend ", pop_lab(), get_lab());;}
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 188 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {acc_id = -1; fprintf(f_asm, "@L%d ", push_lab());;}
    break;

  case 74:

/* Line 1464 of yacc.c  */
#line 189 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {load_check((yyvsp[(4) - (5)]), 0); fprintf(f_asm, "JZ L%dend\n", get_lab()); acc_ok = 0;;}
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 196 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {declar_var((yyvsp[(2) - (5)]),-1,-1); var_set((yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]),0,0);;}
    break;

  case 77:

/* Line 1464 of yacc.c  */
#line 197 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {declar_var((yyvsp[(2) - (5)]),-1,-1); var_set((yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]),0,1);;}
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 204 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {var_set((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]),0,0);;}
    break;

  case 79:

/* Line 1464 of yacc.c  */
#line 205 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {var_set((yyvsp[(1) - (3)]),(yyvsp[(1) - (3)])+1,0,0);;}
    break;

  case 80:

/* Line 1464 of yacc.c  */
#line 206 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {var_set((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]),0,0);;}
    break;

  case 81:

/* Line 1464 of yacc.c  */
#line 207 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {var_set((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]),0,1);;}
    break;

  case 82:

/* Line 1464 of yacc.c  */
#line 208 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {var_set((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]),0,2);;}
    break;

  case 83:

/* Line 1464 of yacc.c  */
#line 209 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {array_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]));;}
    break;

  case 84:

/* Line 1464 of yacc.c  */
#line 210 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {var_set((yyvsp[(1) - (8)]),(yyvsp[(6) - (8)]),1,0);;}
    break;

  case 85:

/* Line 1464 of yacc.c  */
#line 215 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {                    (yyval) = load((yyvsp[(1) - (1)]),1,1         ,0);;}
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 216 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = neg((yyvsp[(2) - (2)]));;}
    break;

  case 88:

/* Line 1464 of yacc.c  */
#line 219 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {                    (yyval) = load((yyvsp[(1) - (1)]),1,2         ,0);;}
    break;

  case 89:

/* Line 1464 of yacc.c  */
#line 220 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {                    (yyval) = load((yyvsp[(1) - (1)]),0,v_type[(yyvsp[(1) - (1)])],0);;}
    break;

  case 90:

/* Line 1464 of yacc.c  */
#line 221 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {array_check((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)])); (yyval) = load((yyvsp[(1) - (4)]),0,v_type[(yyvsp[(1) - (4)])],1);;}
    break;

  case 91:

/* Line 1464 of yacc.c  */
#line 222 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) =     (yyvsp[(1) - (1)])*OFST;;}
    break;

  case 92:

/* Line 1464 of yacc.c  */
#line 223 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) =     (yyvsp[(1) - (1)])*OFST;;}
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 224 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) =     (yyvsp[(1) - (1)])*OFST;;}
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 225 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) =     (yyvsp[(1) - (1)])*OFST;;}
    break;

  case 95:

/* Line 1464 of yacc.c  */
#line 226 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) =         (yyvsp[(2) - (3)]) ;;}
    break;

  case 96:

/* Line 1464 of yacc.c  */
#line 227 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) =         (yyvsp[(2) - (2)]) ;;}
    break;

  case 97:

/* Line 1464 of yacc.c  */
#line 228 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = negacao((yyvsp[(2) - (2)]));;}
    break;

  case 98:

/* Line 1464 of yacc.c  */
#line 229 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = int_oper ((yyvsp[(2) - (2)]), 0, "!"  , "LINV", 1);;}
    break;

  case 99:

/* Line 1464 of yacc.c  */
#line 230 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = int_oper ((yyvsp[(2) - (2)]), 0, "~"  ,  "INV", 0);;}
    break;

  case 100:

/* Line 1464 of yacc.c  */
#line 231 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = int_oper ((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "%"  ,  "MOD", 0);;}
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 232 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = int_oper ((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "<<" ,  "SHL", 0);;}
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 233 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = int_oper ((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), ">>" ,  "SHR", 0);;}
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 234 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = int_oper ((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), ">>>",  "SRS", 0);;}
    break;

  case 104:

/* Line 1464 of yacc.c  */
#line 235 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = int_oper ((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "&"  ,  "AND", 0);;}
    break;

  case 105:

/* Line 1464 of yacc.c  */
#line 236 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = int_oper ((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "|"  ,  "OR" , 0);;}
    break;

  case 106:

/* Line 1464 of yacc.c  */
#line 237 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = int_oper ((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "^"  ,  "XOR", 0);;}
    break;

  case 107:

/* Line 1464 of yacc.c  */
#line 238 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = int_oper ((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "&&" , "LAND", 1);;}
    break;

  case 108:

/* Line 1464 of yacc.c  */
#line 239 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = int_oper ((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "||" , "LOR" , 1);;}
    break;

  case 109:

/* Line 1464 of yacc.c  */
#line 240 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = operacoes((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "MLT", "CALL float_mult"                             , &fmlt);;}
    break;

  case 110:

/* Line 1464 of yacc.c  */
#line 241 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = operacoes((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "DIV", "CALL float_div"                              , &fdiv);;}
    break;

  case 111:

/* Line 1464 of yacc.c  */
#line 242 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = operacoes((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "ADD", "CALL denorm\nCALL float_add"                 , &fadd);;}
    break;

  case 112:

/* Line 1464 of yacc.c  */
#line 243 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {int r=negacao((yyvsp[(3) - (3)])); (yyval) = operacoes((yyvsp[(1) - (3)]), r, "ADD", "CALL denorm\nCALL float_add"                 , &fadd);;}
    break;

  case 113:

/* Line 1464 of yacc.c  */
#line 244 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {     operacoes((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "LES", "CALL denorm\nLOAD float_aux3\nLES float_aux1", &fgen); (yyval) = OFST;;}
    break;

  case 114:

/* Line 1464 of yacc.c  */
#line 245 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {     operacoes((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "GRE", "CALL denorm\nLOAD float_aux3\nGRE float_aux1", &fgen); (yyval) = OFST;;}
    break;

  case 115:

/* Line 1464 of yacc.c  */
#line 246 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {     operacoes((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "LES", "CALL denorm\nLOAD float_aux3\nLES float_aux1", &fgen); (yyval) = OFST; fprintf(f_asm, "LINV\n");;}
    break;

  case 116:

/* Line 1464 of yacc.c  */
#line 247 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {     operacoes((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "GRE", "CALL denorm\nLOAD float_aux3\nGRE float_aux1", &fgen); (yyval) = OFST; fprintf(f_asm, "LINV\n");;}
    break;

  case 117:

/* Line 1464 of yacc.c  */
#line 248 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {     operacoes((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "EQU", "CALL denorm\nLOAD float_aux3\nEQU float_aux1", &fgen); (yyval) = OFST;;}
    break;

  case 118:

/* Line 1464 of yacc.c  */
#line 249 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {     operacoes((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "EQU", "CALL denorm\nLOAD float_aux3\nEQU float_aux1", &fgen); (yyval) = OFST; fprintf(f_asm, "LINV\n");;}
    break;

  case 119:

/* Line 1464 of yacc.c  */
#line 250 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"
    {(yyval) = int_oper ((yyvsp[(2) - (2)]), 0, "/>"  , "NORM", 0);;}
    break;



/* Line 1464 of yacc.c  */
#line 2388 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1684 of yacc.c  */
#line 254 "C:\\Users\\LCOM\\Desktop\\Github\\sapho\\C Compiler\\Source\\c2asm.y"


int main(int argc, char *argv[])
{
  yyin   = fopen(argv[1], "r");
	f_asm  = fopen(argv[2], "w");

  float_init();
    //fprintf(f_asm, "LOAD 0\n");
	yyparse();
	fclose(yyin );
	fclose(f_asm);

	if (fgen && prtype == 0) float_gen(argv[2]);

	check_var();

	return 0;
}

void yyerror (char const *s)
{
	//fprintf (stderr, "Erro de sintaxe na linha %d\n", line_num+1);
	fprintf (stderr, "Po, presta atencao na sintaxe da linha %d\n", line_num+1);
}

int load(int id, int is_const, int dtype, int is_array)
{
    if (dtype == 0)
        //fprintf (stderr, "Erro na linha %d: variavel %s nao foi declarada.\n", line_num+1, rem_fname(v_name[id]));
        fprintf (stderr, "Erro na linha %d: mane, declara a variavel %s direito!\n", line_num+1, rem_fname(v_name[id], fname));

    if ((v_asgn[id] == 0) && (is_const == 0))
        //fprintf (stderr, "Erro na linha %d: variavel %s nao foi inicializada.\n", line_num+1, rem_fname(v_name[id]));
        fprintf (stdout, "Atencao na linha %d: como voce quer usar a variavel %s se voce nem deu um valor pra ela?\n", line_num+1, rem_fname(v_name[id], fname));

    if ((v_isar[id] == 1) && (is_array == 0))
        fprintf (stderr, "Erro na linha %d: cade o indice de array da variavel %s?\n", line_num+1, rem_fname(v_name[id], fname));

    if ((v_isar[id] == 0) && (is_array == 1))
        fprintf (stderr, "Erro na linha %d: %s nao eh array nao, mane!\n", line_num+1, rem_fname(v_name[id], fname));

    v_used[id] = 1;
    v_isco[id] = is_const;
    v_isar[id] = is_array;

    if (is_array == 1)
    {
         load_check(dtype*OFST+id, 0);
         return dtype*OFST;
    }
    else return dtype*OFST+id;
}

void load_check(int et, int sinal)
{
    int id = et % OFST;

    if (v_isar[id] == 1)
    {
      if(sinal == 0)
      {
        fprintf(f_asm, "PUSH\nSRF\nLOAD %s\n", v_name[id]);
      }
      else
      {
        fprintf(f_asm, "PUSH\nSRF\nLOAD -%s\n", v_name[id]);
      }
    }
    else
    {
        char num[64];

        if ((v_isco[id] == 1) && (et >= 2*OFST) && (prtype == 0))
        {
            itoa(f2mf(v_name[id]), num, 10);
            strcat(num, " // ");
            strcat(num, v_name[id]);
        }
        else strcpy(num, v_name[id]);

        if ((et != OFST) && (et != 2*OFST)) // se nao esta na pilha, carrega
        {
            if (acc_ok == 0)
            {
                /*if (acc_id != id)*/
                if(sinal == 0)
                {
                  fprintf(f_asm, "LOAD %s\n", num);
                }
                else
                {
                  fprintf(f_asm, "LOAD -%s\n", num);
                }
            }
            else
            {

                                // se acc carregado
                if(sinal == 0)
                {
                    fprintf(f_asm, "PLD %s\n", num);
                }
                else
                {
                    fprintf(f_asm, "PLD -%s\n", num);
                }
            }
        }
    }

    acc_ok = 1;
}

void func_ret(int id)
{
    if (((v_type[id] == 4) || (v_type[id] == 5)) && (ret_ok == 0))
        fprintf (stderr, "Erro na funcao %s: voce nao definiu nenhum retorno pra essa funcao.\n", v_name[id]);

    if (strcmp(v_name[id], "main") == 0)
    {
        if (mainok == 0)
            fprintf(f_asm, "@fim JMP fim\n");
        else
            fprintf(f_asm, "RETURN\n");

        v_used[id] = 1;
            mainok = 1;
    }
    else if (v_type[id] == 3) fprintf(f_asm, "RETURN\n");

    strcpy(fname, "");
}

void var_set(int id, int et, int is_array, int set_type) // set_type =0 ->SET; set_type =1 -> PSET; set_type =2 ->NORMS
{
    load_check(et,0);

    if (v_type[id] == 0)
    {
        //fprintf (stderr, "Erro na linha %d: variavel %s nao foi declarada.\n", line_num+1, rem_fname(v_name[id]));
        fprintf (stderr, "Erro na linha %d: se voce declarar a variavel %s eu agradeco.\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    if ((v_isar[id] == 0) && (is_array == 1))
    {
        fprintf (stderr, "Erro na linha %d: %s nao eh um array.\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    if ((v_isar[id] == 1) && (is_array == 0))
    {
        fprintf (stderr, "Erro na linha %d: %s eh um array. Cade o indice?\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    if ((v_type[id] == 1) && (et >= 2*OFST))
    {
        fprintf(stdout, "Atencao na linha %d: variavel %s eh int, mas recebe float.\n", line_num+1, rem_fname(v_name[id], fname));
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    if ((v_type[id] == 2) && (et < 2*OFST))
    {
        fprintf(stdout, "Atencao na linha %d: variavel %s eh float, mas recebe int.\n", line_num+1, rem_fname(v_name[id], fname));
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL int2float\n");
            i2f = 1;
        }
    }

    char cset[10];
    if (is_array)
        strcpy(cset, "SRF\nSET");
    else if (set_type == 1)
            strcpy(cset, "PSET");
    else if (set_type == 2)
            strcpy(cset, "NORMS");
        else
        strcpy(cset, "SET");

    fprintf(f_asm, "%s %s\n", cset, v_name[id]);

    acc_ok     = 0;
    acc_id     = id;
    v_asgn[id] = 1;
}


int neg(int et)
{
    load_check(et, 1);
    return (et < 2*OFST) ? OFST : 2*OFST;

}

int negacao(int et)
{
    load_check(et, 0);

    if (prtype == 1)
        fprintf(f_asm, "NEG\n");
    else if (et < 2*OFST)
        fprintf(f_asm, "MLT -1\n");
    else
        fprintf(f_asm, "PLD 1\nPLD float_nbits\nSSHL\nSADD\n");

    return (et < 2*OFST) ? OFST : 2*OFST;
}

int iacc(int et)
{
    return (et == OFST) ? 1 : 0;
}

int imem(int et)
{
    return ((et > OFST) && (et < 2*OFST)) ? 1 : 0;
}

int facc(int et)
{
    return (et == 2*OFST) ? 1 : 0;
}

int fmem(int et)
{
    return (et > 2*OFST) ? 1 : 0;
}

int operacoes(int et1, int et2, char *iop, char *fop, int *op)
{
    if (prtype == 1)
    {
             if ((et1 % OFST != 0) && (et2 % OFST != 0))         // memoria e memoria
        {
            load_check(et2, 0);
            fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
        }
        else if ((et1 % OFST == 0) && (et2 % OFST != 0))         // acc e memoria
        {
            load_check(et2, 0);
            fprintf(f_asm, "S%s\n", iop);
        }
        else if ((et1 % OFST != 0) && (et2 % OFST == 0))         // memoria e acc
        {
            fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
        }
        else                                                     // pilha e acc
        {
            fprintf(f_asm, "S%s\n", iop);
        }
    }
    else
    {
             if (imem(et1) && imem(et2))                         // int memoria e int memoria
        {
            load_check(et2, 0);
            fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
        }
        else if (imem(et1) && fmem(et2))                         // int memoria e float memoria
        {
            load_check(et1, 0);
            fprintf(f_asm, "CALL int2float\n");
            load_check(et2, 0);
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (fmem(et1) && imem(et2))                         // float memoria e int memoria
        {
            load_check(et1, 0);
            load_check(et2, 0);
            fprintf(f_asm, "CALL int2float\n");
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (fmem(et1) && fmem(et2))                         // float memoria e float memoria
        {
            load_check(et1, 0);
            load_check(et2, 0);
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
        }
        else if (iacc(et1) && imem(et2))                         // int acc e int memoria
        {
            load_check(et2, 0);
            fprintf(f_asm, "S%s\n", iop);
        }
        else if (iacc(et1) && fmem(et2))                         // int acc e float memoria
        {
            fprintf(f_asm, "CALL int2float\n");
            load_check(et2, 0);
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (facc(et1) && imem(et2))                         // float acc e int memoria
        {
            load_check(et2, 0);
            fprintf(f_asm, "CALL int2float\n");
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (facc(et1) && fmem(et2))                         // float acc e float memoria
        {
            load_check(et2, 0);
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
        }
        else if (imem(et1) && iacc(et2))                         // int memoria e int acc
        {
            fprintf(f_asm, "%s %s\n", iop, v_name[et1 % OFST]);
        }
        else if (imem(et1) && facc(et2))                         // int memoria e float acc
        {
            if ((strcmp(iop, "DIV") == 0) ||
                (strcmp(iop, "LES") == 0) ||
                (strcmp(iop, "GRE") == 0))
                fprintf(f_asm, "SET float_aux5\nLOAD %s\nCALL int2float\nPLD float_aux5\n", v_name[et1 % OFST]);
            else
            {
                load_check(et1, 0);
                fprintf(f_asm, "CALL int2float\n");
            }
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (fmem(et1) && iacc(et2))                         // float memoria e int acc
        {
            fprintf(f_asm, "CALL int2float\n");
            if ((strcmp(iop, "DIV") == 0) ||
                (strcmp(iop, "LES") == 0) ||
                (strcmp(iop, "GRE") == 0))
            {
                fprintf(f_asm, "SET float_aux4\n");
                acc_ok = 0;
                load_check(et1, 0);
                fprintf(f_asm, "PLD float_aux4\n");
            }
            else
                load_check(et1, 0);
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (fmem(et1) && facc(et2))                         // float memoria e float acc
        {
            if ((strcmp(iop, "DIV") == 0) ||
                (strcmp(iop, "LES") == 0) ||
                (strcmp(iop, "GRE") == 0))
            {
                fprintf(f_asm, "SET float_aux4\n");
                acc_ok = 0;
                load_check(et1, 0);
                fprintf(f_asm, "PLD float_aux4\n");
            }
            else
                load_check(et1, 0);
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
        }
        else if (iacc(et1) && iacc(et2))                         // int pilha e int acc
        {
            fprintf(f_asm, "S%s\n", iop);
        }
        else if (iacc(et1) && facc(et2))                         // int pilha e float acc
        {
            fprintf(f_asm, "SETP float_aux5\nCALL int2float\nPLD float_aux5\n%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (facc(et1) && iacc(et2))                         // float pilha e int acc
        {
            fprintf(f_asm, "CALL int2float\n");
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
            i2f = 1;
        }
        else if (facc(et1) && facc(et2))                         // float pilha e float acc
        {
            fprintf(f_asm, "%s\n", fop);
            *op = 1;
        }
    }

    return ((et1 >= 2*OFST) || (et2 >= 2*OFST)) ? 2*OFST : OFST;
}

void declar_var (int id1, int id2, int id3) // id3 -> indice para pegar string, declaracao de array.
{
    if (v_type[id1] != 0) // variavel ja existe
    {
        //fprintf (stderr, "Erro na linha %d: variavel %s ja existe.\n", line_num+1, rem_fname(v_name[id1]));
        fprintf (stderr, "Erro na linha %d: puts, a variavel %s ja existe, ta doido?\n", line_num+1, rem_fname(v_name[id1], fname));
        return;
    }

    v_type[id1] = type_tmp;
    v_used[id1] = 0;
    v_asgn[id1] = 0;
    v_fnid[id1] = find_var(fname);

    if (id2 != -1) // significa que eh array
    {
        if(id3 == -1)
          fprintf(f_asm, "#array %s %s\n", v_name[id1], v_name[id2]);
        else
          fprintf(f_asm, "#arrays %s %s %s\n", v_name[id1], v_name[id2], v_name[id3]);
          v_isar[id1] = 1;
    }
}

int declar_par(int t, int id)
{
    declar_var(id,-1,-1);
    v_asgn[id] = 1;
    v_fpar[fun_id1] = v_fpar[fun_id1]*10 + t;

    return id;
}

void declar_fun(int id1, int id2) //id1 -> tipo id2 -> indice para o nome; v_name -> tabela com os nomes
{

    if ((mainok == 0) && (strcmp(v_name[id2], "main") != 0))
    {
        fprintf(f_asm, "CALL main\n@fim JMP fim\n");
        mainok = 2;
    }

    fprintf(f_asm, "@%s ", v_name[id2]); strcpy(fname, v_name[id2]);

    v_type[id2] = id1+3;
    fun_id1     = id2;
    ret_ok      = 0;
}

void declar_ret(int et)
{
    load_check(et, 0);

    if ((v_type[fun_id1] != 4) && (v_type[fun_id1] != 5))
        fprintf (stderr, "Erro na linha %d: retorno em funcao void? hi o cara!\n", line_num+1);

    if ((v_type[fun_id1] == 4) && (et >= 2*OFST))
    {
        fprintf(stdout, "Atencao na linha %d: convertendo float para int no retorno da funcao %s.\n", line_num+1, v_name[fun_id1]);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    if ((v_type[fun_id1] == 5) && (et < 2*OFST))
    {
        fprintf(stdout, "Atencao na linha %d: convertendo int para float no retorno da funcao %s.\n", line_num+1, v_name[fun_id1]);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL int2float\n");
            i2f = 1;
        }
    }

    fprintf(f_asm, "RETURN\n");

    acc_ok = 0;
    ret_ok = 1;
}

int fcall(int id)
{
    if  (v_type[id] == 3)
        fprintf (stderr, "Erro na linha %d: funcao %s nao retorna nada.\n", line_num+1, rem_fname(v_name[id], fname));
    else if ((v_type[id] != 4) && (v_type[id] != 5))
        fprintf (stderr, "Erro na linha %d: nao estou achando a funcao %s.\n", line_num+1, rem_fname(v_name[id], fname));

    if (get_npar(p_test) != get_npar(v_fpar[id]))
        fprintf(stderr, "Erro na linha %d: lista de parametros da funcao %s difere da original.\n", line_num+1, rem_fname(v_name[id], fname));
    //fprintf (stderr, "Indice %d, nome %s\n",id, v_name[id]);
    fprintf(f_asm, "CALL %s\n",v_name[id]);
    //fprintf(f_asm, "CALL %s\n", rem_fname(v_name[id], fname));

    v_used[id] = 1;

    return v_type[id]-3;
}

void vcall(int id)
{
    if  ((v_type[id] != 3) && (v_type[id] != 4) && (v_type[id] != 5))
        fprintf (stderr, "Erro na linha %d: nao existe a funcao %s.\n", line_num+1, rem_fname(v_name[id], fname));

    if (get_npar(p_test) != get_npar(v_fpar[id]))
        fprintf(stderr, "Erro na linha %d: lista de parametros da funcao %s difere da original.\n", line_num+1, rem_fname(v_name[id], fname));

    fprintf(f_asm, "CALL %s\n", v_name[id]);

    v_used[id] = 1;
    acc_ok     = 0;
}

int int_oper(int et1, int et2, char *op, char *code, int fok)
{
    if  ((prtype == 1) && (fok == 0))
        fprintf(stderr, "Erro na linha %d: processador em ponto flutuante nao aceita operador %s. Arruma outra logica ai.\n", line_num+1, op);
    if ((prtype == 0) && ((et1 >= 2*OFST) || (et2 >= 2*OFST)))
        fprintf(stderr, "Erro na linha %d: uso incorreto do operador %s. Os operandos tem que ser do tipo int.\n", line_num+1, op);


    if (et2 == 0)
    {
        if (imem(et1)) load_check(et1, 0);
        fprintf(f_asm, "%s\n", code);
    }
    else
    {
        int aux;
        operacoes(et1, et2, code, "", &aux);
    }

    return OFST;
}

void array_check(int id, int et)
{
    if (v_isar[id] == 0)
    {
        fprintf(stderr, "Erro na linha %d: %s nao eh um array.\n", line_num+1, rem_fname(v_name[id], fname));
        return;
    }

    load_check(et, 0);

    if (et >= 2*OFST)
    {
        fprintf(stderr, "Atencao na linha %d: Indice de array tem que ser do tipo int. Estou convertendo float pra int aqui pra quebrar o teu galho.\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }
}

int get_npar(int par)
{
    int t_fun = par;
    int n_par = 0;
    while (t_fun != 0)
    {
        t_fun = t_fun/10;
        n_par++;
    }
    return n_par;
}

void par_check(int et)
{
    // pega numero de parametros original
    int n_par = get_npar(v_fpar[fun_id2]);

    // pega tipo e posicao do parametro atual a ser chamado
    int t_cal = p_test;
    int aux   = p_test;
    int id_cal = n_par;
    int index = 1;
    while (aux > 10)
    {
        aux = aux /10;
        t_cal = t_cal % 10;
        id_cal--;
        index++;
    }

    // pega tipo do parametro atual na funcao original
    int t_fun = v_fpar[fun_id2];
    int i;
    for (i = 1; i < id_cal; i++)  t_fun = t_fun/10;
    t_fun = t_fun % 10;

    // ufa, agora sim, posso testar os parametros
    load_check(et, 0);

    if ((t_fun == 1) && (t_cal == 2))
    {
        fprintf(stdout, "Atencao na linha %d: convertendo float para int no parametro %d da funcao %s.\n", line_num+1, index, v_name[fun_id1]);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }
    else if ((t_fun == 2) && (t_cal == 1))
    {
        fprintf(stdout, "Atencao na linha %d: convertendo int para float no parametro %d da funcao %s.\n", line_num+1, index, v_name[fun_id1]);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL int2float\n");
            i2f = 1;
        }
    }
}

int exec_in(int et)
{
    load_check(et, 0);

    if (et >= 2*OFST)
    {
        fprintf(stdout, "Atencao na linha %d: endereco de entrada tem que ser int. Soh me dando trabalho a toa!\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    fprintf(f_asm, "PUSH\nIN\n");

    return (prtype == 0) ? 1 : 2;
}

int exec_abs(int et)
{
    load_check(et, 0);

    if (et >= 2*OFST)
    {
        //fprintf(stdout, "Atencao na linha %d: endereco de entrada tem que ser int. Soh me dando trabalho a toa!\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    fprintf(f_asm, "ABS\n");

    return (prtype == 0) ? 1 : 2;
}

int exec_sign(int et, int et2)
{
    load_check(et, 0);
    load_check(et2, 0);

    if (et >= 2*OFST)
    {
        //fprintf(stdout, "Atencao na linha %d: endereco de entrada tem que ser int. Soh me dando trabalho a toa!\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    fprintf(f_asm, "SIGN\n");

    return (prtype == 0) ? 1 : 2;
}

void exec_sign1(int et)
{
    load_check(et, 0);

    if (et >= 2*OFST)
    {
        fprintf(stdout, "Atencao na linha %d: endereco de saida tem que ser int. Soh me dando trabalho a toa!\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }
}

int exec_sign2(int et)
{
    load_check(et, 0);

    if (et >= 2*OFST)
    {
        //fprintf(stdout, "Atencao na linha %d: endereco de entrada tem que ser int. Soh me dando trabalho a toa!\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }

    fprintf(f_asm, "SIGN\n");

    return (prtype == 0) ? 1 : 2;
}

int get_type(int et)
{
    int t;
    if (et == 0)
        t = 0;
    else
        t = (et >= 2*OFST) ? 2 : 1;

    return t;
}

void exec_out1(int et)
{
    load_check(et, 0);

    if (et >= 2*OFST)
    {
        fprintf(stdout, "Atencao na linha %d: endereco de saida tem que ser int. Soh me dando trabalho a toa!\n", line_num+1);
        if (prtype == 0)
        {
            fprintf(f_asm, "CALL float2int\n");
            f2i = 1;
        }
    }
}

void exec_out2(int et)
{
    load_check(et, 0);

    if ((et >= 2*OFST) && (prtype == 0))
    {
        fprintf(stdout, "Atencao na linha %d: o processador eh ponto fixo e voce quer mandar um numero em ponto flutuante pra fora. Soh me dando dor de cabeca!\n", line_num+1);
        fprintf(f_asm, "CALL float2int\n");
        f2i = 1;
    }

    if ((et <  2*OFST) && (prtype == 1))
        fprintf(stdout, "Atencao na linha %d: o processador eh ponto flutuante e voce quer mandar um ponto fixo pra fora.\n", line_num+1);

    fprintf(f_asm, "OUT\n");

    acc_ok = 0;
}

