#include <iostream>
using namespace std;
#include <algorithm>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

struct Document
{
    string name;
    int id;
    int license_limit;
    int borrowed_count;

    Document(string docName, int docId, int docLimit)
    {
        name = docName;
        id = docId;
        license_limit = docLimit;
        borrowed_count = 0;
    }
};

struct Patron
{
    int id;

    Patron(int pId)
    {
        id = pId;
    }
};

// Binary Search Tree Nodes

// BST Node for Documents
struct TreeNode
{
    Document *docFile;
    TreeNode *left;
    TreeNode *right;

    TreeNode(Document *docLocal)
    {
        docFile = docLocal;
        left = nullptr;
        right = nullptr;
    }
};

class DocumentManager
{
  private:
    TreeNode *root;                             // BST root for documents
    list<Patron> patrons;                       // This is linked list for adding patrons
    unordered_map<int, list<int>> borrowedDocs; // hash map to keep track of borrowed documents per patron,
                                                // so it works by having patronID -> list of docIDs

    // To insert a document into the binary search tree
    TreeNode *insertDocument(TreeNode *node, Document *docFile)
    {
        if (node == nullptr)
        {
            return new TreeNode(docFile);
        }

        if (node->docFile == docFile)
        {
            return node;
        }

        // recur down tree, if doc to be inserted is greater than node's doc, insert
        // it to right
        if (node->docFile < docFile)
        {
            node->right = insertDocument(node->right, docFile);
        }
        else
        {
            node->left = insertDocument(node->left, docFile);
        }

        return node;
    }

    // search for document by file in binary search tree
    TreeNode *searchDocument(TreeNode *node, const string &targetName)
    {
        if (node == nullptr || node->docFile->name == targetName)
        {
            return node;
        }

        if (targetName < node->docFile->name)
        {
            return searchDocument(node->left, targetName);
        }
        return searchDocument(node->right, targetName);
    }

  public:
    DocumentManager()
    {
        root = nullptr;
    }

    // This is a function to add a document
    void addDocument(string name, int id, int license_limit)
    {
        Document *newDoc = new Document(name, id, license_limit);
        root = insertDocument(root, newDoc);
        cout << "The document " << name << " has been added." << endl;
    };

    // This is a function to add a patron
    void addPatron(int patronID)
    {
        patrons.push_back(Patron(patronID));
        cout << "The patron " << patronID << " has been added." << endl;
    }

    // This is a function to search for a document by name
    int search(string name)
    {
        TreeNode *found = searchDocument(root, name);
        if (found)
        {
            return found->docFile->id;
        }
        return 0;
    }

    // This is a function to borrow a document from the library
    bool borrowDocument(int docID, int patronID)
    {
        for (auto &patron : patrons)
        {
            if (patron.id == patronID)
            {
                TreeNode *node = searchDocument(root, to_string(docID));
                if (node && node->docFile->borrowed_count < node->docFile->license_limit)
                {
                    node->docFile->borrowed_count++;
                    borrowedDocs[patronID].push_back(docID);
                    cout << "The document " << docID << " was borrowed by " << patronID << "." << endl;
                    return true;
                }
                cout << "The document cannot be borrowed because you have reached your borrow limit." << endl;
                return false;
            }
        }

        cout << "Your Patron ID is invalid and you cannot borrow that document." << endl;
        return false;
    }

    // This is a function to return a document
    void returnDocument(int docID, int patronID)
    {
        auto patronIterator = borrowedDocs.find(patronID);
        if (patronIterator != borrowedDocs.end())
        {
            auto &docs = patronIterator->second;
            auto docIterator = find(docs.begin(), docs.end(), docID);
            if (docIterator != docs.end())
            {
                docs.erase(docIterator);
                TreeNode *node = searchDocument(root, to_string(docID));
                if (node)
                {
                    node->docFile->borrowed_count--;
                    cout << "The document " << docID << " was returned by " << patronID << "." << endl;
                }
            }
            else
            {
                cout << "The document was not found in patron's borrowed list." << endl;
            }
        }
        else
        {
            cout << "Your patron ID is invalid." << endl;
        }
    }
};