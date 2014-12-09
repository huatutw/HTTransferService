

struct DG_INI
{
	int MajorVersion;
	int MinorVersion;
	char ServerName[_MAX_PATH];
	char ServerIP[_MAX_PATH];
	char UpdateServer[_MAX_PATH];
	char UpdateServerIP[_MAX_PATH];
	char LogServer[_MAX_PATH];
	char LogServerIP[_MAX_PATH];
	char BackupServer[_MAX_PATH];
	char BackupServerIP[_MAX_PATH];
	int ServerPort;
	int UpdatePort;
	int LogPort;
	int BackupServerPort;
	char ClientUsername[_MAX_PATH];
	char ClientPassword[_MAX_PATH];
};