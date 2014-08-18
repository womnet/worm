
#include "GuardApp.h"

int main(int argc, char* argv[])
{
    CGuardApp oApp;
    if (!oApp.Open())
    {
        return -1;
    }
    oApp.Run();
    oApp.Close();
    return 0;
}
