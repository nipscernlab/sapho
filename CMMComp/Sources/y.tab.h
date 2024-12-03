/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

extern YYSTYPE yylval;


