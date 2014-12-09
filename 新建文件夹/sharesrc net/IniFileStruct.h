

struct DG_INI
{
	int MajorVersion;
	int MinorVersion;
	char ServerName[_MAX_PATH];
	char ServerIP[_MAX_PATH];
	char UpdateServer[_MAX_PATH];
	char UpdateServerIP[_MAX_PATH];
	int ServerPort;
	int UpdatePort;
};