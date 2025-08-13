#include <stdio.h>
#include <stdlib.h>

struct node{
    int start;
    int size;
    struct node*next;
};

struct process{
    int start;
    int size;
};

struct process u_malloc(struct node**freelist, int s){
    struct node* current = *freelist;
    struct node*prev=NULL;
    struct process p;
    p.size=s;
    while (current!=NULL){
        if (current->size==s){
            if (prev==NULL){
                p.start= current->start;
                *freelist=current->next;
                free(current);
                return p;
            }else{
                prev->next=current->next;
                p.start=current->start;
                free(current);
                return p;
            }
        }
        else if(current->size>s){
            p.start = current->start;
            current->size-=s;
            current->start+=s;
            return p;
        }
        prev=current;
        current=current->next;
    }
    
    printf("coudn't allcoate memory memory full \n");
    p.start=-1;
    p.size=0;
    return p;
}


void pintl(struct node *list){
    struct node *current =list;
    if (list==NULL){
        printf("Fully allocated \n");
        return;
    }   
    while (current!=NULL){
        printf("start %d,size %d\n", current->start, current->size);
        current=current->next;
    }
    
}

void coalesce(struct node**freelist){

}

void u_free(struct node **freelist, struct process p){
    struct node* current = *freelist;
    struct node* prev = NULL;
    struct node*new = (struct node*)malloc(sizeof(struct node));
    new->start=p.start;
    new->size=p.size;
    if (current==NULL){
        *freelist=new;
        return;
    }
    while (current!=NULL && current->start<p.start){
        prev=current;
        current=current->next;
    }
    if (prev==NULL){
        if(p.start+p.size==current->start){
            current->size+=p.size;
            current->start-=p.size;
            return;
        }
        else{
            new->next=current;
            *freelist=new;
            return;
        }
    }
    if(p.start+p.size==current->start){
        current->size+=p.size;
        current->start-=p.size;
        return;
    }
    prev->next=new;
    new->next=current;
}

int main(){
    int size=100;
    int memory[size];
    struct node*freelist=(struct node*)malloc(sizeof(struct node));
    freelist->start=0;
    freelist->next=NULL;
    freelist->size=size;
    pintl(freelist);
    struct process p1 =u_malloc(&freelist, 20);
    pintl(freelist);
    struct process p2=u_malloc(&freelist, 30);
    pintl(freelist);
    struct process p3 =u_malloc(&freelist, 50);
    pintl(freelist);
    printf("-------------------\n");
    u_free(&freelist,p3);
    pintl(freelist);
    u_free(&freelist,p1);
    pintl(freelist);
    printf("sfsf");
    u_free(&freelist,p2);
    pintl(freelist);
    return 0;
}