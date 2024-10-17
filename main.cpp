#include "DocumentManager.cpp" // Assuming this is the file where your DocumentManager class and related code is located.
#include <iostream>

using namespace std;

int main()
{
    // Create an instance of DocumentManager
    DocumentManager manager;

    // Add some documents
    manager.addDocument("Document A", 101, 5);
    manager.addDocument("Document B", 102, 3);
    manager.addDocument("Document C", 103, 2);

    // Add some patrons
    manager.addPatron(201);
    manager.addPatron(202);
    manager.addPatron(203);

    // Search for a document
    string searchDoc = "Document B";
    int foundDocId = manager.search(searchDoc);

    if (foundDocId != 0)
    {
        cout << "Document " << searchDoc << " found with ID: " << foundDocId << endl;
    }
    else
    {
        cout << "Document " << searchDoc << " not found." << endl;
    }

    // Patron borrows a document
    bool borrowSuccess = manager.borrowDocument(102, 201);
    if (borrowSuccess)
    {
        cout << "Patron 201 successfully borrowed Document 102." << endl;
    }
    else
    {
        cout << "Failed to borrow Document 102 for Patron 201." << endl;
    }

    // Patron returns a document
    manager.returnDocument(102, 201);
    cout << "Patron 201 returned Document 102." << endl;

    // Search for a document that doesn't exist
    string missingDoc = "Document D";
    int missingDocId = manager.search(missingDoc);

    if (missingDocId != 0)
    {
        cout << "Document " << missingDoc << " found with ID: " << missingDocId << endl;
    }
    else
    {
        cout << "Document " << missingDoc << " not found." << endl;
    }

    return 0;
}
