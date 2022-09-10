#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct node
{
    int value[257],binaryvalue[26];
    int noofones;
    int impls;
    char term[26];
    struct node* link;
};
typedef struct node* NODE; 
NODE root,head,rootofimps,current,fin;
int var,min,number=1,columns=2,check=1;
int limit,pitable[100][100],counter=0;
int essential[1000],t=0,no=0,minterms[1000];
char a[26],b[26];       //variable names are stored as alphabets, can be modified to work for more variable


NODE createnode()
{
	NODE new= (NODE)malloc(sizeof(struct node));
	return new;
}

void swappingnodecontent(NODE temp1,NODE temp2)  //swappingnodecontent all the value of two nodes
{
    int x,y,i=0;
    i=var;
    for(i=0;i<var;i++)      //binary notation is exchanged
    {
        y=temp1->binaryvalue[i];
        temp1->binaryvalue[i]=temp2->binaryvalue[i];
        temp2->binaryvalue[i]=y;
    }
    y=temp1->noofones;          //no. of ones is exchanged
    temp1->noofones=temp2->noofones;
    temp2->noofones=y;
    x=temp1->value[0];           //value(minterm) is exchanged
    temp1->value[0]=temp2->value[0];
    temp2->value[0]=x;
}

void implicants(NODE val)       //initializing each term as a prime implicant
{
    NODE temp;
    temp=val;
    while(temp!=NULL)
    {
        temp->impls
=1;
        temp=temp->link;
    }
}

void reorder()          //arranging the minterms in increasing order of magnitude
{
    NODE temp1,temp2;
    temp1=temp2=root;
    while(temp1!=NULL)
    {
        temp2=root;
        while(temp2!=NULL)
        {
            if(temp1->value[0]<temp2->value[0])       //if not in order their values are exchanged with swapping node content function
            {
                swappingnodecontent(temp1,temp2);
            }
            temp2=temp2->link;
        }
        if(temp1->link==NULL)
        {
            limit=temp1->value[0];           //the magnitude of the last minterm is recorded later for prime implicants table
        }
        temp1=temp1->link;
    }
}
void store_minterms()       //array to store all the minterms
{
    int i=0;
    NODE temp;
    temp=root;
    while(temp!=NULL)
    {
        minterms[i]=temp->value[0];
        i++;
        temp=temp->link;
    }
}



void ordering()       //where the minterms are reordered according to the number of ones
{
    int i,count=0,j,k=0;
    NODE temp,next;
    temp=current=root;
    root=next=createnode();
    for(i=0;i<=var;i++)
    {
        temp=current;
        while(temp!=NULL)
        {
            if(temp->noofones==i)       //minterms are reordered according to no. of ones , first 0 ones then 1 ones... and so on
            {
                next->value[0]=temp->value[0];
                k++;
                for(j=0;j<var;j++)
                {
                    next->binaryvalue[j]=temp->binaryvalue[j];
                }
                next->noofones=temp->noofones;
                next=next->link=createnode();
            }
            temp=temp->link;
        }
    }
    minterms[k]=-1;
    next->link=NULL;
}

void disp()     //for displaying the various column with pairings
{
    int i,j=min;
    NODE temp;
    temp=root;
    printf("\n\nColumn #%d\n\n\n",number);          //number tells us which column is being printed
    while(temp->link!=NULL)
    {
        printf("%d\t",temp->value[0]);
        for(i=var-1;i>=0;i--)
        {
            printf("%d",temp->binaryvalue[i]);
        }
        temp=temp->link;
        printf("\n");
    }
    temp->link=NULL;
    number++;
}

