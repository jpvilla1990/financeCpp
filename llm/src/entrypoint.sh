#!/bin/bash

DIR="$(dirname "$(readlink -f "$0")")"

#python3 ${DIR}/ingest.py

uvicorn main:app --host 0.0.0.0 --port 8000