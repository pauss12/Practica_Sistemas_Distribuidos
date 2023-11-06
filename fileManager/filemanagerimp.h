#ifndef FILEMANAGER_IMP_H
#define FILEMANAGER_IMP_H

#include "filemanager.h"


#define OP_EXIT 0
#define OP_READFILE 1
#define OP_LISTFILES 2
#define OP_WRITEFILE 3

class FileManager_Imp
{
    FileManager* fm = nullptr;
    
public:
    FileManager_Imp(string path);
    ~FileManager_Imp();
    void run();
};

#endif // FILEMANAGER_IMP_H
