#pragma once

#include "MessageDlgDef.h"

/*
dll包装类,方便调用dll.
*/

typedef int  (_cdecl *DlgInstance)(MSG_DLG_INFO info);
typedef int  (_cdecl *DlgAddMessageType)(int nMsgType, CString& str);
typedef void (_cdecl *DlgRemoveInfoByIndex)(int nIndex);
typedef void (_cdecl *DlgRemoveAllInfo)();
typedef void (_cdecl *DlgRemoveBubbleDlgByIndex)(int nIndex);
typedef void (_cdecl *DlgInitAprroveInfo)(TCHAR* szUser, TCHAR* szIniPath);

class CPackMessageDlg
{
public:
	CPackMessageDlg(void);

	~CPackMessageDlg(void);

	/*
	向DLL添加消息类型,DLL只能接受已经添加过的消息类型
	nType: 消息类型
	str:   对应显示字符串
	*/
	int AddMessageType(int nType, CString str);

	/*
	通过赋值MSG_DLG_INFO,向dll传入数据
	*/
	int AddMessage(MSG_DLG_INFO info);

	/*
	根据索引删除消息
	*/
	void RemoveInfoByIndex(int nIndex);

	/*
	根据索引删除bubble dlg消息
	*/
	void RemoveBubbleDlgByIndex(int nIndex);

	/*
	删除所有信息
	*/
	void RemoveAllInfo();

	/*
	设置审批初始化信息
	*/
	void InitApproveInfo(TCHAR* szUser, TCHAR* szIniPath);


private:

	void InitFunction(HMODULE hInst);

	HMODULE						m_hInst;

	DlgInstance					m_pDlgInstance;
	DlgAddMessageType			m_pDlgAddMessageType;
	DlgRemoveInfoByIndex		m_pDlgRemoveInfoByIndex;
	DlgRemoveAllInfo			m_pDlgRemoveAllInfo;
	DlgRemoveBubbleDlgByIndex	m_pRemoveBubbleDlgByIndex;
	DlgInitAprroveInfo			m_pInitAprroveInfo;
};
