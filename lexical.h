
/*The lexical interface for the C- compiler*/

#ifndef _LEXICAL_H_
#define _LEXICAL_H_

/*MAXTOKENLEN 是token的最大值*/
#define MAXTOKENLEN 40

/*函数getNextToken返回在资源文件中的下一个token*/
TokenType getNextToken(void);

/*tokenString队列储存每一个token的信息*/
extern char tokenString[MAXTOKENLEN+1];

#endif
