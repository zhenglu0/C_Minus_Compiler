/* The lexical implementation for the C- compiler */

#include "globalVar.h"
#include "utility.h"
#include "lexical.h"

/*词法DFA的状态*/
typedef enum
{START,INNUM,INID,INCMP,INCMT1,INCMT2,INCMT3,DONE}
StateType;

/*词法的标识符和保留字*/
char tokenString[MAXTOKENLEN+1];

/*BUFLEN = 源代码行的缓冲区长度*/
#define BUFLEN 256

static char lineBuf[BUFLEN];  /*保持在当前行*/
static int linepos = 0;       /*LineBuf的当前位置*/
static int bufsize = 0;       /*当前缓冲区字符串的大小*/


/*getNextChar从lineBuf中提取下一个非空字符
 *如果lineBuf耗尽的话就读取下一行
 */
static char getNextChar(void)
{
    if(!(linepos < bufsize))
    {
        lineno++;
        if(fgets(lineBuf,BUFLEN-1,sourcefile))   
        {
            if(EchoSource) fprintf(listingfile,"%4d: %s",lineno,lineBuf);
            bufsize = strlen(lineBuf);
            linepos = 0;
            return lineBuf[linepos++];
        }
        else return EOF;
    }
    else return lineBuf[linepos++];
}

/*ungetNextChar回溯lineBuf中的一个字符*/
static void ungetNextChar(void)
{
    linepos--;
}

/*保留字的查找表格*/
static struct
{
    char* str;
    TokenType tok;
}reservedWords[MAXRESERVED]
=
{ 
    {"if",IF},{"else",ELSE},{"int",INT},{"return",RETURN},
    {"void",VOID},{"while",WHILE}
};

/*通过临近搜索查找一个字符看它是不是保留字*/

static TokenType reservedLookup(char * s)
{
    int i;
    for(i=0; i<MAXRESERVED; i++)
        if(!strcmp(s,reservedWords[i].str))
            return reservedWords[i].tok;  //是保留字
    return ID;  //it is an average identifier
}

/*词法的主要职能*/
/*函数getNextToken返回源文件中的下一个token*/
TokenType getNextToken(void)
{
    
    int tokenStringIndex = 0;/*插入tokenString的索引*/
    TokenType currentToken;/*保持当前的token以返回*/
    StateType state = START;/*当前状态：通常在START处开始*/
    int saveFlag;/*表示保存到tokenString的标志*/
    char pre_c; /*记录最后一个字符*/ 

    while(state != DONE)
    {
        char c = getNextChar();
        saveFlag = TRUE;

        switch(state)
        {
        case INID:
            if(!isalpha(c))
            { /*在输入备份*/
                ungetNextChar();
                saveFlag = FALSE;
                state = DONE;
                currentToken = ID;
            }
            break;

                case INCMP:
            state = DONE;
            if(c == '=')
            {
                if(pre_c == '<')
                    currentToken = NGT;
                else if(pre_c == '>')
                    currentToken = NLT;
                else if(pre_c == '=')
                    currentToken = EQU;
                else if(pre_c == '!')
                    currentToken = NEQU;
                else
                    currentToken = ASSIGN;
            }
            else
            {
                if(pre_c == '<')
                    currentToken = LT;
                else if(pre_c == '>')
                    currentToken = GT;
                else if(pre_c == '=')
                    currentToken = ASSIGN;
                else
                { /*backup in the input*/                    
                    saveFlag = FALSE;
                    currentToken = ERROR;
                }
                ungetNextChar();
            }

            break;

        case INNUM:
            if(!isdigit(c))
            { /*在输入备份*/
                ungetNextChar();
                saveFlag = FALSE;
                state = DONE;
                currentToken = NUM;
            }
            break;

        case START:
            if(isdigit(c))
                state = INNUM;
            else if(isalpha(c))
                state = INID;
            else if((c=='<') || (c=='>')||(c=='=')||(c=='!'))
                state = INCMP;
            else if((c==' ')||(c=='\t')||(c=='\n'))
                saveFlag = FALSE;
            else if(c=='/'){
                saveFlag = FALSE;
                state = INCMT1;
            }
            else
            { 
                state = DONE;
                switch(c)
                {
                case EOF:
                    saveFlag = FALSE;
                    currentToken = ENDFILE;
                    break;
                case '+':
                    currentToken = PLUS;
                    break;
                case '-':
                    currentToken = MINUS;
                    break;
                case '*':
                    currentToken = TIMES;
                    break;
                case ';':
                    currentToken = SEMI;
                    break;
                case ',':
                    currentToken = DOT;
                    break;
                case '(':
                    currentToken = LBR;
                    break;
                case ')':
                    currentToken = RBR;
                    break;
                case '[':
                    currentToken = LSBR;
                    break;
                case ']':
                    currentToken = RSBR;
                    break;
                case '{':
                    currentToken = LLBR;
                    break;
                case '}':
                    currentToken = RLBR;
                    break;
                default:
                    currentToken = ERROR;
                    break;
                }
            }
            break;

        case INCMT1:
            saveFlag = FALSE;
            if(c == '*')
                state = INCMT2;
            else
            {
                ungetNextChar();
                saveFlag = TRUE;
                state = DONE;
                currentToken = OVER;
            }
            break;

        case INCMT2:
            saveFlag = FALSE;
            if(c == '*')
                state = INCMT3;
            else
                state = INCMT2;
            break;

        case INCMT3:
            saveFlag = FALSE;
            if(c == '/')
                state = START;
            else if(c == '*')
                state = INCMT3;
            else
                state = INCMT2;
            break;

        case DONE:
        default: /*应该永不会发生*/
            fprintf(listingfile,"词法 Bug: state= %d\n", state);
            state = DONE;
            currentToken = ERROR;
            break;
        }

        /*检测是否token的长度超越界限*/
        if((saveFlag)&&(tokenStringIndex <= MAXTOKENLEN)){
            tokenString[tokenStringIndex++] = (char)c;
        }
        if(state == DONE)
        {
            
            tokenString[tokenStringIndex] = '\0';

            /*查找保留字并判断（如果该标示符为保留字）
             */
            if(currentToken == ID)
                currentToken = reservedLookup(tokenString);
        }

        /*记录currentToken*/
        pre_c = c;
    }

    if(TraceLexical){
        fprintf(listingfile,"\t%d: ",lineno);
        printToken(currentToken,tokenString);
    }
    return currentToken;
} /*end getNextToken*/
