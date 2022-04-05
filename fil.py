def minCut(A):
    if (len(A) == 1):
        return 0

    dc = dict()

    def isPal(st):
        return st == st[::-1]

    def helper(st):
        if st in dc:
            print("here")
            return dc[st]
        if len(st) == 0 or len(st) == 1:
            return 0
        if isPal(st):
            return 0
        else:
            cur_min = 500
            for i in range(1, len(st)):
                left_par = helper(st[0:i])
                right_par = helper(st[i:])
                cur_min = min(cur_min, left_par + right_par + 1)
            dc[st] = cur_min
            return cur_min

    return helper(A)

a = "apjesgpsxoeiokmqmfgvjslcjukbqxpsobyhjpbgdfruqdkeiszrlmtwgfxyfostpqczidfljwfbbrflkgdvtytbgqalguewnhvvmcgxboycffopmtmhtfizxkmeftcucxpobxmelmjtuzigsxnncxpaibgpuijwhankxbplpyejxmrrjgeoevqozwdtgospohznkoyzocjlracchjqnggbfeebmuvbicbvmpuleywrpzwsihivnrwtxcukwplgtobhgxukwrdlszfaiqxwjvrgxnsveedxseeyeykarqnjrtlaliyudpacctzizcftjlunlgnfwcqqxcqikocqffsjyurzwysfjmswvhbrmshjuzsgpwyubtfbnwajuvrfhlccvfwhxfqthkcwhatktymgxostjlztwdxritygbrbibdgkezvzajizxasjnrcjwzdfvdnwwqeyumkamhzoqhnqjfzwzbixclcxqrtniznemxeahfozp"

print(minCut(a))

print(a[0:2])
print(a[2:4])