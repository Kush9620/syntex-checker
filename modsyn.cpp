#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iterator>
#include <regex>
#include <vector>
#include <string>

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 1000


typedef struct {
    int lineno;
    int linelen;
    char linetext[MAX_LINE_LENGTH];
} fileline;
struct Line {
    std::string text;
    int lineno;
};
void print(const fileline str[], int len);
int search(char arr[], int len);
void bracket(fileline arr[], int len);
void keyword(fileline arr[], int length);
void function_and_prototype_count(fileline arr[], int len);
void checkkeyword(fileline arr[], int len);
void builtinfunction(fileline arr[], int len);
void varcount(fileline arr[], int len);
int getscheck(char arr[], int size);
int putscheck(char arr[], int size);
int fprintfcheck(char arr[], int size);
int fscanfcheck(char arr[], int size);
int printcheck(char arr[], int size);
int scancheck(char arr[], int size);
int forcheck(char arr[], int size);
int whilecheck(char arr[], int size);
void fcheck(fileline arr[], int len);
int semicolon_check(const char* str, int len);
int forcheck(const char* str, int len);
int whilecheck(const char* str, int len);
int ifcheck(const char* str, int len);
int classcheck(const char* str);
int cincheck(const char* str);
int coutcheck(const char* str);
int newcheck(const char* str);

void checkForErrors(const std::vector<Line>& lines) {
    bool foundError = false;

    // Regex to detect access specifier without a colon, ensuring it's a standalone specifier
    std::regex accessSpecifierWithoutColon(R"(^\s*(public|private|protected)\s*(?!:)\s*$)");

    for (const auto& line : lines) {
        // Check if line contains only an access specifier without a colon
        if (std::regex_match(line.text, accessSpecifierWithoutColon)) {
            // Found an access specifier missing a colon
            std::cout << "Error: Access specifier (public/private/protected) should be followed by ':' ---> line no: " 
                      << line.lineno << std::endl;
            foundError = true;
        }
    }

    if (!foundError) {
        std::cout << "No syntax errors found." << std::endl;
    }
}

// Function to check if a string is a pointer
int is_pointer(char str[]) {
    if (str[0] == '*') {    //This checks if the first character of the string is an (*)
        return 1;  // return 1 if the string starts with *, indicating it might be a pointer declaration (Match found)
    }
    return 0;  // No match found
}

// Function to extract variable names and check for pointers  // char str[]: This is the input string (likely a line of code) from which the function extracts variable names.
void extract_variable_names_and_check_pointers(char str[], char var_names[][MAX_LINE_LENGTH], int *count) {
    int i, j, k;
    for (i = 0, j = 0; str[i] != '\0'; i++) {  //loop goes through each character in the string
        if (isalpha(str[i]) || str[i] == '_') { // checks if the character is an alphabetic character (isalpha()) or an underscore (_) If it finds such a character, it treats this as the start of a potential variable name.
            k = 0;
            while (isalnum(str[i]) || str[i] == '_') { //isalnum(str[i]) checks for alphanumeric characters (letters and digits)
                var_names[*count][k++] = str[i++];
            }
            var_names[*count][k] = '\0'; //After capturing the full variable name, the function adds a null terminator (\0) to mark the end of the string
            (*count)++;
            // Check if the variable is a pointer
            if (is_pointer(var_names[*count])) {
                printf("Pointer variable: %s\n", var_names[*count]);
            }
        }
    }
}

// Function to check for pointer declaration
void check_pointer_declaration(fileline arr[], int len) {
    int i, j;
    for (i = 0; i < len; i++) {
        for (j = 0; j < arr[i].linelen; j++) {
            if (arr[i].linetext[j] == '*') {
                // Check if the asterisk is used for pointer declaration
                if (j > 0 && isalpha(arr[i].linetext[j - 1])) {
                    printf("Pointer declaration found at line %d: %s\n", arr[i].lineno, arr[i].linetext);
                }
            }
        }
    }
}

// Call the function to check for pointer declaration
void fcheck1(fileline arr[], int len) {
    check_pointer_declaration(arr, len);
}

