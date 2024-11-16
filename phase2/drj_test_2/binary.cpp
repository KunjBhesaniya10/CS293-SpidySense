#include<bits/stdc++.h>
using namespace std;
class node{
    public:
    node* left;
    node* right;
    int data;
    node(int data)
    {
        this->data=data;
        this->right=NULL;
        this->left =NULL;
    }

};
void implement(node* &root)
{
    
    cout<<"insert data "<<endl;
    int data;cin>>data;
    if(data==-1) return ;//NULL
    root=new node(data);
    cout<<"To the left"<<endl;
    implement(root->left);
    cout<<"To the right"<<endl;
    implement(root->right);
}
void leveltraversal(node* root)
{
    queue<node*>q;
    q.push(root);
    q.push(NULL);
    vector<int>cnt1;
    int cnt=0;
    while(!q.empty())
    {
        
        node* tmp=q.front();q.pop();
        if(tmp==NULL)
        {
            cout<<endl;
            if(!q.empty())
            {
                q.push(NULL);
                cnt1.push_back(cnt);
                cnt=0;
            }
        }
        else
        {
            cnt++;
            cout<<tmp->data<<" ";
            if(tmp->left) q.push(tmp->left);
            if(tmp->right) q.push(tmp->right);
        }
    }
    cnt1.push_back(cnt);
    for(int i=0;i<cnt1.size();i++) cout<<"hi"<<cnt1[i]<<" ";
    cout<<endl;
}
void reverseLevel(node* root)
{  stack<int>s;
   queue<node*>q;
    q.push(root);
    q.push(NULL);
    while(!q.empty())
    {
        node* tmp=q.front();q.pop();
        if(tmp==NULL)
        {
            s.push(-1);
            if(!q.empty())
            {
                q.push(NULL);
            }
        }
        else
        {
            s.push(tmp->data);
            if(tmp->right) q.push(tmp->right);
            if(tmp->left) q.push(tmp->left);
        }
    } 
    while(!s.empty())
    {
        if(s.top()==-1)
        { cout<<endl; s.pop();}
        else 
        {
            cout<<s.top()<<" ";
            s.pop();
        }
    }
}
void inorder(node *root)
{
    if(root==NULL) return;

    inorder(root->left);
    cout<<root->data<<" ";
    inorder(root->right);
}
void preorder(node* root)
{
    if(root==NULL) return;

    cout<<root->data<<" ";
    preorder(root->left);
    preorder(root->right);
}
void postorder(node* root)
{
    if(root==NULL) return ;

    postorder(root->left);
    postorder(root->right);
    cout<<root->data<<" ";
}
void buildFromLevelTree(node* &root)
{
    int data;
    cout<<"Enter data for root: "<<" ";
    cin>>data;
    root=new node(data);
    queue<node*>q;
    q.push(root);
    while(!q.empty())
    {
        node* tmp=q.front();q.pop();
        cout<<"Enter left data for"<<tmp->data<<" ";
        int ldata;cin>>ldata;
        if(ldata!=-1){
        tmp->left=new node(ldata);
        q.push(tmp->left);}
        cout<<"Enter right data for"<<tmp->data<<" ";
        int rdata;cin>>rdata;
        if(rdata!=-1)
        {tmp->right=new node(rdata);
        q.push(tmp->right);}
    }
}
        
void MorrisTraversal(node* &root)
{
    if(root==NULL) return;

    node* curr=root;
    node* prev=NULL;
    while(curr!=NULL)
    {
        if(curr->left==NULL)
        {
            cout<<curr->data<<" ";
            curr=curr->right;
        }
        else
        {
            prev=curr->left;
            while(prev->right!=NULL && prev->right!=curr)
            {
                prev=prev->right;
            }
            //two cases
            //Case1: prev->right=NULL;
            if(prev->right==NULL)
            {
                prev->right=curr;
                curr=curr->left;
            }
            else
            {
                prev->right=NULL;
                cout<<curr->data<<" ";
                curr=curr->right;
                
            }
        }
    }
}

int main()

{
    //1 2 3 4 5 -1 6 -1 7 -1 -1 -1 -1 -1 -1
    node* root=NULL;
    buildFromLevelTree(root);
    cout<<endl;
    MorrisTraversal(root);
    leveltraversal(root);
        }