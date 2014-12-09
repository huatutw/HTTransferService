#ifndef _ADBC_COMMDEF_
#define _ADBC_COMMDEF_

// sql error
#define	ADBC_WM_SQLERROR					(WM_USER + 8902)		// 数据对象

#define ADBC_DBTYPE_NONE					(0)
#define ADBC_DBTYPE_MSSQL					(1)
#define ADBC_DBTYPE_ORACLE					(2)
#define ADBC_DBTYPE_ACCESS					(3)
#define ADBC_DBTYPE_UDLFILE					(4)

#define ADBC_STATE_CLOSED					0			//adOpenStatic
#define ADBC_STATE_OPEN						0x1
#define ADBC_STATE_CONNECTING				0x2
#define ADBC_STATE_EXECUTING				0x4
#define ADBC_STATE_FECTING					0x8

//是否自动连接
#define	ADBC_CONNECT_AUTO					(1)

#define	ADBC_EXCEPTION_P_NONE				(0)
#define ADBC_EXCEPTION_P_THROW				(1)
#define ADBC_EXCEPTION_P_SENDMSG			(2)
#define ADBC_EXCEPTION_P_MSGBOX				(3)

#define ADBC_EXCEPTION_CAUSE_NONE			(0)
#define ADBC_EXCEPTION_CAUSE_CONNECT		(1)

#define	ADBC_CONNECTDB_TYPE_NET				(1)
#define	ADBC_CONNECTDB_TYPE_LOGIN			(2)

// ConnectDBType 的数据说明：
// 1 ~ 4  CONNECTDB_TYPE_NET
// 5 ~ 8  CONNECTDB_TYPE_LOGIN
#define	ADBC_CONNECTDB_TYPE_NET				(1)
#define	ADBC_CONNECTDB_TYPE_LOGIN			(2)

#define	ADBC_CONNECTDB_NET_PIPE				(0)
#define	ADBC_CONNECTDB_NET_TCPIP			(1)
#define	ADBC_CONNECTDB_LOGIN_SQL			(0)
#define	ADBC_CONNECTDB_LOGIN_NT				(1)

enum AdbcDataType
{	
    adbcEmpty				= ADODB::adEmpty,
    adbcTinyInt				= ADODB::adTinyInt,
    adbcSmallInt			= ADODB::adSmallInt,
    adbcInteger				= ADODB::adInteger,
    adbcBigInt				= ADODB::adBigInt,
    adbcUnsignedTinyInt		= ADODB::adUnsignedTinyInt,
    adbcUnsignedSmallInt	= ADODB::adUnsignedSmallInt,
    adbcUnsignedInt			= ADODB::adUnsignedInt,
    adbcUnsignedBigInt		= ADODB::adUnsignedBigInt,
    adbcSingle				= ADODB::adSingle,
    adbcDouble				= ADODB::adDouble,
    adbcCurrency			= ADODB::adCurrency,
    adbcDecimal				= ADODB::adDecimal,
    adbcNumeric				= ADODB::adNumeric,
    adbcBoolean				= ADODB::adBoolean,
    adbcError				= ADODB::adError,
    adbcUserDefined			= ADODB::adUserDefined,
    adbcVariant				= ADODB::adVariant,
    adbcIDispatch			= ADODB::adIDispatch,
    adbcIUnknown			= ADODB::adIUnknown,
    adbcGUID				= ADODB::adGUID,
    adbcDate				= ADODB::adDate,
    adbcDBDate				= ADODB::adDBDate,
    adbcDBTime				= ADODB::adDBTime,
    adbcDBTimeStamp			= ADODB::adDBTimeStamp,
    adbcBSTR				= ADODB::adBSTR,
    adbcChar				= ADODB::adChar,
    adbcVarChar				= ADODB::adVarChar,
    adbcLongVarChar			= ADODB::adLongVarChar,
    adbcWChar				= ADODB::adWChar,
    adbcVarWChar			= ADODB::adVarWChar,
    adbcLongVarWChar		= ADODB::adLongVarWChar,
    adbcBinary				= ADODB::adBinary,
    adbcVarBinary			= ADODB::adVarBinary,
	adbcLongVarBinary		= ADODB::adLongVarBinary,
    adbcChapter				= ADODB::adChapter,
    adbcFileTime			= ADODB::adFileTime,
    adbcPropVariant			= ADODB::adPropVariant,
    adbcVarNumeric			= ADODB::adVarNumeric
};

enum AdbcParaDirection
{
	adbcParamUnknown = ADODB::adParamUnknown,
    adbcParamInput = ADODB::adParamInput,
    adbcParamOutput = ADODB::adParamOutput,
    adbcParamInputOutput = ADODB::adParamInputOutput,
    adbcParamReturnValue = ADODB::adParamReturnValue
};

