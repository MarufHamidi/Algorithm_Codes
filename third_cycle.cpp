#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdlib>
#define INFINITY 65000
#define MAX 100
#define YES 1
#define NO 0

using namespace std;

fstream fin("third_cycle_input3.txt",ios::in);
int total_vertex=0,total_edges=0,total_sets=0;

struct node{
	char vertex[MAX];
	struct node *next;
    struct node *prev;
	struct node *connected[MAX];
	int weight[MAX];
	int total_adj;
	int key;
	int flag;
	int flag2;
	struct node *prnt;
};

node *head=NULL;

struct edge{
    node *start;
    node *end;
    int cost;
    int flag;
}kruskal[MAX],ktree[MAX];

class stack{
    node *element[MAX];
    int tos;
public:
    int size;
    stack(){
        size=0;
        tos=-1;
    }

    void push(node *e){
        tos++;
        element[tos]=e;
        size++;
    }

    node *pop(){
        size--;
        tos--;
        return element[tos+1];
    }

    bool is_empty(){return size==0;}
};

node *component[MAX][MAX];
int si=0;
int ct[MAX];

void makeset(node *x)
{
    component[si][0]=x;
    ct[si]=1;
    si++;
    total_sets++;
}

int findset(node *x)
{
    int i,j;
    for(i=0;i<total_sets;i++){
        for(j=0;j<ct[i];j++){
            if(component[i][j]==x)return i;
        }
    }
}

void unionset(node *x,node *y)
{
    int one=findset(x);
    int two=findset(y);
    int i,j;
    for(i=ct[one],j=0;j<ct[two];i++,j++){
        component[one][i]=component[two][j];
    }
    ct[one]=ct[one]+ct[two];
}

//build the adjacency linked list from one line of input string
void build_list(char v[],int t)
{
    node *temp,*new_node;
    temp=head;
    new_node = new node;
    strcpy(new_node->vertex,v);
    new_node->next=NULL;
    new_node->total_adj=t;
    //new_node->key=65000;
    if(temp==NULL){
        head=new_node;
        new_node->prev=NULL;
    }

    else{
        while(temp->next){
            temp=temp->next;
        }
        temp->next=new_node;
        new_node->prev=temp;
    }
    //cout<<new_node->vertex<<" ";
}

//returns the address of the node in the list
node *search(char c[])
{
    node *temp=head;
    while(temp){
        if(strcmp(temp->vertex,c)==0)return temp;
        temp=temp->next;
    }
}

//build the graph from the given input file
void build_graph()
{
    int c,i,j,t,tot_con[MAX],dist[MAX][MAX];
    char str[10000],temp[MAX],tn[MAX][MAX],con[100][100][MAX];
    if(!fin){
        cout<<"Error opening file."<<endl;
        return;
    }


    //read from the input file
    i=0;
    while(!fin.eof()){
        c=0;
        fin>>str;
        t=0;
        while(1){
            if(str[c]==':')break;
            temp[t]=str[c];
            t++;
            c++;
        }
        temp[t]='\0';
        strcpy(tn[i],temp);

        j=0;
        while(1){
            c++;
            if(c>=strlen(str))break;
            c++;
            t=0;
            while(1){
                if(str[c]==',')break;
                temp[t]=str[c];
                t++;
                c++;
            }
            temp[t]='\0';
            strcpy(con[i][j],temp);
            c++;

            t=0;
            while(1){
                if(str[c]=='>')break;
                temp[t]=str[c];
                t++;
                c++;
            }
            temp[t]='\0';
            dist[i][j]=atoi(temp);
            c++;
            j++;
        }
        tot_con[i]=j;
        i++;
    }

    total_vertex=i;
    fin.close();

    for(i=0;i<total_vertex;i++)build_list(tn[i],tot_con[i]);

    //build the connected graph
    for(i=0;i<total_vertex;i++){
        node *ver=search(tn[i]);
        for(j=0;j<ver->total_adj;j++){
            ver->connected[j]=search(con[i][j]);
            ver->weight[j]=dist[i][j];
        }
    }

    cout<<"Graph has been built."<<endl;
}

