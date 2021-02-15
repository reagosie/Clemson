from collections import Counter

# initializing strings
cipherText = "bt jpx rmlx pcuv amlx icvjp ibtwxvr ci m lmt'r pmtn, mtn yvcjx cdxv mwmbtrj jpx amtngxrjbah uqctjpx qgmrjxv ci jpx ymgg ci jpx hbtw'r qmgmax; mtn jpx hbtw rmy jpx qmvj ci jpx pmtn jpmj yvcjx.jpxt jpx hbtw'r acutjxtmtax ymr apmtwxn, mtn pbr jpcuwpjr jvcufgxn pbl, rc jpmj jpx scbtjr ci pbrgcbtr yxvx gccrxn, mtn pbr htxxr rlcjx ctx mwmbtrj mtcjpxv. jpx hbtw avbxn mgcun jc fvbtw bt jpxmrjvcgcwxvr, jpx apmgnxmtr, mtn jpx rccjprmexvr. mtn jpx hbtw rqmhx, mtn rmbn jc jpx ybrx lxt ci fmfegct, ypcrcxdxv rpmgg vxmn jpbr yvbjbtw, mtn rpcy lx jpx btjxvqvxjmjbct jpxvxci, rpmgg fxagcjpxn ybjp ramvgxj, mtn pmdx m apmbt ci wcgn mfcuj pbr txah, mtn rpmgg fx jpx jpbvn vugxvbt jpx hbtwncl. jpxt amlx bt mgg jpx hbtw'r ybrx lxt; fuj jpxe acugn tcj vxmn jpx yvbjbtw, tcv lmhxhtcyt jc jpx hbtw jpx btjxvqvxjmjbct jpxvxci. jpxt ymr hbtw fxgrpmoomv wvxmjge jvcufgxn, mtnpbr acutjxtmtax ymr apmtwxn bt pbl, mtn pbr gcvnr yxvx mrjctbrpxn. tcy jpx kuxxt, fe vxmrct cijpx ycvnr ci jpx hbtw mtn pbr gcvnr, amlx btjc jpx fmtkuxj pcurx; mtn jpx kuxxt rqmhx mtn rmbn, chbtw, gbdx icvxdxv; gxj tcj jpe jpcuwpjr jvcufgx jpxx, tcv gxj jpe acutjxtmtax fx apmtwxn; jpxvx brm lmt bt jpe hbtwncl, bt ypcl br jpx rqbvbj ci jpx pcge wcnr; mtn bt jpx nmer ci jpe ybrncl ci jpxwcnr, ymr icutn bt pbl; ypcl jpx hbtw txfuapmntxoomv jpe imjpxv, jpx hbtw, b rme, jpe imjpxv,lmnx lmrjxv ci jpx lmwbabmtr, mrjvcgcwxvr, apmgnxmtr, mtn rccjprmexvr; icvmrluap mr mtxzaxggxtj rqbvbj, mtn htcygxnwx, mtn utnxvrjmtnbtw, btjxvqvxjbtw ci nvxmlr, mtn rpcybtw cipmvn rxtjxtaxr, mtn nbrrcgdbtw ci ncufjr,yxvx icutn bt jpx rmlx nmtbxg, ypcl jpx hbtw tmlxnfxgjxrpmoomv; tcy gxj nmtbxg fx amggxn, mtn px ybgg rpcy jpx btjxvqvxjmjbct."
hamletText = "to be, or not to be, that is the question: whether tis nobler in the mind to suffer the slings and arrows of outrageous fortune, or to take arms against a sea of troubles and by opposing end them. to die-to sleep, no more; and by a sleep to say we end the heart-ache and the thousand natural shocks that flesh is heir to: 'tis a consummation devoutly to be wish'd. to die, to sleep; to sleep, perchance to dream-ay, there's the rub: for in that sleep of death what dreams may come, when we have shuffled off this mortal coil, must give us pause-there's the respect that makes calamity of so long life. for who would bear the whips and scorns of time, th'oppressor's wrong, the proud man's contumely, the pangs of dispriz'd love, the law's delay, the insolence of office, and the spurns that patient merit of th'unworthy takes, when he himself might his quietus make with a bare bodkin? who would fardels bear, to grunt and sweat under a weary life, but that the dread of something after death, the undiscovere'd country, from whose bourn no traveller returns, puzzles the will, and makes us rather bear those ills we have than fly to others that we know not of? thus conscience does make cowards of us all, and thus the native hue of resolution is sicklied o'er with the pale cast of thought, and enterprises of great pitch and moment with this regard their currents turn awry and lose the name of action."
shylockText = "to bait fish withal. if it will feed nothing else, it will feed my revenge. he hath disgraced me and hindered me half a million, laughed at my losses, mocked at my gains, scorned my nation, thwarted my bargains, cooled my friends, heated mine enemies-and what's his reason? i am a jew. hath not a jew eyes? hath not a jew hands, organs, dimensions, senses, affections, passions? fed with the same food, hurt with the same weapons, subject to the same diseases, healed by the same means, warmed and cooled by the same winter and summer as a christian is? if you prick us, do we not bleed? if you tickle us, do we not laugh? if you poison us, do we not die? and if you wrong us, shall we not revenge? if we are like you in the rest, we will resemble you in that. if a jew wrong a christian, what is his humility? revenge. if a christian wrong a jew, what should his sufferance be by christian example? why, revenge. the villainy you teach me i will execute-and it shall go hard but i will better the instruction."