enum AdbcConnectMode
{
	adbcModeUnknown			= ADODB::adModeUnknown,
    adbcModeRead			= ADODB::adModeRead,
    adbcModeWrite			= ADODB::adModeWrite,
    adbcModeReadWrit		= ADODB::adModeReadWrite,
    adbcModeShareDenyRead	= ADODB::adModeShareDenyRead,
    adbcModeShareDenyWrite	= ADODB::adModeShareDenyWrite,
    adbcModeShareExclusive	= ADODB::adModeShareExclusive,
    adbcModeShareDenyNone	= ADODB::adModeShareDenyNone
};

enum AdbcSchema
{
    adbcSchemaProviderSpecific = ADODB::adSchemaProviderSpecific,
    adbcSchemaAsserts = ADODB::adSchemaAsserts,
    adbcSchemaCatalog = ADODB::adSchemaCatalogs,
    adbcSchemaCharacterSets = ADODB::adSchemaCharacterSets,
    adbcSchemaCollations = ADODB::adSchemaCollations,
    adbcSchemaColumn = ADODB::adSchemaColumns,
    adbcSchemaCheckConstraints = ADODB::adSchemaCheckConstraints,
    adbcSchemaConstraintColumnUsage = ADODB::adSchemaConstraintColumnUsage,
    adbcSchemaConstraintTableUsage = ADODB::adSchemaConstraintTableUsage,
    adbcSchemaKeyColumnUsage = ADODB::adSchemaKeyColumnUsage,
    adbcSchemaReferentialContraints = ADODB::adSchemaReferentialContraints,
    adbcSchemaReferentialConstraints = ADODB::adSchemaReferentialConstraints,
    adbcSchemaTableConstraints = ADODB::adSchemaTableConstraints,
    adbcSchemaColumnsDomainUsage = ADODB::adSchemaColumnsDomainUsage,
    adbcSchemaIndexes = ADODB::adSchemaIndexes,
    adbcSchemaColumnPrivileges = ADODB::adSchemaColumnPrivileges,
    adbcSchemaTablePrivileges = ADODB::adSchemaTablePrivileges,
    adbcSchemaUsagePrivileges = ADODB::adSchemaUsagePrivileges,
    adbcSchemaProcedures = ADODB::adSchemaProcedures,
    adbcSchemaSchemata = ADODB::adSchemaSchemata,
    adbcSchemaSQLLanguages = ADODB::adSchemaSQLLanguages,
    adbcSchemaStatistics = ADODB::adSchemaStatistics,
    adbcSchemaTables = ADODB::adSchemaTables,
    adbcSchemaTranslations = ADODB::adSchemaTranslations,
    adbcSchemaProviderTypes = ADODB::adSchemaProviderTypes,
    adbcSchemaView = ADODB::adSchemaViews,
    adbcSchemaViewColumnUsage = ADODB::adSchemaViewColumnUsage,
    adbcSchemaViewTableUsage = ADODB::adSchemaViewTableUsage,
    adbcSchemaProcedureParameters = ADODB::adSchemaProcedureParameters,
    adbcSchemaForeignKeys = ADODB::adSchemaForeignKeys,
    adbcSchemaPrimaryKeys = ADODB::adSchemaPrimaryKeys,
    adbcSchemaProcedureColumns = ADODB::adSchemaProcedureColumns,
    adbcSchemaDBInfoKeywords = ADODB::adSchemaDBInfoKeywords,
    adbcSchemaDBInfoLiterals = ADODB::adSchemaDBInfoLiterals,
    adbcSchemaCubes = ADODB::adSchemaCubes,
    adbcSchemaDimensions = ADODB::adSchemaDimensions,
    adbcSchemaHierarchies = ADODB::adSchemaHierarchies,
    adbcSchemaLevels = ADODB::adSchemaLevels,
    adbcSchemaMeasures = ADODB::adSchemaMeasures,
    adbcSchemaProperties = ADODB::adSchemaProperties,
    adbcSchemaMembers = ADODB::adSchemaMembers,
    adbcSchemaTrustees = ADODB::adSchemaTrustees
};

enum AdbcConnectOption
{
	adbcConnectUnspecified = ADODB::adConnectUnspecified,
	adbcadAsyncConnect = ADODB::adAsyncConnect
};

enum AdbcCommandType
{
    adbcCmdUnspecified = ADODB::adCmdUnspecified,
    adbcCmdUnknown = ADODB::adCmdUnknown,
    adbcCmdText= ADODB::adCmdText,
    adbcCmdTable = ADODB::adCmdTable,
    adbcCmdStoredProc = ADODB::adCmdStoredProc,
    adbcCmdFile = ADODB::adCmdFile,
    adbcCmdTableDirect = ADODB::adCmdTableDirect
};

