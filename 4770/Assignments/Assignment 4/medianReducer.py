#!/software/spackages/linux-centos8-x86_64/gcc-8.3.1/anaconda3-2019.10-v5cuhr6keyz5ryxcwvv2jkzfj2gwrj4a/bin/python

import sys
import statistics
import json

current_genre = None

for line in sys.stdin:
    line = line.strip()
    genre, genreString = line.split("\t", 1)
    genreInfo = json.loads(genreString)

    current_genre_array = genreInfo["genre_array"]
    current_genre_array.sort();

    median = statistics.median(current_genre_array)
    print ("%s\t%s" % (genre, median))
