// Emmanuel Colas
// Homework 4 hw4compiler.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// The symbol table that will hold the variable names,values,levels etc.
typedef struct
{
    int kind;
    char name[12];
    int value;
    int level;
    int address;
    int mark;
} Table;

// All of global variable to allow easier access to information. tokenlist[1000] will hold the tokens created by main.
// The indexes are used through the code as a place holder. There names represent what index they are placeholding.
Table symbolTable[100];
int tableIndex;          // index for addding to the symbol Table. You just add to the index instead of looking for next open spot
int codeindex;           // Assembling code index for jump and jpc instructions
int tokenIndex;          // Moves the index for the tokenList array
int lev;                 // this is the level for main and procedures
int assembly_code[1000]; // array to hold assembly code instruction.
char source[1000];       // array to hold input file
char token[12];
char tokenList[1000];
char temp[1];
FILE *output;

// A boolean for the lex analysis that check if the source code character we are looking at is an invalid character.
// If it finds one it'll return true and stop the program
bool check_invalid(char *source, int i) // This function is used to check that an identifier does not contain a invalid character

{
    if (source[i] == '!' || source[i] == '@' || source[i] == '_' || source[i] == '#' || source[i] == '$' || source[i] == '~' || source[i] == '?' || source[i] == '`' || source[i] == '%' || source[i] == '&' || source[i] == '^')
    {

        return true;
    }

    return false;
}
// These next three booleans are used as delimiters. The first one name delimiter will look for symbols that will mark the end of
//  a token.
bool delimiter(char *source, int i)
{
    if (source[i] == '+' || source[i] == '-' || source[i] == '*' || source[i] == '/' ||
        (source[i - 1] != ':' && source[i] == '=') || (source[i] == '<' && source[i + 1] == '>') || source[i] == '<' ||
        (source[i] == '<' && source[i + 1] == '=') || source[i] == '>' || (source[i] == '>' && source[i + 1] == '=') ||
        source[i] == '(' || source[i] == ')' || source[i] == ',' || source[i] == ';' || source[i] == '.' ||
        (source[i] == ':' && source[i + 1] == '='))
    {
        return true;
    }
    return false;
}

// This will check if the lexeme conatins a keyword
bool Vocab(char *lexeme)
{
    if (strcmp(lexeme, "begin") == 0 || strcmp(lexeme, "end") == 0 ||
        strcmp(lexeme, "if") == 0 || strcmp(lexeme, "then") == 0 || strcmp(lexeme, "while") == 0 ||
        strcmp(lexeme, "do") == 0 || strcmp(lexeme, "const") == 0 ||
        strcmp(lexeme, "var") == 0 || strcmp(lexeme, "write") == 0 ||
        strcmp(lexeme, "read") == 0 || strcmp(lexeme, "odd") == 0 ||
        strcmp(lexeme, "procedure") == 0 || strcmp(lexeme, "call") == 0 || strcmp(lexeme, "xor") == 0 || strcmp(lexeme, "else") == 0)
    {
        return true;
    }
    return false;
}

// Looks at a char in the lexeme and checks to see if it matches to a digit. Return true if it does.
bool checkDigit(char *lexeme, int i)
{
    temp[0] = lexeme[i];
    if (strcmp(temp, "1") == 0 || strcmp(temp, "2") == 0 || strcmp(temp, "3") == 0 || strcmp(temp, "4") == 0 || strcmp(temp, "5") == 0 ||
        strcmp(temp, "6") == 0 || strcmp(temp, "7") == 0 || strcmp(temp, "8") == 0 || strcmp(temp, "9") == 0 || strcmp(temp, "0") == 0)
    {
        return true;
    }

    return false;
}

