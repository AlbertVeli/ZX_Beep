/* (k) ALL RIGHTS REVERSED - Reprint what you like */

/* Parse tiny subset of Sinclair BASIC in ascii-code and generate
 * encoded Spectrum BASIC. Mainly for creating a BASIC loader.
 *
 * Lines similar to these are supported:
 *
 * 10 REM whatever
 * 10 CLEAR VAL "32767"
 * 20 CLEAR 32767
 * 30 LOAD "mcode" CODE
 * 40 LOAD ""
 * 50 RANDOMIZE USR VAL "32768"
 * 60 RANDOMIZE USR 32768
 * 70 PRINT USR VAL "32768"
 * 80 PRINT USR 32768
 * 90 INK 0 : PAPER 7 : BORDER 7
 *
 * All tokens must be surrouded by whitespace.
 *
 *
 * Boomtime, the 56th day of Confusion in the YOLD 3179
 *
 * Albert Veli
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


/* Keycodes for BASIC
 * ------------------
 *
 * Control codes
 * | 00 Fl0| 01 Fl1| 02 Br0| 03 Br1| 04 In0| 05 In1| 06 CAP| 07 EDT|
 * | 08 LFT| 09 RIG| 0A DWN| 0B UP | 0C DEL| 0D ENT| 0E SYM| 0F GRA|
 * | 10 Ik0| 11 Ik1| 12 Ik2| 13 Ik3| 14 Ik4| 15 Ik5| 16 Ik6| 17 Ik7|
 * | 18 Pa0| 19 Pa1| 1A Pa2| 1B Pa3| 1C Pa4| 1D Pa5| 1E Pa6| 1F Pa7|
 *
 * Mostly ascii (except 60 and 7f)
 * | 20 SP | 21  ! | 22  " | 23  # | 24  $ | 25  % | 26  & | 27  ' |
 * | 28  ( | 29  ) | 2A  * | 2B  + | 2C  , | 2D  - | 2E  . | 2F  / |
 * | 30  0 | 31  1 | 32  2 | 33  3 | 34  4 | 35  5 | 36  6 | 37  7 |
 * | 38  8 | 39  9 | 3A  : | 3B  ; | 3C  < | 3D  = | 3E  > | 3F  ? |
 * | 40  @ | 41  A | 42  B | 43  C | 44  D | 45  E | 46  F | 47  G |
 * | 48  H | 49  I | 4A  J | 4B  K | 4C  L | 4D  M | 4E  N | 4F  O |
 * | 50  P | 51  Q | 52  R | 53  S | 54  T | 55  U | 56  V | 57  W |
 * | 58  X | 59  Y | 5A  Z | 5B  [ | 5C  \ | 5D  ] | 5E  ^ | 5F  _ |
 * | 60  £ | 61  a | 62  b | 63  c | 64  d | 65  e | 66  f | 67  g |
 * | 68  h | 69  i | 6A  j | 6B  k | 6C  l | 6D  m | 6E  n | 6F  o |
 * | 70  p | 71  q | 72  r | 73  s | 74  t | 75  u | 76  v | 77  w |
 * | 78  x | 79  y | 7A  z | 7B  { | 7C  | | 7D  } | 7E  ~ | 7F  © |
 *
 * ???
 * | 80 128| 81 129| 82 130| 83 131| 84 132| 85 133| 86 134| 87 135|
 * | 88 136| 89 137| 8A 138| 8B 139| 8C 140| 8D 141| 8E 142| 8F 143|
 * | 90 [A]| 91 [B]| 92 [C]| 93 [D]| 94 [E]| 95 [F]| 96 [G]| 97 [H]|
 * | 98 [I]| 99 [J]| 9A [K]| 9B [L]| 9C [M]| 9D [N]| 9E [O]| 9F [P]|
 * | A0 [Q]| A1 [R]| A2 [S]| A3 [T]| A4 [U]|
 *
 * Function keywords
 *                                         | A5 RND| A6 IK$| A7 PI |
 * | A8 FN | A9 PNT| AA SC$| AB ATT| AC AT | AD TAB| AE VL$| AF COD|
 * | B0 VAL| B1 LEN| B2 SIN| B3 COS| B4 TAN| B5 ASN| B6 ACS| B7 ATN|
 * | B8 LN | B9 EXP| BA INT| BB SQR| BC SGN| BD ABS| BE PEK| BF IN |
 * | C0 USR| C1 ST$| C2 CH$| C3 NOT| C4 BIN| C5 OR | C6 AND| C7 <= |
 * | C8 >= | C9 <> |
 *
 * Statements
 *                 | CA LIN| CB THN| CC TO | CD STP| CE DEF| CF CAT|
 * | D0 FMT| D1 MOV| D2 ERS| D3 OPN| D4 CLO| D5 MRG| D6 VFY| D7 BEP|
 * | D8 CIR| D9 INK| DA PAP| DB FLA| DC BRI| DD INV| DE OVR| DF OUT|
 * | E0 LPR| E1 LLI| E2 STP| E3 REA| E4 DAT| E5 RES| E6 NEW| E7 BDR|
 * | E8 CON| E9 DIM| EA REM| EB FOR| EC GTO| ED GSB| EE INP| EF LOA|
 * | F0 LIS| F1 LET| F2 PAU| F3 NXT| F4 POK| F5 PRI| F6 PLO| F7 RUN|
 * | F8 SAV| F9 RAN| FA IF | FB CLS| FC DRW| FD CLR| FE RET| FF CPY|
 */

