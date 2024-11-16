#include <bits/stdc++.h>
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

class Trie
{

public:
    TrieNode *root;

    // constructor
    Trie()
    {
        this->root = new TrieNode('\0');
    }

    // insertion algo
    void insertUtil(TrieNode *root, string word)
    {

        if (word.length() == 0)
        {
            root->isTerminal = true;
            return;
        }

        int index = word[0] - 'A';
        TrieNode *child;
        if (root->children[index] != NULL) // if the child or the char exists just move forward
        {
            child = root->children[index];
        }
        else
        {
            child = new TrieNode(word[0]);
            root->children[index] = child;
        }
        // recursive call;
        insertUtil(child, word.substr(1));
    }
    void insertWord(string word)
    {
        insertUtil(root, word);
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

    bool searchWord(string word)
    {
        return searchUtil(root, word);
    }
    void deleteUtil(TrieNode *root, string word)
    {
        if (word.length() == 0)
            root->isTerminal = false;

        int index = word[0] - 'A';
        TrieNode *child;
        if (root->children[index] != NULL)
        {
            child = root->children[index];
        }
        else
            return;

        // recursive calls
        deleteUtil(child, word.substr(1));
    }

    void deleteWord(string word)
    {
        deleteUtil(root, word);
    }
};

int main()
{
    Trie *t = new Trie();
    t->insertWord("ABCD");
    t->insertWord("ABCED");
    cout << t->searchWord("ABCED") << endl;
    t->deleteWord("ABCED");
    cout << t->searchWord("ABCED") << endl;
}