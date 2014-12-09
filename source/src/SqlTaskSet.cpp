
#include "wormdb/SqlTaskSet.h"
#include "wormdb/SqlQueryTask.h"

namespace worm
{

CSqlTaskSet::CSqlTaskSet(const uint16& wSize, const bool& bTransaction)
: m_bTransaction(bTransaction)
{
    assert(0 != wSize);
    m_aTaskList = new std::vector<ISqlTask*>;
    m_aTaskList->resize(wSize);
    m_bSucess = false;

    for (uint32 i=0; i<wSize; ++i)
    {
        (*m_aTaskList)[i] = NULL;
    }
}

CSqlTaskSet::CSqlTaskSet(const CSqlTaskSet& oSet)
{
    if (this == &oSet)
    {
        return;
    }

    m_bTransaction = oSet.m_bTransaction;
    m_aTaskList = new std::vector<ISqlTask*>;
    *m_aTaskList = *(oSet.m_aTaskList);
}

CSqlTaskSet::~CSqlTaskSet()
{
    delete m_aTaskList;
    m_aTaskList = NULL;
}

void CSqlTaskSet::ClearTask()
{
	std::vector<ISqlTask*>::iterator iterTask = m_aTaskList->begin();
    for (; m_aTaskList->end()!=iterTask; ++iterTask)
    {
        delete *iterTask;
    }
}

void CSqlTaskSet::SetQuery(const uint16& wIndex, const char* pSql)
{
    assert(NULL != pSql);
    assert(wIndex < m_aTaskList->size());

    ISqlTask*& pTask = (*m_aTaskList)[wIndex];
    if (NULL != pTask)
    {
        delete pTask;
        pTask = NULL;
    }
    pTask = new CSqlQueryTask(pSql);
}

CDbResult*& CSqlTaskSet::GetResult(const uint8& byIndex)
{
    assert(byIndex < m_aTaskList->size());
    return (*m_aTaskList)[byIndex]->GetResult();
}

}
