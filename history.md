# This project occured in a few stages.

## 1: Tested up to n=140k on a single computer (May 11 - August 12, 2021)
## 2: 50 k’s removed using the PRPTop list.
## 3: Tested up to n=300k on a single computer. (August 12 - November 4, 2021)
# Another Discovery
According to [this](https://oeis.org/history/view?seq=A216189&v=9) former revision on the OEIS, k’s under $$ \mathsf{131,072} $$ have been tested to $$ \mathsf{n=1M} $$, however on PRPTop the only primes found for these are under $$ \mathsf{n=500k} $$. No new k’s are eliminated from this. An anonymous contributor claims to have also tested “larger n’s” (seemingly up to $$ \mathsf{n=191,759} $$, the largest k displayed on OEIS) up to $$ \mathsf{n=1M} $$.
At the same time, a faster program (LLR2) to test candidates was found, so a major rewrite of the algorithm was put into place.