// These "is" functions will check the lexeme and if it matches to a symbol,keyword, operator, etc
// it will tokenize the lexeme and add it to the tokenlist
void isOperator(char *lexeme, char tokenList[])
{
    if (strcmp(lexeme, "+") == 0)
    {
        strcat(tokenList, "4 ");
    }
    else if (strcmp(lexeme, "-") == 0)
    {
        strcat(tokenList, "5 ");
    }
    else if (strcmp(lexeme, "*") == 0)
    {
        strcat(tokenList, "6 ");
    }
    else if (strcmp(lexeme, "/") == 0)
    {
        strcat(tokenList, "7 ");
    }
}
void isSymbol(char *lexeme, char tokenList[])
{
    if (strcmp(lexeme, "xor") == 0)
    {
        strcat(tokenList, "8 ");
    }
    else if (strcmp(lexeme, "=") == 0)
    {
        strcat(tokenList, "9 ");
    }
    else if (strcmp(lexeme, "<>") == 0)
    {
        strcat(tokenList, "10 ");
    }
    else if (strcmp(lexeme, "<") == 0)
    {
        strcat(tokenList, "11 ");
    }
    else if (strcmp(lexeme, "<=") == 0)
    {
        strcat(tokenList, "12 ");
    }
    else if (strcmp(lexeme, ">") == 0)
    {
        strcat(tokenList, "13 ");
    }
    else if (strcmp(lexeme, ">=") == 0)
    {
        strcat(tokenList, "14 ");
    }
    else if (strcmp(lexeme, "(") == 0)
    {
        strcat(tokenList, "15 ");
    }
    else if (strcmp(lexeme, ")") == 0)
    {
        strcat(tokenList, "16 ");
    }
    else if (strcmp(lexeme, ",") == 0)
    {
        strcat(tokenList, "17 ");
    }
    else if (strcmp(lexeme, ";") == 0)
    {
        strcat(tokenList, "18 ");
    }
    else if (strcmp(lexeme, ".") == 0)
    {
        strcat(tokenList, "19 ");
    }
    else if (strcmp(lexeme, ":=") == 0)
    {
        strcat(tokenList, "20 ");
    }
}
void isKeyword(char *lexeme, char tokenList[])
{

    if (strcmp(lexeme, "begin") == 0)
    {
        strcat(tokenList, "21 ");
    }
    else if (strcmp(lexeme, "end") == 0)
    {
        strcat(tokenList, "22 ");
    }
    else if (strcmp(lexeme, "if") == 0)
    {
        strcat(tokenList, "23 ");
    }
    else if (strcmp(lexeme, "then") == 0)
    {
        strcat(tokenList, "24 ");
    }
    else if (strcmp(lexeme, "while") == 0)
    {
        strcat(tokenList, "25 ");
    }
    else if (strcmp(lexeme, "do") == 0)
    {
        strcat(tokenList, "26 ");
    }
    else if (strcmp(lexeme, "call") == 0)
    {
        strcat(tokenList, "27 ");
    }
    else if (strcmp(lexeme, "const") == 0)
    {
        strcat(tokenList, "28 ");
    }
    else if (strcmp(lexeme, "var") == 0)
    {
        strcat(tokenList, "29 ");
    }
    else if (strcmp(lexeme, "procedure") == 0)
    {
        strcat(tokenList, "30 ");
    }
    else if (strcmp(lexeme, "write") == 0)
    {
        strcat(tokenList, "31 ");
    }
    else if (strcmp(lexeme, "read") == 0)
    {
        strcat(tokenList, "32 ");
    }
    else if (strcmp(lexeme, "else") == 0)
    {
        strcat(tokenList, "33 ");
    }
    else if (strcmp(lexeme, "odd") == 0)
    {
        strcat(tokenList, "34 ");
    }
}
void isIdentfier(char *lexeme, char tokenList[])
{
    if (atoi(lexeme) == 0)
        strcat(tokenList, "2 ");
}
void isNumeral(char *lexeme, char tokenList[])
{

    if (atoi(lexeme) > 0 || strcmp(lexeme, "0") == 0)
    {
        strcat(tokenList, "3 ");
    }
}

