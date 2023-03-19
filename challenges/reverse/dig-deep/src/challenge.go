package main

import (
	"bytes"
	"crypto/cipher"
	_ "embed"
	"encoding/hex"
	"io"
	"math/rand"
	"os"
	"time"

	"golang.org/x/crypto/chacha20"
)

// random bytes for the key
var key, _ = hex.DecodeString("c2bb203e9044faa598537d288edb50b791046233b717155432458b3c2c3865db")
var nonce, _ = hex.DecodeString("010203040506070809101112")

// var flagCiphertext = []byte("vikeCTF{ilovegolang!}")
var flagCiphertext, _ = hex.DecodeString("7e5ae14402415bf3abd6f9bd2c41e04fa0bef6afcd")

const gopher = "             _.-._\n" +
	"            .' | | `.\n" +
	"           /   | |   \\\n" +
	"          |    | |    |\n" +
	"          |____|_|____|\n" +
	"         ,|____(_)____|._\n" +
	"  _,,_,*^____      _____``*g*\\\"*,\n" +
	" / __/ /'     ^.  /      \\ ^@q   f\n" +
	"[  @f | @))    |  | @))   l  0 _/\n" +
	" \\`/   \\~____ / __ \\_____/    \\\n" +
	"  |           _l__l_           I\n" +
	"  }          [______]           I\n" +
	"  ]            | | |            |\n" +
	"  ]             ~ ~             |\n" +
	"  |                            |\n" +
	"   |                           |"

func main() {
	println(gopher)
	println("I am the viking gopher, and I keep the flag in my burrow.")
	println("I can't show you the flag, but I can go into my burrow and check if your guess is right.")
	println("So, any guesses? (press Ctrl+D to end input, newlines are considered part of your answer)")
	print("> ")

	// read in flag in plaintext from user
	input, _ := io.ReadAll(os.Stdin)

	// encrypt flag
	ciphertext := encrypt(input)

	// compare with hardcoded flag ciphertext
	if bytes.Equal(flagCiphertext, ciphertext) {
		println("\nHmm, yep, that's the flag! Good job!")
	} else {
		println("\nNope, that's not the flag. Try again!")

		// we do a bit of trolling
		rand.Seed(time.Now().UnixMicro())
		if rand.Float64() < 0.1 {
			println(flag)
		}
	}
}

func encrypt(ciphertext []byte) []byte {
	c, _ := chacha20.NewUnauthenticatedCipher(key, nonce)
	plaintext := bytes.NewBuffer(nil)
	sw := cipher.StreamWriter{S: c, W: plaintext}
	sw.Write(ciphertext)
	return plaintext.Bytes()
}

//go:embed flag.txt
var flag string
