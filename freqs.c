/* All Rights Reversed - No Rights Reserved */

/* Code to print out assembly listing for ZX Spectrum with frequencies for the
 * beeper ROM routine. Frequencies are not given in Hz, or tones, but calculated
 * in a rather intricate manner.
 *
 * As a bonus the code can output data for both Equal Temperament (commonly used)
 * and the Harmonic Scale (aka Just Scale or Helmholtz scale). For an explanation
 * see http://www.phy.mtu.edu/~suits/scales.html.
 *
 * If you want to keep things simple, just use EQUAL_TEMPERAMENT and 440 Hz for A:
 *
 *  ./freqs 440.0
 *
 * ---
 *
 * Read on if you want to fine tune harmonies for some key.
 *
 * EQUAL_TEMPERAMENT ; does not depend on key
 * JUST_SCALE        ; has better harmonies with frequencies tuned to a key
 *
 * To fine tune frequencies, for instance for A (minor or major doesn't matter),
 * use JUST_SCALE and input the frequency for A. Usually 440.0 Hz, but can also be
 * for instance 432.0 Hz (Verdi tuning) or 430.0 Hz (classical pitch):
 *
 *  ./freqs -j 432.0
 *
 * Remember to re-run this program and input a different tone as base frequency if
 * you play a song in a different key with JUST scale. For C, 261.63 Hz is commonly
 * used as base frequency. But some people use 256 Hz instead (makes all C:s into
 * powers of two).
 *
 * The code outputs 12 lines, one for each semitone in an octave. If you use 440Hz
 * as base frequency (A4, fourth octave on a piano) and want to play for instance
 * A in the fifth octave (A5). Then just take the line for A4 and double the values
 * (shift left) for both HL and DE. The same can be done for all 12 tones. To go
 * down one octave shift right instead.
 *
 * Shifting does not produce exact results (because of rounding errors and because
 * of the constant in get_hl() below). You might consider generating lines for all
 * octaves used in the song, instead of shifting. To generate 7 octaves JUST_SCALE
 * starting at A0 (27.5 Hz), run:
 *
 *  ./freqs -o 7 -j 27.5
 *
 * This gives better results, but use more memory.
 *
 * Boomtime, the 51st day of Confusion in the YOLD 3179
 *
 * Albert Veli
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define PROGRAM_NAME "freqs"
#define PROGRAM_VERSION "0.1-unreleased"
#define PROGRAM_NAME_VERSION PROGRAM_NAME "-" PROGRAM_VERSION

/* Cmdline options */
struct opts
{
   /* Mandatory */
   double base_freq;

   /* Optional */
   int scale;
   int octaves;
};

struct opts opt;

static char *prog_doc =
   PROGRAM_NAME_VERSION
   "\n\nUsage: " PROGRAM_NAME " [OPTIONS] <frequency>\n\n"
   "MANDATORY\n\n"
   "Frequency - Base frequency for octave (440.0 for \"normal\" A)\n\n"
   "OPTIONS\n\n"
   "-o <num>  - Number of octaves (default 1)\n"
   "-j        - Use JUST scale (default TEMPERED)\n\n"
   "EXAMPLES\n\n"
   "TEMPERED scale, five octaves, starting at 110 Hz (A2 on piano):\n\n"
   " ./" PROGRAM_NAME " -o 5 110.0\n\n"
   "JUST scale, one octave, starting at 261.625565 Hz (C4 on piano, aka Middle C):\n\n"
   " ./" PROGRAM_NAME " -j 261.625565\n\n";


/* ZX Spectrum Z80 CPU-frequency */
#define CPUFREQ ((double)3500000)

/* But something, somewhere, eats 236 T-cycles (clock periods) */
#define MAGICALMYSTERYCONSTANT ((double)236)

/* Scale to use (see http://www.phy.mtu.edu/~suits/scales.html) */
#define JUST_SCALE 1
#define EQUAL_TEMPERAMENT_SCALE 0


/* base_freq = freq for ground note in scale, for
 * instance, for A scale, 440.0 Hz (or something like
 * 432 Hz for Verdi tuning).
 *
 * Pitch [0-11] = number of half notes above base freq.
 * Octave is accomplished by doubling (up, or halving down).
 */