/* emit puts our assembly code instructions into our assembly code array. This is so that we can change the value of the jump to main
address*/
void emit(int op, int L, int M)
{
    assembly_code[codeindex] = op;
    assembly_code[codeindex + 1] = L;
    assembly_code[codeindex + 2] = M;

    codeindex = codeindex + 3;
}

// This function is called when we are checking if a variable being declared has been declared or not.
//  Returns -1 if it is not in the symbol table(not declared) and the index if it is in the table
int SYMBOLTABLECHECK(char token[])
{
    for (int i = 99; i >= 0; i--)
    {

        if (strcmp(symbolTable[i].name, token) == 0 && lev == symbolTable[i].level && symbolTable[i].mark == 0)
            return i;
    }
    return -1;
}

/*This function had to be added to look for a variable value that may not be on the same level as the procedure calling it. It will look through
the symbol table for the variable that has mark value of zero. This mean that it can be accessed by the procedure.
*/
int SYMBOLTABLESEARCH(char token[])
{
    for (int i = 99; i >= 0; i--)
    {

        if (strcmp(symbolTable[i].name, token) == 0 && symbolTable[i].mark == 0)
            return i;
    }
    return -1;
}
// Adds main,variable,constants, and procedures to the symbol table and increases the index for the next.
void addTosymbolable(int kind, char name[], int number, int level, int address)
{
    symbolTable[tableIndex].kind = kind;
    strcpy(symbolTable[tableIndex].name, name);
    symbolTable[tableIndex].value = number;
    symbolTable[tableIndex].level = level;
    symbolTable[tableIndex].address = address;
    tableIndex++;
};

// Grabs the next token in the list and reassigns the token variable with it. Increases tokenIndex so that we can move
// along the tokenList array
void getToken()
{

    for (int t = 0; t < 12; t++)
    {
        token[t] = '\0';
    }
    while (tokenIndex < 1000)
    {
        temp[0] = tokenList[tokenIndex];
        if (strcmp(temp, " ") == 0)
            break;
        if (strcmp(temp, "/r") == 0)
            continue;
        strcat(token, temp);
        tokenIndex++;
    }
    tokenIndex++;
}

void FACTOR();
void BLOCK();

void TERM()
{
    FACTOR();
    // checks comapring for * and / respectively
    while (strcmp(token, "6") == 0 || strcmp(token, "7") == 0)
    {
        if (strcmp(token, "6") == 0)
        {
            getToken();
            FACTOR();
            emit(2, 0, 3); // emit MUL
        }
        else
        {
            getToken();
            FACTOR();
            emit(2, 0, 4); // emit div
        }
    }
}

void EXPRESSION()
{

    TERM(token, tokenList);
    // checks for + and - respectively
    while (strcmp(token, "4") == 0 || strcmp(token, "5") == 0)
    {
        if (strcmp(token, "4") == 0)
        {
            getToken();
            TERM(token, tokenList);
            emit(2, 0, 1); // emit ADD
        }
        else
        {
            getToken();
            TERM(token, tokenList);
            emit(2, 0, 2); // emit sub
        }
    }
}

void FACTOR()
{
    int symIdx = 0;
    if (strcmp(token, "2") == 0) // identsym
    {
        getToken();
        symIdx = SYMBOLTABLESEARCH(token);

        if (symIdx == -1)
        {
            printf("ERROR <Identifier not in the table>");
            exit(0);
        }

        if (symbolTable[symIdx].kind == 1)
        {
            emit(1, 0, symbolTable[symIdx].value); // LIT
        }
        else
        {
            emit(3, lev - symbolTable[symIdx].level, symbolTable[symIdx].address); // LOD
        }
    }
    else if (strcmp(token, "3") == 0) // numbersym
    {
        getToken();
        emit(1, 0, atoi(token)); // emit LIT
    }
    else if (strcmp(token, "15") == 0) // left Parenthesis sym
    {
        getToken();
        EXPRESSION();
        if (strcmp(token, "16") != 0) // right Parenthesis sym
        {
            printf("ERROR <Right parentthesis must follow left parenthesis>");
            exit(0);
        }
    }
    else
    {
        printf("ERROR <arithmetic equations must contain operands, parentheses, numbers, or symbols>");
        exit(0);
    }
    getToken();
}

