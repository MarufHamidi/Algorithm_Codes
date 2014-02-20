#include<iostream>

using namespace std;

void func(int **b)
{
    int i;
    for(i=0;i<5;i++){
        for(int j=0;j<5;j++)b[i][j]=i;
    }
}

int main(void)
{
    int i,j;
    int *a[5];
    for(i=0;i<5;i++)a[i]=new int[5];
    func(a);
    for(i=0;i<5;i++){
        for(j=0;j<5;j++)cout<<a[i][j]<<" ";
        cout<<endl;
    }
    cout<<'\\';
    return 0;
}
