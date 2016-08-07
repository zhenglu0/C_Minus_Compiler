/* Main program for the C- compiler */
#include "globalVar.h"
#include "utility.h"
#include "lexical.h"
#include "grammar.h"

/*全局变量*/
int lineno = 0;
FILE* sourcefile;
FILE* listingfile;

int EchoSource = TRUE;
int TraceLexical = TRUE;
int TraceGrammar = FALSE;
int Error = FALSE;

int main(int argc, char** argv)
{
    TreeNode * syntaxTree;
    char file[20];

    if(argc < 2)
    {
        fprintf(stderr,"用法: %s <file>\n没有语法只打印词法", argv[0]);
        exit(1);
    }

    strcpy(file,argv[1]);
    if(strchr(file,'.') == NULL)
        strcat(file,".c-");

    sourcefile = fopen(file,"r");
    if(sourcefile == NULL)
    {
        fprintf(stderr,"文件 %s 没有找到\n",file);
        exit(1);
    }

    listingfile = stdout;

    fprintf(listingfile,"*********************************************\n\n",file);
    fprintf(listingfile,"**************C- 编译器: %s**************\n\n",file);
    fprintf(listingfile,"*********************************************\n\n",file);
    fprintf(listingfile,"C- 词法分析: %s\n",file);
    if(TraceGrammar)
        while (getNextToken()!=ENDFILE);
    else
    {
        syntaxTree = grammarTree();

        fprintf(listingfile,"\n*****************C- 语法树:*****************\n");
        printGrammarTree(syntaxTree);
    }

    fclose(sourcefile);
    return 0;
}
