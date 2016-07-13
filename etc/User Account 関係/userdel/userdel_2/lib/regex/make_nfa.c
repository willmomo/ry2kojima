#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "regexp.h"

/* 正規表現 オートマトン作成ルーチン 2001-12-25 */

/* エスケープ・キャラクタの定義 */
static struct ESCAPE_CHARA {
  char meta_chara;  /* メタキャラクタ */
  char *mask_chara; /* 対象文字 */
} escape_chara[] = {
  { 'd', "0-9", },
  { 'D', "^0-9", },
  { 'w', "a-zA-Z0-9", },
  { 'W', "^a-zA-Z0-9", },
  { 's', " \t\n", },

  { 'a', "\a", },
  { 'b', "\b", },
  { 'f', "\f", },
  { 'n', "\n", },
  { 'r', "\r", },
  { 't', "\t", },
  { 'v', "\v", },
  { 0, "", },
};

static char *any_chara = "^\n";

/* 文字パターンを0で埋める */
void clear_chara_mask( n )
NFA *n;
{
  int i;

  for ( i = 0 ; i < MASK_SIZE ; i++ )
    ( n->chara_mask )[i] = 0;
}

/* 文字パターンを1で埋める */
void fill_chara_mask( n )
NFA *n;
{
  int i;

  for ( i = 0 ; i < MASK_SIZE ; i++ )
    ( n->chara_mask )[i] = ~0;
}

/* 文字パターンの反転 */
void reverse_chara_mask( n )
NFA *n;
{
  int i;

  for ( i = 0 ; i < MASK_SIZE ; i++ )
    ( n->chara_mask )[i] = ~( n->chara_mask )[i];
}

/* 文字コードに対応する文字パターンのビットを立てる */
void calc_chara_mask( n, c )
NFA *n;
UCHAR c;
{
  int index;
  int mask_num;

  if ( c == 0 ) return;
  index = c / LINT_BIT_MAX;
  mask_num = c % LINT_BIT_MAX;

  if ( --mask_num < 0 ) {
    index--;
    mask_num = LINT_BIT_MAX - 1;
  }

  ( n->chara_mask )[index] |= 1 << mask_num;
}

/* 空文字遷移NFAの作成 */
void make_null_nfa( nfa, next0, next1 )
NFA *nfa,*next0,*next1;
{
  clear_chara_mask( nfa );
  ( nfa->next )[0] = next0;
  ( nfa->next )[1] = next1;
}

/* エスケープ系列用NFA作成

    |-----v
   (nfa) (nfa+1)
*/
NFA *escape_chara_nfa( nfa, nfa_end, p )
NFA *nfa,*nfa_end;
UCHAR *p;
{
  struct ESCAPE_CHARA *c;

  if ( nfa + 1 >= nfa_end ) return( NULL );

  /* エスケープ系列に該当するか */
  for ( c = escape_chara ; c->meta_chara != 0 ; c++ ) {
    if ( c->meta_chara == *p ) {
      nfa = chara_nfa( nfa, nfa_end, c->mask_chara, c->mask_chara + strlen( c->mask_chara ) );
      return( nfa );
    }
  }
  /* 該当なしの場合はキャラクタをそのまま処理 */
  nfa = chara_nfa( nfa, nfa_end, p, p + 1 );

  return( nfa );
}

/* 文字コード用NFA作成

    |-----v
   (nfa) (nfa+1)
*/
NFA *chara_nfa( nfa, nfa_end, s, e )
NFA *nfa,*nfa_end;
UCHAR *s,*e;
{
  UCHAR *p, c;
  int reverse_flg;

  if ( nfa + 1 >= nfa_end ) return( NULL );

  reverse_flg = ( *s == RE_CLASS_REVERSE ) ? *s++ : 0;
  make_null_nfa( nfa, nfa + 1, NULL ); /* 空文字遷移NFAとして初期化 */

  for ( p = s ; p < e ; p++ ) {
    /* 照合順序の処理 */
    if ( ( p - 1 ) >= s && ( p + 1 ) < e && *p == RE_CLASS_SEQUENCE ) {
      /* 'x-y-z'のチェック */
      if ( ( p + 3 ) < e && *( p + 2 ) == RE_CLASS_SEQUENCE ) return( NULL );
      /* 順序が逆ならエラー */
      if ( *( p - 1 ) > *( p + 1 ) ) return( NULL );
      for ( c = *( p - 1 ) + 1 ; c <= *( p + 1 ) ; c++ )
	calc_chara_mask( nfa, c );
      p++;
    } else
      calc_chara_mask( nfa, *p );
  }
  if ( reverse_flg != 0 )
    reverse_chara_mask( nfa );

  return( nfa + 1 );
}