void end_loop(NODE ptr)         //reducing the number of nodes in a list with one extra node
{
    NODE temp;
    temp=ptr;
    while(temp->link->link!=NULL)
    {
        temp=temp->link;
    }
    temp->link=NULL;
}
void regrouping()    //grouping based on difference in binary notation
{
    int i,count,k,j,x;
    NODE temp,next,p,imp;
    check=0;
    if(columns==2)      //for second column
    {
        imp=rootofimps=createnode();
        p=head;
    }
    else        //for other columns
    {
        imp=current;
        root=head;
        p=head=createnode();
    }
    temp=root;
    implicants(root);
    printf("\n\nColumn #%d\n\n\n",number);
    while(temp!=NULL)
    {
        next=temp->link;
        while(next!=NULL)
        {
            count=0;
            if(next->noofones-temp->noofones==1)        //if two terms differ in their no. of ones by one
            {
                for(i=0;i<var;i++)
                {
                    if(temp->binaryvalue[i]!=next->binaryvalue[i])
                    {
                        k=i;            //the place in which they differ is noted
                        count++;
                    }
                }
            }
            if(count==1) //checks if the two terms differ by one place in binary notation
            {
                temp->impls
        =0;        //if they do then they are not a prime implicant
                next->impls
        =0;
                check++;
                for(i=0;i<var;i++)
                {
                    p->binaryvalue[i]=temp->binaryvalue[i];         //binary notation is stored
                }
                p->binaryvalue[k]=-1;
                x=0;
                for(j=0;j<columns/2;j++)            //value from first term is stored
                {
                    p->value[x]=temp->value[j];
                    x++;
                }
                for(j=0;j<columns/2;j++)            //value from second term is stored
                {
                    p->value[x]=next->value[j];
                    x++;
                }
                p->noofones=temp->noofones;
                for(j=0;j<columns;j++)      //the pair formed is displayed
                {
                    printf("%d,",p->value[j]);
                }
                printf("\b ");
                printf("\t");
                for(i=var-1;i>=0;i--)
                {
                    if(p->binaryvalue[i]==-1)
                        printf("-");
                    else
                        printf("%d",p->binaryvalue[i]);
                }
                printf("\n");
                p=p->link=createnode();           // one extra node that is to be deleted
            }
            next=next->link;
        }
        temp=temp->link;
    }
    p->link=NULL;
    if(check!=0)
    {
        end_loop(head);     //extra node is deleted
    }
    temp=root;
    while(temp!=NULL)           //for selecting the prime implicants
    {
        if(temp->impls
==1)        // if term is a prime implicant it is stored separately in list with head pointer rootofimps
        {
            i=0;
            for(i=0;i<columns/2;i++)
            {
                imp->value[i]=temp->value[i];
            }
            imp->value[i]=-1;
            for(i=0;i<var;i++)
            {
                imp->binaryvalue[i]=temp->binaryvalue[i];
            }
            imp=imp->link=createnode();
        }
        temp=temp->link;
    }
    current=imp;
    columns=columns*2;
    number++;
}
void implicants_displayfunc()       //displays the implicants
{
    int i=0;
    NODE temp;
    temp=rootofimps;
    printf("\n\nThe prime implicants are:- \n\n");
    while(temp!=NULL)
    {
        i=0;
        i=var-1;
        while(i>=0)     //displays the binary notation
        {
            if(temp->binaryvalue[i]==-1)
            {
                printf("-");
            }
            else
            {
                printf("%d",temp->binaryvalue[i]);
            }
            i--;
        }
        printf("\t\t");
        i=0;
        while(temp->value[i]!=-1)        //displays the minterm pairs
        {
            printf("%d,",temp->value[i]);
            i++;
        }
        printf("\b ");
        temp=temp->link;
        printf("\n\n");
        counter++;
    }
}


