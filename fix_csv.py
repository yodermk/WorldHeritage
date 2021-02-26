fi = open("whc-sites-2019.csv")
fo = open("whc-sites.csv", "w")

last = ""
for line in fi.readlines():
    if not (line.startswith("Cultural") or line.startswith("Natural") or line.startswith("Mixed")):
        last = last.rstrip()

    fo.write(last)

    last = line

fo.write(last)
fo.close()
fi.close()
