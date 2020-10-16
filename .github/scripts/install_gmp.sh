set -eux

if [ ! -f .cached_ci_done ]; then
    yum install -y wget lzip cmake

    wget https://github.com/kimwalisch/primesieve/archive/v7.5.tar.gz
    tar -xf v7.5.tar.gz
    cd primesieve-7.5 && cmake . && make -j2 && make install && cd ..
    sleep 1

    wget https://gmplib.org/download/gmp/gmp-6.2.0.tar.lz
    tar -xf gmp-6.2.0.tar.lz
    cd gmp-6.2.0 && ./configure && make -j2 >/dev/null && make install && cd ..

    cpp -v /dev/null

    ls /usr/local/include/

    touch .cached_ci_done
fi
