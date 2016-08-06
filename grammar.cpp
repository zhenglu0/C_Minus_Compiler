
/* C- 编译器的语法执行  */

#include "globalVar.h"
#include "utility.h"
#include "lexical.h"
#include "grammar.h"


/*The parser interface for the C- compiler*/

#ifndef _GRAMMER_H_
#define _GRAMMER_H_

/* 函数分析返回最新的语法树
 */
TreeNode * grammar(void);

#endif


/*C-语言的BNF语法*/

static TokenType token;
/* declar_list -> declar_list declartion | declaration */
static TreeNode * iter_stmt(void);
/* return_stmt -> return ; | return express ; */
static TreeNode * add_express(void);
/* terment -> terment mulop factor | factor 
 * mulop -> * | / 
 */
static TreeNode * terment(void);
/* factor -> (express) | var | call | NUM */
static TreeNode * factor(void);
/* call -> ID ( args ) */
static TreeNode * call(void);
/* args -> argment_list | empty */
static TreeNode * return_stmt(void);
/* express -> var = express | simple-express */
static TreeNode * express(void);
/* expr_stmt -> express ; | ; */
static TreeNode * declar_list(void);  
/* declaration -> var_declaration | fun_declaration
 * var_declaration -> type_specifier ID ; | type_specifier ID [ NUM ];
 * type_specifier -> int | void
 * fun_declaration -> type_specifier ID (params) comp_stmt
 */
static TreeNode * param(void);
/* comp_stmt -> { local_declarat stmt_list } */
static TreeNode * comp_stmt(void);
/* local_declarat -> local_declarat var_delcaration | empty */
static TreeNode * local_declarat(void);
/* stmt_list -> stmt_list statement | empty */
static TreeNode * declaration(void);  
/* params -> param_list | void */
static TreeNode * select_stmt(void);
/* iter_stmt -> while (express) statement */

static TreeNode * params(void);  
/* param_list -> param_list, param | param */
static TreeNode * param_list(void);  
/* param -> type_spcifier ID | type_specifier ID[ ] */
static TreeNode * stmt_list(void);
/* statement -> expession_stmt | comp_stmt | select_stmt 
 * | iter_stmt | return_stmt
 */
static TreeNode * statement(void);
/* select_stmt -> if (expession) statement |
 *                   if (express) statement else statement
 */

static TreeNode * expr_stmt(void);
/* var -> ID | ID [express] */
static TreeNode * var(void);
/* simple_express -> add_express relop add_express 
 *                      | add_express
 * relop -> <= | < | > | >= | == | !=
 */
static TreeNode * simple_express(void);
/* add_express -> add_express addop terment | terment 
 * addop -> + | -
 */

static TreeNode * args(void);
/* argment_list -> argment_list , express | express */
static TreeNode * argment_list(void);

static void syntaxError(char * message)
{ fprintf(listingfile,"\n>>> ");
  fprintf(listingfile,"语法错误存在于行 %d: %s",lineno,message);
  Error = TRUE;
}

static void match(TokenType expected)
{ if (token == expected) token = getNextToken();
  else {
    syntaxError("意外的token -> ");
    printToken(token,tokenString);
    fprintf(listingfile,"      ");
  }
}

TreeNode * declaration(void)
{
    TreeNode * t = newDeclearNode();
    switch(token) {
    case INT:
        t->attr.type = copyString(tokenString);
        match(INT);
        break;
    case VOID:
        t->attr.type = copyString(tokenString);
        match(VOID);
        break;
    default:
        syntaxError("形式上的意外token-> ");
        printToken(token,tokenString);
        token = getNextToken();
        break;
    }
    t->name = copyString(tokenString);
    match(ID);
    switch(token) {
        case SEMI:
            match(SEMI);
            t->kind.dec = VarK;
            break;
        case LSBR:
            match(LSBR);
            t->kind.dec = ArrayK;
            t->child[0] = factor();
            match(RSBR);
            match(SEMI);
            break;
        case LBR:
            match(LBR);
            t->kind.dec = FunK;
            if (t != NULL)
                t->child[0] = params();
            match(RBR);
            if (t != NULL)
                t->child[1] = comp_stmt();
            break;
        default:
            syntaxError("声明上的意外token -> ");
            printToken(token,tokenString);
            token = getNextToken();
            break;
    }
    return t;
}

