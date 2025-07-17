#include "stdafx.h"

int main()
{
    Utils::ReloadData();
    FRAMEWORK.Init(1280, 720, "TITLE!!");
    FRAMEWORK.Do();
    FRAMEWORK.Release();

    return 0;
}