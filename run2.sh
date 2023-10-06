#!/bin/bash

# Check if the correct number of command line arguments is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <input_prefix>"
    exit 1
fi

# Input prefix (e.g., "test")
input_prefix="$1"
graph_filename="$input_prefix.graph"
satoutput_filename="$input_prefix.satoutput"
mapping_filename="$input_prefix.mapping"

# Check if the SAT output file and graph file exist
if [ ! -f "$satoutput_filename" ] || [ ! -f "$graph_filename" ]; then
    echo "SAT output file '$satoutput_filename' or '$graph_filename' not found."
    exit 1
fi

# generate the mapping file
g++ mapping.cpp -o subgraph_generator
./subgraph_generator $graph_filename $satoutput_filename $mapping_filename

echo "Mapping file '$mapping_filename' generated."
