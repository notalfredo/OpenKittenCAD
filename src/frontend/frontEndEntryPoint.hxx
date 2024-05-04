#ifndef FRONT_END_ENTRY_POINT
#define FRONT_END_ENTRY_POINT

#include <AIS_Shape.hxx>

typedef enum errorStatus {
    lexical_error,
    parser_error,
    semantic_error,
    other_error,
    success,
}errorStatus;


typedef std::vector<Handle(AIS_Shape)> shapesVector;


typedef struct programResponse{
    errorStatus status;
    char* message;
    shapesVector vec;
    
}programResponse;


typedef enum programType {
    file,
    string,
}programType;

programResponse* frontEndEntryPoint(const char* program, programType progType);

#endif
