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
#line 25 "./CMMComp/c2asm.y"


#include "variaveis.h"   // tabela de variaveis
#include "labels.h"      // geracao de labels para saltos
#include "t2t.h"         // conversoes de tipos
#include "funcoes.h"     // criacao e uso de funcoes
#include "oper.h"        // operacoes da ULA
#include "stdlib.h"      // biblioteca padrao do sapho
#include "diretivas.h"   // diretivas de compilacao
#include "saltos.h"      // gerenciamento saltos (if/else while)
#include "data_declar.h" // declaracao de dados
#include "data_use.h"    // utilizacao de dados
#include "data_assign.h" // atribuicao de dados

// variaveis obrigatorias do flex/bison

FILE *yyin;
int   yylex  (void);
void  yyerror(char const *s);



/* Line 189 of yacc.c  */
#line 95 "./CMMComp/y.tab.c"

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
     PRNAME = 258,
     DIRNAM = 259,
     DATYPE = 260,
     NUBITS = 261,
     NBMANT = 262,
     NBEXPO = 263,
     NDSTAC = 264,
     SDEPTH = 265,
     NUIOIN = 266,
     NUIOOU = 267,
     NUGAIN = 268,
     USEMAC = 269,
     ENDMAC = 270,
     FFTSIZ = 271,
     ITRADD = 272,
     IN = 273,
     OUT = 274,
     NRM = 275,
     PST = 276,
     ABS = 277,
     SIGN = 278,
     SQRT = 279,
     WHILE = 280,
     IF = 281,
     THEN = 282,
     ELSE = 283,
     SWITCH = 284,
     CASE = 285,
     DEFAULT = 286,
     RETURN = 287,
     BREAK = 288,
     SHIFTL = 289,
     SHIFTR = 290,
     SSHIFTR = 291,
     GREQU = 292,
     LESEQ = 293,
     EQU = 294,
     DIF = 295,
     LAND = 296,
     LOR = 297,
     NORM = 298,
     EQNE = 299,
     PPLUS = 300,
     TYPE = 301,
     ID = 302,
     STRING = 303,
     INUM = 304,
     FNUM = 305,
     CNUM = 306
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
#line 188 "./CMMComp/y.tab.c"

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
#define YYFINAL  38
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1919

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  76
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  67
/* YYNRULES -- Number of rules.  */
#define YYNRULES  179
/* YYNRULES -- Number of states.  */
#define YYNSTATES  372

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   306

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    63,     2,     2,    75,    62,    55,     2,
      68,    69,    60,    58,    52,    59,     2,    61,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    72,    65,
      57,    73,    56,     2,    74,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    66,     2,    67,    54,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    70,    53,    71,    64,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    12,    14,    16,    19,
      22,    25,    28,    31,    34,    37,    40,    43,    46,    49,
      52,    55,    57,    60,    62,    64,    68,    76,    87,    89,
      93,    95,   100,   108,   109,   110,   121,   122,   131,   134,
     138,   142,   145,   147,   150,   154,   156,   158,   160,   162,
     164,   166,   168,   170,   172,   174,   176,   178,   180,   181,
     188,   189,   195,   196,   198,   202,   203,   212,   217,   222,
     227,   234,   239,   244,   245,   251,   254,   259,   260,   269,
     271,   274,   278,   279,   285,   286,   292,   293,   298,   300,
     302,   305,   308,   309,   315,   318,   320,   326,   332,   338,
     344,   350,   355,   360,   365,   370,   375,   379,   386,   396,
     397,   406,   407,   416,   417,   426,   427,   436,   437,   446,
     447,   456,   457,   466,   467,   476,   477,   486,   487,   496,
     497,   509,   510,   522,   523,   535,   536,   548,   549,   561,
     563,   565,   567,   569,   574,   579,   587,   589,   591,   593,
     595,   597,   599,   601,   605,   608,   611,   614,   617,   620,
     626,   635,   639,   643,   647,   651,   655,   659,   663,   667,
     671,   675,   679,   683,   687,   691,   695,   699,   703,   707
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      77,     0,    -1,    78,    -1,    79,    -1,    78,    79,    -1,
      80,    -1,    84,    -1,    87,    -1,     3,    47,    -1,     4,
      48,    -1,     5,    49,    -1,     6,    49,    -1,     7,    49,
      -1,     8,    49,    -1,     9,    49,    -1,    10,    49,    -1,
      11,    49,    -1,    12,    49,    -1,    13,    49,    -1,    16,
      49,    -1,    14,    48,    -1,    15,    -1,    14,    48,    -1,
      15,    -1,    17,    -1,    46,    85,    65,    -1,    46,    47,
      66,    49,    67,    48,    65,    -1,    46,    47,    66,    49,
      67,    66,    49,    67,    48,    65,    -1,    86,    -1,    85,
      52,    86,    -1,    47,    -1,    47,    66,    49,    67,    -1,
      47,    66,    49,    67,    66,    49,    67,    -1,    -1,    -1,
      46,    47,    68,    88,    91,    69,    89,    70,    93,    71,
      -1,    -1,    46,    47,    68,    69,    90,    70,    93,    71,
      -1,    46,    47,    -1,    91,    52,    91,    -1,    32,   142,
      65,    -1,    32,    65,    -1,    94,    -1,    93,    94,    -1,
      70,    93,    71,    -1,    95,    -1,   112,    -1,   124,    -1,
      83,    -1,   125,    -1,   126,    -1,   121,    -1,   109,    -1,
     101,    -1,    96,    -1,    92,    -1,    81,    -1,    82,    -1,
      -1,    47,    68,    97,   100,    69,    65,    -1,    -1,    47,
      68,    99,   100,    69,    -1,    -1,   142,    -1,   100,    52,
     142,    -1,    -1,    19,    68,   142,    52,   102,   142,    69,
      65,    -1,    18,    68,   142,    69,    -1,    21,    68,   142,
      69,    -1,    22,    68,   142,    69,    -1,    23,    68,   142,
      52,   142,    69,    -1,    20,    68,   142,    69,    -1,    24,
      68,   142,    69,    -1,    -1,   111,    94,    28,   110,    94,
      -1,   111,    94,    -1,    26,    68,   142,    69,    -1,    -1,
      29,    68,   142,    69,   113,    70,   120,    71,    -1,    95,
      -1,   114,    95,    -1,   114,    33,    65,    -1,    -1,    30,
      49,    72,   116,   114,    -1,    -1,    30,    50,    72,   117,
     114,    -1,    -1,    31,    72,   119,   114,    -1,   115,    -1,
     118,    -1,   115,   120,    -1,   122,    94,    -1,    -1,    25,
     123,    68,   142,    69,    -1,    33,    65,    -1,    84,    -1,
      46,    47,    73,   142,    65,    -1,    46,    47,    74,   142,
      65,    -1,    46,    47,    43,   142,    65,    -1,    46,    47,
      75,   142,    65,    -1,    46,    47,    44,   142,    65,    -1,
      47,    73,   142,    65,    -1,    47,    74,   142,    65,    -1,
      47,    43,   142,    65,    -1,    47,    75,   142,    65,    -1,
      47,    44,   142,    65,    -1,    47,    45,    65,    -1,    47,
      66,   142,    67,    45,    65,    -1,    47,    66,   142,    67,
      66,   142,    67,    45,    65,    -1,    -1,    47,    66,   142,
      67,    73,   127,   142,    65,    -1,    -1,    47,    66,   142,
      67,    74,   128,   142,    65,    -1,    -1,    47,    66,   142,
      67,    43,   129,   142,    65,    -1,    -1,    47,    66,   142,
      67,    75,   130,   142,    65,    -1,    -1,    47,    66,   142,
      67,    44,   131,   142,    65,    -1,    -1,    47,    66,   142,
      69,    73,   132,   142,    65,    -1,    -1,    47,    66,   142,
      69,    74,   133,   142,    65,    -1,    -1,    47,    66,   142,
      69,    43,   134,   142,    65,    -1,    -1,    47,    66,   142,
      69,    75,   135,   142,    65,    -1,    -1,    47,    66,   142,
      69,    44,   136,   142,    65,    -1,    -1,    47,    66,   142,
      67,    66,   142,    67,    73,   137,   142,    65,    -1,    -1,
      47,    66,   142,    67,    66,   142,    67,    74,   138,   142,
      65,    -1,    -1,    47,    66,   142,    67,    66,   142,    67,
      43,   139,   142,    65,    -1,    -1,    47,    66,   142,    67,
      66,   142,    67,    75,   140,   142,    65,    -1,    -1,    47,
      66,   142,    67,    66,   142,    67,    44,   141,   142,    65,
      -1,    49,    -1,    50,    -1,    51,    -1,    47,    -1,    47,
      66,   142,    67,    -1,    47,    66,   142,    69,    -1,    47,
      66,   142,    67,    66,   142,    67,    -1,   103,    -1,   104,
      -1,   105,    -1,   106,    -1,   107,    -1,   108,    -1,    98,
      -1,    68,   142,    69,    -1,    58,   142,    -1,    59,   142,
      -1,    63,   142,    -1,    64,   142,    -1,    47,    45,    -1,
      47,    66,   142,    67,    45,    -1,    47,    66,   142,    67,
      66,   142,    67,    45,    -1,   142,    34,   142,    -1,   142,
      35,   142,    -1,   142,    36,   142,    -1,   142,    55,   142,
      -1,   142,    53,   142,    -1,   142,    54,   142,    -1,   142,
      41,   142,    -1,   142,    42,   142,    -1,   142,    62,   142,
      -1,   142,    60,   142,    -1,   142,    61,   142,    -1,   142,
      58,   142,    -1,   142,    59,   142,    -1,   142,    57,   142,
      -1,   142,    56,   142,    -1,   142,    37,   142,    -1,   142,
      38,   142,    -1,   142,    39,   142,    -1,   142,    40,   142,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    87,    87,    88,    88,    90,    90,    90,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   111,   112,   113,   117,   118,   119,   121,   121,
     123,   124,   125,   129,   130,   129,   132,   132,   138,   139,
     142,   143,   147,   147,   150,   151,   152,   153,   154,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   170,   170,
     173,   173,   180,   181,   182,   186,   186,   188,   189,   190,
     191,   192,   193,   197,   197,   199,   200,   204,   204,   207,
     208,   209,   211,   211,   212,   212,   213,   213,   215,   215,
     215,   219,   220,   220,   222,   226,   227,   228,   229,   230,
     231,   236,   237,   238,   239,   240,   242,   243,   244,   246,
     246,   248,   248,   250,   250,   252,   252,   254,   254,   257,
     257,   259,   259,   261,   261,   263,   263,   265,   265,   268,
     268,   270,   270,   272,   272,   274,   274,   276,   276,   288,
     289,   290,   292,   293,   294,   295,   297,   298,   299,   300,
     301,   302,   303,   305,   306,   308,   309,   310,   311,   312,
     313,   315,   316,   317,   318,   319,   320,   321,   322,   324,
     325,   326,   327,   328,   330,   331,   332,   333,   334,   335
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PRNAME", "DIRNAM", "DATYPE", "NUBITS",
  "NBMANT", "NBEXPO", "NDSTAC", "SDEPTH", "NUIOIN", "NUIOOU", "NUGAIN",
  "USEMAC", "ENDMAC", "FFTSIZ", "ITRADD", "IN", "OUT", "NRM", "PST", "ABS",
  "SIGN", "SQRT", "WHILE", "IF", "THEN", "ELSE", "SWITCH", "CASE",
  "DEFAULT", "RETURN", "BREAK", "SHIFTL", "SHIFTR", "SSHIFTR", "GREQU",
  "LESEQ", "EQU", "DIF", "LAND", "LOR", "NORM", "EQNE", "PPLUS", "TYPE",
  "ID", "STRING", "INUM", "FNUM", "CNUM", "','", "'|'", "'^'", "'&'",
  "'>'", "'<'", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'", "';'",
  "'['", "']'", "'('", "')'", "'{'", "'}'", "':'", "'='", "'@'", "'$'",
  "$accept", "fim", "prog", "prog_elements", "direct", "use_macro",
  "end_macro", "use_inter", "declar", "id_list", "IID", "funcao", "$@1",
  "$@2", "$@3", "par_list", "return_call", "stmt_list", "stmt_full",
  "stmt_case", "void_call", "$@4", "func_call", "$@5", "exp_list",
  "std_out", "$@6", "std_in", "std_pst", "std_abs", "std_sign", "std_nrm",
  "std_sqrt", "if_else_stmt", "$@7", "if_exp", "switch_case", "$@8",
  "case_list", "case", "$@9", "$@10", "default", "$@11", "cases",
  "while_stmt", "while_exp", "$@12", "break", "declar_full", "assignment",
  "$@13", "$@14", "$@15", "$@16", "$@17", "$@18", "$@19", "$@20", "$@21",
  "$@22", "$@23", "$@24", "$@25", "$@26", "$@27", "exp", 0
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,    44,   124,    94,    38,    62,    60,    43,    45,
      42,    47,    37,    33,   126,    59,    91,    93,    40,    41,
     123,   125,    58,    61,    64,    36
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    76,    77,    78,    78,    79,    79,    79,    80,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    81,    82,    83,    84,    84,    84,    85,    85,
      86,    86,    86,    88,    89,    87,    90,    87,    91,    91,
      92,    92,    93,    93,    94,    94,    94,    94,    94,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    97,    96,
      99,    98,   100,   100,   100,   102,   101,   103,   104,   105,
     106,   107,   108,   110,   109,   109,   111,   113,   112,   114,
     114,   114,   116,   115,   117,   115,   119,   118,   120,   120,
     120,   121,   123,   122,   124,   125,   125,   125,   125,   125,
     125,   126,   126,   126,   126,   126,   126,   126,   126,   127,
     126,   128,   126,   129,   126,   130,   126,   131,   126,   132,
     126,   133,   126,   134,   126,   135,   126,   136,   126,   137,
     126,   138,   126,   139,   126,   140,   126,   141,   126,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     2,     1,     1,     3,     7,    10,     1,     3,
       1,     4,     7,     0,     0,    10,     0,     8,     2,     3,
       3,     2,     1,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     6,
       0,     5,     0,     1,     3,     0,     8,     4,     4,     4,
       6,     4,     4,     0,     5,     2,     4,     0,     8,     1,
       2,     3,     0,     5,     0,     5,     0,     4,     1,     1,
       2,     2,     0,     5,     2,     1,     5,     5,     5,     5,
       5,     4,     4,     4,     4,     4,     3,     6,     9,     0,
       8,     0,     8,     0,     8,     0,     8,     0,     8,     0,
       8,     0,     8,     0,     8,     0,     8,     0,     8,     0,
      11,     0,    11,     0,    11,     0,    11,     0,    11,     1,
       1,     1,     1,     4,     4,     7,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     2,     2,     2,     2,     5,
       8,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     2,     3,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    20,    19,    30,     0,    28,     1,     4,
       0,    33,     0,    25,     0,    36,     0,    30,    29,    31,
       0,     0,     0,     0,     0,     0,     0,    38,     0,    34,
       0,    26,     0,     0,    23,    24,     0,    92,     0,     0,
       0,     0,     0,     0,     0,    56,    57,    48,    95,    55,
       0,    42,    45,    54,    53,    52,     0,    46,    51,     0,
      47,    49,    50,    39,     0,    31,    32,    22,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   142,   139,
     140,   141,     0,     0,     0,     0,    41,     0,   152,   146,
     147,   148,   149,   150,   151,     0,    94,    30,     0,     0,
       0,     0,    58,     0,     0,     0,     0,    37,    43,    75,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   158,     0,    60,   154,   155,   156,
     157,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    40,     0,     0,     0,     0,     0,     0,     0,   106,
       0,    62,     0,     0,     0,    44,    73,     0,     0,    27,
      65,     0,    76,    77,     0,     0,     0,     0,     0,     0,
       0,    62,   153,   161,   162,   163,   176,   177,   178,   179,
     167,   168,   165,   166,   164,   175,   174,   172,   173,   170,
     171,   169,     0,     0,     0,     0,     0,   103,   105,     0,
       0,     0,    63,   101,   102,   104,     0,    35,    32,     0,
      93,     0,    67,    71,    68,    69,     0,    72,   143,   144,
       0,    98,   100,    96,    97,    99,   113,   117,     0,     0,
     109,   111,   115,   123,   127,   119,   121,   125,     0,     0,
      74,     0,     0,     0,   159,     0,    61,     0,     0,   107,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
      59,     0,     0,     0,    88,    89,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,     0,    86,    90,    78,   145,   114,   118,   133,
     137,     0,   129,   131,   135,   110,   112,   116,   124,   128,
     120,   122,   126,    82,    84,     0,   160,     0,     0,   108,
       0,     0,     0,     0,     0,    79,    87,     0,     0,     0,
       0,     0,    83,    85,     0,    80,   134,   138,   130,   132,
     136,    81
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    16,    17,    18,    19,    75,    76,    77,    78,    36,
      37,    21,    46,    94,    50,    52,    79,    80,    81,    82,
      83,   191,   118,   211,   241,    84,   249,   119,   120,   121,
     122,   123,   124,    85,   246,    86,    87,   251,   356,   304,
     353,   354,   305,   345,   306,    88,    89,    99,    90,    91,
      92,   291,   292,   287,   293,   288,   296,   297,   294,   298,
     295,   350,   351,   347,   352,   348,   242
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -264
static const yytype_int16 yypact[] =
{
     317,   -38,   -42,    -3,    13,    27,    62,    68,    74,    79,
      80,    83,    73,  -264,    84,    88,   136,   317,  -264,  -264,
    -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,
    -264,  -264,  -264,  -264,  -264,    18,   -49,  -264,  -264,  -264,
      92,    78,   102,  -264,    86,  -264,   108,    89,  -264,   -43,
      91,   118,   -35,   117,   103,   133,     7,  -264,   108,  -264,
     116,  -264,   120,   145,  -264,  -264,   126,  -264,   127,   128,
     245,   119,   150,   834,     7,  -264,  -264,  -264,  -264,  -264,
     105,  -264,  -264,  -264,  -264,  -264,     7,  -264,  -264,     7,
    -264,  -264,  -264,   148,   134,   137,   157,  -264,   294,   138,
     294,   294,   139,   141,   142,   143,   146,   151,   -41,  -264,
    -264,  -264,   294,   294,   294,   294,  -264,   294,  -264,  -264,
    -264,  -264,  -264,  -264,  -264,   876,  -264,    96,   294,   294,
     155,   294,  -264,   294,   294,   294,   131,  -264,  -264,   188,
    -264,     7,   182,   168,  1708,   294,   402,   438,   294,   294,
     294,   294,   294,   294,  -264,   294,  -264,    65,    65,  -264,
    -264,   474,   294,   294,   294,   294,   294,   294,   294,   294,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     294,  -264,   294,   294,   294,   294,   294,   908,   940,  -264,
     330,   294,   972,  1004,  1036,  -264,  -264,   215,   169,  -264,
    -264,   510,  -264,  -264,   546,   582,   618,   654,  1737,   690,
     366,   294,  -264,   -47,   -47,   -47,   239,   239,  1857,  1857,
    1805,  1776,  1814,  1843,  1850,   239,   239,    65,    65,  -264,
    -264,  -264,  1068,  1100,  1132,  1164,  1196,  -264,  -264,   -25,
     115,   -17,  1766,  -264,  -264,  -264,     7,  -264,  -264,   294,
    -264,   165,  -264,  -264,  -264,  -264,   294,  -264,   -37,  -264,
      -1,  -264,  -264,  -264,  -264,  -264,  -264,  -264,   172,   294,
    -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,   294,   173,
    -264,   726,    40,   762,  -264,   294,  -264,   294,   294,  -264,
     798,   294,   294,   294,   294,   294,   294,   294,   294,  1766,
    -264,   174,   -12,   171,    40,  -264,   175,  -264,   832,  1228,
    1260,    99,  1292,  1324,  1356,  1388,  1420,  1452,  1484,  1516,
    -264,   177,   180,  -264,  -264,  -264,   200,  -264,  -264,  -264,
    -264,   189,  -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,
    -264,  -264,  -264,  -264,  -264,   166,  -264,   294,   294,  -264,
     294,   294,   294,   166,   166,  -264,    41,  1548,  1580,  1612,
    1644,  1676,    41,    41,   190,  -264,  -264,  -264,  -264,  -264,
    -264,  -264
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -264,  -264,  -264,   240,  -264,  -264,  -264,  -264,    72,  -264,
     214,  -264,  -264,  -264,  -264,   206,  -264,   -72,   -79,  -103,
    -264,  -264,  -264,  -264,    47,  -264,  -264,  -264,  -264,  -264,
    -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,  -263,  -264,
    -264,  -264,  -264,  -264,   -34,  -264,  -264,  -264,  -264,  -264,
    -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,
    -264,  -264,  -264,  -264,  -264,  -264,   -70
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
     125,   138,   136,    42,   154,    54,    23,   139,   284,    22,
     140,   176,   177,   178,   179,   180,    43,    58,   266,   267,
     268,    63,    64,    55,    65,   155,    66,   156,   144,   285,
     146,   147,    67,    68,    59,   278,    69,   321,   322,    70,
      71,   269,   157,   158,   159,   160,    24,   161,   270,   271,
     272,   278,   279,    72,    73,    63,    64,   138,   187,   188,
      66,   190,    25,   192,   193,   194,    67,    68,   286,   197,
     302,   303,    20,    70,   364,   201,    26,    74,   204,   205,
     206,   207,   208,   209,    40,   210,    41,    72,    73,    20,
     362,   363,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,    27,   232,   233,   234,   235,   236,    28,   138,    63,
      64,    33,    65,    29,    66,   178,   179,   180,    30,    31,
      67,    68,    32,    34,    69,    35,    38,    70,    71,   182,
     183,    44,   329,   330,   331,    63,    64,    45,    65,    47,
      66,    72,    73,    49,    51,    53,    67,    68,   273,   274,
      69,    56,    40,    70,    71,    57,    60,   280,    61,   184,
     185,   186,   332,   333,   334,    74,   137,    72,    73,   281,
      63,    64,    62,    95,   126,    66,   283,    96,   275,   276,
     277,    67,    68,    97,    98,   100,   101,   127,    70,   290,
      58,    74,   195,   142,   141,   143,   145,   148,   299,   149,
     150,   151,    72,    73,   152,   308,   196,   309,   310,   153,
     189,   312,   313,   314,   315,   316,   317,   318,   319,    63,
      64,   198,    65,   199,    66,   282,   248,   289,   300,   320,
      67,    68,   355,   323,    69,   346,   325,    70,    71,   343,
     355,   355,   344,   365,   349,   371,    48,    39,   260,   365,
     365,    72,    73,   102,    93,   103,   104,   105,   106,   107,
     324,     0,     0,   162,   163,   164,     0,   357,   358,     0,
     359,   360,   361,     0,     0,    74,   247,     0,     0,     0,
       0,     0,   108,     0,   109,   110,   111,   176,   177,   178,
     179,   180,     0,   112,   113,     0,     0,     0,   114,   115,
     116,     0,   102,   117,   103,   104,   105,   106,   107,     0,
       1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,     0,     0,     0,     0,     0,     0,
       0,   108,     0,   109,   110,   111,     0,     0,     0,     0,
       0,     0,   112,   113,     0,     0,     0,   114,   115,     0,
       0,     0,   117,    15,   162,   163,   164,   165,   166,   167,
     168,   169,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,     0,     0,     0,     0,   239,     0,   240,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,     0,
       0,     0,     0,   258,     0,   259,   162,   163,   164,   165,
     166,   167,   168,   169,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,     0,     0,     0,     0,     0,
       0,   202,   162,   163,   164,   165,   166,   167,   168,   169,
     170,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,     0,     0,     0,     0,     0,     0,   203,   162,   163,
     164,   165,   166,   167,   168,   169,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,     0,     0,     0,
       0,     0,     0,   212,   162,   163,   164,   165,   166,   167,
     168,   169,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,     0,     0,     0,     0,     0,     0,   250,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,     0,
       0,     0,     0,     0,     0,   252,   162,   163,   164,   165,
     166,   167,   168,   169,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,     0,     0,     0,     0,     0,
       0,   253,   162,   163,   164,   165,   166,   167,   168,   169,
     170,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,     0,     0,     0,     0,     0,     0,   254,   162,   163,
     164,   165,   166,   167,   168,   169,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,     0,     0,     0,
       0,     0,     0,   255,   162,   163,   164,   165,   166,   167,
     168,   169,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,     0,     0,     0,     0,     0,     0,   257,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,     0,
       0,     0,     0,     0,     0,   301,   162,   163,   164,   165,
     166,   167,   168,   169,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,     0,     0,     0,     0,     0,
       0,   307,   162,   163,   164,   165,   166,   167,   168,   169,
     170,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,     0,     0,     0,     0,   311,   162,   163,   164,   165,
     166,   167,   168,   169,   170,     0,     0,   128,   129,   130,
       0,     0,     0,     0,     0,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,     0,     0,     0,     0,   326,
     131,     0,   132,     0,     0,     0,     0,   133,   134,   135,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,     0,
       0,   181,   162,   163,   164,   165,   166,   167,   168,   169,
     170,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,     0,     0,   237,   162,   163,   164,   165,   166,   167,
     168,   169,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,     0,     0,   238,   162,   163,   164,   165,
     166,   167,   168,   169,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,     0,     0,   243,   162,   163,
     164,   165,   166,   167,   168,   169,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,     0,     0,   244,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,     0,
       0,   245,   162,   163,   164,   165,   166,   167,   168,   169,
     170,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,     0,     0,   261,   162,   163,   164,   165,   166,   167,
     168,   169,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,     0,     0,   262,   162,   163,   164,   165,
     166,   167,   168,   169,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,     0,     0,   263,   162,   163,
     164,   165,   166,   167,   168,   169,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,     0,     0,   264,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,     0,
       0,   265,   162,   163,   164,   165,   166,   167,   168,   169,
     170,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,     0,     0,   327,   162,   163,   164,   165,   166,   167,
     168,   169,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,     0,     0,   328,   162,   163,   164,   165,
     166,   167,   168,   169,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,     0,     0,   335,   162,   163,
     164,   165,   166,   167,   168,   169,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,     0,     0,   336,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,     0,
       0,   337,   162,   163,   164,   165,   166,   167,   168,   169,
     170,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,     0,     0,   338,   162,   163,   164,   165,   166,   167,
     168,   169,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,     0,     0,   339,   162,   163,   164,   165,
     166,   167,   168,   169,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,     0,     0,   340,   162,   163,
     164,   165,   166,   167,   168,   169,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,     0,     0,   341,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,     0,
       0,   342,   162,   163,   164,   165,   166,   167,   168,   169,
     170,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,     0,     0,   366,   162,   163,   164,   165,   166,   167,
     168,   169,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,     0,     0,   367,   162,   163,   164,   165,
     166,   167,   168,   169,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,     0,     0,   368,   162,   163,
     164,   165,   166,   167,   168,   169,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,     0,     0,   369,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,     0,
       0,   370,   162,   163,   164,   165,   166,   167,   168,   169,
     170,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     200,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   162,   163,   164,   165,   166,   167,   168,   169,   170,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   256,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     162,   163,   164,   165,   166,   167,   168,   169,   170,     0,
     162,   163,   164,   165,   166,   167,   168,   169,     0,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   162,
     163,   164,   165,   166,   167,   168,     0,     0,   162,   163,
     164,   165,   166,   167,   168,     0,     0,     0,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   162,   163,   164,
     165,   166,   167,   168,   162,   163,   164,   165,   166,   167,
     168,   162,   163,   164,   165,   166,     0,     0,   173,   174,
     175,   176,   177,   178,   179,   180,   174,   175,   176,   177,
     178,   179,   180,   174,   175,   176,   177,   178,   179,   180
};

