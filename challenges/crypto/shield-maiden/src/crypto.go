package main

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"crypto/subtle"
	"errors"
)

const keySize = 32 // AES-256
const ivSize = aes.BlockSize

var (
	ErrPadding   = errors.New("invalid PKCS#7 padding on plaintext")
	ErrBlockSize = errors.New("cannot unpad plaintext with invalid block size")
	ErrKeySize   = errors.New("key must be 32 bytes for AES-256")
	ErrIVSize    = errors.New("IV must be 16 bytes for AES")
	ErrIVPrepend = errors.New("the IV must be prepended to the ciphertext in RandomIV mode")
)

var fixedIV = generateIV()

func Compare(a, b []byte) bool {
	return subtle.ConstantTimeCompare(a, b) == 1
}

func EncryptRandomIV(plaintext, key []byte) ([]byte, error) {
	iv := generateIV()
	ciphertext, err := aesCBCEncrypt(plaintext, key, iv)
	if err != nil {
		return nil, err
	}
	return append(iv, ciphertext...), nil
}

func DecryptRandomIV(ciphertext, key []byte) ([]byte, error) {
	if len(ciphertext) < ivSize+aes.BlockSize {
		return nil, ErrIVPrepend
	}
	iv := ciphertext[:ivSize]
	ciphertext = ciphertext[ivSize:]
	return aesCBCDecrypt(ciphertext, key, iv)
}

func EncryptFixedIV(plaintext, key []byte) ([]byte, error) {
	return aesCBCEncrypt(plaintext, key, fixedIV)
}

func DecryptFixedIV(ciphertext, key []byte) ([]byte, error) {
	return aesCBCDecrypt(ciphertext, key, fixedIV)
}

func GenerateKey() []byte {
	return randomBytes(keySize)
}

func generateIV() []byte {
	return randomBytes(ivSize)
}

func randomBytes(length uint) []byte {
	b := make([]byte, length)
	rand.Read(b)
	return b
}

func aesCBCEncrypt(plaintext, key, iv []byte) ([]byte, error) {
	if len(key) != 32 {
		return nil, ErrKeySize
	}
	if len(iv) != aes.BlockSize {
		return nil, ErrBlockSize
	}

	c, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	bc := cipher.NewCBCEncrypter(c, iv)
	padded := pkcs7Pad(plaintext)
	ciphertext := make([]byte, len(padded))

	bc.CryptBlocks(ciphertext, padded)
	return ciphertext, nil
}

func aesCBCDecrypt(ciphertext, key, iv []byte) ([]byte, error) {
	if len(key) != 32 {
		return nil, ErrKeySize
	}
	if len(iv) != aes.BlockSize {
		return nil, ErrBlockSize
	}

	c, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	bc := cipher.NewCBCDecrypter(c, iv)
	padded := make([]byte, len(ciphertext))
	bc.CryptBlocks(padded, ciphertext)

	plaintext, err := pkcs7Unpad(padded)
	if err != nil {
		return nil, err
	}
	return plaintext, nil
}

// https://datatracker.ietf.org/doc/html/rfc5652#section-6.3
func pkcs7Pad(b []byte) []byte {
	padding := aes.BlockSize - (len(b) % aes.BlockSize)
	for i := 0; i < padding; i++ {
		b = append(b, byte(padding))
	}
	return b
}

func pkcs7Unpad(b []byte) ([]byte, error) {
	if len(b)%aes.BlockSize != 0 || len(b) == 0 {
		return nil, ErrBlockSize
	}

	// last byte tells us how much padding there is
	padding := int(b[len(b)-1])
	if len(b) < padding || padding == 0 {
		return nil, ErrPadding
	}

	// check if padding is valid
	for i := 0; i < padding; i++ {
		if b[len(b)-1-i] != byte(padding) {
			return nil, ErrPadding
		}
	}
	return b[:len(b)-padding], nil
}
