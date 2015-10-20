#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "notes.h"

/*
 *  TODO
 *  - Add errno things
 *  - Add Proper Error Messages
 *  - Clear All Notes Function
 *  - [BUG] Segment Dump When "./main -l" and .notes.dat not exist
 *      - Add Check for file existance
 *  - Maybe change '-a' to accept string in argument
 */

int main(int argc, char* argv[])
{
    
    int lflag, aflag, dflag, cflag, hflag = 0;      /* Flags For getopt                 */
    int c;                                          /*  Arg temp getopt                 */
    char note[256] = {0};                           /* Buffer Where Note Is Stored      */
    int fd;                                         /* File Descripter                  */
    const char *filename = ".notes.dat";            /* Where Notes Will Be Stored       */
    int LineToDelete;                               /* Which Line To Delete             */

    opterr = 0;

    /*
     * Check For Correct Arguments
     */
    if (argc != 2)
    {
        printf("Usage: %s <argument>\n", argv[0]);
        exit(1);
    }

    /*
     * Handle Parameters
     */
    while ((c = getopt (argc, argv, "ladch")) != -1)
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
            case 'c':
                cflag = 1;
                break;
            case 'h':
                hflag = 1;
                break;
            default:
                printf("%s: invalid option - '%c'\n", argv[0], c);
                printf("Try '%s -h' for more information.\n", argv[0]);
                exit(1);
        }
    }

    /*
     * Add Note
     */
    if (aflag == 1)
    {
        fgets(note, sizeof(note), stdin);
        
        fd = open(filename, O_RDWR | O_APPEND | O_CREAT, 0644); // TODO Replace that with fopen
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
        char line[81];
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
     * Clear Notes
     */
    if (cflag == 1)
        clear(filename);

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

void usage()
{
    printf("./main [argument]\n"   ); // TODO add argv[0]
    printf("-l  -   List Notes\n"  );
    printf("-h  -   Show Help\n"   );
    printf("-a  -   Add Notes\n"   );
    printf("-d  -   Delete Notes\n");
    printf("-c  -   Clear Notes\n ");
}

int delete(int deleteLine, const char *file) 
{
  
  FILE *File1, *File2;
  int ln = 1;
  int LineToDelete = deleteLine;
  int ch;
  
  if ( (File1 = fopen(file, "r")) == NULL)
  {
      printf("Error Opening file");
      exit(-1);
  }
  
  rewind(File1);
  
  if ( (File2 = fopen("replica.dat", "w")) == NULL)
  {
    printf("Error Opening replica\n");
    exit(-1);
  }
  
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
  
  return 0;
}

int clear(const char *file)
{
    if ((remove(file)) == -1)
    {
        fprintf(stderr, "Clearing Failed\n"); // TODO proper error msg
        exit(-1);
    }

    return 0;
}
