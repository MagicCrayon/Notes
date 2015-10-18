#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "notes.h"

/*
 *  TODO
 *  - Add errno things
 *  - Clear All Notes Function
 *  - [BUG] Segment Dump When "./main -l" and .notes.dat not exist 
 */

int main(int argc, char* argv[])
{
    
    int lflag, aflag, dflag, hflag = 0;             /* Flags For getopt                 */
    int c;                                          /* Temp For getopt                  */
    char note[256] = {0};                           /* Buffer Where Note Is Stored      */
    int fd;                                         /* File Descripter                  */
    const char *filename = ".notes.dat";            /* Where Notes Will Be Stored       */
    char *line = NULL;                              /* Single Line Buffer               */
    int LineToDelete;                               /* Which Line To Delete             */

    opterr = 0;

    /*
     * Check For Correct Arguments
     */
    if (argc != 2)
    {
        printf("[*]Invalid Option\n"); // TODO Fix that message
        exit(1);
    }

    while ((c = getopt (argc, argv, "ladh")) != -1)
    {
        switch(c)
        {
            case 'l':
                lflag = 1;
                break;
            case 'a':
                aflag = 1;
                break;
            case 'd':
                dflag = 1;
                break;
            case 'h':
                hflag = 1;
                break;
            default:
                exit(-1);
        }
    }

    /*
     * Add Note
     */
    if (aflag == 1)
    {
        fgets(note, sizeof(note), stdin);
        
        int fd = open(filename, O_RDWR | O_APPEND | O_CREAT, 0644); // TODO Replace that with fopen
        if (fd == -1)
        {
            printf("Error in notes.dat");
        }

        if ( write(fd, note, strlen(note)) != strlen(note) ) // TODO strlen to strnlen
        {
            printf("Write Error\n");
            exit(-1);
        }

        close(fd);
        exit(1);
    }

    /*
     * List Note
     */
    if (lflag == 1)
    {   
        char line[81], descrip[10];
        float price;
        FILE *inFile;

        inFile = fopen(filename, "r");

        while (fgets(line, 81, inFile) != NULL)
        {
            printf("%s", line);
        }

        fclose(inFile);
                
        exit(1);
    }

    /*
     * Delete Note
     */
    if (dflag == 1)
    {
        printf("Which Note? ");
	scanf("%d", &LineToDelete); // TODO Change to fgets later for secuirty reasons 
	
	delete(LineToDelete, filename);
        
        exit(1);
    }

    /*
     * Help
     */
    if (hflag == 1)
    {
        usage();
        exit(1);
    }

    return 0;
}

// TODO Create Better Help
void usage()
{
    printf("./main [arg]\n");
    printf("-l  -   List Notes\n");
    printf("-a  -   Add Notes\n" );
    printf("-d  -   Delete Notes\n");
    printf("-h  -   View Help\n" );
}

int delete(int deleteLine, const char *file) 
{
  
  FILE *File1, *File2;
  int ln = 1;
  int LineToDelete = deleteLine;
  int ch;
  
  File1 = fopen(file, "r");
  
  rewind(File1);
  
  File2 = fopen("replica.dat", "w");
  
  while (EOF != (ch = getc(File1))) 
  {
    if ('\n' == ch)
        ++ln;
    if (LineToDelete != ln)
        putc(ch, File2);
  }
  
  fclose(File1);
  fclose(File2);
  remove(file);
  
  rename("replica.dat", file);
  
  File1 = fopen(file, "r");
  
  ch = getc(File1);
  while (ch != EOF)
  {
    printf("%c", ch);
    ch = getc(File1);
  }
  
  fclose(File1);
  
  return 0;
}

