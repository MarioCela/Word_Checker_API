#!/bin/bash

# Get the input file and expected output file from command line arguments
input_file=$1
expected_output_file=$2

# Compile the C file
/usr/bin/gcc -DEVAL -Wall -Werror -std=gnu11 -O2 -pipe -static -s -o main main.c -lm

# Run the program with input from input_file
./main < $input_file > output.txt

# Compare the output with expected_output_file and store the result in a variable
diff_result=$(diff output.txt $expected_output_file)

# Check if the output matches the expected results
if [ -z "$diff_result" ]; then
    echo "Output matches expected results"
else
    echo "Output does not match expected results"
    echo "$diff_result" > differences.txt
fi

