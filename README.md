![image](./images/OpenKittenCad.png)


## High Level Overview
![image](./images/Overview.png)


## Dependencies 
OpenKittenCAD is built on the shoulders of giants.

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
If you find a issue please open a github issue.
If you wish to make a PR please open a github issue and link it with it. Unless its a change like grammar fix.
There is still a lot of work to be done. I work full time so progress on the project may be "slow". :)

## Building

### Specifiable Arguments 

| Variable Name          | Variable Type | Description                                 |
|------------------------|---------------|---------------------------------------------|
| VTK_DIR_ARG            | PATH          | Path to the VTK directory                   |
| OpenCASCADE_DIR_ARG    | PATH          | Path to the OpenCASCADE directory           |
| BUILD_TEST             | BOOL          | Enable or disable testing                   |
| BUILD_MAIN             | BOOL          | Enable or disable the main build            |


### Linux
```bash
$ mkdir build 
$ cd build
$ cmake [-options] ..
$ make
$ ./src/back-end/driver {path to .kts file}
```


### Windows
```

```

#### Mac
```

```
