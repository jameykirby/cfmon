// cfmon.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include "CBFSFlt.h"

LPCSTR g_Guid = "18189531-48ED-43fc-8582-E8BD2F9AB533";
CBFSFilter g_CBFSFlt;
UINT ALTITUDE_FAKE_VALUE_FOR_DEBUG = 360000;

void CBFSFltCreateFileN(CBFSFilter* Sender, LPWSTR FileName,
	ACCESS_MASK DesiredAccess, DWORD FileAttributes, WORD ShareMode,
	DWORD CreateOptions, WORD CreateDisposition, LONG Status) {
	_tprintf(_T("CBFSFltCreateFileN %s\n"), FileName);
}

void CBFSFltOpenFileN(CBFSFilter* Sender, LPWSTR FileName,
	ACCESS_MASK DesiredAccess, DWORD FileAttributes, WORD ShareMode,
	DWORD CreateOptions, WORD CreateDisposition, LONG Status) {
	_tprintf(_T("CBFSFltOpenFileN %s\n"), FileName);
}

void CBFSFltSetAllocationSizeN(CBFSFilter* Sender, LPWSTR FileName,
	__int64 AllocationSize, LONG Status) {
	_tprintf(_T("CBFSFltSetAllocationSizeN %s\n"), FileName);
}

void CBFSFltSetEndOfFileN(CBFSFilter* Sender, LPWSTR FileName,
	__int64 EndOfFile, LONG Status) {
	_tprintf(_T("CBFSFltSetEndOfFileN %s\n"), FileName);
}

void CBFSFltSetFileAttributesN(CBFSFilter* Sender, LPWSTR FileName,
	PFILETIME CreationTime, PFILETIME LastAccessTime, PFILETIME LastWriteTime,
	DWORD FileAttributes, LONG Status) {
	_tprintf(_T("CBFSFltSetFileAttributesN %s\n"), FileName);
}

void CBFSFltCanFileBeDeletedN(CBFSFilter* Sender, LPWSTR FileName,
	BOOL DeleteFile, LONG Status) {
	_tprintf(_T("CBFSFltCanFileBeDeletedN %s\n"), FileName);
}

void CBFSFltRenameOrMoveFileN(CBFSFilter* Sender, LPWSTR FileName,
	LPCWSTR NewFileName, LONG Status) {
	_tprintf(_T("CBFSFltRenameOrMoveFileN %s\n"), FileName);
}

void CBFSFltCreateHardLinkN(CBFSFilter* Sender, LPWSTR FileName,
	LPCWSTR LinkName, LONG Status) {
	_tprintf(_T("CBFSFltCreateHardLinkN %s\n"), FileName);
}

void CBFSFltReadFileN(CBFSFilter* Sender, LPWSTR FileName, __int64 Position,
	DWORD BytesToRead, LONG Status) {
	_tprintf(_T("CBFSFltReadFileN %s\n"), FileName);
}

void CBFSFltWriteFileN(CBFSFilter* Sender, LPWSTR FileName, __int64 Position,
	DWORD BytesToWrite, LONG Status) {
	_tprintf(_T("CBFSFltWriteFileN %s\n"), FileName);
}

void CBFSFltEnumerateDirectoryN(CBFSFilter* Sender, LPCWSTR DirectoryName,
	LPCWSTR FileName, FILETIME CreationTime, FILETIME LastAccessTime,
	FILETIME LastWriteTime, __int64 EndOfFile, __int64 AllocationSize,
	__int64 FileId, DWORD FileAttributes, LONG Status) {
	WORD len = (WORD)wcslen(DirectoryName) + (WORD)wcslen(FileName) + 2;
	LPWSTR buf = (LPWSTR)malloc(len * sizeof(WCHAR));
	wcscpy_s(buf, len, DirectoryName);
	wcscat_s(buf, len, L"\\");
	wcscat_s(buf, len, FileName);
	_tprintf(_T("CBFSFltEnumerateDirectoryN %s\n"), buf);
	free(buf);
}

void CBFSFltCloseFileN(CBFSFilter* Sender, LPCWSTR FileName) {
	_tprintf(_T("CBFSFltCloseFileN %s\n"), FileName);
}

