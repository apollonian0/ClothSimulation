#pragma once

#include <Windows.h>
#include <vector>
#include <string>

namespace win32_util
{
	struct ProcWinHandles
	{
		const DWORD proc_id;
		std::vector< HWND >& handles;

		ProcWinHandles( 
			DWORD _proc_id,
			std::vector< HWND >& _handles )	
			:	proc_id( _proc_id )
			,	handles( _handles )
		{}
	};

	inline BOOL CALLBACK processWinHandle( HWND hwnd, LPARAM lparam )
	{
		ProcWinHandles *proc_win_handles = 
			( ProcWinHandles* )lparam;

		DWORD win_proc_id;
		GetWindowThreadProcessId( hwnd, &win_proc_id );
		
		if( win_proc_id == proc_win_handles->proc_id )
			proc_win_handles->handles.push_back( hwnd );

		return TRUE;
	}

	inline void
	getWinHandles(
		DWORD proc_id,
		std::vector< HWND >& win_ids )
	{
		ProcWinHandles proc_handles( proc_id, win_ids );

		EnumWindows( processWinHandle, ( LPARAM )&proc_handles );
	}

	inline HWND
	getCurrentProcWinHandle()
	{
		std::vector< HWND > win_handles;

		getWinHandles( GetCurrentProcessId(), win_handles );

		if( win_handles.size() )
			return win_handles[ 0 ];	//	assume current process only owns one top-level window

		return 0;
	}

	inline void 
	showMessageBox( 
		char *msg,
		char *caption = "Error" )
	{
		HWND cur_proc_hwnd = getCurrentProcWinHandle();

		if( cur_proc_hwnd )
			MessageBoxA( cur_proc_hwnd, msg, caption, MB_OK );

	}
}