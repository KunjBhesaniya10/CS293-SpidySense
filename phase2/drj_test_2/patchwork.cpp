#include <bits/stdc++.h> //trie queue binary.cpp 
using namespace std;
class TrieNode
{
public:
    char data;
    TrieNode *children[26];
    bool isTerminal;

    // constructor
    TrieNode(char ch)
    {
        this->data = ch;
        for (int i = 0; i < 26; i++)
            children[i] = NULL;
        this->isTerminal = false;
    }
};

void push(int data, int qn)
{
    if (freespot == -1)
        cout << "Queue Overflow" << endl;

    int index = freespot;
    // freespot update
    freespot = nxt[index];
    // if first element
    if (front[qn - 1] == -1)
    {
        front[qn - 1] = index;
    }
    else
    {
        nxt[rear[qn - 1]] = index;
    }
    nxt[index] = -1;
    rear[qn - 1] = index;
    arr[index] = data;
}

void buildFromLevelTree(node *&root)
{
    int data;
    cout << "Enter data for root: " << " ";
    cin >> data;
    root = new node(data);
    queue<node *> q;
    q.push(root);
    while (!q.empty())
    {
        node *tmp = q.front();
        q.pop();
        cout << "Enter left data for" << tmp->data << " ";
        int ldata;
        cin >> ldata;
        if (ldata != -1)
        {
            tmp->left = new node(ldata);
            q.push(tmp->left);
        }
        cout << "Enter right data for" << tmp->data << " ";
        int rdata;
        cin >> rdata;
        if (rdata != -1)
        {
            tmp->right = new node(rdata);
            q.push(tmp->right);
        }
    }
}

bool searchUtil(TrieNode *root, string word)
    {
        if (word.length() == 0)
            return root->isTerminal;

        int index = word[0] - 'A';
        TrieNode *child;
        if (root->children[index] != NULL)
        {
            child = root->children[index];
        }
        else
            return false;

        // recursive calls
        return searchUtil(child, word.substr(1));
    }
Polynomial operator-(Polynomial &b)
    {
        int k;
        int max_capacity = max(this->capacity, b.capacity);
        Polynomial result;
        result.capacity = max_capacity;
        result.degCoeff = new int[max_capacity];
        if (max_capacity > this->capacity)
        {
            for (k = 0; k < this->capacity; k++)
                result.degCoeff[k] = -b.degCoeff[k] + this->degCoeff[k];

            for (; k < max_capacity; k++)
                result.degCoeff[k] = -b.degCoeff[k];
        }
        else
        {

            for (k = 0; k < b.capacity; k++)
                result.degCoeff[k] = -b.degCoeff[k] + this->degCoeff[k];

            for (; k < max_capacity; k++)
                result.degCoeff[k] = this->degCoeff[k];
        }
        return result;
    }
Polynomial()
    {
        degCoeff = new int[10]; // intially 10 elements;
        for (int i = 0; i < 10; i++)
            degCoeff[i] = 0;
        this->capacity = 10;
    }