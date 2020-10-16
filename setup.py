from setuptools import setup, Extension, find_packages

ext = Extension(
    "verify",
    libraries=["gmp", "primesieve"],
    sources=[
        "verify/verifymodule.cpp",
        "verify/verify.cpp",
        "verify/primes.cpp",
        "verify/sieve_util.cpp",
    ],
)

setup(
    name="primegapverify",
    version="0.3",
    author="Seth Troisi",
    author_email="sethtroisi@google.com",
    license="Apache License 2.0",
    url="https://github.com/sethtroisi/prime-gap-verify",
    description="Helpful tools for verifying prime gaps",
    keywords="gmp gmpy2 prime-gaps",
    packages=find_packages(),
    ext_modules=[ext],
    install_requires=[
        "gmpy2 >= 2.1.0b5"
    ],
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: Apache Software License",
        "Programming Language :: C",
        "Programming Language :: Python :: 3",
        "Operating System :: POSIX",
        "Topic :: Scientific/Engineering :: Mathematics",
    ]
)
