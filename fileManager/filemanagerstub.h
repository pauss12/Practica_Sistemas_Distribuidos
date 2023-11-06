#ifndef FILEMANAGER_STUB_H
#define FILEMANAGER_STUB_H

#include <string>
#include <vector>

#define OP_EXIT 0
#define OP_READFILE 1
#define OP_LISTFILES 2
#define OP_WRITEFILE 3

using namespace std;

class FileManager_Stub
{
    int commIndex;
public:
    FileManager_Stub(char* host);
    ~FileManager_Stub();
    vector<string*>* listFiles();
    void readFile(char* fileName, char* &data, unsigned long int & dataLength);
    void writeFile(char* fileName, char* data, unsigned long int dataLength);
    void freeListedFiles(vector<string*>* fileList);
};

#endif // FILEMANAGER_STUB_H
