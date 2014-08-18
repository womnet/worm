
#include <wormnet/wm_std.h>
#include <wormnet/TidyXML.h>

class CGuardApp
{
public:
    CGuardApp();
    ~CGuardApp();

    bool Open();
    void Run();
    void Close();
};
