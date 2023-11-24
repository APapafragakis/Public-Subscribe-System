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

/**
 * @brief Optional function to initialize data structures that
 *        need initialization
 *
 * @param m Size of the hash table.
 * @param p Prime number for the universal hash functions.
 *
 * @return 0 on success
 *         1 on failure
 */

int size_of_array;
int size_of_hash;

struct Group G[MG]; 
struct SubInfo *T[MG];
int rand_a,rand_b;


int hash(int sId){
    return ((rand_a*sId+rand_b) % size_of_hash) % size_of_array;
}

int initialize(int m, int p){

    size_of_array = m;
    size_of_hash = p;
    rand_a = (rand() % (size_of_hash - 1 + 1)) + 1;
    rand_b = (rand() % (size_of_hash - 0 + 1)) + 0;

    for(int i = 0; i<MG; i++){
        G[i].gId = i;
        G[i].gsub = NULL;
        G[i].gr = NULL;
        T[i] = NULL;
    }
   
    return EXIT_SUCCESS;
}

/**
 * @brief Free resources
 *
 * @return 0 on success
 *         1 on failure
 */
int free_all(void){
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

struct TreeInfo* Create_Tree(int tId, int ttm){
    struct TreeInfo* new_tree = (struct TreeInfo*)malloc(sizeof(struct TreeInfo));
    new_tree -> tId = tId;
    new_tree -> ttm = ttm;
    new_tree -> tlc = NULL;
    new_tree -> trc = NULL; 
    new_tree -> tp = NULL;
    new_tree -> next = NULL;
    return new_tree; 
}

struct TreeInfo* BST_LeafInsert(struct Info* root, struct TreeInfo* tgp_root, struct TreeInfo **L, struct TreeInfo **prev){
    if(tgp_root == NULL || tgp_root == 1){
        struct TreeInfo* new_node = Create_Tree(root -> iId, root -> itm);
        if(*L == NULL){
            *L = new_node;
        }else if(*prev != NULL){
            (*prev) -> next = new_node;
        }
        (*prev) = new_node;
        return new_node;
    }
   
    if(tgp_root -> ttm >= root -> itm){
        struct TreeInfo* new_node = BST_LeafInsert(root, tgp_root -> tlc, L, prev);
        struct TreeInfo* dummy_node;

        if(tgp_root->tlc == NULL){
           
            dummy_node = Create_Tree(root -> iId, root -> itm);
            dummy_node->trc=tgp_root;
            dummy_node->tlc = new_node;
            if(tgp_root->tp==NULL){
                new_node->tp=dummy_node;
                tgp_root->tp=dummy_node;
                
            }else{
                dummy_node->tp=tgp_root->tp;
                if(tgp_root->tp->ttm >= dummy_node->ttm){
                    tgp_root->tp->tlc=dummy_node;
                }else{
                    tgp_root->tp->trc=dummy_node;
                }
                
            }
            return dummy_node;
           
        }else{
            tgp_root -> tlc = new_node;
            tgp_root -> tlc -> tp = tgp_root;
        }

        
    
    }else{
        struct TreeInfo* new_node = BST_LeafInsert(root, tgp_root -> trc, L, prev);
        struct TreeInfo* dummy_node;

        if(tgp_root->trc == NULL){
            dummy_node = Create_Tree(root -> iId, tgp_root -> ttm);
            dummy_node->trc=new_node;
            dummy_node->tlc = tgp_root;
            new_node->tp=dummy_node;
            
            if(tgp_root->tp==NULL){
                return dummy_node;
            }else{
               dummy_node->tp=tgp_root->tp;
               tgp_root->tp->trc = dummy_node;
            }
            return dummy_node;
           
        }else{
            tgp_root -> trc = new_node;
            tgp_root -> trc -> tp = tgp_root;
        }

       
    }
    return tgp_root;
}

void BST_Iterate(struct Info* root, struct TreeInfo **tgp_root, struct TreeInfo **L, struct TreeInfo **prev){
    if(root == NULL){
        return;
    }
    BST_Iterate(root -> ilc, tgp_root, L, prev);
    *tgp_root = BST_LeafInsert(root, *tgp_root, L, prev);
    BST_Iterate(root -> irc, tgp_root, L, prev);
}

void BST_Print(struct Info *ir){
    if(ir == NULL){
        return;
    }
    BST_Print(ir->ilc);
    printf("%d ",ir -> iId);
    BST_Print(ir->irc);
}

void BST_Print_Leaf(struct TreeInfo *tr){
    if(tr == NULL || tr == 1){
        return;
    }
    BST_Print_Leaf(tr->tlc);
    printf("%d ",tr -> ttm);
    BST_Print_Leaf(tr->trc);
}

void BST_Print_itm(struct Info *ir){
    if(ir == NULL){
        return;
    }
    BST_Print(ir->ilc);
    printf("%d ",ir -> itm);
    BST_Print(ir->irc);
}

struct Info* Create(int iId, int itm, int* gids_arr, int size_of_gids_arr){
    struct Info* NewTree = (struct Info*)malloc(sizeof(struct Info));
    NewTree -> iId = iId;
    NewTree -> itm = itm;
    NewTree -> ilc = NULL;
    NewTree -> irc = NULL;
    NewTree -> ip = NULL;
    for(int i = 0; i<MG; i++){
        for(int j = 0; j<size_of_gids_arr-1; j++){
            if(gids_arr[j] == i){
                NewTree->igp[i] = 1; 
            }else{
                NewTree->igp[i] = 0; 
            }
        }
    }
    return NewTree;
}

struct Info* BST_Insert(struct Info *ir, int iId, int itm, int* gids_arr, int size_of_gids_arr){
    if(ir == NULL){
        return Create(iId,itm, gids_arr, size_of_gids_arr);
    }
    if(ir-> iId > iId){
        ir -> ilc = BST_Insert(ir -> ilc, iId, itm, gids_arr, size_of_gids_arr);
        ir -> ilc -> ip = ir;
    
    }else{
        ir -> irc = BST_Insert(ir -> irc, iId, itm, gids_arr, size_of_gids_arr);
        ir -> irc -> ip = ir;
    }
    return ir;
}

int Insert_Info(int iTM,int iId,int* gids_arr,int size_of_gids_arr){
    for(int i = 0; i<size_of_gids_arr-1; i++){
        G[gids_arr[i]].gr = BST_Insert(G[gids_arr[i]].gr ,iId, iTM, gids_arr, size_of_gids_arr);
        printf("GROUP ID = %d INFOLIST = ",gids_arr[i]);
        BST_Print(G[gids_arr[i]].gr);
        printf("\n");
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
int Subscriber_Registration(int sTM,int sId,int* gids_arr,int size_of_gids_arr){
    struct SubInfo* new_node = (struct SubInfo*)malloc(sizeof(struct SubInfo));
    
    for(int i = 0; i<MG; i++){
        new_node -> sgp[i] = 1;
        new_node -> tgp[i] = 1;
    }
    int i = 0;
    struct TreeInfo* L, *prev;
    while(gids_arr[i] != -1){
        //new_node -> sgp[gids_arr[i]] = G[gids_arr[i]].gr;
        L = NULL;
        prev = NULL;
        BST_Iterate(G[gids_arr[i]].gr, &new_node -> tgp[gids_arr[i]], &L, &prev);
        new_node -> sgp[gids_arr[i]] = L;
        struct Subscription* new_sub = (struct Subscription*)malloc(sizeof(struct Subscription));
        new_sub -> sId = sId;
        new_sub -> snext = G[gids_arr[i]].gsub;
        G[gids_arr[i]].gsub = new_sub;
        i++;
    }
    int index = hash(sId);
    new_node -> sId = sId;
    new_node -> stm = sTM;
    if(T[index] == NULL || T[index] -> sId >= sId) {
        new_node -> snext = T[index];
        T[index] = new_node;
    }else{
        struct SubInfo* iterate; 
        struct SubInfo* prev; 
        iterate = T[index];
        while(iterate != NULL && iterate -> sId < sId){
            prev = iterate;
            iterate = iterate -> snext;
        }
        if(iterate == NULL){
            prev -> snext = new_node;
            new_node -> snext = NULL;
        }else{
            prev -> snext = new_node;
            new_node -> snext = iterate;
        }
    }
    printf("SUBSCRIBERLIST = \n");
    for(int i = 0; i<size_of_array; i++){
        struct SubInfo* temp = T[i];
        printf("%d: ", i);
        while(temp != NULL){
            printf("%d ", temp -> sId);
            temp = temp -> snext;
        }
        printf("\n");
    }
    i = 0;
    while(gids_arr[i] != -1){
        printf("GROUPID = %d ", G[gids_arr[i]].gId);
        printf("SUBLIST = ");
        struct Subscription* tmp = G[gids_arr[i]].gsub;
        while(tmp != NULL){
            printf("%d ", tmp -> sId);
            tmp = tmp -> snext;
        }
        i++;
        printf("\n");
    }
    return EXIT_SUCCESS;
}
/**
 * @brief Prune Information from server and forward it to client
 *
 * @param tm Information timestamp of arrival
 * @return 0 on success
 *          1 on failure
 */

struct Info* Delete_Tree(struct Info* root, int tm, struct Group group){
    if(root == NULL) return NULL; 
    root -> ilc = Delete_Tree(root -> ilc, tm, group);
    root -> irc = Delete_Tree(root -> irc, tm, group);
    if(root -> itm <= tm && root -> irc == NULL && root -> ilc == NULL){
        struct Subscription* iterate = group.gsub;
        while(iterate != NULL){
            int pos = hash(iterate -> sId);
            struct SubInfo *tmp = T[pos];
            struct TreeInfo *prev = NULL;
            while(tmp != NULL && tmp -> sId != iterate -> sId){
                tmp = tmp -> snext;
            } 
            if(tmp != NULL){
                tmp -> tgp[group.gId]=BST_LeafInsert(root, tmp -> tgp[group.gId], &tmp -> sgp[group.gId], &prev);
            }
            iterate = iterate -> snext;
        } 
        return NULL;
    }
    return root;
}

int Prune(int tm){
    int pos;
    for(int i = 0; i<MG; i++){
        struct Subscription* iterate;
        iterate = G[i].gsub;
        G[i].gr = Delete_Tree(G[i].gr,tm, G[i]);
        printf("GROUPID = %d ", G[i].gId);
        printf("INFOLIST =  ");
        BST_Print(G[i].gr);
        printf("SUBLIST: ");
        while(iterate != NULL){
            printf("%d ", iterate -> sId);
            iterate = iterate -> snext;
        }
        printf("\n");
    }
    printf("\n");

    for(int i = 0; i<size_of_array; i++){
        struct SubInfo* temp = T[i];
        while(temp != NULL){
            printf("SUBSCRIBERID = %d ", temp -> sId);
            printf("GROUPLIST = \n");
            for(int i = 0; i<MG; i++){
                if(temp -> sgp[i] != 1){
                    printf("%d ", i);
                    printf("TREELIST = ");
                    BST_Print_Leaf(temp -> tgp[i]);
                    printf("\n");
                }
                
            }
            temp = temp -> snext;
        }
        printf("\n");
    }
    return EXIT_SUCCESS;
}

struct TreeInfo* FindR(struct TreeInfo* tr){
    if(tr == NULL){
        return NULL;
    }
    if(tr -> trc == NULL){
        return tr;
    }
    FindR(tr -> trc);
}

/**
 * @brief Consume Information for subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Consume(int sId){
    int pos = hash(sId);
    struct SubInfo* iterate = T[pos];
    while(iterate != NULL && iterate -> sId != sId){
        iterate = iterate -> snext;
    }
    if(iterate == NULL){
        printf("User does not exist.");
        return EXIT_FAILURE;
    }
    for(int i = 0; i<MG; i++){
        if(iterate -> tgp[i] != 1 && iterate -> sgp[i] != 1){
            struct TreeInfo* right_leaf = FindR(iterate -> tgp[i]);
            struct TreeInfo* left_leaf = iterate -> sgp[i];
            printf("GROUPID = %d ", i);
            printf("TREELIST = ");
            while(left_leaf != NULL && left_leaf != right_leaf){
                printf("%d ", left_leaf -> tId);
                left_leaf = left_leaf -> next;
            }
            iterate -> sgp[i] = right_leaf; 
            printf("NEWSGP = %d ", right_leaf -> tId);
            printf("\n");
        }
    }
    return EXIT_SUCCESS;
}

void Delete_T(struct TreeInfo* root){
    if(root == NULL){
        return;
    }
    Delete_T(root -> trc);
    Delete_T(root -> tlc);
    free(root);
}

/**
 * @brief Delete subscriber
 *
 * @param sId Subscriber identifier
 * @return 0 on success
 *          1 on failure
 */
int Delete_Subscriber(int sId){
    int pos = hash(sId);
    struct SubInfo* iterate = T[pos];
    struct SubInfo* prev = NULL;
    while(iterate != NULL && iterate -> sId != sId){
        prev = iterate;
        iterate = iterate -> snext;
    }
    if(iterate == NULL){
        printf("User does not exist.");
        return EXIT_FAILURE;
    }
    if(iterate == T[pos]){
        T[pos] = iterate -> snext;
    }else{
        if(iterate -> snext == NULL){
            prev -> snext = NULL;
        }else{
            prev -> snext = iterate -> snext;
        }
    }
    for(int i = 0; i<MG; i++){
        struct Subscription* tmp, *prev;
        if(iterate -> tgp[i] != 1){
            tmp = G[i].gsub;
            Delete_T(iterate -> tgp[i]);
            iterate -> sgp[i] = NULL;
            while(tmp != NULL && tmp -> sId != sId){
                prev = tmp;
                tmp = tmp -> snext;
            }
            if(tmp == G[i].gsub){
                G[i].gsub = tmp -> snext;
            }else{
                if(tmp -> snext == NULL){
                    prev -> snext = NULL;
                }else{
                    prev -> snext = tmp -> snext; 
                }
            } 
            free(tmp);
        }
    }
    
    printf("SUBSCRIBERLIST = ");
    for(int i = 0; i<size_of_array; i++){
        struct SubInfo* itr = T[i];
        while(itr != NULL){
            printf("%d ", itr -> sId);
            itr = itr -> snext;
        }
    }
    printf("\n");
    for(int i = 0; i<MG; i++){
        if(iterate -> tgp[i] != 1){
            struct Subscription* temp = G[i].gsub;
            printf("GROUPID = %d ", i);
            printf("SUBLIST = ");
            while(temp != NULL){
                printf("%d ", temp -> sId);
                temp = temp -> snext; 
            }
            printf("\n");
        }
    }

    free(iterate);
    return EXIT_SUCCESS;
}

/**
 * @brief Print Data Structures of the system
 *
 * @return 0 on success
 *          1 on failure
 */
int Print_all(void){
    int number_of_groups = 0;
    int number_of_subs = 0;
    for(int i = 0; i<MG; i++){
        printf("GROUPID = %d ", G[i].gId);
        printf("INFOLIST = ");
        BST_Print(G[i].gr);
        if(G[i].gr != NULL || G[i].gsub != NULL){
            number_of_groups++;
        }
        struct Subscription* iterate = G[i].gsub;
        printf("SUBLIST = ");
        while(iterate != NULL){
            printf("%d ", iterate -> sId);
            iterate = iterate -> snext;
        }
        printf("\n");
    }
    struct SubInfo* tmp = NULL;
    printf("SUBSCRIBERLIST = ");
    for(int i = 0; i<size_of_array; i++){
        tmp = T[i];
        while(tmp != NULL){
            printf("%d ", tmp -> sId);
            tmp = tmp -> snext;
            number_of_subs++;
        }
    }
    printf("\n");
    for(int i = 0; i<size_of_array; i++){
        tmp = T[i];
        while(tmp != NULL){
            if(tmp -> tgp[i] != 1){
                printf("GROUPID = %d ", i);
                printf("TREELIST = ");
                BST_Print_Leaf(tmp -> tgp[i]);
                printf("\n");
            }
            tmp = tmp -> snext;
        }
    }
    printf("NO_GROUPS = %d ", number_of_groups);
    printf("NO_SUBSCRIBERS = %d ", number_of_subs);
    printf("\n");
    return EXIT_SUCCESS;
}
