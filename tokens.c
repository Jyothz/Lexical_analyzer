
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokens.h"

FILE *fptr;

Status read_and_validation(char *argv)
{
	//checking if argv is a .c file
	if (strstr(argv,".") && strcmp(strstr(argv,"."),".c")==0)
	{
		//opening the file in read mode
		if((fptr=fopen(argv,"r"))==NULL)
		{
			return FAILURE;
		}
		return SUCCESS;
	}
	return FAILURE;
}


Status generate_tokens(void)
{
	char str[100];
	int i;
	int status;
	while(1)
	{
		i=0;
		while(1)
		{
			//reading 1byte from fptr and storing it in str[i]
			fread(&str[i],1,1,fptr);
			//checking if file pointer reaches EOF
			if(feof(fptr))
			{
				return SUCCESS;
			}
			/*checking if str[i] is '/' */
			if(str[i]=='/')
			{
				//reading 1byte from fptr and storing it in str[i+1]
				fread(&str[i+1],1,1,fptr);
				//checking if str[i+1] is '/'
				if(str[i+1]=='/')
				{
					//checking if function returns FAILURE
					if(single_line_comment(fptr)==FAILURE)
					{
						return FAILURE;
					}
				}
				//checking if str[i+1] is '/'
				else if(str[i+1]=='*')
				{
					//checking if function returns FAILURE
					if(multi_line_comment(fptr)==FAILURE)
					{
						return FAILURE;
					}
				}
				else
				{
					//comming out of the loop
					break;
				}
				//updating i and jumping to loop condition
				i=0;
				continue;
			}
			//checking if str[i] is "
			else if(str[i]=='"')
			{
				//checking if string_literal function returns FAILURE
				if(string_literal(fptr)==FAILURE)
				{
					return FAILURE;
				}
				//updating i and jumping to loop condition
				i=0;
				continue;
			}
			//checking if str[i] is '
			else if(str[i]=='\47')
			{
				//checking if string_literal function returns FAILURE
				if(char_literal(fptr)==FAILURE)
				{
					return FAILURE;
				}
				//updating i and jumping to loop condition
				i=0;
				continue;
			}
			//checking if str[i] is # and i is 0 or not
			else if(str[i]=='#' && i==0)
			{
				while(1)
				{
					i++;
					fread(&str[i],1,1,fptr);
					//checking if str[i] is new line
					if(str[i]=='\n')
					{
						//coming out of the loop
						break;
					}
				}
				str[i]='\0';
				printf("%-20s : Preprocessor Directive\n",str);
				//updating i and jumping to loop condition
				i=0;
				continue;
			}
			//checking if str[i] is . and i is not 0
			else if(str[i]=='.')
			{
				//checking if i is 0 or not
				if(i==0)
				{
					i++;
					//reading 1byte from fptr and storing it in str
					fread(&str[i],1,1,fptr);
					//checking if sr[i] is between 0-9
					if(str[i]>='0' && str[i]<='9')
					{
						while(str[i]>='0' && str[i]<='9')
						{
							i++;
							//reading 1byte from fptr and storing it in str
							fread(&str[i],1,1,fptr);
						}
						break;
					}
					else
					{
						str[i]='\0';
						fseek(fptr,-1,SEEK_CUR);
						printf("%-20s : Operator\n",str);
						i=0;
					}
					continue;
				}
				else
				{
					int k=0;
					while(k<i)
					{
						if(str[k]<'0' || str[k]>'9')
						{
							break;
						}
						k++;
					}
					if(k!=i)
					{
						break;
					}

				}

			}
			//checking if str[i] is space or tab or newline or punctuation
			else if(str[i]!='_' && (str[i]==' '|| str[i]=='\t' || str[i]=='\n' || ispunct(str[i])))
			{
				//coming out of the loop
				break;
			}
			i++;
		}
		//checking ig i is 0
		if(i==0)
		{
			//incrementing i
			i++;
		}
		//checking if str[i] is space or tab or newline
		else if(str[i]!=' ' && str[i]!='\t' && str[i]!='\n')
		{
			//moving file pointer 1byte back
			fseek(fptr,-1,SEEK_CUR);
		}
		str[i]='\0';
		//checking if strlen and strcmp functions giving non-zero values
		if(strlen(str) && strcmp(str,"\n") &&strcmp(str,"\t") && strcmp(str," ")) 
		{
			//fuction call and storing return value in status
			status=is_keyword(str);
			//checking if status is SUCCESS
			if(status==SUCCESS)
			{
				printf("%-20s : Data type\n",str);
			}
			//checking if status is TRUE
			else if(status==TRUE)
			{
				printf("%-20s : Keyword\n",str);
			}
			else
			{
				//fuction call and storing return value in status
				status=is_literal(str);
				//checking if status is SUCCESS
				if(status==SUCCESS)
				{
					printf("%-20s : Integral literal\n",str);
				}
				//checking if status is TRUE
				else if(status==TRUE)
				{
					printf("%-20s : Float literal\n",str);
				}
				else
				{
					//checking if is_operator is giving SUCCESS
					if(is_operator(str)==SUCCESS)
					{
						printf("%-20s : operator\n",str);
					}
					//checking if is_special is giving SUCCESS
					else if(is_special(str)==SUCCESS)
					{
						printf("%-20s : Special character\n",str);
					}
					else if(is_valid(str)==SUCCESS)
					{
						printf("%-20s : Identifier\n",str);
					}
					else
					{
						return FAILURE;
					}
				}
			}

		}
	}
	return SUCCESS;
}

