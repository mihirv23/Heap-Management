#include<stdio.h>
#define METADATA_SIZE sizeof(metadata)
#define FREE_SPACE_SIZE 40000

typedef enum{FALSE,TRUE} bool;

typedef struct metadata_tag{
    int size ;
    bool available;
    struct metadata_tag* next ;
}metadata;

int status_intialize = 1;
metadata* free_lptr ;
char free_space[FREE_SPACE_SIZE]; // defines a block of space where memory allocation can happen


void initialize(){
    free_lptr = (metadata*)free_space; //pointer to the free space 
    free_lptr->available = TRUE;
    free_lptr->size = FREE_SPACE_SIZE - METADATA_SIZE; //intially pura khali hoga and metadata will be occupying some space 
    free_lptr->next = NULL;
    printf("Status:%d ,Size Available: %d \n",free_lptr->available,free_lptr->size);//because of enum status ke liye %d

}
metadata* split(metadata* curr, int size_mem_req){
    //curr it is of the node at which the split is to be done 
    
    metadata* a = curr->next;
    metadata* new_next = (metadata*)((char*)curr + METADATA_SIZE + size_mem_req) ;
    new_next->size = (curr->size - size_mem_req - METADATA_SIZE) ;
    new_next->available = TRUE ;
    new_next->next = a;
    curr->next = new_next ;
    curr->available = FALSE ; //allocated to the alloc list 
    curr->size = size_mem_req ;
    // in the allocate fuciton the result of split() will have to be removed from the free list
    return curr ;
}

void* allocate_memory(int req_size){
    //pahele baar initialize karne ki jaroorat hai par after that it should take the existing
    //state and proceed 
    if(status_intialize){
        initialize();
        status_intialize = 0;
    }

    metadata* curr =  free_lptr;
    void* ret_val;

    while ((void*)curr <= (void*)&free_space[FREE_SPACE_SIZE-1]){
        // while (curr <= &free_space[FREE_SPACE_SIZE-1]) {  // Mismatched types!
        // void* is a generic pointer that compares the raw address values 

        //void* used to just represent the pointer pointing to some potental location is at that
        //specific address 

        if ((curr->available == TRUE) && (curr->size >= req_size) && (curr->size < (req_size + METADATA_SIZE))) { 
            // internal fragmentation
            curr->available = FALSE; //here we are required to allocate the entire block and the remining bits are  wasted
            //eg: 24 is curr size, metadata is 6 but call is of 20 
            ret_val = (void*)(curr + 1);
            return ret_val;
        } 
        else if ((curr->available == TRUE) && (curr->size >= (req_size + METADATA_SIZE))) {
            
            curr = split(curr, req_size);
            
            ret_val = (void*)(curr + 1);
            printf("memory allocated\n");
            return ret_val;
        } 
        else {
            
            curr = (metadata*)((char*)curr + (METADATA_SIZE + curr->size));
            //typecasted to char* for byte by byte traversal in char array space and then the entire
            //marker is again typecasted to metadata* in order continue the process
        }
        
    }
    printf("No free space available for memory allocation\n");
    return NULL;

}

void merge(metadata* curr){
    metadata* next = curr->next;
    metadata* prev = free_lptr;

    while((prev != curr) &&(prev->next != curr)){
        prev = prev->next;
    } // this repetitive transversal can be modified by including a prev pointer in the structure definition 
    if((prev != NULL) && (prev->available == TRUE)){
        prev->size += (METADATA_SIZE + curr->size);
        curr = prev;
    }

    if(((void*)next <= (void*)&free_space[FREE_SPACE_SIZE-1]) && (next->available == TRUE)){
        curr->size += (METADATA_SIZE + next->size);
    }

}

void de_allocate(void* loc){
    if((loc < (void*)free_space) || (loc >= (void*)(free_space + FREE_SPACE_SIZE))){
        printf("Out of bounds\n");
    }
    else{
        metadata* curr = (metadata*)loc - 1;
        curr->available = TRUE;
        merge(curr);
        printf("Deallocated\n");
    }
}



int main(){
    printf("Metadata Size: %d\n", METADATA_SIZE);
    printf("Heap Size : %d\n", FREE_SPACE_SIZE);
    printf("Start of free_space :\t%p\n", free_space);
    printf("End of free_space :\t%p\n", &free_space[FREE_SPACE_SIZE-1]);

    int a = 10000;

    int* arr2 = (int*)allocate_memory(a);
    printf("%p\n", arr2);
    printf("Available free space is %d\n", 40000-a-METADATA_SIZE);
    

    int* arr3 = (int*)allocate_memory(10000);
    a = a+10000;
    printf("%p\n", arr3);

    printf("Available free space is %d\n", 40000-a-METADATA_SIZE);

    int* arr4 = (int*)allocate_memory(10000);
    printf("%p\n", arr4);

    de_allocate(arr2);
    int* arr5 = (int*)allocate_memory(20500);
    printf("%p\n", arr5);

    return 0;
}



// we need to do something by which when user wants to allocate 3000bytes of memory
// our program should search if such a space is available in the free_list and accordingly allocate it in alloc list
// initially one would have lets say 10000 bytes of one big free memory 
// when a call of 3000 is made , it will divide itself into 



