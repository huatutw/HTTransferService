#pragma once

#define safe_delete(p)  if (p != NULL) {	delete  p; 	p = NULL;}

//显示对话框类型
enum Dlg_Type{

	dlg_bubble = 0,//气泡
	dlg_linker,//连接
	dlg_ok,//确定
	dlg_okcancel,//确定取消
	dlg_yesnolater,
};

//加载ICON类型
enum MDD_ICON_TYPE
{
	icon_type_warning,
	icon_type_error,
	icon_type_linker,
	icon_type_infomation,
	icon_type_question,
};

//回送消息时的操作类型
typedef	struct __Dlg_Operate_Type
{

	int onOk ;//点击了确定
	int onCancel;//点击了取消
	int onLater;//稍候点击

}Mdd_Operate_Type;

typedef struct __Dlg_InitInfo
{
	__Dlg_InitInfo()
	{

		bModal = false;
		nElapse = 4000;
		hWnd = 0;
		nIconType=icon_type_warning;
		nIndex = 0;
		nMsgCount = 0;
		nApproveId = -1;
		OperateType.onOk =0;
		OperateType.onCancel =1;
		memset(strInfo,0,sizeof(strInfo));
		memset(strUrl,0,sizeof(strUrl));

	}
	bool bModal;//是否模态
	int nMsgCount;//自定义消息条数
	int nDlgType;//对话框类型
	int nIconType;
	int nMessageType;//消息类型
	int nElapse;//如果对话框为dlg_bubble,需要定义该参数2000.
	int nIndex;//结构体索引,无需赋值,dll内部传递时使用.
	int nApproveId;
	HWND hWnd;// 窗体句柄
	Mdd_Operate_Type	OperateType;
	TCHAR strInfo[1024];//文档字符串
	TCHAR strUrl[1024];//超链接地址

}MSG_DLG_INFO;