/***************************************************************
 *
 * file: pss.h
 *
 * @Author  Alexandros Papafragkakis (csd5084@csd.uoc.gr)
 *
 * @brief   Implementation of the "pss.h" header file for the Public Subscribe System,
 * function definitions
 *
 ***************************************************************
 */
#include <stdio.h>
#include <stdlib.h>

#include "pss.h"

struct Group G[MG]; 
struct SubInfo *S;

/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @return 0 on success
 *         1 on failure
 */
int initialize(void)
{
    S=NULL;
    int x;
    for (x = 0; x < MG; x++)
    {
        G[x].gId = x;
        G[x].gfirst = NULL;
        G[x].glast = NULL;
        G[x].ggsub = NULL;
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void)
{
    return EXIT_SUCCESS;
}

/**
 * @brief Insert info
 *
 * @param iTM Timestamp of arrival
 * @param iId Identifier of information
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Insert_Info(int iTM, int iId, int *gids_arr, int size_of_gids_arr)
{
    struct Info* new_entry;
    struct Info* iterate;
    struct Info* print_list;
    int i;
    int number;

    i = 0;
    number = gids_arr[0];


    new_entry = (struct Info*)malloc(sizeof(struct Info));
    new_entry->iId = iId;
    new_entry->itm = iTM;

    while(number!=-1){

        new_entry->igp[number] = 1;

        if(G[number].gfirst==NULL){
            new_entry->iprev=NULL;
            new_entry->inext=NULL;
            G[number].gfirst = new_entry;
            G[number].glast = new_entry; 
            
        }else if( G[number].gfirst->itm >= iTM){
            new_entry->inext=G[number].gfirst;
            G[number].glast = G[number].gfirst;
            new_entry->inext->iprev = new_entry;
            G[number].gfirst = new_entry;
        }else{
            iterate = G[number].gfirst;
            while(iterate!=NULL && iterate->itm < iTM){
                iterate=iterate->inext;
            }
            
            if(iterate!=NULL){
                new_entry->inext=iterate->inext;
                new_entry->inext->iprev=new_entry;
                iterate->inext = new_entry;
                new_entry->iprev=iterate;
            }else{
                new_entry->inext=NULL;
                new_entry->iprev=G[number].glast;
                G[number].glast = new_entry;
            }
            
            
        }
            
            
        i++;
        number = gids_arr[i];
    }
    
    i=0;
    number=gids_arr[0];

     while(number!=-1){

        print_list = G[number].gfirst;
     
        printf("GROUPID= %d INFOLIST= ",number);

        while(print_list!=NULL){
            printf("%d, ",print_list->iId);
            print_list=print_list->inext;
        }
        printf("\n");
     
        i++;
        number=gids_arr[i];
     }

    return EXIT_SUCCESS;
}
/**
 * @brief Subsriber Registration
 *
 * @param sTM Timestamp of arrival
 * @param sId Identifier of subscriber
 * @param gids_arr Pointer to array containing the gids of the Event.
 * @param size_of_gids_arr Size of gids_arr including -1
 * @return 0 on success
 *          1 on failure
 */
int Subscriber_Registration(int sTM, int sId, int *gids_arr, int size_of_gids_arr)
{
    struct SubInfo *new_entry;
    struct SubInfo *iterate;
    struct SubInfo *prev;
    struct SubInfo *print;
    int i,j;
    int number;

    iterate = S;
    prev = S;
    i = 0;
    j=0;
    number = gids_arr[0];

    new_entry = (struct SubInfo *)malloc(sizeof(struct SubInfo));

    new_entry->sId = sId;
    new_entry->stm = sTM;

    
    while (number != -1)
    {
        struct Subscription *new_sub;
        int gid = number;
        new_sub = (struct Subscription *)malloc(sizeof(struct Subscription));
        new_sub->sId = sId;
        for(j=0; j<MG; j++){
            if(gid != j){
                new_entry->sgp[j] = 1;
            }   
        }
        new_entry->sgp[gid] = G[gid].gfirst;

        new_sub->snext = G[gid].ggsub;
        G[gid].ggsub = new_sub;
        i++;
        number = gids_arr[i];
        
    }
   
    while (iterate != NULL && sTM > iterate->stm)
    {
        prev = iterate;
        iterate = iterate->snext;
    }
    if(iterate == S){
        new_entry->snext=S;
        S = new_entry;
    }else{
        new_entry->snext = iterate;
        prev->snext = new_entry;
    }
    

    print = S;
    printf("SUBSCRIBERLIST = ");
    while (print != NULL)
    {
        printf("<%d,%d>,", print->sId,print->stm);
        print = print->snext;
    }
    printf("\n");
    number = gids_arr[0];
    i=0;
    while (number != -1)
    {
        struct Subscription *iterate_sub;
        
        int gid = number;
        iterate_sub = G[gid].ggsub;

        printf("GROUPID = %d, SUBLIST = ", gid);
        while (iterate_sub != NULL)
        {
            printf("%d,", iterate_sub->sId);
            iterate_sub=iterate_sub->snext;
        }
        printf("\n");
        i++;
        number = gids_arr[i];
    }

    return EXIT_SUCCESS;
}
/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId)
{
    struct SubInfo* iterate;
    int i;

    iterate=S;

    while(iterate!=NULL && iterate->sId != sId){
        iterate=iterate->snext;
    }

    if(iterate == NULL){
        printf("Subsciber with this %d sid doesnt exist\n",sId);
        return EXIT_FAILURE;
    }

   for(i=0; i<MG; i++){
    if(iterate->sgp[i]!=1){
        struct Info* iterate_i;
        if(iterate->sgp[i]==NULL){
            iterate_i=G[i].gfirst;
            iterate->sgp[i] = G[i].gfirst;
        }else{
            iterate_i=iterate->sgp[i];
        }
        printf("GROUPID= %d, INFOLIST= ",i);
        
        while(iterate_i!=NULL){
            printf("%d,",iterate_i->iId);
            iterate_i=iterate_i->inext;
        }
        printf("NEWSGP= %d",iterate->sgp[i]->iId);
        iterate_i = G[i].gfirst;

        printf("\n");
    }

   }


    return EXIT_SUCCESS;
}
/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(int sId)
{

    struct SubInfo* iterate;
    struct SubInfo* prev;
    int i;

    iterate=S;
    prev=S;

    while(iterate!=NULL && iterate->sId!=sId) {
        prev=iterate;
        iterate=iterate->snext;
    }

    if(iterate==NULL) {
        printf("Subscriber %d doesnt exist\n",sId);
        return EXIT_FAILURE;
    }

    if(iterate==S){
        S=S->snext;
    }else{
        prev->snext=iterate->snext;
    }

    for(i=0; i<MG; i++){
        struct Subscription* sub_iterate;
        struct Subscription* sub_prev;

        if(iterate->sgp[i]!=1){

            sub_iterate=G[i].ggsub;
            prev=G[i].ggsub;

            while(sub_iterate!=NULL && sub_iterate->sId != sId){
                sub_prev=sub_iterate;
                sub_iterate=sub_iterate->snext;
            }

            if(sub_iterate==NULL){
                printf("Subscription %d doesnt exist\n",sId);
                return EXIT_FAILURE;
            }

            if(sub_iterate==prev){
                G[i].ggsub=G[i].ggsub->snext;
                free(sub_iterate);
            }else{
                sub_prev->snext=sub_iterate->snext;
                free(sub_iterate);
            }
            
        }

        
    
    }
    free(iterate);

    iterate = S;

    printf("SUBSCRIBERLIST= ");
    while(iterate!=NULL){
        printf("%d,",iterate->sId);
        iterate=iterate->snext;
    }
    printf("\n");

    for(i=0; i<MG; i++){
        struct Subscription* sub_iterate;

        sub_iterate=G[i].ggsub;

        printf("GROUPID= %d SUBLIST= ",G[i].gId);
        
        while(sub_iterate!=NULL){
            printf("%d,",sub_iterate->sId);
            sub_iterate=sub_iterate->snext;
        }
        printf("\n");

    }

    return EXIT_SUCCESS;
}
/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void)
{
    int i;
    struct SubInfo* subscribe_iterate;
    int count=0;
    int group_count=0;

    subscribe_iterate=S;

    for(int i=0; i<MG; i++){
        struct Info* iterate;
        struct Subscription* sub_iterate;

        if(G[i].gfirst!=NULL || G[i].ggsub!=NULL){
            group_count++;
        
            iterate = G[i].gfirst;
            sub_iterate = G[i].ggsub; 

            printf("GROUPID= %d INFOLIST= ",G[i].gId);

            while(iterate!=NULL){
                printf("%d,",iterate->iId);
                iterate=iterate->inext;
            }
            printf(" SUBLIST= ");
            while(sub_iterate!=NULL){
                printf("%d,",sub_iterate->sId);
                sub_iterate=sub_iterate->snext;
            }
            printf("\n");
        }

    }
    printf("\nSUBSCRIBERLIST= ");
    while(subscribe_iterate!=NULL){
        printf("%d,",subscribe_iterate->sId);
        count++;
        subscribe_iterate=subscribe_iterate->snext;
    }
    printf("\n");
    subscribe_iterate=S;

    while(subscribe_iterate!=NULL){
        printf("SUBSCRIBERID= %d,GROUPLIST= ",subscribe_iterate->sId);
        for(i=0; i<MG; i++){
            if(subscribe_iterate->sgp[i]!=1){
                printf("%d,",G[i].gId);
            }
        }
        printf("\n");
        subscribe_iterate=subscribe_iterate->snext;
    }
    printf("\nNO_GROUPS = %d\n",group_count);
    printf("NO_SUBSCRIBERS = %d\n",count);



    return EXIT_SUCCESS;
}
