#include<bits/stdc++.h>
using namespace std;
class kQueue{
int *arr;
int freespot;
int *front;
int *rear;
int *nxt;
int n;
int k ;
public:
    kQueue(int n,int k)
    {
        this->n=n;
        this->k=k;
        this->front=new int[k];
        this->rear=new int[k];
        this->arr=new int[n];
        this->nxt=new int[n];
        for(int i=0;i<k;i++)
        {
            front[i]=-1;
            rear[i]=-1;
        }
        for(int i=0;i<n;i++)
        {
            nxt[i]=i+1;
        }
        nxt[n-1]=-1;
        freespot=0;
    }

    void push(int data,int qn)
    {
        if(freespot==-1) cout<<"Queue Overflow"<<endl;

        int index=freespot;
        //freespot update
        freespot=nxt[index];
    //if first element
    if(front[qn-1]==-1) 
    {
        front[qn-1]=index;
    }
    else
    {
        nxt[rear[qn-1]]=index;

    }
    nxt[index]=-1;
    rear[qn-1]=index;
    arr[index]=data;

    }
    int pop(int qn)
    {
        if(front[qn-1]==-1) return -1;

        int index=front[qn-1];
        front[qn-1]=nxt[index];
        nxt[index]=freespot;
        freespot=index;
        return arr[index];
    }

};

int main()
{
    kQueue q(5,3);
    q.push(2,1);
    q.push(3,2);
    q.push(4,3);
    q.push(6,1);
    q.push(5,1);
    cout<<q.pop(1)<<endl;
    cout<<q.pop(1)<<endl;
    cout<<q.pop(1)<<endl;
    cout<<q.pop(3)<<endl;



    // q.push(2,1);

}