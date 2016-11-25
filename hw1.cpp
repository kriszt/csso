#include <windows.h>
#include <assert.h>
#include <iostream>

#include "hw1.h"

static HKEY createKey(HKEY keyPath, char* keyName)
{
    assert(keyPath != NULL);
    assert(keyName != NULL);

    HKEY keyHandle;
    if(RegCreateKeyEx(keyPath,keyName,0,0,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&keyHandle,NULL) == ERROR_SUCCESS)
    {
        std::cout<<"Created/opened key ";
        std::cout<<keyName<<std::endl;
    }
    else
    {
        std::cout<<"Error creating/opening key ";
        std::cout<<keyName<<std::endl;
    }
    return keyHandle;
}

static void findFile(HKEY hKey, std::string path)
{
    assert(path.size() != 0);
    assert(hKey != NULL);

    WIN32_FIND_DATA file;
    std::string fullPath = path + "\\*";
    HANDLE searchHandle = FindFirstFile(fullPath.c_str(), &file);
    HKEY newHKey;
    DWORD size;


    if (searchHandle != INVALID_HANDLE_VALUE)
    {
        do {
            if	(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (!strcmp(file.cFileName,".") || !strcmp(file.cFileName,".."))
                    continue;
                newHKey = createKey(hKey,file.cFileName);
                findFile(newHKey, path + "\\" +  file.cFileName);
            }
            else {
                size = (file.nFileSizeHigh * (MAXDWORD+1)) + file.nFileSizeLow;
                if (RegSetValueEx(hKey,file.cFileName,0,REG_DWORD,(const BYTE*)&size,sizeof(size)) == ERROR_SUCCESS)
                {
                    std::cout<<"Created/modified value ";
                    std::cout<<file.cFileName;
                    std::cout<<" of size ";
                    std::cout<<size<<std::endl;
                }
                else
                {
                    std::cout<<"Couldn't create/modify value ";
                    std::cout<<file.cFileName<<std::endl;
                }
            }
        } while (FindNextFile(searchHandle, &file));
        FindClose(searchHandle);
    } else 
        std::cout<<"Error for the path "<<path;
}

void hw1()
{
    std::string path = "D:\\VMs";
    HKEY hKey;
    hKey = createKey(HKEY_CURRENT_USER,"SOFTWARE");
    hKey = createKey(hKey,"CSSO");
    findFile(hKey, path);
}
