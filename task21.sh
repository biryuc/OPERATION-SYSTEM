#!/bin/bash

n=0

trap 'echo $i; exit' SIGQUIT
trap '((i++)); echo -e "\a"' SIGINT

while true; do
  sleep 100
done

