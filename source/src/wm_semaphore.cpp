
#include <assert.h>
#include "wormnet/wm_semaphore.h"

namespace worm
{

wmSem::wmSem()
{
    m_bInitial = false;
#ifdef WIN32
    m_hHandle  = NULL;
#endif // ifdef WIN32
}

wmSem::~wmSem()
{
    DestorySemaphore();
}

void wmSem::CreateSemaphore(const uint32& dwInitialCount, const uint32& dwMaximumCount)
{
    assert(!m_bInitial);
    m_bInitial = true;

#ifdef WIN32
    m_hHandle = ::CreateSemaphore(NULL, dwInitialCount, dwMaximumCount, NULL);
#else
    sem_init(&m_hHandle, 0, dwInitialCount);
#endif // ifdef WIN32
}

void wmSem::PostSemaphore()
{
    assert(m_bInitial);

#ifdef WIN32
    ::ReleaseSemaphore(m_hHandle, 1, NULL);
#else
    sem_post(&m_hHandle);
#endif // ifdef WIN32
}

void wmSem::WaitSemaphore()
{
    assert(m_bInitial);
#ifdef WIN32
    ::WaitForSingleObject(m_hHandle, INFINITE);
#else
    sem_wait(&m_hHandle);
#endif // ifdef WIN32
}

void wmSem::DestorySemaphore()
{
    if (!m_bInitial)
    {
        return;
    }
    m_bInitial = false;
#ifdef WIN32
    ::CloseHandle(m_hHandle);
    m_hHandle = NULL;
#else
    sem_destroy(&m_hHandle);
#endif // ifdef WIN32
}

}
