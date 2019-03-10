#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>

/* bar.s works this way:
   every block has a cache with a base size of BASE_CACHE.
   if it exceeds that cache, it gets doubled.
   if any cache enlargens, bar.s is enlargend too.
*/

typedef struct {
	int interval;
	int signal;
	char* bg;
	char* cmd;
} Block;

typedef struct {
	char* c;
	int len;
} CacheLine;

typedef struct {
	char* s;
	char* template;
	int s_len;
	Block* blocks;
	/* indecies that indicate alignment */
	int left_index;
	char* preamble;
	int mid_index;
	char* mid_preamble;
	int right_index;
	char* right_preamble;
	char* ending;
	int s_base_size;
	CacheLine* cache;
	int blocks_num;
	char caches_changed;
} Bar;

#define CNF "cmd not found"
#define BASE_CACHE 64

int str_format(
		char* dst,
		char* src,
		CacheLine* blocks,
		int blocks_num,
		char placeholder
);

void init_bar(
	Bar* bar,
	Block* blocks,
	/* indecies that indicate alignment */
	int left_index,
	char* preamble,
	int mid_index,
	char* mid_preamble,
	int right_index,
	char* right_preamble,
	char* ending,
	int blocks_num
);

void update_block(Bar* bar, int i);

int update_interval(Bar* bar);

void recreate_s(Bar* bar);

void update_block_g(Bar* bar, int bi);

void update_init_g(Bar* bar);

int update_interval_g(Bar* bar);

typedef struct {
	Bar* bar;
	int block_index;
} BlockID;

BlockID* sigtable;

void sig_handler(int signum);

void bar_loop(
	Block* blocks,
	/* indecies that indicate alignment */
	int left_index,
	char* preamble,
	int mid_index,
	char* mid_preamble,
	int right_index,
	char* right_preamble,
	char* ending,
	int blocks_num
);


