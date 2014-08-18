
#include "Server/App.h"

int main(int argc, char* argv[])
{
    if (g_pApp->Open())
    {
        g_pApp->Run();
        g_pApp->Close();
    }

    return 0;
}
