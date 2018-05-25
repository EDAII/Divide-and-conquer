def karatsuba_multiplication(num1, num2):
    if num1 < 10 or num2 < 10:
        return num1*num2

    n = max(len(str(num1)), len(str(num2)))
    n2 = n//2

    high1, low1 = num1 // 10**n2, num1 % 10**n2
    high2, low2 = num2 // 10**n2, num2 % 10**n2

    z0 = karatsuba_multiplication(low1, low2)
    z1 = karatsuba_multiplication((low1 + high1), (low2 + high2))
    z2 = karatsuba_multiplication(high1, high2)

    return z2 * 10**(2 * n2) + (z1 - z2 - z0) * 10**n2 + z0


def main():
    number, number2 = map(int, input().split())
    print(karatsuba_multiplication(number, number2))


if __name__ == '__main__':
    main()
