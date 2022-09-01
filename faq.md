# Frequently Asked Questions

## Why aren’t all numbers tested?
If k is divisible by $$ \mathsf{2} $$, let’s say it’s $$ \mathsf{2l} $$; It’s “trivial” that $$ \mathsf{2^n - k = 2^n - 2l = 2 * (2^{n-1}-l)} $$. So even numbers will not be tested.

## What’s the largest PRP you have found?
$$ \mathsf{2^{395018}-224141} $$

## Why are these numbers PRP and not prime?
The specific test used to test these numbers only test if a number is a probable prime (PRP). There is always a small chance that a number is composite but passes the test. There is no test for numbers of the form $$ \mathsf{2^n - k} $$ to check if they are definitely prime (unless $$ \mathsf{k=1} $$). If a number is not a probable prime, it is definitely composite, but if a number is a probable prime, it is extremely likely that it is prime (but not impossible for it to be composite).

## What’s the point?
Let’s talk about the Sierpinski and Riesel Problems for a second. The Sierpinski Problem attempts to prove that $$ \mathsf{78557} $$ is the smallest number $$ \mathsf{k} $$ such that $$ \mathsf{k\cdot 2^n + 1} $$ is always composite. The Riesel Problem attempts to prove that $$ \mathsf{509203} $$ is the smallest number $$ \mathsf{k} $$ such that $$ \mathsf{k\cdot 2^n - 1} $$ is always positive. Due to active participation on the BOINC project PrimeGrid, these problems have only $$ \mathsf{5} $$ and $$ \mathsf{45} $$ candidates left respectively.
Both of these problems also have duals. When trying to use negative values for $$ \mathsf{n} $$, $$ \mathsf{k \cdot 2^n \pm 1} $$ becomes $$ \mathsf{\frac{2^{|n|} \pm k}{2^{|n|}}} $$. We can also try and test if $$ \mathsf{2n \pm k} $$ is a prime/probable prime to prove that the conjecture sort-of works for negative $$ \mathsf{n} $$, too. The Dual Sierpinski Problem already had a distributed computing project and was proven (with PRPs) in 2011. That was over ten years ago. Since there was no organized search for the Dual Riesel Conjecture, we thought that we may as well start one.

## Why do we stop at 509,203?
The only way for a “nontrivial” candidate to appear is through something called a covering set. When increasing $$ \mathsf{n} $$, each factor will repeat. For example, for numbers of the form $$ \mathsf{2n - k} $$, factors of three will repeat every other time. In some cases, these repeating factors will line up such that every $$ \mathf{n} $$ will have a small factor - this is a covering set. In this case, the smallest candidate to have a covering set is $$ \mathsf{509,203}. It’s covering set is as follows: (see next page)
If $$ \mathsf{n ≡ 0 mod 2}, \mathsf{2n - 509,203} $$ has a factor of $$ \mathsf{3} $$
So, only odd numbers ($$ \mathsf{n ≡ 1 mod 2} $$) remain.
If $$ \mathsf{n ≡ 3 mod 4, 2n - 509,203} $$ has a factor of $$ \mathsf{5} $$.
So, only $$ \mathsf{n ≡ 1 mod 4} $$ remain.
If $$ \mathsf{n ≡ 1 mod 3, 2n - 509,203} $$ has a factor of $$ \mathsf{7} $$.
So, only $$ \mathsf{n ≡ 5 or 9 mod 12} $$ remain.
If $$ \mathsf{n ≡ 5 mod 12, 2n - 509,203} $$ has a factor of $$ \mathsf{13} $$.
So, only $$ \mathsf{n ≡ 9 mod 12} $$ remain.
If $$ \mathf{n ≡ 1 mod 8, 2n - 509,203} $$ has a factor of $$ \mathsf{17} $$.
So, only $$ \mathsf{n ≡ 21 mod 24} $$ remain.
If $$ \mathsf{n ≡ 21 mod 24, 2n - 509,203} $$ has a factor of $$ \mathsf{241} $$.
So, all numbers of the form $$ \mathf{2n - 509,203} $$ are composite.

## How many k’s have no Riesel or Dual Riesel prime/PRP?
There are five: $$ \mathsf{327671, 342847, 344759, 386801, and 444637} $$.
