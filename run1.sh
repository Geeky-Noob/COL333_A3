#!/bin/bash

# Check if the correct number of command line arguments is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <input_filename>"
    exit 1
fi

input_prefix="$1"
input_filename="$input_prefix.graph"
satinput_filename="$input_prefix.satinput"

# Check if the input file exists
if [ ! -f "$input_filename" ]; then
    echo "Input file '$input_filename' not found."
    exit 1
fi

# generate SAT input file
g++ main.cpp MiniSAT.cpp -o SAT_inputfile_generator
./SAT_inputfile_generator $input_filename $satinput_filename

echo "SAT input file '$satinput_filename' generated."
