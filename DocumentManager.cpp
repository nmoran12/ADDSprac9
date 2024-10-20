#include <iostream>
using namespace std;
#include <algorithm>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

// adding and searching documents: i used a binary search tree as it is relatively quick at insertion and search,
// O(log(n)), and it keeps order. Keeping order is important for the documents as we will often be accessing the
// documents, and so it is important to have them in order to find what we are looking for efficiently.

// adding patrons: I used a linked list for adding patrons because inserting into a linked list is extremely quick,
// O(1), and it is important to be able to add patrons quickly. Searching is slightly slower for linked lists, O(n), but
// this is less important as we do not search as much.

// returning and borrowing documents: this was done with a queue which is first in first out so the first to be put in
// (borrowed) will be returned first (first out). Inserting at the rear and removing fron the front, or enqueue and
// dequeue, is done in constant time (O(1)) which is extremely fast which is efficient for what we are trying to
// achieve.

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

    Patron(int patronId)
    {
        id = patronId;
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
    // Binary Search Tree root for documents
    TreeNode *root;

    // This is linked list for adding patrons
    list<Patron> patrons;

    // queue to keep track of borrowed documents per patron
    unordered_map<int, vector<int>> borrowedDocs;

  public:
    // To insert a document into the binary search tree
    TreeNode *insertDocument(TreeNode *node, Document *docFile)
    {
        if (node == nullptr)
        {
            return new TreeNode(docFile);
        }
        if (docFile->name < node->docFile->name)
        {
            node->left = insertDocument(node->left, docFile);
        }
        else
        {
            node->right = insertDocument(node->right, docFile);
        }

        return node;
    }

    // search for document by name in binary search tree
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

    // This is a function to search for a document by name and return the documents ID.
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
        for (auto it = patrons.begin(); it != patrons.end(); ++it)
        {
            if (it->id == patronID)
            {
                TreeNode *node = searchDocument(root, to_string(docID));
                if (node && node->docFile->borrowed_count < node->docFile->license_limit)
                {
                    node->docFile->borrowed_count++;
                    borrowedDocs[patronID].push_back(docID);
                    cout << "The document " << docID << " was borrowed by " << patronID << "." << endl;
                    return true;
                }

                // Document cannot be borrowed because the patron has reached their borrow limit, or document does not
                // exist.
                cout << "The document cannot be borrowed because you have reached your borrow limit." << endl;
                return false;
            }
        }

        // Patron ID was not found so document cannot be borrowed
        cout << "Your Patron ID is invalid and you cannot borrow that document." << endl;
        return false;
    };

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