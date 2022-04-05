def divide(dividend: int, divisor: int) -> int:
    cnt = 0
    should_put_neg = False

    if (dividend < 0 and divisor < 0):
        dividend = - dividend
        divisor = - divisor
    elif dividend < 0:
        dividend = - dividend
        should_put_neg = True
    elif divisor < 0:
        divisor = - divisor
        should_put_neg = True

    step = divisor

    while (dividend - divisor >= 0):
        cnt += 1
        divisor += step

    if should_put_neg:
        return -cnt
    else:
        return cnt

print(divide(1, 1))