/* Some example line encodings
 *
 * BASIC                    ; Lineno     Length     Basic code              ENTER
 * 10 CLEAR VAL "32767"     ; "\x00\x0a" "\x0a\x00" "\xfd\xb0\"32767\""     "\x0d"
 * 10 CLEAR 32767           ; "\x00\x0a" "\x0d\x00" "\xfd" "32767"     "\x0e\x00\x00\xff\x7f\x00" "\x0d"
 *                                                                                        ^   ^
 *                                                                                        number
 *
 * 30 RANDOMIZE USR 32768   ; "\x00\x1e" "\x0e\x00" "\xf9\xc0" "32768" "\x0e\x00\x00\x00\x80\x00" "\x0d"
 *                                                                                        ^   ^
 *                                                                                        number
 *
 * 20 PRINT USR VAL "32768" ; "\x00\x14" "\x0b\x00" "\xf5\xc0\xb0\"32768\"" "\x0d"
 * 20 LOAD "mcode" CODE     ; "\x00\x14" "\x0a\x00" "\xef\x22mcode\x22\xaf" "\x0d"
 */

/* Uncomment DEBUG line to get some debug output */
/* #define DEBUG 1 */

/* Token types */
enum ttype {
   NONE,
   STATEMENT,
   FUNCTION,
   NUMBER,
   STRING
};

/* States */
enum stype {
   EXP_STATEMENT,
   EXP_VAL_OR_NUM,
   EXP_STRING,
   EXP_USR,
   EXP_END,
   EXP_CODE_OR_END,
   LINE_DONE
};

struct keyword {
   char *keyword;
   enum ttype type;
   uint8_t val;
};

#define ENTER_C 0x0d
#define VAL_C 0xb0
#define USR_C 0xc0
#define CODE_C 0xaf
#define INK_C 0xd9
#define PAPER_C 0xda
#define BORDER_C 0xe7
#define REM_C 0xea
#define LOAD_C 0xef
#define PRINT_C 0xf5
#define RAND_C 0xf9
#define CLEAR_C 0xfd

static struct keyword keywords[] = {
   { "VAL", FUNCTION, VAL_C },
   { "USR", FUNCTION, USR_C },
   { "CODE", FUNCTION, CODE_C },
   { "INK", STATEMENT, INK_C },
   { "PAPER", STATEMENT, PAPER_C },
   { "BORDER", STATEMENT, BORDER_C },
   { "REM", STATEMENT, REM_C },
   { "LOAD",  STATEMENT, LOAD_C },
   { "PRINT",  STATEMENT, PRINT_C },
   { "RANDOMIZE",  STATEMENT, RAND_C },
   { "CLEAR",  STATEMENT, CLEAR_C },
   { NULL, NONE, 0 }
};

/* Keep it simple. Static buffers for basic, line and token */

/* BASIC program buffer */
#define BBUFLEN 1024
static char basicbuf[BBUFLEN];
static int basiclen = 0;

/* Line buffer */
#define LINELEN 512
static char linebuf[LINELEN];
static int linelen = 0;

