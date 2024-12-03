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
#line 25 "Sources/c2asm.y"


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
#line 95 "Sources/y.tab.c"

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
     REAL = 280,
     IMAG = 281,
     WHILE = 282,
     IF = 283,
     THEN = 284,
     ELSE = 285,
     SWITCH = 286,
     CASE = 287,
     DEFAULT = 288,
     RETURN = 289,
     BREAK = 290,
     SHIFTL = 291,
     SHIFTR = 292,
     SSHIFTR = 293,
     GREQU = 294,
     LESEQ = 295,
     EQU = 296,
     DIF = 297,
     LAND = 298,
     LOR = 299,
     NORM = 300,
     EQNE = 301,
     PPLUS = 302,
     TYPE = 303,
     ID = 304,
     STRING = 305,
     INUM = 306,
     FNUM = 307,
     CNUM = 308
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
#line 190 "Sources/y.tab.c"

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
#define YYLAST   1284

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  76
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  151
/* YYNRULES -- Number of states.  */
#define YYNSTATES  310

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   308

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    65,     2,     2,     2,    64,    57,     2,
      70,    71,    62,    60,    54,    61,     2,    63,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    74,    67,
      59,    75,    58,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    68,     2,    69,    56,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    72,    55,    73,    66,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53
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
     227,   234,   239,   244,   249,   254,   255,   261,   264,   269,
     270,   279,   281,   284,   288,   289,   295,   296,   302,   303,
     308,   310,   312,   315,   318,   319,   325,   328,   330,   336,
     341,   345,   352,   362,   363,   372,   373,   382,   383,   395,
     397,   399,   401,   403,   408,   413,   421,   423,   425,   427,
     429,   431,   433,   435,   437,   439,   443,   446,   449,   452,
     455,   458,   464,   473,   477,   481,   485,   489,   493,   497,
     501,   505,   509,   513,   517,   521,   525,   529,   533,   537,
     541,   545
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      77,     0,    -1,    78,    -1,    79,    -1,    78,    79,    -1,
      80,    -1,    84,    -1,    87,    -1,     3,    49,    -1,     4,
      50,    -1,     5,    51,    -1,     6,    51,    -1,     7,    51,
      -1,     8,    51,    -1,     9,    51,    -1,    10,    51,    -1,
      11,    51,    -1,    12,    51,    -1,    13,    51,    -1,    16,
      51,    -1,    14,    50,    -1,    15,    -1,    14,    50,    -1,
      15,    -1,    17,    -1,    48,    85,    67,    -1,    48,    49,
      68,    51,    69,    50,    67,    -1,    48,    49,    68,    51,
      69,    68,    51,    69,    50,    67,    -1,    86,    -1,    85,
      54,    86,    -1,    49,    -1,    49,    68,    51,    69,    -1,
      49,    68,    51,    69,    68,    51,    69,    -1,    -1,    -1,
      48,    49,    70,    88,    91,    71,    89,    72,    93,    73,
      -1,    -1,    48,    49,    70,    71,    90,    72,    93,    73,
      -1,    48,    49,    -1,    91,    54,    91,    -1,    34,   132,
      67,    -1,    34,    67,    -1,    94,    -1,    93,    94,    -1,
      72,    93,    73,    -1,    95,    -1,   114,    -1,   126,    -1,
      83,    -1,   127,    -1,   128,    -1,   123,    -1,   111,    -1,
     101,    -1,    96,    -1,    92,    -1,    81,    -1,    82,    -1,
      -1,    49,    70,    97,   100,    71,    67,    -1,    -1,    49,
      70,    99,   100,    71,    -1,    -1,   132,    -1,   100,    54,
     132,    -1,    -1,    19,    70,   132,    54,   102,   132,    71,
      67,    -1,    18,    70,   132,    71,    -1,    21,    70,   132,
      71,    -1,    22,    70,   132,    71,    -1,    23,    70,   132,
      54,   132,    71,    -1,    20,    70,   132,    71,    -1,    24,
      70,   132,    71,    -1,    25,    70,   132,    71,    -1,    26,
      70,   132,    71,    -1,    -1,   113,    94,    30,   112,    94,
      -1,   113,    94,    -1,    28,    70,   132,    71,    -1,    -1,
      31,    70,   132,    71,   115,    72,   122,    73,    -1,    95,
      -1,   116,    95,    -1,   116,    35,    67,    -1,    -1,    32,
      51,    74,   118,   116,    -1,    -1,    32,    52,    74,   119,
     116,    -1,    -1,    33,    74,   121,   116,    -1,   117,    -1,
     120,    -1,   117,   122,    -1,   124,    94,    -1,    -1,    27,
     125,    70,   132,    71,    -1,    35,    67,    -1,    84,    -1,
      48,    49,    75,   132,    67,    -1,    49,    75,   132,    67,
      -1,    49,    47,    67,    -1,    49,    68,   132,    69,    47,
      67,    -1,    49,    68,   132,    69,    68,   132,    69,    47,
      67,    -1,    -1,    49,    68,   132,    69,    75,   129,   132,
      67,    -1,    -1,    49,    68,   132,    71,    75,   130,   132,
      67,    -1,    -1,    49,    68,   132,    69,    68,   132,    69,
      75,   131,   132,    67,    -1,    51,    -1,    52,    -1,    53,
      -1,    49,    -1,    49,    68,   132,    69,    -1,    49,    68,
     132,    71,    -1,    49,    68,   132,    69,    68,   132,    69,
      -1,   103,    -1,   104,    -1,   105,    -1,   106,    -1,   107,
      -1,   108,    -1,   109,    -1,   110,    -1,    98,    -1,    70,
     132,    71,    -1,    60,   132,    -1,    61,   132,    -1,    65,
     132,    -1,    66,   132,    -1,    49,    47,    -1,    49,    68,
     132,    69,    47,    -1,    49,    68,   132,    69,    68,   132,
      69,    47,    -1,   132,    36,   132,    -1,   132,    37,   132,
      -1,   132,    38,   132,    -1,   132,    57,   132,    -1,   132,
      55,   132,    -1,   132,    56,   132,    -1,   132,    43,   132,
      -1,   132,    44,   132,    -1,   132,    64,   132,    -1,   132,
      62,   132,    -1,   132,    63,   132,    -1,   132,    60,   132,
      -1,   132,    61,   132,    -1,   132,    59,   132,    -1,   132,
      58,   132,    -1,   132,    39,   132,    -1,   132,    40,   132,
      -1,   132,    41,   132,    -1,   132,    42,   132,    -1
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
     191,   192,   193,   194,   195,   199,   199,   201,   202,   206,
     206,   209,   210,   211,   213,   213,   214,   214,   215,   215,
     217,   217,   217,   221,   222,   222,   224,   228,   229,   234,
     236,   237,   238,   240,   240,   243,   243,   246,   246,   258,
     259,   260,   262,   263,   264,   265,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   277,   278,   280,   281,   282,
     283,   284,   285,   287,   288,   289,   290,   291,   292,   293,
     294,   296,   297,   298,   299,   300,   302,   303,   304,   305,
     306,   307
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
  "SIGN", "SQRT", "REAL", "IMAG", "WHILE", "IF", "THEN", "ELSE", "SWITCH",
  "CASE", "DEFAULT", "RETURN", "BREAK", "SHIFTL", "SHIFTR", "SSHIFTR",
  "GREQU", "LESEQ", "EQU", "DIF", "LAND", "LOR", "NORM", "EQNE", "PPLUS",
  "TYPE", "ID", "STRING", "INUM", "FNUM", "CNUM", "','", "'|'", "'^'",
  "'&'", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'~'",
  "';'", "'['", "']'", "'('", "')'", "'{'", "'}'", "':'", "'='", "$accept",
  "fim", "prog", "prog_elements", "direct", "use_macro", "end_macro",
  "use_inter", "declar", "id_list", "IID", "funcao", "$@1", "$@2", "$@3",
  "par_list", "return_call", "stmt_list", "stmt_full", "stmt_case",
  "void_call", "$@4", "func_call", "$@5", "exp_list", "std_out", "$@6",
  "std_in", "std_pst", "std_abs", "std_sign", "std_nrm", "std_sqrt",
  "std_real", "std_imag", "if_else_stmt", "$@7", "if_exp", "switch_case",
  "$@8", "case_list", "case", "$@9", "$@10", "default", "$@11", "cases",
  "while_stmt", "while_exp", "$@12", "break", "declar_full", "assignment",
  "$@13", "$@14", "$@15", "exp", 0
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
     305,   306,   307,   308,    44,   124,    94,    38,    62,    60,
      43,    45,    42,    47,    37,    33,   126,    59,    91,    93,
      40,    41,   123,   125,    58,    61
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
     106,   107,   108,   109,   110,   112,   111,   111,   113,   115,
     114,   116,   116,   116,   118,   117,   119,   117,   121,   120,
     122,   122,   122,   123,   125,   124,   126,   127,   127,   128,
     128,   128,   128,   129,   128,   130,   128,   131,   128,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132
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
       6,     4,     4,     4,     4,     0,     5,     2,     4,     0,
       8,     1,     2,     3,     0,     5,     0,     5,     0,     4,
       1,     1,     2,     2,     0,     5,     2,     1,     5,     4,
       3,     6,     9,     0,     8,     0,     8,     0,    11,     1,
       1,     1,     1,     4,     4,     7,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     2,     2,     2,     2,
       2,     5,     8,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3
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
       0,    26,     0,     0,    23,    24,     0,    94,     0,     0,
       0,     0,     0,     0,     0,    56,    57,    48,    97,    55,
       0,    42,    45,    54,    53,    52,     0,    46,    51,     0,
      47,    49,    50,    39,     0,    31,    32,    22,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     112,   109,   110,   111,     0,     0,     0,     0,    41,     0,
     124,   116,   117,   118,   119,   120,   121,   122,   123,     0,
      96,    30,     0,     0,    58,     0,     0,    37,    43,    77,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   130,     0,    60,   126,
     127,   128,   129,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    40,     0,   100,     0,    62,     0,    44,
      75,     0,     0,    27,    65,     0,    78,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,   125,   133,
     134,   135,   148,   149,   150,   151,   139,   140,   137,   138,
     136,   147,   146,   144,   145,   142,   143,   141,     0,     0,
       0,     0,    63,    99,     0,    35,    32,     0,    95,     0,
      67,    71,    68,    69,     0,    72,    73,    74,   113,   114,
       0,    98,     0,     0,   103,   105,     0,     0,    76,     0,
       0,     0,   131,     0,    61,   101,     0,     0,     0,    64,
      59,     0,     0,     0,    90,    91,     0,    70,     0,     0,
       0,     0,    66,     0,     0,    88,    92,    80,   115,     0,
     107,   104,   106,    84,    86,     0,   132,   102,     0,     0,
       0,    81,    89,     0,    85,    87,     0,    82,   108,    83
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    16,    17,    18,    19,    75,    76,    77,    78,    36,
      37,    21,    46,    94,    50,    52,    79,    80,    81,    82,
      83,   187,   120,   207,   231,    84,   237,   121,   122,   123,
     124,   125,   126,   127,   128,    85,   234,    86,    87,   239,
     302,   274,   299,   300,   275,   295,   276,    88,    89,    99,
      90,    91,    92,   267,   268,   298,   232
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -224
static const yytype_int16 yypact[] =
{
     279,   -39,   -36,    13,    19,    37,    42,    64,    65,    68,
      70,    81,     1,  -224,    84,    87,   139,   279,  -224,  -224,
    -224,  -224,  -224,  -224,  -224,  -224,  -224,  -224,  -224,  -224,
    -224,  -224,  -224,  -224,  -224,   -55,    -6,  -224,  -224,  -224,
      89,    75,    94,  -224,    85,  -224,   102,    91,  -224,    -9,
      83,   104,   -51,   109,    99,   117,   -10,  -224,   102,  -224,
     100,  -224,   101,   121,  -224,  -224,   107,  -224,   110,   111,
     191,   105,   124,   -15,   -10,  -224,  -224,  -224,  -224,  -224,
     103,  -224,  -224,  -224,  -224,  -224,   -10,  -224,  -224,   -10,
    -224,  -224,  -224,   128,   112,   123,   135,  -224,   211,   119,
     211,   211,   125,   126,   129,   136,   140,   148,   149,   150,
     -18,  -224,  -224,  -224,   211,   211,   211,   211,  -224,   211,
    -224,  -224,  -224,  -224,  -224,  -224,  -224,  -224,  -224,   868,
    -224,   -49,   127,   211,  -224,   211,   130,  -224,  -224,   175,
    -224,   -10,   172,   157,  1060,   211,   332,   368,   211,   211,
     211,   211,   211,   211,   211,   211,  -224,   211,  -224,    28,
      28,  -224,  -224,   404,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,  -224,   211,  -224,   260,   211,   900,  -224,
    -224,   173,   156,  -224,  -224,   440,  -224,  -224,   476,   512,
     548,   584,  1089,   620,   656,   692,   296,   211,  -224,    63,
      63,    63,  1074,  1074,  1209,  1209,  1157,  1128,  1166,  1195,
    1202,  1074,  1074,    28,    28,  -224,  -224,  -224,   932,   -35,
     151,     0,  1118,  -224,   -10,  -224,  -224,   211,  -224,   155,
    -224,  -224,  -224,  -224,   211,  -224,  -224,  -224,   -31,  -224,
      15,  -224,   163,   211,  -224,  -224,   211,   171,  -224,   728,
      34,   764,  -224,   211,  -224,  -224,   800,   211,   211,  1118,
    -224,   174,    22,   165,    34,  -224,   176,  -224,   834,   -41,
     964,   996,  -224,   179,   180,  -224,  -224,  -224,   200,   181,
    -224,  -224,  -224,  -224,  -224,   114,  -224,  -224,   211,   114,
     114,  -224,     8,  1028,     8,     8,   183,  -224,  -224,  -224
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -224,  -224,  -224,   238,  -224,  -224,  -224,  -224,    72,  -224,
     217,  -224,  -224,  -224,  -224,   207,  -224,   -73,   -78,   979,
    -224,  -224,  -224,  -224,    59,  -224,  -224,  -224,  -224,  -224,
    -224,  -224,  -224,  -224,  -224,  -224,  -224,  -224,  -224,  -224,
    -223,  -224,  -224,  -224,  -224,  -224,    -7,  -224,  -224,  -224,
    -224,  -224,  -224,  -224,  -224,  -224,   -70
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
     129,   136,   138,    58,    63,    64,   289,    65,   139,    66,
      22,   140,   252,    40,    23,    41,   262,    67,    68,    40,
      59,    69,    63,    64,    70,    71,   184,    66,   144,   156,
     146,   147,   132,   253,   290,    67,    68,   263,    72,    73,
     254,    54,    70,   306,   159,   160,   161,   162,    42,   163,
     157,    33,   158,   133,   256,   134,    72,    73,   138,    55,
     135,    43,    74,   186,    24,   188,   272,   273,   191,   256,
      25,   257,    20,   283,   284,   195,   304,   305,   198,   199,
     200,   201,   202,   203,   204,   205,   264,   206,    26,    20,
     180,   181,   182,    27,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   138,   228,    28,    29,    63,    64,    30,
      65,    31,    66,   178,   179,   180,   181,   182,    63,    64,
      67,    68,    32,    66,    69,    34,    35,    70,    71,    38,
      44,    67,    68,    47,    63,    64,    45,    65,    70,    66,
      51,    72,    73,    57,    49,    56,   258,    67,    68,    53,
      60,    69,    72,    73,    70,    71,    61,   259,    62,    95,
      96,    97,   130,   131,   261,    74,   137,    98,    72,    73,
     100,   101,    58,   266,   141,   143,   269,    63,    64,   145,
      65,   142,    66,   278,   185,   148,   149,   280,   281,   150,
      67,    68,    74,   189,    69,   190,   151,    70,    71,   102,
     152,   103,   104,   105,   106,   107,   108,   109,   153,   154,
     155,    72,    73,   192,   193,   236,   255,   260,   303,   102,
     265,   103,   104,   105,   106,   107,   108,   109,   270,   285,
     110,   282,   111,   112,   113,    74,   235,   296,   297,   287,
     309,   114,   115,   293,   294,    39,   116,   117,   118,    48,
     110,   119,   111,   112,   113,    93,   250,   286,     0,     0,
       0,   114,   115,     0,     0,     0,   116,   117,     0,     0,
       0,   119,     1,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,   164,   165,   166,   167,
     168,   169,   170,   171,   172,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,     0,     0,    15,     0,   229,
       0,   230,   164,   165,   166,   167,   168,   169,   170,   171,
     172,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,     0,     0,     0,     0,   248,     0,   249,   164,   165,
     166,   167,   168,   169,   170,   171,   172,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,     0,     0,     0,
       0,     0,     0,   196,   164,   165,   166,   167,   168,   169,
     170,   171,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,     0,     0,     0,     0,     0,     0,   197,
     164,   165,   166,   167,   168,   169,   170,   171,   172,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,     0,
       0,     0,     0,     0,     0,   208,   164,   165,   166,   167,
     168,   169,   170,   171,   172,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,     0,     0,     0,     0,     0,
       0,   238,   164,   165,   166,   167,   168,   169,   170,   171,
     172,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,     0,     0,     0,     0,     0,     0,   240,   164,   165,
     166,   167,   168,   169,   170,   171,   172,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,     0,     0,     0,
       0,     0,     0,   241,   164,   165,   166,   167,   168,   169,
     170,   171,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,     0,     0,     0,     0,     0,     0,   242,
     164,   165,   166,   167,   168,   169,   170,   171,   172,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,     0,
       0,     0,     0,     0,     0,   243,   164,   165,   166,   167,
     168,   169,   170,   171,   172,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,     0,     0,     0,     0,     0,
       0,   245,   164,   165,   166,   167,   168,   169,   170,   171,
     172,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,     0,     0,     0,     0,     0,     0,   246,   164,   165,
     166,   167,   168,   169,   170,   171,   172,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,     0,     0,     0,
       0,     0,     0,   247,   164,   165,   166,   167,   168,   169,
     170,   171,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,     0,     0,     0,     0,     0,     0,   271,
     164,   165,   166,   167,   168,   169,   170,   171,   172,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,     0,
       0,     0,     0,     0,     0,   277,   164,   165,   166,   167,
     168,   169,   170,   171,   172,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,     0,     0,     0,     0,   279,
     164,   165,   166,   167,   168,   169,   170,   171,   172,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,     0,
       0,     0,     0,   288,   164,   165,   166,   167,   168,   169,
     170,   171,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,     0,     0,   183,   164,   165,   166,   167,
     168,   169,   170,   171,   172,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,     0,     0,   233,   164,   165,
     166,   167,   168,   169,   170,   171,   172,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,     0,     0,   251,
     164,   165,   166,   167,   168,   169,   170,   171,   172,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,     0,
       0,   291,   164,   165,   166,   167,   168,   169,   170,   171,
     172,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,     0,     0,   292,   164,   165,   166,   167,   168,   169,
     170,   171,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,     0,     0,   308,   164,   165,   166,   167,
     168,   169,   170,   171,   172,     0,     0,     0,     0,     0,
     164,   165,   166,     0,   194,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   178,   179,   180,   181,   182,     0,
       0,     0,     0,   244,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   164,   165,   166,   167,   168,   169,
     170,   171,   172,     0,   164,   165,   166,   167,   168,   169,
     170,   171,     0,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   164,   165,   166,   167,   168,   169,   170,
       0,     0,   164,   165,   166,   167,   168,   169,   170,     0,
       0,     0,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   164,   165,   166,   167,   168,   169,   170,   164,   165,
     166,   167,   168,   169,   170,   164,   165,   166,   167,   168,
       0,     0,   175,   176,   177,   178,   179,   180,   181,   182,
     176,   177,   178,   179,   180,   181,   182,   176,   177,   178,
     179,   180,   181,   182,   301,     0,     0,     0,   301,   301,
       0,   307,     0,   307,   307
};

