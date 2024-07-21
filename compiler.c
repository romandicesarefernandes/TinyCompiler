// Alexander Charkiewicz and Roman Aurelio Di cesare Fernandes
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *input; // input file pointer

int k = 0;
int i = 0;       // i used to loop through tokens
int j = 0;       // j used to loop through symbol table
int error = 0;   // when this is 1 we ran into an error
int tempo = 0;   // This is temporary to get track of lines in emit
int tokens[256]; // Tokens! this is the key of the program
int LoopSym = 0;
int lev = 0;
int max = 100;
int tx, level;
int dx, tx0, cx0;
int cx = 0;
int jmpcodeaddr;

void Program();
void Block();
void ConstDeclaration();
int VarDeclaration();
void Statement();
void Condition();
void Expression();
void Term();
void Factor();
int ProcedureDeclaration();

// print function

typedef struct {
  int kind;      // const = 1, var = 2, proc = 3
  char name[10]; // name up to 11 chars
  int val;       // number (ASCII value)
  int el;        // L el
  int addr;      // M address
  int mark;      // to indicate unavailable or deleted
} symbol;

typedef struct {
  char name[10];
  int M;
  int L;

} Instructions;

Instructions Instruction[500];
symbol symbol_table[500]; // Declaration osf the symbol table
/*
void print() {

  if (error == 0)
    printf("\nNo errors, program is syntactically correct\n\n");

  printf("Assembly Code:\n\n");
  printf("Line\tOP\tL\tM\t\n");

  for (int i = 0; i < tempo; i++) {

    printf("%3d\t%s\t%d\t%d\n", i, Instruction[i].name, Instruction[i].L,
           Instruction[i].M);
  }

  return;
}
*/
/*
void print() {
  if (error == 0)
    printf("\nNo errors, program is syntactically correct\n\n");

  printf("Assembly Code:\n\n");
  // printf("OP\tL\tM\t\n");

  for (int i = 0; i < tempo; i++) {
    int opCode = -1;

    if (strcmp(Instruction[i].name, "LIT") == 0)
      opCode = 1;
    else if (strcmp(Instruction[i].name, "ADD") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "SUB") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "MUL") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "DIV") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "EQL") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "NEQ") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "LSS") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "LEQ") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "GTR") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "GEQ") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "RTN") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "LOD") == 0)
      opCode = 3;
    else if (strcmp(Instruction[i].name, "STO") == 0)
      opCode = 4;
    else if (strcmp(Instruction[i].name, "CAL") == 0)
      opCode = 5;
    else if (strcmp(Instruction[i].name, "INC") == 0)
      opCode = 6;
    else if (strcmp(Instruction[i].name, "JMP") == 0)
      opCode = 7;
    else if (strcmp(Instruction[i].name, "JPC") == 0)
      opCode = 8;
    else if (strcmp(Instruction[i].name, "SYS") == 0)
      opCode = 9;

    printf("%d\t%d\t%d\n", opCode, Instruction[i].L, Instruction[i].M);
  }

  return;
}
*/
void print() {
  if (error == 0)
    printf("\nNo errors, program is syntactically correct\n\n");

  printf("Assembly Code:\n\n");
  printf("Line\tOP\tL\tM\t\n");

  FILE *file = fopen("elf.txt", "w"); // Open file for writing

  for (int i = 0; i < tempo; i++) {
    int opCode = -1;

    if (strcmp(Instruction[i].name, "LIT") == 0)
      opCode = 1;
    else if (strcmp(Instruction[i].name, "ADD") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "SUB") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "MUL") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "DIV") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "EQL") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "NEQ") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "LSS") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "LEQ") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "GTR") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "GEQ") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "RTN") == 0)
      opCode = 2;
    else if (strcmp(Instruction[i].name, "LOD") == 0)
      opCode = 3;
    else if (strcmp(Instruction[i].name, "STO") == 0)
      opCode = 4;
    else if (strcmp(Instruction[i].name, "CAL") == 0)
      opCode = 5;
    else if (strcmp(Instruction[i].name, "INC") == 0)
      opCode = 6;
    else if (strcmp(Instruction[i].name, "JMP") == 0)
      opCode = 7;
    else if (strcmp(Instruction[i].name, "JPC") == 0)
      opCode = 8;
    else if (strcmp(Instruction[i].name, "SYS") == 0)
      opCode = 9;

    printf("%3d\t%s\t%d\t%d\n", i, Instruction[i].name, Instruction[i].L,
           Instruction[i].M);

    fprintf(file, "%d %d %d\n", opCode, Instruction[i].L,
            Instruction[i].M); // Write to file
  }

  fclose(file); // Close file

  return;
}
void printsymbols() {

  printf("\nSymbol Table:\n\n");
  printf("Kind | Name        | Value | el | Address | Mark\n");
  printf("---------------------------------------------------\n");

  for (int h = 0; h < j; h++) {

    printf("%4d | %11s | %5d | %5d | %7d | %5d\n", symbol_table[h].kind,
           symbol_table[h].name, symbol_table[h].val, symbol_table[h].el,
           symbol_table[h].addr, symbol_table[h].mark);
  }
}