void CBFSFltSetFileSecurityN(CBFSFilter* Sender, LPCWSTR FileName,
	SECURITY_INFORMATION SecurityInformation,
	PSECURITY_DESCRIPTOR SecurityDescriptor, DWORD Length, LONG Status) {
	_tprintf(_T("CBFSFltSetFileSecurityN %s\n"), FileName);
}

void CBFSFltCloseEnumeration(CBFSFilter* Sender, LPCWSTR DirectoryName,
	PVOID EnumerationContext) {
	_tprintf(_T("CBFSFltCloseEnumeration %s\n"), DirectoryName);
}

void InstallDriver(void) {
	BOOL reboot = FALSE;
	WCHAR PathToInstall[MAX_PATH];
	GetSystemDirectory(PathToInstall, MAX_PATH);
	try {
		CBFSFilter::Install(L"cbfsfilter.cab", g_Guid, PathToInstall,
			CBFSFilter::LegacyMode, ALTITUDE_FAKE_VALUE_FOR_DEBUG, &reboot);
		if (reboot) {
			_tprintf(_T("Reboot computer to apply changes\n"));
		}
	}
	catch (ECBFSFilterError err) {
		if (err.ErrorCode() == ERROR_ACCESS_DENIED) {
			_tprintf(_T("Installation requires administrator rights; run the app as administrator\n"));
		}
	}
}

void UninstallDriver(void) {
	BOOL reboot = FALSE;
	try {
		CBFSFilter::Uninstall(L"cbfsfilter.cab", g_Guid, NULL, &reboot);
		if (reboot) {
			_tprintf(_T("Reboot computer to apply changes\n"));
		}
	}
	catch (ECBFSFilterError err) {
		if (err.ErrorCode() == ERROR_ACCESS_DENIED) {
			_tprintf(_T("Installation requires administrator rights; run the app as administrator\n"));
		}
	}
}

_TCHAR* optarg = NULL;
int optind = 1;

int getopt(int argc, _TCHAR *const argv[], const _TCHAR *optstring) {
	if ((optind >= argc) || (argv[optind][0] != '-') || (argv[optind][0] == 0)) {
		return -1;
	}
	int opt = argv[optind][1];
	const _TCHAR *p = _tcschr(optstring, opt);
	if (p == NULL) {
		return '?';
	}
	if (p[1] == ':') {
		optind++;
		if (optind >= argc) {
			return '?';
		}
		optarg = argv[optind];
		optind++;
	}
	return opt;
}

