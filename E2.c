//Notes: at the time, I forgot the fact that the instructor would compile the code on a Unix distribution.  I compiled this on Windows.... :(
//Modification: added more comments as a self-review
//Date due: 9/5/2021
//Compiler: Visual Studio Code using MinGW
//Desc: A C Program that creates a directory structure for all system programming files
//directory structure is as follows:
//		<root>
//test			program				exercise
//test1 test2 test3	prog1 prog2 prog3 prog4 prog5	exer1 exer2 exer3 //                                                    exer4 exer5 exer6 
//                                                    exer7 exer8 exer9 
//                                                    exer10
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/limits.h>

#define PATH_MAX 4096

int main(int argc, char*argv[])
{
    /**************************************************************/
    //declaring variables
    //buffers used to hold directory name
    char cwd1[PATH_MAX];
    char cwd2[PATH_MAX];
    //counter variable for loops
    char i;
    //
    int fd;
    //
    char t[] = "test0";
    char p[] = "program0";
    char e[] = "exercise0";
    char e2[] = "exercise10";
    /**************************************************************/
    //grabbing directory paths and making directories
    //stores path to current directory
    //the directory where you called this program
    char *current = getcwd(cwd1, sizeof(cwd1));
    //the following will create a root directory
    //Example call is ./setupClass "name"  If "name" is not empty, 
    //a directory is made with the name "name"
    if (argv[1] != NULL)
    {
        //make a directory with name in argv[1]
        //I forgot what 0777 stood for, but it was supposed to grant permissions to read, write, and search for owner, group, and others 
        mkdir(argv[1], 0777);
        //open the directory
        chdir(argv[1]);
    } else {
        //directory will name itself "myRoot" if no "name" is given
        mkdir("myRoot", 0777);
        chdir("myRoot");
    }
    //we are currently in the root directory
    //stores path to root directory
    char *root = getcwd(cwd2, sizeof(cwd2));//the root directory
    //create three subdirectories within root
    mkdir("test", 0777);
    mkdir("program", 0777);
    mkdir("exercise", 0777);
    /**************************************************************/
    //create files within each subdirectory and in numerical order
    //I did this by moving into each directory and looping file creation
    chdir("test");
    for (i = '1'; i<'4'; i++)//creates test files from 1 to 3
    {
        t[4] = i;
        fd = open(t, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    }

    chdir(root);
    chdir("program");
    for (i = '1'; i<'6'; i++)//creates program files from 1 to 5
    {
        p[7] = i;
        fd = open(p, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    }

    chdir(root);
    chdir("exercise");
    for (i = '1'; i<='9'; i++)//creates exercise files from 1 to 10
    {
        e[8] = i;
        fd = open(e, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    }
    e2[9] = '0';
    fd = open(e2, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    /**************************************************************/
    //exit the main function by returning a 0
    return 0;
}
