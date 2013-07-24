#ifndef __ZXTAP_H
#define __ZXTAP_H

/* (k) ALL RIGHTS REVERSED - Reprint what you like */

#define MAXREMS 10

/* Cmdline options */
struct opts
{
   /* Mandatory */
   char *binfile;

   /* Optional */
   char *tapfile;
   char *scrfile;
   char *rems[MAXREMS];
   int load_address;
   int no_basic;
   int clear_address;
   int jump_address;
   int border;
   int paper;
   int ink;

   /* Internal */
   int numrems;
   int need_free;
   char basename[10]; /* Padded with 0x20 */
   int baselen;
};

/* parse_cmdline.c */
extern struct opts opt;
extern int parse_cmdline(int argc, char *argv[]);
extern void parse_cleanup(void);

/* basic.c */
extern int basic_add_line(char *line);
extern int basic_get(char **bufp, int *len);


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */

#endif /* __ZXTAP_H */
