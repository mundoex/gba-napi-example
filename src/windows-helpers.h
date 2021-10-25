#ifndef WINDOWS_HELPERS_H 
#define WINDOWS_HELPERS_H

#include "windows.h"
#include <vector>
#include <tuple>

std::tuple<int, int> GetHWNDSize(HWND hwnd);
PBITMAPINFO CreateBitmapInfoStruct(HBITMAP);
void CreateBMPFile(LPTSTR pszFile, HBITMAP hBMP);
void CreateBMPFile(const char* pszFile, HBITMAP hBMP);
void GetAllWindowsFromProcessID(DWORD dwProcessID, std::vector<HWND>& vhWnds);
int GetVisibleWindowFromProcessID(DWORD dwProcessID, HWND& hwnd);
void GetHBITMAPPixelBytes(HBITMAP bitmap, long& out_width, long& out_height, std::vector<unsigned char>& bytes);

void WriteStructToBuffer(std::vector<unsigned char>& buffer, const void* inStruct, size_t size);
#endif 