# PART B ###################################################################################################################
# run frequency analysis on Hamlet's speech and Shylock's speech
hamlet = Counter(hamletText)
hamlet = str(hamlet)
hamlet = hamlet[8:]
hamlet2 = Counter(hamletText[idx : idx + 2] for idx in range(len(hamletText) - 1))
hamlet3 = Counter(hamletText[idx : idx + 3] for idx in range(len(hamletText) - 1))
shylock = Counter(shylockText)
shylock = str(shylock)
shylock = shylock[8:]
shylock2 = Counter(shylockText[idx : idx + 2] for idx in range(len(shylockText) - 1))
shylock3 = Counter(shylockText[idx : idx + 3] for idx in range(len(shylockText) - 1))

# get only the first 30 elements from bigram and trigram counts
hamlet2 = str(hamlet2)
hamlet2 = hamlet2[8 : 307]
hamlet3 = str(hamlet3)
hamlet3 = hamlet3[8 : 324]
shylock2 = str(shylock2)
shylock2 = shylock2[8 : 307]
shylock3 = str(shylock3)
shylock3 = shylock3[8 : 324]

print("Part B: Frequency Analysis for famous Shakespearean speeches\n")
# print single letter frequencies for hamletText and shylockText
print("Single letter frequencies:")
print("Hamlet Speech: " + str(hamlet) + "\n")
print("Shylock Speech: " + str(shylock) + "\n\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n")

# print bigram frequencies for hamletText and shylockText
print("Top 30 bigram frequencies:")
print("Hamlet Speech: " + str(hamlet2) + "\n")
print("Shylock Speech: " + str(shylock2) + "\n\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n")

# print trigram frequencies for hamletText and shylockText
print("Top 30 trigram frequencies:")
print("Hamlet Speech: " + str(hamlet3) + "\n")
print("Shylock Speech: " + str(shylock3) + "\n\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n")

# PART C #########################################################################################################################
print("Part C: Comparison of the two above outputs\nOf the top 5 most frequent single letters in each of the Shakespearean speeches, 4 are the same in both (e, a, s, and t). Of the top 5 most infrequent single letters (including letters with a count of 0 which are not shown), again, 4 are the same in both (q, z, v, and k). Based on these similarities, I would say that frequency analysis performed in this way produces fairly accurate and consistent results. Of course there is variation because every text is different, but this variation diminishes the more text is being analyzed. I imagine if I were analyzing entire Shakespearean plays rather than excerpts, the results would be even more similar.\n\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n")

# PART D #########################################################################################################################
# gather counts of single letters, bigrams, and trigrams in cipherText
cipher = Counter(cipherText)
cipher = str(cipher)
cipher = cipher[8:]
cipher2 = Counter(cipherText[idx : idx + 2] for idx in range(len(cipherText) - 1))
cipher3 = Counter(cipherText[idx : idx + 3] for idx in range(len(cipherText) - 1))

# get only the first 30 elements from bigram and trigram counts
cipher2 = str(cipher2)
cipher2 = cipher2[8 : 307]
cipher3 = str(cipher3)
cipher3 = cipher3[8 : 324]

print("Part D: Frequency Analysis for Cipher Text and Decoded Cipher")
# print single letter frequencies for cipherText
print("Single letter frequencies:")
print("Cipher Text: " + str(cipher) + "\n")

# print bigram frequencies for cipherText
print("Top 30 bigram frequencies:")
print("Cipher Text: " + str(cipher2) + "\n")

# print trigram frequencies for cipherText
print("Top 30 trigram frequencies:")
print("Cipher Text: " + str(cipher3) + "\n")

def decoder(cipher):
    code = {
        "x": "e",
        "t": "n",
        "j": "t",
        "m": "a",
        "p": "h",
        "c": "o",
        "r": "s",
        "b": "i",
        "n": "d",
        "v": "r",
        "g": "l",
        "w": "g",
        "y": "w",
        "a": "c",
        "l": "m",
        "i": "f",
        "u": "u",
        "h": "k",
        "f": "b",
        "e": "y",
        "q": "p",
        "d": "v",
        "o": "z",
        "k": "q",
        "s": "j",
        "z": "x",
        " ": " ",
        ";": ";",
        ",": ",",
        "'": "'",
        ".": ".",
    }
    return code.get(cipher, "nothing")

decodedCipher = ""

for i in cipherText:
    decodedLetter = decoder(i)
    decodedCipher += decodedLetter

print("Decoded Cipher:")
print(decodedCipher)
