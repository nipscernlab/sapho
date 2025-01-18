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
#line 27 "Sources/cmm2asm.y"


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
#line 95 "cmm2asm.tab.c"

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
     ATAN = 282,
     FASE = 283,
     WHILE = 284,
     IF = 285,
     THEN = 286,
     ELSE = 287,
     SWITCH = 288,
     CASE = 289,
     DEFAULT = 290,
     RETURN = 291,
     BREAK = 292,
     SHIFTL = 293,
     SHIFTR = 294,
     SSHIFTR = 295,
     GREQU = 296,
     LESEQ = 297,
     EQU = 298,
     DIF = 299,
     LAND = 300,
     LOR = 301,
     NORM = 302,
     EQNE = 303,
     PPLUS = 304,
     TYPE = 305,
     ID = 306,
     STRING = 307,
     INUM = 308,
     FNUM = 309,
     CNUM = 310
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
#line 192 "cmm2asm.tab.c"

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
#define YYFINAL  39
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1406

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  78
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
#define YYNRULES  155
/* YYNRULES -- Number of states.  */
#define YYNSTATES  320

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   310

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    67,     2,     2,     2,    66,    59,     2,
      72,    73,    64,    62,    56,    63,     2,    65,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    76,    69,
      61,    77,    60,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    70,     2,    71,    58,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    74,    57,    75,    68,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
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
     227,   234,   239,   244,   249,   254,   259,   264,   265,   271,
     274,   279,   280,   289,   291,   294,   298,   299,   305,   306,
     312,   313,   318,   320,   322,   325,   328,   329,   335,   338,
     340,   346,   351,   355,   362,   372,   373,   382,   383,   392,
     393,   405,   407,   409,   411,   413,   418,   423,   431,   433,
     435,   437,   439,   441,   443,   445,   447,   449,   451,   453,
     457,   460,   463,   466,   469,   472,   478,   487,   491,   495,
     499,   503,   507,   511,   515,   519,   523,   527,   531,   535,
     539,   543,   547,   551,   555,   559
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      79,     0,    -1,    80,    -1,    81,    -1,    80,    81,    -1,
      82,    -1,   131,    -1,    89,    -1,     3,    51,    -1,     4,
      52,    -1,     5,    53,    -1,     6,    53,    -1,     7,    53,
      -1,     8,    53,    -1,     9,    53,    -1,    10,    53,    -1,
      11,    53,    -1,    12,    53,    -1,    13,    53,    -1,    16,
      53,    -1,    14,    52,    -1,    15,    -1,    14,    52,    -1,
      15,    -1,    17,    -1,    50,    87,    69,    -1,    50,    51,
      70,    53,    71,    52,    69,    -1,    50,    51,    70,    53,
      71,    70,    53,    71,    52,    69,    -1,    88,    -1,    87,
      56,    88,    -1,    51,    -1,    51,    70,    53,    71,    -1,
      51,    70,    53,    71,    70,    53,    71,    -1,    -1,    -1,
      50,    51,    72,    90,    93,    73,    91,    74,    95,    75,
      -1,    -1,    50,    51,    72,    73,    92,    74,    95,    75,
      -1,    50,    51,    -1,    93,    56,    93,    -1,    36,   136,
      69,    -1,    36,    69,    -1,    96,    -1,    95,    96,    -1,
      74,    95,    75,    -1,    97,    -1,   118,    -1,   130,    -1,
      85,    -1,   131,    -1,   132,    -1,   127,    -1,   115,    -1,
     103,    -1,    98,    -1,    94,    -1,    83,    -1,    84,    -1,
      -1,    51,    72,    99,   102,    73,    69,    -1,    -1,    51,
      72,   101,   102,    73,    -1,    -1,   136,    -1,   102,    56,
     136,    -1,    -1,    19,    72,   136,    56,   104,   136,    73,
      69,    -1,    18,    72,   136,    73,    -1,    21,    72,   136,
      73,    -1,    22,    72,   136,    73,    -1,    23,    72,   136,
      56,   136,    73,    -1,    20,    72,   136,    73,    -1,    24,
      72,   136,    73,    -1,    25,    72,   136,    73,    -1,    26,
      72,   136,    73,    -1,    27,    72,   136,    73,    -1,    28,
      72,   136,    73,    -1,    -1,   117,    96,    32,   116,    96,
      -1,   117,    96,    -1,    30,    72,   136,    73,    -1,    -1,
      33,    72,   136,    73,   119,    74,   126,    75,    -1,    97,
      -1,   120,    97,    -1,   120,    37,    69,    -1,    -1,    34,
      53,    76,   122,   120,    -1,    -1,    34,    54,    76,   123,
     120,    -1,    -1,    35,    76,   125,   120,    -1,   121,    -1,
     124,    -1,   121,   126,    -1,   128,    96,    -1,    -1,    29,
     129,    72,   136,    73,    -1,    37,    69,    -1,    86,    -1,
      50,    51,    77,   136,    69,    -1,    51,    77,   136,    69,
      -1,    51,    49,    69,    -1,    51,    70,   136,    71,    49,
      69,    -1,    51,    70,   136,    71,    70,   136,    71,    49,
      69,    -1,    -1,    51,    70,   136,    71,    77,   133,   136,
      69,    -1,    -1,    51,    70,   136,    73,    77,   134,   136,
      69,    -1,    -1,    51,    70,   136,    71,    70,   136,    71,
      77,   135,   136,    69,    -1,    53,    -1,    54,    -1,    55,
      -1,    51,    -1,    51,    70,   136,    71,    -1,    51,    70,
     136,    73,    -1,    51,    70,   136,    71,    70,   136,    71,
      -1,   105,    -1,   106,    -1,   107,    -1,   108,    -1,   109,
      -1,   110,    -1,   111,    -1,   112,    -1,   113,    -1,   114,
      -1,   100,    -1,    72,   136,    73,    -1,    62,   136,    -1,
      63,   136,    -1,    67,   136,    -1,    68,   136,    -1,    51,
      49,    -1,    51,    70,   136,    71,    49,    -1,    51,    70,
     136,    71,    70,   136,    71,    49,    -1,   136,    38,   136,
      -1,   136,    39,   136,    -1,   136,    40,   136,    -1,   136,
      59,   136,    -1,   136,    57,   136,    -1,   136,    58,   136,
      -1,   136,    45,   136,    -1,   136,    46,   136,    -1,   136,
      66,   136,    -1,   136,    64,   136,    -1,   136,    65,   136,
      -1,   136,    62,   136,    -1,   136,    63,   136,    -1,   136,
      61,   136,    -1,   136,    60,   136,    -1,   136,    41,   136,
      -1,   136,    42,   136,    -1,   136,    43,   136,    -1,   136,
      44,   136,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    89,    89,    90,    90,    92,    92,    92,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   113,   114,   115,   119,   120,   121,   123,   123,
     125,   126,   127,   131,   132,   131,   134,   134,   140,   141,
     144,   145,   149,   149,   152,   153,   154,   155,   156,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   172,   172,
     175,   175,   182,   183,   184,   188,   188,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   203,   203,   205,
     206,   210,   210,   213,   214,   215,   217,   217,   218,   218,
     219,   219,   221,   221,   221,   225,   226,   226,   228,   232,
     233,   238,   240,   241,   242,   244,   244,   247,   247,   250,
     250,   262,   263,   264,   266,   268,   269,   270,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   284,
     285,   287,   288,   289,   290,   291,   292,   294,   295,   296,
     297,   298,   299,   300,   301,   303,   304,   305,   306,   307,
     309,   310,   311,   312,   313,   314
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
  "SIGN", "SQRT", "REAL", "IMAG", "ATAN", "FASE", "WHILE", "IF", "THEN",
  "ELSE", "SWITCH", "CASE", "DEFAULT", "RETURN", "BREAK", "SHIFTL",
  "SHIFTR", "SSHIFTR", "GREQU", "LESEQ", "EQU", "DIF", "LAND", "LOR",
  "NORM", "EQNE", "PPLUS", "TYPE", "ID", "STRING", "INUM", "FNUM", "CNUM",
  "','", "'|'", "'^'", "'&'", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'",
  "'%'", "'!'", "'~'", "';'", "'['", "']'", "'('", "')'", "'{'", "'}'",
  "':'", "'='", "$accept", "fim", "prog", "prog_elements", "direct",
  "use_macro", "end_macro", "use_inter", "declar", "id_list", "IID",
  "funcao", "$@1", "$@2", "$@3", "par_list", "return_call", "stmt_list",
  "stmt_full", "stmt_case", "void_call", "$@4", "func_call", "$@5",
  "exp_list", "std_out", "$@6", "std_in", "std_pst", "std_abs", "std_sign",
  "std_nrm", "std_sqrt", "std_real", "std_imag", "std_atan", "std_fase",
  "if_else_stmt", "$@7", "if_exp", "switch_case", "$@8", "case_list",
  "case", "$@9", "$@10", "default", "$@11", "cases", "while_stmt",
  "while_exp", "$@12", "break", "declar_full", "assignment", "$@13",
  "$@14", "$@15", "exp", 0
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
     305,   306,   307,   308,   309,   310,    44,   124,    94,    38,
      62,    60,    43,    45,    42,    47,    37,    33,   126,    59,
      91,    93,    40,    41,   123,   125,    58,    61
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    78,    79,    80,    80,    81,    81,    81,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    83,    84,    85,    86,    86,    86,    87,    87,
      88,    88,    88,    90,    91,    89,    92,    89,    93,    93,
      94,    94,    95,    95,    96,    96,    96,    96,    96,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    99,    98,
     101,   100,   102,   102,   102,   104,   103,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   116,   115,   115,
     117,   119,   118,   120,   120,   120,   122,   121,   123,   121,
     125,   124,   126,   126,   126,   127,   129,   128,   130,   131,
     131,   132,   132,   132,   132,   133,   132,   134,   132,   135,
     132,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136
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
       6,     4,     4,     4,     4,     4,     4,     0,     5,     2,
       4,     0,     8,     1,     2,     3,     0,     5,     0,     5,
       0,     4,     1,     1,     2,     2,     0,     5,     2,     1,
       5,     4,     3,     6,     9,     0,     8,     0,     8,     0,
      11,     1,     1,     1,     1,     4,     4,     7,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     2,     2,     2,     5,     8,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    21,     0,     0,     0,     2,     3,     5,
      99,     7,     6,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    20,    19,    30,     0,    28,     1,
       4,     0,    33,     0,     0,    25,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   114,
     111,   112,   113,     0,     0,     0,     0,     0,   128,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,     0,
      30,    29,    31,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   134,     0,    60,   130,
     131,   132,   133,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,     0,     0,    38,     0,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    62,   129,   137,   138,   139,   152,   153,   154,
     155,   143,   144,   141,   142,   140,   151,   150,   148,   149,
     146,   147,   145,     0,    26,     0,     0,    23,    24,     0,
      96,     0,     0,     0,     0,     0,     0,     0,    56,    57,
      48,    55,     0,    42,    45,    54,    53,    52,     0,    46,
      51,     0,    47,    49,    50,    39,     0,    67,    71,    68,
      69,     0,    72,    73,    74,    75,    76,   115,   116,     0,
      63,    31,    32,    22,     0,     0,     0,     0,    41,     0,
      98,    30,     0,     0,    58,     0,     0,    37,    43,    79,
      95,     0,     0,   135,     0,     0,    61,     0,     0,     0,
       0,     0,     0,    40,   102,     0,    62,     0,    44,    77,
       0,    70,     0,    64,     0,    27,    65,     0,    80,    81,
       0,     0,     0,   101,     0,    35,   117,    32,     0,    97,
       0,     0,     0,   105,   107,     0,    78,   136,     0,     0,
     103,     0,     0,     0,    59,     0,     0,     0,    92,    93,
       0,     0,     0,     0,    66,     0,     0,    90,    94,    82,
       0,   109,   106,   108,    86,    88,     0,   104,     0,     0,
       0,    83,    91,     0,    87,    89,     0,    84,   110,    85
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    16,    17,    18,    19,   178,   179,   180,    20,    37,
      38,    21,    48,   196,    83,    85,   181,   182,   183,   184,
     185,   246,    68,   142,   209,   186,   268,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,   187,   264,   188,
     189,   270,   312,   288,   309,   310,   289,   306,   290,   190,
     191,   215,   192,   193,   194,   282,   283,   308,   210
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -177
static const yytype_int16 yypact[] =
{
       3,     9,   -15,    28,    31,    37,    40,    42,    73,    76,
      86,    88,   -12,  -177,    96,    48,   150,     3,  -177,  -177,
    -177,  -177,  -177,  -177,  -177,  -177,  -177,  -177,  -177,  -177,
    -177,  -177,  -177,  -177,  -177,  -177,   -38,    46,  -177,  -177,
    -177,   102,    83,   284,   118,  -177,    93,  -177,   109,    98,
     115,   121,   122,   123,   124,   128,   129,   131,   132,   -14,
    -177,  -177,  -177,   284,   284,   284,   284,   284,  -177,  -177,
    -177,  -177,  -177,  -177,  -177,  -177,  -177,  -177,  -177,   999,
     111,  -177,   -11,   116,   134,   -31,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,  -177,   284,  -177,    57,
      57,  -177,  -177,   391,   284,   284,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   284,  -177,   119,   120,   154,   197,  -177,   109,
    -177,   427,   463,   499,   535,  1191,   571,   607,   643,   679,
     715,   319,   284,  -177,   113,   113,   113,   -36,   -36,  1340,
    1340,  1288,  1259,  1297,  1326,  1333,   -36,   -36,    57,    57,
    -177,  -177,  -177,   137,  -177,   138,   158,  -177,  -177,   141,
    -177,   143,   145,   231,   149,   170,    39,   197,  -177,  -177,
    -177,  -177,    68,  -177,  -177,  -177,  -177,  -177,   197,  -177,
    -177,   197,  -177,  -177,  -177,   166,   157,  -177,  -177,  -177,
    -177,   284,  -177,  -177,  -177,  -177,  -177,   -13,  -177,    44,
    1249,   153,   172,  -177,   284,   156,   284,   284,  -177,  1031,
    -177,   -39,   163,   284,  -177,   284,    77,  -177,  -177,   203,
    -177,   197,   751,  -177,   284,   284,  -177,   183,   168,  1220,
     284,   787,   823,  -177,  -177,   355,   284,  1063,  -177,  -177,
     169,  -177,   931,  1249,   167,  -177,  -177,   859,  -177,  -177,
      63,   164,    47,  -177,   197,  -177,   193,  -177,   284,  -177,
     171,   177,   284,  -177,  -177,   181,  -177,  -177,   895,   100,
    -177,   965,   284,   284,  -177,   191,    84,   185,   100,  -177,
     187,   -44,  1095,  1127,  -177,   188,   190,  -177,  -177,  -177,
     194,  -177,  -177,  -177,  -177,  -177,   304,  -177,   284,   304,
     304,  -177,   117,  1159,   117,   117,   198,  -177,  -177,  -177
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -177,  -177,  -177,   251,  -177,  -177,  -177,  -177,  -177,  -177,
     225,  -177,  -177,  -177,  -177,   144,  -177,  -176,  -102,  -149,
    -177,  -177,  -177,  -177,    24,  -177,  -177,  -177,  -177,  -177,
    -177,  -177,  -177,  -177,  -177,  -177,  -177,  -177,  -177,  -177,
    -177,  -177,  -165,  -177,  -177,  -177,  -177,  -177,   -16,  -177,
    -177,  -177,  -177,   108,  -177,  -177,  -177,  -177,   -43
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      79,   226,   104,   105,   106,   300,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      99,   100,   101,   102,   103,   129,   118,   119,   120,   121,
     122,    41,    41,   301,    42,    96,   233,    24,    43,    43,
      34,   125,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,    15,   141,   250,    97,   234,    98,   126,
      23,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     228,    25,   166,   167,    26,   168,   229,   169,   222,   230,
      27,   166,   167,    28,   168,    29,   169,   170,   171,    36,
     235,   172,    44,   235,   173,   174,   170,   171,    22,   223,
     172,   224,   271,   173,   174,    45,   225,   236,   175,   176,
     275,   120,   121,   122,   228,    22,    30,   175,   176,    31,
     219,   166,   167,   272,   286,   287,   169,   295,   296,    32,
     273,    33,   177,   227,   314,   315,   170,   171,   228,    35,
      39,   177,   248,   173,   316,    46,    47,   311,   232,    84,
     311,   311,   276,   317,    82,   317,   317,   175,   176,    80,
      86,   239,   163,   241,   242,   118,   119,   120,   121,   122,
     245,   124,   247,   166,   167,   128,   168,    87,   169,   164,
     127,   252,   253,    88,    89,    90,    91,   257,   170,   171,
      92,    93,   172,    94,    95,   173,   174,   165,   211,   212,
     213,   166,   167,   214,   168,   216,   169,   217,   220,   175,
     176,   221,   129,   237,   238,   278,   170,   171,   240,   281,
     172,   231,   244,   173,   174,   249,   254,   255,   267,   292,
     293,   274,   277,   177,   265,   279,   280,   175,   176,    49,
     284,    50,    51,    52,    53,    54,    55,    56,    57,    58,
     294,   297,   299,   307,   304,   313,   305,   319,    40,    81,
     262,   177,   298,   195,     0,     0,     0,     0,     0,     0,
       0,     0,    59,     0,    60,    61,    62,     0,     0,     0,
       0,     0,     0,    63,    64,     0,     0,     0,    65,    66,
     218,     0,    49,    67,    50,    51,    52,    53,    54,    55,
      56,    57,    58,     0,     0,     0,     0,     0,   166,   167,
       0,     0,     0,   169,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   170,   171,    59,     0,    60,    61,    62,
     173,     0,     0,     0,     0,     0,    63,    64,     0,     0,
       0,    65,    66,     0,   175,   176,    67,   104,   105,   106,
     107,   108,   109,   110,   111,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,     0,     0,     0,     0,
     207,     0,   208,   104,   105,   106,   107,   108,   109,   110,
     111,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,     0,     0,     0,     0,   260,     0,   261,   104,
     105,   106,   107,   108,   109,   110,   111,   112,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,     0,     0,
       0,     0,     0,     0,   143,   104,   105,   106,   107,   108,
     109,   110,   111,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,     0,     0,     0,     0,     0,     0,
     197,   104,   105,   106,   107,   108,   109,   110,   111,   112,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
       0,     0,     0,     0,     0,     0,   198,   104,   105,   106,
     107,   108,   109,   110,   111,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,     0,     0,     0,     0,
       0,     0,   199,   104,   105,   106,   107,   108,   109,   110,
     111,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,     0,     0,     0,     0,     0,     0,   200,   104,
     105,   106,   107,   108,   109,   110,   111,   112,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,     0,     0,
       0,     0,     0,     0,   202,   104,   105,   106,   107,   108,
     109,   110,   111,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,     0,     0,     0,     0,     0,     0,
     203,   104,   105,   106,   107,   108,   109,   110,   111,   112,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
       0,     0,     0,     0,     0,     0,   204,   104,   105,   106,
     107,   108,   109,   110,   111,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,     0,     0,     0,     0,
       0,     0,   205,   104,   105,   106,   107,   108,   109,   110,
     111,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,     0,     0,     0,     0,     0,     0,   206,   104,
     105,   106,   107,   108,   109,   110,   111,   112,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,     0,     0,
       0,     0,     0,     0,   251,   104,   105,   106,   107,   108,
     109,   110,   111,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,     0,     0,     0,     0,     0,     0,
     258,   104,   105,   106,   107,   108,   109,   110,   111,   112,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
       0,     0,     0,     0,     0,     0,   259,   104,   105,   106,
     107,   108,   109,   110,   111,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,     0,     0,     0,     0,
       0,     0,   269,   104,   105,   106,   107,   108,   109,   110,
     111,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,     0,     0,     0,     0,     0,     0,   285,   104,
     105,   106,   107,   108,   109,   110,   111,   112,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,     0,     0,
       0,     0,   266,   104,   105,   106,   107,   108,   109,   110,
     111,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,     0,     0,     0,     0,   291,   104,   105,   106,
     107,   108,   109,   110,   111,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,     0,     0,   123,   104,
     105,   106,   107,   108,   109,   110,   111,   112,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,     0,     0,
     243,   104,   105,   106,   107,   108,   109,   110,   111,   112,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
       0,     0,   263,   104,   105,   106,   107,   108,   109,   110,
     111,   112,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,     0,     0,   302,   104,   105,   106,   107,   108,
     109,   110,   111,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,     0,     0,   303,   104,   105,   106,
     107,   108,   109,   110,   111,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,     0,     0,   318,   104,
     105,   106,   107,   108,   109,   110,   111,   112,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   201,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   104,   105,
     106,   107,   108,   109,   110,   111,   112,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   256,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   104,   105,   106,
     107,   108,   109,   110,   111,   112,     0,   104,   105,   106,
     107,   108,   109,   110,   111,     0,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   104,   105,   106,   107,
     108,   109,   110,     0,     0,   104,   105,   106,   107,   108,
     109,   110,     0,     0,     0,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   104,   105,   106,   107,   108,   109,
     110,   104,   105,   106,   107,   108,   109,   110,   104,   105,
     106,   107,   108,     0,     0,   115,   116,   117,   118,   119,
     120,   121,   122,   116,   117,   118,   119,   120,   121,   122,
     116,   117,   118,   119,   120,   121,   122
};

