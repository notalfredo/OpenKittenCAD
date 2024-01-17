#if !defined( __KEYWORDS_H__ )
#define __KEYWORDS_H__

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
