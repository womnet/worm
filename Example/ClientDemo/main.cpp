
#include "ClientDemo/ConnectHandler.h"
#include <conio.h>

int main()
{
    CConnectHandler::Instance()->Open();
    CConnectHandler::Instance()->Run();

    getch();

    return 0;
}
