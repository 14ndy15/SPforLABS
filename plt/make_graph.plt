
set term postscript enhanced color eps dl 2.5
filenameoutput="prob.eps"
set output filenameoutput
set termopt enhanced
reset
set xlabel "free Spins"
set ylabel "Prob" 
set xrange[0:1]
set yrange[0:1]
plot "results/pro20.txt" using ($1/20):2 w lp title "Prob find GS n=20" \
, "results/pro30.txt" using ($1/30):2 w lp title "Prob find GS n=30" \
, "results/pro40.txt" using ($1/40):2 w lp title "Prob find GS n=40" \
, "results/pro50.txt" using ($1/50):2 w lp title "Prob find GS n=50" \
, "results/pro60.txt" using ($1/60):2 w lp title "Prob find GS n=60" \
, "results/pro66.txt" using ($1/66):2 w lp title "Prob find GS n=66"

set term postscript enhanced color eps dl 2.5
filenameoutput="decimate_09.eps"
set output filenameoutput
set termopt enhanced
reset
set xlabel "free Spins"
set ylabel "Prob" 
set xrange[0:1]
set yrange[0:1]
plot "results/pro20.txt" using ($1/20):2 w lp title "Prob find GS n=20" \
, "results/pro30.txt" using ($1/30):2 w lp title "Prob find GS n=30" \
, "results/pro40.txt" using ($1/40):2 w lp title "Prob find GS n=40" \
, "results/pro50.txt" using ($1/50):2 w lp title "Prob find GS n=50" \
, "results/pro60.txt" using ($1/60):2 w lp title "Prob find GS n=60" \
, "results/pro66.txt" using ($1/66):2 w lp title "Prob find GS n=66" \
, 'data.txt' index 0 using ($2/20):($7/20) smooth unique with lp title 'max(rN, rP, rZ)>0.9 (20)' \
, 'data.txt' index 1 using ($2/30):($7/30) smooth unique with lp title 'max(rN, rP, rZ)>0.9 (30)' \
, 'data.txt' index 2 using ($2/40):($7/40) smooth unique with lp title 'max(rN, rP, rZ)>0.9 (40)' \
, 'data.txt' index 3 using ($2/50):($7/50) smooth unique with lp title 'max(rN, rP, rZ)>0.9 (50)' \
, 'data.txt' index 4 using ($2/60):($7/60) smooth unique with lp title 'max(rN, rP, rZ)>0.9 (60)'

set term postscript enhanced color eps dl 2.5
filenameoutput="decimate_07.eps"
set output filenameoutput
set termopt enhanced
reset
set xlabel "free Spins"
set ylabel "Prob" 
set xrange[0:1]
set yrange[0:1]
plot "results/pro20.txt" using ($1/20):2 w lp title "Prob find GS n=20" \
, "results/pro30.txt" using ($1/30):2 w lp title "Prob find GS n=30" \
, "results/pro40.txt" using ($1/40):2 w lp title "Prob find GS n=40" \
, "results/pro50.txt" using ($1/50):2 w lp title "Prob find GS n=50" \
, "results/pro60.txt" using ($1/60):2 w lp title "Prob find GS n=60" \
, "results/pro66.txt" using ($1/66):2 w lp title "Prob find GS n=66" \
, 'data.txt' index 0 using ($2/20):($6/20) smooth unique with lp title 'max(rN, rP, rZ)>0.7 (20)' \
, 'data.txt' index 1 using ($2/30):($6/30) smooth unique with lp title 'max(rN, rP, rZ)>0.7 (30)' \
, 'data.txt' index 2 using ($2/40):($6/40) smooth unique with lp title 'max(rN, rP, rZ)>0.7 (40)' \
, 'data.txt' index 3 using ($2/50):($6/50) smooth unique with lp title 'max(rN, rP, rZ)>0.7 (50)' \
, 'data.txt' index 4 using ($2/60):($6/60) smooth unique with lp title 'max(rN, rP, rZ)>0.7 (60)'


