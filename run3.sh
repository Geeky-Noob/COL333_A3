#!/bin/bash

# Check if the correct number of command line arguments is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <input_prefix>"
    exit 1
fi

# Input prefix (e.g., "test")
input_prefix="$1"
graph_filename="$input_prefix.graph"
satinput_filename="$input_prefix.satinput"
satoutput_filename="$input_prefix.satoutput"
mapping_filename="$input_prefix.mapping"

# Check if the SAT output file and graph file exist
# if [ ! -f "$satoutput_filename" ] || [ ! -f "$graph_filename" ]; then
#     echo "SAT output file '$satoutput_filename' or '$graph_filename' not found."
#     exit 1
# fi

if [ -e "$graph_filename" ]; then
    # Read the first number from the file
    vertices=$(head -n 1 "$graph_filename" | awk '{print $1}')
    echo "The first number is: $vertices"
else
    echo "File 'test.graph' does not exist."
fi

low=1
high=$vertices
echo "Vertices: $vertices"
g++ problem2.cpp MiniSAT2.cpp -o subgraph_generator2

g++ mapping.cpp -o mapping_generator

k=1
while [ $low -le $high ]; do
    mid=$((($low + $high) / 2))
    ./subgraph_generator2 $graph_filename $satinput_filename $mid
    minisat $satinput_filename $satoutput_filename
    echo "Mid: $mid"

    if [ -e "$satoutput_filename" ]; then
        read -r sat < "$satoutput_filename"

        if [ "$sat" == "SAT" ]; then
            if [ $k -lt $mid ]; then
                k=$mid
                ./mapping_generator $graph_filename $satoutput_filename $mapping_filename 2
            fi
            low=$(($mid + 1))
            
        elif [ "$sat" == "UNSAT" ]; then
            high=$(($mid - 1))
        fi
    else
        echo "File 'test.satoutput' does not exist."
    fi
done

echo "Final k: $k" 