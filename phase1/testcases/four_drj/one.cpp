#include<iostream>
#include<vector>
using namespace std;

int bsearch(vector<int>& a,int s,int e,int key){
    while(s<e){
        int mid=(s+e)/2;
        if(a[mid]==key) return mid;
        else if(a[mid]>key) e=mid-1;
        else s=mid+1;

    }
}

int main(){
    vector<int>a={1,2,3,4,5,5,7,8};
    int ans=bsearch(a,0,7,2);
    cout<<ans<<endl;
}