enum AdbcSearchDirection
{
    adbcSearchForward = ADODB::adSearchForward,
    adbcSearchBackward = ADODB::adSearchBackward
};

enum AdbcCursorType
{
    adbcOpenUnspecified = ADODB::adOpenUnspecified,
    adbcOpenForwardOnly = ADODB::adOpenForwardOnly,
    adbcOpenKeyset = ADODB::adOpenKeyset,
    adbcOpenDynamic = ADODB::adOpenDynamic,
    adbcOpenStatic = ADODB::adOpenStatic
};

enum AdbcLockType
{
    adbcLockUnspecified = ADODB::adLockUnspecified,
    adbcLockReadOnly = ADODB::adLockReadOnly,
    adbcLockPessimistic = ADODB::adLockPessimistic,
    adbcLockOptimistic = ADODB::adLockOptimistic,
    adbcLockBatchOptimistic = ADODB::adLockBatchOptimistic
};

enum AdbcAffect
{
    adbcAffectCurrent = ADODB::adAffectCurrent,
    adbcAffectGroup = ADODB::adAffectGroup,
    adbcAffectAll = ADODB::adAffectAll,
    adbcAffectAllChapters = ADODB::adAffectAllChapters
};

enum AdbcEditMode
{
    adbcEditNone = ADODB::adEditNone,
    adbcEditInProgress = ADODB::adEditInProgress,
    adbcEditAdd = ADODB::adEditAdd,
    adbcEditDelete = ADODB::adEditDelete
};

enum AdbcCursorLocation
{
    adbcUseNone = ADODB::adUseNone,
    adbcUseServer = ADODB::adUseServer,
    adbcUseClient = ADODB::adUseClient,
    adbcUseClientBatch = ADODB::adUseClientBatch
};

enum AdbcResync
{
    adbcResyncUnderlyingValues = ADODB::adResyncUnderlyingValues,
    adbcResyncAllValues = ADODB::adResyncAllValues
};

enum AdbcPersistFormat
{
    adbcPersistADTG = ADODB::adPersistADTG,
    adbcPersistXML = ADODB::adPersistXML
};

enum AdbcCursorOption
{
    adbcHoldRecords = ADODB::adHoldRecords,
    adbcMovePrevious = ADODB::adMovePrevious,
    adbcAddNew = ADODB::adAddNew,
    adbcDelete = ADODB::adDelete,
    adbcUpdate = ADODB::adUpdate,
    adbcBookmark = ADODB::adBookmark,
    adbcApproxPosition = ADODB::adApproxPosition,
    adbcUpdateBatch = ADODB::adUpdateBatch,
    adbcResync = ADODB::adResync,
    adbcNotify = ADODB::adNotify,
    adbcFind = ADODB::adFind,
    adbcSeek = ADODB::adSeek,
    adbcIndex = ADODB::adIndex
};

enum AdbcBookmark
{
    adbcBookmarkCurrent = ADODB::adBookmarkCurrent,
    adbcBookmarkFirst = ADODB::adBookmarkFirst,
    adbcBookmarkLast = ADODB::adBookmarkLast
};

enum AdbcObjectState
{
    adbcStateClosed = ADODB::adStateClosed,
    adbcStateOpen = ADODB::adStateOpen,
    adbcStateConnecting = ADODB::adStateConnecting,
    adbcStateExecuting = ADODB::adStateExecuting,
    adbcStateFetching = ADODB::adStateFetching
};

enum AdbcFieldAttribute
{
    adbcFldUnspecified = ADODB::adFldUnspecified,
    adbcFldMayDefer = ADODB::adFldMayDefer,
    adbcFldUpdatable = ADODB::adFldUpdatable,
    adbcFldUnknownUpdatable = ADODB::adFldUnknownUpdatable,
    adbcFldFixed = ADODB::adFldFixed,
    adbcFldIsNullable = ADODB::adFldIsNullable,
    adbcFldMayBeNull = ADODB::adFldMayBeNull,
    adbcFldLong = ADODB::adFldLong,
    adbcFldRowID = ADODB::adFldRowID,
    adbcFldRowVersion = ADODB::adFldRowVersion,
    adbcFldCacheDeferred = ADODB::adFldCacheDeferred,
    adbcFldNegativeScale = ADODB::adFldNegativeScale,
    adbcFldKeyColumn = ADODB::adFldKeyColumn
};

enum AdbcColAttributes
{
    adbcColFixed = ADOX::adColFixed,
    adbcColNullable = ADOX::adColNullable
};

#endif
