
#include "FlashPolicy/PolicySocketHandler.h"
#include <wormnet/ByteBuffer.h>
#include <string.h>
#include "FlashPolicy/PolicyApp.h"

worm::CByteBuffer g_oResponPacket;

CPolicySocketHandler::CPolicySocketHandler()
{

}

CPolicySocketHandler::~CPolicySocketHandler()
{

}

void CPolicySocketHandler::OnAccept()
{
    m_qwStartTime = g_pPolicyApp->GetServerTime();
    g_pPolicyApp->AddSocket(this);
}

void CPolicySocketHandler::OnClose()
{
    g_pPolicyApp->DeleteSocket(this);
}

/// …≥œ‰—È÷§«Î«Û
#define FLASH_POLICY_REQUEST "<policy-file-request/>"

void CPolicySocketHandler::OnRead(void* pData, const uint16& wSize)
{
    static worm::CByteBuffer oRespon;
    if (NULL != strstr((char*)pData, "<policy-file-request/>"))
    {
        g_pPolicyApp->GetServer()->Send(g_oResponPacket, this->Socket());
    }
    else
    {
        g_pPolicyApp->GetServer()->CloseConnection(this->Socket());
    }
}
