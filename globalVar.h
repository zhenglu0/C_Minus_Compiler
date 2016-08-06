/*Global types and vars for C- compiler*/

#ifndef _GLOBALVAR_H_
#define _GLOBALVAR_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*MAXRESERVED = 保留字的数量*/
#define MAXRESERVED 6
typedef enum
{ /*所有token*/
  ENDFILE,ERROR,
  /*保留字*/
  IF,ELSE,INT,RETURN,VOID,WHILE,
  /*多字符tokens*/
  ID,NUM,
  /*特殊符号*/
  PLUS,MINUS,TIMES,OVER,LT,GT,NLT,NGT,EQU,NEQU,ASSIGN,SEMI,DOT,
  RBR,LBR,RSBR,LSBR,RLBR,LLBR
}TokenType;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXCHILDREN 3

extern FILE* sourcefile;  /*源代码文本文件*/
extern FILE* listingfile;  /*列表输出文本文件*/
extern int lineno;  /*需要列出的资源行数*/

typedef enum {StmtK,ExpK,DecK,ParamK} NodeKind;
typedef enum {CompoundK,SelectionK,IterationK,ReturnK,ExpressionK,ElseK} StmtKind;
typedef enum {OpK,ConstK,IdK,AssignK,CallK} ExpKind;
typedef enum {Var,Array,Null} ParamKind;
typedef enum {ArrayK,VarK,FunK} DecKind;

/*****    C-语法分析    *****/

typedef struct treeNode
{
    struct treeNode * child[MAXCHILDREN];
    struct treeNode * sibling;
    NodeKind nodekind;
    int lineno;
    union { TokenType op;
         int val;
         char * type;
          } attr;
    union { StmtKind stmt; ExpKind exp; DecKind dec;ParamKind param;} kind;
    char * name;
} TreeNode;

/*****    追踪标志    *****/

extern int EchoSource;/*EchoSource = TRUE 导致源程序在分析时对列表文件的行数作出相应*/

extern int TraceLexical;/*TraceLexical = TRUE 导致每当token被扫描程序识别时token的信息被打印在列表文件*/

extern int TraceGrammar;/*TraceGrammar = TRUE 导致语法树被以线性方式打印在列表文件 */

/*Error = TRUE 如果一个错误产生防止进一步的步骤*/
extern int Error;
#endif
