for i in `seq 0 6`; do
    ./sched -sS -v ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_S
    diff ./myout/myout_${i}_S ./lab2_assign/refout_vp/out_${i}_S
done