set term postscript enhanced color eps dl 2.5
filenameoutput="decimate_06.eps"
set output filenameoutput
set termopt enhanced
reset
set xlabel "free Spins"
set ylabel "Prob" 
set xrange[0:1]
set yrange[0:1]
plot "results/pro20.txt" using ($1/20):2 w lp title "Prob find GS n=20" \
, "results/pro30.txt" using ($1/30):2 w lp title "Prob find GS n=30" \
, "results/pro40.txt" using ($1/40):2 w lp title "Prob find GS n=40" \
, "results/pro50.txt" using ($1/50):2 w lp title "Prob find GS n=50" \
, "results/pro60.txt" using ($1/60):2 w lp title "Prob find GS n=60" \
, "results/pro66.txt" using ($1/66):2 w lp title "Prob find GS n=66" \
, 'data.txt' index 0 using ($2/20):($5/20) smooth unique with lp title 'max(rN, rP, rZ)>0.6 (20)' \
, 'data.txt' index 1 using ($2/30):($5/30) smooth unique with lp title 'max(rN, rP, rZ)>0.6 (30)' \
, 'data.txt' index 2 using ($2/40):($5/40) smooth unique with lp title 'max(rN, rP, rZ)>0.6 (40)' \
, 'data.txt' index 3 using ($2/50):($5/50) smooth unique with lp title 'max(rN, rP, rZ)>0.6 (50)' \
, 'data.txt' index 4 using ($2/60):($5/60) smooth unique with lp title 'max(rN, rP, rZ)>0.6 (60)'

set term postscript enhanced color eps dl 2.5
filenameoutput="decimate_05.eps"
set output filenameoutput
set termopt enhanced
reset
set xlabel "free Spins"
set ylabel "Prob" 
set xrange[0:1]
set yrange[0:1]
plot "results/pro20.txt" using ($1/20):2 w lp title "Prob find GS n=20" \
, "results/pro30.txt" using ($1/30):2 w lp title "Prob find GS n=30" \
, "results/pro40.txt" using ($1/40):2 w lp title "Prob find GS n=40" \
, "results/pro50.txt" using ($1/50):2 w lp title "Prob find GS n=50" \
, "results/pro60.txt" using ($1/60):2 w lp title "Prob find GS n=60" \
, "results/pro66.txt" using ($1/66):2 w lp title "Prob find GS n=66" \
, 'data.txt' index 0 using ($2/20):($4/20) smooth unique with lp title 'max(rN, rP, rZ)>0.5 (20)' \
, 'data.txt' index 1 using ($2/30):($4/30) smooth unique with lp title 'max(rN, rP, rZ)>0.5 (30)' \
, 'data.txt' index 2 using ($2/40):($4/40) smooth unique with lp title 'max(rN, rP, rZ)>0.5 (40)' \
, 'data.txt' index 3 using ($2/50):($4/50) smooth unique with lp title 'max(rN, rP, rZ)>0.5 (50)' \
, 'data.txt' index 4 using ($2/60):($4/60) smooth unique with lp title 'max(rN, rP, rZ)>0.5 (60)'

set term postscript enhanced color eps dl 2.5
filenameoutput="decimate_01.eps"
set output filenameoutput
set termopt enhanced
reset
set xlabel "free Spins"
set ylabel "Prob" 
set xrange[0:1]
set yrange[0:1]
plot "results/pro20.txt" using ($1/20):2 w lp title "Prob find GS n=20" \
, "results/pro30.txt" using ($1/30):2 w lp title "Prob find GS n=30" \
, "results/pro40.txt" using ($1/40):2 w lp title "Prob find GS n=40" \
, "results/pro50.txt" using ($1/50):2 w lp title "Prob find GS n=50" \
, "results/pro60.txt" using ($1/60):2 w lp title "Prob find GS n=60" \
, "results/pro66.txt" using ($1/66):2 w lp title "Prob find GS n=66" \
, 'data.txt' index 0 using ($2/20):($3/20) smooth unique with lp title 'abs(rN, rP)>0.1 (20)' \
, 'data.txt' index 1 using ($2/30):($3/30) smooth unique with lp title 'abs(rN, rP)>0.1 (30)' \
, 'data.txt' index 2 using ($2/40):($3/40) smooth unique with lp title 'abs(rN, rP)>0.1 (40)' \
, 'data.txt' index 3 using ($2/50):($3/50) smooth unique with lp title 'abs(rN, rP)>0.1 (50)' \
, 'data.txt' index 4 using ($2/60):($3/60) smooth unique with lp title 'abs(rN, rP)>0.1 (60)'
