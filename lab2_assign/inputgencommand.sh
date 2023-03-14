for i in `seq 0 7`; do
    ./sched -sF -v ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_F
    ./sched -sL -v ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_L
    ./sched -sS -v ./input${i} ../myrfile1 > ./myrfile1_out/out_${i}_S
done