// Function to remove tabs and spaces
void remove_tabs_and_spaces(char str[]) {
    int i, j;
    for (i = 0, j = 0; str[i] != '\0'; i++) {
        if (str[i] != '\t' && str[i] != ' ') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

// Function to extract variable names
void extract_variable_names(char str[], char var_names[][MAX_LINE_LENGTH], int *count) {
    int i, j, k;
    for (i = 0, j = 0; str[i] != '\0'; i++) {
        if (isalpha(str[i]) || str[i] == '_') {
            k = 0;
            while (isalnum(str[i]) || str[i] == '_') {
                var_names[*count][k++] = str[i++];
            }
            var_names[*count][k] = '\0';
            (*count)++;
        }
    }
}

// Function to check if a string is a keyword
int is_keyword(char str[]) {
    const char* keywords[] = {
        "auto", "double", "int", "struct", "break", "else", "long", "switch", "case",
        "enum", "register", "typedef", "char", "extern", "return", "union", "const",
        "float", "short", "unsigned", "continue", "for", "signed", "void", "default",
        "goto", "sizeof", "volatile", "do", "if", "static", "while"
    };
    
    int i;
    for (i = 0; i < 32; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;  // Match found
        }
    }
    return 0;  // No match found
}
// Function to check if a string is a data type
int is_data_type(char str[]) {
    const char* data_types[] = {"int", "char", "float", "double", "long", "short"};

    int i;
    for (i = 0; i < 6; i++) {
        if (strcmp(str, data_types[i]) == 0) {
            return 1;  // Match found
        }
    }
    return 0;  // No match found
}
int searchinstring(char str[], int len) {
    // Dummy search function for comments
    // You should implement your actual logic here
    return -1;  // For now, assuming no comments are found
}
int main() {
    FILE *input;
    fileline str[MAX_LINES];
    char str1[MAX_LINE_LENGTH];
    int totallen, j, i = 0, len, found;
    input = fopen("input.txt", "r");
    while (fgets(str1, MAX_LINE_LENGTH, input) != NULL) {
        len = strlen(str1); // length of a line
        found = search(str1, len); // position of comment if exists
        if (str1[0] != '\n' && found == -1) {
            str[i].lineno = i + 1;
            str[i].linelen = len;
            strcpy(str[i].linetext, str1);
            i++;
        }
        else if (str1[0] != '\n' && found != -1) {
            str[i].lineno = i + 1;
            for (j = 0; j < found; j++) str[i].linetext[j] = str1[j];
            str[i].linetext[found] = '\n';
            str[i].linelen = strlen(str[i].linetext);
            i++;
        }
    }
    totallen = i;
    printf("SYNTAX CHECKER BY KUSH SHARMA\n\n\n\n");
    printf("\t\tOUTPUT PRINTED ON TEXT FILE NAME output.txt\n\n\n");
    print(str, totallen);

	printf("\n\n\n\t\t\tKEYWORDS with LINE NUMBER\n\n\n");
	keyword(str,totallen);
	//protocheck(str,totallen);
	printf("\n\n\n\t\t\tTOTAL NUMBER OF BUILTIN FUNCTIONS USED\n\n\n");
	builtinfunction(str,totallen);
	printf("\n\n\n\t\t\tTOTAL FUNCTIONS USED INCLUDING MAIN FUNCTION\n\n\n");
	function_and_prototype_count(str,totallen);
	printf("\n\n\n\t\t\tVARIABLES WITH COUNTS\n\n\n");
	varcount(str,totallen);
	
	
	printf("\n\n\n\t\t\tSYNTAX CHECK\n\n");
	//printscan(str,totallen);
	fcheck(str,totallen);

	printf("\n\n------BRACKET ,PARANTHESIS Check-------\n\n\n\n");
    bracket(str,totallen);
	
	printf("\n\n\n\t\t\tTOTAL LINES IN PROGRAM\n\n\n");
	printf("Total line in Program are: %d",totallen);
	return 0;
} 
void print(const fileline str[], int len) { // Function to print lines from an array of fileline structures to a text file
    FILE * fout; // Declare a file pointer for the output file
    int i; // Loop variable

    // Open the output file "output.txt" for writing ("w" mode)
    fout = fopen("output.txt", "w");
    // Check if the file was opened successfully
    if (fout == NULL) {
        // Print an error message if the file could not be opened
        perror("Error opening file");
        return; // Exit the function if the file could not be opened
    }

    // Iterate through each element in the array
    for (i = 0; i < len; i++) {
        // Write the line number, line length, and line text to the output file
        fprintf(fout, "line %d [%d]: %s", str[i].lineno, str[i].linelen, str[i].linetext);
    }

    // Close the output file
    fclose(fout);
}


int search(char arr[], int len) {    // Function to search for the start of a comment in a character array
    // Iterate through the array up to the second last element
    for (int i = 0; i < len - 1; i++) {
        // Check if the current character is '/' and the next character is also '/'
        if (arr[i] == '/' && arr[i + 1] == '/') {
            // If found, return the index of the first '/' of the comment
            return i;
        }
    } 
    // If no comment is found, return -1
    return -1;
}

void keyword(fileline arr[], int len) {   // Function to identify and print keywords found in an array of fileline structures
    char str[100]; // array to hold the current line of text
    char a[100];   // array to hold individual words extracted from the line
    // Array of known keywords in C
    char sync[32][10] = {
        "auto", "double", "int", "struct", "break", "else", "long", "switch",
        "case", "enum", "register", "typedef", "char", "extern", "return",
        "union", "const", "float", "short", "unsigned", "continue", "for",
        "signed", "void", "default", "goto", "sizeof", "volatile", "do",
        "if", "static", "while"
    };

    int i, j, l; // Loop variables

    // Iterate through each line in the array
    for (i = 0; i < len; i++) {
        int pos = 0; // Position tracker for the current line
        int k = 0;   // Index for the word array

        // Copy the line text into the str array
        strcpy(str, arr[i].linetext);
        
        // Iterate through each character in the current line
        for (j = 0; j < arr[i].linelen; j++) {
            // Check for (space, parentheses, null terminator, tab)
            if (str[j] == ' ' || str[j] == '(' || str[j] == '\0' || str[j] == '\t') {
                a[k] = '\0'; // Null-terminate the current word
                // Compare the extracted word with known keywords
                for (l = 0; l < 32; l++) {
                    // If a keyword match is found, print the line number and keyword
                    if (strcmp(a, sync[l]) == 0) {
                        printf("Line %d: %s\n", arr[i].lineno, a);
                    }
                }
                k = 0; // Reset the word array index
                pos = j + 1; // Update position for the next word
            } else {
                // If character is part of a word, add it to the buffer
                a[k] = str[j];
                k++; // Increment the buffer index
            }
        }
    }
}	

void bracket(fileline arr[], int len) {
    int *p;
    int i, j;
    p = (int*) calloc(3, sizeof(int));  // p[0] for '{}', p[1] for '()', p[2] for '[]'
    
    int linea1[20], linea2[20], lineb1[20], lineb2[20], linec1[20], linec2[20];
    int a = 0, a2 = 0, b = 0, b2 = 0, c = 0, c2 = 0;

    int inLoopOrVarDecl = 0;  // Flag to track if inside a loop or variable declaration

    // Loop through each line and character
    for (i = 0; i < len; i++) {
        inLoopOrVarDecl = 0;  // Reset flag for each line

        // Check if the line is part of a loop or variable declaration
        if (strstr(arr[i].linetext, "for") || strstr(arr[i].linetext, "while") || strstr(arr[i].linetext, "if")) {
            inLoopOrVarDecl = 1;  // Set flag if loop or control structure
        } else if (strstr(arr[i].linetext, "int") || strstr(arr[i].linetext, "float") || strstr(arr[i].linetext, "char") ||
                   strstr(arr[i].linetext, "double") || strstr(arr[i].linetext, "long")) {
            inLoopOrVarDecl = 1;  // Set flag for variable declarations
        }

        for (j = 0; j < arr[i].linelen; j++) {
            // Check for opening and closing braces, parentheses, and brackets
            if (arr[i].linetext[j] == '{') {
                (*p)++;
                linea1[a++] = arr[i].lineno;  // Store the line number for opening '{'
            } else if (arr[i].linetext[j] == '}') {
                if (*p > 0) {
                    (*p)--;  // Decrement if there's an unmatched '{'
                    a--;     // Remove last unmatched '{' since it's paired
                } else {
                    linea2[a2++] = arr[i].lineno;  // Store the line number for unmatched '}'
                }
            } else if (arr[i].linetext[j] == '(') {
                (*(p + 1))++;
                lineb1[b++] = arr[i].lineno;  // Store the line number for opening '('
            } else if (arr[i].linetext[j] == ')') {
                if (*(p + 1) > 0) {
                    (*(p + 1))--;  // Decrement if there's an unmatched '('
                    b--;           // Remove last unmatched '(' since it's paired
                } else {
                    lineb2[b2++] = arr[i].lineno;  // Store the line number for unmatched ')'
                }
            } else if (arr[i].linetext[j] == '[') {
                (*(p + 2))++;
                linec1[c++] = arr[i].lineno;  // Store the line number for opening '['
            } else if (arr[i].linetext[j] == ']') {
                if (*(p + 2) > 0) {
                    (*(p + 2))--;  // Decrement if there's an unmatched '['
                    c--;           // Remove last unmatched '[' since it's paired
                } else {
                    linec2[c2++] = arr[i].lineno;  // Store the line number for unmatched ']'
                }
            }
        }

        // Specific check for mismatched parentheses in variable declarations and loops
        if (inLoopOrVarDecl && (*(p + 1) != 0)) {  // If in a loop/var declaration and mismatched '()'
            printf("Parentheses mismatch in loop or variable declaration ---> line no: %d\n", arr[i].lineno);
            (*(p + 1)) = 0;  // Reset parentheses count for the next line
        }
    }

    // Report only unmatched '{' and '}'
    while (a > 0) {
        printf("line %d: { has no matching }\n", linea1[--a]);
    }
    while (a2 > 0) {
        printf("line %d: } has no matching {\n", linea2[--a2]);
    }

    // Report only unmatched '(' and ')'
    while (b > 0) {
        printf("line %d: ( has no matching )\n", lineb1[--b]);
    }
    while (b2 > 0) {
        printf("line %d: ) has no matching (\n", lineb2[--b2]);
    }

    // Report only unmatched '[' and ']'
    while (c > 0) {
        printf("line %d: [ has no matching ]\n", linec1[--c]);
    }
    while (c2 > 0) {
        printf("line %d: ] has no matching [\n", linec2[--c2]);
    }

    free(p);  // Free the dynamically allocated memory
}
// Function to count occurrences of built-in functions in given lines of code
void builtinfunction(fileline arr[],int len){ 
	char str[100],a[100];
	char sync[30][10]={"printf","scanf","gets","puts","fsanf","fprintf","fgets","fputs","fputc","fgetc","fopen","fclose","feof","fflush","malloc","calloc","rand","strcmp","strlen","strcpy","strncpy","strncmp","tolower","toupper","strrev","getch","strcat","strncat","sqrt","pow"};		
	int i,j,l,total=0;
	
	for (i=0;i<len;i++){
		int pos=0,k=0;
		strcpy(str,arr[i].linetext);
		for(j=0;j<arr[i].linelen;j++){
			if (str[j]==' '||str[j]=='('|| str[j]=='\0'||str[j]=='\t'){
				a[k]='\0';
				for(l=0;l<30;l++){
					if (strcmp(a,sync[l])==0) total+=1;
				}
				k=0;
				pos=j+1;
			}
			else {a[k]=str[j]; k++;}
		}
	}
	printf("Total Builtin Functions: %d\n",total);
}

void printscan(fileline arr[], int size) { //printscan is designed to scan through lines of a file (or code) and identify whether any of the lines contain printf or scanf statements.
    int i, j, k, l, x;
    char str[100];

    // Loop through each line in the array `arr` of type `fileline`
    for (i = 0; i < size; i++) {
        // Skip leading spaces and tabs in each line
        // The loop continues until it finds a non-space, non-tab character
        for (x = 0; (arr[i].linetext[x] == ' ' || arr[i].linetext[x] == '\t'); x++);

        // Print the number of leading spaces/tabs skipped (for debugging or tracking purposes)
        printf("%d", x);

        // Copy the line from the first non-space/tab character into `str`
        // `arr[i].linetext` is offset by `x` (the number of spaces/tabs skipped)
        for (int a = 0; a < arr[i].linelen - x; a++) {
            str[a] = arr[i].linetext[x + a];
        }

        // Check if the current line starts with "printf"
        // `strncmp` compares the first 6 characters of the line with the string "printf"
        if (strncmp(arr[i].linetext, "printf", 6) == 0) {
            // Call `printcheck` to validate or check the content of the printf line
            // `str` is the trimmed version of the line (without leading spaces)
            printcheck(str, arr[i].linelen);
            printf("printf exist\n");  // Print a message indicating that "printf" was found
        }

        // Check if the current line starts with "scanf"
        // `strncmp` compares the first 5 characters of the line with the string "scanf"
        if (strncmp(arr[i].linetext, "scanf", 5) == 0) {
            // Call `scancheck` to validate or check the content of the scanf line
            // `str` is the trimmed version of the line (without leading spaces)
            scancheck(str, arr[i].linelen);
            printf("scanf exist\n");  // Print a message indicating that "scanf" was found
        }
    }
}

int printcheck(char arr[], int size) {
    int pcount = 0; // Counter for '%' characters (for printf format specifiers)
    int ccount = 0; // Counter for ',' characters (for printf argument separation)
    int icount = 0; // Counter for '"' characters (to check for balanced quotes)
    int opcount = 0; // Counter for '<<' operators (for cout stream insertion)
    int isCout = 0;  // Flag to determine if we're checking a cout statement
    int f = 0;       // Flag to indicate inconsistencies (0 = no issue, 1 = issue)

    // First, determine if the line contains printf or cout
    if (strstr(arr, "printf")) {
        // C's printf statement
        for (int i = 0; i < size; i++) {
            if (arr[i] == '%') pcount++;
            if (arr[i] == ',') ccount++;
            if (arr[i] == '\"') icount++;
        }
        
        // Check for matching number of format specifiers (%) and arguments (commas)
        if (pcount != ccount) f = 1;
        // Check if quotes are balanced
        if (icount % 2 != 0) f = 1;
    }
    else if (strstr(arr, "cout")) {
        // C++'s cout statement
        isCout = 1;
        
        for (int i = 0; i < size; i++) {
            // Count double quotes for balanced string literals
            if (arr[i] == '\"') icount++;
            
            // Check for '<<' operator in cout
            if (arr[i] == '<' && i + 1 < size && arr[i + 1] == '<') {
                opcount++;
                i++; // Skip the next character since we've already counted this '<<'
            }
        }

        // Check if quotes are balanced
        if (icount % 2 != 0) f = 1;
        // If there’s a cout but no '<<' operator, mark it as an error
        if (opcount == 0) f = 1;
    }

    // Return 0 if checks passed, 1 if there are inconsistencies
    return f;
}

int scancheck(char arr[], int size) {
    int pcount = 0;    // Counter for '%' characters (format specifiers in scanf)
    int ccount = 0;    // Counter for ',' characters (argument separation in scanf)
    int icount = 0;    // Counter for '"' characters (to check balanced quotes)
    int acount = 0;    // Counter for '&' characters (required for variable addresses in scanf)
    int scount = 0;    // Counter for "%s" (string format specifier in scanf)
    int opcount = 0;   // Counter for '>>' operators (for cin)
    int isCin = 0;     // Flag to determine if the line contains a cin statement
    int f = 0;         // Flag to indicate inconsistencies (0 = no issue, 1 = issue)

    // Check if the line contains scanf or cin
    if (strstr(arr, "scanf")) {
        // C's scanf statement
        for (int i = 0; i < size; i++) {
            if (arr[i] == '%') pcount++;
            if (arr[i] == ',') ccount++;
            if (arr[i] == '\"') icount++;
            if (arr[i] == '&') acount++;
            if (arr[i] == '%' && i + 1 < size && arr[i + 1] == 's') scount++;
        }

        // Check for matching number of format specifiers and arguments
        if (pcount != ccount) f = 1;
        // Check if quotes are balanced
        if (icount % 2 != 0) f = 1;
        // Ensure correct usage of '&' based on the number of non-string specifiers
        if ((pcount - scount) != acount) f = 1;
    }
    else if (strstr(arr, "cin")) {
        // C++'s cin statement
        isCin = 1;

        for (int i = 0; i < size; i++) {
            // Count double quotes for balanced quotes
            if (arr[i] == '\"') icount++;
            
            // Check for '>>' operator in cin
            if (arr[i] == '>' && i + 1 < size && arr[i + 1] == '>') {
                opcount++;
                i++; // Skip the next character since we've already counted this '>>'
            }
        }

        // Check if quotes are balanced
        if (icount % 2 != 0) f = 1;
        // If there’s a cin but no '>>' operator, mark it as an error
        if (opcount == 0) f = 1;
    }

    // Return 0 if checks passed, 1 if there are inconsistencies
    return f;
}

int fprintfcheck(char arr[], int size) {
    int pcount = 0;    // Counter for '%' characters (format specifiers in fprintf)
    int ccount = 0;    // Counter for ',' characters (argument separation in fprintf)
    int icount = 0;    // Counter for '"' characters (to check balanced quotes)
    int opcount = 0;   // Counter for '<<' operators (for ofstream output)
    int isOfstream = 0; // Flag to determine if the line contains an ofstream statement
    int f = 0;         // Flag to indicate inconsistencies (0 = no issue, 1 = issue)

    // Check if the line contains fprintf or an ofstream output statement
    if (strstr(arr, "fprintf")) {
        // C's fprintf statement
        for (int i = 0; i < size; i++) {
            if (arr[i] == '%') pcount++;
            if (arr[i] == ',') ccount++;
            if (arr[i] == '\"') icount++;
        }

        // Check for matching number of format specifiers and arguments
        if (pcount != ccount - 1) f = 1;
        // Check if quotes are balanced
        if (icount % 2 != 0) f = 1;
    }
    else if (strstr(arr, "<<")) {
        // C++'s ofstream output (cout or file output)
        isOfstream = 1;

        for (int i = 0; i < size; i++) {
            // Count double quotes for balanced quotes
            if (arr[i] == '\"') icount++;

            // Check for '<<' operator in ofstream or cout statements
            if (arr[i] == '<' && i + 1 < size && arr[i + 1] == '<') {
                opcount++;
                i++; // Skip the next character since we've already counted this '<<'
            }
        }

        // Check if quotes are balanced
        if (icount % 2 != 0) f = 1;
        // If there's an ofstream statement but no '<<' operator, mark it as an error
        if (opcount == 0) f = 1;
    }

    // Return 0 if checks passed, 1 if there are inconsistencies
    return f;
}

int getscheck(char arr[], int size) {
    int f = 0;        // Flag for indicating errors (0 = no issue, 1 = issue)
    int isCin = 0;    // Flag to determine if the line contains a `cin` statement
    int hasInvalidChars = 0; // Tracks any invalid characters in `gets` or `cin` statements

    // Check if the line contains `gets` (C) or `cin` (C++)
    if (strstr(arr, "gets")) {
        // C's gets statement - this is deprecated and should raise a warning
        f = 1; // Flag the use of `gets` as an issue (unsafe function)
        
        // Check if any format specifiers or other unexpected symbols are used
        for (int i = 0; i < size; i++) {
            if (arr[i] == '%' || arr[i] == ',' || arr[i] == '\"' || arr[i] == '&' || arr[i] == '\'') {
                hasInvalidChars = 1;
                break;
            }
        }
    } 
    else if (strstr(arr, "cin")) {
        // C++'s cin statement
        isCin = 1;

        for (int i = 0; i < size - 1; i++) {
            // Check for the `>>` operator in cin statements
            if (arr[i] == '>' && arr[i + 1] == '>') {
                i++; // Skip next character since we've already counted this '>>'
            }
            // Track any format specifiers or characters not expected in `cin` statements
            else if (arr[i] == '%' || arr[i] == ',' || arr[i] == '\"' || arr[i] == '&' || arr[i] == '\'') {
                hasInvalidChars = 1;
                break;
            }
        }

        // If `cin` is found without `>>` operators or invalid characters, flag an error
        if (isCin && hasInvalidChars) f = 1;
    }

    return f; // Return 1 if there are errors or issues, 0 if the line is valid
}

int putscheck(char arr[], int size) {
    int f = 0;           // Flag to indicate issues (0 = no issue, 1 = issue)
    int isCout = 0;      // Flag to indicate if `cout` is detected
    int hasInvalidChars = 0; // Tracks if any invalid characters are found in `puts` or `cout` statements

    // Check if the line contains `puts` (C) or `cout` (C++)
    if (strstr(arr, "puts")) {
        // Check for invalid characters in `puts` statements
        for (int i = 0; i < size; i++) {
            if (arr[i] == '%' || arr[i] == ',' || arr[i] == '&' || arr[i] == '\'') {
                hasInvalidChars = 1;
                break;
            }
        }
        
        // If there are invalid characters, set the flag
        if (hasInvalidChars) f = 1;
    }
    else if (strstr(arr, "cout")) {
        // Mark that `cout` is present
        isCout = 1;

        // Check for proper use of `<<` and absence of invalid characters
        for (int i = 0; i < size - 1; i++) {
            // Detect the `<<` operator for `cout` statements
            if (arr[i] == '<' && arr[i + 1] == '<') {
                i++; // Move past the `<<` operator
            }
            // Check for any invalid symbols in `cout` statements
            else if (arr[i] == '%' || arr[i] == ',' || arr[i] == '&' || arr[i] == '\'') {
                hasInvalidChars = 1;
                break;
            }
        }

        // If `cout` is present but has invalid characters, flag it as an error
        if (isCout && hasInvalidChars) f = 1;
    }

    return f; // Return 1 if issues are found, otherwise 0
}

const char* built_in_functions[] = {
    // C Standard Library functions
    "printf", "scanf", "strlen", "strcpy", "strncpy", "sprintf", "fopen", "fclose",
    "fscanf", "fprintf", "malloc", "free", "strcmp", "strcat", "strtok", "puts", "gets",

    // C++ Standard Library utilities
    "cin", "cout", "endl", "new", "delete",

    // C++ String library functions
    "std::string", "std::getline", "std::stoi", "std::stof", "std::stod",

    // C++ Vector and algorithm utilities (if needed)
    "std::vector", "std::sort", "std::reverse"
};
// Calculate the total number of elements in the built_in_functions array
const int built_in_function_count = sizeof(built_in_functions) / sizeof(built_in_functions[0]);
// sizeof(built_in_functions) gives the total size of the array in bytes
// sizeof(built_in_functions[0]) gives the size of a single element in bytes
// Dividing the total size by the size of one element gives the number of elements in the array


// Helper function to check if a function is built-in
int is_builtin_function(const char* function_name) {
    for (int i = 0; i < built_in_function_count; i++) {
        if (strcmp(function_name, built_in_functions[i]) == 0) {
            return 1;  // Built-in function
        }
    }
    return 0;  // Not built-in
}

void function_and_prototype_count(fileline arr[], int len) {
    char str[100], function_name[100];
    char return_types[7][10] = { "int", "char", "void", "double", "float", "long", "short" };
    int totalFunctions = 0, totalPrototypes = 0;
    int mainFound = 0;

    // Loop to find user-defined functions
    for (int i = 0; i < len; i++) {
        int strlength = arr[i].linelen;
        strcpy(str, arr[i].linetext);
        int has_return = 0;
        
        // Check if the line starts with a return type (for both C and C++)
        for (int j = 0; j < 7; j++) {
            char* found_return = strstr(str, return_types[j]);
            if (found_return != NULL && (found_return == str || isspace(*(found_return - 1)))) {
                has_return = 1;
                break;
            }
        }

        // For C++: Check for member function syntax (e.g., class_name::function)
        if (strchr(str, ':') != NULL && strchr(str, '(') != NULL) {
            // Handle C++ member functions with scope resolution (::)
            char* start = strchr(str, '(');
            int pos = (int)(start - str) - 1;
            while (pos > 0 && (isalnum(str[pos]) || str[pos] == '_')) {
                pos--;
            }
            strncpy(function_name, &str[pos + 1], start - &str[pos + 1]);
            function_name[start - &str[pos + 1]] = '\0';

            // Skip if it's a built-in function
            if (!is_builtin_function(function_name)) {
                // Check if it is a function definition (does not end with ';')
                if (str[strlength - 2] != ';') {
                    totalFunctions++;
                    if (strcmp(function_name, "main") == 0) {
                        mainFound = 1;
                    }
                } else {
                    // It is a function prototype
                    totalPrototypes++;
                }
            }
        }
        // For both C and C++: Check if it's a non-member function
        else if (has_return && strstr(str, "(") && strstr(str, ")")) {
            // Extract function name before the '('
            char* start = strchr(str, '(');
            int pos = (int)(start - str) - 1;
            while (pos > 0 && (isalnum(str[pos]) || str[pos] == '_')) {
                pos--;
            }
            strncpy(function_name, &str[pos + 1], strchr(str, '(') - &str[pos + 1]);
            function_name[strchr(str, '(') - &str[pos + 1]] = '\0';

            // Skip if it's a built-in function
            if (!is_builtin_function(function_name)) {
                // Check if it is a function definition (does not end with ';')
                if (str[strlength - 2] != ';') {
                    totalFunctions++;
                    if (strcmp(function_name, "main") == 0) {
                        mainFound = 1;
                    }
                } else {
                    // It is a function prototype
                    totalPrototypes++;
                }
            }
        }
    }

    // Output results
    printf("Total user-defined functions found including main function: %d\n", totalFunctions);
    if (mainFound) {
        printf("Main function found.\n");
    } else {
        printf("Main function not found!\n");
    }

    printf("\n\n\n\t\t\tPROTOTYPE CHECK\n\n\n");
    printf("Total user-defined prototypes: %d\n", totalPrototypes);

    if (totalPrototypes < totalFunctions) {
        printf("Some function prototypes are missing.\n");
    } else {
        printf("All function prototypes are defined.\n");
    }
}

// Function to count the variables of different types in C and C++ code
void varcount(fileline arr[], int len) {
    char str[100], a[100];
    // Extended sync array to include C++ data types like bool, string, wchar_t
    char sync[9][15] = { "int", "char", "long", "double", "float", "short", "bool", "std::string", "wchar_t" }; 
    int i, j, l, total[9] = { 0 };  // Arrays to hold the count of each data type

    for (i = 0; i < len; i++) {
        int pos = 0, k = 0;
        strcpy(str, arr[i].linetext);  // Copy line text to local variable `str`
        int strlength = arr[i].linelen;  // Get length of the current line

        for (j = 0; j < arr[i].linelen; j++) {
            // Check for space, '(', end of string, or tab as a word separator
            if (str[j] == ' ' || str[j] == '(' || str[j] == '\0' || str[j] == '\t') {
                a[k] = '\0';  // Terminate the current word
                for (int l = 0; l < 9; l++) {  // Loop through the sync array to check if the word is a data type
                    if (strcmp(a, sync[l]) == 0) {
                        pos = j;  // Store the current position

                        // Check if the line is not a function declaration (a crude check)
                        // This part could lead to errors if the closing parenthesis is not at these exact positions.
                        if ((str[strlength - 3] != ')') && (str[strlength - 2] != ')')) {
                            // Count the variables by checking for ',' or ';'
                            for (int x = pos; x < strlength; x++) {
                                if (str[x] == ',' || str[x] == ';') {
                                    total[l]++;
                                }
                            }
                        }
                    }
                }
                k = 0;  // Reset k to start storing the next word
            }
            else { 
                a[k] = str[j]; 
                k++; 
            }
        }
    }

    // Print total counts for each data type
    printf("Total int: %d\nTotal Char: %d\nTotal long integer: %d\nTotal Double: %d\nTotal float: %d\nTotal short integer: %d\n", total[0], total[1], total[2], total[3], total[4], total[5]);
    printf("Total bool: %d\nTotal std::string: %d\nTotal wchar_t: %d\n", total[6], total[7], total[8]);

    // Calculate memory consumed by each type of variable
    int memory[9] = { 0 };
    memory[0] = total[0] * sizeof(int);
    memory[1] = total[1] * sizeof(char);
    memory[2] = total[2] * sizeof(long);
    memory[3] = total[3] * sizeof(double);
    memory[4] = total[4] * sizeof(float);
    memory[5] = total[5] * sizeof(short);
    memory[6] = total[6] * sizeof(bool);
    memory[7] = total[7] * sizeof(std::string); // C++ std::string memory (example size, adjust as necessary)
    memory[8] = total[8] * sizeof(wchar_t);

    // Print memory consumed by each data type
    printf("\n\n\n\t\t\tMEMORY CONSUMED BY VARIABLES\n\n\n");
    printf("Memory int: %d BYTES\nMemory Char: %d BYTES\nMemory long integer: %d BYTES\nMemory Double: %d BYTES\nMemory float: %d BYTES\nMemory short integer: %d BYTES\n", memory[0], memory[1], memory[2], memory[3], memory[4], memory[5]);
    printf("Memory bool: %d BYTES\nMemory std::string: %d BYTES\nMemory wchar_t: %d BYTES\n", memory[6], memory[7], memory[8]);
}

int fscanfcheck(char arr[], int size) {
    // Initialize various counters to zero
    int pcount = 0, ccount = 0, icount = 0, f = 0, acount = 0, scount = 0;
    int cin_check = 0;  // Flag to check for C++ cin usage

    // Loop through the array character by character
    for (int i = 0; i < size; i++) {
        if (arr[i] == '%') pcount++;     // Count occurrences of '%', typically format specifiers in `scanf`
        if (arr[i] == ',') ccount++;     // Count commas, which separate arguments in the format string
        if (arr[i] == '\"') icount++;    // Count double quotes, to ensure paired quotes in the format string
        if (arr[i] == '&') acount++;     // Count ampersands, as they are used in `scanf` to pass variable addresses
        if (arr[i] == '%' && arr[i + 1] == 's') scount++; // Count occurrences of the string specifier `%s`
        
        // Check if we have a C++ cin operator (this would be ">>")
        if (arr[i] == '>' && i > 0 && arr[i - 1] == '<') {
            cin_check = 1;  // Found a '>>' operator used in cin
        }
    }

    // If using `scanf` or `fscanf` (C code)
    if (!cin_check) {
        // Check if the number of format specifiers (%) matches the number of commas
        if (pcount != ccount - 1) f = 1;  // Set `f = 1` if there is a mismatch in counts

        // Check if the number of double quotes is even, ensuring all quotes are properly closed
        if (icount % 2 != 0) f = 1;  // If the quote count is odd, it means a pair is missing

        // Check if the number of format specifiers (excluding `%s`) matches the number of '&' (address operators)
        if ((pcount - scount) != acount) f = 1;
    }
    // If using `cin` (C++ code)
    else {
        // C++ does not use format specifiers, so we do not need to check for '%', '&', or commas in the same way
        // Instead, we ensure the usage of `>>` (cin operator) and check if it matches the expected pattern
        int cin_count = 0;  // Count occurrences of `>>` for cin
        for (int i = 0; i < size - 1; i++) {
            if (arr[i] == '<' && arr[i + 1] == '>') {
                cin_count++;  // Found a cin operator
                i++;  // Skip the next character as it is part of '>>'
            }
        }
        
        // In C++, we expect that every 'cin' should have an associated variable to store the input
        if (cin_count == 0) f = 1;  // If no 'cin' operator found, return error
    }

    // Return the value of `f` to indicate whether the checks passed (f = 0) or failed (f = 1)
    return f;
}

int forcheck(char arr[], int size) {
    int pcount = 0, qcount = 0, f = 0;

    // Loop through each character in the array
    for (int i = 0; i < size; i++) {
        // Count the number of semicolons (';')
        // In a `for` loop, there should be exactly 2 semicolons (e.g., for(initialization; condition; increment))
        if (arr[i] == ';') pcount++;

        // Count the number of single quotes (')
        // Single quotes are used for character literals, and they should appear in pairs (e.g., 'a')
        if (arr[i] == '\'') qcount++;
    }

    // Check if the number of semicolons is not exactly 2
    // If there are not exactly 2 semicolons, set the flag `f` to 1, indicating an error
    if (pcount != 2) f = 1;

    // Check if the number of single quotes is odd
    // Single quotes should always appear in pairs, so if the count is odd, set `f` to 1 (indicating an error)
    if (qcount % 2 != 0) f = 1;

    // Return the flag `f`
    // If `f` is 0, it means the checks passed (valid for loop); if `f` is 1, there was an error
    return f;
}

int whilecheck(char arr[], int size) {
    int pcount = 0, qcount = 0, f = 0, icount = 0;

    // Loop through each character in the input array
    for (int i = 0; i < size; i++) {
        // Check for a semicolon (';') in the while loop condition
        // A valid while loop condition should not have a semicolon inside the parentheses
        if (arr[i] == ';') f = 1;

        // Count the number of single quotes ('')
        // Single quotes are used for character literals, which should appear in pairs
        if (arr[i] == '\'') qcount++;

        // Count the number of double quotes ("")
        // Double quotes are used for string literals, which should appear in pairs
        if (arr[i] == '\"') icount++;
    }

    // Check if the number of single quotes is odd
    // Single quotes should always appear in pairs, so if the count is odd, set `f = 1` (indicating an error)
    if (qcount % 2 != 0) f = 1;

    // Check if the number of double quotes is odd
    // Double quotes should always be paired, so an odd count indicates an unclosed string literal, set `f = 1`
    if (icount % 2 != 0) f = 1;

    // Return the flag `f`
    // If `f` is 0, it means the checks passed (valid while loop structure); if `f` is 1, there was an error
    return f;
}

int semicolon_check(char* str, int len) {
    // Ignore preprocessor directives
    if (strncmp(str, "#include", 8) == 0 || strncmp(str, "#define", 7) == 0) {
        return 0; // No error
    }

    // Ignore control statements or function definitions that don't need a semicolon
    if (strstr(str, "for") || strstr(str, "while") || strstr(str, "if") || strstr(str, "else") ||
        strstr(str, "switch") || strstr(str, "case") || strstr(str, "default") || strstr(str, "do")) {
        return 0; // No error for control statements
    }

    // Ignore lines with access specifiers in C++ (public, private, protected)
    if (strstr(str, "public") || strstr(str, "private") || strstr(str, "protected")) {
        return 0; // No error for access specifiers
    }

    // Check if the line contains class/struct declaration in C++ (no semicolon required here)
    if (strstr(str, "class") || strstr(str, "struct")) {
        // Check if the line is a class declaration (should not have a semicolon)
        if (strchr(str, '{') != NULL) {
            return 0; // No error (for class/struct definition)
        }
    }

    int i;
    int openParenthesis = 0; // Track if parentheses are left unclosed
    int openBrace = 0;       // Track if braces are left unclosed (for class and function body)

    // Scan the line from the end to the start to check for semicolons and unclosed parentheses
    for (i = len - 1; i >= 0; i--) {
        if (!isspace(str[i])) {
            if (str[i] == ';') {
                return 0; // No error, semicolon found
            }

            // Track open parentheses for function-like syntax
            if (str[i] == ')') {
                openParenthesis++;
            } else if (str[i] == '(') {
                openParenthesis--;
            }

            // Track open/close braces for functions and class bodies
            if (str[i] == '{') {
                openBrace++;
            } else if (str[i] == '}') {
                openBrace--;
            }

            // Ignore semicolon check for function declarations/definitions inside classes
            // C++ class member function definitions do not require semicolons inside class body
            if (openBrace > 0 && openParenthesis == 0) {
                return 0; // No semicolon needed in class member function definition
            }

            // If parentheses and braces are balanced, expect a semicolon for normal code lines
            if (openParenthesis == 0 && openBrace == 0) {
                return 1; // Error: missing semicolon
            }
        }
    }

    // If the line is only whitespace or a class/struct body, ignore it
    return 0;
}

// Modify fcheck to detect errors in semicolons
int check_braces(fileline arr[], int index, int len) {
    // Implement brace checking logic (example logic)
    int openBraces = 0;
    int closeBraces = 0;

    // Iterate over the lines to check for braces
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < arr[i].linelen; j++) {
            if (arr[i].linetext[j] == '{') {
                openBraces++;
            } else if (arr[i].linetext[j] == '}') {
                closeBraces++;
            }
        }
    }

    // Return true if the number of opening braces matches the closing braces
    return (openBraces == closeBraces);
}