/* NFAの要素を後ろにずらす

   (head ... tail)
    v

   (NFA * cnt) (head ... tail) (nfa)
                                |->return
*/
NFA *move2tail_nfa( head, tail, nfa_end, cnt )
NFA *head,*tail,*nfa_end;
int cnt;
{
  NFA *n;

  if ( tail + cnt + 1 >= nfa_end ) return( NULL );

  for ( n = tail + cnt ; n >= head + cnt ; n-- ) {
    *n = *( n - cnt );
    ( n->next )[0] = ( ( n->next )[0] != NULL ) ? ( n->next )[0] + cnt : NULL;
    ( n->next )[1] = ( ( n->next )[1] != NULL ) ? ( n->next )[1] + cnt : NULL;
  }

  return( tail + cnt + 1 );
}

/* NFAの要素を後ろにコピーする

   (head ... tail) (nfa)
    v

   (head ... tail) (head ... tail) * cnt (nfa)
                                          |->return
*/
NFA *copy2tail_nfa( head, tail, nfa, nfa_end, cnt )
NFA *head,*tail,*nfa,*nfa_end;
int cnt;
{
  NFA *n;

  while ( --cnt >= 0 ) {
    for ( n = head ; n <= tail ; n++ ) {
      *nfa = *n;
      ( nfa->next )[0] = ( ( n->next )[0] == NULL ) ? NULL : nfa + ( ( n->next )[0] - n );
      ( nfa->next )[1] = ( ( n->next )[1] == NULL ) ? NULL : nfa + ( ( n->next )[1] - n );
      if ( ++nfa >= nfa_end ) return( NULL );
    }
  }

  return( nfa );
}

/* 繰り返し(bound)用NFA作成

   (branch)
    v

   0回以上n回以下の繰り返し({,n})
    |---v-------------------------v
    |            |---v------------v
   (e) (branch) (e) (branch) ... (next)
       <------------------------>
                  n回

   m回以上n回以下の繰り返し({m,n};m>0,n>0)
                          |---v------------v
   (branch) (branch) ... (e) (branch) ... (next)
   <--------------------><--------------->
            m回               (n-m)回

   m回以上の繰り返し({m,})
                 v--------|---v------------v
   (branch) ... (branch) (e) (next)
   <-------------------->
            m回

   m回の繰り返し({m})
   (branch) ... (branch) (next)
   <-------------------->
            m回
*/
NFA *bound_nfa( nfa, nfa_end, head_nfa, min_time, max_time )
NFA *nfa,*nfa_end;
NFA *head_nfa; /* 直前のアトムの先頭NFA */
int min_time,max_time;
{
  NFA *copy_start, *copy_end; /* コピーするbranchの先頭と末尾 */
  NFA *branch_head;           /* 繰り返し制限が無い場合,末尾で無限ループさせるときの移動先 */
  int branch_length;          /* branchの長さ */ 
  int i;

  copy_start = head_nfa;
  copy_end = nfa - 1;
  branch_length = (int)(nfa - head_nfa);

  /* 0回以上の繰り返しならば,先頭に空分岐のためのNFAを挿入 */
  if ( min_time == 0 ) {
    if ( ( nfa = move2tail_nfa( head_nfa, nfa - 1, nfa_end, 1 ) ) == NULL ) return( NULL );
    branch_head = head_nfa + 1;
  /* そうでなければNFAを直列につなげる */
  } else {
    if ( ( nfa = copy2tail_nfa( copy_start, copy_end, nfa, nfa_end, min_time - 1 ) ) == NULL ) return( NULL );
    branch_head = nfa - branch_length;
  }

  if ( nfa >= nfa_end ) return( NULL );

  /* 繰り返し制限がない(max_time=0)ならば,末尾で無限に繰り返し */
  if ( max_time == 0 ) {
    make_null_nfa( nfa, branch_head, nfa + 1 );
    if ( ++nfa >= nfa_end ) return( NULL );
  /* そうでなければ,間にバイパスを入れながらNFAを残り回数だけ直列につなげる */
  } else {
    for ( i = ( min_time == 0 ) ? 1 : min_time ; i < max_time ; i++ ) {
      make_null_nfa( nfa, nfa + 1, nfa + ( branch_length + 1 ) * ( max_time - i ) );
      if ( ++nfa >= nfa_end ) return( NULL );
      if ( ( nfa = copy2tail_nfa( copy_start, copy_end, nfa, nfa_end, 1 ) ) == NULL ) return( NULL );
    }
  }

  if ( min_time == 0 )
    make_null_nfa( head_nfa, head_nfa + 1, nfa );

  return( nfa );
}