int _tmain(int argc, _TCHAR* argv[]) {
	char *regKey =
		"C7C1CC906E5FBECE414ED31045B29734D941229F3005A7C5119EA3605FDDE761179462AE4"
		"12C784D46607CA3021E0D50E4944C580D5D67E53B54C787CB25AC8340F5E21F7CD15E62C7"
		"E4F9CE63289DC6BBB0E5CADF34C90A1F7409768B00B526DBD005D62BA055EA3F9429F60A2"
		"744A5228F6CB5927FDC39B75CB13AEE7B789D7A17B4615FB449A237DC313E73B8ADC21653"
		"90D917BC11BEF2DF3CB937DC31BEC38035DF8A";

	BOOL driverInstalled;
	int versionLow, versionHigh;
	SERVICE_STATUS serviceStatus;
	g_CBFSFlt.GetDriverStatus(g_Guid, &driverInstalled, &versionHigh, &versionLow,
		&serviceStatus);

	int opt = getopt(argc, argv, _T("iuf"));
	switch (opt) {
	case 'i':
		if (!driverInstalled) {
			_tprintf(_T("Installing driver\n"));
			InstallDriver();
		}
		else {
			_tprintf(_T("Driver already installed\n"));
		}
		return 0;
	case 'u':
		if (driverInstalled) {
			_tprintf(_T("Uninstalling driver\n"));
			UninstallDriver();
		}
		else {
			_tprintf(_T("Driver not installed\n"));
		}
		return 0;
	case 'f':
		if (driverInstalled) {
			if (serviceStatus.dwCurrentState == SERVICE_RUNNING) {
				_tprintf(_T("Driver is running\n"));
				break;
			}
			else {
				_tprintf(_T("Driver is not running %d\n"),
					serviceStatus.dwCurrentState);
				return 0;
			}
		}
		else {
			_tprintf(_T("Install driver first with %s -i\n"), argv[0]);
			return 0;
		}
		break;
	default:
		_tprintf(_T("Usage: %s [-iuf]\n"), argv[0]);
		return 0;
	}

	g_CBFSFlt.SetRegistrationKey(regKey);
	g_CBFSFlt.SetOnCreateFileN(CBFSFltCreateFileN);
	g_CBFSFlt.SetOnSetAllocationSizeN(CBFSFltSetAllocationSizeN);
	g_CBFSFlt.SetOnSetEndOfFileN(CBFSFltSetEndOfFileN);
	g_CBFSFlt.SetOnSetFileAttributesN(CBFSFltSetFileAttributesN);
	g_CBFSFlt.SetOnCanFileBeDeletedN(CBFSFltCanFileBeDeletedN);
	g_CBFSFlt.SetOnRenameOrMoveFileN(CBFSFltRenameOrMoveFileN);
	g_CBFSFlt.SetOnCreateHardLinkN(CBFSFltCreateHardLinkN);
	g_CBFSFlt.SetOnReadFileN(CBFSFltReadFileN);
	g_CBFSFlt.SetOnWriteFileN(CBFSFltWriteFileN);
	g_CBFSFlt.SetOnEnumerateDirectoryN(CBFSFltEnumerateDirectoryN);
	g_CBFSFlt.SetOnOpenFileN(CBFSFltOpenFileN);
	g_CBFSFlt.SetOnCloseFileN(CBFSFltCloseFileN);
	g_CBFSFlt.SetOnSetFileSecurityN(CBFSFltSetFileSecurityN);
	g_CBFSFlt.SetOnCloseEnumeration(CBFSFltCloseEnumeration);

	g_CBFSFlt.AddFilterRule(L"", CBFSFilter::NoCallback,
		(CBFSFilter::CBFSFltNotifyFlags)(
		CBFSFilter::ReadNotify |
		CBFSFilter::WriteNotify |
		CBFSFilter::CreateNotify |
		CBFSFilter::RenameNotify |
		CBFSFilter::CreateHardLinkNotify |
		CBFSFilter::SetSizesNotify |
		CBFSFilter::DeleteNotify |
		CBFSFilter::SetBasicInfoNotify |
		CBFSFilter::EnumerateDirectoryNotify |
		CBFSFilter::OpenNotify |
		CBFSFilter::CloseNotify |
		CBFSFilter::SetSecurityNotify
		));

	g_CBFSFlt.AddFilterRule(L"\\*.*", CBFSFilter::NoCallback,
		(CBFSFilter::CBFSFltNotifyFlags)(
		CBFSFilter::ReadNotify |
		CBFSFilter::WriteNotify |
		CBFSFilter::CreateNotify |
		CBFSFilter::RenameNotify |
		CBFSFilter::CreateHardLinkNotify |
		CBFSFilter::SetSizesNotify |
		CBFSFilter::DeleteNotify |
		CBFSFilter::SetBasicInfoNotify |
		CBFSFilter::EnumerateDirectoryNotify |
		CBFSFilter::OpenNotify |
		CBFSFilter::CloseNotify |
		CBFSFilter::SetSecurityNotify
		));

	try {
		g_CBFSFlt.SetResolveNtDeviceNameToDriveLetter(TRUE);
		g_CBFSFlt.SetProcessFailedRequests(FALSE);
		g_CBFSFlt.Initialize(g_Guid);
		g_CBFSFlt.AttachFilter(10000);
		g_CBFSFlt.SetReadWriteFileInPreCreatePath(FALSE);
		g_CBFSFlt.SetFlushFilesOnClose(FALSE);

		_tprintf(_T("Press any key to stop filtering...\n"));
		getc(stdin);
	}
	catch (ECBFSFilterError err) {
		_tprintf(_T("Exception caught\n"));
	}

	if (g_CBFSFlt.Active()) {
		g_CBFSFlt.DeleteAllFilterRules();
		g_CBFSFlt.DetachFilter();
	}
	else {
		_tprintf(_T("Filter not attached\n"));
	}

	return 0;
}