void emit(char *Str, int M, int L) {

  strcpy(Instruction[tempo].name, Str);
  Instruction[tempo].M = M;
  Instruction[tempo].L = L;
  tempo++;

  return;
}

int SymbolTableCheck(char *name) {
  int i;
  for (i = 0; i < 500; i++) {
    if (strcmp(symbol_table[i].name, name) == 0) {
      return i; // symbol found
    }
  }
  return -1; // symbol not found
}

// converts ascii tokens to string so SymbolTableCheck can be used
char *tokenstostring() {
  char *word = malloc(100 * sizeof(char));
  char letter;
  i++; // update from identifier token (2) to first ascii token
  while (tokens[i] >= 65 && tokens[i] <= 90 ||
         tokens[i] >= 97 && tokens[i] <= 122) {
    letter = tokens[i];
    strncat(word, &letter, 1);
    i++;
  }
  // the tokens updated to the token after ascii and the while loop check failed
  // so we -- so the code matchs the pseudocode with the next token statements
  i--;
  return word;
}

int Reserved_Or_Not(char word[20]) {

  if (strcmp(word, "begin") == 0)
    return 21;

  if (strcmp(word, "var") == 0)
    return 29;

  if (strcmp(word, "const") == 0)
    return 28;

  if (strcmp(word, "procedure") == 0)
    return 30;

  if (strcmp(word, "call") == 0)
    return 27;

  if (strcmp(word, "end") == 0) {
    return 22;
  }

  if (strcmp(word, "if") == 0)
    return 23;

  if (strcmp(word, "then") == 0)
    return 24;

  if (strcmp(word, "else") == 0)
    return 33;

  if (strcmp(word, "while") == 0)
    return 25;

  if (strcmp(word, "do") == 0)
    return 26;

  if (strcmp(word, "read") == 0)
    return 32;

  if (strcmp(word, "write") == 0)
    return 31;

  if (strcmp(word, "odd") == 0)
    return 8;

  return -1;
}

// checks if the string is a number
int IsNumber(char *word) {

  for (int i = 0; i < sizeof(word); i++) {

    if (word[i] <= 122 & word[i] >= 97 | word[i] <= 90 & word[i] >= 65)
      return 0;
  }

  return 1;
}

