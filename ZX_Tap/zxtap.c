/* (k) ALL RIGHTS REVERSED - Reprint what you like */

/* WRITEME
 *
 * Sweetmorn, the 55th day of Confusion in the YOLD 3179
 *
 * Albert Veli
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "zxtap.h"

/* Header types */
#define PROGRAM_T 0
#define CODE_T 3

/* Block types */
#define HEADER_FLAG 0x00
#define DATA_FLAG 0xff

/* Size of SCREEN$-file */
#define SCRLEN 0x1b00
#define SCRADDR 0x4000


/* locally global variables */
int binlen = 0;
uint8_t checksum = 0;
FILE *tapfp = NULL;
FILE *binfp = NULL;
FILE *scrfp = NULL;
char basenamez[11];

static void write_byte(uint8_t b)
{
   fputc(b, tapfp);
   checksum ^= b;
}


/* Write little-endian word */
static void write_word(uint16_t w)
{
   write_byte(w & 0xff);
   write_byte(w >> 8);
}


static void write_blocklen(uint16_t len)
{
   /* No checksum */
   fputc(len & 0xff, tapfp);
   fputc(len >> 8, tapfp);
}


static int get_binlen(const char *filename)
{
   struct stat buf;

   if (stat(filename, &buf)) {
      perror(filename);
      return 0;
   }

   return buf.st_size;
}


static int open_files(void)
{
   binfp = fopen(opt.binfile, "rb");
   if (!binfp) {
      perror(opt.binfile);

      return 1;
   }

   tapfp = fopen(opt.tapfile, "wb");
   if (!tapfp) {
      perror(opt.tapfile);
      fclose(binfp);
      binfp = NULL;
      return 1;
   }

   if (opt.scrfile) {
      scrfp = fopen(opt.scrfile, "rb");
      if (!scrfp) {
         perror(opt.scrfile);
         fclose(binfp);
         binfp = NULL;
         fclose(tapfp);
         tapfp = NULL;
         return 1;
      }
   }

   return 0;
}


/* type: 0 ; PROGRAM
 *       1 ; Number array (not implemented)
 *       2 ; Character array (not implemented)
 *       3 ; CODE
 *
 * filename: Max 10 characters. Will be padded by spaces.
 */
static int write_tape_header(uint8_t filetype, const char *filename, int datablocklen, int address)
{
   int i, len;

   if ((filetype != PROGRAM_T) && (filetype != CODE_T)) {
      fprintf(stderr, "Error: unsupported header type\n");
      return 1;
   }

   len = strlen(filename);
   if ((len < 0) || (len > 10)) {
      fprintf(stderr, "F Invalid filename, 0:1\n");
      return 1;
   }

   /* Header block is always 19 bytes */
   write_blocklen(0x13);
   checksum = 0;
   write_byte(HEADER_FLAG);
   write_byte(filetype); /* Type of following data block, 0 = PROGRAM, 3 = CODE */
   for (i = 0; i < len; i++) {
      if (((filename[i] >= 'A') && (filename[i] <= 'Z')) ||
          ((filename[i] >= 'a') && (filename[i] <= 'z')) ||
          ((filename[i] >= '0') && (filename[i] <= '9'))) {
         write_byte(filename[i]);
      } else {
         fprintf(stderr, "Error: unsupported characters in filename at pos %d\n", i);
         return 1;
      }
   }
   /* Pad with spaces */
   for (; i < 10; i++) {
      write_byte(0x20);
   }

   /* Length of the following data block */
   write_word(datablocklen);

   /*              PROGRAM           CODE
    * Parameter 1  autostart line    start address
    * Parameter 2  variable area     >= 0x8000
    */
   if (filetype == PROGRAM_T) {
      /* If autostart write line number, else 0x800e */
      write_word(10);            /* Autostart at line 10 */
      write_word(datablocklen);  /* Put variable area after program */
   } else {
      /* CODE header */
      write_word(address); /* (default 0x8000 = 32768) */
      write_word(0x8020);    /* = 0x8020  */
   }

   write_byte(checksum);

   return 0;
}


int write_data_block(const char *buf, int len)
{
   int i;

   write_blocklen(len + 2);
   checksum = 0;
   write_byte(DATA_FLAG);
   for (i = 0; i < len; i++) {
      write_byte(buf[i]);
   }
   write_byte(checksum);

   return 0;
}