void CONDITION()
{

    if (strcmp(token, "34") == 0) // oddsym
    {
        getToken();
        EXPRESSION();
        emit(2, 0, 11); // emit ODD
    }
    else
    {
        EXPRESSION();
        if (strcmp(token, "9") == 0) // equalsym
        {
            getToken();
            EXPRESSION();
            emit(2, 0, 5); // emit EQL
        }
        else if (strcmp(token, "10") == 0) // notequalsym
        {
            getToken();
            EXPRESSION();
            emit(2, 0, 6); // emit NotEQL
        }
        else if (strcmp(token, "11") == 0) // lessthansym
        {
            getToken();
            EXPRESSION();
            emit(2, 0, 7); // emit less
        }
        else if (strcmp(token, "12") == 0) // lessthanequalsym
        {
            getToken();
            EXPRESSION();
            emit(2, 0, 8); // emit lesseql
        }
        else if (strcmp(token, "13") == 0) // greatersym
        {
            getToken();
            EXPRESSION();
            emit(2, 0, 9); // emit greater
        }
        else if (strcmp(token, "14") == 0) // greaterthanequalsym
        {
            getToken();
            EXPRESSION();
            emit(2, 0, 10); // emit greatereq
        }
        else
        {
            printf("ERROR <Relational operator missing in conditional statement>");
            exit(0);
        }
    }
}

