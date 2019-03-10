#include "lemonbarc.h"

char* left_sep= "";
char* mid_sep = " | ";
char* right_sep = "";

char* preamble = "%{+u +o l}";
char* mid_preamble = "%{c}";
char* right_preamble = "%{r}";
char* ending = "%{-u -o}";
/*
block: {
	interval,
	signal,
	color;
	cmd
} */

int main(){
	/* BLOCKS */
	Block cpu = {
		2,
		-1,
		"#335577",
		"scripts/cpu"
	};
	Block bat = {
		60,
		-1,
		"#336633",
		"scripts/battery"
	};
	Block date = {
		60,
		-1,
		"#c3202020",
		"date '+%D %R'"
		//"echo '      '"
	};
	Block ws = {
		-1,
		1,
		"#002211",
		"scripts/wspy"
	};
	Block mem = {
		30,
		-1,
		"#664a33",
		"scripts/memory"
	};
	Block xkb = {
		-1,
		2,
		"#336666",
		"scripts/xkb"
	};
	Block pa_control = {
		-1,
		3,
		"#444444",
		"scripts/pa-control"
	};

	Block blocks[] =
		{ws, xkb, date, mem, cpu, pa_control, bat};
	bar_loop(blocks, 0, preamble, 2, mid_preamble, 3, right_preamble, ending, 7);
	printf("done!\n");
}