int main(int argc, char **argv) {

  char *filename;
  char info[1024]; // Large string that  will hold everything
  char temp;
  int count = 0;
  int comment = 0;

  // Check if file name is given in command line, if so it stores it in a
  // variable, if not it ends the program
  if (argc > 1) {
    filename = argv[1];
  } else {
    printf("No file was included in the command line. Please try again with a "
           "file name.\n");
    return 0;
  }

  FILE *input = fopen(filename, "r"); // Create a pointer for the file

  // print outside of loop to state we will be printing the source program
  printf("Source Program:\n");

  while (temp != EOF) { // Loop to get all input from file given

    temp = fgetc(input); // Get the imput char by char and store

    // print the source program before processing it (getting rid of comments,
    // spaces etc)
    if (temp != EOF)
      printf("%c", temp);

    if (temp == ' ' | temp == '\n')
      continue; // Check for white spaces and new lines

    // ingnore comments
    // we check if there is a / or if there already was one,
    // we updated comment and can bypass this check, this is used often in this
    // section of code
    if (comment >= 1 || temp == '/') {
      // if * does not immediately follow / then the / is for division so we
      // stop processing it as a comment comment being 1 means we are checking
      // the next char after / because comment updates to 1 later
      if (comment == 1 && temp != '*') {
        // if the char after / is not * then we set comment to 0
        comment = 0;
        // we also store / and the current char to info because its division
        info[count] = '/';
        count++;
        info[count] = temp;
        count++;
        continue;
      }
      // if the next char is * then continue processing the comment,
      // or if we already had a * and comment is >= 2, we can bypass it
      if (comment >= 2 || temp == '*') {
        // while ignoring the comment text if we notice a * we check if its
        // followed by a /, if so we close the comment if not then * is part of
        // the comment text and we reset to skipping and looking for a */
        if (comment >= 3 || temp == '*') {
          // if * isn't followed by / then * is likely part of the comment's
          // text so we go back to comment being 2 and checking for *
          if (comment >= 3 && temp != '/') {
            comment = 2;
            continue;
          }

          // if * is followed by / then the comment is over
          if (temp == '/') {
            comment = 0;
            continue;
          }
          comment = 3;
          continue;
        }
        comment = 2;
        continue;
      }
      comment = 1;
      continue;
    }

    info[count] = temp; // Fill array
    count++;
  }

  printf("\n");

  info[count] = '\0'; // Set a terminator character
                      // printf("\n\n\n");

  // this means that a comment has not been closed
  if (comment != 0)
    printf("Error: The input ended during a comment\n");

  int j = 0;
  int i = 0;
  int CFG = 0; // keeps track of context free grammar, if var was processed then
               // an identifier is expected, etc
  char word[50];
  // when testing sometimes the case for the identifier being too long would get
  // triggered immediately my best guess is this happened because some garbage
  // value was in word[11] and that caused it to evaluate to true, this loop
  // avoids that issue by making sure every index is empty
  for (i = 0; i < 50; i++)
    word[i] = ' ';

  // reset i to 0 so we can use it later
  i = 0;

  // printf("Lexeme Table: \n\n");
  // printf("lexeme\t\t\t\t token type\n");

  // loop that processes the input
  while (info[i] != '\0') {

    if (info[i] >= 48 & info[i] <= 57 | info[i] >= 97 & info[i] <= 122 |
        info[i] >= 65 & info[i] <= 90) {

      while (info[i] >= 48 & info[i] <= 57 | info[i] >= 97 & info[i] <= 122 |
             info[i] >= 65 & info[i] <= 90) {
        // store the word one char at a time, add \0 at the end so values after
        // the j index don't affect the word comparisons
        word[j] = info[i];
        word[j + 1] = '\0';

        // if a var, const, or comma was processed, then we know to expect an
        // identifier so if it starts with a digit we will have an error since
        // identifiers can't start with a digit
        if (CFG == 1) {
          CFG = 0;
          if (isdigit(word[0]))
            printf("Error: Identifiers can't begin with a digit\n");
        }

        // if an "if" was processed we expect a "then"
        if (CFG == 2 && info[i + 1] == 't' && info[i + 2] == 'h' &&
            info[i + 3] == 'e' && info[i + 4] == 'n') {
          // if this is true then "then" starts at info[i+1] so we process what
          // we currently have in word
          CFG = 0;
          j = 0;
          break;
        }

        // if the first char is a digit, then when the next char isn't a digit
        // we stop and process the number
        if (isdigit(word[0]) && !isdigit(word[j])) {
          word[j] = '\0';
          i--;
          k++;
          j = 0;
          break;
        }

        // checks that the number isn't longer than 5 digits
        if (isdigit(word[0]) && isdigit(word[5])) {
          printf("Error #2: number is longer than 5 digits\n");
          k++;
          j = 0;
        }

        // checks that the identifier isn't longer than 11 characters
        if (isalpha(word[0]) && (isdigit(word[11]) || isalpha(word[11]))) {
          printf(
              "Error #1: identifier is longer than 11 characters in length\n");
          k++;
          j = 0;
        }

        // if a reserved word is detected, process it
        if (Reserved_Or_Not(word) > 0) {
          tokens[k] = Reserved_Or_Not(word);
          k++;
          j = 0;
          break;
        }

        j++;
        i++;
      }
      if (Reserved_Or_Not(word) > 0) {
        // if a var or const was processed then we expect and identifier after
        if (strcmp(word, "var") == 0 || strcmp(word, "const") == 0)
          CFG = 1;
        // if a "if" was processed sets flag that "then" is expected
        if (strcmp(word, "if") == 0)
          CFG = 2;

        //     printf("%-32s %-32d\n", word, Reserved_Or_Not(word));
        memset(word, 0, sizeof(word));

      } else {
        // check if word is a number, if so print it
        if (IsNumber(word)) {
          tokens[k] = 3;
          //       printf("%-32s %-32d\n", word, tokens[k]);
          k++;
          tokens[k] = atoi(word);
          k++;
        } else // if its not a number then its an identifier
        {
          tokens[k] = 2;
          //     printf("%-32s %-32d\n", word, tokens[k]);
          k++;
          int m = 0;
          while (word[m] != '\0') {
            tokens[k] = word[m];
            m++;
            k++;
          }
        }
        j = 0;
        memset(word, 0, sizeof(word));
      }
    }

    // process specific symbols
    if (info[i] == ':') {
      if (info[i + 1] == '=') {
        tokens[k] = 20;
        //      printf(":=\t\t\t\t %d\n", tokens[k]);
        k++;
      }
    } else if (info[i] == ',') {
      // if a comma is detected then there could be an identifier that starts
      // with a number after it so we flag CFG
      CFG = 1;
      tokens[k] = 17;
      //    printf(",\t\t\t\t %d\n", tokens[k]);
      k++;
    } else if (info[i] == ';') {
      tokens[k] = 18;
      //      printf(";\t\t\t\t %d\n", tokens[k]);
      k++;
    } else if (info[i] == '.') {
      tokens[k] = 19;
      //   printf(".\t\t\t\t %d\n", tokens[k]);
      k++;
    } else if (info[i] == '+') {
      tokens[k] = 4;
      //   printf("+\t\t\t\t %d\n", tokens[k]);
      k++;
    } else if (info[i] == '-') {
      tokens[k] = 5;
      //   printf("=\t\t\t\t %d\n", tokens[k]);
      k++;
    } else if (info[i] == '*') {
      tokens[k] = 6;
      //    printf("*\t\t\t\t %d\n", tokens[k]);
      k++;
    } else if (info[i] == '/') {
      tokens[k] = 7;
      //   printf("/\t\t\t\t %d\n", tokens[k]);
      k++;
    } else if (info[i] == '=') {
      if (info[i - 1] != ':') {
        tokens[k] = 9;
        //    printf("=\t\t\t\t %d\n", tokens[k]);
        k++;
      }
    } else if (info[i] == '<') {
      tokens[k] = 11;
      //  printf("<\t\t\t\t %d\n", tokens[k]);
      k++;
    } else if (info[i] == '>') {
      tokens[k] = 13;
      //   printf(">\t\t\t\t %d\n", tokens[k]);
      k++;
    } else if (info[i] == '(') {
      tokens[k] = 15;
      //   printf("(\t\t\t\t %d\n", tokens[k]);
      k++;
    } else if (info[i] == ')') {
      tokens[k] = 16;
      //   printf(")\t\t\t\t %d\n", tokens[k]);
      k++;
    } else if (isalpha(info[i]) || isdigit(info[i]) || info[i] == 13 ||
               info[i] == EOF || info[i] == '\t') {
      ;
    } else { // if none of the previous cases were true then the symbol is
             // invalid
      printf("Error: %c is not a valid symbol\n", info[i]);
    }

    i++; // carefull with this
  }

  Program();
  if (error == 0) {
    print();

    // printsymbols();
  }

  // printf("\nNo errors, program is syntactically correct\n");
  return 0;
}

