#Parallelize
*Simple shell command parallelizer*

##Synopsis
Useful if you have a multiprocessor computer and need to run a batch of
single-threaded CPU-intensive tasks, such as image processing or 
media encoding.

Pipe a list of commands to stdin.
The only argument is the number of simultaneous processes to run.

`parallelize` will execute that number of the input commands simultaneously
until the list is exhausted.

##Building and running
`gcc -pthread parallelize.c -o parallelize`

**Demo:**
`(for i in 1 2 3 4 5 6 7 8 9 10; do echo "echo 'Delay $i'; sleep $i ; \
echo 'Done $i'" ; done) | ./parallelize 4`

##Author
By Marco Arment <me@marco.org>, released into the public domain with no
restrictions or conditions. I'd appreciate bug reports, though.

Tweaks and maintenance by Fredrick Galoso <fgaloso@stackd.com>. 