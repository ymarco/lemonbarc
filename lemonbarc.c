#include "lemonbarc.h"

extern char* left_sep;
extern char* mid_sep;
extern char* right_sep;
char* placeholder = "\7";


int str_format(
		char* dst,
		char* src,
		CacheLine* blocks,
		int blocks_num,
		char placeholder
		) {
	int i = 0;
	int temp_len = 0;
	//printf("src: \"%s\"\n", src);
	while (*src) {
		if(*src == placeholder) {
			if (i > blocks_num)
				break;
			temp_len = strlen(blocks[i].c);
			memcpy(dst, blocks[i].c, temp_len);
			dst += temp_len;
			i++;
		} else {
			*dst = *src;
			dst++;
		}
		src++;
	}
	/* loop stopped without copying the \0 from src, so we add it manually */
	*dst = '\0';
	//fprintf(stderr, "str_format result: \"%s\"\n", dst);
	return i;
}

void create_template(Bar* bar){
	/* now filling the bar */
	char* dst = bar->template;
	int i = 0;
	char no_sep = 0;
	char bg_start[] = "%{B#b5202070!}";
	char* bg_stop = "%{B-}";
	char sep_fg_start[] = "%{F#b5202070!}";
	char* sep_fg_stop = "%{F-}";
	//strcat(dst, /*bar->preamble*/ "asdasdasd");
	//return;
	strcat(dst, bar->preamble);
	sprintf(bg_start, "%{B%s}", bar->blocks[i].bg);
	strcat(dst, bg_start);
	while (i < bar->mid_index ) {
		strcat(dst, " ");
		strcat(dst, placeholder);
		strcat(dst, " ");
		strcat(dst, bg_stop);
		sprintf(bg_start, "%{B%s}", bar->blocks[i+1].bg);
		strcat(dst, bg_start);
		sprintf(sep_fg_start, "%{F%s}", bar->blocks[i].bg);
		strcat(dst, sep_fg_start);
		strcat(dst, left_sep);
		strcat(dst, sep_fg_stop);
		i++;
	}
	/* now i is mid_index */
	strcat(dst, bar->mid_preamble);
	no_sep = 1;
	while (i < bar->right_index ) {
		if(no_sep)
			no_sep = 0;
		else
			strcat(dst, mid_sep);
		sprintf(bg_start, "%{B%s}", bar->blocks[i].bg);
		strcat(dst, bg_start);
		strcat(dst, " ");
		strcat(dst, placeholder);
		strcat(dst, " ");
		strcat(dst, bg_stop);
		i++;
	}
	/* now i is right_index */
	strcat(dst, bar->right_preamble);

	while (i < bar->blocks_num) {
		if (no_sep) {
			no_sep = 0;
			strcat(dst, bg_stop);
		} else {
			sprintf(sep_fg_start, "%{F%s}", bar->blocks[i].bg);
			strcat(dst, sep_fg_start);
			strcat(dst, right_sep);
			strcat(dst, sep_fg_stop);
			strcat(dst, bg_stop);
		}
		sprintf(bg_start, "%{B%s}", bar->blocks[i].bg);
		strcat(dst, bg_start);
		strcat(dst, " ");
		strcat(dst, placeholder);
		//strcat(dst, " ");
		/* bg_stop will be drawn next iteration */
		i++;
	}
	strcat(dst, bg_stop);
	strcat(dst, bar->ending);
	//fprintf(stderr, "dst:\"%s\"\n", dst);
}

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
) {
	bar->blocks_num = blocks_num;
	bar->blocks = blocks;
	bar->preamble = preamble;
	bar->left_index = left_index;
	bar->mid_preamble = mid_preamble;
	bar->mid_index = mid_index;
	bar->right_index = right_index;
	bar->right_preamble = right_preamble;
	bar->ending = ending;

	#define COLOR_CHANGE_LEN (strlen("%{B#########}%{F##########}%{F-}%{B-}"))
	bar->s_base_size = (
		// preambles
		strlen(preamble) + strlen(mid_preamble) + strlen(right_preamble) + strlen(ending) 
		+ blocks_num * COLOR_CHANGE_LEN
		//separators
		+ (mid_index-left_index+1)*(strlen(left_sep) + COLOR_CHANGE_LEN) 
		+ (right_index-mid_index+1)*(strlen(mid_sep)+ COLOR_CHANGE_LEN)
		+ (mid_index-blocks_num+1)*(strlen(right_sep) + COLOR_CHANGE_LEN)
	);
	fprintf(stderr, "s_base_len: %d\n", bar->s_base_size);
	bar->template = calloc(bar->s_base_size, sizeof(char));
	create_template(bar);
	fprintf(stderr, "template len:%zu\n", strlen(bar->template));
	fprintf(stderr, "created template:\"%s\"\n", bar->template);
	bar->cache = malloc(blocks_num * sizeof(CacheLine));
	for (int i=0; i < bar->blocks_num; i++) {
		bar->cache[i].c = malloc(BASE_CACHE);
		bar->cache[i].len = BASE_CACHE;
	}
	bar->s_len = bar->s_base_size + BASE_CACHE*blocks_num + 200;
	/* calloc instead of malloc is important as calloc initializes ints return memory. */
	bar->s = calloc(bar->s_len, sizeof(char));
	//fprintf(stderr, "init bar:\"%s\"\n", bar->s);
}


