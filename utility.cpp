/*Utility function implementation for the C- compiler*/

#include "globalVar.h"
#include "utility.h"

/* 函数printToken打印token和它的词法到文件列表 
 */
void printToken( TokenType token, const char* tokenString)
{
    switch (token)
    {
    case ENDFILE:
        fprintf(listingfile,"EOF\n");
        break;
    case VOID:
        fprintf(listingfile,"保留字, name= %s\n",tokenString);
        break;
    case WHILE:
        fprintf(listingfile,"保留字, name= %s\n",tokenString);
        break;
    case IF:
        fprintf(listingfile,"保留字, name= %s\n",tokenString);
        break;
    case ERROR:
        fprintf(listingfile,"错误, val= %s\n",tokenString);
        break;
    case RETURN:
        fprintf(listingfile,"保留字, name= %s\n",tokenString);
        break;
    
    case ELSE:
        fprintf(listingfile,"保留字, name= %s\n",tokenString);
        break;
    case INT:
        fprintf(listingfile,"保留字, name= %s\n",tokenString);
        break;
    
    case ID:
        fprintf(listingfile,"ID, name= %s\n",tokenString);
        break;
    case NUM:
        fprintf(listingfile,"NUM, val= %s\n",tokenString);
        break;
    
    case NEQU:    fprintf(listingfile,"Special symbol: !=\n"); break;
    case LSBR:    fprintf(listingfile,"Special symbol: [\n"); break;
    case RSBR:    fprintf(listingfile,"Special symbol: ]\n"); break;
    case LLBR:    fprintf(listingfile,"Special symbol: {\n"); break;
    case RLBR:    fprintf(listingfile,"Special symbol: }\n"); break;
    case ASSIGN:    fprintf(listingfile,"Special symbol: =\n"); break;
    case LT:    fprintf(listingfile,"Special symbol: <\n"); break;
    case GT:    fprintf(listingfile,"Special symbol: >\n"); break;
    case RBR:    fprintf(listingfile,"Special symbol: )\n"); break;
    case PLUS:    fprintf(listingfile,"Special symbol: +\n"); break;
    case MINUS:    fprintf(listingfile,"Special symbol: -\n"); break;
    case NLT:    fprintf(listingfile,"Special symbol: >=\n"); break;
    case NGT:    fprintf(listingfile,"Special symbol: <=\n"); break;
    case SEMI:    fprintf(listingfile,"Special symbol: ;\n"); break;
    case DOT:    fprintf(listingfile,"Special symbol: ,\n"); break;
    case TIMES:    fprintf(listingfile,"Special symbol: *\n"); break;
    case OVER:    fprintf(listingfile,"Special symbol: /\n"); break;
    case EQU:    fprintf(listingfile,"Special symbol: ==\n"); break;
    case LBR:    fprintf(listingfile,"Special symbol: (\n"); break;

    default: /*应该永远不会发生*/
        fprintf(listingfile,"Unknown token: %d\n",token);
    }
}

/* 函数newStmtNode为语法树结构产生一个新的状态结点 
 */
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listingfile,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

/* 函数newExpressNode为语法构造树创建一个新的表达式结点
 */
TreeNode * newExpressNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listingfile,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
  }
  return t;
}

/*函数newParamNode为语法构造树创建一个新的界限结点
 */
TreeNode * newParamNode(ParamKind kind)
{
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listingfile,"Out of memory error at line %d\n",lineno);
    else
    {
        for (i = 0; i<MAXCHILDREN;i++)    t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ParamK;
        t->kind.param = kind;
        t->lineno = lineno;
    }
    return t;
}

/* 函数newDeclearNode为语法构造树创建一个新的声明结点
 */
TreeNode * newDeclearNode()
{
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listingfile,"Out of memory error at line %d\n",lineno);
    else
    {
        for (i=0;i<MAXCHILDREN;i++)    t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = DecK;
        t->lineno = lineno;
    }
    return t;
}

/*函数copyString分配并产生一个存在串的新的副本
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = (int)strlen(s)+1;
  t = (char *)malloc(n);
  if (t==NULL)
    fprintf(listingfile,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/*变量indentno是被printGrammarTree用来储存当前空间的缩进数
 */
static int indentno = 0;

#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces通过打印空格缩进*/
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listingfile," ");
}

/* 程序printGrammarTree通过使用标示符识别子树并打印一个语法树到文件列表 
 */
void printGrammarTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case IterationK:
          fprintf(listingfile,"While\n");
          break;
        case SelectionK:
          fprintf(listingfile,"If\n");
          break;
        case ElseK:
            fprintf(listingfile,"Else\n");
          break;
        case CompoundK:
          fprintf(listingfile,"Compound\n");
          break;
        case ReturnK:
          fprintf(listingfile,"Return\n");
          break;
        case ExpressionK:
          fprintf(listingfile,"express\n");
          break;
        default:
          fprintf(listingfile,"Unknown ExpNode statement kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
        case IdK:
          fprintf(listingfile,"Id: %s\n",tree->name);
          break;
        case CallK:
          fprintf(listingfile,"call: %s\n",tree->name);
          break;
        case OpK:
          fprintf(listingfile,"Op: ");
          printToken(tree->attr.op,"\0");
          break;
        case ConstK:
          fprintf(listingfile,"Const: %d\n",tree->attr.val);
          break;
        default:
          fprintf(listingfile,"Unknown ExpNode express kind\n");
          break;
      }
    }
    else if (tree->nodekind == DecK)
    {
        switch(tree->kind.dec) {
            case FunK:
                fprintf(listingfile,"Fun_dec: ");
                fprintf(listingfile," %s\n",tree->name);
                break;
            case VarK:
                fprintf(listingfile,"Var_dec: ");
                fprintf(listingfile," %s\n",tree->name);
                break;
            case ArrayK:
                fprintf(listingfile,"Array_dec: ");
                fprintf(listingfile," %s\n",tree->name);
            default:
                fprintf(listingfile,"Unknown declaration kind\n");
                break;
        }
    }
    else if (tree->nodekind == ParamK)
    {
        switch(tree->kind.param) {
            case Var: 
                if(strcmp(tree->attr.type , "void") != 0)
                {
                    fprintf(listingfile,"%s %s\n",tree->attr.type,tree->name);
                }
                else
                    fprintf(listingfile," param void\n");
                break;
            case Array:
                fprintf(listingfile,"%s %s\n",tree->attr.type,tree->name);
                break;
            case Null:
                fprintf(listingfile," param void\n");
                break;
            default:
                fprintf(listingfile,"Unknown params kind\n");
                break;
        }
    }
    else fprintf(listingfile,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printGrammarTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}
