#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <time.h>
#include <dirent.h>

struct hist
{
    char *hname;
    char hdt[50]; // contains date and time of command name
    int hstat;    // 0 means fails, 1 means success
} histarr[11];

static int h_val = 0; // why static ?

#define MAXCOM 1000 // max numbers of letters to be supp
#define MAXLIST 100 // max number of commands supported
char pwd_for_man[100];

// This is to add status fail or success to the hist structure
void add_stat(int stat)
{
    if (stat == 1)
    {
        printf("\nSUCCESS");
        histarr[h_val - 1].hstat = 1;
    }
    else
    {
        printf("\nFAILURE");
        histarr[h_val - 1].hstat = 0;
    }
}

void FIND(char **parsed)
{
    int count = 0;
    struct dirent *de;                  // pointer to directory entry
    DIR *dir = opendir(".");            // directory
    while ((de = readdir(dir)) != NULL) // read directory thorough readdir
    {
        if (!strncmp(parsed[1], de->d_name, strlen(parsed[1])))
        {
            printf("\n%s\n", de->d_name);
            ++count;
        }
    }
    if (count == 0)
    {
        printf("\n NO Match Found");
        histarr[h_val - 1].hstat = 1;
    }
    else
    {
        printf("\n%d Matches Found \n", count);
        histarr[h_val - 1].hstat = 1;
    }
    closedir(dir); // close the opened directory
}
void PWD()
{
    histarr[h_val - 1].hstat = 1;
    char pwd[100];
    // Internally uses getcwd(temp_arr_to_store_pwd,size_of_arr) system call
    getcwd(pwd, sizeof(pwd)); // stores output of getcwd in character array
    printf("\nPresent Working Directory is - %s\n", pwd);
}
void show_history()
{
    // Uses modular counting to get last 10 commands
    // Modular counting is circular and doesn't overflow the size of hist
    int x = 0;
    int i = 0;
    printf("* %35s, %10s, %30s \n", "COMMAND NAME ", " STATUS ", " DATE      AND      TIME "); // put n spaces before string "%nStr"
    printf("* %35s, %10s, %30s \n", "------------ ", " ------ ", " ----------------------- ");
    printf("\n");
    histarr[h_val - 1].hstat = 1;
    while (x < 11)
    {
        if (histarr[x].hstat == 1)
        {
            if (x == h_val - 1)
                printf("* -->%32s, %10s, %30s \n", histarr[x].hname, "SUCCESS", histarr[x].hdt);
            else
                printf("* %35s, %10s, %30s \n", histarr[x].hname, "SUCCESS", histarr[x].hdt);
        }
        else
        {
            if (x == h_val - 1)
                printf("* -->%32s, %10s, %30s \n", histarr[x].hname, "FAILED", histarr[x].hdt);
            else
                printf("* %35s, %10s, %30s \n", histarr[x].hname, "FAILED", histarr[x].hdt);
        }
        x++;
    }
    printf("\nCURRENT COMMAND IS POINTED  BY --> \n");
}

void enter_shell()
{

    int i;

    while (1) // can't write true not c++
    {

        printf("To enter the shell type ENTRY\n");
        i = strcmp("ENTRY", readline(""));
        if (i == 0)
        {
            printf("Welcome..");
            break;
        }
    }
}

