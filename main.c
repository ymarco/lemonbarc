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
		"scripts/wscpp"
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
		"#eaea7a",
		"xbacklight | awk -F . '{print \"%{F#222} \"$1\"%%{F-}\"}'"
	};
	Block mpd_state = {
		-1,
		5,
		"#4464ca",
		"scripts/mpd-state"
	};
	Block blocks[] =
		{ws, xkb, brightness, pa_control, mpd_state,date , mem, cpu, bat};
	bar_loop(blocks, 0, preamble, 5, mid_preamble, 6, right_preamble, ending, 9);
	printf("done!\n");
}
