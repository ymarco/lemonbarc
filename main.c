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
		"date '+%R, %D'"
		//"echo '      '"
	};
	Block ws = {
		-1,
		1,
		"#001122",
		"scripts/wspy"
	};
	Block mem = {
		30,
		-1,
		"#aa6a33",
		"scripts/memory"
	};
	Block xkb = {
		-1,
		2,
		"#337766",
		"scripts/xkb"
	};
	Block pa_control = {
		-1,
		3,
		"#444444",
		"scripts/pa-control"
	};
	Block brightness = {
		-1,
		4,
		"#aaaa33",
		"xbacklight | awk -F . '{print \" \"$1\"%\"}'"
	};
	Block blocks[] =
		{ws, xkb, brightness, date, mem, cpu, pa_control, bat};
	bar_loop(blocks, 0, preamble, 3, mid_preamble, 4, right_preamble, ending, 8);
	printf("done!\n");
}
