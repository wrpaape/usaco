#!/bin/bash -e

task=$1
if [ -z "$task" ] || [ $task == -h ] || [ $task == --help ]
then
    echo "$0 <task> [<section> <subsection>]"
    exit
fi

section=$2
subsection=$3
if [ -n "$section" ] && [ -n "$subsection" ]
then
    task_dirname=sec${section}/sub${subsection}
    mkdir --parents $task_dirname
else
    task_dirname=$(echo sec*/sub* | xargs --max-args=1 | sort | tail --lines=1)
fi
path_to_task=$task_dirname/$task

cp --recursive templatetask $path_to_task
for file in $path_to_task/templatetask*
do
    mv $file ${file//templatetask/$task}
done
sed --in-place --expression="s/templatetask/$task/g" $path_to_task/*
