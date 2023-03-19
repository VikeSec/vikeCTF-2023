alphabet = "abcdefghijklmnopqrstuvwxyz"
prefix = "vikeCTF"

ciphertext = input("text to decrypt:\n")

def decrypt(text, key):
	plaintext = ""
	for i, c in enumerate(text):
		if not c.isalpha():
			plaintext += c
			continue

		offset = alphabet.find(c.lower())
		rotation = key[i % len(key)]

		result = alphabet[(offset + rotation) % len(alphabet)]
		if c.islower():
			plaintext += result
		else:
			plaintext += result.upper()

	return plaintext

key = []
while len(key) < len("vikeCTF"):
	for i in range(27):
		if i == 26:
			print("no solution :(")
			exit(1)
		key.append(i)
		plaintext = decrypt(ciphertext[:len(key)], key)
		if prefix.startswith(plaintext):
			break
		key.pop()
		
print("key:", *map(lambda x: (26-x), key))
print("plaintext:")
print(decrypt(ciphertext, key))
	