/* Next token, if string */
#define TOKENLEN 512
static char next_token[TOKENLEN];
static int tokenlen = 0;
static enum ttype next_token_type = NONE;


/* Write byte to basicbuf */
static void write_bbyte(char c)
{
   if (basiclen < BBUFLEN) {
      basicbuf[basiclen] = c;
      basiclen++;
   } else {
      fprintf(stderr, "G No room for line (B)\n");
   }
}


/* Write byte to linebuf */
static void write_lbyte(char c)
{
   if (linelen < LINELEN) {
      linebuf[linelen] = c;
      linelen++;
   } else {
      fprintf(stderr, "G No room for line (L)\n");
   }
}

/* Write 16-bit little endian word to linebuf */
static void write_lword(uint16_t num)
{
   write_lbyte(num & 0xff);
   write_lbyte(num >> 8);
}

/* Write 16-bit little endian word to basicbuf */
static void write_bword(uint16_t num)
{
   write_bbyte(num & 0xff);
   write_bbyte(num >> 8);
}


/* Line numbers are big endian */
static void write_lineno(uint16_t num)
{
   write_bbyte(num >> 8);
   write_bbyte(num & 0xff);
}


/* Return start of next token in line (NULL if end of line) */
static char *eat_whitespace(char *line)
{
   char *p;

   if ((!line) || (! *line)) {
      return NULL;
   }

   p = line;

   while (*p == ' ' || *p == '\t') {
      p++;
   }

   if (*p != 0) {
      return p;
   }

   return NULL;
}


/* little helper for get_next_token */
static void analyze_token(void)
{
   int i = 0;

   /* Check if keyword */
   while (keywords[i].keyword) {
      if (strcmp(next_token, keywords[i].keyword) == 0) {
         /* found keyword, either FUNCTION or STATEMENT */
         next_token_type = keywords[i].type;
         /* place kbd code in next_token */
         next_token[0] = keywords[i].val;
         tokenlen = 1;
         return;
      }
      i++;
   }

   /* number or unquoted string? */
   next_token_type = NUMBER;
   for (i = 0; i < tokenlen; i++) {
      if ((next_token[i] < '0') || (next_token[i] > '9')) {
         next_token_type = STRING;
      }
   }
}


/* Get next token from line.
 *
 * next_token <- token
 * tokentype <- type
 * tokenlen <- length of next_token
 *
 * Return: pointer to first character after token in line or NULL.
 */
static char *get_next_token(char *line)
{
   char *p;
   int quotes = 0;

   next_token_type = NONE;

   /* Remove whitespace */
   line = eat_whitespace(line);
   if ((!line) || (! *line)) {
      return NULL;
   }

   p = line;

   /* Copy to next_token */
   tokenlen = 0;
   /* Only allow the following characters in our BASIC subset */
   while (((*p >= 'A') && (*p <= 'Z')) ||
          ((*p >= 'a') && (*p <= 'z')) ||
          ((*p >= '0') && (*p <= '9')) ||
          (*p == '"') ||
          (*p == ':'))
      {
         if (*p == '"') {
            quotes++;
         } else {
            if (tokenlen < TOKENLEN - 1) {
               next_token[tokenlen] = *p;
               tokenlen++;
            } else {
               /* Internal buffer overflow */
               fprintf(stderr, "G No room for line\n");
               return NULL;
            }
         }
         p++;
      }

   /* Unallowed characters in line */
   if ((*p != 0) && (*p != ' ') && (*p != '\t')) {
      fprintf(stderr, "C Nonsense in BASIC (invalid char)\n");
      return NULL;
   }

   /* quoted string? */
   if ((quotes > 2) || (quotes == 1)) {
      fprintf(stderr, "C Nonsense in BASIC (quotes)\n");
      return NULL;
   }

   if (quotes == 2) {
      next_token_type = STRING;
      return p;
   }

   if (tokenlen == 0) {
      /* Only valid token with len 0 is empty string */
      fprintf(stderr, "C Nonsense in BASIC\n");
      return NULL;
   }

   next_token[tokenlen] = 0; /* for strcmp */

   /* Try to identify keywords/numbers/strings */
   analyze_token();

   return p;
}