void collect()          //reduces the terms that occur more than once to a single
{
    int common=0,i;
    NODE temp1,temp2,temp3;
    temp1=temp2=rootofimps;
    while(temp1!=NULL)
    {
        temp2=temp1->link;
        while(temp2!=NULL)
        {
            common=0;
            for(i=0;i<var;i++)          //if their binary notation is same one will be deleted
            {
                if(temp2->binaryvalue[i]==temp1->binaryvalue[i])
                {
                    common++;
                }
            }
            if(common==var)
            {
                temp3=rootofimps;
                while(temp3->link!=temp2)      //the repeated term is deleted
                {
                    temp3=temp3->link;
                }
                temp3->link=temp2->link;
                temp2=temp3;
            }
            temp2=temp2->link;
        }
        temp1=temp1->link;
    }
}

void variables()            //stores variables(alphabets)
{
    int i;
    for(i=0;i<26;i++)
    {
        a[i]=65+i;      //variables
        b[i]=97+i;      //their compliments
    }
}

void replacer()          //it converts the binary notation of each term to variables
{
    int i,j;
    NODE temp;
    temp=fin;
    while(temp!=NULL)
    {
        j=0;
        for(i=0;i<var;i++)
        {
            if(temp->binaryvalue[i]==0)
            {
                temp->term[j]=b[i];
                j++;
            }
            if(temp->binaryvalue[i]==1)
            {
                temp->term[j]=a[i];
                j++;
            }
        }
        temp=temp->link;
    }
}

void func()         //displays the minimized function in SOP form
{
    NODE temp;
    temp=fin;
    printf("\n\nThe minimized function is :- ");
    while(temp!=NULL)
    {
        printf("%s",temp->term);
        if(temp->link!=NULL)
        {
            printf(" + ");
        }
        temp=temp->link;
    }
    printf("\n\n");
}

void pi_table()         //function for creating prime implicants table as well as selecting essential prime implicants
{
    NODE temp;
    int i,j,k,m,n,x,y,count=0,count2=0,a=0;
    for(i=0;i<counter;i++)
    {
        for(j=0;j<=limit;j++)
        {
            pitable[i][j]=0;           //0 or - is placed in all places of a table
        }
    }
    i=0;
    j=0;
    k=0;
    temp=rootofimps;
    while(temp!=NULL)
    {
        k=0;
        while(temp->value[k]!=-1)
        {
            pitable[i][temp->value[k]]=1;  // 1 or X is placed for the column with same index as that of the number in the pair
            k++;
        }
        i++;
        temp=temp->link;
    }
    printf("\n\n\t\t\tPrime Implicants Table\n\n\n");
    temp=rootofimps;
    i=0;
    printf(" ");
    while(minterms[i]!=-1)
    {
        printf("%d\t",minterms[i]);         //the minterms are displayed in row
        i++;
    }
    printf("\n\n");
    for(i=0;i<counter;i++)          //X and - are placed for the terms with corresponding minterm values
    {
        printf(" ");
        a=0;
        for(j=0;j<=limit;j++)
        {
            if(j==minterms[a])
            {
                if(pitable[i][j]==0)
                {
                    printf("-");
                }
                if(pitable[i][j]==1)
                {
                    printf("X");
                }
                printf("\t");
                a++;
            }
        }
        y=0;
        while(temp->value[y]!=-1)        //prints the minterm pair
        {
            printf("%d,",temp->value[y]);
            y++;
        }
        printf("\b ");
        temp=temp->link;
        printf("\n\n");
    }
    printf("\n\n");
    for(i=0;i<counter;i++)      //for finding essential prime implicants
    {
        for(j=0;j<=limit;j++)
        {
            count=0;
            if(pitable[i][j]==1)
            {
                y=j;
                x=i;
                for(k=0;k<counter;k++)
                {
                    if(pitable[k][j]==1)       //checks if there is only one X in a column
                    {
                        count++;
                    }
                }
                if(count==1)  //places - in place of X in every column of the table whose one row contains only one X in a column
                {
                    essential[t]=x;
                    t++;
                    for(n=0;n<=limit;n++)
                    {
                        if(pitable[i][n]==1)
                        {
                            for(m=0;m<counter;m++)
                            {
                                pitable[m][n]=0;
                            }
                        }
                    }
                }
            }
        }
    }
    essential[t]=-1;
    i=0;
}
void other_implicants()     //after finding the essential prime implicants other terms necessary are marked
{
    no=0;           //to check if any term is found in each iteration
    int count1=0,count2=0;
    int i,j;
    for(i=0;i<counter;i++)
    {
        count1=0;
        for(j=0;j<=limit;j++)
        {
            if(pitable[i][j]==1)       //no. of X's or 1's are calculated
            {
                no++;
                count1++;
            }
        }
        if(count1>count2)       //to find the term with maximum X's in a row
        {
            essential[t]=i;
            count2=count1;
        }
    }
    for(j=0;j<=limit;j++)           //removing the X's in the row as well a those X's which are in same column
    {
        if(pitable[essential[t]][j]==1)
        {
            for(i=0;i<counter;i++)
            {
                pitable[i][j]=0;
            }
        }
    }
    t++;
    essential[t]=-1;
    if(no>0)            //if one or more terms is found the function is called again otherwise not
    {
        other_implicants();
    }
}

