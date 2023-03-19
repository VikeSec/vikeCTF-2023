#####################################################
# Helper functions                                  #
#####################################################

def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

def lcm(a, b):
    return (a * b) // gcd(a, b)

def modular_multiplicative_inverse(a, n):
    t = 0
    new_t = 1
    r = n
    new_r = a

    while new_r != 0:
        quotient = r // new_r
        t, new_t = new_t, t - quotient * new_t
        r, new_r = new_r, r - quotient * new_r
    
    if r > 1:
        raise Exception("inverse does not exist")
    if t < 0:
        t += n
    
    return t

def iroot(k, n):
    u, s = n, n + 1
    while u < s:
        s = u
        t = (k - 1) * s + n // pow(s, k - 1)
        u = t // k
    return s

def solve(n):
    p = iroot(3, n) | 1
    while n % p != 0:
        p -= 2

    n = n // p
    q = iroot(2, n) | 1
    while n % q != 0:
        q -= 2
    
    r = n // q

    return p, q, r

#####################################################
# Decryption                                        #
#####################################################

e = int(input("Enter exponent e: "))
n = int(input("Enter modulus n: "))
c = int(input("Enter ciphertext c: "))

p, q, r = solve(n)

λ_n = lcm(lcm(p - 1, q - 1), r - 1)
d = modular_multiplicative_inverse(e, λ_n)

m = pow(c, d, n)

flag = m.to_bytes((m.bit_length() + 7) // 8, byteorder="little").decode("utf8")
print(flag)