void Program() {

  symbol_table[j].kind = 3;
  strcpy(symbol_table[j].name, "main");
  symbol_table[j].val = 0;
  symbol_table[j].mark = 1;
  symbol_table[j].el = lev;
  symbol_table[j].addr = 3;
  j++;

  Block();

  // if (error != 0)
  // return;
  // no period error

  emit("SYS", 3, 0);
}

void Block() {

  // call consts
  ConstDeclaration();
  if (error != 0)
    return;
  // cal var
  int numvars = VarDeclaration();
  // if (numvars)
  //  emit("INC", 3 + numvars, 0);

  int space = ProcedureDeclaration();
  // lev = 0;

  emit("JMP", 2, 0);
  emit("INC", 3 + numvars, 0);
  dx = 0;

  Statement();
  if (error != 0)
    return;
}

int ProcedureDeclaration() {

  int spacereserve = 3;
  if (tokens[i] == 30) {

    //  printf("We got to procedure\n The next token is %d", tokens[i + 1]);
    jmpcodeaddr = tempo; // Remember in which position of the symboltable we are
    emit("JMP", 0, 0);   // Jump instruction, second 0  is gonna be changed.

    if (lev == max)
      printf("Error: Too many nested functions(PROCEDURE)\n");
    // while(tokens[i] != 22){
    do {
      if (tokens[i] == 28) { // Case we are deling with a constant
        i++;                 // Get next symbol

        do {

          ConstDeclaration();

          while (tokens[i] == 17) {

            i++;
            ConstDeclaration();
          }

          if (tokens[i] == 18)
            i++;

          else
            printf("Error #5: Semicolon or comma missing\n");

        } while (tokens[i] == 2);
        spacereserve++;
      } // Case we are dealing with a constant.

      if (tokens[i] == 29) {
        i++;

        do {

          int numvars = VarDeclaration();
          // if (numvars)
          // emit("INC", 3 + numvars, 0);

          while (tokens[i] == 17) {

            i++;
            numvars = VarDeclaration();
            // if (numvars)
            // emit("INC", 3 + numvars, 0);
          }

          if (tokens[i] == 18)
            printf("Error\n");

        } while (tokens[i] == 2);
        spacereserve++;
      } // Case we are dealing with a variable

      while (tokens[i] == 30) {
        i++;

        if (tokens[i] == 2) {

          symbol_table[j].kind = 3;
          strcpy(symbol_table[j].name, tokenstostring());
          symbol_table[j].val = 0;
          symbol_table[j].mark = 1;
          symbol_table[j].el = lev;
          symbol_table[j].addr = 3;
          j++;
          i++;

        }

        else
          printf("Error identifier expected\n");

        if (tokens[i] == 18) {
          i++;

        } else
          printf("Error #5: Semicolon or comma missing\n");

        // dx++;
        lev++;
        Block();

        if (tokens[i] == 18)
          i++;

        else
          printf("Error #5: Semicolon or comma missing\n");
      }

    } while ((tokens[i] == 28) || (tokens[i] == 29) || (tokens[i] == 30));
  }
  //}
  Instruction[jmpcodeaddr].M = (tempo + 1) * 3;

  // Statement();
  // symbol_table[jmpcodeaddr].addr = spacereserve;

  // Statement();
  // emit("SYS", 0, 0);
  // printf("THE SYMBOL IS %d////////////////\n", tokens[i - 2]);
  //  ProcedureDeclaration();

  // print();
  // printsymbols();
  // Statement();

  return spacereserve;
}

