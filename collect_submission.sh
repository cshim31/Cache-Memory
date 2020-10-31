#!/bin/bash
rm -f lab2_submission.tar.gz
tar -czvf lab2_submission.tar.gz cachesim.c cachesim.h lrustack.c lrustack.h
echo "Done!"
echo "The files that will be submitted are:"
tar -ztvf lab2_submission.tar.gz
