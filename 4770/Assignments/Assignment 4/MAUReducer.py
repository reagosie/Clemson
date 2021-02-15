#!/software/spackages/linux-centos8-x86_64/gcc-8.3.1/anaconda3-2019.10-v5cuhr6keyz5ryxcwvv2jkzfj2gwrj4a/bin/python

import sys
import csv
import json

userList = {}

for line in sys.stdin:
    line = line.strip()
    userInfo = json.loads(line)
    userId = userInfo["id"]
    userList[userId] = userInfo

max_count = 0
MAU = 0
for userId in userList:
    if int(userList[userId]["rating_count"]) > max_count:
        max_count = int(userList[userId]["rating_count"])
        MAU = userId

max_genre_count = 0
maxGenre = ''
genre = 'Comedy'
for genre in userList[MAU]:
    if genre != 'rating_count' and genre != 'id':
        if userList[MAU][genre] > max_genre_count:
            max_genre_count = userList[MAU][genre]
            maxGenre = genre

print("Most Active User: %s -- Total Rating Counts: %s -- Most Rated Genre: %s - %s" % (userList[MAU]["id"], userList[MAU]["rating_count"], maxGenre, max_genre_count))
