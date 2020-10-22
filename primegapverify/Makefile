# Copyright 2020 Seth Troisi
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


OBJS	= verify/primes.o verify/sieve_util.o
OUT	= large_sieve
CC	= g++
CFLAGS	= -Wall -Werror -O3
# Need for local gmp / primesieve
LDFLAGS	= -L /usr/local/lib -lgmp -lprimesieve
#LDFLAGS	= -lgmp -lprimesieve
DEFINES =

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

all: $(OUT)

large_sieve : %: %.cpp $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(DEFINES)

.PHONY: clean

clean:
	rm -rf $(OBJS) $(OUT) *.so __pycache__/ test/__pycache__ pfgw.ini pfgw.log
