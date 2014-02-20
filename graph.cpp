#include<iostream>
#include<cstring>
#include<fstream>
#define WHITE 0
#define GREY 1
#define BLACK 2

using namespace std;

fstream fin("graph input.txt");
int total_ver;

class Queue{
    char ch[80];
    int size;
    int head;
    int tail;
public:
    Queue(){
        size = 80;
        head = 0;
        tail = -1;
    }
    void enqueue(char c){
        if(tail==size){
            cout<<"Queue is full."<<endl;
            return;
        }
        tail++;
        ch[tail] = c;
    }
    int dequeue(){
        if(tail<head){
            cout<<"Queue is empty.";
            return -999;
        }
        head++;
        return ch[head-1];
    }
    bool is_empty(){return tail<head;}
};

Queue Q;

class Stack{
    char ch[80];
    int tos;
    int size;
public:
    Stack(){
        tos=-1;
        size=80;
    }
    void push(char c){
        tos++;
        ch[tos]=c;
    }
    char pop(){
        tos--;
        return ch[tos+1];
    }
    bool is_empty(){return tos<0;}
};

Stack stk;

struct Vertex{
    char key;
    int color;
    int total_adj;
    int dtime;
    int ftime;
    Vertex *adj[30];
    Vertex *prev;
    Vertex *next;
};

Vertex *head=NULL;
Vertex *Graph[100];
int v=0;
int time;

void insert_graph(char ch)
{
    Vertex *new_vertex=new Vertex;
    new_vertex->key=ch;
    new_vertex->color=WHITE;
    if(head==NULL){
        head=new_vertex;
        new_vertex->prev=new_vertex;
        new_vertex->next=head;
        Graph[v]=new_vertex;
        v++;
    }
    else{
        Vertex *temp = head;
        while(temp){
            if(temp->next==head){
                temp->next = new_vertex;
                new_vertex->prev = temp;
                new_vertex->next=head;
                Graph[v]=new_vertex;
                v++;
                return;
            }
            temp = temp->next;
        }
    }
}

Vertex *search(char ch)
{
    Vertex *temp=head;
    while(temp){
        if(temp->key==ch)return temp;
        temp=temp->next;
    }
}

void build_adj(char *str)
{
    int len = strlen(str);
    Vertex *temp[strlen(str)];
    for(int i=0;i<len;i++)temp[i]=search(str[i]);
    for(int j=1;j<len;j++)temp[0]->adj[j-1]=temp[j];
    temp[0]->total_adj=strlen(str)-1;
}

void build_graph()
{
    fin>>total_ver;
    cout<<"Total vertices are "<<total_ver<<endl;
    char str[total_ver];
    char data[total_ver][total_ver];
    for(int i=0;fin>>str;i++){
        cout<<str<<endl;
        strcpy(data[i],str);
    }
    for(int j=0;j<total_ver;j++)insert_graph((data[j][0]));
    for(int k=0;k<total_ver;k++)build_adj(data[k]);
    cout<<"Graph has been built."<<endl;
}

void bfs()
{
    char ch;
    cout<<"Enter the starting point: ";
    cin>>ch;
    Vertex *start=search(ch);
    start->color=GREY;
    start=search(ch);
    Q.enqueue(start->key);
    cout<<"\n\nThe BFS travaersal is :  ";
    while(!Q.is_empty()){
        ch=Q.dequeue();
        Vertex *temp=search(ch);
        for(int i=0;i<temp->total_adj;i++){
            if(temp->adj[i]->color==WHITE){
                temp->adj[i]->color=GREY;
                Q.enqueue(temp->adj[i]->key);
            }
        }
        temp->color=BLACK;
        cout<<temp->key;
        if(!Q.is_empty())cout<<"->";
    }
    cout<<endl<<endl;
}

void dfs_visit(Vertex *u)
{
    time++;
    u->color=GREY;
    u->dtime=time;
    cout<<u->key<<" ";
    for(int i=0;i<u->total_adj;i++){
        if(u->adj[i]->color==WHITE){
            dfs_visit(u->adj[i]);
        }
    }
    u->color=BLACK;
    time++;
    u->ftime=time;
    stk.push(u->key);
}

void dfs()
{
    char ch;
    time=0;
    cout<<"Enter the starting point : ";
    cin>>ch;
    Vertex *start=search(ch);
    Vertex *temp=start;
    cout<<"\n\nThe DFS traversal is: \n\n";
    for(;;){
        if(temp->next==start)break;
        if(temp->color==WHITE)dfs_visit(temp);
        temp=temp->next;
    }
    cout<<endl<<endl;
}

void topological_sort()
{
    cout<<endl<<endl<<"The topological sort for he given graph: "<<endl;
    while(!stk.is_empty()){
        cout<<stk.pop()<<" ";
    }
    cout<<"\n\n";
}

int main(void)
{
    int c;
    build_graph();
    cout<<"1. BFS traversal\n2. DFS traversal\n3. Topological sort\nEnter your choice: ";
    cin>>c;
    if(c==1)bfs();
    else if(c==2)dfs();
    else if(c==3){dfs(); topological_sort();}
    else{
        cout<<"Wrong choice.";
    }
    return 0;
}