void update_block(Bar* bar, int i){
	CacheLine* cache = &bar->cache[i];
	FILE* fp = popen(bar->blocks[i].cmd, "r");
	/* check for errors */
	if (!fp) {
		cache->c = realloc(cache->c, 256);
		strcpy(cache->c, "cmd not found");
		fprintf(stderr, "cmd not found");
		return;
	}
	char* res = fgets(cache->c, cache->len, fp);
	if (res != cache->c ) {
		strcpy(cache->c, "ERROR");
		//printf("ERROR");
		return;
		/* cache->len remains unchanged */
	}
	/* check if cache was large enough to fit the result */
	if (feof(fp) || cache->c[strlen(cache->c)-1] == '\n') {
		//fprintf(stderr, "%d was enough\n", cache->len);
		// shrink it
		while(strlen(cache->c) < cache->len/3 - 1 && cache->len >= BASE_CACHE){
			cache->len /= 2;
			cache->c = realloc(cache->c, cache->len);
		}
	} else { // enlargen it
		//fprintf(stderr, "%d wasnt enough\n", cache->len);
		int old_len;
		bar->caches_changed = 1;
		do {
			//fprintf(stderr, "cache contents: %s\n", cache->c);
			old_len = cache->len;
			cache->len *= 2;
			cache->c = realloc(cache->c, cache->len); ////////
		} while (fgets(cache->c+old_len-1, old_len+1, fp)
			 && cache->c[strlen(cache->c)-1]!='\n');
	}
	pclose(fp);
	int l = strlen(cache->c)-1;
	/* deleting \n char */
	if(cache->c[l]=='\n')
		cache->c[l] = '\0';
}

int update_interval(Bar* bar) {
	time_t t = time(NULL);
	int r = INT_MAX; // recommended re-update time
	int m;
	int itvl;
	for (int i=0; i < bar->blocks_num; i++) {
		itvl = bar->blocks[i].interval;
		if (itvl <= 0)
			continue;
		m = t % itvl;
		if (m == 0) {
			update_block(bar, i);
			r = (r>itvl ? itvl : r);
		} else {
			r = (r>m ? m : r);
		}
	}
	return r;
}


void recreate_s(Bar* bar) {
	/* reallocate if needed */
	if (bar->caches_changed) {
		free(bar->s);
		bar->s_len = bar->s_base_size;
		for (int i=0; i<bar->blocks_num; i++) {
			bar->s_len += bar->cache[i].len;
		}
		/* calloc instead of malloc is important. calloc initializes ints return memory. */
		bar->s = calloc(bar->s_len, sizeof(char));
		bar->caches_changed = 0;
		//printf("s_len: %d\n", bar->s_len);
	} else {
		/* erace the bar */
		memset(bar->s, '\0', bar->s_len);
	}
	/* now is the actuall update */
	str_format(bar->s, bar->template, bar->cache, bar->blocks_num, *placeholder);
}

void update_block_g(Bar* bar, int bi){
	update_block(bar, bi);
	recreate_s(bar);
	/* update lemonbar */
	printf("%s\n",bar->s);
	fflush(stdout);
}

int update_interval_g(Bar* bar){
	int res = update_interval(bar);
	recreate_s(bar);
	/* update lemonbar */
	printf("%s\n",bar->s);
	fflush(stdout);
	return res;
}

void update_init_g(Bar* bar){
	for (int i=0; i < bar->blocks_num; i++) {
		update_block(bar, i);
	}
	recreate_s(bar);
	printf("%s\n", bar->s);
	fflush(stdout);
}

void sig_handler(int signum){
	BlockID bid=sigtable[signum-SIGRTMIN];
	if (!bid.bar)
		return;
	update_block_g(bid.bar, bid.block_index);
}




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
) {
	BlockID st[SIGRTMAX-SIGRTMIN];
	sigtable = st;
	Bar bar;
	init_bar(&bar, blocks, left_index, preamble, mid_index, mid_preamble, right_index, right_preamble, ending, blocks_num);
	int waittime;
	/* create sigtable */
	for (int i=0, sig=0; i<bar.blocks_num; i++) {
		sig = bar.blocks[i].signal;
		if (sig > 0) {
			sigtable[sig].bar = &bar;
			sigtable[sig].block_index = i;
			signal(sig+SIGRTMIN, sig_handler);
			fprintf(stderr, "set sig %d for block %d\n", sig, i);
		}
	}
	update_init_g(&bar);
	while (1) {
		waittime = update_interval_g(&bar);
		//fprintf(stderr, "waittime:%d\n",waittime);
		sleep(waittime);
	}
}
