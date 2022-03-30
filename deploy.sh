#!/bin/bash

make nanvix
make image
bash tools/run/run.sh
