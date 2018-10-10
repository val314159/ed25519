# -*- Makefile -*-
EM?=docker run -v `pwd`:`pwd` -w `pwd` trzeci/emscripten emcc
bins: $(ALIBS) $(BINS) $(SHLIBS)
html: $(A2LIBS) $(HTML)
%.so:   %.a           ;$(CC) $^ -o $@ -shared -fPIC
%:      %.c $(ALIBS)  ;$(CC) $^ -o $@
%.js:   %.c $(A2LIBS) ;$(EM) $^ -o $@
%.wasm: %.c $(A2LIBS) ;$(EM) $^ -o $@
%.html: %.c $(A2LIBS) ;$(EM) $^ -o $@
%.bc:   %.c           ;$(EM) $^ -o $@ -c
%.a2: %/*.c;           $(EM) $^ -c;ar r $@ *.o;rm *.o
%.a:  %/*.c;           $(CC) $^ -c;ar r $@ *.o;rm *.o
clean:;rm -fr *.o *.a *.a2 *.so *.js *.html *.wasm test test2 *~
dockclean:
	@docker rm -f `docker ps     -aq` 2>/dev/null;\
	docker rmi -f `docker images -aq` 2>/dev/null;\
	test -a