/* 論理和用NFA作成

   (nfa0 ...) (nfa1 ...) (nfa)
    v

    |--------v---------------------v        |---v   |---v
   (branch) (nfa0+1 ...) (nfa1+1) (nfa1+2 ...) (nfa+2) (nfa+3)
                      |---^    |------------------------^
*/
NFA *or_nfa( nfa, nfa_end, nfa0, nfa1 )
NFA *nfa,*nfa_end;
NFA *nfa0,*nfa1;
{
  if ( nfa + 3 >= nfa_end ) return( NULL );

  /* 各枝の先頭に空分岐のためのNFAを挿入 */
  move2tail_nfa( nfa1, nfa - 1, nfa_end, 2 );
  move2tail_nfa( nfa0, nfa1 - 1, nfa_end, 1 );

  /* branchに分岐先を代入 */
  make_null_nfa( nfa0, nfa0 + 1, nfa1 + 2 );

  /* 2つの枝の末尾の分岐先をnfa+3に */
  make_null_nfa( nfa1 + 1, nfa + 3, NULL );
  make_null_nfa( nfa + 2, nfa + 3, NULL );

  return( nfa + 3 );
}

/* ブラケット表現[...]の終端を見つける */
int find_class_end( p0, p1, e )
UCHAR **p0,**p1;
UCHAR *e;
{
  
  for ( *p0 = (*p1)++ ; *p1 < e && **p1 != RE_CLASS_END ; (*p1)++ );
  if ( **p1 != RE_CLASS_END ) return( 0 ); /* ']'で閉じていなかったら0を返す */

  /* 先頭に']'(または'^]')があった場合の対処 */
  if ( ( *p1 - *p0 ) == 1 || ( ( *p1 - *p0 ) == 2 && *( *p0 + 1 ) == RE_CLASS_REVERSE ) )
    for ( (*p1)++ ; *p1 < e && **p1 != RE_CLASS_END ; (*p1)++ );

  return( ( **p1 == RE_CLASS_END ) ? 1 : 0 ); /* ']'で閉じていなかったら0を返す */
}

/* グループ化(...)の終端を見つける */
int find_group_end( p0, p1, e )
UCHAR **p0,**p1;
UCHAR *e;
{
  int group_count = 0;

  for ( *p0 = (*p1)++ ; *p1 < e ; (*p1)++ ) {
    if ( **p1 == RE_GROUP_END )
      if ( --group_count < 0 ) break;
    if ( **p1 == RE_GROUP_START )
      group_count++;
  }    

  return( ( **p1 == RE_GROUP_END ) ? 1 : 0 ); /* ')'で閉じていなかったら0を返す */
}

/* boundのチェック */
UCHAR *check_bound( p, min_time, max_time, e )
UCHAR *p;
int *min_time, *max_time;
UCHAR *e;
{
  *min_time = *max_time = 0;

  for ( p++ ; *p != RE_BOUND_DELIMITOR && *p != RE_BOUND_END ; p++ ) {
    if ( ! ( p < e && isdigit( *p ) ) ) return( NULL );
    *min_time = *min_time * 10 + ( *p - '0' );
  }

  if ( *p++ == RE_BOUND_END ) {
    *max_time = *min_time;
    return( p - 1 );
  }

  for ( ; *p != RE_BOUND_END ; p++ ) {
    if ( ! ( p < e && isdigit( *p ) ) ) return( NULL );
    *max_time = *max_time * 10 + ( *p - '0' );
  }

  return( p );
}

