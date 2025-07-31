# Use a minimal Ubuntu image
FROM ubuntu:22.04

# Avoid prompts when installing packages
ENV DEBIAN_FRONTEND=noninteractive

# Install system dependencies required to build and run
RUN apt-get update && apt-get install -y \
    build-essential cmake libfftw3-dev libspdlog-dev libfmt-dev \
    nlohmann-json3-dev git wget

# Set working directory inside the container
WORKDIR /app

# Copy entire project into container
COPY . .

# Build your C++ project
RUN mkdir -p build && cd build && cmake .. && make -j4

# Set the default config path
ENV CONFIG_PATH=/app/configs/pipeline.json

# Start the CLI application
CMD ["./build/pipeline_runner", "--config", "/app/configs/pipeline.json"]

