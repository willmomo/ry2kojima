#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "regexp.h"

/* ����ɽ�� �����ȥޥȥ�����롼���� 2001-12-25 */

/* ���������ס�����饯������� */
static struct ESCAPE_CHARA {
  char meta_chara;  /* �᥿����饯�� */
  char *mask_chara; /* �о�ʸ�� */
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

/* ʸ���ѥ������0������ */
void clear_chara_mask( n )
NFA *n;
{
  int i;

  for ( i = 0 ; i < MASK_SIZE ; i++ )
    ( n->chara_mask )[i] = 0;
}

/* ʸ���ѥ������1������ */
void fill_chara_mask( n )
NFA *n;
{
  int i;

  for ( i = 0 ; i < MASK_SIZE ; i++ )
    ( n->chara_mask )[i] = ~0;
}

/* ʸ���ѥ������ȿž */
void reverse_chara_mask( n )
NFA *n;
{
  int i;

  for ( i = 0 ; i < MASK_SIZE ; i++ )
    ( n->chara_mask )[i] = ~( n->chara_mask )[i];
}

/* ʸ�������ɤ��б�����ʸ���ѥ�����ΥӥåȤ�Ω�Ƥ� */
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

/* ��ʸ������NFA�κ��� */
void make_null_nfa( nfa, next0, next1 )
NFA *nfa,*next0,*next1;
{
  clear_chara_mask( nfa );
  ( nfa->next )[0] = next0;
  ( nfa->next )[1] = next1;
}

/* ���������׷�����NFA����

    |-----v
   (nfa) (nfa+1)
*/
NFA *escape_chara_nfa( nfa, nfa_end, p )
NFA *nfa,*nfa_end;
UCHAR *p;
{
  struct ESCAPE_CHARA *c;

  if ( nfa + 1 >= nfa_end ) return( NULL );

  /* ���������׷���˳������뤫 */
  for ( c = escape_chara ; c->meta_chara != 0 ; c++ ) {
    if ( c->meta_chara == *p ) {
      nfa = chara_nfa( nfa, nfa_end, c->mask_chara, c->mask_chara + strlen( c->mask_chara ) );
      return( nfa );
    }
  }
  /* �����ʤ��ξ��ϥ���饯���򤽤Τޤ޽��� */
  nfa = chara_nfa( nfa, nfa_end, p, p + 1 );

  return( nfa );
}

/* ʸ����������NFA����

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
  make_null_nfa( nfa, nfa + 1, NULL ); /* ��ʸ������NFA�Ȥ��ƽ���� */

