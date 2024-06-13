#ifndef TOKEN_H
#define TOKEN_H



/* All keywords and operators */
static char *res_kwords_data[]={"const","volatile","extern","auto","register","static","signed","unsigned","short","long","int","char","float","double","typedef","enum","struct","union","\0"};

static char *res_kwords_non_data[]={"goto","return","continue","break","if","else","for","while","do","switch","case","default","sizeof","\0"}; 

static char operators[]={'?',':','/','+','*','-','%','=','<','>','~','&',',','!','^','|','.','\0'};

static char *operators2[]={"++","--",">=","<=","==","!=","+=","-=","/+","*=","&=","|=","^=","%=","<<",">>","&&","||","->","\0"};

static char symbols[]={'{','}','(',')','[',']',';',',','\0'};

typedef enum
{
	SUCCESS,
	FAILURE,
	TRUE
}Status;

/* to validate cla arguements */
Status read_and_validation(char *argv);

/* to generate tokens */
Status generate_tokens(void);

/* to check if given keyword is valid */
Status is_valid(char *str);

/* to check str is a keyword */
Status is_keyword(char *str);

/* to check it is a string literal */
Status string_literal(FILE *fptr);

/* to check it is a character literal */
Status char_literal(FILE *fptr);

/* to check it is a integer or float literal */
Status is_literal(char *str);

/* to check str is a operator */
Status is_operator(char *str);

/* to check str is a special character */
Status is_special(char *str);

/* to check it is a single line or multiline comment*/
Status single_line_comment(FILE *fptr);
Status multi_line_comment(FILE *fptr);
#endif

