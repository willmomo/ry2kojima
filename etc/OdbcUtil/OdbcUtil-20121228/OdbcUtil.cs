// kjmlib series ... 2012/12/25
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Runtime.InteropServices;

namespace kjm {
	class OdbcUtil {

		//
		// SQLRETURN => SQLSMALLINT => short => short(C#)
		// SQLUSMALLINT => unsigned short => ushort(C#)
		// SQLHENV => SQLHANDLE => void* => IntPtr
		// SQLCHAR => unsigned char
		// SQLINTEGER => long => Int32
		// SQLPOINTER => void* => IntPtr
		//

		// SQLRETURN SQL_API SQLAllocEnv(SQLHENV *EnvironmentHandle);
		[DllImport("odbc32")]
		private static extern short SQLAllocEnv(out IntPtr EnvironmentHandle);

		// SQLRETURN SQL_API SQLAllocHandle(SQLSMALLINT HandleType, SQLHANDLE InputHandle, SQLHANDLE *OutputHandle);
		[DllImport("odbc32")]
		private static extern short SQLAllocHandle(short HandleType, IntPtr InputHandle, out IntPtr OutputHandle);

		// SQLRETURN SQL_API SQLFreeEnv(SQLHENV EnvironmentHandle);
		[DllImport("odbc32")]
		private static extern short SQLFreeEnv(IntPtr InputHandle);

		// SQLRETURN SQL_API SQLFreeHandle(SQLSMALLINT HandleType, SQLHANDLE Handle);
		[DllImport("odbc32")]
		private static extern short SQLFreeHandle(short HandleType, IntPtr Handle);

		[DllImport("odbc32", CharSet = CharSet.Ansi)]
		private static extern short SQLDataSources(IntPtr EnvironmentHandle, ushort Direction, StringBuilder ServerName, short BufferLength1, ref short NameLength1Ptr, StringBuilder Description, short BufferLength2, ref short NameLength2Ptr);

		// [Unicode]
		// SQLRETURN SQL_API SQLDriversW(SQLHENV henv, SQLUSMALLINT fDirection, 
		//		__out_ecount_opt(cchDriverDescMax) SQLWCHAR* szDriverDesc, SQLSMALLINT cchDriverDescMax, SQLSMALLINT* pcchDriverDesc,
		//		__out_ecount_opt(cchDrvrAttrMax) SQLWCHAR* szDriverAttributes, SQLSMALLINT cchDrvrAttrMax, SQLSMALLINT* pcchDrvrAttr);
		//
		// [ANSI]
		// SQLRETURN SQL_API SQLDrivers(SQLHENV henv, SQLUSMALLINT fDirection,
		//		SQLCHAR *szDriverDesc, SQLSMALLINT cbDriverDescMax, SQLSMALLINT *pcbDriverDesc,
		//		SQLCHAR *szDriverAttributes, SQLSMALLINT cbDrvrAttrMax, SQLSMALLINT *pcbDrvrAttr);
		[DllImport("odbc32", CharSet = CharSet.Auto)]
		private static extern short SQLDrivers(IntPtr henv, ushort fDirection,
			StringBuilder szDriverDesc, short cbDriverDescMax, ref short pcbDriverDesc,
			StringBuilder szDriverAttributes, short cbDrvrAttrMax, ref short pcbDrvrAttr);

		// SQLRETURN SQL_API SQLSetEnvAttr(SQLHENV EnvironmentHandle, SQLINTEGER Attribute, SQLPOINTER Value, SQLINTEGER StringLength);
		[DllImport("odbc32")]
		private static extern short SQLSetEnvAttr(IntPtr EnvironmentHandle, Int32 Attribute, IntPtr Value, Int32 StringLength);

		// handle type identifiers (SQLSMALLINT)
		private const short SQL_HANDLE_ENV = 1;

		// Codes used for FetchOrientation (SQLUSMALLINT)
		private const ushort SQL_FETCH_NEXT = 1;
		private const ushort SQL_FETCH_FIRST = 2;

		// env attribute (SQLINTEGER)
		private const Int32 SQL_ATTR_ODBC_VERSION = 200;

		// return values from functions (SQLRETURN)
		private const short SQL_SUCCESS = 0;

		//-------------------------------------------------------------
		// インストール済みのODBCドライバ一覧を取得
		//-------------------------------------------------------------
		public static List<string> GetDriverList() {
			List<string> result = new List<string>();

			IntPtr henv;
			short ret = SQLAllocHandle(SQL_HANDLE_ENV, IntPtr.Zero, out henv);
			if (ret == SQL_SUCCESS) {
				IntPtr SQL_OV_ODBC3 = (IntPtr)3;
				SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, SQL_OV_ODBC3, 0);

				StringBuilder desc = new StringBuilder(1024);
				StringBuilder attr = new StringBuilder(1024);
				short descLen = 0;
				short attrLen = 0;

				ret = SQLDrivers(henv, SQL_FETCH_FIRST, desc, 1024, ref descLen, attr, 1024, ref attrLen);
				while (ret == 0) {
					result.Add(desc.ToString());
					ret = SQLDrivers(henv, SQL_FETCH_NEXT, desc, 1024, ref descLen, attr, 1024, ref attrLen);
				}

				ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);
			}

			return result;
		}

		//-------------------------------------------------------------
		// インストール済みのODBCドライバ一覧を取得(ODBC2.0版)
		//-------------------------------------------------------------
		public static List<string> GetDriverList_ODBCv2() {
			List<string> result = new List<string>();

			IntPtr henv;
			short ret = SQLAllocEnv(out henv);
			if (ret == SQL_SUCCESS) {
				StringBuilder desc = new StringBuilder(1024);
				StringBuilder attr = new StringBuilder(1024);
				short descLen = 0;
				short attrLen = 0;

				ret = SQLDrivers(henv, SQL_FETCH_FIRST, desc, 1024, ref descLen, attr, 1024, ref attrLen);
				while (ret == 0) {
					result.Add(desc.ToString());
					ret = SQLDrivers(henv, SQL_FETCH_NEXT, desc, 1024, ref descLen, attr, 1024, ref attrLen);
				}

				ret = SQLFreeEnv(henv);
			}

			return result;
		}

		//-------------------------------------------------------------
		// インストール済みのドライバを名前(正規表現)で検索
		//
		// 見つからない時は、nullを返す。
		//-------------------------------------------------------------
		public static string FindDriver(string pattern) {
			return kjm.OdbcUtil.GetDriverList().Find(delegate(string s) {
				return Regex.IsMatch(s, pattern, RegexOptions.IgnoreCase);
			});
		}

		//-------------------------------------------------------------
		// 指定されたドライバがインストール済みかどうか調べる
		//-------------------------------------------------------------
		public static bool IsInstalled(string driverName) {
			string driverNameUC = driverName.ToUpper();
			return (kjm.OdbcUtil.GetDriverList().FindIndex(delegate(string s) {
				return (s.ToUpper().CompareTo(driverNameUC) == 0);
			}) >= 0);
		}
	}
}
