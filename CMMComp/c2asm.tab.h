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

extern YYSTYPE yylval;


