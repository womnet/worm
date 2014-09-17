
#include "wormnet/wm_std.h"
#include "wormdb/DbResult.h"

namespace worm
{

CDbResult::CDbResult()
{
    m_ppRow = NULL;
    m_dwRowCount -= m_dwRowCount;
    m_dwCurRow -= m_dwCurRow;
    m_dwFieldCount -= m_dwFieldCount;
}

CDbResult::~CDbResult()
{
    CleanUp();
}

void CDbResult::CleanUp()
{
    for (uint32 i=0; i!=m_dwRowCount; ++i)
    {
        delete m_ppRow[i];
        m_ppRow[i] = NULL;
    }

    m_dwRowCount -= m_dwRowCount;
    delete []m_ppRow;
    m_ppRow = NULL;
}

bool CDbResult::NextRow()
{
    ++m_dwCurRow;

    if (m_dwCurRow >= m_dwRowCount)
    {
        --m_dwCurRow;
        return false;
    }
    else
    {
        return true;
    }
}

CDbField* CDbResult::Fetch() const
{
    if (m_dwCurRow == m_dwRowCount)
    {
        return NULL;
    }

    return m_ppRow[m_dwCurRow]->GetFields();
}

}
