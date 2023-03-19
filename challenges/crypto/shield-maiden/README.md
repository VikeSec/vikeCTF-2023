# Shield Maiden's Secret

**Author: [`Malcolm Seyd`](https://github.com/malcolmseyd)**

**Category: `Crypto Hard`**

## Description

The shield maiden has divine sight; she can read the texts that nobody else can. She gives you the flag, but it is in its divine, unreadable format. She will tell you whether any divine text is readable. Ask her some questions, and she will reveal unearthly wisdom.

> http://shield-maiden.vikesec.ca

## Organizers

Users should receive a copy of `shieldmaidens.zip`, and the docker container needs to be running containing the below binary.

To run the challenge:

```console
$ go build -o challenge .
$ FLAG='vikeCTF{here_is_my_flag}' ./challenge
```

## Solution

This challenge utilizes the padding oracle attack with a user-controlled IV to decrypt an arbitrary ciphertext (the flag).

From looking at `crypto.go`, we can see that the flag is encrypted with AES-CBC with a random nonce prepended. It uses PKCS-7 padding, and if the padding is invalid, it will throw an error and fail the decryption. The key here is that padding errors are displayed to the user, so they can differentiate what valid and invalid padding is. This is called a Padding Oracle attack, and is a well-known attack in cryptography. You can read more about it here if you're curious: https://robertheaton.com/2013/07/29/padding-oracle-attack/

Luckily for us, since it's a well known problem, there are many pre-made programs to solve this problem. We'll use one called [rustpad](https://github.com/Kibouo/rustpad).

Rustpad only supports a few encodings, so we'll need to write a script that translates the ciphertext to something that rustpad can recognize. The challenge uses the URL-safe base64 encoding that ships with Go, so we'll translate that to hex so that rustpad can understand it.

We can see that if a decryption error occurs we get an HTTP 400, but if the ciphertext is wrong and has valid padding we get an HTTP 200. We'll write our script to return an exit code of 1 if decryption errors, and report a success otherwise. Here's the code I settled on:

```go
const remoteBase = "http://0.0.0.0:8080/submit?ciphertext="

ciphertextEncoded := os.Args[1]
ciphertextDecoded, _ := hex.DecodeString(ciphertextEncoded)
ciphertext := base64.URLEncoding.EncodeToString(ciphertextDecoded)

resp, err := http.Get(remoteBase + url.QueryEscape(ciphertext))
if err != nil {
    log.Fatalln("get request failed:", err)
}

b, err := io.ReadAll(resp.Body)
fmt.Println(string(b))
if err != nil || resp.StatusCode == http.StatusBadRequest {
    os.Exit(1)
}
```

All we need to do now is pipe rustpad into this, and it'll work. Finally, we need to translate the key from URL-safe base64 into hex using [this Cyberchef recipe](<https://gchq.github.io/CyberChef/#recipe=From_Base64('A-Za-z0-9-_',true,false)To_Hex('None',0)>) and run rustpad.

```console
$ rustpad --script --oracle './rustpadscript' --block-size 16 --decrypt 'f3c06c9461b4b44961262dc6d5476b9e86f94ca1a396fd81b534b0e44f56091652060feead9a128e149d7c806f7e0f78' --no-cache
```

And voilà, we have the flag!

## Flag

```
vikeCTF{7h15_fl46_15_b1663r_7h4n_0n3_bl0ck}
```
