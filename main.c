#include "debug.h"
#include "loop.h"
#include "input.h"
#include "opts.h"
#include "plot.h"
#include "psi.h"
#include <unistd.h>

#define DYNAMIC_AXIS_TMP 0
#define TMPFILE1 "/tmp/file1"
int
main(int argc, char **argv)
{
	char* argv_fake[] = { "psimon", "-f", "-b", "0:1023", "-c", "r", "-i", TMPFILE1}; //"-b", "0:1024", 
	struct plot pl = { 0 };
	FILE * file1 = fopen(TMPFILE1, "w");
	plot_init(&pl);
	int lc = parse_opts(&pl, sizeof(argv_fake)/sizeof(argv_fake[0]) - (2 * DYNAMIC_AXIS_TMP), argv_fake);

/*
	int f[2];
	if (pipe(f)){
		die("%s", "can't allocate anonymous pipe\n");
	}
	FILE* fpr = fdopen(f[0], "r");
	FILE* fpw = fdopen(f[1], "w");
	if(fpr == NULL || fpw == NULL) {
		die("%s", "problem opening pipe\n");
	}
*/

/*
	if (pl.datasets == 0) {
		plot_add(&pl, fpr, lc);
	}
*/

	/* TODO: plumb some options: 
	* - select between memory/io/cpu  (or maybe support them simultaniously)
	* - different cgroups
	* - define colors
	* - configure speed/timestep?
	*/
	struct psi ps = { 0 };
	psi_init(&ps, "/proc/pressure/cpu", file1);

	set_input_buffer_size(8);
	loop(&pl, &ps, pl.follow_rate);
	

	psi_destroy(&ps);
	plot_destroy(&pl);

	fflush(stdout);

	return 0;
}
