package main

import (
	"encoding/base64"
	"encoding/hex"
	"fmt"
	"io"
	"log"
	"net/http"
	"net/url"
	"os"
)

const remoteBase = "http://0.0.0.0:8080/submit?ciphertext="

func main() {
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
}
