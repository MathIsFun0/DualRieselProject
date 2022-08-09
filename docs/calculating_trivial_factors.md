# How DSRS calculates trivial factors

These formulas were discovered from data used in Conjectures 'R Us, so much of the credit goes to the work done there.

## Modulos

If $k$ is divisible by the base or some factor $f$ of the base ($k\equiv 0\ \text{mod}\  f$), then $b^n \pm k$ is always composite.

$k$ is of the form $fx$. We have $b^n \pm bx = f(\frac{b}{f}\cdot b^{n-1}\pm x)$ is always composite.

If $k\mp 1$ is divisible by a factor $f$ of $b\pm 1$, $b^n\pm k$ is always composite.

Since $k\ \text{mod} \ f\equiv\pm 1$, and $b^n \ \text{mod}\ f\equiv\mp 1$, $b^n \pm k\ \text{mod}\ f\equiv 0$.

## Algebraic Factors

If the base $b$ is a perfect power $a^m$ for a dual Riesel conjecture, then any $k$ that is also a perfect power $l^m$ will cause there to be a factor since:

$(a^m)^n-l^m=(a^n)^m-l^m$

Let's say $m=px$, where $p$ is a prime. This can be done for all $m\ge 2$.

We now have $(a^{nx})^p-(l^x)^p$.

Substituting some variables to make the expression nicer gives:

$A^p-L^p=(A-L)(A^{p-1}+A^{p-2}L+A^{p-3}L^2+\dots +L^{p-1})$.

Something similar can be done if $m$ isn't a power of 2 in dual Sierpinski conjectures.

## Algebraic Factors unique to Dual Riesel Conjectures

Much of the math behind this is over at [this forum post](https://www.mersenneforum.org/showthread.php?t=11143).

For all prime factors $p$ of $b+1$ that are $1\text{ mod }4$, we have the following:

For $k=m^2,m\equiv x,y\text{ mod }p$ such that $x^2,y^2\equiv -1\text{ mod }p$, $b^n-k$ will always be composite.

If $n$ is odd, then $b^n-k$ is always divisible by $p$, since $k\equiv -1\text{ mod }p$ and $b^n\equiv -1\text{ mod }p$ for odd $n$ since $b\equiv -1\text{ mod }p$, and $-1-(-1)\equiv 0\text{ mod }p$.

If $n$ is even, then for $k=m^2$ and $n=2q$ we have $b^n-k=b^{2q}-m^2=(b^q+m)(b^q-m)$.

Something similar happens when $k=fm^2$ if $f$ is a factor of $b$ and $\frac{b}{f}$ is a perfect square. However, it's not important since $k$ is divisible by $b$ and will have already been filtered out.