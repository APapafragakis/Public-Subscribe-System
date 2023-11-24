/***************************************************************
 *
 * file: main.c
 *
 * @Author  Alexandros Papafragkakis (csd5084@csd.uoc.gr)
 * 
 * @brief   Main function for the needs of CS-240 project 2022.Prof. Panagiota Fatourou.
 * @see     Compile with command: gcc -ansi main.c pss.c -o run
 ***************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <ctype.h>

#include "pss.h"

#define BUFFER_SIZE 1024 /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints
 * or comment to disable it */
#define DEBUG
#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */

/**
 * @brief Parsing event arguments from buffer
 *
 * @param buff String of current line in the test file
 * @param int_arr_size Pointer to integer that represents the number of arguments in the test after the Event char
 *
 * @return 0 on success
 *         1 on failure
 */
static int * event_args(char *buff,unsigned int * int_arr_size){
    int * int_arr;
    char event='\0';
    int value;
    char *p =buff+1;
    unsigned int len = 0;
    unsigned int count_args=1;
    unsigned int i =0;
    sscanf(buff, "%c", &event);
    while (sscanf(p, "%d%n", &value, &len)==1 && value!=-1 ) {
        count_args++;
        p += len;
    }
    p=buff+1;
    (*int_arr_size)=count_args;
    int_arr= malloc(count_args*sizeof(int));
    while (sscanf(p, "%d%n", &value, &len)==1) {
        int_arr[i++]=value;
        p += len;
        if(value ==-1){
            break;
        }
    }

    return int_arr;
}

/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char **argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event='\0';

	/* Check command buff arguments */
	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s <m> <p> <input_file>\n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Parse command buff arguments */
	int hashTableSize = atoi(argv[1]);
	int universalHashingNumber = atoi(argv[2]);

	if (hashTableSize <= 0)
	{
		fprintf(stderr, "\n Invalid hash table size: %d\n", hashTableSize);
		perror("Parsing command line argument\n");
		return EXIT_FAILURE;
	}

	if (universalHashingNumber <= 0)
	{
		fprintf(stderr, "\n Invalid universal hashing number: %d\n", universalHashingNumber);
		perror("Parsing command line argument\n");
		return EXIT_FAILURE;
	}

	/* Open input file */
	if ((fin = fopen(argv[3], "r")) == NULL)
	{
		fprintf(stderr, "\n Could not open file: %s\n", argv[3]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}
    
	/* Initializations */
	initialize(hashTableSize, universalHashingNumber);

	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin))
	{
		DPRINT("\n>>> Event: %s\n", buff);
		switch (buff[0])
		{

		/* Comment */
		case '#':
			break;

		/* Insert_Info
		 * I <itm> <iId> <gId1> <gId2> ... <gIdk> -1 */
		case 'I':
		{
			int itm;
			int iId;
            unsigned int num_of_gids;
            int * event_args_arr;
            int * gids_arr;
            event_args_arr=event_args(buff, &num_of_gids);
            itm=event_args_arr[0];
            iId=event_args_arr[1];
            gids_arr= event_args_arr+2;
            num_of_gids-=2;
			if (Insert_Info(itm,iId,gids_arr,num_of_gids)==0)
			{
				DPRINT("%c <%d> <%d> DONE\n",buff[0], itm,iId);
			}
			else
			{
				fprintf(stderr, "%s failed\n", buff);
			}
            num_of_gids=0;
            free(event_args_arr);
			break;
		}

		/* Subscriber_Registration
		 * S <sΤΜ> <sId> <gId1> <gId2> ... <gIdm> -1  */
		case 'S':
		{
            int sTM, sId;
            unsigned int num_of_gids;
            int * event_args_arr;
            int * gids_arr;
            event_args_arr=event_args(buff, &num_of_gids);
            sTM=event_args_arr[0];
            sId=event_args_arr[1];
            gids_arr= event_args_arr+2;
            num_of_gids-=2;
            if (Subscriber_Registration(sTM,sId,gids_arr,num_of_gids)==0)
            {
                DPRINT("%c <%d> <%d> DONE\n", buff[0],sTM,sId);
            }
            else
            {
                fprintf(stderr, "%s failed\n", buff);
            }
            num_of_gids=0;
            free(event_args_arr);
			break;
		}

		/* Prune
		 * R <tm> */
		case 'R':
		{
            int tm;
			sscanf(buff, "%c %d", &event, &tm);
			if (Prune(tm)==0)
			{
				DPRINT("%c <%d> DONE\n", event, tm);
			}
			else
			{
				fprintf(stderr, "%c %d failed\n", event, tm);
			}
			break;
		}

		/* Consume
		 * C <sId> */
		case 'C':
		{
            int sId;
			sscanf(buff, "%c %d", &event, &sId);
			if (Consume(sId)==0)
			{
				DPRINT("%c <%d> DONE\n", event,sId);
			}
			else
			{
				fprintf(stderr, "%c %d failed\n", event,sId);
			}
			break;
		}

		/* Delete_Subscriber
		 * D <sId>: */
		case 'D':
		{
			int sId;
			sscanf(buff, "%c %d", &event, &sId);
            if (Delete_Subscriber(sId)==0)
			{
				DPRINT("%c <%d> DONE\n", event, sId);
			}
			else
			{
				fprintf(stderr, "%c %d failed\n", event, sId);
			}
			break;
		}

		/* Print
		 * P */
		case 'P':
		{
			if (Print_all()==0)
			{
				DPRINT("%c DONE\n", buff[0]);
			}
			else
			{
				fprintf(stderr, "%c failed\n", buff[0]);
			}

			break;
		}
		/* Empty line */
		case '\n':
			break;
		/* Ignore everything else */
		default:
			DPRINT("Ignoring line: %s \n", buff);
			break;
		}
	}

	free_all();
	return (EXIT_SUCCESS);
}
