from distutils.core import setup, Extension

ext = Extension(
    "_verify",
    libraries = ["gmp", "primesieve"],
    sources = [
        "verify/verifymodule.cpp",
        "verify/verify.cpp",
        "verify/primes.cpp",
        "verify/sieve_util.cpp",
    ],
)

setup(
	name="primegapverify",
	version="0.2",
    author="Seth Troisi",
    author_email="sethtroisi@google.com",
    license='Apache License 2.0',
    url="https://github.com/sethtroisi/prime-gap-verify",
    description="Helpful tools for verifying prime gaps",
    keywords="gmp gmpy2 prime-gaps",
    packages = ["verify"],
    ext_modules = [ext],
);