void ConstDeclaration() {

  char *identifier = malloc(100 * sizeof(char));

  // const check
  if (tokens[i] == 28) {
    do {
      // get next token
      i++;
      // no identifier error
      if (tokens[i] != 2) {
        printf("Error: const declaration not followed by an identifier\n");
        error = 1;
        return;
      }
      // use function to translate ascii tokens to the identifier string then
      // store string so we can store that name in the symbol table
      identifier = tokenstostring();

      // symbol already declared error
      if (SymbolTableCheck(identifier) != -1) {
        // printf("Error: symbol name has already been declared\n");
        //  error = 1;
        //  return;
      }
      // name is already saved to a var

      i++;
      // const not followed by = error
      if (tokens[i] != 9) {
        printf("Error: constants must be assigned with =\n");
        error = 1;
        return;
      }
      i++;
      if (tokens[i] != 3) {
        printf("Error: constants must be assigned to an integer value\n");
        error = 1;
        return;
      }
      // add to symbol table (kind 1, saved name, number, 0, 0)
      symbol_table[j].kind = 1;
      strcpy(symbol_table[j].name, identifier);
      symbol_table[j].val = tokens[i++];
      symbol_table[j].mark = 1;
      symbol_table[j].el = lev;
      j++;
      i++;

    } while (tokens[i] == 17 /*while = comma*/);

    // no semicolon error
    if (tokens[i] != 18) {
      printf("Error: const declaration not followed by a semicolon\n");
      error = 1;
      // i++;
      return;
    }
    i++;
  }
}

