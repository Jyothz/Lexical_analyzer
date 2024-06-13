
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tokens.h"
int main(int argc,char *argv[])
{
	//checking if argc is equal to 2 or not
	if(argc==2)
	{
		// function call to validate cla arguement and checking if it returns SUCCESS or not
		if(read_and_validation(argv[1])==SUCCESS)
		{
			//function call to generate tokens and checking if it returns SUCCESS or not
			if(generate_tokens()==SUCCESS)
			{
				printf("Tokens Generated\n");
			}
			else
			{
				printf("Tokens not generated\n");
			}
			return 0;
		}

	}
	printf("ERROR: Invalid input\n");
	printf("Give input as: ./a.out <file.c>\n");
	return 0;
}

