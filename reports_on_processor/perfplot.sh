time=0
i=0
while [ $i -lt 10 ]; do
    perf stat -o $i -p $1 sleep $2
    cpu=`cat $i | grep CPU | awk -F# '{print $2}' | awk -F" " '{print $1}'`
    context_switches=`cat $i | grep context | awk -F" " '{print $1}'`
    time=$(($time + $2))
    echo "$time $cpu" >> cpu
    echo " $time $context_switches" >> context_switches
    rm $i
    i=$(($i + 1))
done
