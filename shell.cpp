#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

 #define MAX_LINE 80 /* The maximum length of a command */

#define BUFFER_SIZE 50

#define buffer "\n\Shell Command History:\n"



//declarations

char history[10][BUFFER_SIZE]; //history array to store history commands

int count = 0;







int main(void)

{

    // max arguments

    char *args[MAX_LINE/2 + 1];

    //------------------------------------------string command;

    /* buffer to hold the input command */

    char inputBuffer[MAX_LINE];

                //------------------------------------------string commands;

    // equals 1 if a command is followed by "&"

    int flag;

    pid_t pid;

    int i;



     //infinite loop for shell prompt

    while (1)

    {

        //flag =0 by default



        flag = 0;

        printf("enter command>");

        fflush(stdout);

        if(formatCommand(inputBuffer,args,&flag)!=-1) // get next command

                {

                                pid = fork();



               if (pid < 0)//if pid is less than 0, forking fails

               {



                               printf("Fork failed.\n");

                               exit (1);

               }

                else if (pid == 0)//if pid ==0

               {



                               //command not executed

                               if (execvp(args[0], args) == -1)

                               {



                                              printf("Error executing command\n");

                               }

                                }



                                // if flag == 0, the parent will wait,

               // otherwise returns to the formatCommand() function.

               else

               {

                               i++;

                               if (flag == 0)

                                {

                                              i++;

                                              wait(NULL);

                                }

               }

                 }

     }

}



//function to display the history of commands

void displayHistory()

{



    printf("Shell command history:\n");



    int i;

    int histCount = count;

    int j = 0;



    //for loop

    for (i = 0; i<10;i++)

    {

        //print out the command

        printf("%d.  ", histCount);

        while (history[i][j] != '\n' && history[i][j] != '\0')

        {

                                //printing command

            printf("%c", history[i][j]);

            j++;

        }





        printf("\n");

        j = 0;



        histCount--;

        if (histCount ==  0)

            break;

    }



    printf("\n");

}



//Function to display all the available commands



void exit_prog()

{

    exit(0);

}
void displayCommands()
{
    printf("------------List of Common Commands------------\n");
    printf("1. ls        list all files in the current directory\n");
    printf("2. cd        change directory\n");
    printf("3. mkdir     make directory\n");
    printf("4. man       Manual of commands\n");
    printf("5. mv        Move file \n");
    printf("6. rmdir     Remove directory\n");
    printf("7. touch     Make file\n");
    printf("8. rm        Remove file\n");
    printf("9. locate    locate file \n");
    printf("10.clear      clears the screen\n");

}

//Fuction to get the command from shell, tokenize it and set the args parameter



int formatCommand(char inputBuffer[], char *args[],int *flag)

{

                int length; // # of chars in command line

                int i;     // loop index for inputBuffer

                int start;  // index of beginning of next command

                int ct = 0; // index of where to place the next parameter into args[]

                int hist;

                //read user input on command line and checking whether the command is !! or !n



               length = read(STDIN_FILENO, inputBuffer, MAX_LINE);



    start = -1;

    if (length == 0)

        exit(0);

    //end of command

    if (length < 0)

    {

        printf("Command not read\n");

        exit(-1);  //terminate

    }



   //examine each character

    for (i=0;i<length;i++)

    {

        switch (inputBuffer[i])

        {

            case ' ':



            //final char

            case '\n':

                if (start != -1)

                {

                    args[ct] = &inputBuffer[start];

                    ct++;

                }



                inputBuffer[i] = '\0';

                args[ct] = '\0'; // no more args

                break;

                case '\t' :               // to seperate arguments

                if(start != -1)

                {

                    args[ct] = &inputBuffer[start];

                    ct++;

                }

                // add a null char at the end

                inputBuffer[i] = '\0';

                start = -1;

                break;

            default :

                if (start == -1)

                    start = i;

                if (inputBuffer[i] == '&')

                {

                    *flag  = 1; //this flag is the differentiate whether the child process is invoked in background

                   inputBuffer[i] = '\0';

                }

        }

    }



    args[ct] = NULL; //if the input line was > 80



    if(strcmp(args[0],"exit")==0)

    {

        exit_prog();

    }

    if(strcmp(args[0],"?")==0)

        {

            displayCommands();

        }


if(strcmp(args[0],"history")==0)

        {

               if(count>0)

                                {



                displayHistory();

                                }

                                else

                                {

                                printf("\nNo Commands in the history\n");

                                }

                                return -1;

        }



                else if (args[0][0]-'!' ==0)

                {

            int x = args[0][1]- '0';

                    int z = args[0][2]- '0';



                    if(x>count) //second letter check

                    {

                    printf("\nNo Such Command in the history\n");

                    strcpy(inputBuffer,"Wrong command");

                    }

                    else if (z!=-48) //third letter check

                    {

                    printf("\nNo Such Command in the history. Enter <=!9 (buffer size is 10 along with current command)\n");

                    strcpy(inputBuffer,"Wrong command");

                    }

                    else

                                {



                                                if(x==-15)//Checking for '!!',ascii value of '!' is 33.

                                                {

                            strcpy(inputBuffer,history[0]);  // this will be your 10 th(last) command

                                                }

                                                else if(x==0) //Checking for '!0'

                                                {              printf("Enter proper command");

                                                                strcpy(inputBuffer,"Wrong command");

                                                }



                                                else if(x>=1) //Checking for '!n', n >=1

                                                {

                                                                strcpy(inputBuffer,history[count-x]);



                                                }



                                }

                }

   //Moving the history elements one step higher

for (i = 9;i>0; i--)

               strcpy(history[i], history[i-1]);

//Updating the history array

    strcpy(history[0],inputBuffer);

    count++;

                if(count>10)

                {

            count=10;

                }

}
