#!/software/spackages/linux-centos8-x86_64/gcc-8.3.1/anaconda3-2019.10-v5cuhr6keyz5ryxcwvv2jkzfj2gwrj4a/bin/python

import sys
import csv
import json
import statistics

current_genre = None
current_rating_sum = 0
current_rating_count = 0
current_stdev = 0

for line in sys.stdin:
    line = line.strip()
    genre, ratingString = line.split("\t", 1)
    ratingInfo = json.loads(ratingString)

    if current_genre == genre:
        try:
            current_rating_count += ratingInfo["total_count"]
        except ValueError:
            continue
    else:
        if current_genre:
            current_stdev = statistics.stdev(ratingInfo["genre_array"])
            print ("%s\t%s" % (current_genre, current_stdev))
        current_genre = genre
        try:
            current_rating_count = ratingInfo["total_count"]
        except ValueError:
            continue