/* 枝の構文解析 */
NFA *compile_branch( nfa, nfa_end, s, e )
NFA *nfa,*nfa_end;
UCHAR *s,*e;
{
  UCHAR *p0, *p1;
  NFA *head_nfa;
  int min_time, max_time;

  head_nfa = nfa;
  for ( p1 = s ; p1 < e ; p1++ ) {
    switch ( *p1 ) {
    case RE_ANY:
      head_nfa = nfa;
      if ( ( nfa = chara_nfa( nfa, nfa_end, any_chara, any_chara + strlen( any_chara ) ) ) == NULL ) return( NULL );
      break;
    case RE_REPEAT0:
      if ( ( nfa = bound_nfa( nfa, nfa_end, head_nfa, 0, 0 ) ) == NULL ) return( NULL );
      break;
    case RE_REPEAT1:
      if ( ( nfa = bound_nfa( nfa, nfa_end, head_nfa, 1, 0 ) ) == NULL ) return( NULL );
      break;
    case RE_REPEAT01:
      if ( ( nfa = bound_nfa( nfa, nfa_end, head_nfa, 0, 1 ) ) == NULL ) return( NULL );
      break;
    case RE_BOUND_START:
      if ( ( p1 = check_bound( p1, &min_time, &max_time, e ) ) == NULL ) return( NULL );
      if ( ( nfa = bound_nfa( nfa, nfa_end, head_nfa, min_time, max_time ) ) == NULL ) return( NULL );
      break;
    case RE_GROUP_START:
      if ( find_group_end( &p0, &p1, e ) == 0 ) return( NULL );
      head_nfa = nfa;
      if ( ( nfa = compile_nfa( nfa, nfa_end, p0 + 1, p1 ) ) == NULL ) return( NULL );
      break;
    case RE_CLASS_START:
      if ( find_class_end( &p0, &p1, e ) == 0 ) return( NULL );
      head_nfa = nfa;
      if ( ( nfa = chara_nfa( nfa, nfa_end, p0 + 1, p1 ) ) == NULL ) return( NULL );
      break;
    case RE_QUOTE:
      if ( ++p1 >= e ) return( NULL ); /* 末尾が'\'の場合はNULLを返す */
      head_nfa = nfa;
      if ( ( nfa = escape_chara_nfa( nfa, nfa_end, p1 ) ) == NULL ) return( NULL );
      break;
    default:
      head_nfa = nfa;
      if ( ( nfa = chara_nfa( nfa, nfa_end, p1, p1 + 1 ) ) == NULL ) return( NULL );
    }
  }

  return( nfa );
}

/* 構文解析ルーチン */
NFA *compile_nfa( nfa, nfa_end, s, e )
NFA *nfa,*nfa_end;
UCHAR *s,*e;
{
  UCHAR *p0,*p1;
  NFA *nfa0,*nfa1;

  nfa0 = nfa;
  nfa1 = NULL;
  for ( p1 = s ; p1 < e ; p1++ ) {
    switch ( *p1 ) {
    case RE_QUOTE:
      p1++;
      break;
    case RE_OR:
      if ( ( nfa = compile_branch( nfa, nfa_end, s, p1 ) ) == NULL ) return( NULL );
      if ( ( s = p1 + 1 ) > e ) return( nfa ); /* 末尾に'|'があった場合は無視 */
      if ( nfa1 != NULL )
	if ( ( nfa = or_nfa( nfa, nfa_end, nfa0, nfa1 ) ) == NULL ) return( NULL );
      nfa1 = nfa;
      break;
    case RE_GROUP_START:
      if ( find_group_end( &p0, &p1, e ) == 0 ) return( NULL );
    }
  }
  if ( ( nfa = compile_branch( nfa, nfa_end, s, e ) ) == NULL ) return( NULL );
  if ( nfa1 != NULL )
    if ( ( nfa = or_nfa( nfa, nfa_end, nfa0, nfa1 ) ) == NULL ) return( NULL );

  /* 何も処理されていなければNULLを返す */
  if ( nfa0 == nfa ) return( NULL );

  clear_chara_mask( nfa );
  ( nfa->next )[0] = ( nfa->next )[1] = NULL;

  return( nfa );
}