static const yytype_int16 yycheck[] =
{
      43,   177,    38,    39,    40,    49,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      63,    64,    65,    66,    67,    56,    62,    63,    64,    65,
      66,    70,    70,    77,    72,    49,    49,    52,    77,    77,
      52,    52,    73,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    50,    97,   231,    70,    70,    72,    70,
      51,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     182,    53,    14,    15,    53,    17,   188,    19,    49,   191,
      53,    14,    15,    53,    17,    53,    19,    29,    30,    51,
      56,    33,    56,    56,    36,    37,    29,    30,     0,    70,
      33,    72,    49,    36,    37,    69,    77,    73,    50,    51,
      73,    64,    65,    66,   226,    17,    53,    50,    51,    53,
     173,    14,    15,    70,    34,    35,    19,    53,    54,    53,
      77,    53,    74,    75,   309,   310,    29,    30,   250,    53,
       0,    74,    75,    36,    37,    53,    73,   306,   201,    50,
     309,   310,   264,   312,    71,   314,   315,    50,    51,    51,
      72,   214,    53,   216,   217,    62,    63,    64,    65,    66,
     223,    70,   225,    14,    15,    51,    17,    72,    19,    69,
      74,   234,   235,    72,    72,    72,    72,   240,    29,    30,
      72,    72,    33,    72,    72,    36,    37,    53,    71,    71,
      52,    14,    15,    72,    17,    72,    19,    72,    69,    50,
      51,    51,    56,    70,    52,   268,    29,    30,    72,   272,
      33,    74,    69,    36,    37,    32,    53,    69,    71,   282,
     283,    77,    49,    74,    75,    74,    69,    50,    51,    18,
      69,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      69,    76,    75,    69,    76,   308,    76,    69,    17,    44,
     246,    74,   288,   129,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    53,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    63,    -1,    -1,    -1,    67,    68,
      69,    -1,    18,    72,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    -1,    -1,    -1,    -1,    14,    15,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    30,    51,    -1,    53,    54,    55,
      36,    -1,    -1,    -1,    -1,    -1,    62,    63,    -1,    -1,
      -1,    67,    68,    -1,    50,    51,    72,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    -1,    -1,    -1,
      71,    -1,    73,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    -1,    -1,    -1,    -1,    71,    -1,    73,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    73,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    73,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    -1,    -1,    -1,    -1,    -1,    -1,    73,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    -1,    -1,
      -1,    -1,    71,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    -1,    -1,    -1,    -1,    71,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    -1,    69,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    -1,    -1,
      69,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      -1,    -1,    69,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    -1,    -1,    69,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    -1,    69,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    -1,    69,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    -1,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    38,    39,    40,    41,
      42,    43,    44,    -1,    -1,    38,    39,    40,    41,    42,
      43,    44,    -1,    -1,    -1,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    38,    39,    40,    41,    42,    43,
      44,    38,    39,    40,    41,    42,    43,    44,    38,    39,
      40,    41,    42,    -1,    -1,    59,    60,    61,    62,    63,
      64,    65,    66,    60,    61,    62,    63,    64,    65,    66,
      60,    61,    62,    63,    64,    65,    66
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    50,    79,    80,    81,    82,
      86,    89,   131,    51,    52,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    52,    53,    51,    87,    88,     0,
      81,    70,    72,    77,    56,    69,    53,    73,    90,    18,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    51,
      53,    54,    55,    62,    63,    67,    68,    72,   100,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   136,
      51,    88,    71,    92,    50,    93,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    49,    70,    72,   136,
     136,   136,   136,   136,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    69,    70,    52,    70,    74,    51,    56,
      73,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   101,    73,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,    53,    69,    53,    14,    15,    17,    19,
      29,    30,    33,    36,    37,    50,    51,    74,    83,    84,
      85,    94,    95,    96,    97,    98,   103,   115,   117,   118,
     127,   128,   130,   131,   132,    93,    91,    73,    73,    73,
      73,    56,    73,    73,    73,    73,    73,    71,    73,   102,
     136,    71,    71,    52,    72,   129,    72,    72,    69,   136,
      69,    51,    49,    70,    72,    77,    95,    75,    96,    96,
      96,    74,   136,    49,    70,    56,    73,    70,    52,   136,
      72,   136,   136,    69,    69,   136,    99,   136,    75,    32,
      95,    73,   136,   136,    53,    69,    56,   136,    73,    73,
      71,    73,   102,    69,   116,    75,    71,    71,   104,    73,
     119,    49,    70,    77,    77,    73,    96,    49,   136,    74,
      69,   136,   133,   134,    69,    73,    34,    35,   121,   124,
     126,    71,   136,   136,    69,    53,    54,    76,   126,    75,
      49,    77,    69,    69,    76,    76,   125,    69,   135,   122,
     123,    97,   120,   136,   120,   120,    37,    97,    69,    69
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
#line 96 "Sources/cmm2asm.y"
    {exec_diretivas("#PRNAME",(yyvsp[(2) - (2)]),0);;}
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 97 "Sources/cmm2asm.y"
    {exec_diretivas("#DIRNAM",(yyvsp[(2) - (2)]),0);;}
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 98 "Sources/cmm2asm.y"
    {exec_diretivas("#DATYPE",(yyvsp[(2) - (2)]),1);;}
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 99 "Sources/cmm2asm.y"
    {exec_diretivas("#NUBITS",(yyvsp[(2) - (2)]),0);;}
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 100 "Sources/cmm2asm.y"
    {exec_diretivas("#NBMANT",(yyvsp[(2) - (2)]),2);;}
    break;

  case 13:

/* Line 1464 of yacc.c  */
#line 101 "Sources/cmm2asm.y"
    {exec_diretivas("#NBEXPO",(yyvsp[(2) - (2)]),3);;}
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 102 "Sources/cmm2asm.y"
    {exec_diretivas("#NDSTAC",(yyvsp[(2) - (2)]),0);;}
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 103 "Sources/cmm2asm.y"
    {exec_diretivas("#SDEPTH",(yyvsp[(2) - (2)]),0);;}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 104 "Sources/cmm2asm.y"
    {exec_diretivas("#NUIOIN",(yyvsp[(2) - (2)]),4);;}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 105 "Sources/cmm2asm.y"
    {exec_diretivas("#NUIOOU",(yyvsp[(2) - (2)]),5);;}
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 106 "Sources/cmm2asm.y"
    {exec_diretivas("#NUGAIN",(yyvsp[(2) - (2)]),0);;}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 107 "Sources/cmm2asm.y"
    {exec_diretivas("#FFTSIZ",(yyvsp[(2) - (2)]),0);;}
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 108 "Sources/cmm2asm.y"
    {     use_macro(  v_name[(yyvsp[(2) - (2)])],1);;}
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 109 "Sources/cmm2asm.y"
    {     end_macro(              );;}
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 113 "Sources/cmm2asm.y"
    {use_macro(v_name[(yyvsp[(2) - (2)])],0);;}
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 114 "Sources/cmm2asm.y"
    {end_macro(            );;}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 115 "Sources/cmm2asm.y"
    {use_inter(            );;}
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 120 "Sources/cmm2asm.y"
    {declar_arr_1d((yyvsp[(2) - (7)]),(yyvsp[(4) - (7)]),(yyvsp[(6) - (7)])   );;}
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 121 "Sources/cmm2asm.y"
    {declar_arr_2d((yyvsp[(2) - (10)]),(yyvsp[(4) - (10)]),(yyvsp[(7) - (10)]),(yyvsp[(9) - (10)]));;}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 125 "Sources/cmm2asm.y"
    {declar_var   ((yyvsp[(1) - (1)])         );;}
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 126 "Sources/cmm2asm.y"
    {declar_arr_1d((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)])   ,-1);;}
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 127 "Sources/cmm2asm.y"
    {declar_arr_2d((yyvsp[(1) - (7)]),(yyvsp[(3) - (7)]),(yyvsp[(6) - (7)]),-1);;}
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 131 "Sources/cmm2asm.y"
    {declar_fun     ((yyvsp[(1) - (3)]),(yyvsp[(2) - (3)]));;}
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 132 "Sources/cmm2asm.y"
    {declar_firstpar((yyvsp[(5) - (6)])   );;}
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 133 "Sources/cmm2asm.y"
    {func_ret       ((yyvsp[(2) - (10)])   );;}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 134 "Sources/cmm2asm.y"
    {declar_fun     ((yyvsp[(1) - (4)]),(yyvsp[(2) - (4)]));;}
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 135 "Sources/cmm2asm.y"
    {func_ret       ((yyvsp[(2) - (8)])   );;}
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 140 "Sources/cmm2asm.y"
    {(yyval) = declar_par((yyvsp[(1) - (2)]),(yyvsp[(2) - (2)]));;}
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 141 "Sources/cmm2asm.y"
    {        set_par((yyvsp[(3) - (3)])   );;}
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 144 "Sources/cmm2asm.y"
    {declar_ret((yyvsp[(2) - (3)]),1);;}
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 145 "Sources/cmm2asm.y"
    {  void_ret(    );;}
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 172 "Sources/cmm2asm.y"
    {fun_id2  = (yyvsp[(1) - (2)]) ;;}
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 173 "Sources/cmm2asm.y"
    {vcall     ((yyvsp[(1) - (6)]));;}
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 175 "Sources/cmm2asm.y"
    {fun_id2  = (yyvsp[(1) - (2)]) ;;}
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 176 "Sources/cmm2asm.y"
    {(yyval) = fcall((yyvsp[(1) - (5)]));;}
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 183 "Sources/cmm2asm.y"
    {par_exp    ((yyvsp[(1) - (1)]));;}
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 184 "Sources/cmm2asm.y"
    {par_listexp((yyvsp[(3) - (3)]));;}
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 188 "Sources/cmm2asm.y"
    {     exec_out1((yyvsp[(3) - (4)])   );;}
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 189 "Sources/cmm2asm.y"
    {     exec_out2((yyvsp[(6) - (8)])   );;}
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 190 "Sources/cmm2asm.y"
    {(yyval) = exec_in  ((yyvsp[(3) - (4)])   );;}
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 191 "Sources/cmm2asm.y"
    {(yyval) = exec_pst ((yyvsp[(3) - (4)])   );;}
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 192 "Sources/cmm2asm.y"
    {(yyval) = exec_abs ((yyvsp[(3) - (4)])   );;}
    break;

  case 70:

/* Line 1464 of yacc.c  */
#line 193 "Sources/cmm2asm.y"
    {(yyval) = exec_sign((yyvsp[(3) - (6)]),(yyvsp[(5) - (6)]));;}
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 194 "Sources/cmm2asm.y"
    {(yyval) = exec_norm((yyvsp[(3) - (4)])   );;}
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 195 "Sources/cmm2asm.y"
    {(yyval) = exec_sqrt((yyvsp[(3) - (4)])   );;}
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 196 "Sources/cmm2asm.y"
    {(yyval) = exec_real((yyvsp[(3) - (4)])   );;}
    break;

  case 74:

/* Line 1464 of yacc.c  */
#line 197 "Sources/cmm2asm.y"
    {(yyval) = exec_imag((yyvsp[(3) - (4)])   );;}
    break;

  case 75:

/* Line 1464 of yacc.c  */
#line 198 "Sources/cmm2asm.y"
    {(yyval) = exec_atan((yyvsp[(3) - (4)])   );;}
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 199 "Sources/cmm2asm.y"
    {(yyval) = exec_fase((yyvsp[(3) - (4)])   );;}
    break;

  case 77:

/* Line 1464 of yacc.c  */
#line 203 "Sources/cmm2asm.y"
    {else_stmt(  );;}
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 204 "Sources/cmm2asm.y"
    {if_fim   (  );;}
    break;

  case 79:

/* Line 1464 of yacc.c  */
#line 205 "Sources/cmm2asm.y"
    {if_stmt  (  );;}
    break;

  case 80:

/* Line 1464 of yacc.c  */
#line 206 "Sources/cmm2asm.y"
    {if_exp   ((yyvsp[(3) - (4)]));;}
    break;

  case 81:

/* Line 1464 of yacc.c  */
#line 210 "Sources/cmm2asm.y"
    {exec_switch((yyvsp[(3) - (4)]));;}
    break;

  case 82:

/* Line 1464 of yacc.c  */
#line 211 "Sources/cmm2asm.y"
    { end_switch(  );;}
    break;

  case 85:

/* Line 1464 of yacc.c  */
#line 215 "Sources/cmm2asm.y"
    {switch_break();;}
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 217 "Sources/cmm2asm.y"
    {  case_test((yyvsp[(2) - (3)]),1);;}
    break;

  case 88:

/* Line 1464 of yacc.c  */
#line 218 "Sources/cmm2asm.y"
    {  case_test((yyvsp[(2) - (3)]),2);;}
    break;

  case 90:

/* Line 1464 of yacc.c  */
#line 219 "Sources/cmm2asm.y"
    {defaut_test(    );;}
    break;

  case 95:

/* Line 1464 of yacc.c  */
#line 225 "Sources/cmm2asm.y"
    {while_stmt  (  );;}
    break;

  case 96:

/* Line 1464 of yacc.c  */
#line 226 "Sources/cmm2asm.y"
    {while_expp  (  );;}
    break;

  case 97:

/* Line 1464 of yacc.c  */
#line 227 "Sources/cmm2asm.y"
    {while_expexp((yyvsp[(4) - (5)]));;}
    break;

  case 98:

/* Line 1464 of yacc.c  */
#line 228 "Sources/cmm2asm.y"
    {exec_break  (  );;}
    break;

  case 100:

/* Line 1464 of yacc.c  */
#line 233 "Sources/cmm2asm.y"
    {declar_var((yyvsp[(2) - (5)])); var_set((yyvsp[(2) - (5)]),(yyvsp[(4) - (5)]),0,0,1,0);;}
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 238 "Sources/cmm2asm.y"
    {var_set((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]),0,0,1,0);;}
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 240 "Sources/cmm2asm.y"
    {  pplus_assign((yyvsp[(1) - (3)])      );;}
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 241 "Sources/cmm2asm.y"
    {  aplus_assign((yyvsp[(1) - (6)]),(yyvsp[(3) - (6)])   );;}
    break;

  case 104:

/* Line 1464 of yacc.c  */
#line 242 "Sources/cmm2asm.y"
    {  aplu2_assign((yyvsp[(1) - (9)]),(yyvsp[(3) - (9)]),(yyvsp[(6) - (9)]));;}
    break;

  case 105:

/* Line 1464 of yacc.c  */
#line 244 "Sources/cmm2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),  0    );;}
    break;

  case 106:

/* Line 1464 of yacc.c  */
#line 245 "Sources/cmm2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),  1,0,1,0);;}
    break;

  case 107:

/* Line 1464 of yacc.c  */
#line 247 "Sources/cmm2asm.y"
    {array_1d_check((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)]),  2    );;}
    break;

  case 108:

/* Line 1464 of yacc.c  */
#line 248 "Sources/cmm2asm.y"
    {var_set       ((yyvsp[(1) - (8)]),(yyvsp[(7) - (8)]),  1,0,1,0);;}
    break;

  case 109:

/* Line 1464 of yacc.c  */
#line 250 "Sources/cmm2asm.y"
    {array_2d_check((yyvsp[(1) - (8)]), (yyvsp[(3) - (8)]),(yyvsp[(6) - (8)])    );;}
    break;

  case 110:

/* Line 1464 of yacc.c  */
#line 251 "Sources/cmm2asm.y"
    {var_set       ((yyvsp[(1) - (11)]),(yyvsp[(10) - (11)]), 2,0,1,0);;}
    break;

  case 111:

/* Line 1464 of yacc.c  */
#line 262 "Sources/cmm2asm.y"
    {(yyval) = num2exp((yyvsp[(1) - (1)]),1);;}
    break;

  case 112:

/* Line 1464 of yacc.c  */
#line 263 "Sources/cmm2asm.y"
    {(yyval) = num2exp((yyvsp[(1) - (1)]),2);;}
    break;

  case 113:

/* Line 1464 of yacc.c  */
#line 264 "Sources/cmm2asm.y"
    {(yyval) = num2exp((yyvsp[(1) - (1)]),5);;}
    break;

  case 114:

/* Line 1464 of yacc.c  */
#line 266 "Sources/cmm2asm.y"
    {(yyval) =  id2exp((yyvsp[(1) - (1)])  );;}
    break;

  case 115:

/* Line 1464 of yacc.c  */
#line 268 "Sources/cmm2asm.y"
    {(yyval) = array1d2exp((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]), 0);;}
    break;

  case 116:

/* Line 1464 of yacc.c  */
#line 269 "Sources/cmm2asm.y"
    {(yyval) = array1d2exp((yyvsp[(1) - (4)]),(yyvsp[(3) - (4)]), 1);;}
    break;

  case 117:

/* Line 1464 of yacc.c  */
#line 270 "Sources/cmm2asm.y"
    {(yyval) = array2d2exp((yyvsp[(1) - (7)]),(yyvsp[(3) - (7)]),(yyvsp[(6) - (7)]));;}
    break;

  case 118:

/* Line 1464 of yacc.c  */
#line 272 "Sources/cmm2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 119:

/* Line 1464 of yacc.c  */
#line 273 "Sources/cmm2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 120:

/* Line 1464 of yacc.c  */
#line 274 "Sources/cmm2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 121:

/* Line 1464 of yacc.c  */
#line 275 "Sources/cmm2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 122:

/* Line 1464 of yacc.c  */
#line 276 "Sources/cmm2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 123:

/* Line 1464 of yacc.c  */
#line 277 "Sources/cmm2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 124:

/* Line 1464 of yacc.c  */
#line 278 "Sources/cmm2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 125:

