/*Utility function for the C- compiler*/


#ifndef _UTILITY_H_
#define _UTILITY_H_

/* 函数printToken打印token和它的词法到文件列表 
 */
void printToken( TokenType, const char* );

/* 函数copyString分配并产生一个存在串的新的副本
 */
char * copyString( char * );

/* 程序printGrammarTree通过使用标示符识别子树并打印一个语法树到文件列表 
 */
void printGrammarTree( TreeNode * );

/* 函数newExpressNode为语法构造树创建一个新的表达式结点
 */
TreeNode * newExpressNode(ExpKind);

/* 函数newStmtNode为语法树结构产生一个新的状态结点 
 */
TreeNode * newStmtNode(StmtKind);

/* 函数newParamNode为语法构造树创建一个新的界限结点
 */
TreeNode * newParamNode(ParamKind);

/* 函数newDeclearNode为语法构造树创建一个新的声明结点
 */
TreeNode * newDeclearNode();

#endif
