# Prime Gap Verify

Code to verify prime gaps for [prime-gap-project](https://github.com/primegap-list-project/prime-gap-list/)

## Primes

Either [kimwalisch's](https://github.com/kimwalisch/)
excellent [primesieve](https://github.com/kimwalisch/primesieve)
or a hand rolled version based on a Sieve of Eratosthenes.

to use handrolled prime iterator make with `make DEFINES=-DHANDROLLED`

```bash
sudo apt install libprimesieve-dev
```

## Prime Test

[GMPlib's](https://gmplib.org/)
[`mpz_probab_prime_p`](https://gmplib.org/manual/Number-Theoretic-Functions#Number-Theoretic-Functions)
or [OpenPFGW](https://sourceforge.net/projects/openpfgw/).

## Example

```bash
$ make
$ time ./large_sieve 11159 6661 278784870 -58818 110658 1> tests.txt
sieving 11159 * 6661# / 278784870 + [-58818, 51840]
bits:  9448  gap: 110658  limit: 1182348479
expect ~~2974 remaining
109309 / 110658 = 98.78 composite, 1349 remaining (primes 10508)
109919 / 110658 = 99.33 composite, 739 remaining (primes 59610170)

real	0m8.097s

$ head -n 5 tests.txt
11159 * 6661# / 278784870 + -58818
11159 * 6661# / 278784870 + -58746
11159 * 6661# / 278784870 + -58418
11159 * 6661# / 278784870 + -58358
11159 * 6661# / 278784870 + -58346

$ tail -n5 tests.txt
11159 * 6661# / 278784870 + 51582
11159 * 6661# / 278784870 + 51654
11159 * 6661# / 278784870 + 51694
11159 * 6661# / 278784870 + 51714
11159 * 6661# / 278784870 + 51840

$ rm -f pfgw.log pfgw.ini; time pfgw64 -f0 tests.txt
PFGW Version 4.0.1.64BIT.20191203.x86_Dev [GWNUM 29.8]

No factoring at all, not even trivial division
11159 * 6661# / 278784870 + -58818 is 3-PRP! (0.0926s+0.0002s)
11159 * 6661# / 278784870 + 51840 is 3-PRP! (0.0647s+0.0001s)


real	0m49.659s
```