void fcheck(fileline arr[], int len) {
    char str[100], a[100];
    char sync[14][10] = { "printf", "scanf", "gets", "puts", "fscanf", "fprintf", "for", "while", 
                           "if", "else", "class", "cin", "cout", "new" };
    int i, j, k, f;
    int foundInclude = 0;
    int insideCodeBlock = 0;
    // Sample lines of code to test
    std::vector<Line> lines = {
        {"class MyClass {", 1},
        {"public", 2},                  // This line should trigger an error
        {"    MyClass(int value) { this->value = value; }", 3},
        {"    void display() { std::cout << \"Value: \" << value << std::endl; }", 4},
        {"private", 5},                 // This line should trigger an error
        {"    int value;", 6},
        {"};", 7}
    };

    // Check each line for errors
    checkForErrors(lines);

    // Check for the presence of #include and proper starting code block
    for (i = 0; i < len; i++) {
        strcpy(str, arr[i].linetext);

        if (str[0] == '\0' || isspace(str[0])) {
            continue; // Skip empty or space-only lines
        }

        // Detect start of code block (main function or parentheses)
        if (strstr(str, "int main") || strstr(str, "(")) {
            insideCodeBlock = 1;
            break;
        }

        if (strncmp(str, "#include", 8) == 0) {
            foundInclude = 1;
            break;
        } else if (str[0] == '#') {
            std::cout << "Error: Invalid preprocessor directive ---> line no: " << arr[i].lineno << std::endl;
        } else if (!isspace(str[0])) {
            std::cout << "Error: Unexpected text before #include ---> line no: " << arr[i].lineno << std::endl;
        }
    }

    if (!foundInclude && !insideCodeBlock) {
        std::cout << "Error: Missing #include directive at the start of the file.\n";
    }

    // Check for semicolon errors and handle C++ and C-specific syntax
    for (; i < len; i++) {
        strcpy(str, arr[i].linetext);
        int foundError = 0;

        // Check if the line has missing semicolons
        if (semicolon_check(str, arr[i].linelen)) {
            std::cout << "Error: Missing semicolon ---> line no: " << arr[i].lineno << std::endl;
            foundError = 1;
        }

        k = 0;
        for (j = 0; j < arr[i].linelen; j++) {
            if (str[j] == ' ' || str[j] == '(' || str[j] == '\0' || str[j] == '\t') {
                a[k] = '\0';

                // Check for function statements like printf, scanf, etc.
                if (strcmp(a, sync[0]) == 0 || strcmp(a, sync[1]) == 0) {
                    f = (strcmp(a, "printf") == 0) ? printcheck(str, arr[i].linelen) : scancheck(str, arr[i].linelen);
                    if (f) {
                        std::cout << "Error: In " << a << " statement; ---> line no: " << arr[i].lineno << std::endl;
                        foundError = 1;
                    }

                    int closingParenIndex = -1;
                    for (int idx = 0; idx < arr[i].linelen; idx++) {
                        if (str[idx] == ')') {
                            closingParenIndex = idx;
                            break;
                        }
                    }

                    if (closingParenIndex != -1) {
                        int semicolonFound = 0;
                        for (int idx = closingParenIndex + 1; idx < arr[i].linelen; idx++) {
                            if (str[idx] == ';') {
                                semicolonFound = 1;
                                break;
                            } else if (!isspace(str[idx])) {
                                break;
                            }
                        }

                        if (!semicolonFound) {
                            std::cout << "Error: Missing semicolon in " << a << " statement ---> line no: " << arr[i].lineno << std::endl;
                            foundError = 1;
                        }
                    }
                } 
                // Check for C++ loop constructs
                else if (strcmp(a, sync[6]) == 0) { // for loop
                    f = forcheck(str, arr[i].linelen);
                    if (f) {
                        std::cout << "Error: In for loop; ---> line no: " << arr[i].lineno << std::endl;
                        foundError = 1;
                    }
                    if (!check_braces(arr, i, len)) {
                        std::cout << "Error: Missing braces in for loop; ---> line no: " << arr[i].lineno << std::endl;
                        foundError = 1;
                    }
                } 
                else if (strcmp(a, sync[7]) == 0) { // while loop
                    f = whilecheck(str, arr[i].linelen);
                    if (f) {
                        std::cout << "Error: In while loop; ---> line no: " << arr[i].lineno << std::endl;
                        foundError = 1;
                    }
                    if (!check_braces(arr, i, len)) {
                        std::cout << "Error: Missing braces in while loop; ---> line no: " << arr[i].lineno << std::endl;
                        foundError = 1;
                    }
                }
                // Check for conditionals like if/else
                else if (strcmp(a, sync[8]) == 0 || strcmp(a, sync[9]) == 0) {
                    if (ifcheck(str, arr[i].linelen)) {
                        std::cout << "Error: In conditional statement (if/else); ---> line no: " << arr[i].lineno << std::endl;
                        foundError = 1;
                    }
                }
                // Check for C++ class declarations and member function syntax
                else if (strcmp(a, sync[10]) == 0) {
                    // Check for class declaration syntax and member functions
                    if (classcheck(str)) {
                        std::cout << "Error: In class declaration; ---> line no: " << arr[i].lineno << std::endl;
                        foundError = 1;
                    }

                    // Check for virtual functions (functions in a class that are marked as virtual)
                    if (strstr(str, "virtual")) {
                        // Check if virtual function has correct syntax (should end with semicolon or be defined in cpp file)
                        if (strchr(str, ';') == NULL) {
                            std::cout << "Error: Virtual function needs to be terminated with a semicolon ---> line no: " << arr[i].lineno << std::endl;
                            foundError = 1;
                        }
                    }

                    // Check inheritance in class declarations
                    if (strchr(str, ':')) {
                        if (strstr(str, "public") == NULL && strstr(str, "private") == NULL && strstr(str, "protected") == NULL) {
                            std::cout << "Error: Inheritance should use public/private/protected after ':' ---> line no: " << arr[i].lineno << std::endl;
                            foundError = 1;
                        }
                    }
                }
                // Check for C++ input and output statements (cin, cout)
                else if (strcmp(a, sync[11]) == 0) {
                    if (cincheck(str)) {
                        std::cout << "Error: In cin statement; ---> line no: " << arr[i].lineno << std::endl;
                        foundError = 1;
                    }
                } 
                else if (strcmp(a, sync[12]) == 0) {
                    if (coutcheck(str)) {
                        std::cout << "Error: In cout statement; ---> line no: " << arr[i].lineno << std::endl;
                        foundError = 1;
                    }
                }
                // Check for memory allocation in C++ (new operator)
                else if (strcmp(a, sync[13]) == 0) {
                    if (newcheck(str)) {
                        std::cout << "Error: In new memory allocation; ---> line no: " << arr[i].lineno << std::endl;
                        foundError = 1;
                    }
                }
                // NEW: Check for missing parentheses in object creation
                if (strstr(str, "new") && strchr(str, '(') == NULL) {
                    std::cout << "Error: Missing parentheses in object creation ---> line no: " << arr[i].lineno << std::endl;
                    foundError = 1;
                }

                k = 0;
          } else {
                a[k] = str[j];
                k++;
            }
        }
    }
}

// Utility function implementations (dummy for now)
int semicolon_check(const char* str, int len) { return (str[len-1] != ';' && str[len-1] != '}'); }
int forcheck(const char* str, int len) { return 0; }
int whilecheck(const char* str, int len) { return 0; }
int ifcheck(const char* str, int len) { return 0; }
int classcheck(const char* str) { return 0; }
int cincheck(const char* str) { return 0; }
int coutcheck(const char* str) { return 0; }
int newcheck(const char* str) { return 0; }