# Frequently Asked Questions

## Why aren’t all numbers tested?
If k is divisible by 2, let’s say it’s 2l; It’s “trivial” that 2n - k = 2n- 2l = 2 * (2n-1-l). So even numbers will not be tested.

## What’s the largest PRP you have found?
2^395018-224141

## Why are these numbers PRP and not prime?
The specific test used to test these numbers only test if a number is a probable prime (PRP). There is always a small chance that a number is composite but passes the test. There is no test for numbers of the form 2n - k to check if they are definitely prime (unless k=1). If a number is not a probable prime, it is definitely composite, but if a number is a probable prime, it is extremely likely that it is prime (but not impossible for it to be composite).

## What’s the point?
Let’s talk about the Sierpinski and Riesel Problems for a second. The Sierpinski Problem attempts to prove that 78,557 is the smallest number k such that k * 2n + 1 is always composite. The Riesel Problem attempts to prove that 509,203 is the smallest number k such that k * 2n - 1 is always positive. Due to active participation on the BOINC project PrimeGrid, these problems have only 5 and 45 candidates left respectively.
Both of these problems also have duals. When trying to use negative values for n, k * 2n +/- 1 becomes (2|n| +/- k) / (2|n|). We can also try and test if 2n +/- k is a prime/probable prime to prove that the conjecture sort-of works for negative n, too. The Dual Sierpinski Problem already had a distributed computing project and was proven (with PRPs) in 2011. That was over ten years ago. Since there was no organized search for the Dual Riesel Conjecture, we thought that we may as well start one.

## Why do we stop at 509,203?
The only way for a “nontrivial” candidate to appear is through something called a covering set. When increasing n, each factor will repeat. For example, for numbers of the form 2n - k, factors of three will repeat every other time. In some cases, these repeating factors will line up such that every n will have a small factor - this is a covering set. In this case, the smallest candidate to have a covering set is 509,203. It’s covering set is as follows: (see next page)
If n ≡ 0 mod 2, 2n - 509,203 has a factor of 3
So, only odd numbers (n ≡ 1 mod 2) remain.
If n ≡ 3 mod 4, 2n - 509,203 has a factor of 5.
So, only n ≡ 1 mod 4 remain.
If n ≡ 1 mod 3, 2n - 509,203 has a factor of 7.
So, only n ≡ 5 or 9 mod 12 remain.
If n ≡ 5 mod 12, 2n - 509,203 has a factor of 13.
So, only n ≡ 9 mod 12 remain.
If n ≡ 1 mod 8, 2n - 509,203 has a factor of 17.
So, only n ≡ 21 mod 24 remain.
If n ≡ 21 mod 24, 2n - 509,203 has a factor of 241.
So, all numbers of the form 2n - 509,203 are composite.

## How many k’s have no Riesel or Dual Riesel prime/PRP?
There are five: 327671, 342847, 344759, 386801, and 444637.
