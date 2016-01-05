GLPK_ROOT := $(HOME)/Public/gplk/4.57
sample.out: sample.c
	gcc $^ -o $@ -I$(GLPK_ROOT)/include -L$(GLPK_ROOT)/lib -lglpk -lm 

clean:
	rm -rf *.o sample.out
