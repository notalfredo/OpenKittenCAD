#if !defined( __KEYWORDS_H__ )
#define __KEYWORDS_H__

//Temp will move to bison later
typedef enum tokens {
    tok_ID,
    tok_AND,
    tok_NOT,     
    tok_OR,    
    tok_BREAK,
    tok_CONTINUE,
    tok_IF,
    tok_ELIF,
    tok_ELSE,   
    tok_REPEAT,  
    tok_UNTIL,   
    tok_WHILE,  
    tok_FOR,     
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