static const yytype_int16 yycheck[] =
{
      70,    80,    74,    52,    45,    48,    48,    86,    45,    47,
      89,    58,    59,    60,    61,    62,    65,    52,    43,    44,
      45,    14,    15,    66,    17,    66,    19,    68,    98,    66,
     100,   101,    25,    26,    69,    52,    29,    49,    50,    32,
      33,    66,   112,   113,   114,   115,    49,   117,    73,    74,
      75,    52,    69,    46,    47,    14,    15,   136,   128,   129,
      19,   131,    49,   133,   134,   135,    25,    26,    69,   141,
      30,    31,     0,    32,    33,   145,    49,    70,   148,   149,
     150,   151,   152,   153,    66,   155,    68,    46,    47,    17,
     353,   354,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,    49,   182,   183,   184,   185,   186,    49,   197,    14,
      15,    48,    17,    49,    19,    60,    61,    62,    49,    49,
      25,    26,    49,    49,    29,    47,     0,    32,    33,    43,
      44,    49,    43,    44,    45,    14,    15,    69,    17,    47,
      19,    46,    47,    67,    46,    66,    25,    26,    43,    44,
      29,    70,    66,    32,    33,    47,    49,   246,    65,    73,
      74,    75,    73,    74,    75,    70,    71,    46,    47,   249,
      14,    15,    49,    67,    65,    19,   256,    67,    73,    74,
      75,    25,    26,    48,    68,    68,    68,    47,    32,   269,
      52,    70,    71,    66,    70,    48,    68,    68,   278,    68,
      68,    68,    46,    47,    68,   285,    28,   287,   288,    68,
      65,   291,   292,   293,   294,   295,   296,   297,   298,    14,
      15,    49,    17,    65,    19,    70,    67,    65,    65,    65,
      25,    26,   345,    72,    29,    45,    71,    32,    33,    72,
     353,   354,    72,   356,    65,    65,    42,    17,   211,   362,
     363,    46,    47,    18,    58,    20,    21,    22,    23,    24,
     304,    -1,    -1,    34,    35,    36,    -1,   347,   348,    -1,
     350,   351,   352,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    47,    -1,    49,    50,    51,    58,    59,    60,
      61,    62,    -1,    58,    59,    -1,    -1,    -1,    63,    64,
      65,    -1,    18,    68,    20,    21,    22,    23,    24,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    -1,    49,    50,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    59,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    68,    46,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    -1,    -1,    67,    -1,    69,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    -1,    67,    -1,    69,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    -1,    67,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    -1,    43,    44,    45,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    -1,    67,
      66,    -1,    68,    -1,    -1,    -1,    -1,    73,    74,    75,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    65,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    65,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    65,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    65,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    65,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    65,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    65,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    65,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    65,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    65,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    65,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    65,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    65,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    65,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    65,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    65,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    65,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    65,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    65,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    65,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    65,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    65,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    65,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    65,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    65,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    65,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    34,
      35,    36,    37,    38,    39,    40,    -1,    -1,    34,    35,
      36,    37,    38,    39,    40,    -1,    -1,    -1,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    34,    35,    36,
      37,    38,    39,    40,    34,    35,    36,    37,    38,    39,
      40,    34,    35,    36,    37,    38,    -1,    -1,    55,    56,
      57,    58,    59,    60,    61,    62,    56,    57,    58,    59,
      60,    61,    62,    56,    57,    58,    59,    60,    61,    62
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    46,    77,    78,    79,    80,
      84,    87,    47,    48,    49,    49,    49,    49,    49,    49,
      49,    49,    49,    48,    49,    47,    85,    86,     0,    79,
      66,    68,    52,    65,    49,    69,    88,    47,    86,    67,
      90,    46,    91,    66,    48,    66,    70,    47,    52,    69,
      49,    65,    49,    14,    15,    17,    19,    25,    26,    29,
      32,    33,    46,    47,    70,    81,    82,    83,    84,    92,
      93,    94,    95,    96,   101,   109,   111,   112,   121,   122,
     124,   125,   126,    91,    89,    67,    67,    48,    68,   123,
      68,    68,    18,    20,    21,    22,    23,    24,    47,    49,
      50,    51,    58,    59,    63,    64,    65,    68,    98,   103,
     104,   105,   106,   107,   108,   142,    65,    47,    43,    44,
      45,    66,    68,    73,    74,    75,    93,    71,    94,    94,
      94,    70,    66,    48,   142,    68,   142,   142,    68,    68,
      68,    68,    68,    68,    45,    66,    68,   142,   142,   142,
     142,   142,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    65,    43,    44,    73,    74,    75,   142,   142,    65,
     142,    97,   142,   142,   142,    71,    28,    93,    49,    65,
      52,   142,    69,    69,   142,   142,   142,   142,   142,   142,
     142,    99,    69,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,    65,    65,    67,
      69,   100,   142,    65,    65,    65,   110,    71,    67,   102,
      69,   113,    69,    69,    69,    69,    52,    69,    67,    69,
     100,    65,    65,    65,    65,    65,    43,    44,    45,    66,
      73,    74,    75,    43,    44,    73,    74,    75,    52,    69,
      94,   142,    70,   142,    45,    66,    69,   129,   131,    65,
     142,   127,   128,   130,   134,   136,   132,   133,   135,   142,
      65,    69,    30,    31,   115,   118,   120,    69,   142,   142,
     142,    67,   142,   142,   142,   142,   142,   142,   142,   142,
      65,    49,    50,    72,   120,    71,    67,    65,    65,    43,
      44,    45,    73,    74,    75,    65,    65,    65,    65,    65,
      65,    65,    65,    72,    72,   119,    45,   139,   141,    65,
     137,   138,   140,   116,   117,    95,   114,   142,   142,   142,
     142,   142,   114,   114,    33,    95,    65,    65,    65,    65,
      65,    65
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
        case 8:

/* Line 1464 of yacc.c  */
#line 94 "./CMMComp/c2asm.y"
    {exec_diretivas("#PRNAME",(yyvsp[(2) - (2)]),0);;}
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 95 "./CMMComp/c2asm.y"
    {exec_diretivas("#DIRNAM",(yyvsp[(2) - (2)]),0);;}
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 96 "./CMMComp/c2asm.y"
    {exec_diretivas("#DATYPE",(yyvsp[(2) - (2)]),1);;}
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 97 "./CMMComp/c2asm.y"
    {exec_diretivas("#NUBITS",(yyvsp[(2) - (2)]),0);;}
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 98 "./CMMComp/c2asm.y"
    {exec_diretivas("#NBMANT",(yyvsp[(2) - (2)]),2);;}
    break;

  case 13:

/* Line 1464 of yacc.c  */
#line 99 "./CMMComp/c2asm.y"
    {exec_diretivas("#NBEXPO",(yyvsp[(2) - (2)]),3);;}
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 100 "./CMMComp/c2asm.y"
    {exec_diretivas("#NDSTAC",(yyvsp[(2) - (2)]),0);;}
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 101 "./CMMComp/c2asm.y"
    {exec_diretivas("#SDEPTH",(yyvsp[(2) - (2)]),0);;}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 102 "./CMMComp/c2asm.y"
    {exec_diretivas("#NUIOIN",(yyvsp[(2) - (2)]),4);;}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 103 "./CMMComp/c2asm.y"
    {exec_diretivas("#NUIOOU",(yyvsp[(2) - (2)]),5);;}
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 104 "./CMMComp/c2asm.y"
    {exec_diretivas("#NUGAIN",(yyvsp[(2) - (2)]),0);;}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 105 "./CMMComp/c2asm.y"
    {exec_diretivas("#FFTSIZ",(yyvsp[(2) - (2)]),0);;}
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 106 "./CMMComp/c2asm.y"
    {use_macro(v_name[(yyvsp[(2) - (2)])],1);;}
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 107 "./CMMComp/c2asm.y"
    {end_macro(            );;}
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 111 "./CMMComp/c2asm.y"
    {use_macro(v_name[(yyvsp[(2) - (2)])],0);;}
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 112 "./CMMComp/c2asm.y"
    {end_macro(            );;}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 113 "./CMMComp/c2asm.y"
    {use_inter(            );;}
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 118 "./CMMComp/c2asm.y"
    {declar_arr_1d((yyvsp[(2) - (7)]),(yyvsp[(4) - (7)]),(yyvsp[(6) - (7)])   );;}
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 119 "./CMMComp/c2asm.y"
    {declar_arr_2d((yyvsp[(2) - (10)]),(yyvsp[(4) - (10)]),(yyvsp[(7) - (10)]),(yyvsp[(9) - (10)]));;}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 123 "./CMMComp/c2asm.y"
    {declar_var   ((yyvsp[(1) - (1)])         );;}
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 124 "./CMMComp/c2asm.y"
    {declar_arr_1d((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)])   ,-1);;}
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 125 "./CMMComp/c2asm.y"
    {declar_arr_2d((yyvsp[(1) - (7)]),(yyvsp[(3) - (7)]),(yyvsp[(6) - (7)]),-1);;}
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 129 "./CMMComp/c2asm.y"
    {declar_fun     ((yyvsp[(1) - (3)]),(yyvsp[(2) - (3)]));;}
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 130 "./CMMComp/c2asm.y"
    {declar_firstpar((yyvsp[(5) - (6)])   );;}
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 131 "./CMMComp/c2asm.y"
    {func_ret       ((yyvsp[(2) - (10)])   );;}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 132 "./CMMComp/c2asm.y"
    {declar_fun     ((yyvsp[(1) - (4)]),(yyvsp[(2) - (4)]));;}
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 133 "./CMMComp/c2asm.y"
    {func_ret       ((yyvsp[(2) - (8)])   );;}
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 138 "./CMMComp/c2asm.y"
    {(yyval) = declar_par((yyvsp[(1) - (2)]),(yyvsp[(2) - (2)]));;}
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 139 "./CMMComp/c2asm.y"
    {        set_par((yyvsp[(3) - (3)])   );;}
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 142 "./CMMComp/c2asm.y"
    {declar_ret((yyvsp[(2) - (3)]));;}
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 143 "./CMMComp/c2asm.y"
    {  void_ret(  );;}
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 170 "./CMMComp/c2asm.y"
    {fun_id2  = (yyvsp[(1) - (2)]) ;;}
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 171 "./CMMComp/c2asm.y"
    {vcall     ((yyvsp[(1) - (6)]));;}
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 173 "./CMMComp/c2asm.y"
    {fun_id2  = (yyvsp[(1) - (2)]) ;;}
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 174 "./CMMComp/c2asm.y"
    {(yyval) = fcall((yyvsp[(1) - (5)]));;}
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 181 "./CMMComp/c2asm.y"
    {par_exp    ((yyvsp[(1) - (1)]));;}
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 182 "./CMMComp/c2asm.y"
    {par_listexp((yyvsp[(3) - (3)]));;}
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 186 "./CMMComp/c2asm.y"
    {     exec_out1((yyvsp[(3) - (4)])   );;}
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 187 "./CMMComp/c2asm.y"
    {     exec_out2((yyvsp[(6) - (8)])   );;}
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 188 "./CMMComp/c2asm.y"
    {(yyval) = exec_in  ((yyvsp[(3) - (4)])   );;}
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 189 "./CMMComp/c2asm.y"
    {(yyval) = exec_pst ((yyvsp[(3) - (4)])   );;}
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 190 "./CMMComp/c2asm.y"
    {(yyval) = exec_abs ((yyvsp[(3) - (4)])   );;}
    break;

  case 70:

/* Line 1464 of yacc.c  */
#line 191 "./CMMComp/c2asm.y"
    {(yyval) = exec_sign((yyvsp[(3) - (6)]),(yyvsp[(5) - (6)]));;}
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 192 "./CMMComp/c2asm.y"
    {(yyval) = exec_norm((yyvsp[(3) - (4)])   );;}
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 193 "./CMMComp/c2asm.y"
    {(yyval) = exec_sqrt((yyvsp[(3) - (4)])   );;}
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 197 "./CMMComp/c2asm.y"
    {else_stmt(  );;}
    break;

  case 74:

/* Line 1464 of yacc.c  */
#line 198 "./CMMComp/c2asm.y"
    {if_fim   (  );;}
    break;

  case 75:

/* Line 1464 of yacc.c  */
#line 199 "./CMMComp/c2asm.y"
    {if_stmt  (  );;}
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 200 "./CMMComp/c2asm.y"
    {if_exp   ((yyvsp[(3) - (4)]));;}
    break;

  case 77:

/* Line 1464 of yacc.c  */
#line 204 "./CMMComp/c2asm.y"
    {exec_switch((yyvsp[(3) - (4)]));;}
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 205 "./CMMComp/c2asm.y"
    { end_switch(  );;}
    break;

  case 81:

/* Line 1464 of yacc.c  */
#line 209 "./CMMComp/c2asm.y"
    {switch_break();;}
    break;

  case 82:

/* Line 1464 of yacc.c  */
#line 211 "./CMMComp/c2asm.y"
    {  case_test((yyvsp[(2) - (3)]),1);;}
    break;

  case 84:

/* Line 1464 of yacc.c  */
#line 212 "./CMMComp/c2asm.y"
    {  case_test((yyvsp[(2) - (3)]),2);;}
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 213 "./CMMComp/c2asm.y"
    {defaut_test(    );;}
    break;

  case 91:

/* Line 1464 of yacc.c  */
#line 219 "./CMMComp/c2asm.y"
    {while_stmt  (  );;}
    break;

  case 92:

/* Line 1464 of yacc.c  */
#line 220 "./CMMComp/c2asm.y"
    {while_expp  (  );;}
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 221 "./CMMComp/c2asm.y"
    {while_expexp((yyvsp[(4) - (5)]));;}
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 222 "./CMMComp/c2asm.y"
    {exec_break  (  );;}
    break;

  case 96:

/* Line 1464 of yacc.c  */
#line 227 "./CMMComp/c2asm.y"
    {declar_var((yyvsp[(2) - (5)])); var_set((yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]),0,0,0);;}
    break;

  case 97:

/* Line 1464 of yacc.c  */
#line 228 "./CMMComp/c2asm.y"
    {declar_var((yyvsp[(2) - (5)])); var_set((yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]),0,1,0);;}
    break;

  case 98:

/* Line 1464 of yacc.c  */
#line 229 "./CMMComp/c2asm.y"
    {declar_var((yyvsp[(2) - (5)])); var_set((yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]),0,2,0);;}
    break;

  case 99:

/* Line 1464 of yacc.c  */
#line 230 "./CMMComp/c2asm.y"
    {declar_var((yyvsp[(2) - (5)])); var_set((yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]),0,3,0);;}
    break;

  case 100:

/* Line 1464 of yacc.c  */
#line 231 "./CMMComp/c2asm.y"
    {declar_var((yyvsp[(2) - (5)])); var_set((yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]),0,4,0);;}
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 236 "./CMMComp/c2asm.y"
    {var_set((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]),0,0,0);;}
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 237 "./CMMComp/c2asm.y"
    {var_set((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]),0,1,0);;}
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 238 "./CMMComp/c2asm.y"
    {var_set((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]),0,2,0);;}
    break;

  case 104:

/* Line 1464 of yacc.c  */
#line 239 "./CMMComp/c2asm.y"
    {var_set((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]),0,3,0);;}
    break;

  case 105:

/* Line 1464 of yacc.c  */
#line 240 "./CMMComp/c2asm.y"
    {var_set((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]),0,4,0);;}
    break;

  case 106:

/* Line 1464 of yacc.c  */
#line 242 "./CMMComp/c2asm.y"
    {pplus_assign((yyvsp[(1) - (3)])      );;}
    break;

  case 107:

/* Line 1464 of yacc.c  */
#line 243 "./CMMComp/c2asm.y"
    {aplus_assign((yyvsp[(1) - (6)]),(yyvsp[(3) - (6)])   );;}
    break;

  case 108:

/* Line 1464 of yacc.c  */
#line 244 "./CMMComp/c2asm.y"
    {aplu2_assign((yyvsp[(1) - (9)]),(yyvsp[(3) - (9)]),(yyvsp[(6) - (9)]));;}
    break;

  case 109:

/* Line 1464 of yacc.c  */
#line 246 "./CMMComp/c2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),0    );;}
    break;

  case 110:

/* Line 1464 of yacc.c  */
#line 247 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),1,0,0);;}
    break;

  case 111:

/* Line 1464 of yacc.c  */
#line 248 "./CMMComp/c2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),0    );;}
    break;

  case 112:

/* Line 1464 of yacc.c  */
#line 249 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),1,1,0);;}
    break;

  case 113:

/* Line 1464 of yacc.c  */
#line 250 "./CMMComp/c2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),0    );;}
    break;

  case 114:

/* Line 1464 of yacc.c  */
#line 251 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),1,2,0);;}
    break;

  case 115:

/* Line 1464 of yacc.c  */
#line 252 "./CMMComp/c2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),0    );;}
    break;

  case 116:

/* Line 1464 of yacc.c  */
#line 253 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),1,3,0);;}
    break;

  case 117:

/* Line 1464 of yacc.c  */
#line 254 "./CMMComp/c2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),0    );;}
    break;

  case 118:

/* Line 1464 of yacc.c  */
#line 255 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),1,4,0);;}
    break;

  case 119:

/* Line 1464 of yacc.c  */
#line 257 "./CMMComp/c2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),2    );;}
    break;

  case 120:

/* Line 1464 of yacc.c  */
#line 258 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),1,0,0);;}
    break;

  case 121:

/* Line 1464 of yacc.c  */
#line 259 "./CMMComp/c2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),2    );;}
    break;

  case 122:

/* Line 1464 of yacc.c  */
#line 260 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),1,1,0);;}
    break;

  case 123:

/* Line 1464 of yacc.c  */
#line 261 "./CMMComp/c2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),2    );;}
    break;

  case 124:

/* Line 1464 of yacc.c  */
#line 262 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),1,2,0);;}
    break;

  case 125:

/* Line 1464 of yacc.c  */
#line 263 "./CMMComp/c2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),2    );;}
    break;

  case 126:

/* Line 1464 of yacc.c  */
#line 264 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),1,3,0);;}
    break;

  case 127:

/* Line 1464 of yacc.c  */
#line 265 "./CMMComp/c2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),2    );;}
    break;

  case 128:

/* Line 1464 of yacc.c  */
#line 266 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),1,4,0);;}
    break;

  case 129:

/* Line 1464 of yacc.c  */
#line 268 "./CMMComp/c2asm.y"
    {array_2d_check((yyvsp[(1) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(6) - (8)])    );;}
    break;

  case 130:

/* Line 1464 of yacc.c  */
#line 269 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (11)]),(yyvsp[(10) - (11)]),  2,0,0);;}
    break;

  case 131:

/* Line 1464 of yacc.c  */
#line 270 "./CMMComp/c2asm.y"
    {array_2d_check((yyvsp[(1) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(6) - (8)])    );;}
    break;

  case 132:

/* Line 1464 of yacc.c  */
#line 271 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (11)]),(yyvsp[(10) - (11)]),  2,1,0);;}
    break;

  case 133:

/* Line 1464 of yacc.c  */
#line 272 "./CMMComp/c2asm.y"
    {array_2d_check((yyvsp[(1) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(6) - (8)])    );;}
    break;

  case 134:

/* Line 1464 of yacc.c  */
#line 273 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (11)]),(yyvsp[(10) - (11)]),  2,2,0);;}
    break;

  case 135:

/* Line 1464 of yacc.c  */
#line 274 "./CMMComp/c2asm.y"
    {array_2d_check((yyvsp[(1) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(6) - (8)])    );;}
    break;

  case 136:

/* Line 1464 of yacc.c  */
#line 275 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (11)]),(yyvsp[(10) - (11)]),  2,3,0);;}
    break;

  case 137:

/* Line 1464 of yacc.c  */
#line 276 "./CMMComp/c2asm.y"
    {array_2d_check((yyvsp[(1) - (8)]), (yyvsp[(3) - (8)]), (yyvsp[(6) - (8)])    );;}
    break;

  case 138:

/* Line 1464 of yacc.c  */
#line 277 "./CMMComp/c2asm.y"
    {var_set       ((yyvsp[(1) - (11)]),(yyvsp[(10) - (11)]),  2,4,0);;}
    break;

  case 139:

/* Line 1464 of yacc.c  */
#line 288 "./CMMComp/c2asm.y"
    {(yyval) = num2exp((yyvsp[(1) - (1)]),1);;}
    break;

  case 140:

/* Line 1464 of yacc.c  */
#line 289 "./CMMComp/c2asm.y"
    {(yyval) = num2exp((yyvsp[(1) - (1)]),2);;}
    break;

  case 141:

/* Line 1464 of yacc.c  */
#line 290 "./CMMComp/c2asm.y"
    {(yyval) = num2exp((yyvsp[(1) - (1)]),5);;}
    break;

  case 142:

/* Line 1464 of yacc.c  */
#line 292 "./CMMComp/c2asm.y"
    {(yyval) =      id2exp((yyvsp[(1) - (1)])      );;}
    break;

  case 143:

/* Line 1464 of yacc.c  */
#line 293 "./CMMComp/c2asm.y"
    {(yyval) = array1d2exp((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]), 0);;}
    break;

  case 144:

/* Line 1464 of yacc.c  */
#line 294 "./CMMComp/c2asm.y"
    {(yyval) = array1d2exp((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]), 1);;}
    break;

  case 145:

/* Line 1464 of yacc.c  */
#line 295 "./CMMComp/c2asm.y"
    {(yyval) = array2d2exp((yyvsp[(1) - (7)]),(yyvsp[(3) - (7)]),(yyvsp[(6) - (7)]));;}
    break;

  case 146:

/* Line 1464 of yacc.c  */
#line 297 "./CMMComp/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 147:

/* Line 1464 of yacc.c  */
#line 298 "./CMMComp/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 148:

/* Line 1464 of yacc.c  */
#line 299 "./CMMComp/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 149:

/* Line 1464 of yacc.c  */
#line 300 "./CMMComp/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 150:

/* Line 1464 of yacc.c  */
#line 301 "./CMMComp/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 151:

/* Line 1464 of yacc.c  */
#line 302 "./CMMComp/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 152:

/* Line 1464 of yacc.c  */
#line 303 "./CMMComp/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 153:

/* Line 1464 of yacc.c  */
#line 305 "./CMMComp/c2asm.y"
    {(yyval) = (yyvsp[(2) - (3)]);;}
    break;

  case 154:

/* Line 1464 of yacc.c  */
#line 306 "./CMMComp/c2asm.y"
    {(yyval) = (yyvsp[(2) - (2)]);;}
    break;

  case 155:

/* Line 1464 of yacc.c  */
#line 308 "./CMMComp/c2asm.y"
    {(yyval) =     negacao((yyvsp[(2) - (2)])                );;}
    break;

  case 156:

/* Line 1464 of yacc.c  */
#line 309 "./CMMComp/c2asm.y"
    {(yyval) =    int_oper((yyvsp[(2) - (2)]),0,"!" ,"LINV",1);;}
    break;

  case 157:

/* Line 1464 of yacc.c  */
#line 310 "./CMMComp/c2asm.y"
    {(yyval) =    int_oper((yyvsp[(2) - (2)]),0,"~" , "INV",0);;}
    break;

  case 158:

/* Line 1464 of yacc.c  */
#line 311 "./CMMComp/c2asm.y"
    {(yyval) =   exp_pplus((yyvsp[(1) - (2)])                );;}
    break;

  case 159:

/* Line 1464 of yacc.c  */
#line 312 "./CMMComp/c2asm.y"
    {(yyval) = array_pplus((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)])             );;}
    break;

  case 160:

/* Line 1464 of yacc.c  */
#line 313 "./CMMComp/c2asm.y"
    {(yyval) = array_2plus((yyvsp[(1) - (8)]),(yyvsp[(3) - (8)]),(yyvsp[(6) - (8)])          );;}
    break;

  case 161:

/* Line 1464 of yacc.c  */
#line 315 "./CMMComp/c2asm.y"
    {(yyval) = int_oper((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]),"<<" , "SHL",0);;}
    break;

  case 162:

/* Line 1464 of yacc.c  */
#line 316 "./CMMComp/c2asm.y"
    {(yyval) = int_oper((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]),">>" , "SHR",0);;}
    break;

  case 163:

/* Line 1464 of yacc.c  */
#line 317 "./CMMComp/c2asm.y"
    {(yyval) = int_oper((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]),">>>", "SRS",0);;}
    break;

  case 164:

/* Line 1464 of yacc.c  */
#line 318 "./CMMComp/c2asm.y"
    {(yyval) = int_oper((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "&" , "AND",0);;}
    break;

  case 165:

/* Line 1464 of yacc.c  */
#line 319 "./CMMComp/c2asm.y"
    {(yyval) = int_oper((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "|" ,  "OR",0);;}
    break;

  case 166:

/* Line 1464 of yacc.c  */
#line 320 "./CMMComp/c2asm.y"
    {(yyval) = int_oper((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "^" , "XOR",0);;}
    break;

  case 167:

/* Line 1464 of yacc.c  */
#line 321 "./CMMComp/c2asm.y"
    {(yyval) = int_oper((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "&&","LAND",1);;}
    break;

  case 168:

/* Line 1464 of yacc.c  */
#line 322 "./CMMComp/c2asm.y"
    {(yyval) = int_oper((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), "||", "LOR",1);;}
    break;

  case 169:

/* Line 1464 of yacc.c  */
#line 324 "./CMMComp/c2asm.y"
    {(yyval) = int_oper((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]),  "%", "MOD",0);;}
    break;

  case 170:

/* Line 1464 of yacc.c  */
#line 325 "./CMMComp/c2asm.y"
    {(yyval) = oper_ari((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)])             ,0);;}
    break;

  case 171:

/* Line 1464 of yacc.c  */
#line 326 "./CMMComp/c2asm.y"
    {(yyval) = oper_ari((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)])             ,1);;}
    break;

  case 172:

/* Line 1464 of yacc.c  */
#line 327 "./CMMComp/c2asm.y"
    {(yyval) = oper_ari((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)])             ,2);;}
    break;

  case 173:

/* Line 1464 of yacc.c  */
#line 328 "./CMMComp/c2asm.y"
    {(yyval) = oper_ari((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)])             ,3);;}
    break;

  case 174:

/* Line 1464 of yacc.c  */
#line 330 "./CMMComp/c2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]),0);;}
    break;

  case 175:

/* Line 1464 of yacc.c  */
#line 331 "./CMMComp/c2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]),1);;}
    break;

  case 176:

/* Line 1464 of yacc.c  */
#line 332 "./CMMComp/c2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]),2);;}
    break;

  case 177:

/* Line 1464 of yacc.c  */
#line 333 "./CMMComp/c2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]),3);;}
    break;

  case 178:

/* Line 1464 of yacc.c  */
#line 334 "./CMMComp/c2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]),4);;}
    break;

  case 179:

/* Line 1464 of yacc.c  */
#line 335 "./CMMComp/c2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]),5);;}
    break;



/* Line 1464 of yacc.c  */
#line 3048 "./CMMComp/y.tab.c"
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
#line 337 "./CMMComp/c2asm.y"


int main(int argc, char *argv[])
{
    yyin   = fopen(argv[1], "r");
    f_asm  = fopen(argv[2], "w");

    // da problema com o reset se nao colocar isso se
    // a primeira instrucao for CALL main. Resolver ...
    fprintf(f_asm, "LOAD NULL\n");

    // iniciaiza variaveis de estado
    using_macro  = 0;
    exec_fft_use = 0;
    exec_fft_set = 0;
    prtype       = 0;
    acc_ok       = 0;
    ret_ok       = 0;
    mainok       = 0;

    float_init (); // inicializa variaveis de estado pra float (t2t.c)
	yyparse    (); // aqui a magica acontece!!
	fclose(yyin );
	fclose(f_asm);

	// carrega macros de ponto flutuante pra proc de ponto fixo
	// caso precise (espero que nao)
	if (fgen && prtype == 0) float_gen(argv[2]);
	if (mgen           == 1)  math_gen(argv[2]);

	// checa consistencia de todas as variaveis e funcoes
	check_var(); // (variaveis.c)

	return 0;
}

// erro de sintaxes do bison
void yyerror (char const *s)
{
	fprintf (stderr, "Pô, presta atenção na sintaxe da linha %d!\n", line_num+1);
}

