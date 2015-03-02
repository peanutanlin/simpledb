#include<stdio.h>
#include<stdlib.h>
int count;
typedef struct COMPONENT
{
	int id;
	char name[10];
	char details[20];
	int num;
	double eachvalue;
	double tvalue;
	struct COMPONENT *next;
}component;

void printthis(FILE *fp,component *pthis);
component *myfix(component *head,int fid);
component *olddata(FILE *fp);
component *seek(component *head,FILE *fp,int sid,int eid);

int main(void)
{
	component *newcomponent=(component *)malloc(sizeof(component));
	component *current,*head,*end;;
	char ch2='0';
	char ch3='0';
	//fpos_t *pos=(fpos_t *)malloc(sizeof(fpos_t));
	while(1)
	{
		FILE *fp=fopen("mainfile.txt","a+");
		if(fp==NULL)
		{
			printf("error!\n");
			exit(0);
		}
		else
		{
			printf("the old data is:\n");
			printf("------------------------------------------------------------------\n");
			head=olddata(fp);
			end=seek(head,stdout,0,0);
		}
		if(count==0)
		{	
			//printf("do you want to create a new file?y\\n:");
			printthis(fp,NULL);
			count++;
		}
		printf("------------------------------------------------------------------\n");
		printf("please select option:\n");
		printf("1)new  (press  <n> to add).\n");
		printf("2)seek (press  <s> to seek).\n");
		printf("3)fix  (press  <f> to fix).\n");
		printf("4)del  (press  <d> to delete).\n");
		printf("5)press other key to quit.\n");
		fflush(stdin);
		scanf("%c",&ch2);
		switch(ch2)
		{
			case 'n':
			{
				if(end!=NULL)	
					end->next=newcomponent;
				else
					head=newcomponent;
				newcomponent->id=count;
				printf("please enter the name:");
				scanf("%s",newcomponent->name);
				printf("please enter the details:");
				scanf("%s",newcomponent->details);
				printf("please enter the num:");
				scanf("%d",&(newcomponent->num));
				printf("please enter the eachvalue:");
				scanf("%lf",&(newcomponent->eachvalue));
				newcomponent->tvalue=(newcomponent->num)*(newcomponent->eachvalue);
				printf("do you want to save?y\\n:");
				newcomponent->next=NULL;
				fflush(stdin);
				if(getchar()=='y')
				{
					//fsetpos(fp,pos);
					printthis(fp,newcomponent);
					printf("do you want to show the whole or the new add?w or a:");
					fflush(stdin);
					char option=getchar();
					if(option=='w')
					{
						printthis(stdout,NULL);
						end=seek(head,stdout,0,0);
					}
					if(option=='a')
					{
						printthis(stdout,NULL);
						end=seek(newcomponent,stdout,0,0);
					}
				}
			}break;
			case 's':
			{
				int sid=0,eid=0;
				scanf("%d,%d",&sid,&eid);
				printthis(stdout,NULL);
				seek(head,stdout,sid,eid);
			}break;
			case 'f':
			{
				int fid=0;
				char fname[10];
				printf("please enter the id you want to fit:");
				scanf("%d",&fid);
				component *pfix=myfix(head,fid);
				if(pfix!=NULL)
				{
					printthis(stdout,NULL);
					printthis(stdout,pfix);
					printf("please enter the new name:");
					scanf("%s",pfix->name);
					fclose(fp);
					fp=fopen("mainfile.txt","w+");
					printthis(fp,NULL);
					current=head;
					while(current!=NULL)
					{
						printthis(fp,current);
						current=current->next;
					}
					printthis(stdout,NULL);
					printthis(stdout,pfix);
				}
				else
					printf("don't exist.\n");
			}break;
			case 'd':
			{	
				int did=0;
				printf("please enter the id you want to delete:");
				scanf("%d",&did);
				component *mdel=myfix(head,did);
				if(mdel!=NULL)
				{
					current=mdel->next;
					while(current!=NULL)
					{
						current->id=current->id-1;
						current=current->next;
					}
					printthis(stdout,NULL);
					printthis(stdout,mdel);
					fclose(fp);
					fp=fopen("mainfile.txt","w+");
					printthis(fp,NULL);
					current=head;
					while(current!=NULL)
					{
						if(current==mdel)
						{
							current=current->next;
							continue;
						}
						else
						{
							printthis(fp,current);
							current=current->next;
						}
					}
					printf("\nthis record has been deleted!\n");
				}
				else
					printf("don't exist.\n");
			}break;
		}
		fclose(fp);
		current=head;
		while(current!=NULL)
		{
			free(current);
			current=current->next;
		}
		printf("continue?(q to quit):");
		fflush(stdin);
		ch3=getchar();
		if(ch3=='q')
			break;
	}
	free(newcomponent);
	//free(pos);
	getchar();
	return 0;
}//main end

component* olddata(FILE *fp)
{
	count=0;
	component *head=NULL;
	component *prev,*current;
	char *buffer=(char *)malloc(1024*sizeof(char));
	while(fgets(buffer,1023*sizeof(char),fp)!=NULL)
	{
		count++;
		if(count==1)
		{
			printf("%s",buffer);
			printf("------------------------------------------------------------------\n");
		}
		if(count>1)
		{
			current=(component *)malloc(sizeof(component));
			if(head==NULL)
				head=current;
			else
				prev->next=current;
			current->next=NULL;
			sscanf(buffer,"%d %s %s %d %lf %lf",
				&(current->id),
				current->name,
				current->details,
				&(current->num),
				&(current->eachvalue),
				&(current->tvalue));
			prev=current;
		}	
	}
	return head;
}//将原始数据载入内存，链表实现
component *seek(component *head,FILE *fp,int sid,int eid)
{
	component *end;
	component *current=head;
	if(fp==NULL)
		fp=stdout;
	if(sid==0 && eid==0)
	{
		while(current!=NULL)
		{
			if(fp!=NULL)
				printthis(fp,current);
			end=current;
			current=current->next;
		}
		return end;
	}
	if(sid!=0 && eid!=0)
	{
		while(current!=NULL)
		{
			if(current->id>=sid && current->id<=eid)
				printthis(fp,current);
			current=current->next;
		}
		return NULL;
	}
}//查询

component *myfix(component *head,int fid)
{
	component *current=head;
	while(current!=NULL)
	{
		if(current->id==fid)
		{
			break;
		}
		else
			current=current->next;
	}
	return current;
}//删除、修改

void printthis(FILE *fp,component *pthis)
{
	if(pthis!=NULL)
	{
		fprintf(fp,"%-6d%-10s%-20s%-10d%-10.2lf%-15.2lf\n",
				pthis->id,
				pthis->name,
				pthis->details,
				pthis->num,
				pthis->eachvalue,
				pthis->tvalue);
	}
	else
	{
		fprintf(fp,"%-6s%-10s%-20s%-10s%-10s%-15s\n",
						"ID",
						"Name",
						"Details",
						"Number",
						"EachValue",
						"TotalValue");
	}
}

