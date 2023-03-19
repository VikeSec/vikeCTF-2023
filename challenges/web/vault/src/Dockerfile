## Build image
FROM node:lts-alpine as build
WORKDIR /app

COPY package*.json ./
RUN npm install --ignore-scripts --no-audit --no-progress
RUN npm explore better-sqlite3 -- npm install --no-audit --no-progress

COPY . .
RUN npm run build

## Runtime image
FROM node:lts-alpine as runtime
WORKDIR /app
ENV HOST=0.0.0.0
ENV PORT=3000
EXPOSE 3000

COPY users.db ./
RUN chmod a-w users.db

COPY package*.json ./
RUN npm install --ignore-scripts --no-audit --no-progress
# All of the sqlite packages are broken. At least this one runs
RUN npm explore better-sqlite3 -- npm install --no-audit --no-progress

COPY --from=build /app/dist /app/dist/

CMD ["/usr/local/bin/node", "/app/dist/server/entry.mjs"]
