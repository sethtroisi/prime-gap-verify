from setuptools import setup, Extension, find_packages

ext = Extension(
    "verify",
    libraries=["gmp", "primesieve"],
    library_dirs=["/usr/local/lib"],
    sources=[
        "primegapverify/verify/verifymodule.cpp",
        "primegapverify/verify/verify.cpp",
        "primegapverify/verify/primes.cpp",
        "primegapverify/verify/sieve_util.cpp",
    ],
    undef_macros=['NDEBUG'],
)

# Load version without trying to load module
exec(open("primegapverify/_version.py").read())

setup(
    name="primegapverify",
    version=__version__,
    author="Seth Troisi",
    author_email="sethtroisi@google.com",
    license="Apache License 2.0",
    url="https://github.com/sethtroisi/prime-gap-verify",
    description="Helpful tools for verifying prime gaps",
    keywords="gmp gmpy2 prime-gaps",
    packages=find_packages(),
    ext_modules=[ext],
    install_requires=[
#        "gmpy2 >= 2.1.0b5"
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
