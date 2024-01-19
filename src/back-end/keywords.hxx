#if !defined( __KEYWORDS_H__ )
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

}tokens;


typedef struct {
    char *keyWord;
    int token;
} Keyword;


//Tokens get imported from "parser.tab.h" that bison genarates
Keyword keywords[] = {
  { "and",      tok_AND      },
  { "not",      tok_NOT      },
  { "or",       tok_OR       },
  { "break",    tok_BREAK    },
  { "continue", tok_CONTINUE },
  { "if",       tok_IF       },
  { "elif",     tok_ELIF     },
  { "else",     tok_ELSE     },
  { "repeat",   tok_REPEAT   },
  { "until",    tok_UNTIL    },
  { "while",    tok_WHILE    },
  { "for",      tok_FOR      },
  { "let",      tok_LET      },
};


#define NUM_KEYWORDS (sizeof(keywords) / sizeof(Keyword))

#endif
