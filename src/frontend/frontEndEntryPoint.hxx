#ifndef FRONT_END_ENTRY_POINT
#define FRONT_END_ENTRY_POINT

typedef enum errorStatus {
    lexical_error,
    parser_error,
    semantic_error,
    other_error,
    success,
}errorStatus;


typedef struct programResponse{
    errorStatus status;
    char* message;
}programResponse;


typedef enum programType {
    file,
    string,
}programType;

programResponse* frontEndEntryPoint(char* program, programType progType);


#endif
