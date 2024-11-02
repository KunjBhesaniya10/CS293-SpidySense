#include<iostream>
using namespace std;

int binarysearch(int * a,int s,int e,int key){
    while(s<e){
        int mid=s+(e-s)/2;
        if(a[mid]==key) return mid;
        else if(a[mid]<key) s=mid+1;
        else e=mid-1;

    }
}

int main(){
    int a[8]={1,2,3,4,5,5,7,8};
    int b=binarysearch(a,0,7,2);
    cout<<b<<endl;
}