TreeNode * params(void)
{
    TreeNode * t = newParamNode(Null);
    if (token == VOID)
    {
        t->kind.param = Var;
        t->attr.type = copyString(tokenString);
        match(VOID);
    }
    else if(token == INT){
        //t->kind.param = Var;
        t->child[0] = param_list();
    }
    else
    {
        syntaxError("界限上的意外token -> ");
        printToken(token,tokenString);
        token = getNextToken();
    }
    return t;
}

TreeNode * param_list(void)
{
    TreeNode * t = param();
    TreeNode * p = t;
    while(token == DOT)
    {
        match(DOT);
        TreeNode * q = param();
        if(q != NULL)
        {
            if(t == NULL)    t = p = q;
            else
            {
                p->sibling = q;
                p = q;
            }
        }
    }
    return t;
}


TreeNode * iter_stmt(void)
{
    TreeNode * t = newStmtNode(IterationK);
    match(WHILE);
    match(LBR);
    if(t != NULL)    t->child[0] = express();
    match(RBR);
    if(t != NULL)    t->child[1] = statement();
    return t;
}

TreeNode * return_stmt(void)
{
    TreeNode * t = newStmtNode(ReturnK);
    match(RETURN);
    if(token == SEMI)
        match(SEMI);
    else 
    {
        t->child[0]=express();
        match(SEMI);
    }
    return t;
}

TreeNode * var(void)
{
    TreeNode * t = newExpressNode(IdK);
    t->name = copyString(tokenString);
    match(ID);
    if(token == LBR)
    {
        match(LSBR);
        t->child [0] = express();
        match(RSBR);
    }
    return t;
}

TreeNode * param(void)
{
    TreeNode * t = newParamNode(Null);
    t->attr.type = copyString(tokenString);
    match(INT);
    t->kind.param = Var;
    t->name = copyString(tokenString);
    match(ID);
    if (token == LSBR)
    {
        match(LSBR);
        t->kind.param = Array;
        match(RSBR);
    }
    return t;
}

TreeNode * comp_stmt()
{
    TreeNode * t = newStmtNode(CompoundK);
    match(LLBR);
    if((t != NULL)&&(token == INT || token == VOID))
        t->child[0] = local_declarat();
    if (t!=NULL)
        t->child[1] = stmt_list();
    match(RLBR);
    return t;
}

TreeNode * argment_list(void)
{
    TreeNode * t = express();
    TreeNode * p = t;
    while(token != RBR)
    {
        match(DOT);
        TreeNode * q = express();
        if(q != NULL)
        {
            if(t == NULL)    t = p = q;
            else
            {
                p->sibling = q;
                p = q;
            }
        }
    }
    return t;
}

TreeNode * declar_list(void)
{
    TreeNode * t = NULL;
    if(token == INT|| token == VOID)
    {
        t = declaration();
        TreeNode * p = t;
        while ((token != ENDFILE)&&(token == INT || token == VOID))
        {
            TreeNode * q = declaration();
            if (q != NULL)
            {
                if (t == NULL)    t = p = q;
                else
                {
                    p->sibling = q;
                    p = q;
                }
            }
        }
    }
    return t;
}

TreeNode * local_declarat(void)
{
    TreeNode * t = declaration();
    TreeNode * q =t;
    while(token == INT)
    {
        TreeNode * p = declaration();
        q->sibling=p;
        q=p;
    }
    return t;
}

TreeNode * stmt_list(void)
{
    TreeNode * t = statement();
    TreeNode * q = t;
    while((token != RLBR)&&(token != ENDFILE))
    {
        TreeNode *  p = statement();
        if(p!=NULL)
        { 
            if(q==NULL)t=q=p;
            else
            {  
            q->sibling = p;
            q = p;}
            }
    }
    return t;
}