void STATEMENT()
{
    int index = 0;
    int jpcIdx = 0;
    int jmpIdx = 0;
    int loopIdx = 0;
    int symIdx = 0;
    if (strcmp(token, "2") == 0) // identsym
    {
        getToken();
        if (SYMBOLTABLESEARCH(token) == -1)
        {
            printf("ERROR <Undeclared identifier %s>", token);
            exit(0);
        }
        index = SYMBOLTABLESEARCH(token);
        if (symbolTable[index].kind != 2)
        {
            printf("ERROR <Only variables can be altered>");
            exit(0);
        }

        getToken();
        if (strcmp(token, "20") != 0) // becomesym
        {

            printf("ERROR <Assignment statements must use := >");
            exit(0);
        }
        getToken();
        EXPRESSION();
        emit(4, lev - symbolTable[index].level, symbolTable[index].address); // EMIT STO
    }

    if (strcmp(token, "27") == 0) // callsym
    {
        getToken();
        if (strcmp(token, "2") != 0) // identsym
        {
            printf("ERROR <Missing Identifier>");
            exit(0);
        }
        getToken();
        int index = SYMBOLTABLESEARCH(token);
        if (index == -1)
        {
            printf("ERROR <Undeclared Identifier> ");
            exit(0);
        }
        if (symbolTable[index].kind == 3) // checks if identifier is a procedure name
        {
            emit(5, lev - symbolTable[index].level, symbolTable[index].address); // EMIT CALL
        }
        else
        {
            printf("ERROR <call must be followed by a procedure identifier>");
            exit(0);
        }
        getToken();
    }
    if (strcmp(token, "21") == 0) // beginsym
    {
        do
        {
            getToken();
            STATEMENT();
        } while (strcmp(token, "18") == 0); // semicolonsym

        if (strcmp(token, "22") != 0) // endsym
        {                             // error recovery. If token doesnt equal end then it will look for the next semicolon;
            do
            {
                getToken();
            } while (strcmp(token, "18") != 0); // semicolonsym
            getToken();
        }

        if (strcmp(token, "22") != 0) // endsym
        {
            printf("ERROR <begin must be followed by end>");
            exit(0);
        }
        getToken();
    }
    if (strcmp(token, "23") == 0) // ifsym
    {
        getToken();
        CONDITION();
        jpcIdx = codeindex;
        emit(8, 0, jpcIdx);
        if (strcmp(token, "24") != 0) // thensym
        {
            printf("ERROR <if must be followed by then>");
            exit(0);
        }
        getToken();
        STATEMENT();
        assembly_code[jpcIdx + 2] = codeindex;
        return;
    }
    if (strcmp(token, "8") == 0) // XORsym
    {

        getToken();
        CONDITION();
        jpcIdx = codeindex;
        emit(8, 0, jpcIdx);
        if (strcmp(token, "24") != 0) // thensym
        {
            printf("ERROR <XOR must be followed by then>");
            exit(0);
        }
        getToken();
        STATEMENT();
        if (strcmp(token, "18") != 0) // semicolonsym
        {
            //  printf("ERROR <A statement should be followed by ; >"); // sends error code but parses to next semicolon !!!
            while (strcmp(token, "18") != 0)
            {
                getToken();
            }
            //  exit(0);
        }
        else
        {
            getToken();
        }
        if (strcmp(token, "33") != 0) // elsesym
        {
            printf("ERROR <then should be followed by else>");
            exit(0);
        }
        jmpIdx = codeindex;
        emit(7, 0, jmpIdx); // look at M again
        getToken();
        assembly_code[jpcIdx + 2] = codeindex;
        STATEMENT();
        assembly_code[jmpIdx + 2] = codeindex;
    }
    if (strcmp(token, "25") == 0) // whilesym
    {
        getToken();
        loopIdx = codeindex;
        CONDITION();
        if (strcmp(token, "26") != 0) // dosym
        {
            printf("ERROR <while must be followed by do>");
            exit(0);
        }
        getToken();
        jpcIdx = codeindex;
        emit(8, 0, jpcIdx);
        STATEMENT();
        emit(7, 0, loopIdx);
        assembly_code[jpcIdx + 2] = codeindex; // code[jpcIdx].M = current code index
    }
    if (strcmp(token, "32") == 0) // readsym
    {
        getToken();
        if (strcmp(token, "2") != 0)
        {
            printf("ERROR <Should be an IDENTIFIER>");
            exit(0);
        }
        getToken();
        symIdx = SYMBOLTABLECHECK(token);
        if (symIdx == -1)
        {
            printf("ERROR <%s is an undeclared identifier", token);
            exit(0);
        }
        if (symbolTable[symIdx].kind != 2)
        {
            printf("ERROR <Not a variable>");
            exit(0);
        }

        getToken();
        emit(9, 0, 2); // EMIT read
        emit(4, lev - symbolTable[symIdx].level, symbolTable[symIdx].address);
    }
    if (strcmp(token, "31") == 0) // writesym
    {
        getToken();
        EXPRESSION();
        emit(9, 0, 1); // emit WRITE
    }
}

void PROC_DECL()
{
    int proc_index;
    getToken();
    if (strcmp(token, "2") == 0) // identsym
    {
        getToken();
    }
    else
    {
        printf("ERROR <Need Procedure name>");
        exit(0);
    }
    if (SYMBOLTABLESEARCH(token) == -1)
    {
        addTosymbolable(3, token, 0, lev, codeindex); // codeindex as address could be changed later
        proc_index = SYMBOLTABLECHECK(token);         // used to keep index in symbol table
    }
    else
    {
        printf("ERROR <Duplicate procedure name>");
        exit(0);
    }
    getToken();
    if (strcmp(token, "18") == 0) // semicolonsym
    {
        getToken();
    }
    else
    {
        printf("ERROR <Expected a semi colon>");
        exit(0);
    }

    BLOCK(proc_index);
    if (strcmp(token, "18") == 0)
    {
        getToken();
    }
    else
    {
        printf("ERROR <Expected semicolon>");
        exit(0);
    }
    emit(2, 0, 0); // marks end of procedure or return
}

