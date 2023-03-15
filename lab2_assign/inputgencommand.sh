for i in `seq 0 8`; do
    ./sched -sF -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_F
    ./sched -sL -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_L
    ./sched -sS -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_S
    ./sched -sR3 -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_R3
    ./sched -sR7 -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_R7
    ./sched -sR11 -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_R11
    ./sched -sP3 -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_P3
    ./sched -sP5:2 -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_P5_2
    ./sched -sP4:34 -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_P4_34
    ./sched -sP6:32 -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_P6_32
    ./sched -sE3:9 -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_E3_9
    ./sched -sE7:19 -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_E7_19
    ./sched -sE5:28 -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_E5_28
    ./sched -sE3:36 -vp ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_E3_36
done