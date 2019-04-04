#!/bin/bash
#SBATCH --account=def-ikotsire
#SBATCH --time=00:30:00
#SBATCH --mem-per-cpu=400M
#SBATCH --job-name=SP-InitialCond
#SBATCH --output=start-jobs.out
#SBATCH --error=start-jobs-error.out

#Solve initialCondition
for n in {20..60..10}; do
    #in SP
        touch SP-n-$n-initialCond.sh
        echo \#!/bin/bash > SP-n-$n-initialCond.sh

        echo \#SBATCH --account=def-ikotsire >> SP-n-$n-initialCond.sh
        #tres dias
        echo \#SBATCH --time=24:00:00 >> SP-n-$n-initialCond.sh
        #requesting a 4gb
        echo \#SBATCH --mem-per-cpu=4000M >> SP-n-$n-initialCond.sh
        #name of the job
        echo \#SBATCH --job-name=SP-n-$n >> SP-n-$n-initialCond.sh
        #set output file
        echo \#SBATCH --output=results/SP-n-$n-initialCond.txt >> SP-n-$n-initialCond.sh
        #set error file
        echo \#SBATCH --error=errors/SP-n-$n-initialCond.txt >> SP-n-$n-initialCond.sh
        #send email at finish
        echo \#SBATCH --mail-user=landy@fisica.uh.cu >> SP-n-$n-initialCond.sh
        echo \#SBATCH --mail-type=END >> SP-n-$n-initialCond.sh

        echo ./sp.exe -n $n -t 50 -a 0.75 -initialCond >> SP-n-$n-initialCond.sh
        sbatch SP-n-$n-initialCond.sh
        #it necessary wait a little amount of seconds for send another job
        sleep 4
done
