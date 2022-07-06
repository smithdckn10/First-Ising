import matplotlib.pyplot as plt
sweeps = 0
dataE = []
dataM = []
dataAvgE = []
dataAvgE2 = []
dataAvgM = []
dataAvgM2 = []
count = 0

##As of right now, you must use 20 different temperatures, mainly just because of line 16

with open('/Users/Smith/Desktop/UnderGradProject/MINE/Smith_Energies.txt') as in_file:
    lines = in_file.readlines()
    line = lines[0].split(",", -1)
    print("Size: ", line[0], " Sweeps: ", line[1], " Temps: ") ##This just takes the first line from the text file and pulls out the size, number of sweeps, and the temperatures the sweeps were performed at
    sweeps = int(line[1])
    count = (len(line) - 2)
    name = ["" for k in range(count)]
    for x in range(count):
        name[x] = "J = " + str(line[x+2])

    print("count: ",count)
    dataEA = [[0 for x in range(sweeps+1)] for y in range(count+1)] ##Array that stores the energies after each sweep, along with the "temperature" the sweep was performed at
    sweepArr = [x for x in range(1,sweeps+1)] ##Just an array to serve as the x component of the data

    del lines[0] ##Delete the first line in the text file, leaving just the data
    i = 0
    j = 1
    temp = ""
    check = False
    while i < count:
        temp = lines[sweeps*i].split(",",7)
        while j <= sweeps:
            if i == 18 and j == 500:
                ##print("temp: ", temp[0])
                pass

            line = lines[sweeps * i + (j-1)].split(",",7)
            ##print(line)
            dataEA[i][j] = (float(line[2]))
            ##print("dataEA[",i,"][",j,"]: ",dataEA[i][j],"  dataEA[500][18]: ",dataEA[18][500])
            
            ##print("lol")
            j+=1
        ##print("i: ",i)
        dataEA[i][0] = float(temp[0])
        j = 1
        i+=1

##print("dataEA[1]: ",dataEA[1])
##print("dataEA[18][500]: ",dataEA[18][500])

plt.figure(figsize=(10,8))
for i in range(count):
    del dataEA[i][0]
    plt.plot(sweepArr, dataEA[i], label = name[i])
plt.plot(sweepArr, dataEA[18])
##plt.plot(dataM)
plt.title('Comparing Energy Density For Varying Temperatures (J)')
plt.xlabel('Sweep')
plt.ylabel('Energy Density After Each Sweep[idk units]')
leg = plt.legend(ncol = 4) ##I did some digging on how to make the legend not so awful, but the only thing I was actually able to implement was changing the number of columns in the legend :(

plt.show()
