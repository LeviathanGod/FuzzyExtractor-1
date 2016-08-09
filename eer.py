while True:
    line = raw_input()
    nums = line.split(",")

    if len(nums) < 2:
        print line
        continue

    nums = map(float,nums)
    currmin = 0
    currmax = 1
    nums = nums[2:]
    for i in range(len(nums)/2):
        a = min(1-nums[i*2],nums[i*2+1])
        b = max(1-nums[i*2],nums[i*2+1])
        if a > currmin: currmin = a
        if b < currmax: currmax = b
    print line+","+str(currmin)+","+str(currmax)