  for ( p = s ; p < e ; p++ ) {
    /* �ȹ����ν��� */
    if ( ( p - 1 ) >= s && ( p + 1 ) < e && *p == RE_CLASS_SEQUENCE ) {
      /* 'x-y-z'�Υ����å� */
      if ( ( p + 3 ) < e && *( p + 2 ) == RE_CLASS_SEQUENCE ) return( NULL );
      /* ������դʤ饨�顼 */
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

/* NFA�����Ǥ���ˤ��餹

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

/* NFA�����Ǥ���˥��ԡ�����

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

/* �����֤�(bound)��NFA����

   (branch)
    v

   0��ʾ�n��ʲ��η����֤�({,n})
    |---v-------------------------v
    |            |---v------------v
   (e) (branch) (e) (branch) ... (next)
       <------------------------>
                  n��

   m��ʾ�n��ʲ��η����֤�({m,n};m>0,n>0)
                          |---v------------v
   (branch) (branch) ... (e) (branch) ... (next)
   <--------------------><--------------->
            m��               (n-m)��

   m��ʾ�η����֤�({m,})
                 v--------|---v------------v
   (branch) ... (branch) (e) (next)
   <-------------------->
            m��

   m��η����֤�({m})
   (branch) ... (branch) (next)
   <-------------------->
            m��
*/
NFA *bound_nfa( nfa, nfa_end, head_nfa, min_time, max_time )
NFA *nfa,*nfa_end;
NFA *head_nfa; /* ľ���Υ��ȥ����ƬNFA */
int min_time,max_time;
{
  NFA *copy_start, *copy_end; /* ���ԡ�����branch����Ƭ������ */
  NFA *branch_head;           /* �����֤����¤�̵�����,������̵�¥롼�פ�����Ȥ��ΰ�ư�� */
  int branch_length;          /* branch��Ĺ�� */ 
  int i;

  copy_start = head_nfa;
  copy_end = nfa - 1;
  branch_length = (int)(nfa - head_nfa);

  /* 0��ʾ�η����֤��ʤ��,��Ƭ�˶�ʬ���Τ����NFA������ */
  if ( min_time == 0 ) {
    if ( ( nfa = move2tail_nfa( head_nfa, nfa - 1, nfa_end, 1 ) ) == NULL ) return( NULL );
    branch_head = head_nfa + 1;
  /* �����Ǥʤ����NFA��ľ��ˤĤʤ��� */
  } else {
    if ( ( nfa = copy2tail_nfa( copy_start, copy_end, nfa, nfa_end, min_time - 1 ) ) == NULL ) return( NULL );
    branch_head = nfa - branch_length;
  }

  if ( nfa >= nfa_end ) return( NULL );

  /* �����֤����¤��ʤ�(max_time=0)�ʤ��,������̵�¤˷����֤� */
  if ( max_time == 0 ) {
    make_null_nfa( nfa, branch_head, nfa + 1 );
    if ( ++nfa >= nfa_end ) return( NULL );
  /* �����Ǥʤ����,�֤˥Х��ѥ�������ʤ���NFA��Ĥ�������ľ��ˤĤʤ��� */
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

/* ��������NFA����

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

  /* �ƻޤ���Ƭ�˶�ʬ���Τ����NFA������ */
  move2tail_nfa( nfa1, nfa - 1, nfa_end, 2 );
  move2tail_nfa( nfa0, nfa1 - 1, nfa_end, 1 );

  /* branch��ʬ��������� */
  make_null_nfa( nfa0, nfa0 + 1, nfa1 + 2 );

  /* 2�Ĥλޤ�������ʬ�����nfa+3�� */
  make_null_nfa( nfa1 + 1, nfa + 3, NULL );
  make_null_nfa( nfa + 2, nfa + 3, NULL );

  return( nfa + 3 );
}

/* �֥饱�å�ɽ��[...]�ν�ü�򸫤Ĥ��� */
int find_class_end( p0, p1, e )
UCHAR **p0,**p1;
UCHAR *e;
{
  
  for ( *p0 = (*p1)++ ; *p1 < e && **p1 != RE_CLASS_END ; (*p1)++ );
  if ( **p1 != RE_CLASS_END ) return( 0 ); /* ']'���Ĥ��Ƥ��ʤ��ä���0���֤� */

  /* ��Ƭ��']'(�ޤ���'^]')�����ä������н� */
  if ( ( *p1 - *p0 ) == 1 || ( ( *p1 - *p0 ) == 2 && *( *p0 + 1 ) == RE_CLASS_REVERSE ) )
    for ( (*p1)++ ; *p1 < e && **p1 != RE_CLASS_END ; (*p1)++ );

  return( ( **p1 == RE_CLASS_END ) ? 1 : 0 ); /* ']'���Ĥ��Ƥ��ʤ��ä���0���֤� */
}

/* ���롼�ײ�(...)�ν�ü�򸫤Ĥ��� */
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

  return( ( **p1 == RE_GROUP_END ) ? 1 : 0 ); /* ')'���Ĥ��Ƥ��ʤ��ä���0���֤� */
}

/* bound�Υ����å� */
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

/* �ޤι�ʸ���� */
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
      if ( ++p1 >= e ) return( NULL ); /* ������'\'�ξ���NULL���֤� */
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

/* ��ʸ���ϥ롼���� */
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
      if ( ( s = p1 + 1 ) > e ) return( nfa ); /* ������'|'�����ä�����̵�� */
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

  /* �����������Ƥ��ʤ����NULL���֤� */
  if ( nfa0 == nfa ) return( NULL );

  clear_chara_mask( nfa );
  ( nfa->next )[0] = ( nfa->next )[1] = NULL;

  return( nfa );
}
