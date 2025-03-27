# Use a specific version tag for better reproducibility
FROM ubuntu:22.04 

# Group RUN commands to reduce layers
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    make \
    gcc \
    libsdl2-dev \
    libsdl2-ttf-dev \
    libsdl2-image-dev \
    libsdl2-mixer-dev \
    check \
    valgrind \
    lcov \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy only what's needed for building dependencies first
COPY Makefile .
COPY include/ include/
COPY lib/ lib/

# Set environment variables together
ENV LANG=C.UTF-8 \
    LC_ALL=C.UTF-8

# Copy source code (changes more frequently)
COPY src/ src/
COPY test/ test/
COPY assets/ assets/

# Build the application
RUN make

# Define default command
CMD ["./bin/main"]