int VarDeclaration() {

  int numVars = 0;
  char *identifier = malloc(100 * sizeof(char));
  if (tokens[i] == 29) {
    //  printf("IM IN VAR MY GUY\n\n"); //**********************
    do {
      numVars++;
      i++;
      // no identifier error
      if (tokens[i] != 2) {
        printf("Error: var declaration not followed by an identifier\n");
        error = 1;
        return numVars;
      }
      // use function to translate ascii tokens to the identifier string then
      // store string so we can store that name in the symbol table
      identifier = tokenstostring();

      // symbol already declared error
      if (SymbolTableCheck(identifier) != -1) {
        // printf("Error: symbol name has already been declared\n");
        //  error = 1;
        return numVars;
      }
      // add to symbol table (kind 2, ident, 0, 0, var# + 2)
      symbol_table[j].kind = 2;
      strcpy(symbol_table[j].name, identifier);
      symbol_table[j].val = 0;
      symbol_table[j].el = lev;
      symbol_table[j].addr = numVars + 2;
      symbol_table[j].mark = 1;
      j++;
      i++;

    } while (tokens[i] == 17 /*while = comma*/);

    // no semicolon error
    if (tokens[i] != 18) {

      //   printf("\n\nIM NOT A ; IM A %d\n", tokens[i]);
      printf("Error: var declaration not followed by a semicolon\n");
      error = 1;
      return numVars;
    }
    i++;
  }
  return numVars;
}