static void line_err_helper(char *line, int errpos)
{
   int i;
   fprintf(stderr, "%s\n", line);
   for (i = 0; i < errpos - 1; i++) {
      fprintf(stderr, " ");
   }
   fprintf(stderr, "^\n");
}


/* Parse and encode one line of BASIC and add it to basicbuf */
int basic_add_line(char *line)
{
   int ret = 1;
   int lineno;
   enum stype state;
   char *orgline = line;
   int i;

   if (!line) {
      goto line_end;
   }

#ifdef DEBUG
   printf("DBG: %s - ", line);
#endif

   linelen = 0;

   line = get_next_token(line);
   if (next_token_type != NUMBER) {
      fprintf(stderr, "Parse error: expected line number\n");
      line_err_helper(orgline, line - orgline);
      goto line_end;
   }

   lineno = strtoul(next_token, NULL, 10);
   if (lineno <= 0 || lineno > 9999) {
      fprintf(stderr, "B Line number out of range\n");
      line_err_helper(orgline, line - orgline);
      goto line_end;
   }

   write_lineno(lineno);

   state = EXP_STATEMENT;
   line = get_next_token(line);

   while (line) {

      switch (state) {

         /* Read statement */
      case EXP_STATEMENT:
         if (next_token_type != STATEMENT) {
            fprintf(stderr, "Parse error: expected STATEMENT\n");
            line_err_helper(orgline, line - orgline);
            goto line_end;
         }

         /* keycode is in next_token[0] */
         write_lbyte(next_token[0]);

         /* Next state */
         if (((uint8_t)next_token[0] == INK_C) ||
             ((uint8_t)next_token[0] == PAPER_C) ||
             ((uint8_t)next_token[0] == BORDER_C) ||
             ((uint8_t)next_token[0] == CLEAR_C)) {
            state = EXP_VAL_OR_NUM;
         } else if (((uint8_t)next_token[0] == PRINT_C) ||
                    ((uint8_t)next_token[0] == RAND_C)) {
            state = EXP_USR;
         } else if ((uint8_t)next_token[0] == LOAD_C) {
            state = EXP_STRING;
         } else if ((uint8_t)next_token[0] == REM_C) {
            /* REM, write rest of line */
            if (line) line++; /* eat first space */
            if (line) {
               while (*line) {
                  /* anything, except ENTER */
                  if (*line != ENTER_C) {
                     write_lbyte(*line);
                  }
                  line++;
               }
               line = NULL;
            }
            state = LINE_DONE;
         } else {
            fprintf(stderr, "Error: STATEMENT not implemented\n");
            line_err_helper(orgline, line - orgline);
            goto line_end;
         }

         break;

      case EXP_VAL_OR_NUM:
         if ((next_token_type != NUMBER) &&
             ((next_token_type != FUNCTION) || ((uint8_t)next_token[0] != VAL_C))) {
            fprintf(stderr, "Parse error: expected VAL or NUMBER\n");
            line_err_helper(orgline, line - orgline);
            goto line_end;
         }

         if (next_token_type == FUNCTION) {
            /* Got VAL */
            write_lbyte(next_token[0]);
            state = EXP_STRING;
         } else {
            /* Got NUMBER, translate to
             * number string + 0e 00 00 ff 7f 00
             *                          ^  ^
             *                         number
             *
             * Note. It would also be possible to
             * translate to VAL "number", which has
             * shorter encoding. But that would show
             * up different in the Spectrum.
             */
            int num = strtoul(next_token, NULL, 10);

            if (num > 65535) {
               fprintf(stderr, "B Integer out of range\n");
               line_err_helper(orgline, line - orgline);
               goto line_end;
            }

            for (i = 0; i < tokenlen; i++) {
               write_lbyte(next_token[i]);
            }

            write_lbyte(0x0e);
            write_lword(0x0000);
            write_lword((uint16_t)num);
            write_lbyte(0x00);

            state = EXP_END;
         }
         break;

      case EXP_STRING:
         if ((next_token_type != STRING) &&
             /* Ok to interpret number as string here */
             (next_token_type != NUMBER)) {
            fprintf(stderr, "Parse error: expected STRING\n");
            line_err_helper(orgline, line - orgline);
            goto line_end;
         }

         write_lbyte('"');
         for (i = 0; i < tokenlen; i++) {
            write_lbyte(next_token[i]);
         }
         write_lbyte('"');

         state = EXP_END;

         break;

      case EXP_USR:
         if ((next_token_type != FUNCTION) || ((uint8_t)next_token[0] != USR_C)) {
            fprintf(stderr, "Parse error: expected USR\n");
            line_err_helper(orgline, line - orgline);
            goto line_end;
         }
         write_lbyte(next_token[0]);
         state = EXP_VAL_OR_NUM;
         break;

      case EXP_END:
         /* Should only get here if:
          * : - expect new statement, or
          * CODE_C and line[0] == LOAD_C
          */
         if ((uint8_t)next_token[0] == ':') {
            /* : */
            state = EXP_STATEMENT;
         } else if (((uint8_t)linebuf[0] == LOAD_C) &&
                    (next_token_type == FUNCTION) &&
                    ((uint8_t)next_token[0] == CODE_C)) {
            /* CODE_C (with LOAD_C at beginning) */
            state = LINE_DONE;
         } else {
            fprintf(stderr, "Parse error: expected end of line or colon\n");
            line_err_helper(orgline, line - orgline);
            goto line_end;
         }

         write_lbyte(next_token[0]);

         break;

      default:
         fprintf(stderr, "Error: unexpected internal state\n");
         line_err_helper(orgline, line - orgline);
         goto line_end;
         /* break; */
      }

      line = get_next_token(line);
   }

   if ((state != LINE_DONE) && (state != EXP_END)) {
      fprintf(stderr, "Error: unexpected internal state (EOL)\n");
      goto line_end;
   }

   /* End line with ENTER */
   write_lbyte(ENTER_C);

   if (linelen == LINELEN) {
      fprintf(stderr, "G No room for line (EOL)\n");
      goto line_end;
   }

   /* Success, write length + linebuf to basicbuf */
   ret = 0;

   write_bword(linelen);

#ifdef DEBUG
   printf("Ok (%d):", linelen);
#endif
   for (i = 0; i < linelen; i++) {
#ifdef DEBUG
      printf(" %02x", (uint8_t)linebuf[i]);
#endif
      write_bbyte(linebuf[i]);
   }
#ifdef DEBUG
   printf("\n");
#endif

 line_end:

   return ret;
}


