#include "pch.h"

//--------------------------------------------------------------------

// デバッグ用コールバック関数。デバッグメッセージを出力する。
void ___outputLog(LPCTSTR text, LPCTSTR output)
{
	::OutputDebugString(output);
}

//--------------------------------------------------------------------

DECLARE_HOOK_PROC(BOOL, WINAPI, drawObjectText, (HDC dc, int x, int y, UINT options, LPCRECT rc, LPCSTR text, UINT c, CONST INT* dx));
IMPLEMENT_HOOK_PROC_NULL(BOOL, WINAPI, drawObjectText, (HDC dc, int x, int y, UINT options, LPCRECT rc, LPCSTR text, UINT c, CONST INT* dx))
{
	MY_TRACE(_T("drawObjectText(0x%08X, %d, %d, 0x%08X)\n"), dc, x, y, options);
	MY_TRACE_RECT2(rc[0]); // クリッピング矩形
	MY_TRACE_RECT2(rc[1]); // アイテム全体の矩形

	x = std::max(70, x);

	return true_drawObjectText(dc, x, y, options, rc, text, c, dx);
}

//--------------------------------------------------------------------

BOOL func_init(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("func_init()\n"));

	uintptr_t exedit = (uintptr_t)::GetModuleHandle(_T("exedit.auf"));
	MY_TRACE_HEX(exedit);

	true_drawObjectText = hookAbsoluteCall(exedit + 0x0003794B, hook_drawObjectText);
	MY_TRACE_HEX(true_drawObjectText);

	return TRUE;
}

BOOL func_exit(AviUtl::FilterPlugin* fp)
{
	MY_TRACE(_T("func_exit()\n"));

	return TRUE;
}

BOOL APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		{
			MY_TRACE(_T("DLL_PROCESS_ATTACH\n"));

			break;
		}
	case DLL_PROCESS_DETACH:
		{
			MY_TRACE(_T("DLL_PROCESS_DETACH\n"));

			break;
		}
	}

	return TRUE;
}

//--------------------------------------------------------------------

AviUtl::FilterPluginDLL* WINAPI GetFilterTable()
{
	LPCSTR name = "アイテム名常時表示";
	LPCSTR information = "アイテム名常時表示 1.0.0 by 蛇色";

	static AviUtl::FilterPluginDLL filter =
	{
		.flag =
			AviUtl::FilterPluginDLL::Flag::AlwaysActive |
			AviUtl::FilterPluginDLL::Flag::DispFilter |
			AviUtl::FilterPluginDLL::Flag::ExInformation,
		.x = 0,
		.y = 0,
		.name = name,
		.func_init = func_init,
		.func_exit = func_exit,
		.information = information,
	};

	return &filter;
}

//--------------------------------------------------------------------