void Statement() {

  int symIdx, jpcIdx, loopIdx;
  if (tokens[i] == 2) {
    symIdx = SymbolTableCheck(tokenstostring());
    if (symIdx == -1) {
      printf("Error: undeclared identifier NUMBER 1\n");
      error = 1;
      return;
    }
    if (symbol_table[symIdx].kind != 2) {
      printf("Error: only variable values may be altered\n");
      error = 1;
      return;
    }
    i++;
    if (tokens[i] != 20) {
      printf("Error: assignment statements must use :=\n");
      error = 1;
      return;
    }
    i++;
    // call expression
    Expression();
    if (error != 0) {
      // return;
    }
    // printf("symIDX is %d", symIdx);
    emit("STO", symbol_table[symIdx].addr, symbol_table[symIdx].el);
    return;
  }

  // NEW CODE HerE

  if (tokens[i] == 27) {

    i++;

    if (tokens[i] != 2)
      printf("Error #14: Call must be followed by an identifier\n");

    else {

      int pos = SymbolTableCheck(tokenstostring());

      if (pos == 0)
        printf("Error #11: Undeclared identifier\n");

      else if (symbol_table[pos].kind == 3) {

        emit("CAL", symbol_table[pos].addr - 1, lev - symbol_table[pos - 1].el);

      }

      else {
        printf("Error #15: Call of a constant or variable is meaningless\n");
      }

      // printf("Kind is: %d and POS is %d***********\n",
      // symbol_table[pos].kind,       pos);
      i++;
    }

    // lev++;
  }

  // END OF NEW  CODE

  if (tokens[i] == 21) {
    do {
      i++;
      // call statement
      Statement();

      if (error != 0) {
        return;
      }

    } while (tokens[i] == 18);

    i++;
    return;
  }
  if (tokens[i] == 23) {
    i++;
    // call condition
    Condition();
    if (error != 0) {
      return;
    }
    jpcIdx = i;
    emit("JPC", 0, lev);

    if (tokens[i] != 24) {
      printf("Error: if must be followed by then\n");
      error = 1;
      return;
    }
    i++;
    // call statement
    Statement();
    if (error != 0) {
      return;
    }
    symbol_table[jpcIdx].addr = i;
    return;
  }
  if (tokens[i] == 25) {
    i++;
    loopIdx = i;
    // call condition
    Condition();
    if (error != 0) {
      return;
    }
    if (tokens[i] != 26) {
      printf("Error: while must be followed by do\n");
      error = 1;
      return;
    }
    i++;
    jpcIdx = i;
    emit("JPC", 0, lev);
    // call statement
    Statement();
    if (error != 0) {
      return;
    }
    emit("JMP", loopIdx, 0);
    symbol_table[jpcIdx].addr = i;
    return;
  }
  if (tokens[i] == 32) {
    i++;
    if (tokens[i] != 2) {
      printf("Error: read must be followed by an identifier\n");
      error = 1;
      return;
    }
    symIdx = SymbolTableCheck(tokenstostring());
    if (symIdx == -1) {
      printf("Error: undeclared identifier NUMBER 2\n");
      error = 1;
      return;
    }
    if (symbol_table[symIdx].kind != 2) {
      printf("Error: identifier is not a variable\n");
      error = 1;
      return;
    }
    i++;

    emit("SYS", 2, 0);
    emit("STO", symbol_table[symIdx].addr, lev);

    return;
  }
  if (tokens[i] == 31) {
    i++;

    // call expression
    Expression();
    if (error != 0) {
      return;
    }
    emit("SYS", 1, 0);
    return;
  }

  if (tokens[i] == 21) {
    do {
      i++;
      // call statement
      Statement();

      if (error != 0) {
        return;
      }

    } while (tokens[i] == 18);

    if (tokens[i] != 22) {
      printf("Error: begin must be followed by end\n");
      error = 1;
      return;
    }
    i++;
    return;
  }
  if (tokens[i] == 23) {
    i++;
    // call condition
    Condition();
    if (error != 0) {
      return;
    }
    jpcIdx = i;
    emit("JPC", 0, lev);
    if (tokens[i] != 24) {
      printf("Error: if must be followed by then\n");
      error = 1;
      return;
    }
    i++;
    // call statement
    Statement();
    if (error != 0) {
      return;
    }
    symbol_table[jpcIdx].addr = i;
    return;
  }
  if (tokens[i] == 25) {
    i++;
    loopIdx = i;
    // call condition
    Condition();
    if (error != 0) {
      return;
    }
    if (tokens[i] != 26) {
      printf("Error: while must be followed by do\n");
      // error = 1;
      return;
    }
    i++;
    jpcIdx = i;
    emit("JPC", 0, lev);
    // call statement
    Statement();
    if (error != 0) {
      return;
    }
    emit("JMP", loopIdx, lev);
    symbol_table[jpcIdx].addr = i;
    return;
  }
  if (tokens[i] == 32) {
    i++;
    if (tokens[i] != 2) {
      printf("Error: read must be followed by an identifier\n");
      // error = 1;
      return;
    }
    symIdx = SymbolTableCheck(tokenstostring());
    if (symIdx == -1) {
      printf("Error: undeclared identifier NUMBER 3\n");
      error = 1;
      return;
    }
    if (symbol_table[symIdx].kind != 2) {
      printf("Error: identifier is not a variable\n");
      error = 1;
      return;
    }
    i++;

    emit("SYS", 2, 0);
    emit("STO", symbol_table[symIdx].addr, lev);

    return;
  }
  if (tokens[i] == 31) {
    i++;

    // call expression
    Expression();
    if (error != 0) {
      return;
    }
    emit("SYS", 1, 0);
    return;
  }
}

