#!/usr/bin/env bash

echo "Remember to run this with a memory limit to be safe."
for file in ../input/*; do
  echo "Working on $file..."
  ./player "../input/$file" >"../output/$file"
done
echo "Done."
