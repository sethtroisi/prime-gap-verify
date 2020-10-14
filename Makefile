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


OBJS	= primes.o
OUT	= large_sieve
CC	= g++
CFLAGS	= -Wall -Werror -O3 -lgmp
# Need for local gmp / primesieve
LDFLAGS	= -L /usr/local/lib
#LDFLAGS	=
DEFINES =

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OUT)

$(OUT) : %: %.cpp $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(DEFINES)

.PHONY: clean

clean:
	rm -f $(OBJS) $(OUT)
