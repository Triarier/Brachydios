#!/bin/sh
aflag=0
pflag=0
dflag=0
while getopts ":apd" opt; do
  case $opt in
    a)
      aflag=1
      ;;
    p)
      pflag=1
      ;;
    d)
      dflag=1
      ;;
  esac
done

direct=${@: $[OPTIND+1]:1}
if [ "$direct"="" ]; then
  direct=results
fi
prog=${@: $OPTIND:1}
if [ -d "$direct" ]; then
rm -R $direct
fi
mkdir $direct
if [ "$aflag" -eq 0 ]; then
  if [ "$pflag" -eq 0 ]; then
    echo
    echo Parameter:
    echo
    echo TIME LINE
    time ./rathalos $prog 1> $direct/result_l.log
    echo TIME CHAR
    time ./rathian $prog 1> $direct/result_c.log
  else
    echo
    echo Parameter: p
    echo
    echo TIME LINE
    time ./rathalos -p $prog 1> $direct/result_l.log
    echo
    echo TIME CHAR
    time ./rathian -p $prog 1> $direct/result_c.log
  fi
else
  if [ "$pflag" -eq 0 ]; then
    echo
    echo Parameter: a
    echo
    echo TIME LINE
    time ./rathalos -a $prog 1> $direct/result_l.log
    echo
    echo TIME CHAR
    time ./works/rathian -a $prog 1> $direct/result_c.log
  else
    echo
    echo Parameter: ap
    echo
    echo TIME LINE
    time ./rathalos -ap $prog 1> $direct/result_l.log
    echo
    echo TIME CHAR
    time ./rathian -ap $prog 1> $direct/result_c.log
  fi
fi
if [ "$dflag" -eq 1 ]; then
  vimdiff $direct/result_l.log $direct/result_c.log

 
fi

