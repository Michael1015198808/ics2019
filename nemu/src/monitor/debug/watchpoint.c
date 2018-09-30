#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i];
  }
	wp_pool[NR_WP-1].next=NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(void){
	if(free_->next==NULL){
		assert(0);
	}
	if(head==NULL){
		head=free_;
	}
	WP *temp=head;
	while(temp->next!=free_){
		temp=temp->next;
	}
	temp->next=free_;
	free_=free_->next;
	return temp;
}
void free_wp_by_int(int i){
	free_wp(wp_pool+i);
}
void free_wp(WP *wp){
	WP *temp=free_;
	while(temp->next!=NULL){
		temp=temp->next;
	}
	temp->next=wp;
	temp=head;
	if(head==wp){
		head=wp->next;
	}else{
		while(temp->next!=wp){
			temp=temp->next;
		}
	}
	temp->next=wp->next;
	wp->next=NULL;
}
bool check_wp(void){
	bool success=false,trash=true;
	WP *temp=head;
	while(temp!=NULL&&temp!=free_){
		int i=expr(temp->exprr,&trash);
		if(i!=temp->old_value){
			printf("Watchpoint%d :\n%s\n changes from %d to %d\n",temp->NO,temp->exprr,temp->old_value,i);
			temp->old_value=i;
			success=true;
		}
		temp=temp->next;
	}
	return success;
}
void show_wp(void){
		printf("%d\t%d\n",head->NO,free_->NO);
		WP *temp=head;
		while(temp!=NULL&&temp!=free_){
			printf("Watchpoint%d :\n%s=%d\n",temp->NO,temp->exprr,temp->old_value);
			temp=temp->next;
		}
}
