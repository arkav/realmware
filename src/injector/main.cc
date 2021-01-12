#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <windows.h>

#include <iostream>
#include <tlhelp32.h>

#define DLL_PATH "..\\dll\\libdll.dll"
#define REALM_PROC_NAME "RotMG Exalt.exe"

DWORD nameToProcID(const char *szProcName) {
  PROCESSENTRY32 entry;
  entry.dwSize = sizeof(PROCESSENTRY32);
  HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
  if (Process32First(snap, &entry) == TRUE) {
    while (Process32Next(snap, &entry) == TRUE) {
      if (strcmp(entry.szExeFile, szProcName) == 0) {
        CloseHandle(snap);
        return entry.th32ProcessID;
      }
    }
  }
  CloseHandle(snap);
  return 0;
}
int main() {
  // get full path name of dll
  char dllRealativePath[] = DLL_PATH;
  char dllAbsolutePath[4096];
  GetFullPathNameA(dllRealativePath, 4096, dllAbsolutePath, NULL);
  std::cout << "load dll path: " << dllAbsolutePath << std::endl;

  DWORD dwProcId = nameToProcID(REALM_PROC_NAME);
  if (!dwProcId) {
    std::cout << "E: unable to find rotmg" << '\n';
    return 1;
  }
  std::cout << "pid: " << dwProcId << '\n';

  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcId);
  if (!hProcess) {
    std::cout << "E: could not create handle to process"
              << "\n";
    return 1;
  }
  // fn pointer to LoadLibraryA
  LPVOID pLoadLibraryA = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

  // alloc memory inside of hProcess to fit path of dll, then write path
  // into it
  LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, strlen(dllAbsolutePath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  WriteProcessMemory(hProcess, pDllPath, dllAbsolutePath, strlen(dllAbsolutePath), NULL);

  std::cout << "wrote dll to: 0x" << std::hex << reinterpret_cast<uintptr_t>(pDllPath) << std::endl;
  // create new run load library indside of remote proc
  HANDLE hCreateSuccess =
      CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryA, pDllPath, 0, nullptr);
  if (hCreateSuccess == nullptr) {
    std::cout << "E: failure CreateRemoteThread returned " << hCreateSuccess << '\n';
  }
  WaitForSingleObject(hCreateSuccess, INFINITE);
}