void start_shell()
{
    //  clear();
    printf("\n\n\n\n\t Entering shell");
    char *username = getenv("USER");
    printf("\n\n\n\n\tUser is : %s ", username);
    printf("\n\n\n");

    puts(
        "\n\n\nTHE FOLLOWING COMMANDS ARE SUPPORTED BY THIS SHELL\n"
        "\n* HELP"
        "\n* CD"
        "\n* HISTORY"
        "\n* PWD"
        "\n* EXIT"
        "\n* FIND"
        "\n* MAN");
    sleep(6);
    //  clear();
}
void get_time()
{
    time_t current_time = time(NULL);
    struct tm *tm = localtime(&current_time);
    strcpy(histarr[h_val].hdt, asctime(tm));
}
int read_input_from_terminal(char *str)
{

    char terminal_line[1024];
    getcwd(terminal_line, sizeof(terminal_line));
    printf("\nYou are in Dir: %s", terminal_line);

    char *buf;
    // readline() is used to make the user input as a string until enter is pressed
    buf = readline("$ ");
    histarr[h_val].hname = buf;
    get_time();

    h_val = (h_val + 1) % 11;

    if (strlen(buf) != 0)
    {
        add_history(buf);
        //   printf("here");
        strcpy(str, buf);
        return 0;
    }
    else
    {
        return 1;
    }
}
// HELP command implementation
void openHelp()
{
    histarr[h_val - 1].hstat = 1;
    puts(
        "\n\n**ALL USER DEFINED COMMANDS ARE IN CAPITAL LETTERS"
        "\n**BASIC LINUX COMMNADS ARE ALSO SUPPORTED"
        "\n**USE CAPITAL <MAN COMMANDNAME> WITHOUT <> FOR USER CREATED COMMANDS"
        "\n**USE SMALL   <man commandname> WITHOUT <> FOR LINUX COMMANDS"
        "\n\n\nTHE FOLLOWING COMMANDS ARE SUPPORTED BY THIS SHELL\n"
        "\n* HELP"
        "\n* CD"
        "\n* HISTORY"
        "\n* PWD"
        "\n* EXIT"
        "\n* FIND"
        "\n* MAN");
    return;
}
void MAN(char **parsed)
{
    // Just compare the required manual page using strcmp.
    // Followed by opening the file and printing the text on screen.

    //Manual page for CD
    char pwd_temp_man[100];
    getcwd(pwd_temp_man, sizeof(pwd_temp_man));

    if (strcmp(pwd_for_man, pwd_temp_man) != 0)
    {
        printf("\n MAN only works in the directory ~shell_in_c/\n");
        histarr[h_val - 1].hstat = 0;
    }
    else
    {
        histarr[h_val - 1].hstat = 1;
        if (!strcmp(parsed[1], "CD"))
        {
            FILE *fp;
            char ch;
            fp = fopen("CD.txt", "r");
            while ((ch = getc(fp)) != EOF)
                printf("%c", ch);
        }
        //Manual page for EXIT
        else if (!strcmp(parsed[1], "EXIT"))
        {
            FILE *fp;
            char ch;
            fp = fopen("EXIT.txt", "r");
            while ((ch = getc(fp)) != EOF)
                printf("%c", ch);
        }
        //Manual page for PWD
        else if (!strcmp(parsed[1], "PWD"))
        {
            FILE *fp;
            char ch;
            fp = fopen("PWD.txt", "r");
            while ((ch = getc(fp)) != EOF)
                printf("%c", ch);
        }
        //Manual page for FIND
        else if (!strcmp(parsed[1], "FIND"))
        {
            FILE *fp;
            char ch;
            fp = fopen("FIND.txt", "r");
            while ((ch = getc(fp)) != EOF)
                printf("%c", ch);
        }
        //Manual page for HISTORY
        else if (!strcmp(parsed[1], "HISTORY"))
        {
            FILE *fp;
            char ch;
            fp = fopen("HISTORY.txt", "r");
            while ((ch = getc(fp)) != EOF)
                printf("%c", ch);
        }
        // For manual to work we must be in the same directory where final.c file is in.
        else
        {
            histarr[h_val - 1].hstat = 0;
            // This is to handle if MAN is followed by wrong command name
            printf("\nCommands supported are CD, HISTORY, EXIT, PWD, MAN, FIND\n");
            printf("\n syntax error.. please type MAN COMMAND_NAME\n");
            printf("\n Please stay in the path /shell_in_c to access the manual pages\n");
        }
    }
}

int user_implemented_commands(char **parsed)
{
    int user_commands_count = 7, i, switch_user_command = 0;
    char *user_commands_array[user_commands_count];
    char *username;

    user_commands_array[0] = "EXIT";
    user_commands_array[1] = "CD";
    user_commands_array[2] = "HELP";
    user_commands_array[3] = "HISTORY";
    user_commands_array[4] = "PWD";
    user_commands_array[5] = "MAN";
    user_commands_array[6] = "FIND";

    for (i = 0; i < user_commands_count; i++)
    {
        // Here given command is compared with list of implemented commands
        if (strcmp(parsed[0], user_commands_array[i]) == 0)
        {
            switch_user_command = i + 1;
            break;
        }
    }
    // Depending on the command given call the corresponding implementation using switch
    switch (switch_user_command)
    {
    case 1:
        // Implementation of EXIT
        printf("\nExiting Shell\n");
        exit(0);
    case 2:
        // Implementation of CD
        chdir(parsed[1]);
        histarr[h_val - 1].hstat = 1;
        return 1;
    case 3:
        // Implementation of HELP
        openHelp();
        return 1;
    case 4:
        // Implementation of HISTORY
        printf("\nlast 10 commands are\n");
        printf("---------------------\n\n");
        show_history();
        return 1;
    case 5:
        // Implementation of PWD
        PWD();
        return 1;
    case 6:
        // Implementation of MAN
        if (!strcmp(parsed[0], "MAN") && (parsed[1] != NULL))
        {
            MAN(parsed);
        }
        else
        {
            histarr[h_val - 1].hstat = 0;
            // This is to avoid segmentation error if MAN is not followed by any command name
            printf("\nCommands supported are CD, HISTORY, EXIT, PWD, MAN, FIND\n");
            printf("\n syntax error.. please type MAN COMMAND_NAME\n");
            printf("\n Please stay in the path /shell_in_c to access the manual pages\n");
        }
        return 1;
    case 7:
        // Implementation of FIND command which works like grep
        if (parsed[1] != NULL)
            FIND(parsed);
        else
        {
            printf("\n SYNTAX error. please type FIND FILENAME \n");
            histarr[h_val - 1].hstat = 0;
        }
        return 1;
    default:
        break;
    }

    return 0;
}

