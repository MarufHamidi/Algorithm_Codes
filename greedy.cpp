#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdlib>
#define MAX 50

using namespace std;

fstream fin("greedy_input2.txt");
int total_request,total_accepted,depth,counter;

struct info{
    char stime[MAX];
    char ftime[MAX];
    char name[MAX];
    int preceeding_overlap;
    int allotment;
}request_main[MAX],sorted_ftime[MAX],sorted_stime[MAX],request_accepted[MAX];

struct partition{
    int end;
    int space;
}label[MAX];

// taking input from file
void take_input()
{
    fin>>total_request;
    char s[MAX],f[MAX],item[MAX];
    for(int i=0;i<total_request;i++){
        fin>>s;
        strcpy(request_main[i].stime,s);
        fin>>f;
        strcpy(request_main[i].ftime,f);
        fin>>item;
        strcpy(request_main[i].name,item);
        request_main[i].preceeding_overlap=0;
        request_main[i].allotment=NULL;
    }
    cout<<total_request;
    cout<<" requests has been recieved for the further analysis."<<endl;
}

// copy the request into two array
void copy_request()
{
	int j;
	for(j=0;j<total_request;j++)sorted_ftime[j]=request_main[j];
	for(j=0;j<total_request;j++)sorted_stime[j]=request_main[j];
	cout<<"Requests has been copied."<<endl;
}

//sorting in order of the finish time
void fsort()
{
	int i,k;
	info temp;
	for(i=0;i<total_request;i++){
		for(k=i+1;k<total_request;k++){
			if(atoi(sorted_ftime[i].ftime)>atoi(sorted_ftime[k].ftime)){
				temp=sorted_ftime[k];
				sorted_ftime[k]=sorted_ftime[i];
				sorted_ftime[i]=temp;
			}
		}
	}
	cout<<"Requests has been sorted as per the finish time."<<endl;
}

//sorting the request in order of the starting time
void ssort()
{
	int i,k;
	info temp;
	for(i=0;i<total_request;i++){
		for(k=i+1;k<total_request;k++){
			if(atoi(sorted_stime[i].stime)>atoi(sorted_stime[k].stime)){
				temp=sorted_stime[k];
				sorted_stime[k]=sorted_stime[i];
				sorted_stime[i]=temp;
			}
		}
	}
	cout<<"Requests has been sorted as per the start time."<<endl;
}

// printing the main request list
void mprint()
{
	int j;
	cout<<endl<<"Request in order they have been received."<<endl<<endl;
	for(j=0;j<total_request;j++){
        cout<<request_main[j].stime<<"\t"<<request_main[j].ftime<<"\t"<<request_main[j].name<<endl;
    }
	cout<<endl;
}

// printing the list in order of finish time
void fprint()
{
	int j;
	cout<<endl<<"Sorted request in order of their finish time."<<endl<<endl;
	for(j=0;j<total_request;j++){
        cout<<sorted_ftime[j].stime<<"\t"<<sorted_ftime[j].ftime<<"\t"<<sorted_ftime[j].name<<endl;
    }
	cout<<endl;
}

// printing the list in order of start time
void sprint()
{
	int j;
	cout<<endl<<"Sorted request in order of their start time."<<endl<<endl;
	for(j=0;j<total_request;j++){
        cout<<sorted_stime[j].stime<<"\t"<<sorted_stime[j].ftime<<"\t"<<sorted_stime[j].name<<endl;
    }
	cout<<endl;
}

// interval scheduling
void greedy()
{
    total_accepted=0;
    request_accepted[total_accepted]=sorted_ftime[0];
    total_accepted++;
    for(int i=1;i<total_request;i++){
        if(atoi(request_accepted[total_accepted-1].ftime)<atoi(sorted_ftime[i].stime)){
            request_accepted[total_accepted]=sorted_ftime[i];
            total_accepted++;
        }
    }
    cout<<endl<<"Total "<<total_accepted<<" requests has been accepted."<<endl<<endl;
}

//printing the accepted list
void gprint()
{
    cout<<"The accepted requests are: ";
    for(int i=0;i<total_accepted;i++){
        cout<<request_accepted[i].name<<"  ";
    }
    cout<<endl;
}

void pprint()
{
    int j;
	cout<<endl<<"Request along with their labelling - -"<<endl<<endl;
	cout<<"Request\tLabel\n_______\t_____\n";
	for(j=0;j<total_request;j++){
        cout<<sorted_stime[j].name<<"\t"<<sorted_stime[j].allotment<<endl<<endl;
    }
	cout<<endl;
}

// finding depth
void find_depth()
{
    int temp;
    for(int i=1;i<total_request;i++){
        temp=0;
        for(int j=0;j<i;j++){
            if(atoi(request_main[j].ftime)>atoi(request_main[i].stime))temp++;
        }
        request_main[i].preceeding_overlap=temp;
    }

    temp=0;
    for(int k=0;k<total_request;k++){
        if(temp<request_main[k].preceeding_overlap)temp=request_main[k].preceeding_overlap;
    }

    depth=temp+1;
    cout<<endl<<"Depth of the intervals is "<<depth<<endl;
}

// finding out the partition for each request
void find_partition()
{
    int i,j;
    for(i=0;i<depth;i++){
        label[i].space=i+1;
        label[i].end=-1;
    }
    sorted_stime[0].allotment=label[0].space;
    label[0].end=atoi(sorted_stime[0].ftime);
    for(j=1;j<total_request;j++){
        for(i=0;i<depth;i++){
            if(label[i].end<atoi(sorted_stime[j].stime)){
                sorted_stime[j].allotment=label[i].space;
                label[i].end=atoi(sorted_stime[j].ftime);
                break;
            }
        }
    }
}

// if two resources are available
void custpart()
{
    int i,j;
    counter=0;
    for(i=0;i<depth;i++){
        label[i].space=i+1;
        label[i].end=-1;
    }
    sorted_ftime[0].allotment=label[0].space;
    label[0].end=atoi(sorted_ftime[0].ftime);
    counter++;
    for(i=1;i<total_request;i++){
        for(j=0;j<2;j++){
            if(label[j].end<atoi(sorted_ftime[i].stime)){
                sorted_ftime[i].allotment=label[i].space;
                label[j].end=atoi(sorted_ftime[i].ftime);
                counter++;
            }
        }
    }
}

int main(void)
{
    take_input();
	copy_request();

	fsort();
	ssort();

	/*mprint();
	fprint();
	sprint();*/

	/*greedy();
	gprint();

	find_depth();
	find_partition();

	pprint();*/

	custpart();
	cout<<counter;

    return 0;
}
