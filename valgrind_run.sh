#!/bin/sh
valgrind --tool=memcheck --leak-check=full  --track-origins=yes --show-leak-kinds=all  ./main testinput1.in
