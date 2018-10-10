# -*- Makefile -*-
ALIBS= ed25519.a
A2LIBS=ed25519.a2
SHLIBS=ed25519.so
BINS=test test2
HTML=test.html test2.html
all: bins html
include makefile.inc
