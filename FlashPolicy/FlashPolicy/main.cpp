
#include <wormnet/ILogs.h>
#include "FlashPolicy/PolicyApp.h"

int main()
{
    if (g_pPolicyApp->Open())
    {
        g_pPolicyApp->Run();
        g_pPolicyApp->Close();
    }

    Sleep(100);

    return 0;
}
