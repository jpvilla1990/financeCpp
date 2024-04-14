# Use a base image
FROM ubuntu:latest

# Install dependencies
RUN apt-get update && apt-get install -y \
     g++ \
     libcurl4-openssl-dev \
     libmysqlcppconn-dev \
     libboost-all-dev

# Set working directory
WORKDIR /app

# Copy the binary to the container
COPY /cppBackend/bin/collector /app/collector

# Run the binary
CMD ["./collector"]