int VAR_DECL()
{
    int numVars = 0;
    if (strcmp(token, "29") == 0)
    {
        do
        {
            numVars++;
            getToken();
            if (strcmp(token, "2") != 0)
            {
                printf("ERROR <Var must be followed by an identifier>");
                exit(0);
            }
            getToken();
            if (SYMBOLTABLECHECK(token) != -1)
            {
                printf("ERROR <Var name %s has already been declared>", token);
                exit(0);
            }
            addTosymbolable(2, token, 0, lev, numVars + 2); // Maybe change to 3
            getToken();

        } while (strcmp(token, "17") == 0);
        if (strcmp(token, "18") != 0)
        {
            printf("ERROR <Variable declaration must be followed by a ; >"); // sends error code but parses to next semicolon
            exit(0);
        }
        getToken();
    }
    return numVars;
}

void CONST_DECL()
{
    char savedName[12] = {'\0'};
    do
    {
        getToken();
        if (strcmp(token, "2") != 0)
        {
            printf("ERROR <Constant must be followed by an identifier>");
            exit(0);
        }
        getToken();
        if (SYMBOLTABLECHECK(token) != -1)
        {
            printf("ERROR <Constant %s has already been declared>", token);
            exit(0);
        }

        strcpy(savedName, token);

        getToken();
        if (strcmp(token, "9") != 0)
        {
            printf("ERROR <Constant must be assigned with = >");
            exit(0);
        }
        getToken();
        if (strcmp(token, "3") != 0)
        {
            printf("ERROR <Constant must be assigned to an integer value >");
            exit(0);
        }
        getToken();
        addTosymbolable(1, savedName, atoi(token), 0, 0);

        getToken();
    } while (strcmp(token, "17") == 0);
    if (strcmp(token, "18") != 0)
    {
        printf("ERROR <Constant declarations must end with a ; >"); // sends error code but parses to next semicolon
        exit(0);
    }
    getToken();
}

void BLOCK(int proc_index)
{
    int numVars = 0;
    lev++;
    int space = 3;

    if (strcmp(token, "28") == 0)
        CONST_DECL();

    if (strcmp(token, "29") == 0)
    {
        numVars = VAR_DECL();
        space += numVars;
    }

    while (strcmp(token, "30") == 0)
    {
        PROC_DECL();
    }

    for (int i = proc_index + 1; i <= proc_index + numVars; i++) // marks every variable in the procedure as usable when the procedure is called
    {
        symbolTable[i].mark = 0;
    }

    symbolTable[proc_index].address = codeindex;
    emit(6, 0, space);
    STATEMENT();

    for (int i = proc_index + 1; i <= proc_index + numVars; i++) // marks every variable in the procedure as unusable when the procedure is returned
    {
        symbolTable[i].mark = 1;
    }
    lev--;
}

void PROGRAM()
{
    lev = -1;
    getToken();
    addTosymbolable(0, "main", 0, 0, 3);
    emit(7, 0, codeindex);
    BLOCK(0);
    assembly_code[2] = symbolTable[0].address;
    if (strcmp(token, "19") != 0)
    {
        printf("ERROR <Program must end with a period>");
        exit(0);
    }
    emit(9, 0, 3);
}

