package main

import (
	"crypto/cipher"
	"encoding/hex"
	"os"

	"golang.org/x/crypto/chacha20"
)

func main() {
	key, _ := hex.DecodeString("c2bb203e9044faa598537d288edb50b791046233b717155432458b3c2c3865db")
	nonce, _ := hex.DecodeString("010203040506070809101112")
	ciphertext, _ := hex.DecodeString("7e5ae14402415bf3abd6f9bd2c41e04fa0bef6afcd")

	c, _ := chacha20.NewUnauthenticatedCipher(key, nonce)
	sw := cipher.StreamWriter{S: c, W: os.Stdout}
	sw.Write(ciphertext)
}
