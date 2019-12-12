# Set the base image.
FROM mhart/alpine-node:latest

# Create a directory to store the app in.
RUN mkdir /app
WORKDIR /app

# Install node dependencies.
COPY package.json /app/package.json
COPY yarn.lock /app/yarn.lock
RUN yarn install

# Environment variables.
ENV DATABASE database.sqlite
ENV PORT 8080

# Make the application accessible out of the container.
EXPOSE $PORT

# Copy the rest of the application files.
COPY . /app/

# Start command.
CMD yarn start