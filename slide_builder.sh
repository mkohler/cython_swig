#!/bin/sh
while :; do
    inotifywait -qq README.rst
    make slides
done
