#!/bin/bash

DIR="$(dirname "$(readlink -f "$0")")"

python ${DIR}/ingest.py

onprem --port 8000