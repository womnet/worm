
#ifndef _WORMNET_QUEUE_H
#define _WORMNET_QUEUE_H

#include <queue>
#include "wormnet/wm_types.h"

namespace worm
{

template <class T>
class WORM_DLL_EXPORT wmQueue : public std::queue<T>
{

};

}

#endif // ifndef _WORMNET_QUEUE_H
