#!/bin/bash

#rm -rf venv

python='python3'
script_dir="$(cd "$(dirname "$0")" && pwd)"
venv_dir="$script_dir/venv"

if [ ! -d "$venv_dir" ]; then
    $python -m venv --system-site-packages "$venv_dir"

    $venv_dir/bin/python -m pip install pyserial
    $venv_dir/bin/python -m pip install pandas 


fi


source venv/bin/activate
python3 import_data.py 