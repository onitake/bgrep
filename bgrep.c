/*
 * bgrep.c - binary grep
 *
 * Copyright (c) 2018, Gregor Riepl <onitake@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("Usage: bgrep file.ext string\n");
		exit(-1);
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("Can't open file");
	}

	struct stat statbuf;
	memset(&statbuf, 0, sizeof(statbuf));

	if (fstat(fd, &statbuf) < 0) {
		perror("Can't stat file");
	}

	char *mem = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (mem == NULL) {
		perror("Can't mmap file");
	}

	size_t matchlen = strlen(argv[2]);

	for (off_t o = 0; o < statbuf.st_size;) {
		if (memcmp(&mem[o], argv[2], matchlen) == 0) {
			printf("%s found at %zu [0x%08zx]\n", argv[2], (size_t) o, (size_t) o);
			o += matchlen;
		} else {
			o++;
		}
	}

	if (munmap(mem, statbuf.st_size) < 0) {
		perror("Can't munmap page");
	}

	close(fd);

	return 0;
}
