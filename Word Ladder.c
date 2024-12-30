#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_WORDS 100
#define WORD_LENGTH 100

void Lower(char* input)
{
    for (int i = 0; i <= strlen(input); i++) {
        input[i] = tolower((unsigned char)input[i]);
    }
}
//this function is to give a hint word
char *Hint(const char *prevword[], char target[])
{
	FILE *fptr;
    fptr = fopen("dictionary.txt", "r");
    char word[WORD_LENGTH]; // Array to read words from the file 
    
    while (fscanf(fptr, "%s", word) != EOF)
	{
        Lower(word);
        
        int common = 0;
        
		if(Check(word,strlen(prevword)))
		{
			if (Compare(prevword,word) == 1)
        	{
           		char *hint = (char *)malloc((strlen(word) + 1) * sizeof(char));
                strcpy(hint, word);
                
                for(int i = 0; i < strlen(hint); i ++)
                {
               		if(hint[i] == target[i])
               		{
               			common ++;
               			break;
					}
				}
				
				if(common > 0)
				{
					return hint;
				}
        	}
        }
    }

    fclose(fptr);
    return NULL;
}
//this function checks the validity of word
int file_access(const char *input_word[])
	{
    FILE *fptr;
    fptr = fopen("dictionary.txt", "r");
    char word[WORD_LENGTH]; // Array to read words from the file

    Lower(input_word);

    // Reading words from the file and then comparing with the input word
    while (fscanf(fptr, "%s", word) != EOF)
	{
        Lower(word);

        if (strcmp(input_word, word) == 0) 
        {
            fclose(fptr);
            return 1;
        }
    }

    fclose(fptr);
    return 0;
}
//this functions prints the error messages
void PrintMessage(const int result)
{
	switch(result)
	{
		case 0:printf("The word should be of the length mentioned\n");
		printf("Try again!\n");
		break;
		
		case 1:break;
		
		case 2:printf("This word is invalid try another word\n");
		printf("Try again!\n");
		break;
		
		case 4:printf("You can only change one letter of the word\n");
		printf("Try again!\n");
		break;
		
		case 5:printf("Sorry, There is no word to help you");
		break;
	}
}
//this function Checks if the string is of the given length
int Check(const char string[], int length)
{
	if(strlen(string) != length)
	{
		return 0;
	}
	else
	{
		if(file_access(string))
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
}
//this function compares the two string and checks if there is only one change or not
int Compare(const char string1[], const char string2[])
{
	int count = 0;
	
	for(int i = 0; i < strlen(string2); i ++)
	{
		if(string1[i] == string2[i])
		{
			continue;
		}
		else
		{
			count ++;
		}
	}
	if(count == 1)
	{
		return 1;
	}
	else
	{
		return 4;
	}
}

int main()
{
	int length;
	
	printf("\t\t\t\t\t\tWelcome!\n");
	printf("\t\t\t\t\t Let's start the game!\n");
	printf("------------------------------------------------------------------------------------------------------------------------\n");
	
	do
	{
		printf("Enter the size of the word: ");
		scanf("%d", &length);
		if(length <= 1)
		{
			printf("Length should be greater than 1!\n");
			continue;
		}
		else
		{
			break;
		}
	}while(1);
	
	//memory allocation of 2d array
	char **string;
	string = (char**)malloc(100*sizeof(char*));
	for(int i = 0;i < 100;i ++)
	{
		string[i] = (char*)malloc((length + 1) * sizeof(char));
	}
	//wordcount is 2 because 0 stores target and 1 stores starting word
	int wordcount = 2;
	//result is used to check errors and help print error messages using function
	int result;
	//this part takes input and checks the strating word
	do
	{
		printf("Enter a starting word: ");
		scanf("%s", string[1]);
		result = Check(string[1],length);
		if(result == 1)
		{
			break;
		}
		else
		{
			PrintMessage(result);
			continue;
		}
	}while(1);
	PrintMessage(result);
	//this part takes input and checks the target word
	do
	{
		printf("Enter a target word: ");
		scanf("%s", string[0]);
		if(strncmp(string[0], string[1], length) == 0)
		{
			printf("Target and Starting word should not be same \n");
		}
		else
		{
			result = Check(string[0],length);
			if(result == 1)
			{
				break;
			}
			else
			{
				PrintMessage(result);
				continue;
			}
		}
	}while(1);
	PrintMessage(result);
	
	printf("\nNow start with changing the starting word letter by letter to get to your target word\n\n");
	printf("You have to reach the target word as soon as possible\n\n");
	printf("If you enter \"-help\" you will be provided with settings menu\n\n");
	printf("Which gives you options of HINT, EXIT and RESTART\n\n");
	printf("You have only 2 hints, GOODLUCK\n\n");
	printf("NOTE: It may take several seconds to find hint\n\n");
	//hints are initially 2, choice is used for settings or options
	int choice, hints = 2;
	
	do
	{
		do
		{
			printf("Enter a word to continue towards target word: ");
			scanf("%s", string[wordcount]);
			printf("\n");
			//this part checks if the word "-seetings" is typed
			if(strncmp(string[wordcount], "-help", 5) == 0)
			{
				//the settings menu is shown
				//we have to add options of exit and restart too
				printf("HELP\n");
				printf("1.Hint\n");
				printf("2.Restart\n");
				printf("3.Exit\n");
				printf("Press any other number to exit help\n");
				scanf("%d", &choice);
				//switch case to check which option is chosen
				switch(choice)
				{
					case 1:
					if(hints > 0)
					{
						//decrements hints and uses hint function to get a hint word
						hints --;
						for(int i = 0; i < strlen(string[wordcount]); i ++)
						{
							string[wordcount][i] = NULL;
						}
						string[wordcount] = Hint(string[wordcount - 1], string[0]);
						//if hint function is unable to help, it retains a hint and asks user to input
						if(string[wordcount] == NULL)
						{
							hints ++;
							printf("Sorry I couldn't help you\n");
							string[wordcount] = "-help";
						}
						else
						{
							printf("You have %d hints left\n", hints);
							printf("Hint : %s\n", string[wordcount]);
							break;
						}
					}
					//if there no hints left it asks user to input the word
					else
					{
						printf("You have no Hints left\n");
						break;
					}
					
					case 2:
						for(int i = 2; i < wordcount + 1; i ++)
						{
							for(int j = 0; j < strlen(string[i]); j ++)
							{	
								string[i][j] = NULL;
							}
						}
						wordcount = 2;
						string[wordcount] = "-help";
						break;
						
					case 3:
						printf("\nYou are ending the game\n");
						end();
						break;
						
					default:
						break;
				}
			}
			// this part checks if user were out of hints or functions couldn't help so it can continue
			//and ask the user to input the word himself
			if(strncmp(string[wordcount], "-help", 5) == 0)
			{
				for(int i = 0; i < strlen(string[wordcount]); i ++)
				{
					string[wordcount][i] = NULL;
				}
			}
			else
			{
				//this part checks validity of word and functions Check and Compare are used
				result = Check(string[wordcount], length);
				if(result == 1)
				{
					result = Compare(string[wordcount],string[wordcount - 1]);
					if(result == 1)
					{
						wordcount ++;
						break;
					}
					else
					{
						PrintMessage(result);
						continue;
					}
				}
				else
				{
					PrintMessage(result);
					continue;
				}
			}
		}while(1);
		PrintMessage(result);
		//the game will only until the target word is achieved
	}while(strncmp(string[wordcount - 1],string[0], length) != 0);
	free(string);
	
	//celebrating the victory
	printf("You successfully reached the target word!!\n");
	printf("------------------------------------------------------------------------------------------------------------------------\n");
	printf("Congrats!\n\n");
	printf("You completed it in %d steps\n", wordcount - 2);
}