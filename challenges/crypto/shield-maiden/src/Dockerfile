## Builder image
FROM golang:alpine as builder
WORKDIR /go/src/app

COPY . .

# Static build required so that we can safely copy the binary over.
# `-tags timetzdata` embeds zone info from the "time/tzdata" package.
RUN CGO_ENABLED=0 go install -ldflags '-extldflags "-static"' -tags timetzdata


## Runtime image
FROM scratch

COPY --from=builder /go/bin/ctf /ctf

# Pull TLS certs directly from the upstream image
COPY --from=alpine:latest /etc/ssl/certs/ca-certificates.crt /etc/ssl/certs/
ENTRYPOINT ["/ctf"]