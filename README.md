# Prime Gap Verify

Python library to verify prime gaps for
[prime-gap-project](https://github.com/primegap-list-project/prime-gap-list/)

## primegapverify

```python
>>> import primegapverify
>>> primegapverify.sieve(101, 20, 20)
[False, False, True, False, False, True, False, True, True, True, True]

>>> [101 + 2 * i for i, v in enumerate(primegapverify.sieve(101, 100, 20)) if v is False]
[101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199]

>>> import sympy
>>> list(sympy.primerange(100, 101+100+1))
[101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199]
```

## Testing

If I remove `__init__.py` everything works but I need `__init__.py` for `python setup.py install`
Not sure how to fix. Wasted 3 hours already.
```bash
cd primegapverify
python -m pytest
```

## Primes

Either [kimwalisch's](https://github.com/kimwalisch/)
excellent [primesieve](https://github.com/kimwalisch/primesieve)
or a hand rolled version based on a Sieve of Eratosthenes.

to use handrolled prime iterator make with `make DEFINES=-DHANDROLLED`

```bash
sudo apt install libprimesieve-dev

# for gmpy2
sudo apt install libgmp-dev libmpc-dev libmpfr-dev
sudo pip install gmpy2==2.1.0b5
```

## Prime Test

[GMPlib's](https://gmplib.org/)
[`mpz_probab_prime_p`](https://gmplib.org/manual/Number-Theoretic-Functions#Number-Theoretic-Functions)
or [OpenPFGW](https://sourceforge.net/projects/openpfgw/).


## TODO

* [ ] `isPrimeLarge` using pfgw
* [ ] Estimated PRP/s using benchmark & interpolation
* [x] Parse string ("123 * 73# / 5# - 1000" to primorial form)
