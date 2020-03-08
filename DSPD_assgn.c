#include<stdio.h>
#include<stdlib.h>
typedef struct node
{
	int row;
	int col;
	int cost;	//0 for blank and 1 for filled(boolean)
	struct node *next;
}node;

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

void display(node *first)
{
	while(first!=NULL)
	{
		printf("Row=%d\tCol=%d\tCost=%d\n",first->row,first->col,first->cost);
		first=first->next;
	}
}

main()
{
	node *sparse;
	sparse=create_list();
	display(sparse);
}