double get_freq(double base_freq, int pitch, int use_just_scale)
{
   /* Just scale */
   double helmholtz_table[12] = {
      1.0,        /*  0 Unison */
      25 / 24.0,  /*  1 Minor Second */
      9  / 8.0,   /*  2 Major Second */
      6  / 5.0,   /*  3 Minor Third */
      5  / 4.0,   /*  4 Major Third */
      4  / 3.0,   /*  5 Fourth */
      45 / 32.0,  /*  6 Diminished Fifth */
      3  / 2.0,   /*  7 Fifth */
      8  / 5.0,   /*  8 Minor Sixth */
      5  / 3.0,   /*  9 Major Sixth */
      9  / 5.0,   /* 10 Minor Seventh */
      15 / 8.0,   /* 11 Major Seventh */
   };

   /* Equal temperament scale, see print_equal_temperament.c */
   double equal_temperament_table[12] = {
      1.000000000000, /*  0 Unison */
      1.059463094359, /*  1 Minor Second */
      1.122462048309, /*  2 Major Second */
      1.189207115003, /*  3 Minor Third */
      1.259921049895, /*  4 Major Third */
      1.334839854170, /*  5 Fourth */
      1.414213562373, /*  6 Diminished Fifth */
      1.498307076877, /*  7 Fifth */
      1.587401051968, /*  8 Minor Sixth */
      1.681792830507, /*  9 Major Sixth */
      1.781797436281, /* 10 Minor Seventh */
      1.887748625363  /* 11 Major Seventh */
   };

   double freq = base_freq;
   double *ratio;
   int i;

   if (use_just_scale) {
      ratio = helmholtz_table;
   } else {
      ratio = equal_temperament_table;
   }

   if (pitch < 0 || pitch > 11) {
      if (pitch > 11) {
         /* Multiply by 2 to scale up one octave */
         for (i = 0; i < pitch / 12; i++) {
            freq *= 2.0;
         }
         pitch = pitch % 12;
      } else {
         /* Negative pitch */
         int octaves = -(pitch / 12 - 1);
         /* printf("DBG: scale down %d octaves\n", octaves); */
         for (i = 0; i < octaves; i++) {
            freq /= 2.0;
         }
         /* Pitch will be negative, scale down one more octave and set positive pitch */
         pitch = (pitch % 12) + 12;
      }
   }

   return freq * ratio[pitch];
}


/* Frequency/duration -> hl/de register values:
 *
 * tp = 1/Hz ; tone period (s)
 * tc = tp * (CPU freq) = 3500000/Hz ; CPU cycles per tone period
 * hl = (tc - 236) / 8 = (3500000/Hz - 236) / 8
 * de = tone duration, (in number of tone periods).
 *
 * Example: Beep 430.54 Hz (scientific pitch A) for 1 s:
 * hl = (3500000/430.54 - 236) / 8 = $03db (rounded, $03da floored)
 * de = 1 / tp = Hz = 430.54 = $01af
 */
uint16_t get_hl(double freq)
{
   /* Ease-up return expression bloat */
   double tmp;

   /* CPU cycles per tone period - mystical cycle-munching constant */
   tmp = CPUFREQ / freq - MAGICALMYSTERYCONSTANT;

   /* hl = (Cycles/TonePeriod - MAGICALMYSTERYCONSTANT) / 8
    * Careful with conversion to integer here. (int)(floatval + 0.5)
    * rounds it to roughly the closest integer.
    */
   return (uint16_t)((tmp / 8.0) + 0.499);
}


void print_usage(void)
{
   printf("%s", prog_doc);
}


int main(int argc, char *argv[])
{
   char *intervals[12] = {
      "Unison",
      "Minor Second",
      "Major Second",
      "Minor Third",
      "Major Third",
      "Fourth",
      "Diminished Fifth",
      "Fifth",
      "Minor Sixth",
      "Major Sixth",
      "Minor Seventh",
      "Major Seventh"
   };

   double freq;  /* Frequency (Hz), periods per second */
   uint16_t hl, de;
   int c, pitch;

   /* Parse cmdline args using getopt (from unistd.h) */

   /* Defaults */
   opt.scale = EQUAL_TEMPERAMENT_SCALE;
   opt.octaves = 1;

   opterr = 0;
   while ((c = getopt (argc, argv, "jo:")) != -1) {
      switch (c) {

      case 'j':
         opt.scale = JUST_SCALE;
         break;

      case 'o':
         opt.octaves = strtoul(optarg, NULL, 0);
         if (opt.octaves < 1 || opt.octaves > 12) {
            fprintf(stderr, "Octaves out of range >:(\n");
            print_usage();
            return 1;
         }
         break;

      case '?':
         /* Unknown option, print help and quit */
         print_usage();
         return 1;

      default:
         return 1;
      }
   }

   /* Need exactly 1 mandatory option (frequency) */
   if (argc != optind + 1) {
      print_usage();
      return 1;
   }

   opt.base_freq = strtod(argv[optind], NULL);
   if (opt.base_freq < 10 || opt.base_freq > 9000) {
      fprintf(stderr, "Frequency out of range >:(\n");
      print_usage();
      return 1;
   }

   /* Finally we have base_freq, octaves and scale */

   printf("beepology:\n");
   printf("\t;;\tHL     DE    Octave Freq      Interval\n");

   for (pitch = 0; pitch < (12 * opt.octaves); pitch++) {
      freq = get_freq(opt.base_freq, pitch, opt.scale);
      if (freq > 15000) {
         printf("Warning, freq > 15000, values might overflow\n");
      }
      hl = get_hl(freq);
      de = (uint16_t)(freq + 0.499);
      printf("\tdw\t");
      /* Little endian, swap bytes? */
      /* printf("$%02x%02x, $%02x%02x", */
      /*        hl & 0xff, hl >> 8, */
      /*        de & 0xff, de >> 8); */
      /*        hl & 0xff, hl >> 8, */
      /*        de & 0xff, de >> 8); */
      printf("$%04x, $%04x", hl, de);
      /* Add comment after each note */
      printf("  ; ");
      printf("%d, ", pitch / 12); /* Octave number */
      printf("%.3f Hz, %s\n", freq, intervals[pitch % 12]);
   }

   return 0;
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
