# for i in `seq 0 7`; do
#     ./sched -sF -v ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/myout_${i}_F
#     diff ./myrfile1_out/myout_${i}_F ./lab2_assign/myrfile1_out/out_${i}_F
#     ./sched -sL -v ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/myout_${i}_L
#     diff ./myrfile1_out/myout_${i}_L ./lab2_assign/myrfile1_out/out_${i}_L
#     ./sched -sS -v ./lab2_assign/input${i} ./myrfile1 > ./myrfile1_out/myout_${i}_S
#     diff ./myrfile1_out/myout_${i}_S ./lab2_assign/myrfile1_out/out_${i}_S
# done

for i in `seq 0 7`; do
    ./sched -sF -v ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_F
    diff ./myout/myout_${i}_F ./lab2_assign/refout_vp/out_${i}_F
    ./sched -sL -v ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_L
    diff ./myout/myout_${i}_L ./lab2_assign/refout_vp/out_${i}_L
    ./sched -sS -v ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_S
    diff ./myout/myout_${i}_S ./lab2_assign/refout_vp/out_${i}_S
    ./sched -sR2 -v ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_R2
    diff ./myout/myout_${i}_R2 ./lab2_assign/refout_vp/out_${i}_R2
    ./sched -sR5 -v ./lab2_assign/input${i} ./rfile > ./myout/myout_${i}_R5
    diff ./myout/myout_${i}_R5 ./lab2_assign/refout_vp/out_${i}_R5
done

