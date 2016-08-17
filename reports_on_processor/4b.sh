#! /bin/sh
m_sort(){
#    stat --printf="%s\n" $1;
    if [ `stat --printf="%s" $1` -lt $2 ]
    then
	sort -g $1 > tmp;
	cat tmp>$1;
    else
	no_of_lines=`cat "$1" | wc -l`;
	first_file=$(($no_of_lines/2));
	second_file=$(($no_of_lines - $first_file));
	local n_f1 n_f2 a;
	n_f1=$(($1*2));
	n_f2=$((($1*2)+1));
	head $1 -n $first_file > $n_f1;
	tail $1 -n $second_file > $n_f2;
	a=$1;
	m_sort $n_f1 $2;
	m_sort $n_f2 $2;
	sort -m -g $n_f1 $n_f2 > $a;
    fi
}

mkdir -p .dir_for_sort;
cp $1 .dir_for_sort;
cd .dir_for_sort;
mv $1 1;
m_sort 1 $2;
cat 1;