void lastres()          //in this function all the terms in the minimized expression are stored in a linked list
{
    int i=0,j,c=0,x;
    NODE temp,ptr;
    fin=temp=createnode();
    while(essential[i]!=-1)
    {
        ptr=rootofimps;
        x=essential[i];
        for(j=0;j<x;j++)        //so that pointer points to the node whose index was stored in array named essential
        {
            ptr=ptr->link;
        }
        j=0;
        while(ptr->value[j]!=-1)         // the value of the node is stored
        {  
            temp->value[j]=ptr->value[j];
            j++;
        }
        temp->value[j]=-1;
        for(j=0;j<var;j++)          //the binary code is stored
        {
            temp->binaryvalue[j]=ptr->binaryvalue[j];
        }
        temp=temp->link=createnode();
        i++;
        c++;
    }
    temp->link=NULL;
}

int main()
{
    int i,j,k,x;
    NODE temp;
    printf("\nEnter the number of variables used\n");       //no. of variables and minterms are recorded
    scanf("%d",&var);
    printf("\nEnter the number of minterms (i.e values in decimal whose o/p is 1)\n");
    scanf("%d",&min);
    i=min-1;
    root=temp=createnode();
    printf("\nEnter the decimal values of minterms\n\n");
    scanf("%d",&temp->value[0]);                     //first minterm is stored
    j=temp->value[0];
    temp->noofones=0;
    x=var;
    k=0;
    while(x--)      //converts minterm to binary value notation
    {
        if(j%2==1)
        {
            temp->binaryvalue[k]=1;
            temp->noofones++;
        }
        else
        {
            temp->binaryvalue[k]=0;
        }
        j=j/2;
        k++;
    }
    while(i--)      //rest of the minterms are stored
    {
        temp=temp->link=createnode();
        scanf("%d",&temp->value[0]);
        j=temp->value[0];
        temp->noofones=0;
        x=var;
        k=0;
        while(x--)
        {
            if(j%2==1)          //converts the minterms to binary notation
            {
                temp->binaryvalue[k]=1;
                temp->noofones++;       //the number of ones in binary notation
            }
            else
            {
                temp->binaryvalue[k]=0;
            }
            j=j/2;
            k++;
        }
    }
    temp->link=NULL;
    reorder();      //various functions are called according to their needs
    store_minterms();
    ordering();
    disp();
    end_loop(root);
    head=createnode();
    while(check>0)
    {
        regrouping();
    }
    current->link=NULL;           //storing null value in link field of list storing prime implicants
    printf("No pairs formed hence no further calculation required\n\n");
    end_loop(rootofimps);
    collect();
    implicants_displayfunc();
    variables();
    pi_table();
    other_implicants();
    lastres();
    end_loop(fin);
    replacer();
    func();
    return 0;
}
