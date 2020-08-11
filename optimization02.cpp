#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

/* Project : OMT (NEC Telecom Software Philippines Inc.) */
/*
 * Construction of objects with large members such as large buffer or files has a cost which has an effect on performance. For this optimization
 * a reference count was added as data member to prevent unnecesarry object creation.
 */

#define OPTIMIZED

using namespace std;

typedef unsigned char BYTE;


class LargeObject
{
    BYTE* data;
    size_t size;
    int* ref_cnt;
public:
    LargeObject(const LargeObject& obj) 
    {
        if (this != &obj) {
#ifdef OPTIMIZED

            data = obj.data;
            ref_cnt = obj.ref_cnt;
            *ref_cnt = (*ref_cnt) + 1;
#else
            data = new BYTE[obj.size];
            memcpy(data, obj.data, size);
#endif
            size = obj.size;
           
        }
    }
    
    LargeObject& operator=(const LargeObject& obj)
    {

        if (this != &obj) {

#ifdef OPTIMIZED

            *ref_cnt = *ref_cnt - 1;

            if (*ref_cnt < 1) {
                delete ref_cnt;
                if (data != NULL) {
                    delete[] data;
                    data = NULL;
                }
            }

            ref_cnt = obj.ref_cnt;
            data = obj.data;
            *ref_cnt = *ref_cnt + 1;
            size = obj.size;
#else


            if (data != NULL) {
                delete[] data;
                data = NULL;
            }
            data = new BYTE[size];
            size = obj.size;

            memcpy(data, obj.data, size);
#endif
        }
    }


    LargeObject(size_t size)
    {
        data = new BYTE[size];
        this->size = size;
        ref_cnt = new int(1);
    }
    ~LargeObject()
    {        
#ifdef OPTIMIZED
        if (ref_cnt != NULL)
        {
            *ref_cnt = *ref_cnt - 1;


            if (*ref_cnt < 1) {
                delete ref_cnt;
#endif
                if (data != NULL) {
                    delete[] data;
                    data = NULL;
                }
#ifdef OPTIMIZED
            }

        }
#endif
    }
};

void process(LargeObject& obj)
{
    //..
    LargeObject local = obj;
    //....
}

void optimize_test02()
{   
    DWORD dwStartTime = GetTickCount();
    for (int i = 0; i < 500000; i++) {
        LargeObject obj(100000);
        process(obj);
    }
    DWORD dwFinishTime = GetTickCount();
    cout << "ms. passed: " << dwFinishTime - dwStartTime << endl;

    int n = 0;

}