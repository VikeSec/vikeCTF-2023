## Build image
FROM node:lts-alpine as build
WORKDIR /app

COPY package*.json ./
RUN npm install --ignore-scripts --no-audit --no-progress

COPY . .
RUN npm run build

## Runtime image
FROM node:lts-alpine as runtime
WORKDIR /app
ENV HOST=0.0.0.0
ENV PORT=3000
EXPOSE 3000

RUN apk --no-cache --update add supervisor curl iptables ip6tables && \
    rm /etc/supervisord.conf && \
    mkdir -p /etc/supervisor/conf.d && \
    mkdir /var/log/supervisor && \
    rm -rf /var/cache/apk/*

RUN curl -o /usr/local/bin/ec2-metadata-mock -L https://github.com/aws/amazon-ec2-metadata-mock/releases/download/v1.11.2/ec2-metadata-mock-linux-amd64 && \
    chmod +x /usr/local/bin/ec2-metadata-mock

COPY supervisord.conf /etc/supervisor/conf.d/supervisord.conf
COPY ec2mock.json /app/

COPY package*.json ./
RUN npm install --ignore-scripts --no-audit --no-progress

COPY --from=build /app/dist /app/dist/

CMD ["/bin/ash", "-c", \
    "iptables -t nat -A OUTPUT -p tcp -d 169.254.169.254 --dport 80 -j DNAT --to-destination 127.0.0.1:1338 && \
    /usr/bin/supervisord -c /etc/supervisor/conf.d/supervisord.conf"]
