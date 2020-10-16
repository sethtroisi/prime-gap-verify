from distutils.core import setup, Extension

ext = Extension(
    "primegapverify",
    language="c++",
    libraries = ["gmp", "primesieve"],
    sources = [
        "primegapverifymodule.cpp",
        "primegapverify.cpp",
        "primes.cpp",
        "sieve_util.cpp",
    ],
)

setup(
	name = "primegapverify",
	version = "0.1",
	ext_modules = [ext],
);
