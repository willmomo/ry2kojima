#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "regexp.h"

void print_chara( c )
UCHAR c;
{
  if ( isprint( c ) )
    if ( c == ' ' )
      printf( "'%c'", c );
    else
      printf( "%c", c );
  else
    printf( "[x%X]", c );
}

void print_nfa( head, tail )
NFA *head,*tail;
{
  NFA *n;
  int index[2], i, j;
  int seq_flg;

  for ( n = head ; n <= tail ; n++ ) {
    index[0] = ( (n->next)[0] != NULL ) ? (size_t)( (n->next)[0] - head ) : -1;
    index[1] = ( (n->next)[1] != NULL ) ? (size_t)( (n->next)[1] - head ) : -1;
    printf( "%.3d : mask=", (size_t)(n - head) );
    seq_flg = 0;
    for ( i = 0 ; i < MASK_SIZE ; i++ ) {
      for ( j = 0 ; j < LINT_BIT_MAX ; j++ ) {
	if ( ( ( ( n->chara_mask )[i] >> j ) & 1 ) != 0 ) {
	  if ( seq_flg == 0 ) {
	    seq_flg = i * LINT_BIT_MAX + j + 1;
	    print_chara( (UCHAR)seq_flg );
	  };
	} else {
	  if ( seq_flg != 0 ) {
	    if ( seq_flg < i * LINT_BIT_MAX + j ) {
	      printf( "-" );
	      print_chara( (UCHAR)( i * LINT_BIT_MAX + j ) );
	    }
	    seq_flg = 0;
	  }
	}
      }
    }
    if ( seq_flg != 0 ) {
      printf( "-" );
      print_chara( (UCHAR)( MASK_SIZE * LINT_BIT_MAX - 1 ) );
    }
    printf( " next0=%d next1=%d\n",
	    ( (n->next)[0] != NULL ) ? (size_t)( (n->next)[0] - head ) : -1,
	    ( (n->next)[1] != NULL ) ? (size_t)( (n->next)[1] - head ) : -1 );
  }
}

int main( argc, argv )
int argc;
UCHAR *argv[];
{
  NFA nfa[NFA_MAX], *tail;
  UCHAR *s, *e;
  FILE *fp;
  UCHAR linedata[256];

  int start_flg = 0,end_flg = 0;

  if ( argc > 2 ) {
    if ( ( fp = fopen( argv[2], "r" ) ) == NULL ) {
      fprintf( stderr, "can't open file %s.\n", argv[2] );
      return( 0 );
    }
  } else
    fp = stdin;

  s = argv[1];
  e = s + strlen( s );
  if ( *s == RE_START ) {
    start_flg = 1;
    s++;
  }
  if ( *( e - 1 ) == RE_END ) {
    end_flg = 1;
    e--;
  }

  if ( ( tail = compile_nfa( nfa, &nfa[NFA_MAX - 1] + 1, s, e ) ) == NULL ) return( 0 );
#ifdef debug
  print_nfa( nfa, tail );
#endif

  while ( fgets( linedata, sizeof( linedata ), fp ) != NULL ) {
    s = linedata;
    e = s + strlen( s );
    if ( *( e - 1 ) == '\n' ) e--;
    if ( regexp_main( nfa, &s, &e, start_flg, end_flg ) != 0 ) {
      printf( "%.*s\n", (int)( strlen( linedata ) - 1 ), linedata );
#ifdef debug
      printf( "%*s^", (int)( s - linedata ), "" );
      printf( "%*s^\n", (int)( e - s - 1 ), "" );
#endif
    }
  }
  fclose( fp );

  return( 1 );
}
