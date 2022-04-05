def threeSum(nums):
    def solve_two(nums, target):
        dc = dict()
        n = len(nums)
        possible = []
        for i in range(n):
            if (target - nums[i] in dc) and (i != dc[target - nums[i]]):
                possible.append([nums[i], target - nums[i]])
            if nums[i] not in dc:
                dc[nums[i]] = i
        return possible

    all_sols = []
    to_ret = []
    for num in nums:
        temp = - num
        cop = nums.copy()
        cop.remove(num)
        temp_lst = solve_two(cop, temp)
        if len(temp_lst) > 0:
            for l in temp_lst:
                l.append(num)
                if set(l) not in all_sols:
                    all_sols.append(set(l))
                    to_ret.append(l)
    return to_ret


inp = [-1,0,1,2,-1,-4]

print(threeSum(inp))