/* A2-CreatingAClassDirectoryFramework */
//Date: 9/5/2021
//Compiler: Visual Studio Code using MinGW
//Desc: A C Program that creates a directory structure for all system programming files
//directory structure is as follows:
//		                        <root>
//      test			        program				      exercise
//test1 test2 test3	  prog1 prog2 prog3 prog4 prog5	  exer1 exer2 exer3 
//                                                    exer4 exer5 exer6 
//                                                    exer7 exer8 exer9 
//                                                    exer10
//Issue: was supposed to use compiler-> gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
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
    //declaring variables
    /**************************************************************/
    char cwd1[PATH_MAX];
    char cwd2[PATH_MAX];
    char i;
    int fd;
    char t[] = "test0";
    char p[] = "program0";
    char e[] = "exercise0";
    char e2[] = "exercise10";

    //grabbing directory paths and making directories
    /**************************************************************/
    char *current = getcwd(cwd1, sizeof(cwd1));
    if (argv[1] != NULL) {
        //I forgot what 0777 stood for, but it was supposed to grant 
        //permissions to read, write, and search for owner, group, and others 
        mkdir(argv[1], 0777);
        chdir(argv[1]);
    } else {
        mkdir("myRoot", 0777);
        chdir("myRoot");
    }
    //we are currently in <root>
    char *root = getcwd(cwd2, sizeof(cwd2));
    //create three subdirectories within root
    mkdir("test", 0777);
    mkdir("program", 0777);
    mkdir("exercise", 0777);
    
    //create files within each subdirectory and in numerical order*/
    /**************************************************************/
    chdir("test");
    for (i = '1'; i<'4'; i++)
    {
        t[4] = i;
        fd = open(t, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    }

    chdir(root);
    chdir("program");
    for (i = '1'; i<'6'; i++)
    {
        p[7] = i;
        fd = open(p, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    }

    chdir(root);
    chdir("exercise");
    for (i = '1'; i<='9'; i++)
    {
        e[8] = i;
        fd = open(e, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    }
    e2[9] = '0';//This line is completely unnecessary through being overkill.  I will keep it for the overkill.
    fd = open(e2, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    return 0;
}