TreeNode * express(void)
{
    TreeNode * t = simple_express();
    if(token==ASSIGN)
    {
        TreeNode * p = newExpressNode(OpK);
        if (p!=NULL)
        {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = express();
        }
        
    }
    return t;
}

TreeNode * simple_express(void)
{
    TreeNode * t = add_express();
    if ((token == LT)||(token == NGT)||(token == GT)||(token == NLT)||(token == EQU)||(token ==NEQU))
    {
        TreeNode * p = newExpressNode(OpK);
        if(p != NULL)
        {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
        }
            match(token);
            if(t != NULL)
                t->child[1] = add_express();
    }
    return t;
}

TreeNode * add_express(void)
{
    TreeNode * t = terment();
    while((token == PLUS)||(token == MINUS))
    {
        TreeNode * p = newExpressNode(OpK);
        if (p !=NULL) 
        {
            p->child[0] = t;
            p->attr.op = token;
            t = p;
            match(token);
            t->child[1] = terment();
        }
    }
    return t;
}

TreeNode * terment(void)
{
    TreeNode * t = factor();
    while ((token == TIMES) || (token == OVER))
    { TreeNode * p = newExpressNode(OpK);
    if (p != NULL) {
        p ->child[0] = t;
        p ->attr.op = token;
        t = p;
        match (token);
        p ->child[1] = factor();
    }
    }
    return t;
}


TreeNode * statement(void)
{
    TreeNode * t = NULL;
    switch(token)
    {
        case WHILE:    t = iter_stmt();break;
        case RETURN:    t = return_stmt();break;
        case LLBR:    t = comp_stmt();break;
        case IF:    t = select_stmt();break;
        case LBR:
        case ID:
        case NUM:
        case SEMI:
            t = expr_stmt();break;
        default:
            syntaxError("意外的状态 -> ");
            printToken(token,tokenString);
            token = getNextToken();
            break;
    }
    return t;
}

TreeNode * expr_stmt(void)
{
    TreeNode * t = newStmtNode(ExpressionK);
    if (token == SEMI){
        match(SEMI); 
    }
    else
    {
        t->child[0] = express();
        match(SEMI);
    }
    return t;
}

TreeNode * select_stmt(void)
{
    TreeNode * t = newStmtNode(SelectionK);
    match(IF);
    match(LBR);
    if(t != NULL)    t->child[0] = express();
    match(RBR);
    if(t != NULL)    t->child[1] = statement();
    if(token == ELSE)
    {
        t->sibling = newStmtNode(ElseK);
        match(ELSE);        
        if(t != NULL)    t->sibling->child[0] = statement();
    }
    return t;
}

TreeNode * factor(void)
{
    TreeNode * t = NULL;
    char * varToken;
    switch(token) {
        case NUM:
            t = newExpressNode(ConstK);
            if ((t != NULL)&&(token == NUM))
            {
                t->attr.val = atoi(tokenString);
                match(NUM);
            }
            break;
        case LBR:
            match(LBR);
            t = express();
            match(RBR);
            break;
        case ID:
            {
            varToken = copyString(tokenString);
            match(ID);
            if(token == LSBR)
            {
                    t = newExpressNode(IdK);
                    match(LSBR);
                    t->name = copyString(varToken);
                    t->child[0] = express();
                    match(RSBR);
            }
            else if(token == LBR)
            {
                    t = newExpressNode(CallK);
                    t->name = copyString(varToken);
                    match(LBR);
                    t->child[0] = args();
                    match(RBR);
            }
            else
            {
                    t = newExpressNode(IdK);
                    t->name = copyString(varToken);
            }
            }
            break;
        default:
            syntaxError("因子上的意外token-> ");
            printToken(token,tokenString);
            token = getNextToken();
            break;
    }
    return t;
}

TreeNode * args(void)
{
    TreeNode * t = NULL;
    if (token != RBR)
        t = argment_list();
    return t;
}



/****************************************/
/* 语法分析器的主要职能   */
/****************************************/

TreeNode * grammarTree(void)
{ 
    TreeNode * t;
    token = getNextToken();
    t = declar_list();
    if (token!=ENDFILE)
        syntaxError("代码在文件之前结束\n");
    return t;
}