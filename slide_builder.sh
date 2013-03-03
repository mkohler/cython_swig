#!/bin/sh
while :; do
    inotifywait -e modify -qq README.rst
    make slides
done
