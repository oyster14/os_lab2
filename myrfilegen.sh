for i in `seq 0 8`; do
    ./sched -sF -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_F
    ./sched -sL -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_L
    ./sched -sS -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_S
    ./sched -sR3 -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_R3
    ./sched -sR7 -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_R7
    ./sched -sR11 -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_R11
    ./sched -sP3 -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_P3
    ./sched -sP5:2 -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_P5_2
    ./sched -sP4:34 -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_P4_34
    ./sched -sP6:32 -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_P6_32
    ./sched -sE3:9 -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_E3_9
    ./sched -sE7:19 -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_E7_19
    ./sched -sE5:28 -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_E5_28
    ./sched -sE3:36 -vp ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/out_${i}_E3_36
    echo "F ${i}"
    diff ./myrfile1_out/out_${i}_F ./lab2_assign/myrfile1_out/out_${i}_F
    echo "L ${i}"
    diff ./myrfile1_out/out_${i}_L ./lab2_assign/myrfile1_out/out_${i}_L
    echo "S ${i}"
    diff ./myrfile1_out/out_${i}_S ./lab2_assign/myrfile1_out/out_${i}_S
    echo "R3 ${i}"
    diff ./myrfile1_out/out_${i}_R3 ./lab2_assign/myrfile1_out/out_${i}_R3
    echo "R7 ${i}"
    diff ./myrfile1_out/out_${i}_R7 ./lab2_assign/myrfile1_out/out_${i}_R7
    echo "R11 ${i}"
    diff ./myrfile1_out/out_${i}_R11 ./lab2_assign/myrfile1_out/out_${i}_R11
    echo "P3 ${i}"
    diff ./myrfile1_out/out_${i}_P3 ./lab2_assign/myrfile1_out/out_${i}_P3
    echo "P5_2 ${i}"
    diff ./myrfile1_out/out_${i}_P5_2 ./lab2_assign/myrfile1_out/out_${i}_P5_2
    echo "P4_34 ${i}"
    diff ./myrfile1_out/out_${i}_P4_34 ./lab2_assign/myrfile1_out/out_${i}_P4_34
    echo "P6_32 ${i}"
    diff ./myrfile1_out/out_${i}_P6_32 ./lab2_assign/myrfile1_out/out_${i}_P6_32
    echo "E3_9 ${i}"
    diff ./myrfile1_out/out_${i}_E3_9 ./lab2_assign/myrfile1_out/out_${i}_E3_9
    echo "E7_19 ${i}"
    diff ./myrfile1_out/out_${i}_E7_19 ./lab2_assign/myrfile1_out/out_${i}_E7_19
    echo "E5_28 ${i}"
    diff ./myrfile1_out/out_${i}_E5_28 ./lab2_assign/myrfile1_out/out_${i}_E5_28
    echo "E3_36 ${i}"
    diff ./myrfile1_out/out_${i}_E3_36 ./lab2_assign/myrfile1_out/out_${i}_E3_36
done



