from random import randbytes, randint

#####################################################
# Helper functions                                  #
#####################################################

def is_prime(n):
    if n == 2 or n == 3:
        return True

    if n < 2 or n % 2 == 0:
        return False
    
    bitsize = n.bit_length()
    if bitsize >= 1536:
        k = 3
    elif bitsize >= 1024:
        k = 4
    elif bitsize >= 512:
        k = 7
    else:
        k = 10
    
    s, d = 0, n - 1
    while d % 2 == 0:
        s, d = s + 1, d // 2

    for _ in range(k):
        a = randint(2, n - 2)
        x = pow(a, d, n)
        
        if x == 1 or x == n - 1:
            continue

        for _ in range(s - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    
    return True

def generate_primes(bits):
    seed = int.from_bytes(randbytes(bits // 8), byteorder="little")
    p = (seed + randint(-10e5, 10e5)) | 1
    while not is_prime(p):
        p += 2
    
    q = (seed + randint(-10e5, 10e5)) | 1
    while not is_prime(q):
        q += 2
    
    r = (seed + randint(-10e5, 10e5)) | 1
    while not is_prime(r):
        r += 2
    
    return p, q, r

def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return a

def lcm(a, b):
    return (a * b) // gcd(a, b)

#####################################################
# Encryption                                        #
#####################################################

flag = input("Enter flag: ").encode("utf8")
p, q, r = generate_primes(1024)
e = 65537
n = p*q*r
λ_n = lcm(lcm(p - 1, q - 1), r - 1)

if not 1 < e < λ_n or gcd(e, λ_n) != 1:
    print("try again")
    exit()

m = int.from_bytes(flag, byteorder="little")

c = pow(m, e, n)

print("e:", e)
print("n:", n)
print("c:", c)
