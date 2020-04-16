#include <windows.h>
#include <iostream>
#pragma comment(lib, "user32.lib")

int main()
{
    BOOL fResult;
    UINT* bmpPath = (UINT*)"E:\\1.bmp";
    fResult = SystemParametersInfo(SPI_SETDESKWALLPAPER,
                                   1,
                                   bmpPath,
                                   SPIF_UPDATEINIFILE);
    return 0;
}
