#include"LLNode.h"

//This file is just to test LLnode functionality, or any other .c functionality
//not meant to be part of final submission, or used for main code

int main()
{
    LLNodePTR front = 0; //malloc(sizeof(LLNode));
    front = LLNodeInit(front,"fruit","apple");
    front = LLNodeInit(front,"citrus","orange");

    //front = SelectionSort(front);
    LLPrint(front);
       FreeLL(front); 
}
