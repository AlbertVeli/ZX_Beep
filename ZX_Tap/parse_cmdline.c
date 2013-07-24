/* (k) ALL RIGHTS REVERSED - Reprint what you like */

/* Parse cmdline options for zxtap
 *
 * Sweetmorn, the 55th day of Confusion in the YOLD 3179
 *
 * Albert Veli
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "zxtap.h"

#define PROGRAM_NAME "zxtap"
#define PROGRAM_VERSION "0.1-unreleased"
#define PROGRAM_NAME_VERSION PROGRAM_NAME "-" PROGRAM_VERSION

/* Cmdline options */
struct opts opt;

static char *prog_doc =
   PROGRAM_NAME_VERSION
   "\n\nUsage: " PROGRAM_NAME " [OPTIONS] <BIN-file>\n\n"
   "MANDATORY\n\n"
   " BIN-file      - Name of file containing Z80 machine code\n\n"
   "OPTIONS         Description             [Defaults]\n"
   "-------         -----------             ----------\n"
   "-o <TAP-file> - Name of output TAP-file [basename of BIN-file + .tap]\n"
   "-l <address>  - Load address for BIN    [32768]\n"
   "-n            - No BASIC loader (only CODE data)\n"
   "BASIC loader OPTIONS\n"
   "-c <address>  - Clear address           [32767]\n"
   "-j <address>  - Jump address for loader [32768]\n"
   "                (USR <jump address>)\n"
   "-s <SCR-file> - Load SCREEN$ before BIN\n"
   "-b <colour>   - Border colour  (0-7)    [7]\n"
   "-p <colour>   - Paper colour   (0-7)    [7]\n"
   "-i <colour>   - Ink colour     (0-7)    [0]\n"
   "-r <comment>  - Insert REM-comment\n\n"
   "EXAMPLES\n\n"
   "Create hello.tap from hello.bin:\n\n"
   " " PROGRAM_NAME " hello.bin\n\n"
   "Create test.tap with multiple REMs at beginning of loader:\n\n"
   " " PROGRAM_NAME " -r \"+-----------+\" -r \"| Test v1.0 |\" -r \"+-----------+\" test.bin\n\n"
   "10 REM +-----------+\n"
   "20 REM | Test v1.0 |\n"
   "30 REM +-----------+\n"
   "40 rest of loader...\n\n"
   "Load SCREEN$ with black border before bin:\n\n"
   " " PROGRAM_NAME " -s examples/googledoodle.scr -b 0 -p 0 -i 7 examples/test.bin\n\n";


static void print_usage(void)
{
   printf("%s", prog_doc);
}


int parse_cmdline(int argc, char *argv[])
{
   int c, len, tmp;
   char *loc;
   char *basestart;

   /* Defaults */
   opt.tapfile = NULL;
   opt.scrfile = NULL;
   opt.need_free = 0;
   opt.load_address = 32768;
   opt.no_basic = 0;
   opt.clear_address = 32767;
   opt.jump_address = 32768;
   opt.border = 7;
   opt.paper = 7;
   opt.ink = 0;

   opterr = 0;
   while ((c = getopt (argc, argv, "o:s:nb:c:l:j:p:i:r:")) != -1) {
      switch (c) {

      case 'o':
         opt.tapfile = optarg;
         break;

      case 's':
         opt.scrfile = optarg;
         break;

      case 'n':
         opt.no_basic = 1;
         break;

      case 'c':
         tmp = strtoul(optarg, NULL, 0);
         if (tmp < 0x5cca || tmp > 0xffff) {
            fprintf(stderr, "\nM RAMTOP no good, 0:1\n\n");
            print_usage();
            return 1;
         }
         opt.clear_address = tmp;
         break;

      case'l':
      case'j':
         tmp = strtoul(optarg, NULL, 0);
         if (tmp < 0 || tmp > 0xffff) {
            fprintf(stderr, "\nB Integer out of range, 0:1\n\n");
            print_usage();
            return 1;
         }
         if (c == 'j') {
            opt.jump_address = tmp;
         } else if (c == 'l') {
            opt.load_address = tmp;
         } else {
            /* Should not get here... */
            fprintf(stderr, "Z Guru meditation, 0:1\n");
            return 1;
         }
         break;

      case 'b':
      case 'p':
      case 'i':
         tmp = strtoul(optarg, NULL, 0);
         if (tmp < 0 || tmp > 7) {
            fprintf(stderr, "\nK Invalid colour, 0:1\n\n");
            print_usage();
            return 1;
         }
         if (c == 'b') {
            opt.border = tmp;
         } else if (c == 'p') {
            opt.paper = tmp;
         } else if (c == 'i') {
            opt.ink = tmp;
         } else {
            /* Should not get here... */
            fprintf(stderr, "Z Guru meditation, 0:1\n");
            return 1;
         }
         break;

      case 'r':
         if (opt.numrems < MAXREMS) {
            opt.rems[opt.numrems] = optarg;
            opt.numrems++;
         }
         break;

      case '?':
         /* Did not get argument */
         fprintf(stderr, "\nA Invalid argument, 0:1\n\n");
         print_usage();
         return 1;

      default:
         print_usage();
         return 1;
      }
   }

   /* Need one mandatory option (BIN-file) */
   if (argc != optind + 1) {
      fprintf(stderr, "\nQ Parameter error, 0:1\n\n");
      print_usage();
      return 1;
   }

   opt.binfile = argv[optind];

   /* basestart = last slash in filename */
   basestart = strrchr(opt.binfile, '/');
   if (!basestart) {
      /* no slash, use start of string */
      basestart = opt.binfile;
   } else {
      /* basestart = first char after last slash */
      basestart++;
   }

   /* Save basename of binfile in opts */
   loc = strstr(basestart, ".bin");

   /* Try some more file endings */
   if (!loc) {
      loc = strstr(basestart, ".BIN");
   }
   if (!loc) {
      loc = strstr(basestart, ".Bin");
   }

   if (loc) {
      /* len = position of .bin */
      len = loc - basestart;
   } else {
      /* Use up to 10 letters of binfile as basename */
      len = strlen(basestart);
   }
   if (len > 10) {
      len = 10;
   }
   memcpy(opt.basename, basestart, len);
   opt.baselen = len; /* Save len (without padding) */

   /* Pad with 0x20 */
   for (; len < 10; len++) {
      opt.basename[len] = 0x20;
   }

   if (!opt.tapfile) {
      /* No tapfile given, create a filename as basename.tap */
      opt.tapfile = (char *)malloc(opt.baselen + 5);
      if (!opt.tapfile) {
         perror("malloc");
         return 1;
      }
      opt.need_free = 1;
      memcpy(opt.tapfile, opt.basename, opt.baselen);
      strcpy(&(opt.tapfile[opt.baselen]), ".tap");
   }

   return 0;
}


/* Free used memory */
void parse_cleanup(void)
{
   if (opt.need_free) {
      free(opt.tapfile);
   }
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