int main()
{
    tableIndex = 0;
    codeindex = 0;
    int count = 0;
    FILE *file = fopen("test.txt", "r");
    char source[1000] = {'\0'};
    char lexeme[100] = {'\0'};
    int j = 0;

    // read the source code file and parses to our source array.
    while (fscanf(file, "%c", &source[count]) == 1) // Assinging values to the array source code array
        count++;

    printf("%s\n\n", source);
    fclose(file);

    // Used to remove new line character because it was giving problems.
    for (int i = 0; i < count; i++)
    {
        if (source[i] == '\n')
        {
            source[i] = ' ';
        }
    }

    //  Looks at every character of source and and filters out comments and whitespaces
    for (int i = 0; i < count; i++)
    {

        if (source[i] == '/' && source[i + 1] == '*') // looks for /* in the source code
        {
            i = i + 2;
            while (source[i] != '*' || source[i + 1] != '/') // skips chars wntil it finds */ which marks the end of the comment
            {
                i++;
            }
            i = i + 2;
        }
        if (source[i] == ' ' || source[i] == '\n' || source[i] == '\t' || source[i] == '\r')
            continue;

        if (check_invalid(source, i) == true)
        {
            printf("%c invalid character", source[i]);
            exit(0);
        }

        // Copies source char one by one to lexeme
        lexeme[j] = source[i];
        j++;

        // this if statement is so that ":=", "<=", and ">=" can be read
        if ((lexeme[0] == ':' || lexeme[0] == '<' || lexeme[0] == '>') && (source[i + 1] == '=' || source[i + 1] == '>'))
        {
            continue;
        }

        // then is checked to see if any delimiters mark a token.
        if (delimiter(source, i + 1) == true || source[i + 1] == ' ' || delimiter(lexeme, 0) == true || Vocab(lexeme) == true || source[i + 1] == '\n' || source[i + 1] == '\r')
        {

            // checks if the lexeme is more than 11 charcters
            if (strlen(lexeme) > 12)
            {
                printf("%s is too long of a name for an Identifier", lexeme);
                exit(0);
            }
            // if the first char is a digit, checks the rest of the lexeme to make sure its not an identifier
            else if (checkDigit(lexeme, 0) == true)
            {
                int len = strlen(lexeme);
                for (int d = 0; d < len; d++)
                {
                    for (int e = d + 1; e < len; e++)
                    {
                        if (checkDigit(lexeme, e) == false)
                        {
                            printf("%s Identifier can not begin with a number", lexeme);
                            exit(0);
                            // break;
                        }
                    }
                }

                // then checks if the lexeme is only 5 digits
                if (atoi(lexeme) >= 100000)
                {
                    printf("%s is over 5 digits", lexeme);
                    exit(0);
                }
                // if it gets to here that means its a number
                else
                {
                    isNumeral(lexeme, tokenList);
                    strcat(tokenList, lexeme);
                    strcat(tokenList, " ");
                }
            }

            // checks that the lexeme is and identifier and not a keyword, or symbol
            else if (delimiter(lexeme, 0) == false && check_invalid(lexeme, 0) == false && Vocab(lexeme) == false)
            {

                isIdentfier(lexeme, tokenList);
                strcat(tokenList, lexeme);
                strcat(tokenList, " ");
            }
            // Finally checks if the lexeme is keyword,symbol or operator and adds to tokenList
            else
            {
                isOperator(lexeme, tokenList);
                isSymbol(lexeme, tokenList);
                isKeyword(lexeme, tokenList);
            }

            // resets lexeme for next token
            for (int j = 0; j < 20; j++)
            {
                lexeme[j] = '\0';
            }
            j = 0;
        }
    }

    //________________________--Parser--________________________________________________________________

    output = fopen("elf.txt", "w");
    PROGRAM();
    printf("No errors, program is syntactically correct\n");
    printf("Assembly Code:\n");
    printf("Line\tOP\tL\tM\n");
    int line = 0;
    int op;
    for (int l = 0; l < 999; l += 3)
    {
        if (assembly_code[l] == 0 && assembly_code[l + 1] == 0 && assembly_code[l + 2] == 0)
            break;
        fprintf(output, "%d %d %d\n", assembly_code[l], assembly_code[l + 1], assembly_code[l + 2]);
        printf("%d\t", line);
        switch (op = assembly_code[l])
        {
        case 1:
            printf("LIT");
            break;
        case 2:
            printf("OPR");
            break;
        case 3:
            printf("LOD");
            break;
        case 4:
            printf("STO");
            break;
        case 5:
            printf("CAL");
            break;
        case 6:
            printf("INC");
            break;
        case 7:
            printf("JMP");
            break;
        case 8:
            printf("JPC");
            break;
        case 9:
            printf("SYS");
            break;
        }
        printf("\t%d\t%d\n", assembly_code[l + 1], assembly_code[l + 2]);
        line++;
    }
    return 0;
}