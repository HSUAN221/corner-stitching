#!/bin/bash

if command -v lcov &> /dev/null ; then
    if [ $# -eq 0 ]
    then
        search_dir=$PWD
        output_dir=$PWD
    else
        search_dir=$1
        output_dir=$2
    fi
    echo "Search directory is $search_dir"
    echo "Output directory is $output_dir"
    lcov --no-external -b $search_dir -d . -c -i -o $output_dir/coverage_initial.info
    lcov --no-external -b $search_dir -d . -c -o $output_dir/coverage_tests.info 
    if [ -s $output_dir/coverage_initial.info ] && [ -s $output_dir/coverage_tests.info ]; then
        lcov -a $output_dir/coverage_initial.info -a $output_dir/coverage_tests.info \
             -o $output_dir/coverage.info
        lcov -r $output_dir/coverage.info \
         '*.cpp' \
         '*/examples/*' \
         '*/tests/*' \
         -o $output_dir/coverage.info
        lcov --list $output_dir/coverage.info
    else
        echo "coverage_initial.info or coverage_tests.info without contents or generating error!"
    fi
else
    echo "You should install lcov at first then calculate coverage!"
fi
