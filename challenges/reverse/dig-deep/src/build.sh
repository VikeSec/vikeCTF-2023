#!/bin/bash

set -e pipefail
# set -x

IMAGE_NAME="go-crackme" 

docker build --tag $IMAGE_NAME .
container_id=$(docker create $IMAGE_NAME)
docker cp $container_id:/app/challenge - > challenge
docker rm -v $container_id
tar -xf challenge

echo "Done! File is at ./challenge"