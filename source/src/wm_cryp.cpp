
#include <stdlib.h>

#include "Tea.h"

namespace worm
{

WORM_DLL_EXPORT class ICryp* ICreateCrypModule(const wmECrypType& eCrypType)
{
    return new(CTea);
}

WORM_DLL_EXPORT void IReleaseCrypModule(ICryp*& pCryp)
{
    delete pCryp;
    pCryp = NULL;
}

}
