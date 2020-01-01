latest_task=$(ls -t --directory sec*/sub*/* | head --lines=1)
cd $latest_task
