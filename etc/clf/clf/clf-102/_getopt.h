// getopt.hを環境に合わせてインクルードする

#if !defined(___GETOPT_H)
#define ___GETOPT_H

// VC++の時
// ローカルで用意したgetopt.hをインクルードする
// その際、__STDC__が未定義の場合は一時的に定義してからインクルードする
#if defined(_MSC_VER)
#if defined(__STDC__)
#include "getopt.h"
#else
#define __STDC__	1
#include "getopt.h"
#undef __STDC__
#endif

// その他(CYGWIN, LINUX)
// 処理系標準のgetopt.hをインクルードする
#else
#include <getopt.h>

#endif

#endif
