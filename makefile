filename=simpledb
objs=$(filename).o
cfile=$(filename).c

myapp:$(objs)
	gcc -o myapp $(objs)


$(objs):$(cfile)
	gcc -c $(cfile)
	
RM=del

clean:
	-$(RM) myapp.exe $(objs)