void is_space_in_input(char *str, char **parsed)
{

    for (int i = 0; i < MAXLIST; i++)
    {

        parsed[i] = strsep(&str, " ");
        if (parsed[i] == NULL)
        {
            break;
        }
        if (strlen(parsed[i]) == 0)
        {
            i--;
        }
    }
}

int is_pipe_in_input(char *str, char **str_piped)
{

    for (int i = 0; i < 2; i++)
    {
        str_piped[i] = strsep(&str, "|");
        if (str_piped[i] == NULL)
            break;
    }
    if (str_piped[1] == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
int process_string(char *str, char **parsed, char **parsed_pipe)
{

    char *str_piped[2];
    int piped = 0;

    piped = is_pipe_in_input(str, str_piped);

    if (piped)
    {

        is_space_in_input(str_piped[0], parsed);
        is_space_in_input(str_piped[1], parsed_pipe);
    }
    else
    {
        is_space_in_input(str, parsed);
    }

    if (user_implemented_commands(parsed))
        return 0;
    else
    {
        return 1 + piped;
    }
}

int execute_without_pipeop(char **parsed)
{

    pid_t pid = fork();

    if (pid == -1)
    {

        printf("\nFailed forking child..");
    }
    else if (pid == 0)
    {

        int stat1 = execvp(parsed[0], parsed);
        if (stat1 < 0)
        {
            printf("Couldn't execute command");
        }
        exit(stat1);
    }
    else
    {

        int stat1;
        wait(&stat1);
        stat1 = (255 - stat1) / 255;
        if (stat1 >= 0)
        {
            add_stat(1);
        }
        else
        {
            add_stat(0);
        }
        return 0;
    }
    return 0;
}
void execute_with_pipeop(char **parsed, char **parsedpipe)
{
    int pipefd[2];
    pid_t p1, p2;
    if (pipe(pipefd) < 0)
    {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0)
    {

        printf("\nCouldn't fork");
        return;
    }
    if (p1 == 0) //child 1
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0], parsed) < 0)
        {
            printf("\n could not execute command 1.. ");
            exit(0);
        }
    }
    else
    {

        p2 = fork();
        if (p2 < 0)
        {
            printf("\nCould not fork");
            return;
        }
        if (p2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);

            int stat2 = execvp(parsedpipe[0], parsedpipe);

            if (stat2 < 0)
            {
                printf("\nCould not execute command 2..");
                exit(stat2);
            }
        }
        else
        {
            // parent executing, waiting for two children
            int stat3, stat4;

            wait(&stat3);
            wait(&stat4);
            stat3 = (255 - stat3) / 255;
            stat4 = (255 - stat4) / 255;
            if (stat3 >= 0 && stat4 >= 0)
            {
                add_stat(1);
            }
            else
            {
                add_stat(0);
            }
        }
    }
}
int main(int argc, char const *argv[])
{
    char inputString[MAXCOM];
    char *arguments_with_pipe[MAXLIST];
    char *arguments_without_pipe[MAXLIST];

    int pipe_flag = 0;

    getcwd(pwd_for_man, sizeof(pwd_for_man));

    enter_shell(); // provides entry to shell
    start_shell(); // shows info about shell

    while (1)
    {

        if (read_input_from_terminal(inputString))
            continue;
        pipe_flag = process_string(inputString, arguments_without_pipe, arguments_with_pipe);

        if (pipe_flag == 1)
        {
            execute_without_pipeop(arguments_without_pipe);
        }
        if (pipe_flag == 2)
        {
            execute_with_pipeop(arguments_without_pipe, arguments_with_pipe);
        }
    }
    return 0;
}
