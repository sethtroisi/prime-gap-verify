set -eux

yum install -y curl lzip cmake

curl -O https://gmplib.org/download/gmp/gmp-6.2.0.tar.lz
tar -xvf gmp-6.2.0.tar.lz
cd gmp-6.2.0 && ./configure && make -j2 && make install && cd ..

curl -LJO https://github.com/kimwalisch/primesieve/archive/v7.5.tar.gz
tar -xvf primesieve-7.5.tar.gz
cd primesieve-7.5 && cmake . && make -j2 && make install && cd ..