Status is_valid(char *str)
{
	int i=0;
	if( (str[i]>='0' && str[i]<='9') || (str[i]!='_' && isalpha(str[i])==0))
	{
		return FAILURE;
	}
	while(str[i] && (str[i]=='_' || isdigit(str[i]) || isalpha(str[i])))
	{
		i++;
	}
	if(i!= strlen(str))
	{
		return FAILURE;
	}
	return SUCCESS;
}
Status is_keyword(char *str)
{
	int i=0;
	//iterating loop till i equals to 18
	while(res_kwords_data[i])
	{
		//checking if strcmp returns 0 
		if(strcmp(res_kwords_data[i],str)==0)
		{
			return SUCCESS;
		}
		i++;
	}
	i=0;
	//iterating loop till i equals to 13
	while(res_kwords_non_data[i])
	{
		//checking if strcmp returns 0 
		if(strcmp(res_kwords_non_data[i],str)==0)
		{
			return TRUE;
		}
		i++;
	}
	return FAILURE;
}

Status is_literal(char *str)
{
	int i=0;
	//iterating loop till str[i] reaches NULL
	while(str[i])
	{
		//checking if str[i] is '.'
		if(str[i]=='.')
		{
			if(strlen(str)==1)
			{
				return FAILURE;
			}
			i++;
			//iterating till str[i] reaches NULL
			while(str[i])
			{
				//checking if str[i] is between '0' and '9'
				if(str[i]<'0' || str[i]>'9')
				{
					return FAILURE;
				}
				i++;
			}
			return TRUE;
		}
		//checking if str[i] is not in between 0 and 9
		if(str[i]<'0' || str[i]>'9')
		{
			return FAILURE;
		}
		i++;
	}
	return SUCCESS;
}

Status string_literal(FILE *fptr)
{
	char buff[100];
	int i=0;
	while(1)
	{
		//checking if file pointer reaches EOF
		if(feof(fptr))
		{
			return FAILURE;
		}
		//reading 1 byte from fptr and storing it in buff[i]
		fread(&buff[i],1,1,fptr);
		//checking if buff[i] is '"'
		if(buff[i]=='"')
		{
			//coming out of the loop
			break;
		}
		i++;
	}
	buff[i]='\0';
	printf("%-20s : String literal\n",buff);
	return SUCCESS;
}
Status char_literal(FILE *fptr)
{
	char buff[100];
	int i=0;
	while(1)
	{
		//checking if file pointer reaches EOF
		if(feof(fptr))
		{
			return FAILURE;
		}
		//reading 1 byte from fptr and storing it in buff[i]
		fread(&buff[i],1,1,fptr);
		//checking if buff[i] is '
		if(buff[i]=='\47')
		{
			//coming out of the loop
			break;
		}
		i++;
	}
	buff[i]='\0';
	printf("%-20s : Character literal\n",buff);
	return SUCCESS;
}
Status is_special(char *str)
{
	int i=0;
	//checking if string length of str is greater than 1
	if(strlen(str)>1)
	{
		return FAILURE;
	}
	//running loop till i equals to 6
	while(symbols[i])
	{
		//checking if symbols[i] and str[0] are equal or not
		if(symbols[i]==str[0])
		{
			return SUCCESS;
		}
		i++;
	}
	return FAILURE;
}

Status is_operator(char *str)
{
	int i=0;
	//checking if string length of str is greater than 1
	if(strlen(str)>1)
	{
		return FAILURE;
	}
	//running loop till i equals to 14
	while(operators[i])
	{
		//checking if operators[i] and str[0] are equal or not
		if(operators[i]==str[0])
		{
			i=0;
			//reading 1byte form fptr and storing it in str[1]
			fread(&str[1],1,1,fptr);
			str[2]='\0';
			//running loop till operators2[i] reaches NULL
			while(operators2[i])
			{
				//checking if operators2[i] and str are same 
				if(strcmp(operators2[i],str)==0)
				{
					//checking if str is << or >>
					if(strcmp(str,"<<")==0 ||strcmp(str,">>")==0)
					{
						//reading 1byte from fptr and storing it in str[2]
						fread(&str[2],1,1,fptr);
						//checking if str[2] is =
						if(str[2]=='=')
						{
							str[3]='\0';
							return SUCCESS;
						}
						else
						{
							//moving file pointer 1byte backward
							fseek(fptr,-1,SEEK_CUR);
							str[2]='\0';
						}
					}
					return SUCCESS;
				}
				i++;
			}
			//moving file pointer 1byte backward
			fseek(fptr,-1,SEEK_CUR);
			str[1]='\0';
			return SUCCESS;
		}
		i++;
	}
	return FAILURE;
}

Status single_line_comment(FILE *fptr)
{
	char ch=0;
	//running loop till ch is equal to '\n'
	while(ch!='\n')
	{
		//checking if fptr reaches EOF
		if(feof(fptr))
		{
			return FAILURE;
		}
		//reading 1byte from fptr
		fread(&ch,1,1,fptr);
	}
	return SUCCESS;
}
Status multi_line_comment(FILE *fptr)
{
	char ch=0;
	while(1)
	{
		//checking if fptr reaches EOF
		if(feof(fptr))
		{
			return FAILURE;
		}
		//reading 1byte from fptr
		fread(&ch,1,1,fptr);
		//checking if ch is equal to '*'
		if(ch=='*')
		{
			//reading 1byte from fptr
			fread(&ch,1,1,fptr);
			//checking if ch is '/'
			if(ch=='/')
			{
				return SUCCESS;
			}
		}
	}
}
