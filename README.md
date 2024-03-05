![image](./images/OpenKittenCad.png)


## High Level Overview
![image](./images/Overview.png)


## Dependencies 
I am a true believer that for a project to be 
maintainable in the long term we have to keep
as little dependancies as possible. This project
depends on the following dependancies 

- Flex
- Bison
- OpenCascade
- VTK (Soon to be removed as a dependent)

> :warning: **Note**: Not listing any of the dependancies dependances!


## Grammar
```txt
#---------------------------------------
# Statements
#---------------------------------------

stmt      : block
          | functionStmt
          | declStmt
          | exprStmt
          | returnStmt


stmtList  : stmtList ';' stmt
          | stmt

#---------------------------------------

functionStmt    : 'fn' ID '(' paramDeclList ')' [ '->' TYPE ]? block

returnStmt      : 'return' expr

declStmt        :  ID ':' TYPE initOpt

exprStmt        : expr

block           : '{' stmtList* '}'

paramDeclList   : empty
                | paramDeclList ',' paramDecl
                | paramDecl

paramDecl       : ID ':' TYPE

initOpt         : [ ':=' expr ]?


#---------------------------------------
# Expressions
#---------------------------------------


expr            : expr binaryOpr expr
                | NUM 
                | TRANS 
                | ID 
                | ID '(' argList ')'

argList         : empty
                | argList ',' '%'
                | argList ',' expr
                | expr 
                | '%'
    


# Binary operators :
#   Pipe          : '|>'
#   Math          : '+' '-' '*' '/' 
```


## CheatSheet

[cheatsheet](./cheatsheet.md)

## Examples

[examples](./examples.md)


## Contributing
If you are wanting to contribute a good start is [Roadmap](https://github.com/notalfredo/OpenKittenCAD/issues/1).
If you find an issue please open a issue then link it with a pull request. Any help is appriciated :)

## Building

### Specifiable Arguments 
`VTK_DIR_ARG` PATH
`OpenCASCADE_DIR_ARG` PATH
`BUILD_TEST` BOOL
`BUILD_MAIN` BOOL

### Linux
```
cmake [-options] ..
make
```


### Windows
```

```

#### Mac
```

```
