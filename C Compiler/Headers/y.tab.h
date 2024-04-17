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

extern YYSTYPE yylval;


