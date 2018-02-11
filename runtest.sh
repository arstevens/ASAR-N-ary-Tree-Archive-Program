#!/bin/bash
for TEST in `seq 1 1 100`; do
	./a.out
	echo `ls -lh | grep outtest`
done