/* Fetch basic buffer/len as outparameters.
 * Return 0 ok. 1 error.
 */
int basic_get(char **bufp, int *len)
{
   if ((basiclen > 0) && (basiclen < BBUFLEN)) {
      *bufp = basicbuf;
      *len = basiclen;
      return 0;
   }

   return 1;
}


/* Set this to 1 to compile this file standalone (for debugging). */
#if 0
int main(void)
{
   /* int i; */

   if (basic_add_line("10 INK 0 : PAPER 7 : BORDER 7")) return 1;

   /* if (basic_add_line("10 REM Loader v0.1")) return 1; */
   /* if (basic_add_line("20 CLEAR 32767")) return 1; */
   /* if (basic_add_line("30 LOAD \"mcode\" CODE")) return 1; */
   /* if (basic_add_line("40 RANDOMIZE USR 32768")) return 1; */

   /* if (basic_add_line("10 CLEAR VAL \"32767\"")) return 1; */
   /* if (basic_add_line("10 CLEAR VAL 32767")) return 1; */
   /* if (basic_add_line("10 CLEAR 32767")) return 1; */

   /* if (basic_add_line("30 RANDOMIZE USR VAL \"32768\"")) return 1; */
   /* if (basic_add_line("30 RANDOMIZE USR VAL 32768")) return 1; */
   /* if (basic_add_line("30 RANDOMIZE USR 32768")) return 1; */

   /* if (basic_add_line("40 PRINT USR VAL \"32768\"")) return 1; */
   /* if (basic_add_line("40 PRINT USR VAL 32768")) return 1; */
   /* if (basic_add_line("40 PRINT USR 32768")) return 1; */

   /* if (basic_add_line("50 LOAD \"mcode\" CODE")) return 1; */
   /* if (basic_add_line("60 LOAD \"\"")) return 1; */

   printf("\nBASIC:\n");
   for (i = 0; i < basiclen; i++) {
      printf(" %02x", (uint8_t)basicbuf[i]);
   }
   printf("\n");

   return 0;
}
#endif

/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
