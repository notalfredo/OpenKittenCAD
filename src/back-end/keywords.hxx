#ifndef __KEYWORDS_H__
#define __KEYWORDS_H__

//Temp will move to bison later
typedef enum tokens {
    tok_ILLEGAL,

    tok_EQ,
    tok_LT,
    tok_GT,
    tok_LE,
    tok_GE,
    tok_NE,

    tok_AND,
    tok_OR,    
    tok_NOT,     

    tok_UNION,
    tok_DIFFERENCE,
    tok_INTERSECTION,

    tok_ID,


    tok_FOR,     
    tok_REPEAT,  
    tok_UNTIL,   
    tok_WHILE,  
    tok_BREAK,
    tok_CONTINUE,

    tok_IF,
    tok_ELIF,
    tok_ELSE,   

    tok_LET,    
    tok_NUM,

}tokens;


typedef struct {
    char *keyWord;
    int token;
} Keyword;



#endif
