if __name__ == "__main__":
    path = "/Users/parker/Dropbox/Brown/Fall_2011/cs123/final/PAKPAK/resources/point_clouds/med_mbox"
    verts = []
    f = open(path, "r")
    while True:
        l = f.readline()
        if (not l): break
        l = l.strip("\n")
        parts = l.split(",")
        verts.append(map(float, parts))
    f.close()
    print verts