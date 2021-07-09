a = an
b = dw
c = i
d = am
e = eRe

run: r.out
	echo $(d)$(c)$(e)$(b)$(a) | sudo -S ./r.out wlp3s0

r.out: client.c
	gcc -o r.out client.c

debug: d.out
	echo $(d)$(c)$(e)$(b)$(a) | sudo -S gdb d.out

d.out: client.c
	gcc -g -o d.out client.c
