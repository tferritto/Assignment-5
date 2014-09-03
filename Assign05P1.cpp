#include "llcpInt.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

void SeedRand();
int BoundedRandomInt(int lowerBound, int upperBound);
int ListLengthCheck(Node* head, int whatItShouldBe);
bool match(Node* head, const int procInts[], int procSize);
void PruneDups(int a[], int& used);
void ShowArray(const int a[], int size);
void DebugShowCase(int whichCase, int totalCasesToDo,
                   const int caseValues[], int caseSize);

int main()
{
    int testCasesToDo = 900000,
    testCasesDone = 0,
    loSize = 1,
    hiSize = 20,
    loValue = 0,
    hiValue = 5;
    int numInts,
    used0,
    used1,
    used2,
    intCount,
    newInt,
    intToCopy,
    iLenChk1;
    int *intArr0 = 0,
    *intArr1 = 0,
    *intArr2 = 0;
    Node *head1 = 0;
    
    MakeDistinctPairs(head1);
    cout << "================================" << endl;
    if (head1 == 0)
        cout << "passed test on empty list" << endl;
    else
    {
        cout << "failed test on empty list..." << endl;
        exit(EXIT_FAILURE);
    }
    
    // SeedRand(); // disabled for reproducible result
    
    do
    {
        ++testCasesDone;
        numInts = BoundedRandomInt(loSize, hiSize);
        intArr0 = new int [numInts];
        intArr1 = new int [numInts];
        
        used0 = used1 = 0;
        for (intCount = 0; intCount < numInts; ++intCount)
        {
            newInt = BoundedRandomInt(loValue, hiValue);
            intArr0[used0++] = newInt;
            intArr1[used1++] = newInt;
            InsertAsTail(head1, newInt);
        }
        PruneDups(intArr1, used1);
        intArr2 = new int [2 * used1];
        used2 = 0;
        for (intCount = 0; intCount < used1; ++intCount)
        {
            intToCopy = intArr1[intCount];
            intArr2[used2++] = intToCopy;
            intArr2[used2++] = intToCopy;
        }
        
        // DebugShowCase(testCasesDone, testCasesToDo, intArr0, used0);
        
        MakeDistinctPairs(head1);
        
        iLenChk1 = ListLengthCheck(head1, used2);
        if (iLenChk1 != 0)
        {
            if (iLenChk1 == -1)
            {
                cout << "List node-count error ... too few" << endl;
                cout << "test_case: ";
                ShowArray(intArr0, used0);
                cout << "#expected: " << used2 << endl;
                cout << "#returned: " << FindListLength(head1) << endl;
            }
            else
            {
                cout << "List node-count error ... too many (circular list?)" << endl;
                cout << "test_case: ";
                ShowArray(intArr0, used0);
                cout << "#expected: " << used2 << endl;
                cout << "returned # is higher (may be infinite)" << endl;
            }
            exit(EXIT_FAILURE);
        }
        
        if ( !match(head1, intArr2, used2) )
        {
            cout << "Contents error ... mismatch found in value or order" << endl;
            cout << "initial: ";
            ShowArray(intArr0, used0);
            cout << "ought2b: ";
            ShowArray(intArr2, used2);
            cout << "outcome: ";
            ShowAll(cout, head1);
            exit(EXIT_FAILURE);
        }
        
        if (testCasesDone % 30000 == 0)
        {
            cout << "================================" << endl;
            clog << "testing case " << testCasesDone
            << " of " << testCasesToDo << endl;
            clog << "================================" << endl;
            // cout << "test case " << testCasesDone
            //      << " of " << testCasesToDo << endl;
            cout << "initial: ";
            ShowArray(intArr0, used0);
            cout << "ought2b: ";
            ShowArray(intArr2, used2);
            cout << "outcome: ";
            ShowAll(cout, head1);
        }
        
        ListClear(head1, 1);
        delete [] intArr0;
        delete [] intArr1;
        delete [] intArr2;
        intArr0 = intArr1 = intArr2 = 0;
    }
    while (testCasesDone < testCasesToDo);
    cout << "================================" << endl;
    cout << "test program terminated normally" << endl;
    cout << "================================" << endl;
    
    return EXIT_SUCCESS;
}

/////////////////////////////////////////////////////////////////////
// Function to seed the random number generator
// PRE:  none
// POST: The random number generator has been seeded.
/////////////////////////////////////////////////////////////////////
void SeedRand()
{
    srand( (unsigned) time(NULL) );
}

/////////////////////////////////////////////////////////////////////
// Function to generate a random integer between
// lowerBound and upperBound (inclusive)
// PRE:  lowerBound is a positive integer.
//       upperBound is a positive integer.
//       upperBound is larger than lowerBound
//       The random number generator has been seeded.
// POST: A random integer between lowerBound and upperBound
//       has been returned.
/////////////////////////////////////////////////////////////////////
int BoundedRandomInt(int lowerBound, int upperBound)
{
    return ( rand() % (upperBound - lowerBound + 1) ) + lowerBound;
}

/////////////////////////////////////////////////////////////////////
// Function to check # of nodes in list against what it should be
// POST: returns
//          -1 if # of nodes is less than what it should be
//           0 if # of nodes is equal to  what it should be
//           1 if # of nodes is more than what it should be
/////////////////////////////////////////////////////////////////////
int ListLengthCheck(Node* head, int whatItShouldBe)
{
    int length = 0;
    while (head != 0)
    {
        ++length;
        if (length > whatItShouldBe) return 1;
        head = head->link;
    }
    return (length < whatItShouldBe) ? -1 : 0;
}

bool match(Node* head, const int procInts[], int procSize)
{
    int iProc = 0;
    while (head != 0)
    {
        if (iProc == procSize) return false;
        if (head->data != procInts[iProc]) return false;
        ++iProc;
        head = head->link;
    }
    return true;
}

void PruneDups(int a[], int& used)
{
    int target, dupCount, i, j;
    for (i = 0; i < used; ++i)
    {
        dupCount = 0;
        target = a[i];
        for (j = i + 1; j < used; ++j)
        {
            if (a[j] == target)
                ++dupCount;
            else if (dupCount)
                a[j - dupCount] = a[j];
        }
        used -= dupCount;
    }
}

void ShowArray(const int a[], int size)
{
    for (int i = 0; i < size; ++i)
        cout << a[i] << "  ";
    cout << endl;
}

void DebugShowCase(int whichCase, int totalCasesToDo,
                   const int caseValues[], int caseSize)
{
    cout << "test case " << whichCase
    << " of " << totalCasesToDo << endl;
    cout << "given list: ";
    ShowArray(caseValues, caseSize);
}