void Condition() {

  if (tokens[i] == 8) {
    i++;
    // call expression
    Expression();
    if (error != 0) {
      return;
    }
    emit("ODD", 11, 0);
  } else {
    // call expression
    Expression();
    if (error != 0) {
      return;
    }
    if (tokens[i] == 9) {
      i++;
      // call expression
      Expression();
      if (error != 0) {
        return;
      }
      emit("EQL", 5, 0);
    } else if (tokens[i] == 10) {
      i++;
      // call expression
      Expression();
      if (error != 0) {
        return;
      }
      emit("NEQ", 6, 0);
    } else if (tokens[i] == 11) {
      i++;
      // call expression
      Expression();
      if (error != 0) {
        return;
      }
      emit("LSS", 7, 0);
    } else if (tokens[i] == 12) {
      i++;
      // call expression
      Expression();
      if (error != 0) {
        return;
      }
      emit("LEQ", 8, 0);
    } else if (tokens[i] == 13) {
      i++;
      // call expression
      Expression();
      if (error != 0) {
        return;
      }
      emit("GTR", 9, 0);
    } else if (tokens[i] == 14) {
      i++;
      // call expression
      Expression();
      if (error != 0) {
        return;
      }
      emit("GEQ", 11, 0);
    } else {
      printf("Error: condition must contain comparison operator\n");
      error = 1;
      return;
    }
  }
}

void Expression() {

  if (tokens[i] == 5) {
    i++;
    Term();
    if (error != 0) {
      return;
    }
    emit("NEG", 0, 0);
    while (tokens[i] == 4 || tokens[i] == 5) {
      if (tokens[i] == 4) {
        i++;
        Term();
        if (error != 0) {
          return;
        }
        emit("ADD", 1, 0);
      } else {
        i++;
        Term();
        if (error != 0) {
          return;
        }
        emit("SUB", 2, 0);
      }
    }
  } else {
    if (tokens[i] == 4)
      i++;
    Term();
    if (error != 0) {
      return;
    }
    while (tokens[i] == 4 || tokens[i] == 5) {
      if (tokens[i] == 4) {
        i++;
        Term();
        if (error != 0) {
          return;
        }
        emit("ADD", 1, 0);
      } else {
        i++;
        Term();
        if (error != 0) {
          return;
        }
        emit("SUB", 2, 0);
      }
    }
  }
}

void Term() {

  Factor();
  if (error != 0) {
    return;
  }
  // an announcement said to not include MOD so we'll skip that
  while (tokens[i] == 6 || tokens[i] == 7) {
    if (tokens[i] == 6) {
      i++;
      Factor();
      if (error != 0) {
        return;
      }
      emit("MUL", 3, 0);
    } else if (tokens[i] == 7) {
      i++;
      Factor();
      if (error != 0) {
        return;
      }
      emit("DIV", 4, 0);
    } else {
      ;
    }
  }
}

void Factor() {

  // for some reason that I couldn't find out
  // a 0 was put in the tokens array so we just skip it
  if (tokens[i] == 0)
    i++;

  if (tokens[i] == 2) {
    int symidx = SymbolTableCheck(tokenstostring());
    // printf("IN FACTOR: %d", tokens[i]);

    if (symbol_table[symidx].kind == 1)
      emit("LIT", symbol_table[symidx].val, 0);
    else
      emit("LOD", symbol_table[symidx].addr, lev);

    i++;
  } else if (tokens[i] == 3) {
    emit("LIT", tokens[++i], 0);
    // I think we need to ++ twice because its at 3, then skip the number, then
    // to the next identifier
    i++;
  } else if (tokens[i] == 15) {
    i++;
    Expression();
    if (error != 0) {
      return;
    }
    if (tokens[i] != 16) {
      printf("Error: right parenthesis must follow left parenthesis\n");
      error = 1;
      return;
    }
    i++;
  } else {
    printf("Error: arithmetic equations must contain operands\n");
    error = 1;
    return;
  }
}
