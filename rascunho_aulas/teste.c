#include <windows.h>
#include <stdio.h>

int main() {
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(&mbi, &mbi, sizeof(mbi));
    
    printf("Tamanho máximo da pilha: %zu bytes (%.2f MB)\n", 
           mbi.RegionSize, mbi.RegionSize / (1024.0 * 1024.0));

    return 0;
}