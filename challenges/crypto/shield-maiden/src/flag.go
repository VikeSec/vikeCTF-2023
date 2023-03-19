package main

import (
	"crypto/subtle"
	_ "embed"
	"encoding/base64"
	"io"
	"log"
	"net/http"
	"os"
	"text/template"
)

func mustEnv(key string) string {
	value, ok := os.LookupEnv(key)
	if !ok {
		return "vikeCTF{7h15_fl46_15_b1663r_7h4n_0n3_bl0ck}"
		// log.Fatalln("Fatal: envvar", key, "is missing!")
	}
	return value
}

var flag = []byte(mustEnv("FLAG"))

//go:embed views/flag.tmpl.html
var flagTemplateRaw string
var flagTemplate = template.Must(template.New("flag").Parse(flagTemplateRaw))

//go:embed views/correct.html
var correctHTML string

//go:embed views/incorrect.html
var incorrectHTML string

//go:embed views/error.tmpl.html
var errorTemplateRaw string
var errorTemplate = template.Must(template.New("error").Parse(errorTemplateRaw))

func FlagHandler() http.Handler {
	key := GenerateKey()
	ciphertext, err := EncryptRandomIV(flag, key)
	if err != nil {
		log.Fatalln("The code is broken, failed to encrypt ciphertext for flag:", err)
	}

	mux := http.NewServeMux()

	mux.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		w.Header().Add("Content-Type", "text/html; charset=utf-8")
		flagTemplate.Execute(w, base64.URLEncoding.EncodeToString(ciphertext))
		// flagTemplate.Execute(w, hex.EncodeToString(ciphertext))
	})

	mux.HandleFunc("/submit", func(w http.ResponseWriter, r *http.Request) {
		w.Header().Add("Content-Type", "text/html; charset=utf-8")

		value := r.URL.Query().Get("ciphertext")
		if value == "" {
			w.WriteHeader(http.StatusBadRequest)
			errorTemplate.Execute(w, "ciphertext is a required parameter")
			return
		}

		valueBytes, err := base64.URLEncoding.DecodeString(value)
		// valueBytes, err := hex.DecodeString(value)
		if err != nil {
			w.WriteHeader(http.StatusBadRequest)
			errorTemplate.Execute(w, "couldn't decode base64")
			return
		}

		plaintext, err := DecryptRandomIV(valueBytes, key)
		if err != nil {
			w.WriteHeader(http.StatusBadRequest)
			errorTemplate.Execute(w, "failed to decrypt ciphertext: "+err.Error())
			return
		}

		if subtle.ConstantTimeCompare(plaintext, flag) == 1 {
			io.WriteString(w, correctHTML)
			return
		} else {
			io.WriteString(w, incorrectHTML)
			return
		}
	})

	return mux
}
