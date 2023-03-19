package main

import (
	_ "embed"
	"log"
	"net/http"
)

const listenAddr = "0.0.0.0:8080"

func main() {
	http.Handle("/", FlagHandler())

	log.Printf("Listening on http://%v/\n", listenAddr)
	log.Fatalln(http.ListenAndServe(listenAddr, http.DefaultServeMux))
}
