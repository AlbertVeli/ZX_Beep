/* All Rights Reversed - No Rights Reserved */

#include <stdio.h>
#include <math.h>

/* Equal Temperament multiplication factors, a^n
 * n = half steps away from base_freq
 * freq[n] = base_freq * a^n
 * a = 2^(1/12)
 * multiplication factor = 2^(n/12)
 */

int main(void)
{
   int i;

   for (i = 0; i < 12; i++) {
      printf("%.12f\n", exp2(i / 12.0));
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