int write_program(const char *basicbuf, int len, const char *name)
{
   if (write_tape_header(PROGRAM_T, name, len, 0)) {
      return 1;
   }

   if (write_data_block(basicbuf, len)) {
      return 1;
   }

   return 0;
}


int write_code(const char *name, FILE *fp, int len, int address)
{
   char *buf;

   if (write_tape_header(CODE_T, name, len, address)) {
      return 1;
   }

   buf = (char *)malloc(len);
   if (!buf) {
      perror("4 Out of memory, 0:1");
      return 1;
   }

   /* Read binary into buf */
   if (fread(buf, len, 1, fp) != 1) {
      fprintf(stderr, "R Tape loading error, 0:1\n");
      free(buf);
      return 1;
   }

   if (write_data_block(buf, len)) {
      free(buf);
      return 1;
   }

   free(buf);

   return 0;
}


/* Create BASIC loader, options in opt */
int create_basic_loader(void)
{
   int lineno = 10;
   char line[128];
   int i;

   /* REM-lines */
   for (i = 0; i < opt.numrems; i++) {
      snprintf(line, sizeof(line) - 1, "%d REM %s", lineno, opt.rems[i]);
      lineno += 10;
      if (basic_add_line(line)) {
         return 1;
      }
   }

   /* Colours (if any) */
   if ((opt.ink != 0) || (opt.paper != 7) || (opt.border != 7)) {
      snprintf(line, sizeof(line) - 1, "%d BORDER %d : PAPER %d : INK %d",
               lineno, opt.border, opt.paper, opt.ink);
      lineno += 10;
      if (basic_add_line(line)) {
         return 1;
      }
   }

   snprintf(line, sizeof(line) - 1, "%d CLEAR %d", lineno, opt.clear_address);
   lineno += 10;
   if (basic_add_line(line)) {
      return 1;
   }

   /* Load screen (if -s is given) */
   if (opt.scrfile) {
      snprintf(line, sizeof(line) - 1, "%d CLS : LOAD \"Screen\" CODE", lineno);
      lineno += 10;
      if (basic_add_line(line)) {
         return 1;
      }
   }

   snprintf(line, sizeof(line) - 1, "%d LOAD \"%s\" CODE", lineno, basenamez);
   lineno += 10;
   if (basic_add_line(line)) {
      return 1;
   }

   snprintf(line, sizeof(line) - 1, "%d RANDOMIZE USR %d", lineno, opt.jump_address);
   lineno += 10;
   if (basic_add_line(line)) {
      return 1;
   }

   return 0;
}


int main(int argc, char *argv[])
{
   int ret = 1;
   char *basic_buf;
   int basic_len;

   /* Get options */
   if (parse_cmdline(argc, argv)) {
      return ret;
   }

   /* Get length of binfile */
   binlen = get_binlen(opt.binfile);
   if (binlen == 0) {
      fprintf(stderr, "Error: empty or non-existing BIN-file, giving up\n");
      goto cleanup;
   }

   if (opt.scrfile) {
      if (get_binlen(opt.scrfile) != SCRLEN) {
         fprintf(stderr, "Error: wrong filesize on %s\n", opt.scrfile);
         goto cleanup;
      }
   }

   /* Open filepointers */
   if (open_files()) {
      fprintf(stderr, "Error: could not open files, giving up\n");
      goto cleanup;
   }

   snprintf(basenamez, opt.baselen + 1, "%s", opt.basename);

   if (!opt.no_basic) {
      /* Creater BASIC loader code */
      if (create_basic_loader()) {
         fprintf(stderr, "Error: could not create BASIC loader, giving up\n");
      }

      if (basic_get(&basic_buf, &basic_len)) {
         fprintf(stderr, "Error: could not create BASIC loader, giving up\n");
         goto cleanup;
      }

      /* Write BASIC loader */
      if (write_program(basic_buf, basic_len, "Loader")) {
         goto cleanup;
      }

      /* Write SCREEN$ data */
      if (opt.scrfile) {
         if (write_code("Screen", scrfp, SCRLEN, SCRADDR)) {
            goto cleanup;
         }
      }
   }

   /* Write machine code from BIN-file */
   if (write_code(basenamez, binfp, binlen, opt.load_address)) {
      goto cleanup;
   }

   /* Success */

   printf("0 OK, 0:1 (%s)\n", opt.tapfile);

   ret = 0;

 cleanup:

   if (scrfp) fclose(scrfp);
   if (binfp) fclose(binfp);
   if (tapfp) fclose(tapfp);
   parse_cleanup();

   return ret;
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
