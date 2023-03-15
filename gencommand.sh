# for i in `seq 0 7`; do
#     ./sched -sF -v ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/myout_${i}_F
#     diff ./myrfile1_out/myout_${i}_F ./lab2_assign/myrfile1_out/out_${i}_F
#     ./sched -sL -v ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/myout_${i}_L
#     diff ./myrfile1_out/myout_${i}_L ./lab2_assign/myrfile1_out/out_${i}_L
#     ./sched -sS -v ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/myout_${i}_S
#     diff ./myrfile1_out/myout_${i}_S ./lab2_assign/myrfile1_out/out_${i}_S
# done

for i in `seq 0 7`; do
    # echo "F ${i}"
    ./sched -sF -vp ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_F
    diff ./myout/myout_${i}_F ./lab2_assign/refout_vp/out_${i}_F
    # echo "L ${i}"
    ./sched -sL -vp ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_L
    diff ./myout/myout_${i}_L ./lab2_assign/refout_vp/out_${i}_L
    # echo "S ${i}"
    ./sched -sS -vp ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_S
    diff ./myout/myout_${i}_S ./lab2_assign/refout_vp/out_${i}_S
    # echo "R2 ${i}"
    ./sched -sR2 -vp ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_R2
    diff ./myout/myout_${i}_R2 ./lab2_assign/refout_vp/out_${i}_R2
    # echo "R5 ${i}"
    ./sched -sR5 -vp ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_R5
    diff ./myout/myout_${i}_R5 ./lab2_assign/refout_vp/out_${i}_R5
    # echo "P2 ${i}"
    ./sched -sP2 -vp ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_P2
    diff ./myout/myout_${i}_P2 ./lab2_assign/refout_vp/out_${i}_P2
    # echo "P5:3 ${i}"
    ./sched -sP5:3 -vp ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_P5_3
    diff ./myout/myout_${i}_P5_3 ./lab2_assign/refout_vp/out_${i}_P5_3
    ./sched -sE2:5 -vp ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_E2_5
    diff ./myout/myout_${i}_E2_5 ./lab2_assign/refout_vp/out_${i}_E2_5
    ./sched -sE4 -vp ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_E4
    diff ./myout/myout_${i}_E4 ./lab2_assign/refout_vp/out_${i}_E4
done



