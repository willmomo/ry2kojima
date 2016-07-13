#ifndef REGEXP
#define REGEXP

#define UCHAR unsigned char

/* 正規表現のメタキャラクタ */
#define RE_START '^'
#define RE_END '$'
#define RE_REPEAT0 '*'
#define RE_REPEAT1 '+'
#define RE_REPEAT01 '\?'
#define RE_ANY '.'
#define RE_QUOTE '\\'
#define RE_CLASS_START '['
#define RE_CLASS_END ']'
#define RE_CLASS_REVERSE '^'
#define RE_CLASS_SEQUENCE '-'
#define RE_GROUP_START '('
#define RE_GROUP_END ')'
#define RE_BOUND_START '{'
#define RE_BOUND_END '}'
#define RE_BOUND_DELIMITOR ','
#define RE_OR '|'

/* 文字パターンのサイズ */
#define CHARA_MASK_BIT_NUM 256
#define CHAR_BIT_MAX 8
#define LINT_BIT_MAX ( sizeof(long int) * CHAR_BIT_MAX )
#define MASK_SIZE ( CHARA_MASK_BIT_NUM / LINT_BIT_MAX )

#define NFA_MAX 256 /* NFA配列の最大 */

typedef struct NFA {
  long int chara_mask[MASK_SIZE]; /* 遷移できる文字パターン(256bit) */
  struct NFA *next[2];            /* 遷移先 */
} NFA;

#if defined(__cplusplus)
extern "C" {
#endif

void clear_chara_mask( NFA * );
void fill_chara_mask( NFA * );
void reverse_chara_mask( NFA * );
void calc_chara_mask( NFA *, UCHAR );
NFA *escape_chara_nfa( NFA *, NFA *, UCHAR * );
NFA *chara_nfa( NFA *, NFA *, UCHAR *, UCHAR * );
NFA *move2tail_nfa( NFA *, NFA *, NFA *, int );
NFA *bound_nfa( NFA *, NFA *, NFA *, int, int );
NFA *or_nfa( NFA *, NFA *, NFA *, NFA * );
int find_class_end( UCHAR **, UCHAR **, UCHAR * );
int find_group_end( UCHAR **, UCHAR **, UCHAR * );
UCHAR *check_bound( UCHAR *, int *, int *, UCHAR * );
NFA *compile_branch( NFA *, NFA *, UCHAR *, UCHAR * );
NFA *compile_nfa( NFA *, NFA *, UCHAR *, UCHAR * );

int isnull_chara_mask( NFA * );
int check_chara( NFA *, UCHAR );
int check_nfa2buf( NFA *, NFA **, NFA ** );
void add_nfa2buf( NFA *, NFA **, NFA *** );
UCHAR *regexp_sub( NFA *, UCHAR *, UCHAR * );
int regexp_main( NFA *, UCHAR **, UCHAR **, int, int );

#if defined(__cplusplus)
}
#endif

#endif