//find the shortest path applying dijkstra's algorithm
node *fm1,*fm2;
void shortest_path(char src[],char dest[])
{
    node *min,*temp,*dijkstra[MAX];
    stack stck;
    temp=head;
    node *start=search(src);
    node *finish=search(dest);
    int i,j,tk,minkey;
    for(i=0;i<total_vertex;i++){
        dijkstra[i]=temp;
        temp=temp->next;
        dijkstra[i]->key=INFINITY;
        dijkstra[i]->flag=NO;
    }

    temp=start;
    start->key=0;
    while(1){
        if(finish->flag==YES){
            temp=finish;
            while(1){
                stck.push(temp);
                if(temp==start)break;
                temp=temp->prnt;
            }
            cout<<"The sortest path is: "<<start->vertex;
            temp=stck.pop();
            while(!stck.is_empty()){
                temp=stck.pop();
                cout<<" -> "<<temp->vertex;
            }
            cout<<endl<<"The shortest distance from "<<start->vertex<<" to "<<finish->vertex<<" is "<<finish->key<<endl;
            break;
        }

        tk=temp->key;
        for(i=0;i<temp->total_adj;i++){
            if(temp->connected[i]->key>(tk+temp->weight[i])){
                temp->connected[i]->key=tk+temp->weight[i];
                temp->connected[i]->prnt=temp;
            }
        }

        temp->flag=YES;
        minkey=temp->key;
        min=temp;
        min->key=INFINITY;
        for(j=0;j<total_vertex;j++){
            if(min->key>dijkstra[j]->key && dijkstra[j]->flag==NO)min=dijkstra[j];
        }

        temp->key=minkey;
        temp=min;

        if(min==fm2){
            cout<<"There is no path from "<<start->vertex<<" to "<<finish->vertex<<". \n";
            break;
        }

        if(min==fm1)fm2=min;
        if(fm1!=min)fm1=min;
    }
}

//minium spanning tree
void mst_kruskal()
{
    node *temp=head;
    edge *min,test;
    int i,j,min_cost,k=0;

    //sorting the edges in the ascending order of their costs
    for(i=0;i<total_edges-1;i++){
        for(j=i+1;j<total_edges;j++){
            if(kruskal[i].cost>kruskal[j].cost){
                test=kruskal[i];
                kruskal[i]=kruskal[j];
                kruskal[j]=test;
            }
        }
    }

    for(i=0;i<total_edges;i++){
        kruskal[i].start->flag=NO;
        kruskal[i].end->flag=NO;
        kruskal[i].start->flag2=NO;
        kruskal[i].end->flag2=NO;
        cout<<kruskal[i].start->vertex<<kruskal[i].end->vertex<<" "<<kruskal[i].cost<<endl;
    }

    temp=head;
    for(i=0;i<total_vertex;i++){
        makeset(temp);
        temp=temp->next;
    }

    min_cost=0;
    for(i=0,k=0;i<total_edges;i++){
        if(findset(kruskal[i].start)==findset(kruskal[i].end)){
            i++;
        }

        else{
            ktree[k]=kruskal[i];
            kruskal[i].start->flag=YES;
            kruskal[i].end->flag=YES;
            min_cost=min_cost+ktree[k].cost;
            k++;
            cout<<kruskal[i].start->vertex<<kruskal[i].end->vertex<<" ";
            unionset(kruskal[i].start,kruskal[i].end);
        }
    }

    cout<<endl<<min_cost;
}

//print the vertices
void print_vertices()
{
    int i,j;
    node *temp=head;
    cout<<"The number of vertices in the graph is: "<<total_vertex<<". They are:"<<endl;
    for(i=0;i<total_vertex;i++){
        cout<<temp->vertex<<" ";
        temp=temp->next;
    }
    cout<<endl<<endl;
}

//print all the edges
void print_edges()
{
    int i,j,k;
    node *temp=head;
    for(i=0,k=0;i<total_vertex;i++){
        for(j=0;j<temp->total_adj;j++){
            if(temp->connected[j]->flag==NO){           //for directed graph comment this line
                kruskal[k].start=temp;
                kruskal[k].end=temp->connected[j];
                kruskal[k].cost=temp->weight[j];
                kruskal[k].flag=NO;
                kruskal[k].start->flag=NO;
                kruskal[k].end->flag=NO;
                k++;
            }                                           //for directed graph comment this line
        }
        temp->flag=YES;
        temp=temp->next;
    }

    total_edges=k;
    cout<<"There are total "<<total_edges<<" edges. They are: "<<endl;
    for(i=0;i<total_edges;i++){
        cout<<kruskal[i].start->vertex<<kruskal[i].end->vertex<<" <"<<kruskal[i].cost<<">\n";
    }
    cout<<endl;
}

//print graph
void print_graph()
{
    int i;
    node *temp=head;
    //cout<<"The graph represented in adjacency list: \n";
    while(temp){
        cout<<temp->vertex<<":";
        for(i=0;i<temp->total_adj;i++){
            cout<<"<"<<temp->connected[i]->vertex<<","<<temp->weight[i]<<">";
            if(i<temp->total_adj-1)cout<<",";
        }
        cout<<endl;
        temp=temp->next;
    }
    cout<<endl;
}

void wait()
{
    cout<<"Thank You.\nPress ENTER key to exit.";
    int n;
    getchar();
}

int main(void)
{
    char src[MAX],dest[MAX];
    build_graph();
    print_graph();
    print_vertices();
    print_edges();
    /*cout<<"Enter starting: ";
    cin>>src;
    cout<<"Enter destination: ";
    cin>>dest;
    shortest_path(src,dest);*/
    mst_kruskal();
    wait();
	return 0;
}
