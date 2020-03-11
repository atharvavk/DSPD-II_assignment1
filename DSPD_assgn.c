#include<stdio.h>
#include<stdlib.h>



typedef struct node
{
	int row;
	int col;
	int cost;	//0 for blank and 1 for filled(boolean)
	struct node *next;
}node;

typedef struct priority_q
{
	int row;
	int col;
	int cost;
	int came_from[2];
	struct priority_q *next;
}pq;

node *sparse;
pq *deleted;

node* create_node(int row,int col,int cost)
{
	node *new;
	new=(node*)malloc(sizeof(node));
	new->row=row;
	new->col=col;
	new->cost=cost;
	new->next=NULL;
	return new;
}

node* create_list()
{
	FILE *f;
	f=fopen("maze.txt","r");
	int i=0,j=0;
	char c;
	c=fgetc(f);
	node *first;
	first=NULL;
	node *current;
	current=NULL;
	node *new;
	new=NULL;
	while(c!=EOF)
	{
		if(first==NULL)
		{
			if(c=='#')
			{
				j++;
			}
			else if(c=='f')
			{
				first=create_node(i,j,1);
				current=first;
				j++;
			}
			else if(c==' ')
			{
				first=create_node(i,j,0);
				current=first;
				j++;
			}
			else if(c=='\n')
			{
				i++;
				j=0;
			}
		}
		else
		{
			if(c=='#')
			{
				j++;
			}
			else if(c=='f')
			{
				new=create_node(i,j,1);
				current->next=new;
				current=current->next;
				j++;
			}
			else if(c==' ')
			{
				new=create_node(i,j,0);
				current->next=new;
				current=current->next;
				j++;
			}
			else if(c=='\n')
			{
				i++;
				j=0;
			}
		}
		c=fgetc(f);
	}
	return first;
}

void display()
{
	node *first;
	first=sparse;
	while(first!=NULL)
	{
		printf("Row=%d\tCol=%d\tCost=%d\n",first->row,first->col,first->cost);
		first=first->next;
	}
}

pq* insert(pq *adding , int row , int col ,int cost, int came_from_row, int came_from_col )
{
	pq *new_node;
	new_node=(pq *)malloc(sizeof(pq));
//	printf("malloc worked successfully\n");
	new_node->row=row;
	new_node->col=col;
	new_node->cost=cost;
	new_node->next=NULL;
	new_node->came_from[0]=came_from_row;
	new_node->came_from[1]=came_from_col;
	pq *current;
	current=adding;
	if(adding==NULL)
	{
		return new_node;
	}
	else if(new_node->cost < adding->cost)
	{
		new_node->next=adding;
		return new_node;
	}
	else
	{
//		printf("here2\n");
		while(current->next!=NULL && current->next->cost < new_node->cost )
		{
			current=current->next;
		}
//		printf("here3\n");
		pq *temp;
		temp=current->next;
		current->next=new_node;
		new_node->next=temp;
		return adding;
	}
}

int isValid(int row , int col)
{
	node *current;
	current = sparse;
	int flag1=0 , flag2=0;
	while(current != NULL && flag1==0)
	{
		if(current->row==row  && current->col==col)
		{
			flag1=1;
		}
		current=current->next;
	}
	pq *temp;
	temp=deleted;
	while(temp!=NULL && flag2==0)
	{
		if(temp->row == row && temp->col==col)
		{
			flag2=1;
		}
		else
		{
			temp=temp->next;
		}
	}
	if(flag1==1 && flag2==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

pq* exists_in_adding(pq *adding,int row ,int col)
{
	pq *current;
	current=adding;
	int flag=0;
	while(current!=NULL && flag==0)
	{
		if(current->row==row && current->col==col)
		{
			flag=1;
		}
		else
		{
			current=current->next;
		}
	}
	return current;
}

pq* update(pq *adding, int row , int col, int came_from_row , int came_from_col , int cost )
{
	if(isValid(row,col))
	{
		pq *address_in_pq;
		address_in_pq=exists_in_adding(adding,row,col);
		if(address_in_pq==NULL)
		{
//			printf("here1\n");
			adding=insert(adding,row,col,cost,came_from_row,came_from_col);
//			printf("insert worked successfully\n");
		}
		else
		{
			if(address_in_pq->cost > cost)
			{
				address_in_pq->cost=cost;
				address_in_pq->came_from[0]=came_from_row;
				address_in_pq->came_from[1]=came_from_col;
			}
		}
	}
	return adding;
}

int getCost(int row,int col)
{
	node *current;
	current=sparse;
	int cost=-1;
	while(current!=NULL && cost==-1)
	{
		if(current->row==row && current->col==col)
		{
			cost=current->cost;
		}
		current=current->next;
	}
	return cost;
}

pq* delete(pq *adding,pq *current)
{
	pq *check;
	check=adding;
	if(adding==current)
	{
		adding=adding->next;
		current->next=deleted;
		deleted=current;
	}
	else
	{
		while(check->next!=current)
		{
			check=check->next;
		}
		check->next=current->next;
		current->next=deleted;
		deleted=current;
	}
	return adding;
}

void display_deleted()
{
	pq *temp;
	temp=deleted;
	while(temp!=NULL)
	{
		printf("Row=%d\tcol=%d\tcame form=(%d,%d)\tcost=%d\n",temp->row,temp->col,temp->came_from[0],temp->came_from[1],temp->cost );
		temp=temp->next;
	}
}

void display_adding(pq *adding)
{
	pq *temp;
	temp=adding;
	while(temp!=NULL)
	{
		printf("Row=%d\tcol=%d\tcame form=(%d,%d)\tcost=%d\n",temp->row,temp->col,temp->came_from[0],temp->came_from[1],temp->cost );
		temp=temp->next;
	}
}

main()
{
	sparse=create_list();
	display();
	int start[2];
	int end[2];
	printf("please enter starting address(row,col)\n");
	scanf("%d%d",start,start+1);
	printf("please enter reaching address(row,col)\n");
	scanf("%d%d",end,end+1);
	//assuming starting address and reaching address both exist in sparse matrix
	pq *adding;
	deleted=NULL;
	adding=NULL;
	adding=update(adding,start[0],start[1],-1,-1,0);
	display_adding(adding);
	pq *current;
	int row,col,cost;
	while(adding->row!=end[0] || adding->col!=end[1])
	{
		current=adding;
		row=current->row;
		col=current->col;
		cost=current->cost;
		if(current->came_from[0]!=row+1 || current->came_from[1]!=col){		
//			printf("hiiii\t");
			adding=update(adding,row+1,col,row,col,cost+getCost(row+1,col));}
		if(current->came_from[0]!=row-1 || current->came_from[1]!=col)
			adding=update(adding,row-1,col,row,col,cost+getCost(row-1,col));
//		display_adding(adding);
		if(current->came_from[0]!=row || current->came_from[1]!=col+1)
			adding=update(adding,row,col+1,row,col,cost+getCost(row,col+1));
		if(current->came_from[0]!=row || current->came_from[1]!=col-1)
			adding=update(adding,row,col-1,row,col,cost+getCost(row,col-1));
		adding=delete(adding,current);
		
	}
	printf("displaying deleted\n\n");
	display_deleted();
	printf("dipslaying addin priority queue\n\n");
	display_adding(adding);
}
