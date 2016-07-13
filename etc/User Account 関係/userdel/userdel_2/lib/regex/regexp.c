#include <stdlib.h>
#include <stdio.h>
#include "regexp.h"

/* ����ɽ�� ʸ����ȹ�롼���� 2001-12-25 */

/* �����ܤǤ��뤫������å����� */
int isnull_chara_mask( n )
NFA *n;
{
  int i;

  for ( i = 0 ; i < MASK_SIZE ; i++ )
    if ( ( n->chara_mask )[i] != 0 ) return( 0 );

  return( 1 );
}

/* ʸ���ȹ�롼���� */
int check_chara( n, c )
NFA *n;
UCHAR c;
{
  int index;
  int mask_num;

  if ( c == 0 ) return( 0 );
  index = c / LINT_BIT_MAX;
  mask_num = c % LINT_BIT_MAX;

  if ( --mask_num < 0 ) {
    index--;
    mask_num = LINT_BIT_MAX - 1;
  }

  return( ( ( n->chara_mask )[index] ) & ( 1 << mask_num ) );
}

/* ���֤ؤΥݥ�����Ͽ���Υ����å� */
int check_nfa2buf( n, head, tail )
NFA *n;
NFA **head,**tail;
{
  if ( n == NULL ) return( 0 );

  for ( ; head < tail ; head++ )
    /* ���Ǥ���Ͽ���줿���֤������0���֤� */
    if ( n == *head ) return( 0 );

  return( 1 );
}

/* ���֤ؤΥݥ�����Ͽ */
void add_nfa2buf( n, head, tail )
NFA *n;
NFA **head,***tail;
{
  if ( check_nfa2buf( n->next[0], head, *tail ) != 0 ) {
    **tail = n->next[0];
    *tail += 1;
  }
  if ( check_nfa2buf( n->next[1], head, *tail ) != 0 ) {
    **tail = n->next[1];
    *tail += 1;
  }

  return;
}

/* ����ɽ���ˤ��ѥ�����ȹ� */
UCHAR *regexp_sub( nfa, s, e )
NFA *nfa;
UCHAR *s, *e;
{
  NFA *buffer[2][NFA_MAX];
  NFA **head[2],**tail[2];
  UCHAR *matched_tail = NULL;
  int sw = 0;

  buffer[sw][0] = nfa;
  tail[sw] = &buffer[sw][1];
  for ( ; s <= e ; s++ ) {
    for ( head[sw] = buffer[sw] , tail[sw^1] = buffer[sw^1] ; head[sw] < tail[sw] ; head[sw]++ ) {
      /* ������ã������,ʸ���������Υݥ��󥿤���Ͽ */
      if ( (*head[sw])->next[0] == NULL && (*head[sw])->next[1] == NULL )
	matched_tail = s - 1;
      /* ��ʸ�����ܤξ���,���ߤΥХåե������˼��ξ��֤���Ͽ */
      else if ( isnull_chara_mask( *head[sw] ) != 0 )
	add_nfa2buf( *head[sw], buffer[sw], &tail[sw] );
      /* ʸ���ѥ����󤬥ޥå�������,�⤦�����ΥХåե��˼��ξ��֤���Ͽ */
      else if ( s < e && check_chara( *head[sw], *s ) != 0 )
	add_nfa2buf( *head[sw], buffer[sw^1], &tail[sw^1] );
    }
    sw ^= 1;
    if ( tail[sw] == buffer[sw] ) break; /* ���ΥХåե������ʤ齪λ */
  }

  return( matched_tail );
}

/* ʸ�������Ƭ�ݥ��󥿤���ˤ��餷�ʤ���
   �ѥ�����ȹ�롼�����ƤӽФ�           */
int regexp_main( nfa, s, e, start_flg, end_flg )
NFA *nfa;
UCHAR **s, **e;
int start_flg,end_flg;
{
  UCHAR *matched_tail;

  for ( ; *s < *e ; (*s)++ ) {
    if ( ( matched_tail = regexp_sub( nfa, *s, *e ) ) != NULL ) {
      if ( end_flg == 0 || matched_tail == *e - 1 ) {
	*e = matched_tail;
	return( 1 );
      }
    }
    /* ��Ƭ�򼨤��᥿����饯�������ä�����1�٤������� */
    if ( start_flg != 0 ) return( 0 );
  }

  return( 0 );
}