/* Line 1464 of yacc.c  */
#line 279 "Sources/cmm2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 126:

/* Line 1464 of yacc.c  */
#line 280 "Sources/cmm2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 127:

/* Line 1464 of yacc.c  */
#line 281 "Sources/cmm2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 128:

/* Line 1464 of yacc.c  */
#line 282 "Sources/cmm2asm.y"
    {(yyval) = (yyvsp[(1) - (1)]);;}
    break;

  case 129:

/* Line 1464 of yacc.c  */
#line 284 "Sources/cmm2asm.y"
    {(yyval) = (yyvsp[(2) - (3)]);;}
    break;

  case 130:

/* Line 1464 of yacc.c  */
#line 285 "Sources/cmm2asm.y"
    {(yyval) = (yyvsp[(2) - (2)]);;}
    break;

  case 131:

/* Line 1464 of yacc.c  */
#line 287 "Sources/cmm2asm.y"
    {(yyval) =     negacao((yyvsp[(2) - (2)])      );;}
    break;

  case 132:

/* Line 1464 of yacc.c  */
#line 288 "Sources/cmm2asm.y"
    {(yyval) =    oper_int((yyvsp[(2) - (2)]), 0, 0);;}
    break;

  case 133:

/* Line 1464 of yacc.c  */
#line 289 "Sources/cmm2asm.y"
    {(yyval) =    oper_int((yyvsp[(2) - (2)]), 0, 1);;}
    break;

  case 134:

/* Line 1464 of yacc.c  */
#line 290 "Sources/cmm2asm.y"
    {(yyval) =   exp_pplus((yyvsp[(1) - (2)])      );;}
    break;

  case 135:

/* Line 1464 of yacc.c  */
#line 291 "Sources/cmm2asm.y"
    {(yyval) = array_pplus((yyvsp[(1) - (5)]),(yyvsp[(3) - (5)])   );;}
    break;

  case 136:

/* Line 1464 of yacc.c  */
#line 292 "Sources/cmm2asm.y"
    {(yyval) = array_2plus((yyvsp[(1) - (8)]),(yyvsp[(3) - (8)]),(yyvsp[(6) - (8)]));;}
    break;

  case 137:

/* Line 1464 of yacc.c  */
#line 294 "Sources/cmm2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 2);;}
    break;

  case 138:

/* Line 1464 of yacc.c  */
#line 295 "Sources/cmm2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 3);;}
    break;

  case 139:

/* Line 1464 of yacc.c  */
#line 296 "Sources/cmm2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 4);;}
    break;

  case 140:

/* Line 1464 of yacc.c  */
#line 297 "Sources/cmm2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 5);;}
    break;

  case 141:

/* Line 1464 of yacc.c  */
#line 298 "Sources/cmm2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 6);;}
    break;

  case 142:

/* Line 1464 of yacc.c  */
#line 299 "Sources/cmm2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 7);;}
    break;

  case 143:

/* Line 1464 of yacc.c  */
#line 300 "Sources/cmm2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 8);;}
    break;

  case 144:

/* Line 1464 of yacc.c  */
#line 301 "Sources/cmm2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 9);;}
    break;

  case 145:

/* Line 1464 of yacc.c  */
#line 303 "Sources/cmm2asm.y"
    {(yyval) = oper_int((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]),10);;}
    break;

  case 146:

/* Line 1464 of yacc.c  */
#line 304 "Sources/cmm2asm.y"
    {(yyval) = oper_ari((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 0);;}
    break;

  case 147:

/* Line 1464 of yacc.c  */
#line 305 "Sources/cmm2asm.y"
    {(yyval) = oper_ari((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 1);;}
    break;

  case 148:

/* Line 1464 of yacc.c  */
#line 306 "Sources/cmm2asm.y"
    {(yyval) = oper_ari((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 2);;}
    break;

  case 149:

/* Line 1464 of yacc.c  */
#line 307 "Sources/cmm2asm.y"
    {(yyval) = oper_ari((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 3);;}
    break;

  case 150:

/* Line 1464 of yacc.c  */
#line 309 "Sources/cmm2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 0);;}
    break;

  case 151:

/* Line 1464 of yacc.c  */
#line 310 "Sources/cmm2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 1);;}
    break;

  case 152:

/* Line 1464 of yacc.c  */
#line 311 "Sources/cmm2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 2);;}
    break;

  case 153:

/* Line 1464 of yacc.c  */
#line 312 "Sources/cmm2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 3);;}
    break;

  case 154:

/* Line 1464 of yacc.c  */
#line 313 "Sources/cmm2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 4);;}
    break;

  case 155:

/* Line 1464 of yacc.c  */
#line 314 "Sources/cmm2asm.y"
    {(yyval) = oper_cmp((yyvsp[(1) - (3)]),(yyvsp[(3) - (3)]), 5);;}
    break;



/* Line 1464 of yacc.c  */
#line 2740 "cmm2asm.tab.c"
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
#line 316 "Sources/cmm2asm.y"


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
    itr_ok       = 0;

    float_init (); // inicializa variaveis de estado pra float (t2t.c)
	yyparse    (); // aqui a magica acontece!!
	fclose(yyin );
	fclose(f_asm);

	// carrega macros de ponto flutuante pra proc de ponto fixo
	// caso precise (espero que nao)
	if (fgen && prtype == 0) float_geni(argv[2]);
	if (mgen && prtype == 1) float_genf(argv[2]);
	if (mgen           == 1)  math_gen (argv[2]);

	// checa consistencia de todas as variaveis e funcoes
	check_var(); // (variaveis.c)

	return 0;
}

// erro de sintaxes do bison
void yyerror (char const *s)
{
	fprintf (stderr, "Pô, presta atenção na sintaxe da linha %d!\n", line_num+1);
}

