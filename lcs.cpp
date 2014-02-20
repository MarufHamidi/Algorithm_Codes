#include<iostream>
#include<fstream>
#include<cstring>
#define MAX 100

using namespace std;

int c[MAX][MAX];
char x[MAX],y[MAX],b[MAX][MAX];

//reading from the file
void take_input()
{
    int i,j;
    fstream fin("lcs_input.txt",ios::in);
    char ch[MAX];
    strcpy(x,"");
    strcpy(y,"");
    fin>>ch;
    for(i=3,j=0;i<strlen(ch)-1;j++,i++)x[j]=ch[i];
    x[j]='\0';
    fin>>ch;
    for(i=3,j=0;i<strlen(ch)-1;j++,i++)y[j]=ch[i];
    y[j]='\0';
    fin.close();
}

//finding the lcs
void lcs_length()
{
    int i,j,m=strlen(x),n=strlen(y);

    for(i=0;i<=n;i++)c[0][i]=0;
    for(i=0;i<=m;i++)c[i][0]=0;

    for(i=1;i<=m;i++){
        for(j=1;j<=n;j++){
            if(x[i-1]==y[j-1]){
                c[i][j]=c[i-1][j-1]+1;
                b[i][j]='\\';
            }
            else if(c[i-1][j]>=c[i][j-1]){
                c[i][j]=c[i-1][j];
                b[i][j]='|';
            }
            else{
                c[i][j]=c[i][j-1];
                b[i][j]='-';
            }
        }
    }
}

//print the lcs
void print_lcs(int i,int j)
{
    if(i==0 || j==0)return;

    if(b[i][j]=='\\'){

        print_lcs(i-1,j-1);
        cout<<x[i-1];
    }
    else if(b[i][j]=='|'){
        print_lcs(i-1,j);
    }
    else print_lcs(i,j-1);
}

int main(void)
{
    take_input();

    int i,j,m=strlen(x),n=strlen(y);

    lcs_length();
    print_lcs(m,n);



    return 0;
}
