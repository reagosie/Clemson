#!/software/spackages/linux-centos8-x86_64/gcc-8.3.1/anaconda3-2019.10-v5cuhr6keyz5ryxcwvv2jkzfj2gwrj4a/bin/python

import sys
import csv
import json

movieFile = "./movies.csv"

movieList = {}
genreList = {}
userList = {}

with open(movieFile, mode = 'r') as infile:
    reader = csv.reader(infile)
    for row in reader:
        movieList[row[0]] = {}
        movieList[row[0]]["title"] = row[1]
        movieList[row[0]]["genre"] = row[2]

for oneMovie in sys.stdin:
    oneMovie = oneMovie.strip()
    ratingInfo = oneMovie.split(",")
    try:
        userId = int(ratingInfo[0])
        if userId not in userList:
            user = {
                "id": userId,
                "rating_count": 0,
            }
            userList[userId] = user

        genres = movieList[ratingInfo[1]]["genre"]
        rating = float(ratingInfo[2])

        for genre in genres.split("|"):
            if genre in userList[userId]:
                userList[userId]["rating_count"] += 1
                userList[userId][genre] += 1
            else:
                userList[userId]["rating_count"] += 1
                userList[userId][genre] = 1
    except ValueError:
        continue

for userId in userList:
    print ("%s" % (json.dumps(userList[userId]) ) )
