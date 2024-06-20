#ifndef UNUSED
#define UNUSED __attribute__((unused))
#endif

#include <libdragon.h>

// static int is_randomized;
static int member_cnt;
static int member_cur;
static int *member_indis;
static int cycle_cnt;

static void member_indis_init(void);
static const char *last_hitter_determine(const char **argv);
#if 0
static void log_write(const char **argv, const int total_hits,
		      const char *last_hitter);
#endif

int main(void)
{
	const int argc = 3;
	const char *argv[3] = { "./weedtracker64", "Rosie", "Aeryk" };
	const char *last_hitter;
	int total_hits;

	console_init();
	joypad_init();
	console_set_render_mode(RENDER_MANUAL);
	cycle_cnt = 0;
	member_cnt = argc - 1;
	member_indis_init();
	member_cur = -1;
	joypad_buttons_t press;
	for (;;) {
		console_clear();
		if (++member_cur >= member_cnt) {
			member_cur = 0;
			cycle_cnt++;
		}
		printf("\n");
		printf("ROTATIONS: %d\n", cycle_cnt);
		for (int i = 0; i < member_cnt; i++) {
			printf("\t%d. %s%s\n", i + 1, argv[member_indis[i] + 1],
			       (i == member_cur) ? " <<" : "");
		}
		console_render();
		do {
			joypad_poll();
			press = joypad_get_buttons_pressed(JOYPAD_PORT_1);
			if (press.start)
				goto quit_roto;
		} while (!press.a);
	}
quit_roto:
	total_hits = member_cnt * cycle_cnt + member_cur;
	last_hitter = last_hitter_determine(argv);
	console_clear();
	printf("\nYou all have smoked %d bowls\n", total_hits);
	printf("The last person to hit was %s\n", last_hitter);
	console_render();
#if 0
	log_write(argv, total_hits, last_hitter);
#endif

	for (;;) {
	}
}

void member_indis_init(void)
{
	int i, j;

	member_indis = malloc(sizeof(*member_indis) * member_cnt);
#if 0
	if (is_randomized) {
		srand(time(NULL));
		for (i = 0; i < member_cnt; i++) {
retry_rand:
			member_indis[i] = rand() % member_cnt;
			for (j = 0; j < i; j++) {
				if (member_indis[j] == member_indis[i])
					goto retry_rand;
			}
		}

		return;
	}
#endif

	for (i = 0; i < member_cnt; i++)
		member_indis[i] = i;
}

const char *last_hitter_determine(const char **argv)
{
	if (member_cur)
		return (argv[1 + member_indis[member_cur - 1]]);

	if (!cycle_cnt)
		return ("FUCKING NOBODY! GOD, YOU GUYS ARE LAME!");

	return (argv[1 + member_indis[((member_cur - 1 < 0) ? member_cnt :
							      member_cur) -
				      1]]);
}

#if 0
static void log_write(const char **argv, const int total_hits,
		      const char *last_hitter)
{
	FILE *logfile;
	char namebuf[1024] = { 0 };
	char fullbuf[2048] = { 0 };

	logfile = fopen("log", "a");
	for (int i = 0; i < member_cnt; i++) {
		if (i > (member_cnt - 3)) {
			sprintf(namebuf + strlen(namebuf),
				(i == member_cnt - 2) ? "%s and " : "%s",
				argv[member_indis[i] + 1]);
			continue;
		}
		sprintf(namebuf + strlen(namebuf), "%s, ",
			argv[member_indis[i] + 1]);
	}
	sprintf(fullbuf, "%s took %d bong rips, with %s hitting last.\n",
		namebuf, total_hits, last_hitter);
	fwrite(fullbuf, 1, strlen(fullbuf), logfile);
	fclose(logfile);
}
#endif
