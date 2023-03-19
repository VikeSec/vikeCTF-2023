FROM golang:1.19-alpine

WORKDIR /app

COPY challenge.go go.mod go.sum flag.txt ./

RUN go mod download
# strip symbols, debug info, and paths
RUN go build -ldflags "-s -w" -trimpath -o challenge challenge.go 

CMD sh