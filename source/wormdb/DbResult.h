
#ifndef _WORMNET_DBI_DB_RESULT_SET_H
#define _WORMNET_DBI_DB_RESULT_SET_H

#include <assert.h>
#include "wormnet/wm_types.h"
#include "wormdb/DbRow.h"

namespace worm
{

class WORM_DLL_EXPORT CDbResult
{
public:
    //CDbResult(void* const pResult, const uint64& qwRowCount, const uint32& dwFieldCount);
    virtual ~CDbResult();

    bool NextRow();
    ///   
    /// 获取一条记录
    /// @return 当前记录指针
    /// @note 可能为NULL
    /// 
    CDbField* Fetch() const;

    ///   
    /// 获取记录数量
    /// @return >=0
    /// 
    uint64 GetRowCount() const { return m_dwRowCount; }

protected:
    CDbResult();
    ///   
    /// 释放内存
    /// @see
    /// @note 子类必须要调用父类函数
    /// 
    virtual void CleanUp();

protected:
    CDbRow**        m_ppRow;
    uint32          m_dwFieldCount;
    uint32          m_dwRowCount;
    uint32          m_dwCurRow;
};

}

#endif // ifndef _WORMNET_DBI_DB_RESULT_SET_H