static const yytype_int16 yycheck[] =
{
      70,    74,    80,    54,    14,    15,    47,    17,    86,    19,
      49,    89,    47,    68,    50,    70,    47,    27,    28,    68,
      71,    31,    14,    15,    34,    35,    75,    19,    98,    47,
     100,   101,    47,    68,    75,    27,    28,    68,    48,    49,
      75,    50,    34,    35,   114,   115,   116,   117,    54,   119,
      68,    50,    70,    68,    54,    70,    48,    49,   136,    68,
      75,    67,    72,   133,    51,   135,    32,    33,   141,    54,
      51,    71,     0,    51,    52,   145,   299,   300,   148,   149,
     150,   151,   152,   153,   154,   155,    71,   157,    51,    17,
      62,    63,    64,    51,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   191,   184,    51,    51,    14,    15,    51,
      17,    51,    19,    60,    61,    62,    63,    64,    14,    15,
      27,    28,    51,    19,    31,    51,    49,    34,    35,     0,
      51,    27,    28,    49,    14,    15,    71,    17,    34,    19,
      48,    48,    49,    49,    69,    72,   234,    27,    28,    68,
      51,    31,    48,    49,    34,    35,    67,   237,    51,    69,
      69,    50,    67,    49,   244,    72,    73,    70,    48,    49,
      70,    70,    54,   253,    72,    50,   256,    14,    15,    70,
      17,    68,    19,   263,    67,    70,    70,   267,   268,    70,
      27,    28,    72,    73,    31,    30,    70,    34,    35,    18,
      70,    20,    21,    22,    23,    24,    25,    26,    70,    70,
      70,    48,    49,    51,    67,    69,    75,    72,   298,    18,
      67,    20,    21,    22,    23,    24,    25,    26,    67,    74,
      49,    67,    51,    52,    53,    72,    73,    47,    67,    73,
      67,    60,    61,    74,    74,    17,    65,    66,    67,    42,
      49,    70,    51,    52,    53,    58,   207,   274,    -1,    -1,
      -1,    60,    61,    -1,    -1,    -1,    65,    66,    -1,    -1,
      -1,    70,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    -1,    48,    -1,    69,
      -1,    71,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    71,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    -1,    -1,    -1,    69,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    69,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    -1,    -1,    67,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    -1,    67,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    -1,    -1,    67,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    -1,
      -1,    67,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    -1,    -1,    67,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    -1,    -1,    67,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,    -1,
      36,    37,    38,    -1,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    60,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    36,    37,    38,    39,    40,    41,    42,
      -1,    -1,    36,    37,    38,    39,    40,    41,    42,    -1,
      -1,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    36,    37,    38,    39,    40,    41,    42,    36,    37,
      38,    39,    40,    41,    42,    36,    37,    38,    39,    40,
      -1,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      58,    59,    60,    61,    62,    63,    64,    58,    59,    60,
      61,    62,    63,    64,   295,    -1,    -1,    -1,   299,   300,
      -1,   302,    -1,   304,   305
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    48,    77,    78,    79,    80,
      84,    87,    49,    50,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    50,    51,    49,    85,    86,     0,    79,
      68,    70,    54,    67,    51,    71,    88,    49,    86,    69,
      90,    48,    91,    68,    50,    68,    72,    49,    54,    71,
      51,    67,    51,    14,    15,    17,    19,    27,    28,    31,
      34,    35,    48,    49,    72,    81,    82,    83,    84,    92,
      93,    94,    95,    96,   101,   111,   113,   114,   123,   124,
     126,   127,   128,    91,    89,    69,    69,    50,    70,   125,
      70,    70,    18,    20,    21,    22,    23,    24,    25,    26,
      49,    51,    52,    53,    60,    61,    65,    66,    67,    70,
      98,   103,   104,   105,   106,   107,   108,   109,   110,   132,
      67,    49,    47,    68,    70,    75,    93,    73,    94,    94,
      94,    72,    68,    50,   132,    70,   132,   132,    70,    70,
      70,    70,    70,    70,    70,    70,    47,    68,    70,   132,
     132,   132,   132,   132,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    67,    75,    67,   132,    97,   132,    73,
      30,    93,    51,    67,    54,   132,    71,    71,   132,   132,
     132,   132,   132,   132,   132,   132,   132,    99,    71,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,    69,
      71,   100,   132,    67,   112,    73,    69,   102,    71,   115,
      71,    71,    71,    71,    54,    71,    71,    71,    69,    71,
     100,    67,    47,    68,    75,    75,    54,    71,    94,   132,
      72,   132,    47,    68,    71,    67,   132,   129,   130,   132,
      67,    71,    32,    33,   117,   120,   122,    71,   132,    69,
     132,   132,    67,    51,    52,    74,   122,    73,    69,    47,
      75,    67,    67,    74,    74,   121,    47,    67,   131,   118,
     119,    95,   116,   132,   116,   116,    35,    95,    67,    67
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
#line 94 "Sources/c2asm.y"
    {exec_diretivas("#PRNAME",(yyvsp[(2) - (2)]),0);;}
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 95 "Sources/c2asm.y"
    {exec_diretivas("#DIRNAM",(yyvsp[(2) - (2)]),0);;}
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 96 "Sources/c2asm.y"
    {exec_diretivas("#DATYPE",(yyvsp[(2) - (2)]),1);;}
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 97 "Sources/c2asm.y"
    {exec_diretivas("#NUBITS",(yyvsp[(2) - (2)]),0);;}
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 98 "Sources/c2asm.y"
    {exec_diretivas("#NBMANT",(yyvsp[(2) - (2)]),2);;}
    break;

  case 13:

/* Line 1464 of yacc.c  */
#line 99 "Sources/c2asm.y"
    {exec_diretivas("#NBEXPO",(yyvsp[(2) - (2)]),3);;}
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 100 "Sources/c2asm.y"
    {exec_diretivas("#NDSTAC",(yyvsp[(2) - (2)]),0);;}
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 101 "Sources/c2asm.y"
    {exec_diretivas("#SDEPTH",(yyvsp[(2) - (2)]),0);;}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 102 "Sources/c2asm.y"
    {exec_diretivas("#NUIOIN",(yyvsp[(2) - (2)]),4);;}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 103 "Sources/c2asm.y"
    {exec_diretivas("#NUIOOU",(yyvsp[(2) - (2)]),5);;}
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 104 "Sources/c2asm.y"
    {exec_diretivas("#NUGAIN",(yyvsp[(2) - (2)]),0);;}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 105 "Sources/c2asm.y"
    {exec_diretivas("#FFTSIZ",(yyvsp[(2) - (2)]),0);;}
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 106 "Sources/c2asm.y"
    {use_macro(v_name[(yyvsp[(2) - (2)])],1);;}
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 107 "Sources/c2asm.y"
    {end_macro(            );;}
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 111 "Sources/c2asm.y"
    {use_macro(v_name[(yyvsp[(2) - (2)])],0);;}
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 112 "Sources/c2asm.y"
    {end_macro(            );;}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 113 "Sources/c2asm.y"
    {use_inter(            );;}
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 118 "Sources/c2asm.y"
    {declar_arr_1d((yyvsp[(2) - (7)]),(yyvsp[(4) - (7)]),(yyvsp[(6) - (7)])   );;}
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 119 "Sources/c2asm.y"
    {declar_arr_2d((yyvsp[(2) - (10)]),(yyvsp[(4) - (10)]),(yyvsp[(7) - (10)]),(yyvsp[(9) - (10)]));;}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 123 "Sources/c2asm.y"
    {declar_var   ((yyvsp[(1) - (1)])         );;}
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 124 "Sources/c2asm.y"
    {declar_arr_1d((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)])   ,-1);;}
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 125 "Sources/c2asm.y"
    {declar_arr_2d((yyvsp[(1) - (7)]),(yyvsp[(3) - (7)]),(yyvsp[(6) - (7)]),-1);;}
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 129 "Sources/c2asm.y"
    {declar_fun     ((yyvsp[(1) - (3)]),(yyvsp[(2) - (3)]));;}
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 130 "Sources/c2asm.y"
    {declar_firstpar((yyvsp[(5) - (6)])   );;}
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 131 "Sources/c2asm.y"
    {func_ret       ((yyvsp[(2) - (10)])   );;}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 132 "Sources/c2asm.y"
    {declar_fun     ((yyvsp[(1) - (4)]),(yyvsp[(2) - (4)]));;}
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 133 "Sources/c2asm.y"
    {func_ret       ((yyvsp[(2) - (8)])   );;}
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 138 "Sources/c2asm.y"
    {(yyval) = declar_par((yyvsp[(1) - (2)]),(yyvsp[(2) - (2)]));;}
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 139 "Sources/c2asm.y"
    {        set_par((yyvsp[(3) - (3)])   );;}
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 142 "Sources/c2asm.y"
    {declar_ret((yyvsp[(2) - (3)]));;}
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 143 "Sources/c2asm.y"
    {  void_ret(  );;}
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 170 "Sources/c2asm.y"
    {fun_id2  = (yyvsp[(1) - (2)]) ;;}
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 171 "Sources/c2asm.y"
    {vcall     ((yyvsp[(1) - (6)]));;}
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 173 "Sources/c2asm.y"
    {fun_id2  = (yyvsp[(1) - (2)]) ;;}
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 174 "Sources/c2asm.y"
    {(yyval) = fcall((yyvsp[(1) - (5)]));;}
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 181 "Sources/c2asm.y"
    {par_exp    ((yyvsp[(1) - (1)]));;}
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 182 "Sources/c2asm.y"
    {par_listexp((yyvsp[(3) - (3)]));;}
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 186 "Sources/c2asm.y"
    {     exec_out1((yyvsp[(3) - (4)])   );;}
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 187 "Sources/c2asm.y"
    {     exec_out2((yyvsp[(6) - (8)])   );;}
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 188 "Sources/c2asm.y"
    {(yyval) = exec_in  ((yyvsp[(3) - (4)])   );;}
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 189 "Sources/c2asm.y"
    {(yyval) = exec_pst ((yyvsp[(3) - (4)])   );;}
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 190 "Sources/c2asm.y"
    {(yyval) = exec_abs ((yyvsp[(3) - (4)])   );;}
    break;

  case 70:

/* Line 1464 of yacc.c  */
#line 191 "Sources/c2asm.y"
    {(yyval) = exec_sign((yyvsp[(3) - (6)]),(yyvsp[(5) - (6)]));;}
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 192 "Sources/c2asm.y"
    {(yyval) = exec_norm((yyvsp[(3) - (4)])   );;}
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 193 "Sources/c2asm.y"
    {(yyval) = exec_sqrt((yyvsp[(3) - (4)])   );;}
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 194 "Sources/c2asm.y"
    {(yyval) = exec_real((yyvsp[(3) - (4)])   );;}
    break;

  case 74:

/* Line 1464 of yacc.c  */
#line 195 "Sources/c2asm.y"
    {(yyval) = exec_imag((yyvsp[(3) - (4)])   );;}
    break;

  case 75:

/* Line 1464 of yacc.c  */
#line 199 "Sources/c2asm.y"
    {else_stmt(  );;}
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 200 "Sources/c2asm.y"
    {if_fim   (  );;}
    break;

  case 77:

/* Line 1464 of yacc.c  */
#line 201 "Sources/c2asm.y"
    {if_stmt  (  );;}
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 202 "Sources/c2asm.y"
    {if_exp   ((yyvsp[(3) - (4)]));;}
    break;

  case 79:

/* Line 1464 of yacc.c  */
#line 206 "Sources/c2asm.y"
    {exec_switch((yyvsp[(3) - (4)]));;}
    break;

  case 80:

/* Line 1464 of yacc.c  */
#line 207 "Sources/c2asm.y"
    { end_switch(  );;}
    break;

  case 83:

/* Line 1464 of yacc.c  */
#line 211 "Sources/c2asm.y"
    {switch_break();;}
    break;

  case 84:

/* Line 1464 of yacc.c  */
#line 213 "Sources/c2asm.y"
    {  case_test((yyvsp[(2) - (3)]),1);;}
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 214 "Sources/c2asm.y"
    {  case_test((yyvsp[(2) - (3)]),2);;}
    break;

  case 88:

/* Line 1464 of yacc.c  */
#line 215 "Sources/c2asm.y"
    {defaut_test(    );;}
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 221 "Sources/c2asm.y"
    {while_stmt  (  );;}
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 222 "Sources/c2asm.y"
    {while_expp  (  );;}
    break;

  case 95:

/* Line 1464 of yacc.c  */
#line 223 "Sources/c2asm.y"
    {while_expexp((yyvsp[(4) - (5)]));;}
    break;

  case 96:

/* Line 1464 of yacc.c  */
#line 224 "Sources/c2asm.y"
    {exec_break  (  );;}
    break;

  case 98:

/* Line 1464 of yacc.c  */
#line 229 "Sources/c2asm.y"
    {declar_var((yyvsp[(2) - (5)])); var_set((yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]),0,0,1);;}
    break;

  case 99:

/* Line 1464 of yacc.c  */
#line 234 "Sources/c2asm.y"
    {var_set((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]),0,0,1);;}
    break;

  case 100:

/* Line 1464 of yacc.c  */
#line 236 "Sources/c2asm.y"
    {pplus_assign((yyvsp[(1) - (3)])      );;}
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 237 "Sources/c2asm.y"
    {aplus_assign((yyvsp[(1) - (6)]),(yyvsp[(3) - (6)])   );;}
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 238 "Sources/c2asm.y"
    {aplu2_assign((yyvsp[(1) - (9)]),(yyvsp[(3) - (9)]),(yyvsp[(6) - (9)]));;}
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 240 "Sources/c2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),0    );;}
    break;

  case 104:

/* Line 1464 of yacc.c  */
#line 241 "Sources/c2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),1,0,1);;}
    break;

  case 105:

/* Line 1464 of yacc.c  */
#line 243 "Sources/c2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),2    );;}
    break;

  case 106:

/* Line 1464 of yacc.c  */
#line 244 "Sources/c2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),1,0,1);;}
    break;

  case 107:

/* Line 1464 of yacc.c  */
#line 246 "Sources/c2asm.y"
    {array_2d_check((yyvsp[(1) - (8)]), (yyvsp[(3) - (8)]),(yyvsp[(6) - (8)])    );;}
    break;

  case 108:

/* Line 1464 of yacc.c  */
#line 247 "Sources/c2asm.y"
    {var_set       ((yyvsp[(1) - (11)]),(yyvsp[(10) - (11)]), 2,0,1);;}
    break;

  case 109:

/* Line 1464 of yacc.c  */
#line 258 "Sources/c2asm.y"
    {(yyval) = num2exp((yyvsp[(1) - (1)]),1);;}
    break;

  case 110:

/* Line 1464 of yacc.c  */
#line 259 "Sources/c2asm.y"
    {(yyval) = num2exp((yyvsp[(1) - (1)]),2);;}
    break;

  case 111:

/* Line 1464 of yacc.c  */
#line 260 "Sources/c2asm.y"
    {(yyval) = num2exp((yyvsp[(1) - (1)]),5);;}
    break;

  case 112:

/* Line 1464 of yacc.c  */
#line 262 "Sources/c2asm.y"
    {(yyval) =      id2exp((yyvsp[(1) - (1)])      );;}
    break;

  case 113:

/* Line 1464 of yacc.c  */
#line 263 "Sources/c2asm.y"
    {(yyval) = array1d2exp((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]), 0);;}
    break;

  case 114:

/* Line 1464 of yacc.c  */
#line 264 "Sources/c2asm.y"
    {(yyval) = array1d2exp((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]), 1);;}
    break;

  case 115:

/* Line 1464 of yacc.c  */
#line 265 "Sources/c2asm.y"
    {(yyval) = array2d2exp((yyvsp[(1) - (7)]),(yyvsp[(3) - (7)]),(yyvsp[(6) - (7)]));;}
    break;

  case 116:

/* Line 1464 of yacc.c  */
#line 267 "Sources/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 117:

/* Line 1464 of yacc.c  */
#line 268 "Sources/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 118:

/* Line 1464 of yacc.c  */
#line 269 "Sources/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 119:

/* Line 1464 of yacc.c  */
#line 270 "Sources/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 120:

/* Line 1464 of yacc.c  */
#line 271 "Sources/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 121:

/* Line 1464 of yacc.c  */
#line 272 "Sources/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 122:

/* Line 1464 of yacc.c  */
#line 273 "Sources/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 123:

/* Line 1464 of yacc.c  */
#line 274 "Sources/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 124:

/* Line 1464 of yacc.c  */
#line 275 "Sources/c2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 125:

/* Line 1464 of yacc.c  */
#line 277 "Sources/c2asm.y"
    {(yyval) = (yyvsp[(2) - (3)]);;}
    break;

  case 126:

/* Line 1464 of yacc.c  */
#line 278 "Sources/c2asm.y"
    {(yyval) = (yyvsp[(2) - (2)]);;}
    break;

  case 127:

/* Line 1464 of yacc.c  */
#line 280 "Sources/c2asm.y"
    {(yyval) =     negacao((yyvsp[(2) - (2)])      );;}
    break;

  case 128:

/* Line 1464 of yacc.c  */
#line 281 "Sources/c2asm.y"
    {(yyval) =    oper_int((yyvsp[(2) - (2)]), 0, 0);;}
    break;

  case 129:

/* Line 1464 of yacc.c  */
#line 282 "Sources/c2asm.y"
    {(yyval) =    oper_int((yyvsp[(2) - (2)]), 0, 1);;}
    break;

  case 130:

/* Line 1464 of yacc.c  */
#line 283 "Sources/c2asm.y"
    {(yyval) =   exp_pplus((yyvsp[(1) - (2)])      );;}
    break;

  case 131:

/* Line 1464 of yacc.c  */
#line 284 "Sources/c2asm.y"
    {(yyval) = array_pplus((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)])   );;}
    break;

  case 132:

/* Line 1464 of yacc.c  */
#line 285 "Sources/c2asm.y"
    {(yyval) = array_2plus((yyvsp[(1) - (8)]),(yyvsp[(3) - (8)]),(yyvsp[(6) - (8)]));;}
    break;

  case 133:

/* Line 1464 of yacc.c  */
#line 287 "Sources/c2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 2);;}
    break;

  case 134:

/* Line 1464 of yacc.c  */
#line 288 "Sources/c2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 3);;}
    break;

  case 135:

/* Line 1464 of yacc.c  */
#line 289 "Sources/c2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 4);;}
    break;

  case 136:

/* Line 1464 of yacc.c  */
#line 290 "Sources/c2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 5);;}
    break;

  case 137:

/* Line 1464 of yacc.c  */
#line 291 "Sources/c2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 6);;}
    break;

  case 138:

/* Line 1464 of yacc.c  */
#line 292 "Sources/c2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 7);;}
    break;

  case 139:

/* Line 1464 of yacc.c  */
#line 293 "Sources/c2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 8);;}
    break;

  case 140:

/* Line 1464 of yacc.c  */
#line 294 "Sources/c2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 9);;}
    break;

  case 141:

/* Line 1464 of yacc.c  */
#line 296 "Sources/c2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]),10);;}
    break;

  case 142:

/* Line 1464 of yacc.c  */
#line 297 "Sources/c2asm.y"
    {(yyval) = oper_ari((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 0);;}
    break;

  case 143:

/* Line 1464 of yacc.c  */
#line 298 "Sources/c2asm.y"
    {(yyval) = oper_ari((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 1);;}
    break;

  case 144:

/* Line 1464 of yacc.c  */
#line 299 "Sources/c2asm.y"
    {(yyval) = oper_ari((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 2);;}
    break;

  case 145:

/* Line 1464 of yacc.c  */
#line 300 "Sources/c2asm.y"
    {(yyval) = oper_ari((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 3);;}
    break;

  case 146:

/* Line 1464 of yacc.c  */
#line 302 "Sources/c2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 0);;}
    break;

  case 147:

/* Line 1464 of yacc.c  */
#line 303 "Sources/c2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 1);;}
    break;

  case 148:

/* Line 1464 of yacc.c  */
#line 304 "Sources/c2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 2);;}
    break;

  case 149:

/* Line 1464 of yacc.c  */
#line 305 "Sources/c2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 3);;}
    break;

  case 150:

/* Line 1464 of yacc.c  */
#line 306 "Sources/c2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 4);;}
    break;

  case 151:

/* Line 1464 of yacc.c  */
#line 307 "Sources/c2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 5);;}
    break;



/* Line 1464 of yacc.c  */
#line 2679 "Sources/y.tab.c"
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
#line 309 "Sources/c2asm.y"


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

